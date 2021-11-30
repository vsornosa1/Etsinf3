/*********************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *	        Pedro López (plopez@disca.upv.es)
 *
 * File: main.h
 *
 * Description: Contiene las definiciones de constantes y de variables
 *    globales a todo el simulador
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *********************************************************************/

#ifndef main_H
#define main_H

/*** Includes ********************************************************/

#include <stdio.h>
#include "tipos.h"
#include "estado.h"
#include "enlazador.h"

/***** Macros *************************************************/

#ifdef MIN
#undef MIN
#endif
#define MIN(x, y) (((x)<(y)) ? (x) : (y))

#ifdef MAX
#undef MAX
#endif
#define MAX(x, y) (((x)>(y)) ? (x) : (y))

/*** Defines *********************************************************/

#define MAX_FICHEROS 5

#define TAM_ESTADO 128 // Tamaño de la tabla hash para mantener el estado previo

/*** Tamaño de las estructuras de datos */

#define MAX_VIAS_SUPERESCALAR   16

#define _NUM_VIAS_ISSUE  1 /* Valor por omisión */
#define _NUM_VIAS_BUS  1 /* Valor por omisión */
#define _NUM_VIAS_COMMIT 1 /* Valor por omisión */

#define MAX_BUFFER_PREDIC       16
#define _TAM_BUFFER_PREDIC      2

#if PRAC == 0
#define MAX_CICLOS_HTML     1000
#define MAX_CICLOS_TOTAL    100000000
#else
#define MAX_CICLOS_HTML  100
#define MAX_CICLOS_TOTAL    100000
#endif

#define _TAM_RS_ENTEROS  5
#define _TAM_RS_SUMREST 3
#define _TAM_RS_MULTDIV 2
#define _TAM_BUFFER_CARGA 3 /* 1 */
#define _TAM_BUFFER_ALMACEN 3 /* 1 */

#define _TAM_ESTACIONES (_TAM_RS_ENTEROS + \
                        _TAM_RS_SUMREST + \
                        _TAM_RS_MULTDIV + \
                        _TAM_BUFFER_CARGA + \
                        _TAM_BUFFER_ALMACEN)

#define MAX_ESTACIONES  4096

#define _TAM_REGISTROS  32
#define PC_RET    TAM_REGISTROS-1

#define MAX_REORDER    4096
#define _TAM_REORDER    20 /* Valor por omisión */

#define MAX_OPERADORES  100 /* Número de operadores * latencia máxima */

#define _NUM_OPER_ENTEROS 1
#define _NUM_OPER_SUMREST 1
#define _NUM_OPER_MULTDIV 1
#define _NUM_OPER_DIRECCIONES 1
#define _NUM_OPER_MEMDATOS 1

#define _NUM_INICIO_ENTEROS _NUM_OPER_ENTEROS
#define _NUM_INICIO_SUMREST _NUM_OPER_SUMREST
#define _NUM_INICIO_MULTDIV _NUM_OPER_MULTDIV
#define _NUM_INICIO_DIRECCIONES _NUM_OPER_DIRECCIONES
#define _NUM_INICIO_MEMDATOS _NUM_OPER_MEMDATOS

/*** Tiempos de evaluacion */

#define _TEVAL_ENTEROS  1
#define _TEVAL_MEMDATOS  2 /* + 1 ciclo del cálculo de la dirección efectiva */
#define _TEVAL_SUMREST  4
#define _TEVAL_MULTDIV  7
#define _TEVAL_DIRECCIONES 1

/* Nombre del fichero de traza */
#define trace_filename "_branch_trace.txt"

#define CHECK_DEFAULT_VALUE(v) \
do { \
   if ((v) == 0) { \
      (v) = _##v; \
   } \
} while (0)

#define console_name "_consola.txt"
#define log_name "_log.txt"


// Lo siguiente es necesario para que los ficheros .o que no se compilan
// vean los cambios planificados en la practica y para la modificación línea de ordenes


#ifdef main_C

int NUM_VIAS_ISSUE = _NUM_VIAS_ISSUE;
int NUM_VIAS_BUS = _NUM_VIAS_BUS;
int NUM_VIAS_COMMIT = _NUM_VIAS_COMMIT;

int TAM_REORDER = _TAM_REORDER;
int TAM_BUFFER_PREDIC = _TAM_BUFFER_PREDIC;

int TAM_RS_ENTEROS = _TAM_RS_ENTEROS;
int TAM_RS_SUMREST = _TAM_RS_SUMREST;
int TAM_RS_MULTDIV = _TAM_RS_MULTDIV;
int TAM_BUFFER_CARGA = _TAM_BUFFER_CARGA;
int TAM_BUFFER_ALMACEN = _TAM_BUFFER_ALMACEN;
int TAM_ESTACIONES = _TAM_ESTACIONES;

const int TAM_REGISTROS = _TAM_REGISTROS;

int NUM_OPER_ENTEROS = _NUM_OPER_ENTEROS;
int NUM_OPER_SUMREST = _NUM_OPER_SUMREST;
int NUM_OPER_MULTDIV = _NUM_OPER_MULTDIV;
int NUM_OPER_DIRECCIONES = _NUM_OPER_DIRECCIONES;
int NUM_OPER_MEMDATOS = _NUM_OPER_MEMDATOS;

int NUM_INICIO_ENTEROS = _NUM_OPER_ENTEROS;
int NUM_INICIO_SUMREST = _NUM_OPER_SUMREST;
int NUM_INICIO_MULTDIV = _NUM_OPER_MULTDIV;
int NUM_INICIO_DIRECCIONES = _NUM_OPER_DIRECCIONES;
int NUM_INICIO_MEMDATOS = _NUM_OPER_MEMDATOS;

int NUM_OPERADORES = 0;

int TEVAL_ENTEROS = _TEVAL_ENTEROS;
int TEVAL_SUMREST = _TEVAL_SUMREST;
int TEVAL_MULTDIV = _TEVAL_MULTDIV;
int TEVAL_DIRECCIONES = _TEVAL_DIRECCIONES;
int TEVAL_MEMDATOS = _TEVAL_MEMDATOS;

int INICIO_OP_ENTEROS;
int FIN_OP_ENTEROS;
int INICIO_OP_SUMREST;
int FIN_OP_SUMREST;
int INICIO_OP_MULTDIV;
int FIN_OP_MULTDIV;
int INICIO_OP_DIRECCIONES;
int FIN_OP_DIRECCIONES;
int INICIO_OP_MEMDATOS;
int FIN_OP_MEMDATOS;

int INICIO_REORDER;
int FIN_REORDER;

#else


extern int NUM_VIAS_ISSUE;
extern int NUM_VIAS_BUS;
extern int NUM_VIAS_COMMIT;

extern int TAM_REORDER;
extern int TAM_BUFFER_PREDIC;


extern int TAM_RS_ENTEROS;
extern int TAM_RS_SUMREST;
extern int TAM_RS_MULTDIV;
extern int TAM_BUFFER_CARGA;
extern int TAM_BUFFER_ALMACEN;
extern int TAM_ESTACIONES;
extern int TAM_REGISTROS;

extern int NUM_OPER_ENTEROS;
extern int NUM_OPER_SUMREST;
extern int NUM_OPER_MULTDIV;
extern int NUM_OPER_DIRECCIONES;
extern int NUM_OPER_MEMDATOS;

extern int NUM_INICIO_ENTEROS;
extern int NUM_INICIO_SUMREST;
extern int NUM_INICIO_MULTDIV;
extern int NUM_INICIO_DIRECCIONES;
extern int NUM_INICIO_MEMDATOS;

extern int NUM_OPERADORES;

extern int TEVAL_ENTEROS;
extern int TEVAL_SUMREST;
extern int TEVAL_MULTDIV;
extern int TEVAL_DIRECCIONES;
extern int TEVAL_MEMDATOS;

extern int INICIO_OP_ENTEROS;
extern int FIN_OP_ENTEROS;
extern int INICIO_OP_SUMREST;
extern int FIN_OP_SUMREST;
extern int INICIO_OP_MULTDIV;
extern int FIN_OP_MULTDIV;
extern int INICIO_OP_DIRECCIONES;
extern int FIN_OP_DIRECCIONES;
extern int INICIO_OP_MEMDATOS;
extern int FIN_OP_MEMDATOS;

extern int INICIO_REORDER;
extern int FIN_REORDER;

#endif

/*** Contantes */

#define MARCA_NULA    (MAX_REORDER)
#define MAX_ORDEN     MAX_CICLOS_TOTAL
#define MAX_TEXTO_CONSOLA 512
#define MAX_TEXTO_LOG 512



/*** Inicios y finales de las estaciones de reserva */

#define INICIO_ESTACION_RESERVA INICIO_RS_ENTEROS

#define INICIO_RS_ENTEROS (0)
#define FIN_RS_ENTEROS  (INICIO_RS_ENTEROS + TAM_RS_ENTEROS -1)

#define INICIO_RS_SUMREST (FIN_RS_ENTEROS + 1)
#define FIN_RS_SUMREST (INICIO_RS_SUMREST + TAM_RS_SUMREST -1)

#define INICIO_RS_MULTDIV (FIN_RS_SUMREST + 1)
#define FIN_RS_MULTDIV  (INICIO_RS_MULTDIV + TAM_RS_MULTDIV -1)

#define FIN_ESTACION_RESERVA FIN_RS_MULTDIV

#define INICIO_BUFFER_CARGA (FIN_RS_MULTDIV + 1)
#define FIN_BUFFER_CARGA  (INICIO_BUFFER_CARGA + TAM_BUFFER_CARGA -1)

#define INICIO_BUFFER_ALMACEN (FIN_BUFFER_CARGA + 1)
#define FIN_BUFFER_ALMACEN  (INICIO_BUFFER_ALMACEN + TAM_BUFFER_ALMACEN -1)

#define INICIO_BUFFERS         INICIO_BUFFER_CARGA
#define FIN_BUFFERS            FIN_BUFFER_ALMACEN

#define PRIMERA_ESTACION_RESERVA (INICIO_RS_ENTEROS)
#define ULTIMA_ESTACION_RESERVA  (FIN_BUFFER_ALMACEN)

#define ES_ESTACION(s, tipo)  ((s) >= INICIO_##tipo && (s) <= FIN_##tipo)

/*** IDENTIFICADORES */

#define REG_FPSR        TAM_REGISTROS

/*** Types ***********************************************************/

typedef table_state_t * estado_simulador_t;

/*** Inclusion condicional *************************************************/

/***************************************************************************/
/***************************************************************************/

#ifdef main_C   /* Privado para el fichero main.c */

/* Indica que en el ensamblador ya se han inicializado los registros o el btb,
 * ya que sólo se deben inicializar en un fichero */

boolean init_ireg = NO,
        init_fpreg = NO,
        init_btb = NO;

/* Indica que en el ensamblador se utilizan los tipos de datos correspondientes */
boolean hay_double = NO,
        hay_float = NO,
        hay_dword = NO,
        hay_word = NO,
        hay_half = NO,
        hay_byte = NO,
        hay_char = NO;

/*** Opciones de Ejecución */

boolean f_etiquetas = NO;
boolean f_macros = NO;

/*** Variables internas  */

ciclo_t Ciclo;
boolean terminando;
boolean SIG_SYSCALL;

char nombre_fich[32*MAX_FICHEROS];
dword PC_ISS = -1;
int RB_cab = -1;
estadisticas_t estat;
utilizacion_t max_util;
utilizacion_t aver_util;
boolean IFstall;

dep_mem_t dep_mem = l_bypassing;
tipo_predictor_t tipo_predictor = BTB_1_BIT;

enum {
    TRAZA, FINAL
} fase_ejecucion = FINAL;

boolean final;

/*** Estructuras visibles al programador */

reg_t Rfp[_TAM_REGISTROS];
tipo_t Rfp_tipo[_TAM_REGISTROS];
boolean Rfp_dirty[_TAM_REGISTROS];
reg_t Rint[_TAM_REGISTROS + 1]; /* El último simula el 'fpsr' */
tipo_t Rint_tipo[_TAM_REGISTROS + 1];
boolean Rint_dirty[_TAM_REGISTROS + 1];

/*** Registros inter-etapa para la segmentación */

/* Representan a los registros inter-etapa que contienen los valores
   para el siguiente ciclo, los valores futuros */
/* DEBEN ESTAR SIEMPRE A LA IZQUIERDA DE UNA ASIGNACION */

reg_if_iss_t M_IF_ISS_1[MAX_VIAS_SUPERESCALAR];

reg_if_iss_t IF_ISS_1;
reg_control_t Control_1;

/* Representan a los registros inter-etapa que contienen los valores
   del presente ciclo. Serán estables hasta que se vuelvan escribir al
   final del ciclo con los nuevos valores */
/* DEBEN ESTAR SIEMPRE A LA DERECHA DE UNA ASIGNACION O EN LAS
   COMPARACIONES */

reg_if_iss_t M_IF_ISS_2[MAX_VIAS_SUPERESCALAR];

reg_if_iss_t IF_ISS_2;
reg_control_t Control_2;

/*** Estructuras internas de la ruta de datos de coma flotante */

/* Todas las estaciones están en el mismo array, pero se puede usar
   RS, LB o SB */
estacion_t RS[MAX_ESTACIONES];
#define LB RS
#define SB RS

int RB_inicio, RB_fin, RB_long;
reorder_t RB[MAX_REORDER];

bus_comun_t BUS;

operador_t Op[MAX_OPERADORES];

/*** Estructuras para el predictor */

/* BTB */
entrada_btb_t BTB[MAX_BUFFER_PREDIC];
word n_bits_pred_local = 1;
char subtipo_pred_local = 's';
word n_bits_branch_mask_local = 0;
word branch_mask_local;

/* Global */
word Global[pwrtwo(MAX_BITS_BRANCH_MASK_GLOBAL)];
word n_bits_pred_global = 1;
char subtipo_pred_global = 's';
word n_bits_branch_mask_global = 1;
word branch_mask_global;

/* Hibrido */
word n_bits_pred_hybrid = 1;

/* Predictor perfecto */
FILE *f_trace = NULL;

/* Ultima traza leida del fichero */
dword PC1 = -1;
ciclo_t orden1;
ciclo_t ciclo1 = 0;
dword dest1;
boolean cond1;

/* Seguir leyendo del fichero */
boolean leer_sig_traza = SI;

/* Predictor clarividente: El salto no está en la tabla y salta */
boolean parar_clarividente = NO;
ciclo_t orden_clarividente;

dword PC_inicial = 0;

/* Consola generada en cada ciclo */
char CONSOLA[MAX_TEXTO_CONSOLA];

/* Log generado en cada ciclo */
char LOG[MAX_TEXTO_LOG];

/* Estado del simulador */
estado_simulador_t estado_previo;

/* Estructura del programa completo */
programa_t programa;

#endif

/***************************************************************************/
/***************************************************************************/

#ifndef main_C  /* Publico (excepto main.c) */

extern boolean init_ireg,
        init_fpreg,
        init_btb;

extern boolean hay_double,
        hay_float,
        hay_dword,
        hay_word,
        hay_half,
        hay_byte,
        hay_char;

/*** Opciones de Ejecución */

extern boolean f_etiquetas;
extern boolean f_macros;

/*** Variables internas  */

extern ciclo_t Ciclo;
extern boolean terminando;
extern boolean SIG_SYSCALL;

extern char nombre_fich[32*MAX_FICHEROS];
extern dword PC_ISS;
extern int RB_cab;
extern estadisticas_t estat;
extern utilizacion_t max_util;
extern utilizacion_t aver_util;
extern boolean IFstall;

extern dep_mem_t dep_mem;
extern tipo_predictor_t tipo_predictor;

extern enum {
    TRAZA, FINAL
} fase_ejecucion;

extern boolean final;

/*** Estructuras visibles al programador */

//extern reg_fp_t Rfp[_TAM_REGISTROS];
extern reg_t Rfp[_TAM_REGISTROS];
extern tipo_t Rfp_tipo[_TAM_REGISTROS];
extern boolean Rfp_dirty[_TAM_REGISTROS];
//reg_int_t Rint[_TAM_REGISTROS + 1]; /* El último simula el 'fpsr' */
extern reg_t Rint[_TAM_REGISTROS + 1]; /* El último simula el 'fpsr' */
extern tipo_t Rint_tipo[_TAM_REGISTROS + 1];
extern boolean Rint_dirty[_TAM_REGISTROS + 1];

//extern reg_int_t Rint[_TAM_REGISTROS + 1]; /* El último simula el 'fpsr' */
extern reg_t Rint[_TAM_REGISTROS + 1]; /* El último simula el 'fpsr' */

/*** Registros inter-etapa para la segmentación */

/* Representan a los registros inter-etapa que contienen los valores
   para el siguiente ciclo, los valores futuros */
/* DEBEN ESTAR SIEMPRE A LA IZQUIERDA DE UNA ASIGNACION */

extern reg_if_iss_t IF_ISS_1;
extern reg_control_t Control_1;

/* Representan a los registros inter-etapa que contienen los valores
   del presente ciclo. Serán estables hasta que se vuelvan escribir al
   final del ciclo con los nuevos valores */
/* DEBEN ESTAR SIEMPRE A LA DERECHA DE UNA ASIGNACION O EN LAS
   COMPARACIONES */
extern reg_if_iss_t IF_ISS_2;
extern reg_control_t Control_2;

/*** Estructuras internas de la ruta de datos de coma flotante */

/* Todas las estaciones están en el mismo array, pero se puede usar
   RS, LB o SB */
extern estacion_t RS[MAX_ESTACIONES];
#define LB RS
#define SB RS

extern int RB_inicio, RB_fin, RB_long;
extern reorder_t RB[MAX_REORDER];

extern bus_comun_t BUS;

extern operador_t Op[MAX_OPERADORES];

/*** Estructuras para el predictor */

/* BTB */
extern entrada_btb_t BTB[MAX_BUFFER_PREDIC];
extern word n_bits_pred_local;
extern char subtipo_pred_local;
extern word n_bits_branch_mask_local;
extern word branch_mask_local;


/* Global */
extern word Global[pwrtwo(MAX_BITS_BRANCH_MASK_GLOBAL)];
extern word n_bits_pred_global;
extern char subtipo_pred_global;

extern word n_bits_branch_mask_global;
extern word branch_mask_global;

/* Hibrido */
extern word n_bits_pred_hybrid;
extern char subtipo_pred_hybrid;



extern FILE* f_trace;

extern dword PC1;
extern ciclo_t orden1, ciclo1;
extern dword dest1;
extern boolean cond1;

//extern ciclo_t orden2;
//extern dword dest2;
//extern boolean pred2;

extern boolean leer_sig_traza;

extern boolean parar_clarividente;
extern ciclo_t orden_clarividente;

/* PC inicial */
extern dword PC_inicial;


/* Consola generada en cada ciclo */
extern char CONSOLA[MAX_TEXTO_CONSOLA];

/* Log generado en cada ciclo */
extern char LOG[MAX_TEXTO_LOG];

/* Estado del simulador */
extern estado_simulador_t estado_previo;

/* Estructura del programa completo */
extern programa_t programa;

#endif

#endif /* Include main.h */


