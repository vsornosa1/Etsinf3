/*********************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad PolitÈcnica de Valencia.
 *
 * Autor: Sergio Sáez (ssaez@disca.upv.es)
 *	  Pedro López (plopez@gap.upv.es)
 *
 * Fichero: enlazador.h
 *
 * Descripción:
 *   Definiciones
 *
 ***********************************************************************/

#ifndef enlazador_H
#define enlazador_H

/***** Includes ***********************************************/

#include "memoria.h"
#include "simbolos.h"

/***** Types **************************************************/

typedef struct {
    tabla_simbolos_t ts;

    mapa_memoria_t memoria;
} fichero_objeto_t;

typedef struct {
    fichero_objeto_t * l_objs;
    int n_objs;

    simbolo_t * simbolos;

    mapa_memoria_t memoria;
} programa_t;

/***** Prototypes *********************************************/

#ifndef CPROTO
#include "enlazador_api.h"
#endif

#endif
