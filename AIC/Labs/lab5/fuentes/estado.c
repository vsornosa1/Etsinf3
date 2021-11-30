/*
 * A hash table to maintain the previous state
 */

#define _XOPEN_SOURCE 500 /* Enable certain library functions (strdup) on linux.  See feature_test_macros(7) */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "estado.h"

/* Create a new table_state. */
table_state_t *ht_create(int size) {

    table_state_t *table_state = NULL;
    int i;

    if (size < 1) return NULL;

    /* Allocate the table itself. */
    if ((table_state = malloc(sizeof(table_state_t))) == NULL) {
        return NULL;
    }

    /* Allocate pointers to the head nodes. */
    if ((table_state->table = malloc(sizeof(table_value_t *) * size)) == NULL) {
        return NULL;
    }

    for (i = 0; i < size; i++) {
        table_state->table[i] = NULL;
    }

    table_state->size = size;

    return table_state;
}

/* Hash a string for a particular hash table. */
int ht_hash(table_state_t *table_state, char *key) {

    unsigned long int hashval = 0;
    int i = 0;

    /* Convert our string to an integer */
    while (hashval < ULONG_MAX && i < strlen(key)) {
        hashval = hashval << 8;
        hashval += key[i];
        i++;
    }

    return (int) (hashval % table_state->size);
}

/* Create a key-value pair. */
table_value_t *ht_newpair(char *key, char *text, char *class) {
    table_value_t *newpair;

    if ((newpair = malloc(sizeof(table_value_t))) == NULL) {
        return NULL;
    }

    if ((newpair->key = strdup(key)) == NULL) {
        return NULL;
    }

    if ((newpair->text = strdup(text)) == NULL) {
        return NULL;
    }

    if (class == NULL) {
        newpair->class = NULL;
    } else {
        if ((newpair->class = strdup(class)) == NULL) {
            return NULL;
        }
    }

    newpair->next = NULL;

    return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set(table_state_t *table_state, char *key, char *text, char *class) {
    int bin = 0;
    table_value_t *newpair = NULL;
    table_value_t *next = NULL;
    table_value_t *last = NULL;

    bin = ht_hash(table_state, key);

    next = table_state->table[bin];
    while (next != NULL && next->key != NULL && strcmp(key, next->key) > 0) {
        last = next;
        next = next->next;
    }

    /* There's already a pair.  Let's replace the contents */
    if (next != NULL && next->key != NULL && strcmp(key, next->key) == 0) {

        free(next->text);
        next->text = strdup(text);

        if (next->class != NULL) {
            free(next->class);
        }
        if (class == NULL || strlen(class) == 0) {
            next->class = NULL;
        } else {
            next->class = strdup(class);
        }

        /* Nope, could't find it.  Time to grow a pair. */
    } else {
        newpair = ht_newpair(key, text, class);

        /* We're at the start of the linked list in this bin. */
        if (next == table_state->table[bin]) {
            newpair->next = next;
            table_state->table[bin] = newpair;

            /* We're at the end of the linked list in this bin. */
        } else if (next == NULL) {
            last->next = newpair;

            /* We're in the middle of the list. */
        } else {
            newpair->next = next;
            last->next = newpair;
        }
    }
}

/* Retrieve a key-value pair from a hash table. */
table_value_t *ht_get(table_state_t *table_state, char *key) {
    int bin = 0;
    table_value_t *pair;

    bin = ht_hash(table_state, key);

    /* Step through the bin, looking for our value. */
    pair = table_state->table[bin];
    while (pair != NULL && pair->key != NULL && strcmp(key, pair->key) > 0) {
        pair = pair->next;
    }

    /* Did we actually find anything? */
    if (pair == NULL || pair->key == NULL || strcmp(key, pair->key) != 0) {
        return NULL;
    } else {
        return pair;
    }

}
