//
// Created by ssaez on 5/12/19.
//

#ifndef MIPS_OOO_ESTADO_H
#define MIPS_OOO_ESTADO_H


typedef struct table_value_s {
    char *key;
    char *text;
    char *class;
    struct table_value_s *next;
} table_value_t;

typedef struct table_state_s {
    int size;
    table_value_t **table;
} table_state_t;


#ifndef CPROTO

#include "estado_api.h"

#endif


#endif //MIPS_OOO_ESTADO_H
