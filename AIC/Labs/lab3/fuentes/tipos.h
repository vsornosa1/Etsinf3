/*********************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politecnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *	   Pedro López (plopez@gap.upv.es)
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



/*** Estructuras especificas ****/

typedef ubyte codop_t; /* Código de operación */

typedef enum {
    FormatoR,
    FormatoI,
    FormatoJ
} formato_t; /* Formatos de instrucción */

typedef enum {
    parada,
    cortocircuito,
    ninguno
} riesgos_d_t; /* Solución riesgos de datos */

typedef enum {
    stall3,
    stall2,
    stall1,
    pnt3,
    pnt2,
    pnt1,
    ds3,
    ds2,
    ds1
} riesgos_c_t; /* Solución riesgos de control */

typedef enum {
    html,
    html_final,
    // res_ciclo, // No funciona en esta versión
    res_tiempo,
    res_final
} sal_t; /* Opciones para la salida de resultados */

/* Esta estructura no es necesaria. La definimos por compatibilidad con otros módulos */
typedef union {
    byte int_b;
    half int_h;
    word int_w;
    dword int_d;
    float fp_s;
    float fp_ps[2];
    double fp_d;
} valor_t; /* Dato utilizado */

/*** Branch Target Buffer ******/

typedef enum {
    NO_SALTA,
    NO_SALTA_UN_FALLO,
    SALTA_UN_FALLO,
    SALTA
} estado_predic_t; /* Estado del predictor de 2 bits */

typedef struct {
    dword PC; /* Dirección de la instrucción de salto */
    estado_predic_t estado; /* Estado del predictor */
    dword destino; /* Dirección de destino */

    ciclo_t orden; /* Antiguedad de la última consulta */
    ciclo_t ciclo; /* Antiguedad de la última consulta */
} entrada_btb_t;

/*** Formato de instruccion ****/

typedef struct {
    codop_t codop;
    formato_t tipo;

    ubyte Rfuente1, /* Índice del registro fuente 1 */
    Rfuente2; /* Índice del registro fuente 2 */
    ubyte Rdestino; /* Índice del registro destino */
    half inmediato; /* Valor Inmediato */

    char etiqueta[128]; /* Posible etiqueta */
} instruccion_t;

/*** Banco de registros ********/

typedef struct {
    dword valor; /* Valor del registro */
} reg_int_t;

typedef struct {
    valor_t valor; /* Valor del registro */
    ciclo_t busy; /* Ciclo en el que se liberará. 0 = está libre */
} reg_fp_t;


/*** Registros inter-etapas ************************************************/

/*** Registros intermedios *******************/

typedef struct {
    instruccion_t IR; /* IR */
    dword NPC; /* PC+4 */

    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} IF_ID_t;

typedef struct {
    instruccion_t IR; /* IR */
    dword NPC; /* PC+4 */
    dword Ra; /* Valores de los registros*/
    dword Rb;
    dword Imm; /* Inmediato con signo extendido */

    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} ID_EX_t;

typedef struct {
    instruccion_t IR; /* IR */
    dword ALUout; /* Resultado */
    dword data; /* Dato a escribir */
    boolean cond; /* Resultado condicion de salto */

    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} EX_MEM_t;

typedef struct {
    instruccion_t IR; /* IR */
    dword ALUout; /* Resultado */
    dword MEMout; /* Resultado */

    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} MEM_WB_t;

typedef struct {
    instruccion_t IR; /* IR */
    dword NPC; /* PC+4 */
    valor_t Fa; /* Valores de los registros*/
    valor_t Fb;
    dword Ra; /* Valores de los registros*/
    dword Rb;
    dword Imm; /* Inmediato con signo extendido */
    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} ID_FP_t;

typedef struct {
    instruccion_t IR; /* IR */
    dword intALUout; /* Resultado */
    valor_t fpALUout; /* Resultado */
    valor_t Fa; /* Valores de los registros*/
    valor_t Fb;
    dword address; /* LOAD y STORE */
    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} FP_FP_t;

typedef struct {
    instruccion_t IR; /* IR */
    valor_t fpALUout; /* Resultado */

    dword iPC;
    /* Información de visualización */
    ciclo_t orden;
} FP_WB_t;

/*** Señales intermedios *******************/

typedef struct {
    dword compin1, compin2;
    dword cond_out;
    dword dir;
} ID_t;

typedef struct {
    dword aluin1, aluin2, aluout;
    dword compin1, compin2;
    dword cond_out;
    dword data;
    word muxALU_sup, muxALU_inf;
} EX_t;

typedef struct {
    dword dir;
    dword data;
} MEM_t;

typedef struct {
    dword data;
} WB_t;

/*** Estadisticas ***/

typedef struct {
    long ciclos;
    long instrucciones;
    long enteras;
    long multiciclo;
    long flops;

} estadisticas_t;

/***** Macros *************************************************/



#define latencia_salto1(i) ((i) == ds1 || (i) == pnt1 || (i) == stall1)
#define latencia_salto2(i) ((i) == ds2 || (i) == pnt2 || (i) == stall2)
#define latencia_salto3(i) ((i) == ds3 || (i) == pnt3 || (i) == stall3)


#endif /* end tipos.h */
