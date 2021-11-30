/**************************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *
 * File: f_busqueda.c
 *
 * Description:
 *    Contiene la fase de búsqueda
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *************************************************************************/

#define f_busqueda_C

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "instrucciones.h"
#include "prediccion.h"
#include "presentacion.h"
#include "memoria.h"
#include "tipos.h"
#include "depurar.h"

/***************************************************************
 *
 * Func: fase_IF
 *
 **************************************************************/

void fase_IF() {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    boolean salto;
    boolean prediccion;
    dword destino;
    prediccion_t pred_otros;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    if (Control_2.Parar) return;

    IF_ISS_1.PC = Control_2.NPC;
    IF_ISS_1.orden = Control_2.orden;
    Control_1.orden = Control_2.orden + 1;

    if (Control_1.Excepcion) {
        /*** VISUALIZACIÓN ****/
        init_instruc(IF_ISS_1.PC, IF_ISS_1.orden);
        muestra_fase("if", IF_ISS_1.orden);
        /**********************/
        return;
    }

    if (en_region(mmem, Control_2.NPC, r_text)) {
        if (esta_alineada_dir_instruc(IF_ISS_1.PC)) {
            IF_ISS_1.IR = lee_mem_instruc(mmem, IF_ISS_1.PC); /* Busca la siguiente instrucción */
            /* Incrementa el número de instrucciones leidas */
            IF_ISS_1.excepcion = EXC_NONE;
        } else {
            /* Dirección de acceso a memoria de instrucciones desalineada */
            IF_ISS_1.IR.codop = OP_TRAP; /* Cancela la instrucción */
            /* Incrementa el número de instrucciones leidas */
            IF_ISS_1.excepcion = EXC_UNALIGN_MEM_INST;
        }
    } else {
        /*** Excepción en el acceso a la memoria de instrucciones */

        IF_ISS_1.IR.codop = OP_TRAP; /* Cancela la instrucción */
        /* Incrementa el número de instrucciones leidas */
        IF_ISS_1.excepcion = EXC_OUT_MEM_INST;
    } /* endif */

    /*** PREDICCIÓN ************/

    /* Almacenamos máscaras actuales */
    if (!Control_1.Cancelar & !IFstall) {
        salto = obtener_prediccion(IF_ISS_1.PC, IF_ISS_1.orden, &prediccion, &destino, &pred_otros);
        //      fprintf(stdout, "DEBUG -- IF -- -- Ciclo %d No Salta %d %d %d %d\n", Ciclo, IF_ISS_1.orden, IF_ISS_1.PC, prediccion, destino);
        if (salto && prediccion) {
            /*** Es un salto y predice que salta */
            //IF_ISS_1.prediccion = SI;
            IF_ISS_1.prediccion = prediccion;
            Control_1.NPC = destino;

            /* Predictor hibrido y mascara global */
            IF_ISS_1.pred_data = pred_otros;
            IF_ISS_1.pred_data.dir_destino = destino; // Destino ofrecido por el predictor


            /*** Mascaras ***/
            // printf("%d ", global_branch_mask);
            branch_mask_local = (branch_mask_local << 1) | IF_ISS_1.prediccion;
            branch_mask_local = branch_mask_local & (pwrtwo(n_bits_branch_mask_local) - 1);

            branch_mask_global = (branch_mask_global << 1) | IF_ISS_1.prediccion;
            branch_mask_global = branch_mask_global & (pwrtwo(n_bits_branch_mask_global) - 1);
            // printf("%d\n", global_branch_mask);

            /*** Estadisticas ***/
            estat.saltos_accedidos++;
            //fnDebug("+++ IF Ciclo=%lld Encontrados=%d Accedidos=%d\n", Ciclo, estat.saltos_encontrados, estat.saltos_accedidos);


            //fnDebug("+++ IF -- -- Ciclo:%d Orden:%d PC:%d pred:%s NPC:%d\n", Ciclo, IF_ISS_1.orden, IF_ISS_1.PC, IF_ISS_1.prediccion ? "SI" : "NO", Control_1.NPC);
            //fnDebug("Accedidos++ Ciclo=%lld\n", Ciclo);
        } else {
            /*** O no es un salto, o es un salto y predice que no salta */
            IF_ISS_1.prediccion = NO;
            Control_1.NPC = IF_ISS_1.PC + 4;



            if (inst_salto(IF_ISS_1.IR.codop)) {

                /* Predictor hibrido y mascara global */
                IF_ISS_1.pred_data = pred_otros;
                IF_ISS_1.pred_data.dir_destino = destino; // 0 si no estaba en la tabla

                /*** Mascaras ***/
                // printf("%d ", global_branch_mask);
                branch_mask_local = (branch_mask_local << 1) | IF_ISS_1.prediccion;
                branch_mask_local = branch_mask_local & (pwrtwo(n_bits_branch_mask_local) - 1);

                branch_mask_global = (branch_mask_global << 1) | IF_ISS_1.prediccion;
                branch_mask_global = branch_mask_global & (pwrtwo(n_bits_branch_mask_global) - 1);

                estat.saltos_accedidos++;
                //fnDebug("+++ IF Ciclo=%lld Encontrados=%d Accedidos=%d\n", Ciclo, estat.saltos_encontrados, estat.saltos_accedidos);


                //fnDebug("+++ IF -- -- Ciclo:%d Orden:%d PC:%d pred:%s NPC:%d\n", Ciclo, IF_ISS_1.orden, IF_ISS_1.PC, IF_ISS_1.prediccion ? "SI" : "NO", Control_1.NPC);
            }
        } /* endif */
    } /* endif */



    /*** VISUALIZACIÓN ****/
    init_instruc(IF_ISS_1.PC, IF_ISS_1.orden);
    if (!Control_1.Cancelar) {
        char * fase;
        if (!IFstall) {
            fase = "IF";
        } else {
            fase = "if";
        }
        if (IF_ISS_1.excepcion == EXC_NONE) {
            muestra_fase(fase, IF_ISS_1.orden);
        } else {
            char aux[80];
            sprintf(aux, "<span style='color: red'>%s</span>", fase);
            muestra_fase(aux, IF_ISS_1.orden);
        }
    } else {
        muestra_fase("X", IF_ISS_1.orden);
    }
    /**********************/

} /* end fase_IF */

