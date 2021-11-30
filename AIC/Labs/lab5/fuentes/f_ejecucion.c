/**************************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *
 * File: f_ejecucion.c
 *
 * Description:
 *    Contiene la fase de ejecución de los operadores multiciclo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *************************************************************************/

#define f_ejecucion_C

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "main.h"
#include "instrucciones.h"
#include "prediccion.h"
#include "presentacion.h"
#include "memoria.h"

#include "tipos.h"
#include "depurar.h"

#ifndef CPROTO

#include "f_ejecucion_api.h"

#endif

/*** Macros locales ********************************************/

#define F_FASE(f) f F_CICLO

/***************************************************************
 *
 * Func: fase_EX_1
 *
 * Desc: Implementa la fase de ejecucion de los operadores
 * multiciclo. Comienza la ejecución de las operaciones que tienen
 * todos los operandos, y el operador libre.
 *
 **************************************************************/

void fase_EX_1() {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    unsigned int oper;
    unsigned int oper_iniciadas[NUM_TIPOS_OPERADORES];

    boolean iniciada;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    for (oper = 0; oper < NUM_TIPOS_OPERADORES; oper++) {
        oper_iniciadas[oper] = 0;
    }

    for (oper = 0; oper < NUM_OPERADORES; oper++) {
        iniciada = NO;
        if (!Op[oper].ocupado) { /* Si el operador está libre */
            switch (Op[oper].tipo) {
                case OPER_ENTEROS:
                    if (oper_iniciadas[OPER_ENTEROS] >= NUM_INICIO_ENTEROS)
                        break;
                    iniciada = fase_EX_1_ENTEROS(oper);
                    break;
                case OPER_SUMREST:
                    if (oper_iniciadas[OPER_SUMREST] >= NUM_INICIO_SUMREST)
                        break;
                    iniciada = fase_EX_1_SUMREST(oper);
                    break;
                case OPER_MULTDIV:
                    if (oper_iniciadas[OPER_MULTDIV] >= NUM_INICIO_MULTDIV)
                        break;
                    iniciada = fase_EX_1_MULTDIV(oper);
                    break;
                case OPER_DIRECCIONES:
                    if (oper_iniciadas[OPER_DIRECCIONES] >= NUM_INICIO_DIRECCIONES)
                        break;
                    iniciada = fase_EX_1_DIRECCIONES(oper);
                    break;
                case OPER_MEMDATOS:
                    if (oper_iniciadas[OPER_MEMDATOS] >= NUM_INICIO_MEMDATOS)
                        break;
                    iniciada = fase_EX_1_MEMDATOS(oper);
                    break;
                default:
                    fprintf(stderr, "ERROR (%s:%d): Operador no implementado\n",
                            __FILE__, __LINE__);
                    exit(1);
                    break;
            }

            if (iniciada)
                oper_iniciadas[Op[oper].tipo]++;
        }
    }
}

boolean fase_EX_1_ENTEROS(unsigned int oper) {

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    marca_t s, r;

    ciclo_t siguiente;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    s = TAM_ESTACIONES;
    siguiente = MAX_ORDEN;

    /*** Busca la instrucción más antigua que este preparada */

    for (r = INICIO_RS_ENTEROS;
            r <= FIN_RS_ENTEROS; r++) {
        if (!RS[r].ocupado || RS[r].estado != PENDIENTE)
            continue;

        if (RS[r].Q1 == MARCA_NULA &&
                RS[r].Q2 == MARCA_NULA &&
                /* Tiene todos los operandos disponibles */
                RS[r].orden < siguiente) {
            siguiente = RS[r].orden;
            s = r;
        } /* endif */
    } /* endfor */

    /*** Entra en ejecución la operación más antigua, si existe */

    if (s < TAM_ESTACIONES) {
        Op[oper].ocupado = SI;
        Op[oper].ciclo = 0;
        Op[oper].codigo = RS[s].rob;
        Op[oper].estacion = s;
        RS[s].estado = EN_CURSO;
        RS[s].operador = oper;

        return SI;
    } /* endif */

    return NO;

} /* end fase_EX_1_ENTEROS */

boolean fase_EX_1_SUMREST(unsigned int oper) {

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    marca_t s, r;

    ciclo_t siguiente;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    s = TAM_ESTACIONES;
    siguiente = MAX_ORDEN;

    /*** Busca la instrucción más antigua que este preparada */

    for (r = INICIO_RS_SUMREST;
            r <= FIN_RS_SUMREST; r++) {
        if (!RS[r].ocupado || RS[r].estado != PENDIENTE)
            continue;

        if (RS[r].Q1 == MARCA_NULA &&
                RS[r].Q2 == MARCA_NULA &&
                /* Tiene todos los operandos disponibles */
                RS[r].orden < siguiente) {
            siguiente = RS[r].orden;
            s = r;
        } /* endif */
    } /* endfor */

    /*** Entra en ejecución la operación más antigua, si existe */

    if (s < TAM_ESTACIONES) {
        Op[oper].ocupado = SI;
        Op[oper].ciclo = 0;
        Op[oper].codigo = RS[s].rob;
        Op[oper].estacion = s;
        RS[s].estado = EN_CURSO;
        RS[s].operador = oper;

        return SI;
    } /* endif */

    return NO;

} /* end fase_EX_1_SUMREST */

boolean fase_EX_1_MULTDIV(unsigned int oper) {

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    marca_t s, r;

    ciclo_t siguiente;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    s = TAM_ESTACIONES;
    siguiente = MAX_ORDEN;

    /*** Busca la instrucción más antigua que este preparada */

    for (r = INICIO_RS_MULTDIV;
            r <= FIN_RS_MULTDIV; r++) {
        if (!RS[r].ocupado || RS[r].estado != PENDIENTE)
            continue;

        if (RS[r].Q1 == MARCA_NULA &&
                RS[r].Q2 == MARCA_NULA &&
                /* Tiene todos los operandos disponibles */
                RS[r].orden < siguiente) {

            siguiente = RS[r].orden;
            s = r;
        } /* endif */
    } /* endfor */

    /*** Entra en ejecución la operación más antigua, si existe */

    if (s < TAM_ESTACIONES) {
        Op[oper].ocupado = SI;
        Op[oper].ciclo = 0;
        Op[oper].codigo = RS[s].rob;
        Op[oper].estacion = s;
        RS[s].estado = EN_CURSO;
        RS[s].operador = oper;

        return SI;
    } /* endif */

    return NO;

} /* end fase_EX_1_MULTDIV */

boolean fase_EX_1_DIRECCIONES(unsigned int oper) {

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    marca_t s, r;

    ciclo_t siguiente;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    s = TAM_ESTACIONES;
    siguiente = MAX_ORDEN;

    /*** Busca la instrucción más antigua que este preparada */

    for (r = INICIO_BUFFERS;
            r <= FIN_BUFFERS; r++) {
        if (!RS[r].ocupado || RS[r].estado != PENDIENTE)
            continue;

        if (RS[r].Q1 == MARCA_NULA &&
                /* Tiene todos los operandos disponibles */
                RS[r].orden < siguiente) {
            siguiente = RS[r].orden;
            s = r;
        } /* endif */
    } /* endfor */

    /*** Entra en ejecución la operación más antigua, si existe */

    if (s < TAM_ESTACIONES) {
        Op[oper].ocupado = SI;
        Op[oper].ciclo = 0;
        Op[oper].codigo = RS[s].rob; // Se utiliza para las excepciones
        Op[oper].estacion = s;
        RS[s].estado = EN_CURSO;
        RS[s].operador = oper;
        bzero(&RS[s].resultado, sizeof (valor_t));

        return SI;
    } /* endif */

    return NO;

} /* end fase_EX_1_DIRECCIONES */

boolean fase_EX_1_MEMDATOS(unsigned int oper) {

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    marca_t s, r, t;

    ciclo_t siguiente;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    s = TAM_ESTACIONES;
    siguiente = MAX_ORDEN;

    /*** Busca la instrucción más antigua que este preparada */

    for (r = INICIO_BUFFER_CARGA;
            r <= FIN_BUFFER_CARGA; r++) {
        if (!LB[r].ocupado || LB[r].estado != PREPARADA)
            continue;

        /* Comprueba que no haya ninguna escritura pendiente en la misma
         * dirección */
        /* No comprueba si las escrituras sin la dirección
           calculada porque como las direcciones se calculan en
           orden esa escritura será posterior a la lectura. REVISAR
         */
        for (t = INICIO_BUFFER_ALMACEN;
                t <= FIN_BUFFER_ALMACEN; t++) {
            if (SB[t].ocupado && SB[t].estado != PENDIENTE &&
                    SB[t].direccion == LB[r].direccion &&
                    SB[t].orden < LB[r].orden) {
                if (dep_mem == l_forwarding &&
                        SB[t].Q2 == MARCA_NULA) {
                    /* Load forwarding */
                    LB[r].estado = FINALIZADA;
                    LB[r].resultado = SB[t].V2;
                } /* endif */
                break;
            } /* endif */
        } /* endfor */

        if (t > FIN_BUFFER_ALMACEN) { /* No hay conflictos con ninguna escritura */
            if (LB[r].orden < siguiente) {
                siguiente = LB[r].orden;
                s = r;
            } /* endif */
        }
    } /* endfor */

    for (r = INICIO_BUFFER_ALMACEN;
            r <= FIN_BUFFER_ALMACEN; r++) {
        if (!SB[r].ocupado || !SB[r].confirm || SB[r].estado == EN_CURSO)
            continue;

        if (SB[r].Q1 == MARCA_NULA &&
                SB[r].orden < siguiente) {
            siguiente = SB[r].orden;
            s = r;
        } /* endif */
    } /* endfor */

    /*** Entra en ejecución la operación más antigua, si existe */

    if (s < TAM_ESTACIONES) {
        Op[oper].ocupado = SI;
        Op[oper].ciclo = 0;
        Op[oper].codigo = RS[s].rob; /* RS = LB = SB */
        Op[oper].estacion = s;
        RS[s].estado = EN_CURSO;
        RS[s].operador = oper;

        return SI;
    } /* endif */

    return NO;

} /* end fase_EX_1_MEMDATOS */

/***************************************************************
 *
 * Func: fase_EX_2
 *
 * Desc: Implementa la fase de ejecucion de los operadores
 * multiciclo. Avanza la ejecución de las operaciones que se están
 * ejecutando en cada operador.
 *
 **************************************************************/

void fase_EX_2() {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    unsigned int oper;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    for (oper = 0; oper < NUM_OPERADORES; oper++) {
        if (Op[oper].ocupado) { /* Si el operador está ocupado */
            switch (Op[oper].tipo) {
                case OPER_ENTEROS:
                    fase_EX_2_ENTEROS(oper);
                    break;
                case OPER_SUMREST:
                    fase_EX_2_SUMREST(oper);
                    break;
                case OPER_MULTDIV:
                    fase_EX_2_MULTDIV(oper);
                    break;
                case OPER_DIRECCIONES:
                    fase_EX_2_DIRECCIONES(oper);
                    break;
                case OPER_MEMDATOS:
                    fase_EX_2_MEMDATOS(oper);
                    break;
                default:
                    fprintf(stderr, "ERROR (%s:%d): Operador no implementado\n",
                            __FILE__, __LINE__);
                    exit(1);
                    break;
            }
        }
    }
}

void fase_EX_2_ENTEROS(unsigned int oper) {

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    marca_t s;
    uint64_t aux1, aux2;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    if (Op[oper].ciclo < Op[oper].Teval)
        Op[oper].ciclo++; /* Si quedan ciclos por realizar */

    s = Op[oper].estacion;

    /*** VISUALIZACIÓN ****/
    sprintf(p_str, F_FASE("E"), Op[oper].ciclo);
    muestra_fase(p_str, RS[s].orden);
    /**********************/

    if (Op[oper].ciclo == Op[oper].Teval) {
        RS[s].estado = FINALIZADA;
        Op[oper].ocupado = NO;

        /* Chanchullo. Inicializamos el valor de la direccion de salto a la 
        calculada en Issue. La tenemos en el ROB */
        //RS[s].resultado = RB[RS[s].rob].valor;
        RS[s].condicion = NO; // Por defecto
        RS[s].resultado.int_d = 0; // Por defecto

        /* Cálculo del resultado */
        switch (RS[s].OP) {
            case OP_DADD:
            case OP_DADDI:
                RS[s].resultado.int_d = RS[s].V1.int_d + RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case OP_DSUB:
            case OP_DSUBI:
                RS[s].resultado.int_d = RS[s].V1.int_d - RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case OP_DMUL:
                RS[s].resultado.int_d = RS[s].V1.int_d * RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case OP_DDIV:
                RS[s].resultado.int_d = RS[s].V1.int_d / RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case OP_LUI:
                RS[s].resultado.int_d = RS[s].V2.int_d << 16;
                RS[s].control = RESULT;
                break;
            case OP_AND:
            case OP_ANDI:
                RS[s].resultado.int_d = RS[s].V1.int_d & RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case OP_OR:
            case OP_ORI:
                RS[s].resultado.int_d = RS[s].V1.int_d | RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case OP_XOR:
            case OP_XORI:
                RS[s].resultado.int_d = RS[s].V1.int_d ^ RS[s].V2.int_d;
                RS[s].control = RESULT;
                break;
            case OP_DSRA:
            case OP_DSRAI:
                RS[s].resultado.int_d = RS[s].V1.int_d >> (RS[s].V2.int_d & 0x1f);
                RS[s].control = RESULT;
                break;
            case OP_DSLL:
            case OP_DSLLI:
                RS[s].resultado.int_d = RS[s].V1.int_d << (RS[s].V2.int_d & 0x1f);
                RS[s].control = RESULT;
                break;
            case OP_DSRL:
            case OP_DSRLI:
                aux1 = (uint64_t) RS[s].V1.int_d;
                aux2 = (uint64_t) RS[s].V2.int_d & 0x1f;
                RS[s].resultado.int_d = aux1 >> aux2;
                RS[s].control = RESULT;
                break;
            case OP_SEQ:
            case OP_SEQI:
                RS[s].resultado.int_d = (RS[s].V1.int_d == RS[s].V2.int_d);
                RS[s].control = RESULT;
                break;
            case OP_SNE:
            case OP_SNEI:
                RS[s].resultado.int_d = (RS[s].V1.int_d != RS[s].V2.int_d);
                RS[s].control = RESULT;
                break;
            case OP_SGT:
            case OP_SGTI:
                RS[s].resultado.int_d = (RS[s].V1.int_d > RS[s].V2.int_d);
                RS[s].control = RESULT;
                break;
            case OP_SLT:
            case OP_SLTI:
                RS[s].resultado.int_d = (RS[s].V1.int_d < RS[s].V2.int_d);
                RS[s].control = RESULT;
                break;
            case OP_SGE:
            case OP_SGEI:
                RS[s].resultado.int_d = (RS[s].V1.int_d >= RS[s].V2.int_d);
                RS[s].control = RESULT;
                break;
            case OP_SLE:
            case OP_SLEI:
                RS[s].resultado.int_d = (RS[s].V1.int_d <= RS[s].V2.int_d);
                RS[s].control = RESULT;
                break;

            case OP_BC1F:
            case OP_BEQZ:
            case OP_BEQ:
                //RS[s].direccion = RS[s].PC + 4 + RS[s].desplazamiento;
                RS[s].condicion = (RS[s].V1.int_d == RS[s].V2.int_d);
                RS[s].control = CONDICION;
                break;
            case OP_BC1T:
            case OP_BNEZ:
            case OP_BNE:
                //RS[s].direccion = RS[s].PC + 4 + RS[s].desplazamiento;
                RS[s].condicion = (RS[s].V1.int_d != RS[s].V2.int_d);
                RS[s].control = CONDICION;
                break;
            case OP_J:
                // No pasa por EX
                //RS[s].direccion = RS[s].PC + 4 + RS[s].desplazamiento;
                //RS[s].condicion = 1; // Salta
                break;
            case OP_JAL:
                // No pasa por EX
                //RS[s].direccion = RS[s].PC + 4 + RS[s].desplazamiento;
                //RS[s].resultado.int_d = RS[s].PC + 4; // R31
                //RS[s].condicion = 1; // Salta
                break;
            case OP_JR:
                RS[s].resultado = RS[s].V1;
                //RS[s].condicion = 1; // Salta
                RS[s].condicion = RS[s].V1.int_d != RS[s].V2.int_d; // Comprueba prediccion
                RS[s].control = DIRECCION;
                break;
            case OP_JALR:
                RS[s].resultado = RS[s].V1;
                //RS[s].resultado.int_d = RS[s].PC + 4; // R31
                //RS[s].condicion = 1; // Salta
                RS[s].condicion = RS[s].V1.int_d != RS[s].V2.int_d; // Comprueba prediccion
                RS[s].control = DIRECCION;
                break;
            case OP_MOVN:
                RS[s].condicion = ((RS[s].V2.int_d != 0) ? SI : NO);
                RS[s].resultado.int_d = RS[s].V1.int_d;
                RS[s].control = RESULT;
                break;
            case OP_MOVZ:
                RS[s].condicion = ((RS[s].V2.int_d == 0) ? SI : NO);
                RS[s].resultado.int_d = RS[s].V1.int_d;
                RS[s].control = RESULT;
                break;
            default:
                fprintf(stderr, "ERROR (%s:%d:%d): Operacion no implementada\n",
                        __FILE__, __LINE__, RS[s].OP);
                exit(1);
        } /* endswitch */
    } /* endif */

}

void fase_EX_2_SUMREST(unsigned int oper) {

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    marca_t s;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    if (Op[oper].ciclo < Op[oper].Teval)
        Op[oper].ciclo++; /* Si quedan ciclos por realizar */

    s = Op[oper].estacion;

    /*** VISUALIZACIÓN ****/
    sprintf(p_str, F_FASE("A"), Op[oper].ciclo);
    muestra_fase(p_str, RS[s].orden);
    /**********************/

    if (Op[oper].ciclo == Op[oper].Teval) {
        RS[s].estado = FINALIZADA;
        RS[s].condicion = NO;

        Op[oper].ocupado = NO;

        /*** Estadisticas ***/
        // Se actualizan en Commit
        //estat.flops++;

        RS[s].control = RESULT;
        /* Cálculo del resultado */
        switch (RS[s].OP) {
            case OP_FP_ADD_D:
                RS[s].resultado.fp_d = RS[s].V1.fp_d + RS[s].V2.fp_d;
                break;
            case OP_FP_SUB_D:
                RS[s].resultado.fp_d = RS[s].V1.fp_d - RS[s].V2.fp_d;
                break;
            case OP_FP_ADD_S:
                RS[s].resultado.fp_s = RS[s].V1.fp_s + RS[s].V2.fp_s;
                break;
            case OP_FP_SUB_S:
                RS[s].resultado.fp_s = RS[s].V1.fp_s - RS[s].V2.fp_s;
                break;
            case OP_FP_ADD_PS:
                RS[s].resultado.fp_ps[Lo] = RS[s].V1.fp_ps[Lo] + RS[s].V2.fp_ps[Lo];
                RS[s].resultado.fp_ps[Hi] = RS[s].V1.fp_ps[Hi] + RS[s].V2.fp_ps[Hi];
                break;
            case OP_FP_SUB_PS:
                RS[s].resultado.fp_ps[Lo] = RS[s].V1.fp_ps[Lo] - RS[s].V2.fp_ps[Lo];
                RS[s].resultado.fp_ps[Hi] = RS[s].V1.fp_ps[Hi] - RS[s].V2.fp_ps[Hi];
                break;
            case OP_FP_GT_S:
                RS[s].resultado.int_d = (RS[s].V1.fp_s > RS[s].V2.fp_s);
                break;
            case OP_FP_LT_S:
                RS[s].resultado.int_d = (RS[s].V1.fp_s < RS[s].V2.fp_s);
                break;
            case OP_FP_GE_S:
                RS[s].resultado.int_d = (RS[s].V1.fp_s >= RS[s].V2.fp_s);
                break;
            case OP_FP_LE_S:
                RS[s].resultado.int_d = (RS[s].V1.fp_s <= RS[s].V2.fp_s);
                break;
            case OP_FP_EQ_S:
                RS[s].resultado.int_d = (RS[s].V1.fp_s == RS[s].V2.fp_s);
                break;
            case OP_FP_NE_S:
                RS[s].resultado.int_d = (RS[s].V1.fp_s != RS[s].V2.fp_s);
                break;
            case OP_FP_GT_D:
                RS[s].resultado.int_d = (RS[s].V1.fp_d > RS[s].V2.fp_d);
                break;
            case OP_FP_LT_D:
                RS[s].resultado.int_d = (RS[s].V1.fp_d < RS[s].V2.fp_d);
                break;
            case OP_FP_GE_D:
                RS[s].resultado.int_d = (RS[s].V1.fp_d >= RS[s].V2.fp_d);
                break;
            case OP_FP_LE_D:
                RS[s].resultado.int_d = (RS[s].V1.fp_d <= RS[s].V2.fp_d);
                break;
            case OP_FP_EQ_D:
                RS[s].resultado.int_d = (RS[s].V1.fp_d == RS[s].V2.fp_d);
                break;
            case OP_FP_NE_D:
                RS[s].resultado.int_d = (RS[s].V1.fp_d != RS[s].V2.fp_d);
                break;
            case OP_FP_MOV_S:
                RS[s].resultado.fp_s = RS[s].V1.fp_s;
                break;
            case OP_FP_MOV_D:
                RS[s].resultado.fp_d = RS[s].V1.fp_d;
                break;
            default:
                fprintf(stderr, "ERROR (%s:%d): Operacion %d no implementada\n",
                        __FILE__, __LINE__, RS[s].OP);
                exit(1);
        } /* endswitch */
    } /* endif */

}

void fase_EX_2_MULTDIV(unsigned int oper) {

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    marca_t s;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    if (Op[oper].ciclo < Op[oper].Teval)
        Op[oper].ciclo++; /* Si quedan ciclos por realizar */

    s = Op[oper].estacion;

    /*** VISUALIZACIÓN ****/
    sprintf(p_str, F_FASE("M"), Op[oper].ciclo);
    muestra_fase(p_str, RS[s].orden);
    /**********************/

    if (Op[oper].ciclo == Op[oper].Teval) {
        RS[s].estado = FINALIZADA;
        Op[oper].ocupado = NO;
        RS[s].condicion = NO;


        /*** Estadisticas ***/
        // Se actualizan en Commit
        //estat.flops++;
        RS[s].control = RESULT;
        /* Cálculo del resultado */
        switch (RS[s].OP) {
            case OP_FP_MUL_D:
                RS[s].resultado.fp_d = RS[s].V1.fp_d * RS[s].V2.fp_d;
                break;
            case OP_FP_DIV_D:
                RS[s].resultado.fp_d = RS[s].V1.fp_d / RS[s].V2.fp_d;
                break;
            case OP_FP_MUL_S:
                RS[s].resultado.fp_s = RS[s].V1.fp_s * RS[s].V2.fp_s;
                break;
            case OP_FP_DIV_S:
                RS[s].resultado.fp_s = RS[s].V1.fp_s / RS[s].V2.fp_s;
                break;
            case OP_FP_MUL_PS:
                RS[s].resultado.fp_ps[Lo] = RS[s].V1.fp_ps[Lo] * RS[s].V2.fp_ps[Lo];
                RS[s].resultado.fp_ps[Hi] = RS[s].V1.fp_ps[Hi] * RS[s].V2.fp_ps[Hi];
                break;
            case OP_FP_DIV_PS:
                RS[s].resultado.fp_ps[Lo] = RS[s].V1.fp_ps[Lo] / RS[s].V2.fp_ps[Lo];
                RS[s].resultado.fp_ps[Hi] = RS[s].V1.fp_ps[Hi] / RS[s].V2.fp_ps[Hi];
                break;
            default:
                fprintf(stderr, "ERROR (%s:%d): Operacion no implementada\n",
                        __FILE__, __LINE__);
                exit(1);
        } /* endswitch */
    } /* endif */

}

void fase_EX_2_DIRECCIONES(unsigned int oper) {

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    marca_t s;
    region_t r;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /*** Cálculo de la dirección efectiva */

    if (Op[oper].ciclo < Op[oper].Teval)
        Op[oper].ciclo++; /* Si quedan ciclos por realizar */

    s = Op[oper].estacion;

    /*** VISUALIZACIÓN ****/
    if (Op[oper].Teval > 1) {
        sprintf(p_str, F_FASE("AC"), Op[oper].ciclo);
    } else {
        sprintf(p_str, "AC");
    }
    muestra_fase(p_str, RS[s].orden);
    /**********************/

    if (Op[oper].ciclo == Op[oper].Teval) {
        Op[oper].ocupado = NO;

        RS[s].direccion = RS[s].V1.int_d + RS[s].desplazamiento;

        if (ES_ESTACION(s, BUFFER_CARGA)) {
            LB[s].estado = PREPARADA;
            if ((LB[s].OP == OP_LB) || (LB[s].OP == OP_LBU)) {
                if (!esta_alineada_dir_byte(LB[s].direccion)) {
                    LB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    LB[s].resultado.int_d = LB[s].excepcion;
                    LB[s].estado = FINALIZADA;
                    LB[s].control = EXCEPCION;
                }
            } else if ((LB[s].OP == OP_LH) || (LB[s].OP == OP_LHU)) {
                if (!esta_alineada_dir_half(LB[s].direccion)) {
                    LB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    LB[s].resultado.int_d = LB[s].excepcion;
                    LB[s].estado = FINALIZADA;
                    LB[s].control = EXCEPCION;
                }
            } else if ((LB[s].OP == OP_LW) || (LB[s].OP == OP_LWU)) {
                if (!esta_alineada_dir_word(LB[s].direccion)) {
                    LB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    LB[s].resultado.int_d = LB[s].excepcion;
                    LB[s].estado = FINALIZADA;
                    LB[s].control = EXCEPCION;
                }
            } else if (LB[s].OP == OP_LD) {
                if (!esta_alineada_dir_dword(LB[s].direccion)) {
                    LB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    LB[s].resultado.int_d = LB[s].excepcion;
                    LB[s].estado = FINALIZADA;
                    LB[s].control = EXCEPCION;
                }
            } else if (LB[s].OP == OP_FP_L_D) {
                if (!esta_alineada_dir_double(LB[s].direccion)) {
                    LB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    LB[s].resultado.int_d = LB[s].excepcion;
                    LB[s].estado = FINALIZADA;
                    LB[s].control = EXCEPCION;
                }
            } else if (LB[s].OP == OP_FP_L_PS) {
                if (!esta_alineada_dir_double(LB[s].direccion)) {
                    LB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    LB[s].resultado.int_d = LB[s].excepcion;
                    LB[s].estado = FINALIZADA;
                    LB[s].control = EXCEPCION;
                }
            }
            r = dir_a_region(mmem, LB[s].direccion);

            if (r != r_data && r != r_stack) {
                //   fnDebug("LOAD Ciclo=%ld dir=%ld\n", Ciclo, RS[s].direccion);
                LB[s].excepcion = EXC_OUT_MEM_DAT;
                LB[s].resultado.int_d = LB[s].excepcion;
                LB[s].estado = FINALIZADA;
                LB[s].control = EXCEPCION;
            }
        } else {
            SB[s].estado = PREPARADA;
            if (SB[s].OP == OP_SB) {
                if (!esta_alineada_dir_byte(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    Control_1.Excepcion = SI;
                }
            } else if (SB[s].OP == OP_SH) {
                if (!esta_alineada_dir_half(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    Control_1.Excepcion = SI;
                }
            } else if (SB[s].OP == OP_SW) {
                if (!esta_alineada_dir_word(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    Control_1.Excepcion = SI;
                }
            } else if (SB[s].OP == OP_SD) {
                if (!esta_alineada_dir_dword(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    Control_1.Excepcion = SI;
                }
            } else if (SB[s].OP == OP_FP_S_D) {
                if (!esta_alineada_dir_double(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    Control_1.Excepcion = SI;
                }
            } else if (SB[s].OP == OP_FP_S_PS) {
                if (!esta_alineada_dir_double(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    Control_1.Excepcion = SI;
                }
            } else if (SB[s].OP == OP_FP_S_S) {
                if (!esta_alineada_dir_float(SB[s].direccion)) {
                    SB[s].excepcion = EXC_UNALIGN_MEM_DAT;
                    Control_1.Excepcion = SI;
                }
            }

            r = dir_a_region(mmem, SB[s].direccion);

            if (r != r_data && r != r_stack) {
                SB[s].excepcion = EXC_OUT_MEM_DAT;
                Control_1.Excepcion = SI;
            }
        }

    } /* endif */

}

void fase_EX_2_MEMDATOS(unsigned int oper) {

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    marca_t s;
    boolean dirty;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    if (Op[oper].ciclo < Op[oper].Teval)
        Op[oper].ciclo++; /* Si quedan ciclos por realizar */

    s = Op[oper].estacion;

    /*** VISUALIZACIÓN ****/
    /*
        if (ES_ESTACION(s, BUFFER_CARGA)) {
            sprintf(p_str, "L%d", Op[oper].ciclo);
        } else {
            sprintf(p_str, "S%d", Op[oper].ciclo);
        }
     */
    sprintf(p_str, F_FASE("L"), Op[oper].ciclo);
    muestra_fase(p_str, RS[s].orden);
    /**********************/

    if (Op[oper].ciclo == Op[oper].Teval) {

        if (ES_ESTACION(s, BUFFER_CARGA)) {

            LB[s].estado = FINALIZADA;
            Op[oper].ocupado = NO;
            LB[s].condicion = NO;

            RS[s].control = RESULT;
            /* Cálculo del resultado */
            switch (RS[s].OP) {
                case OP_LD:
                    LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_dword, &dirty);
                    break;
                case OP_FP_L_D:
                    LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_double, &dirty);
                    break;
                case OP_FP_L_S:
                    LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_float, &dirty);
                    break;
                case OP_FP_L_PS:
                    LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_float_ps, &dirty);
                    break;
                case OP_LB:
                    LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_byte, &dirty);
                    break;
                case OP_LBU:
                    LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_byte, &dirty);
                    LB[s].resultado.int_d = 0xFF & LB[s].resultado.int_d;
                    break;
                case OP_LH:
                    LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_half, &dirty);
                    break;
                case OP_LHU:
                    LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_half, &dirty);
                    LB[s].resultado.int_d = 0xFFFF & LB[s].resultado.int_d;
                    break;
                case OP_LW:
                    LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_word, &dirty);
                    break;
                case OP_LWU:
                    LB[s].resultado = lee_mem_datos(mmem, LB[s].direccion, t_word, &dirty);
                    LB[s].resultado.int_d = 0xFFFFFFFF & LB[s].resultado.int_d;
                    break;
                default:
                    fprintf(stderr, "ERROR (%s:%d): Operacion no implementada\n",
                            __FILE__, __LINE__);
                    exit(1);
            } /* endswitch */

        } else {
            Op[oper].ocupado = NO;

            SB[s].ocupado = NO; /* El operador libera la estación de reserva */
            //SB[s].ocupado = LIBERANDOSE; /* El operador libera la estación de reserva */
            SB[s].excepcion = EXC_NONE;
            SB[s].condicion = NO;

            /* Escritura al final del último ciclo */
            switch (RS[s].OP) {
                case OP_SD:
                    escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_dword, SI);
                    break;
                case OP_FP_S_S:
                    escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_float, SI);
                    break;
                case OP_FP_S_D:
                    escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_double, SI);
                    break;
                case OP_FP_S_PS:
                    escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_float_ps, SI);
                    break;
                case OP_SW:
                    escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_word, SI);
                    break;
                case OP_SH:
                    escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_half, SI);
                    break;
                case OP_SB:
                    escribe_mem_datos(mmem, SB[s].direccion, SB[s].V2, t_byte, SI);
                    break;
                default:
                    fprintf(stderr, "ERROR (%s:%d): Operacion no implementada\n",
                            __FILE__, __LINE__);
                    exit(1);
            } /* endswitch */
        } /* endif */
    } /* endif */

}

