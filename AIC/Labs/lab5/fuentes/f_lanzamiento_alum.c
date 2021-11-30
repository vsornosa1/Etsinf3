/**************************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *
 * File: f_lanzamiento.c
 *
 * Description:
 *      Contiene la fase de lanzamiento de instrucciones del algoritmo de
 *      Tomasulo con especulación
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *************************************************************************/


#define f_lanzamiento_alum_C

/***** Includes ***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <tipos.h>

#include "main.h"
#include "instrucciones.h"
#include "prediccion.h"
#include "presentacion.h"
#include "algoritmo.h"
#include "tipos.h"
#include "depurar.h"


/***************************************************************
 *
 * Func: fase_ISS
 *
 **************************************************************/

void fase_ISS_alum() {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int s = 0;
    marca_t b;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /* Decodificación */

#define I_OP IF_ISS_2.IR.codop
#define I_S1 IF_ISS_2.IR.Rfuente1
#define I_S2 IF_ISS_2.IR.Rfuente2
#define I_D IF_ISS_2.IR.Rdestino
#define I_INM IF_ISS_2.IR.inmediato
#define I_ETIQ IF_ISS_2.IR.etiqueta
#define I_PC IF_ISS_2.PC
#define I_ORDEN IF_ISS_2.orden
#define I_EXC IF_ISS_2.excepcion
#define I_PRED IF_ISS_2.prediccion
#define I_PRED_DATA IF_ISS_2.pred_data

    
    /*** VISUALIZACIÓN ****/
    PC_ISS = I_PC;
    /**********************/

    /*** Si no sale correctamente hay que parar */

    if (Control_2.Cancelar || IF_ISS_2.ignorar) {
        Control_1.Parar = NO;
        return;
    } else if (Control_1.Cancelar || IF_ISS_2.cancelar) { /* Este ciclo está cancelado */
        /*** VISUALIZACIÓN ****/
        muestra_fase("X", I_ORDEN);
        /**********************/
        return;
    } else if (Control_1.Parar || Control_1.Excepcion) {
        /*** VISUALIZACIÓN ****/
        muestra_fase("i", I_ORDEN);
        /**********************/

        Control_1.Parar = SI;

        /* Si la instrucción anterior del mismo grupo se ha parado o
         * hay una excepción en el reorder buffer
         * entonces esta instrucción ni siquiera se intenta */

        return;
    } /* endif */

    /*** VISUALIZACIÓN ****/
    if (I_OP != OP_NOP) {
        if (I_OP == OP_TRAP) {
            muestra_fase_exception("I", I_ORDEN);
        } else {
            muestra_fase("I", I_ORDEN);
        }
    }
    /**********************/

    Control_1.Parar = SI;

    /*** Busca un hueco en la cola */

    if (RB_long < TAM_REORDER) {
        b = RB_fin;
    } else {
        return; /* No hay huecos en el ROB */
    }

    RB[b].excepcion = I_EXC;
    RB[b].prediccion = I_PRED;
    RB[b].pred_data = I_PRED_DATA;

    /*** Lanza la instruccion */

    switch (I_OP) {
                    case OP_FP_L_D:
            /*** Busca un hueco en el tampón de lectura */
            for (s = INICIO_BUFFER_CARGA; s <= FIN_BUFFER_CARGA; s++)
                if (!LB[s].ocupado) break;

            if (s > FIN_BUFFER_CARGA) return;
            /* No hay sitio en la estación de reserva */

            /*** Reserva el tampón de lectura */
            LB[s].ocupado = SI;
            LB[s].OP = I_OP;
            LB[s].rob = b;

            /*** Operando 1 (en Rint) ***/
            if (Rint[I_S1].rob == MARCA_NULA) {
                LB[s].V1 = Rint[I_S1].valor;
                LB[s].Q1 = MARCA_NULA;
            } else if (RB[Rint[I_S1].rob].completado) {
                LB[s].V1 = RB[Rint[I_S1].rob].valor;
                LB[s].Q1 = MARCA_NULA;
            } else {
                LB[s].Q1 = Rint[I_S1].rob;
            } /* endif */

            /*** Operando 2 ***/
            LB[s].Q2 = MARCA_NULA;

            /*** Desplazamiento */
            LB[s].desplazamiento = I_INM;

            /*** Reserva la entrada del ROB */
            RB[b].ocupado = SI;
            RB[b].OP = I_OP;
            RB[b].dest = I_D;
            RB[b].completado = NO;

            /*** Reserva del registro destino */
                            Rfp[I_D].rob = b; // OP_FP_LD

            /*** VISUALIZACION ***/
            LB[s].estado = PENDIENTE;
            LB[s].orden = I_ORDEN;
            //LB[s].PC = I_PC;
            sprintf(LB[s].etiqueta, "%s", I_ETIQ);
            RB[b].orden = I_ORDEN;
            RB[b].PC = I_PC;

            break;
            
        case OP_FP_S_D:
            /*** Busca un hueco en el tampón de escritura */
            for (s = INICIO_BUFFER_ALMACEN; s <= FIN_BUFFER_ALMACEN; s++)
                if (!SB[s].ocupado) break;
            if (s > FIN_BUFFER_ALMACEN) return;


            /*** Reserva el tampón de escritura */
            SB[s].ocupado = SI;
            SB[s].OP = I_OP;


            /*** Operando 1 (en Rint) ***/
            if (Rint[I_S1].rob == MARCA_NULA) {
                SB[s].V1 = Rint[I_S1].valor;
                SB[s].Q1 = MARCA_NULA;
            } else if (RB[Rint[I_S1].rob].completado) {
                SB[s].V1 = RB[Rint[I_S1].rob].valor;
                SB[s].Q1 = MARCA_NULA;
            } else {
                SB[s].Q1 = Rint[I_S1].rob;
            } /* endif */


            /*** Operando 2 (en Rfp) ***/
            if (Rfp[I_S2].rob == MARCA_NULA) {
                SB[s].V2 = Rfp[I_S2].valor;
                SB[s].Q2 = MARCA_NULA;
            } else if (RB[Rfp[I_S2].rob].completado) {
                SB[s].V2 = RB[Rfp[I_S2].rob].valor;
                SB[s].Q2 = MARCA_NULA;
            } else {
                SB[s].Q2 = Rfp[I_S2].rob;
            } /* endif */


            /*** Desplazamiento */
            SB[s].desplazamiento = I_INM;


            /*** Reserva la entrada del ROB */
            RB[b].ocupado = SI;
            RB[b].OP = I_OP;
            RB[b].dest = s;
            RB[b].completado = NO;


            /*** VISUALIZACION ***/
            SB[s].estado = PENDIENTE;
            SB[s].orden = I_ORDEN;
            //SB[s].PC = I_PC;
            sprintf(SB[s].etiqueta, "%s", I_ETIQ);
            /*** La instrucción de escritura se debe confirmar */
            SB[s].confirm = NO;
            SB[s].rob = b; /* En teoría, no hace falta para las stores. Se queda aquí por si es necesario en el simulador. */
            RB[b].orden = I_ORDEN;
            RB[b].PC = I_PC;

            break;
            
            
            
            
            
            
            
            
                    case OP_FP_ADD_D:
        case OP_FP_SUB_D:
            /*** Busca un hueco en la estación de reserva */
            for (s = INICIO_RS_SUMREST; s <= FIN_RS_SUMREST; s++)
                if (!RS[s].ocupado) break;
            if (s > FIN_RS_SUMREST) return;


            /*** Reserva la estación de reserva */
            RS[s].ocupado = SI;
            RS[s].OP = I_OP;
            RS[s].rob = b;


            /*** Operando 1 (en Rfp) ***/
            if (Rfp[I_S1].rob == MARCA_NULA) {
                RS[s].V1 = Rfp[I_S1].valor;
                RS[s].Q1 = MARCA_NULA;
            } else if (RB[Rfp[I_S1].rob].completado) {
                RS[s].V1 = RB[Rfp[I_S1].rob].valor;
                RS[s].Q1 = MARCA_NULA;
            } else {
                RS[s].Q1 = Rfp[I_S1].rob;
            } /* endif */


            /*** Operando 2 (en Rfp) ***/
            if (Rfp[I_S2].rob == MARCA_NULA) {
                RS[s].V2 = Rfp[I_S2].valor;
                RS[s].Q2 = MARCA_NULA;
            } else if (RB[Rfp[I_S2].rob].completado) {
                RS[s].V2 = RB[Rfp[I_S2].rob].valor;
                RS[s].Q2 = MARCA_NULA;
            } else {
                RS[s].Q2 = Rfp[I_S2].rob;
            } /* endif */


            /*** Reserva la entrada del ROB */
            RB[b].ocupado = SI;
            RB[b].OP = I_OP;
            RB[b].dest = I_D;
            RB[b].completado = NO;


            /*** Reserva del registro destino */
            Rfp[I_D].rob = b;


            /*** VISUALIZACION ***/
            RS[s].estado = PENDIENTE;
            RS[s].orden = I_ORDEN;
            //RS[s].PC = I_PC;
            RB[b].orden = I_ORDEN;
            RB[b].PC = I_PC;

            break;
            
            
            
            
            
            
            
                    case OP_FP_MUL_D:
        case OP_FP_DIV_D:
            /*** Busca un hueco en la estación de reserva */
            for (s = INICIO_RS_MULTDIV; s <= FIN_RS_MULTDIV; s++)
                if (!RS[s].ocupado) break;
            if (s > FIN_RS_MULTDIV) return;


            /*** Reserva el operador virtual */
            RS[s].ocupado = SI;
            RS[s].OP = I_OP;
            RS[s].rob = b;


            /*** Operando 1 ***/
            if (Rfp[I_S1].rob == MARCA_NULA) {
                RS[s].V1 = Rfp[I_S1].valor;
                RS[s].Q1 = MARCA_NULA;
            } else if (RB[Rfp[I_S1].rob].completado) {
                RS[s].V1 = RB[Rfp[I_S1].rob].valor;
                RS[s].Q1 = MARCA_NULA;
            } else {
                RS[s].Q1 = Rfp[I_S1].rob;
            } /* endif */


            /*** Operando 2 ***/
            if (Rfp[I_S2].rob == MARCA_NULA) {
                RS[s].V2 = Rfp[I_S2].valor;
                RS[s].Q2 = MARCA_NULA;
            } else if (RB[Rfp[I_S2].rob].completado) {
                RS[s].V2 = RB[Rfp[I_S2].rob].valor;
                RS[s].Q2 = MARCA_NULA;
            } else {
                RS[s].Q2 = Rfp[I_S2].rob;
            } /* endif */


            /*** Reserva la entrada del ROB */
            RB[b].ocupado = SI;
            RB[b].OP = I_OP;
            RB[b].dest = I_D;
            RB[b].completado = NO;


            /*** Reserva del registro destino */
            Rfp[I_D].rob = b;



            /*** VISUALIZACION ***/
            RS[s].estado = PENDIENTE;
            RS[s].orden = I_ORDEN;
            //RS[s].PC = I_PC;
            RB[b].orden = I_ORDEN;
            RB[b].PC = I_PC;

            break;
        default:
            fprintf(stderr, "ERROR (%s:%d): Operacion %d no implementada\n", __FILE__, __LINE__, I_OP);
            exit(1);
            break;
    } /* endswitch */

    /*** La instrucción se ha lanzado correctamente */

    Control_1.Parar = NO;
    RB_fin = (RB_fin + 1) % TAM_REORDER;
    RB_long++;

    return;

} /* end fase_ISS */
