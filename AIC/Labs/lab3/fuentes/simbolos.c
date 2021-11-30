/**************************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *
 * File: simbolos.c
 *
 * Description:
 *      Contiene las funciones para manejo de la tabla de simbolos
 *
 *************************************************************************/

#define simbolos_C

/***** Includes ***********************************************/


#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#include "memoria.h"
#include "simbolos.h"

/***** Definiciones externas **********************************/

extern int yyerror(char *);

extern int linenumber;

/***** Macros *************************************************/

// #define DEBUG(fmt, ...) fprintf(stderr, "DEBUG: " fmt "\n", __VA_ARGS__)
#define DEBUG(fmt, ...) do {} while(0)

/***** Funciones locales **************************************/

static
etiqueta_t *nueva_etiqueta(
        char *nombre,
        dword offset,
        region_t region
        ) {
    etiqueta_t *etiqueta;

    etiqueta = (etiqueta_t *) malloc(sizeof (etiqueta_t));

    strncpy(etiqueta->texto, nombre, LONG_ETIQUETA);
    etiqueta->texto[LONG_ETIQUETA - 1] = '\0';
    etiqueta->offset = offset;
    etiqueta->region = region;
    etiqueta->sig = NULL;

    return etiqueta;
}

static
referencia_t *nueva_referencia(
        char *nombre,
        region_t region,
        dword offset,
        parte_direccion_t parte_dir,
        tipo_region_t tipo_region
        ) {
    referencia_t *referencia;

    referencia = (referencia_t *) malloc(sizeof (referencia_t));

    strncpy(referencia->texto, nombre, LONG_ETIQUETA);
    referencia->texto[LONG_ETIQUETA - 1] = '\0';
    referencia->region = region;
    referencia->offset = offset;
    referencia->parte_dir = parte_dir;
    referencia->tipo_region = tipo_region;
    referencia->sig = NULL;

    return referencia;
}

/***** Funciones **********************************************/

simbolo_t *nuevo_simbolo(
        char *nombre,
        dword direccion,
        boolean global
        ) {
    simbolo_t *simbolo;

    simbolo = (simbolo_t *) malloc(sizeof (simbolo_t));

    strncpy(simbolo->texto, nombre, LONG_ETIQUETA);
    simbolo->texto[LONG_ETIQUETA - 1] = '\0';
    simbolo->direccion = direccion;
    simbolo->global = global;
    simbolo->sig = NULL;

    return simbolo;
}

/**
 * Inserta una nueva etiqueta en la tabla de símbolos
 *
 * @param ts Tabla de símbolos
 * @param nombre Nombre de la etiqueta
 * @param offset Desplazamiento dentro de la región
 * @param region Tipo de región
 */
void ts_inserta_etiqueta(
        tabla_simbolos_t *ts,
        char *nombre,
        dword offset,
        region_t region
        ) {

    etiqueta_t *p_etiq;
    etiqueta_t **p_last;

    for (p_etiq = ts->etiqs, p_last = &ts->etiqs;
            p_etiq != NULL;
            p_last = &(p_etiq->sig), p_etiq = p_etiq->sig) {
        if (strcmp(p_etiq->texto, nombre) == 0) {
            char mensaje[128];
            sprintf(mensaje, "%s:%d. Etiqueta '%s' redefinida", basename(ts->fichero), linenumber, nombre);
            yyerror(mensaje);
        }
    }

    *p_last = nueva_etiqueta(nombre, offset, region);
}

/**
 * Busca una etiqueta
 *
 * @param ts Tabla de símbolos
 * @param nombre Nombre de la etiqueta
 * @return El puntero a la etiqueta (etiqueta_t), o NULL si no la encuentra
 */
etiqueta_t *ts_busca_etiqueta(
        tabla_simbolos_t *ts,
        char *nombre
        ) {
    etiqueta_t *p_etiq;

    for (p_etiq = ts->etiqs;
            p_etiq != NULL;
            p_etiq = p_etiq->sig) {
        if (strcmp(p_etiq->texto, nombre) == 0) {
            return p_etiq;
        }
    }

    return NULL;
}

/**
 * Inserta una referencia a una etiqueta en la tabla de símbolos
 *
 * @param ts Tabla de símbolos
 * @param nombre Nombre de la etiqueta referenciada
 * @param region Tipo de región en el que se encuentra la referencia
 * @param offset Desplazamiento dentro de la región
 * @param parte_dir Parte de la dirección absoluta de la etiqueta que se necesita.
 * @param tipo_region Tipo de región donde se debe encontrar la etiqueta
 */
void ts_inserta_referencia(
        tabla_simbolos_t *ts,
        char *nombre,
        region_t region,
        dword offset,
        parte_direccion_t parte_dir,
        tipo_region_t tipo_region
        ) {
    referencia_t *p_ref;
    referencia_t **p_last;

    // Va hasta el final de la lista
    for (p_ref = ts->refs, p_last = &ts->refs;
            p_ref != NULL;
            p_last = &(p_ref->sig), p_ref = p_ref->sig);

    *p_last = nueva_referencia(nombre, region, offset, parte_dir, tipo_region);
}

/**
 * Resuelve las referencias locales a un objeto
 *
 * @param ts Tabla de símbolos
 * @param mm Mapa de memoria
 */
void ts_resuelve_referencias_locales(
        mapa_memoria_t *mm_global,
        tabla_simbolos_t *ts,
        mapa_memoria_t *mm
        ) {
    referencia_t *p_ref;
    referencia_t *p_next;
    referencia_t **p_last;

    // simbolo_t *p_sim;
    etiqueta_t *p_etiq;

    referencia_t *p_deleted = NULL;

    // Recorre la lista de referencias del objeto
    for (p_ref = ts->refs, p_last = &ts->refs;
            p_ref != NULL;
            p_ref = p_next) {

        // Guardamos el valor del siguiente por si la referencia actual se borra
        p_next = p_ref->sig;

        p_etiq = ts_busca_etiqueta(ts, p_ref->texto);

        if (p_etiq != NULL) {
            // Referencia local encontrada
            // char mensaje[128];
            instruccion_t instruccion;
            dword valor;
            dword dir_ref;
            dword dir_instruc;

            // Obtiene la instrucción implicada
            instruccion = mem_lee_region_instruc(mm, r_text, p_ref->offset);
            dir_ref = offset_a_dir(mm, p_etiq->offset, p_etiq->region);
            dir_instruc = offset_a_dir(mm, p_ref->offset, p_ref->region);

            switch (p_ref->parte_dir) {
                case PC_REL_16:
                    valor = dir_ref - dir_instruc - 4;
                    if (valor > INT16_MAX || valor < INT16_MIN) {
                        fprintf(stderr,
                                "%s: Valor de desplazamiento para la referencia '%s' (" F_DWORD ") fuera de límite.",
                                ts->fichero, p_ref->texto, valor);
                        exit(1);
                    }
                    // TODO El valor del desplazamiento debería estar dividido por 4
                    instruccion.inmediato = (half) valor;

                    DEBUG("%s: Referencia '%s' resuelta: PC-REL (PC: "
                            F_DWORD
                            ")", ts->fichero, p_ref->texto, p_ref->offset);

                    break;
                case A32_LO:
                    valor = dir_ref;
                    valor &= UINT16_MAX;
                    instruccion.inmediato = (half) valor;

                    DEBUG("%s: Referencia '%s' resuelta: %%lo(A32)", ts->fichero, p_ref->texto);

                    break;
                case A32_HI:
                    valor = dir_ref;
                    valor >>= 16;
                    valor &= UINT16_MAX;
                    instruccion.inmediato = (half) valor;

                    DEBUG("%s: Referencia '%s' resuelta: %%hi(A32)", ts->fichero, p_ref->texto);

                    break;
                case DATA_REL_16:
                    valor = dir_ref - mm_global->regiones[r_data].inicio;

                    instruccion.inmediato = (half) valor;

                    DEBUG("%s: Referencia '%s' resuelta: %%disp(A32)", ts->fichero, p_ref->texto);

                    break;
                default:
                    fprintf(stderr, "%s: Referencia '%s'. Tipo no soportado.", ts->fichero, p_ref->texto);
                    exit(1);
                    break;
            }

            // Actualiza la instrucción implicada
            mem_escribe_region_instruc(mm, r_text, p_ref->offset, instruccion);

            // Borrar la referencia de la lista
            *p_last = p_next; // la quitamos de esta lista

            p_ref->sig = p_deleted; // La añadimos a la lista de borradas
            p_deleted = p_ref;

        } else {
            // La referencia no se ha encontrado
            // Hay que actualizar p_last porque avanzamos en la lista
            // Si se borra el nodo previo no cambia, y por lo tanto, el p_last tampoco
            p_last = &(p_ref->sig);
        }
    }

    // Liberamos la memoria de las referencias resueltas
    while (p_deleted != NULL) {
        p_ref = p_deleted;
        p_deleted = p_deleted->sig;
        free(p_ref);
    }
}

/**
 * Inserta un nuevo símbolo global en la tabla de símbolos.
 *
 * @param ts Tabla de símbolos
 * @param nombre Nombre del símbolo
 * @param region Región a la que pertenece el símbolo
 */
simbolo_t *ts_inserta_simbolo(
        tabla_simbolos_t *ts,
        char *nombre
        ) {
    simbolo_t *p_sim;

    // Crea un símbolo global pendiente de resolver
    p_sim = nuevo_simbolo(nombre, 0, SI);
    p_sim->sig = ts->sims;
    ts->sims = p_sim;

    return p_sim;
}

/**
 * Resuelve los símbolos con las direcciones absolutas de las etiquetas
 *
 * @param ts Tabla de símbolos
 * @param mm Mapa de memoria
 */
void ts_resuelve_simbolos(
        tabla_simbolos_t *ts,
        mapa_memoria_t *mm
        ) {
    simbolo_t *p_sim;
    etiqueta_t *p_etiq;

    for (p_sim = ts->sims;
            p_sim != NULL;
            p_sim = p_sim->sig) {

        p_etiq = ts_busca_etiqueta(ts, p_sim->texto);

        if (p_etiq != NULL) {
            // Etiqueta encontrada
            p_sim->direccion = offset_a_dir(mm, p_etiq->offset, p_etiq->region);

            DEBUG("%s: Símbolo '%s' resuelto: "
                    F_DWORD, ts->fichero, p_sim->texto, p_sim->direccion);
        } else {
            fprintf(stderr, "%s: Símbolo '%s' no definido.\n", ts->fichero, p_sim->texto);
            exit(1);
        }
    }
}

/**
 * Mueve la lista de símbolos del objeto ya resueltos a la lista de símbolos globales.
 *   La lista de símbolos locales se destruye.
 *
 * @param p_sims_global Lista de símbolos globales
 * @param sims_local Lista de símbolos local del objeto
 */
void ts_mueve_simbolos(
        char *fichero_objeto,
        simbolo_t **p_sims_global,
        simbolo_t **sims_local
        ) {
    simbolo_t *p_local;
    simbolo_t *p_sim;
    simbolo_t *p_next;
    simbolo_t **pp_last;

    if (*sims_local == NULL) return;

    for (p_local = *sims_local; p_local != NULL; p_local = p_next) {
        p_next = p_local->sig;

        for (p_sim = *p_sims_global, pp_last = p_sims_global;
                p_sim != NULL;
                pp_last = &(p_sim->sig), p_sim = p_sim->sig) {
            if (p_local->global && p_sim->global && strcmp(p_local->texto, p_sim->texto) == 0) {
                fprintf(stderr, "%s: Símbolo '%s' re-definido.\n", fichero_objeto, p_sim->texto);
                exit(1);
            }
        }

        *pp_last = p_local;
        p_local->sig = NULL;
    }

    *sims_local = NULL;
}

/**
 * Ordena la lista de símbolos.
 *   Va insertando por la cabeza de la lista el símbolo que queda con la dirección más alta
 *
 * @param sims Lista de símbolos
 */
void ts_ordena_simbolos(
        simbolo_t **sims
        ) {
    simbolo_t *l_sim;
    simbolo_t *p_sim;
    simbolo_t **pp_last;
    simbolo_t **pp_max;

    dword direccion;

    // Mueve la lista de símbolos
    l_sim = *sims;
    *sims = NULL;

    // Mientras queden símbolos en la lista original
    while (l_sim != NULL) {
        direccion = 0;
        pp_max = &l_sim;

        // Busca el símbolo con la mayor dirección
        for (p_sim = l_sim, pp_last = &l_sim;
                p_sim != NULL;
                pp_last = &(p_sim->sig), p_sim = p_sim->sig) {

            if (p_sim->direccion > direccion) {
                pp_max = pp_last;
                direccion = p_sim->direccion;
            } else if (p_sim->direccion == direccion) {
                if ((*pp_max)->texto[0] == '_') {
                    // Los símbolos que comienzan por _ van antes en la lista
                    pp_max = pp_last;
                }
            }
        }

        // Quita el símbolo de la lista
        p_sim = *pp_max;
        *pp_max = p_sim->sig;

        // Lo añade a la cabeza de la lista de símbolos
        p_sim->sig = *sims;
        *sims = p_sim;
    }

}

/**
 * Busca un símbolo
 *
 * @param sims Lista de símbolos globales
 * @param nombre Nombre dels símbolo
 * @return El puntero al símbolo (simbolo_t), o NULL si no la encuentra
 */
simbolo_t *ts_busca_simbolo(
        simbolo_t *sims,
        char *nombre
        ) {
    simbolo_t *p_sim;

    for (p_sim = sims; p_sim != NULL; p_sim = p_sim->sig) {
        if (p_sim->global && strcmp(p_sim->texto, nombre) == 0) {
            return p_sim;
        }
    }

    return NULL;
}

/**
 * Inserta en la tabal de símbolos las etiquetas locales
 *   como símbolos locales a un objeto.
 *
 * @param ts Tabla de simbolos
 * @param mm Mapa de memoria
 */
void ts_inserta_simbolos_locales(
        tabla_simbolos_t *ts,
        mapa_memoria_t *mm
        ) {
    simbolo_t *p_sim;
    etiqueta_t *p_etiq;

    for (p_etiq = ts->etiqs; p_etiq != NULL; p_etiq = p_etiq->sig) {
        p_sim = ts_busca_simbolo(ts->sims, p_etiq->texto);

        if (p_sim == NULL) {
            dword dir_ref = offset_a_dir(mm, p_etiq->offset, p_etiq->region);
            // Añade el nuevo símbolo local
            p_sim = nuevo_simbolo(p_etiq->texto, dir_ref, NO);
            p_sim->sig = ts->sims;
            ts->sims = p_sim;
        }
    }
}

/**
 * Busca el símbolo más cercano a una dirección.
 *   Asume que la lista de símbolos está ordenada en orden creciente de direcciones.
 *
 * @param sims Lista de símbolos
 * @param direccion Dirección de memoria
 * @param p_index Si != NULL, se almacena el índice del símbolo encontrado.
 * @return El símbolo con la dirección de memoria más cercana menor o igual que 'direccion'.
 *      Si no hay ningún símbolo con dirección menor, devuelve null. No debería pasar.
 */
simbolo_t *ts_busca_direccion(
        simbolo_t *sims,
        dword direccion,
        int *p_index
        ) {
    simbolo_t *p_sim;
    simbolo_t *p_last;
    int i_sim = 0;

    p_last = NULL;
    for (p_sim = sims; p_sim != NULL && p_sim->direccion <= direccion; p_last = p_sim, p_sim = p_sim->sig, i_sim++);

    if (p_index != NULL) {
        *p_index = i_sim;
    }

    return p_last;
}

/**
 * Escribe el símbolo o el símbolo más el desplazamiento asociado a una dirección
 *
 * @param sims Lista de símbolos
 * @param mm Mapa de memoria
 * @param direccion Dirección de memoria
 * @param etiqueta Dirección donde escribir la etiqueta
 * @param f_etiqueta Si es cierto, y la dirección no coincide con un símbolo,
 *                   escribe el símbolo más el desplazamiento
 */
void ts_simbolo_direccion(
        simbolo_t *sims,
        dword direccion,
        char *etiqueta,
        boolean f_etiqueta
        ) {
    simbolo_t *p_sim;

    p_sim = ts_busca_direccion(sims, direccion, NULL);

    if (p_sim != NULL) {
        if (p_sim->direccion == direccion) {
            // Dirección exacta
            strcpy(etiqueta, p_sim->texto);
        } else {
            if (f_etiqueta) {
                sprintf(etiqueta, "%s + %04" PRId64, p_sim->texto, direccion - p_sim->direccion);
            } else {
                /* Etiqueta exacta no encontrada */
                sprintf(etiqueta, F_DWORD, direccion);
            }
        }
    } else {
        /* Etiqueta no encontrada */
        sprintf(etiqueta, F_DWORD, direccion);
    }
}

/**
 * Resuelve las referencias globales de cada objeto y las elimina
 *
 * @param sims Tabla de símbolos globales
 * @param ts Tabla de símbolos del objeto
 * @param mm Mapa de memoria del objeto
 */
void ts_resuelve_referencias_globales(
        simbolo_t *sims_globales,
        tabla_simbolos_t *ts,
        mapa_memoria_t *mm
        ) {
    referencia_t *p_ref;
    referencia_t *p_next;

    simbolo_t *p_sim;

    // Va hasta el final de la lista
    for (p_ref = ts->refs; p_ref != NULL; p_ref = p_next) {
        p_next = p_ref->sig;

        p_sim = ts_busca_simbolo(sims_globales, p_ref->texto);

        if (p_sim != NULL) {
            // Referencia local encontrada
            instruccion_t instruccion;
            dword dir_instruc;
            dword valor;

            // Obtiene la instrucción implicada
            instruccion = mem_lee_region_instruc(mm, r_text, p_ref->offset);

            switch (p_ref->parte_dir) {
                case PC_REL_16:
                    dir_instruc = offset_a_dir(mm, p_ref->offset, p_ref->region);
                    valor = p_sim->direccion - dir_instruc - 4;
                    if (valor > INT16_MAX || valor < INT16_MIN) {
                        fprintf(stderr,
                                "%s: Valor de desplazamiento para la referencia '%s' (" F_DWORD ") fuera de límite.",
                                ts->fichero, p_ref->texto, valor);
                        exit(1);
                    }
                    // TODO El valor del desplazamiento debería estar dividido por 4
                    instruccion.inmediato = (half) valor;

                    DEBUG("%s: Referencia '%s' resuelta: PC-REL (PC: "
                            F_DWORD
                            ")", ts->fichero, p_ref->texto, p_ref->offset);

                    break;
                case A32_LO:
                    valor = p_sim->direccion;
                    valor &= UINT16_MAX;
                    instruccion.inmediato = (half) valor;

                    DEBUG("%s: Referencia '%s' resuelta: %%lo(A32)", ts->fichero, p_ref->texto);

                    break;
                case A32_HI:
                    valor = p_sim->direccion;
                    valor >>= 16;
                    valor &= UINT16_MAX;
                    instruccion.inmediato = (half) valor;

                    DEBUG("%s: Referencia '%s' resuelta: %%hi(A32)", ts->fichero, p_ref->texto);

                    break;
                default:
                    fprintf(stderr, "%s: Referencia '%s'. Tipo no soportado.", ts->fichero, p_ref->texto);
                    exit(1);
                    break;
            }

            // Actualiza la instrucción implicada
            mem_escribe_region_instruc(mm, r_text, p_ref->offset, instruccion);

        } else {
            // La referencia no se ha encontrado
            fprintf(stderr, "%s: Referencia a '%s' sin definir.\n", ts->fichero, p_ref->texto);
            exit(1);
        }

        // Libera la referencia
        free(p_ref);
    }

    ts->refs = NULL;
}

void ts_libera_etiquetas(
        etiqueta_t **l_etiq
        ) {
    etiqueta_t *p_etiq;
    etiqueta_t *p_next;

    for (p_etiq = *l_etiq;
            p_etiq != NULL; p_etiq = p_next) {
        p_next = p_etiq->sig;

        free(p_etiq);
    }

    *l_etiq = NULL;
}

void ts_libera_simbolos(
        simbolo_t **l_sim
        ) {
    simbolo_t *p_sim;
    simbolo_t *p_next;

    for (p_sim = *l_sim;
            p_sim != NULL; p_sim = p_next) {
        p_next = p_sim->sig;

        free(p_sim);
    }

    *l_sim = NULL;
}
