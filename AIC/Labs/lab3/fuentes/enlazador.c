/**************************************************************************
 *
 * Departamento de Informatica de Sistemas y Computadores (DISCA)
 * Universidad Politecnica de Valencia.
 *
 * Author:	Sergio Sáez (ssaez@disca.upv.es)
 *
 * File: enlazador.c
 *
 * Description: Contiene las funciones necesarias para enlazar los segmentos de
 *      memoria en una zona de memoria única.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *************************************************************************/

#define enlazador_C

/***** Includes ***********************************************/

#include <strings.h>
#ifdef LIBMIPS
void bzero(void *s, size_t n); /* Por alguna razón no la exporta strings.h con el compilador emscripten. */
#endif
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "registros.h"
#include "main.h"

#include "enlazador.h"

/***** Funciones **********************************************/

void inicializa_programa(
        programa_t *prog,
        int num_ficheros
        ) {

    prog->l_objs = (fichero_objeto_t *) malloc(num_ficheros * sizeof (fichero_objeto_t));
    prog->n_objs = num_ficheros;
    prog->simbolos = NULL;

    // La memoria se inicializa al construir el programa.
}

void inicializa_objeto(
        fichero_objeto_t *obj,
        char *fichero
        ) {
    bzero(obj, sizeof (fichero_objeto_t));

    strcpy(obj->ts.fichero, fichero);

    inicializa_memoria(&obj->memoria);
}

void destruye_objeto(
        fichero_objeto_t *obj
        ) {
    destruye_memoria(&obj->memoria);
}

void destruye_objetos_programa(
        programa_t *prog
        ) {
    fichero_objeto_t *obj;

    for (int i = 0; i < prog->n_objs; ++i) {
        obj = &prog->l_objs[i];

        destruye_objeto(obj);
    }
}

/**
 * Enlaza los ficheros objeto en un único mapa de memoria resolviendo todos los símbolos globales
 *
 * @param prog
 */
void enlazador(
        programa_t *prog
        ) {
    mapa_memoria_t *mm;
    fichero_objeto_t *obj;

    region_t r;
    dword dir;
    char etiqueta[LONG_ETIQUETA];

    simbolo_t *p_sim;

    prog->simbolos = NULL;
    mm = &prog->memoria;

    // Calcula los nuevos límites de las regiones de cada objeto
    dir = 0;
    for (r = (region_t) 0; r < num_regiones; ++r) {
        switch (r) {
            case r_reserved:
                // Es la primera región
                dir += TAM_MEM_RESERVED;

                mm->regiones[r].inicio = 0;
                mm->regiones[r].final = TAM_MEM_RESERVED;
                break;
            case r_text:
            case r_data:
                dir = mem_direccion_alineada(dir, ALIGN_REGION);

                // Concatena las regiones de todos los objetos realineando si procede
                for (int i = 0; i < prog->n_objs; ++i) {
                    dword r_tam;

                    obj = &prog->l_objs[i];

                    /* Corrige el alineamiento de la región (v.g 4 u 8 bytes)
                     * Las regiones del mismo tipo se concatenan
                     * considernado el alineamiento requerido por cada una.
                     */
                    dir = mem_direccion_alineada(dir, obj->memoria.regiones[r].align);

                    // Tamaño de la región del objeto actual
                    r_tam = tam_region(&obj->memoria, r);

                    /*
                     * Si la región está vacia, los cálculos no afectan al resto.
                     * No hace falta eliminar las asignaciones.
                     * Así los valores inicial y final son correctos.
                     */

                    // Modificación de los límites de la región
                    obj->memoria.regiones[r].inicio = dir;
                    obj->memoria.regiones[r].final = dir + r_tam;

                    dir += r_tam;
                }

                // Calcula los límites del mapa de memoria global
                mm->regiones[r].inicio = prog->l_objs[0].memoria.regiones[r].inicio;
                mm->regiones[r].final = prog->l_objs[prog->n_objs - 1].memoria.regiones[r].final;

                // Inserta un símbolo global al principio y al final de la región
                sprintf(etiqueta, "_%s", mm->regiones[r].nombre);
                p_sim = nuevo_simbolo(etiqueta, mm->regiones[r].inicio, SI);
                p_sim->sig = prog->simbolos;
                prog->simbolos = p_sim;

                sprintf(etiqueta, "_e%s", mm->regiones[r].nombre);
                p_sim = nuevo_simbolo(etiqueta, mm->regiones[r].final, SI);
                p_sim->sig = prog->simbolos;
                prog->simbolos = p_sim;

                break;
            case r_stack:
                // Experimental
                dir = DIR_FINAL_PILA;

                // TODO Concatenar en orden inverso

                mm->regiones[r].inicio = DIR_FINAL_PILA - TAM_MEMO_PILA;
                mm->regiones[r].final = DIR_FINAL_PILA;

                // Inserta un símbolo global al principio y al final de la región
                sprintf(etiqueta, "_%s", mm->regiones[r].nombre);
                p_sim = nuevo_simbolo(etiqueta, mm->regiones[r].inicio, SI);
                p_sim->sig = prog->simbolos;
                prog->simbolos = p_sim;

                sprintf(etiqueta, "_e%s", mm->regiones[r].nombre);
                p_sim = nuevo_simbolo(etiqueta, mm->regiones[r].final, SI);
                p_sim->sig = prog->simbolos;
                prog->simbolos = p_sim;

                break;
            default:
                break;
        }
    }

    // Una vez actualizados los comienzos y finales de cada región en cada objeto ...
    for (int i = 0; i < prog->n_objs; ++i) {
        obj = &prog->l_objs[i];

        // Resuelve las referencias locales para cada objeto
        ts_resuelve_referencias_locales(mm, &obj->ts, &obj->memoria);

        // Calcula las direcciones de los símbolos globales
        ts_resuelve_simbolos(&obj->ts, &obj->memoria);

        // Inserta los símbolos locales
        ts_inserta_simbolos_locales(&obj->ts, &obj->memoria);

        // Construye una tabla de símbolos global
        ts_mueve_simbolos(obj->ts.fichero, &prog->simbolos, &obj->ts.sims);

        // Destruye la lista de etiquetas del objeto
        ts_libera_etiquetas(&obj->ts.etiqs);

    }

    // Ordena la lista de símbolos por direcciones de memoria
    ts_ordena_simbolos(&prog->simbolos);

    // Con la tabla de símbolos globales resolver las referencias pendientes
    for (int i = 0; i < prog->n_objs; ++i) {
        obj = &prog->l_objs[i];

        // Resuelve las referencias globales para cada objeto
        ts_resuelve_referencias_globales(prog->simbolos, &obj->ts, &obj->memoria);
    }

    // Concatena la memoria de los diferentes objetos
    concatena_objetos(prog);

}

/**
 * Establece los valores iniciales de lso registros del procesador antes de comenzar
 *   la ejecución.
 *
 * @param prog
 */
void cargador(
        programa_t *prog
        ) {

    simbolo_t *p_sim;
    dword dir;

    /* Inicializa los registros antes de la ejecución */

    p_sim = ts_busca_simbolo(prog->simbolos, "_data");
    if (p_sim != NULL) {
        dir = p_sim->direccion;
    } else {
        fprintf(stderr, "Error interno. Símbolo '_data' sin definir.");
        exit(1);
    }
    // $gp contiene la dirección base de la zona de datos
    escribe_int_reg(_gp, dir, t_dword, NO);

    p_sim = ts_busca_simbolo(prog->simbolos, "_estack");
    if (p_sim != NULL) {
        dir = p_sim->direccion;
    } else {
        fprintf(stderr, "Error interno. Símbolo '_estack' sin definir.");
        exit(1);
    }
    // $sp apunta al final de la región de la pila.
    escribe_int_reg(_sp, dir, t_dword, NO);

    /* Dirección inicial del código a ejecutar (opcional) */
    p_sim = ts_busca_simbolo(prog->simbolos, "_start");
    if (p_sim != NULL) {
        printf("Iniciando desde la etiqueta _start:\n\n");
        dir = p_sim->direccion;
    } else {
        dir = inicio_region(&prog->memoria, r_text);
    }
    PC_inicial = dir;

}

/**
 * Copia las regiones de memoria de cada objeto a la memoria global
 *
 * @param prog
 */
void concatena_objetos(
        programa_t *prog
        ) {
    dword dir = 0;
    region_t r;
    fichero_objeto_t *obj;

    for (r = (region_t) 0; r < num_regiones; ++r) {
        for (int i = 0; i < prog->n_objs; ++i) {
            size_t r_tam;
            dword ind_prog;
            dword ind_obj;

            obj = &prog->l_objs[i];

            dir = inicio_region(&obj->memoria, r);
            r_tam = (size_t) tam_region(&obj->memoria, r);

            if (r_tam == 0) continue;

            switch (r) {
                case r_text:
                    ind_prog = indice_instruc(dir_a_offset(&prog->memoria, dir, r));
                    ind_obj = indice_instruc(dir_a_offset(&obj->memoria, dir, r));

                    // El número de instrucciones es el tamaño de la región dividido por 4
                    memcpy(&(prog->memoria.instruc[ind_prog]),
                            &(obj->memoria.instruc[ind_obj]), (r_tam / 4) * sizeof (instruccion_t));
                    break;
                case r_data:
                case r_stack:
                    ind_prog = indice_byte(dir_a_offset(&prog->memoria, dir, r));
                    ind_obj = indice_byte(dir_a_offset(&obj->memoria, dir, r));

                    memcpy(&(prog->memoria.datos[r].m_byte[ind_prog]),
                            &(obj->memoria.datos[r].m_byte[ind_obj]), r_tam);
                    // Hay un tipo por cada byte, así que se puede utilizar el mismo índice
                    memcpy(&(prog->memoria.tipos[r][ind_prog]),
                            &(obj->memoria.tipos[r][ind_obj]), r_tam * sizeof (tipo_t));
                    break;
                default:
                    // Nada que copiar
                    break;
            }
        }
    }

}
