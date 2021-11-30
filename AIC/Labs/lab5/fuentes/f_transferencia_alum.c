/**************************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *
 * File: f_transferencia.c
 *
 * Description:
 *      Contiene la fase de transferencia por el bus común de datos para el
 *      algoritmo de Tomasulo con especulación.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *************************************************************************/


#define f_transferencia_alum_C

/***** Includes ***********************************************/

#include <stddef.h>
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
 * Func: fase_WB
 *
 **************************************************************/

void fase_WB_alum() {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    marca_t i, s;

    ciclo_t orden;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    
    /*** Busca RS con resultados disponibles */

    orden = MAX_ORDEN;
    s = 0;

    for (i = 0; i < TAM_ESTACIONES; i++) {
        if (RS[i].ocupado && RS[i].estado == FINALIZADA && RS[i].orden < orden) {
            s = i;
            orden = RS[i].orden;
        } /* endif */
    } /* endif */

    /*** VISUALIZACIÓN ****/
    for (i = 0; i < TAM_ESTACIONES; i++) {
        if (RS[i].ocupado && RS[i].estado == FINALIZADA && s != i) {
            muestra_fase("-", RS[i].orden);
        } /* endif */
    } /* endif */
    /**********************/

    if (orden >= MAX_ORDEN) return; /* No hay ninguna RS con resultados disponibles */

    /*** Volcado de resultados */
    BUS.valor = RS[s].resultado;
    BUS.codigo = RS[s].rob;


    /* En caso de que la instrucción fuera un salto o
     * se hubiera producido una excepción, se vuelca por el BUS */
    BUS.condicion = RS[s].condicion;
    BUS.control = RS[s].control;

    /*** Libera la RS */
    RS[s].ocupado = NO;


    // Una vez liberada, inicializa el estado
    RS[s].estado = PENDIENTE;

    /*** VISUALIZACIÓN ****/
    if (BUS.control != EXCEPCION) {
        muestra_fase("WB", RS[s].orden);
    } else {
        muestra_fase("<font color=\"red\">WB</font>", RS[s].orden);
    }
    /**********************/

    
    // Escritura en el ROB
    RB[BUS.codigo].valor = BUS.valor; // Copia al RB
    RB[BUS.codigo].completado = SI; // Lista para Commit


    /*** Lectura de resultados */
    
    /* Reorder buffer */
    // Situaciones excepcionales: saltos, excepciones, etc.
    switch (BUS.control) {
        case DIRECCION:
            RB[BUS.codigo].direccion = BUS.valor.int_d;
            RB[BUS.codigo].condicion = BUS.condicion;
            return;

        case CONDICION:
            RB[BUS.codigo].condicion = BUS.condicion;
            return;

        case EXCEPCION:
            RB[BUS.codigo].excepcion = BUS.valor.int_d;
            return;

        default:
            break;
    }

    /* Estaciones de reserva */

    for (s = INICIO_RS_ENTEROS; s <= FIN_RS_ENTEROS; s++) {
        if(RS[s].Q1 == BUS.codigo) {
            RS[s].V1 = BUS.valor; // Lee dato del bus
            RS[s].Q1 = MARCA_NULA; // Borra marca
        }
        if(RS[s].Q2 == BUS.codigo) {
            RS[s].V2 = BUS.valor; // Lee dato del bus
            RS[s].Q2 = MARCA_NULA; // Borra marca
        }

    } /* endfor */

    for (s = INICIO_RS_SUMREST; s <= FIN_RS_SUMREST; s++) {
        if(RS[s].Q1 == BUS.codigo) {
            RS[s].V1 = BUS.valor; // Lee dato del bus
            RS[s].Q1 = MARCA_NULA; // Borra marca
        }
        if(RS[s].Q2 == BUS.codigo) {
            RS[s].V2 = BUS.valor; // Lee dato del bus
            RS[s].Q2 = MARCA_NULA; // Borra marca
        }

    } /* endfor */

    for (s = INICIO_RS_MULTDIV; s <= FIN_RS_MULTDIV; s++) {
        if(RS[s].Q1 == BUS.codigo) {
            RS[s].V1 = BUS.valor; // Lee dato del bus
            RS[s].Q1 = MARCA_NULA; // Borra marca
        }
        if(RS[s].Q2 == BUS.codigo) {
            RS[s].V2 = BUS.valor; // Lee dato del bus
            RS[s].Q2 = MARCA_NULA; // Borra marca
        }

    } /* endfor */

    
    for (s = INICIO_BUFFER_CARGA; s <= FIN_BUFFER_CARGA; s++) {
        if(LB[s].Q1 == BUS.codigo) {
            LB[s].V1 = BUS.valor; // Lee dato del bus
            LB[s].Q1 = MARCA_NULA; // Borra marca
        }
        if(LB[s].Q2 == BUS.codigo) {
            LB[s].V2 = BUS.valor; // Lee dato del bus
            LB[s].Q2 = MARCA_NULA; // Borra marca
        }

    } /* endfor */

    for (s = INICIO_BUFFER_ALMACEN; s <= FIN_BUFFER_ALMACEN; s++) {
        if(SB[s].Q1 == BUS.codigo) {
            SB[s].V1 = BUS.valor; // Lee dato del bus
            SB[s].Q1 = MARCA_NULA; // Borra marca
        }
        if(SB[s].Q2 == BUS.codigo) {
            SB[s].V2 = BUS.valor; // Lee dato del bus
            SB[s].Q2 = MARCA_NULA; // Borra marca
        }

    } /* endfor */

} /* end fase_WB */
