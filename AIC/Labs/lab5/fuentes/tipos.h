/*********************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politecnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *	        Pedro López (plopez@gap.upv.es)
 *
 * File: tipos.h
 *
 * Description:
 *    Contiene las definiciones de tipos comunes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *********************************************************************/

#ifndef tipos_H
#define tipos_H

/***** Includes ***********************************************/

#include <sys/types.h>
#include <stdint.h>
#include <inttypes.h>

/***** Constants **********************************************/



/*** Excepciones */

#define EXC_NONE                0

#define EXC_OUT_MEM_INST        1
#define EXC_OUT_MEM_DAT         2
#define EXC_UNALIGN_MEM_INST    3
#define EXC_UNALIGN_MEM_DAT     4

#define EXC_USER                32
#define EXC_TRAP(t)  (EXC_USER+(t))
#define EXC_SYSCALL            255
#define excepcion_fatal(i) ((i) == EXC_OUT_MEM_INST  || (i) == EXC_OUT_MEM_DAT  || (i) == EXC_UNALIGN_MEM_INST || (i) == EXC_UNALIGN_MEM_DAT || (i) == EXC_TRAP(0))
#define excepcion_syscall(i) ((i) == EXC_SYSCALL)

/*** Registros MIPS */
#define _zero 0
#define _v0   2
#define _v1   3
#define _a0   4
#define _a1   5
#define _a2   6
#define _a3   7
#define _t0  8
#define _t1  9
#define _t2  10
#define _t3  11
#define _t4  12
#define _t5  13
#define _t6  14
#define _t7  15
#define _t8  24
#define _t9  25
#define _s0  16
#define _s1  17
#define _s2  18
#define _s3  19
#define _s4  20
#define _s5  21
#define _s6  22
#define _s7  23
#define _k1      26
#define _k2      27
#define _gp  28
#define _sp  29
#define _fp  30
#define _ra  31

/* Registros para las .ps */
#define Lo 0
#define Hi 1


/***** Types **************************************************/

//typedef unsigned char ubyte; /* Esta definicion se le atraganta al cproto. Alternativa: */
#define ubyte unsigned char /* Un byte: 8 bits */
typedef int8_t byte; /* Un byte: 8 bits */
typedef int16_t half; /* Media palabra: 16 bits */
typedef int32_t word; /* Una palabra: 32 bits */
typedef int64_t dword; /* Una doble palabra: 64 bits */

typedef uint32_t ciclo_t;


#define MAX_BYTE (0xFF)
#define MAX_HALF (0xFFFF)
#define MAX_WORD (0xFFFFFFFF)
#define MAX_DWORD (0xFFFFFFFFFFFFFFFF)

#define F_BYTE "%" PRId8
#define F_HALF "%" PRId16
#define F_WORD "%" PRId32
#define F_DWORD "%" PRId64

#define F_CICLO "%" PRIu32
#define F_LCICLO "%03" PRIu32

typedef enum {
    NO = 0,
    SI = 1
} boolean; /* Valor lógico */

typedef enum {
    t_ninguno,
    t_char,
    t_byte,
    t_half,
    t_word,
    t_dword,
    t_float,
    t_float_ps,
    t_double,

    t_code // Representa un instrucción
} tipo_t; /* Tipo de dato utilizado */

typedef union {
    //byte int_b;
    //half int_h;
    //word int_w;
    dword int_d;
    float fp_s;
    float fp_ps[2];
    double fp_d;
} valor_t; /* Dato utilizado */

typedef enum {
    l_bypassing,
    l_forwarding
} dep_mem_t;

/*** Estructuras espécificas ****/

typedef int codop_t; /* Código de operación */

typedef enum {
    FormatoR,
    FormatoI,
    FormatoJ
} formato_t; /* El formato J no esta implementado */

typedef enum {
    PENDIENTE,
    PREPARADA, /* Este estado sólo lo utiliza la memoria de datos */
    EN_CURSO,
    FINALIZADA
} estado_oper_t; /* Estado de una operación en una ER */

typedef int marca_t; /* Tipo marca/código */

typedef enum {
    NO_SALTA,
    NO_SALTA_UN_FALLO,
    SALTA_UN_FALLO,
    SALTA
} estado_predic_t; /* Estado del predictor de 2 bits */

typedef enum {
    OPER_ENTEROS,
    OPER_SUMREST,
    OPER_MULTDIV,
    OPER_MEMDATOS,
    OPER_DIRECCIONES,
    NUM_TIPOS_OPERADORES
} tipo_operador_t;

typedef enum {
    HYBRID_n_BIT_S,
    GLOBAL_n_BIT_S,
    GLOBAL_n_BIT_H,
    BTB_n_BIT_S,
    BTB_n_BIT_H,
    BTB_1_BIT,
    BTB_2_BIT_H,
    BTB_2_BIT_S,
    PERFECTO,
    CLARIVIDENTE,
    PREDICT_NOT_TAKEN
} tipo_predictor_t; /* Tipos de predictor */

typedef enum {
    EXCEPCION,
    RESULT,
    CONDICION,
    DIRECCION
} codigo_bus_t;

/*** Prediccion ***/
typedef struct {
    int BHR_global; /* BHR para la historia global */
    int BHR_local; /* BHR para la historia local */
    int pred_global; /* Estado pred global */
    int pred_local; /* Estado pred local */
    dword dir_destino; /* Direccion destino predicha */
} prediccion_t;

/*** Branch Target Buffer ******/
#define pwrtwo(x) (1 << (x))
#define MAX_BITS_BRANCH_MASK_LOCAL 4
#define MAX_BITS_BRANCH_MASK_GLOBAL 4

typedef struct {
    dword PC; /* Dirección de la instrucción de salto */
    estado_predic_t estado; /* Estado del predictor */
    dword destino; /* Dirección de destino */

    /* Predictor correlacionado */
    word v_estado[pwrtwo(MAX_BITS_BRANCH_MASK_LOCAL)]; /* Vector de Estado del predictor */

    /* Predictor híbrido */
    word estado_selector; /* Estado del selector local/global */

    ciclo_t orden; /* Antiguedad de la última consulta */
    /* ciclo_t ciclo; *//* Antiguedad de la última consulta */
} entrada_btb_t;

/*** Formato de instruccion ****/

typedef struct {
    codop_t codop;
    formato_t tipo;

    int Rfuente1, /* Índice del registro fuente 1 */
            Rfuente2, /* Índice del registro fuente 2 */
            Rdestino; /* Índice del registro destino */
    half inmediato; /* Valor Inmediato */

    dword PC; /* Posición de memoria de la instrucción */
    char etiqueta[128]; /* Posible etiqueta */
} instruccion_t;

/*** Banco de registros ********/

typedef struct {
    valor_t valor; /* Valor del registro */
    marca_t rob; /* Marca del registro */
} reg_t;

/*** Registros inter-etapas ************************************************/

typedef struct { /* IF/ID */
    instruccion_t IR; /* Instrucción */

    dword PC; /* Posición de memoria de la instrucción */
    ciclo_t orden; /* Orden de la instrucción */

    boolean prediccion; /* Indica se se ha predicho salto o no */
    prediccion_t pred_data; /* Datos auxiliares  */


    int excepcion; /* Indica si se ha producido alguna
                                           excepción y cual */
    boolean cancelar; /* Índica que esta instrucción se debe descartar en ISSUE */
    boolean ignorar; /* Índica que esta instrucción ya se ha lanzado anteriormente */
} reg_if_iss_t;

typedef struct {
    dword NPC; /* Siguiente PC */
    boolean Parar; /* Instrucción bloqueada en ISS */
    boolean Cancelar; /* Ejecución cancelada */
    boolean Excepcion; /* Hay una excepción en la unidad segmentada. Dejar de buscar */
    ciclo_t orden; /* Mantiene el orden de las instrucciones */
} reg_control_t;

/*** Estructuras internas de la ruta de datos de coma flotante *************/

/*** Estaciones de reserva ******/

typedef struct {
    boolean ocupado; /* Bit de ocupado */
    codop_t OP; /* Código de operación a realizar */

    marca_t Q1; /* Marca del primer operando. ALU */
    valor_t V1; /* Valor del primer operando. ALU */

    marca_t Q2; /* Marca del segundo operando. ALU y SB */
    valor_t V2; /* Valor del segundo operando. ALU y SB */

    dword direccion; /* Dirección de acceso a memoria. LB y SB. Enteras */
    dword desplazamiento; /* Desplazamiento en el acceso a memoria. LB y SB */
    char etiqueta[64]; /* Idem, pero etiqueta, LB y SB */

    boolean confirm; /* Indica si la operación de memoria
				   ha sido confirmada. SB */

    estado_oper_t estado; /* Indica si la operación está pendiente, preparada, en curso o si
                           *  ya ha terminado y el resultado está en la RS */
    unsigned int operador; /* Indica qué operador la está atendiendo mientras está en el estado EN_CURSO */
    valor_t resultado; /* Resultado de la operación */
    boolean condicion; /* En las RS enteras, para la condicion de salto */

    marca_t rob; /* Indica para quien es la operación.  */

    int excepcion; /* Indica si se ha producido alguna excepción
                                   al ejecutar está operación */
    //dword PC; /* Posición de memoria de la instrucción */
    codigo_bus_t control;
    ciclo_t orden; /* Orden de la instrucción */
} estacion_t;

/*** Reorder buffer *************/

typedef struct {
    boolean ocupado; /* Bit de ocupado */
    codop_t OP; /* Código de operación a realizar */

    boolean completado; /* Estado de la operación */

    dword dest; /* Registro destino, TE */
    // ^ podria ser un byte!!!
    valor_t valor; /* Resultado de la operación */

    dword direccion; /* Direccion de salto */
    boolean condicion; /* Resultado de la condicion */
    boolean prediccion; /* Indica se se ha predicho salto o no */

    prediccion_t pred_data; /* Datos auxiliares  */

    int excepcion; /* Indica si se ha producido alguna excepción
                                   al ejecutar está instrucción */
    dword PC; /* Posición de memoria de la instrucción */
    codigo_bus_t control;
    ciclo_t orden; /* Orden de la instrucción */
} reorder_t;

/*** Operador *******************/

typedef struct {
    boolean ocupado; /* Bit de ocupado */
    tipo_operador_t tipo; /* Tipo de operador */
    int estacion; /* Estación de reserva en uso */
    marca_t codigo; /* Código del reorder buffer */
    ciclo_t ciclo; /* Ciclo actual de la operación */
    ciclo_t Teval; /* Tiempo de evaluación */

    ciclo_t orden; /* Orden de la instrucción */
} operador_t;

/*** Bus común ******************/

typedef struct {
    boolean ocupado; /* Linea de ocupado */
    marca_t codigo; /* Lineas para los códigos */
    valor_t valor; /* Lineas de datos: resultado, direccion o excepcion */
    boolean condicion;
    codigo_bus_t control;
    //int excepcion; /* Indica si se ha producido alguna excepción */
} bus_comun_t;

/*** Estadisticas ***/

typedef struct {
    long ciclos;
    long instrucciones;
    long flops;
    long saltos_accedidos;
    long saltos_encontrados;
    long saltos_ejecutados;
    long saltos_acertados;
} estadisticas_t;

typedef struct {
    int RS_ENTEROS;
    int RS_SUMREST;
    int RS_MULTDIV;
    int BUFFER_CARGA;
    int BUFFER_ALMACEN;
    int OP_ENTEROS;
    int OP_SUMREST;
    int OP_MULTDIV;
    int OP_MEMDATOS;
    int OP_DIRECCIONES;
    int REORDER;
    int BTB;
} utilizacion_t;

#endif /* end tipos.h */
