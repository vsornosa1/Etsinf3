/*********************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *	        Pedro López (plopez@disca.upv.es)
 * 
 * File: presentacion-html.h
 *
 * Description:
 *    Definiciones para el módulo de presentación
 *
 *********************************************************************/

#ifndef presentacion_html_H
#define presentacion_html_H

/***** Includes ***********************************************/

#include "tipos.h"

/***** Constants **********************************************/

#define MAX_INSTRUC 70 /* Número máximo de instrucciones visibles */
#define MAX_CICLOS_INIC 45 /* Número máximo de ciclos visibles
                                   inicialmente */
#define MAX_CICLOS 50 /* Número máximo de ciclos visibles */

/***** Types **************************************************/

typedef char n_fase_t[64]; /* Nombre de la fase */
typedef char n_instruccion_t[64]; /* Instrucción */

typedef struct {
    dword PC;
    n_instruccion_t inst; /* Instrucción */
    n_fase_t fase[MAX_CICLOS];
    /* Fases de la instrucción */
} linea_crono_t; /* Una linea del cronograma */

/***** Prototypes *********************************************/

#ifndef CPROTO
#include "presentacion_html_api.h"
#endif

#ifdef presentacion_html_C  /*** Privado ***/

linea_crono_t crono[MAX_INSTRUC];
/* Instrucciones visibles en el
   cronograma */

boolean html_merge = NO;

#else    /*** Público ***/

extern linea_crono_t crono[MAX_INSTRUC];
/* Instrucciones visibles en el
   cronograma */

extern boolean html_merge;

extern presentacion_interface_t presentacion_html;

#endif

#endif
