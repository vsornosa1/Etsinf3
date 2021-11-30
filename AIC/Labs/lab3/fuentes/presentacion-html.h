/*********************************************************************
 * 
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.                         
 * 
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *	   Pedro López (plopez@disca.upv.es)
 * 
 * File: presentacion-html.h
 *  
 * Description: 
 *    Definiciones para el módulo de presentación
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *********************************************************************/


/***** Includes ***********************************************/


/***** Constants **********************************************/

#define NUM_STYLES 16

#define _MAX_INSTRUC 20 /* Número máximo de instrucciones visibles */
#define _MAX_CICLOS_INIC _MAX_INSTRUC /* Número máximo de ciclos visibles
                                   inicialmente */
#define _MAX_CICLOS _MAX_INSTRUC+_MAX_INSTRUC/2 /* Número máximo de ciclos visibles */

// Pantallas pequeñas
#define _MAX_INSTRUC_s 16 /* Número máximo de instrucciones visibles */
#define _MAX_CICLOS_INIC_s _MAX_INSTRUC_s /* Número máximo de ciclos visibles
                                   inicialmente */
#define _MAX_CICLOS_s _MAX_INSTRUC_s+_MAX_INSTRUC_s/2 /* Número máximo de ciclos visibles */


/* Colores */
#define BG_HEADER "#ccddff"// Header 

#define BG_DIT1 "#ddeeff"  // D i-t
#define BG_DIT2 "#ccddff"  // D i-t
#define BG_VAL1 "#eeeeee"  // D i-t
#define BG_VAL2 "#dddddd"  // D i-t

#define BG_OPER BG_DIT2  // Oper Name 
#define BG_STAGE BG_DIT1 // Stage

#define BG_REGS BG_DIT1  // Registros

#define BG_SIG BG_DIT2   // Signals

#define BG_VAL BG_VAL1   // Value

#define BG_HEADER_COL1 BG_DIT2// Header col
#define BG_HEADER_COL2 BG_DIT1// Header col

#define BG_COL1 BG_VAL1// Value col 
#define BG_COL2 BG_VAL2// Value col

#define BG_DIR1 BG_DIT2
#define BG_DIR2 BG_DIT1
#define BG_DIR_VAL BG_VAL2

#define TEXT_GRAY "darkgray"


#define BG_RW "yellow"     //  RW
//#define BG_W "tomato"      //  W
//#define BG_R "turquoise"   //  R

/* Anchos */
#define PC_CRONO_W 10
#define INSTR_CRONO_W 190
#define FASE_CRONO_W 25
#define NOMBRE_ETAPAS_W 80
#define ETAPAS_W 90
#define SENYALES_W ETAPAS_W
#define MEM_W 50
#define REG_W 60

/* Tamaño fuente */
#define FONT_SIZE 10
#define FONT_NAME "Courier"
#define LINE_HEIGHT "100%"


/***** Types **************************************************/

typedef char n_fase_t[64]; /* Nombre de la fase */
typedef char n_instruccion_t[128]; /* Instrucción */

typedef struct {
    char PC[16]; /* PC o etiqueta */
    n_instruccion_t inst; /* Instrucción */
    n_fase_t fase[_MAX_CICLOS];
    /* Fases de la instrucción */
} linea_crono_t; /* Una linea del cronograma */




/***** Macros *************************************************/


/*** Funciones ************************************************/

#ifndef CPROTO
#include "presentacion_html_api.h"
#endif




