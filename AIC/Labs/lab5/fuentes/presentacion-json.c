/**************************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politecnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *
 * File: presentacion-json.c
 *
 * Description: Contiene las funciones necesarias para presentar la
 *    información sobre la ejecución del algoritmo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *************************************************************************/

#define presentacion_json_C

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memoria.h>
#include <enlazador.h>
#include <tipos.h>

#include "main.h"
#include "tipos.h"
#include "instrucciones.h"
#include "presentacion.h"
#include "memoria.h"
#include "registros.h"
#include "depurar.h"

#include "presentacion-json.h"

#include "jwrite.h"

/***** Variables globales *************************************/

presentacion_interface_t presentacion_json = {
    .imprime_inicio_f = imprime_inicio_json,
    .imprime_final_f = imprime_final_json,
    .inicio_ciclo_f = inicio_ciclo_json,
    .fin_ciclo_f = fin_ciclo_json,
    .imprime_estado_f = imprime_estado_json,
    .imprime_crono_f = imprime_crono_null, // No es necesaria
    .imprime_predictor_f = imprime_predictor_null, // Sin implementar
    .init_instruc_f = init_instruc_json,
    .muestra_fase_f = muestra_fase_json,
    .muestra_datos_f = muestra_datos_json
};

/***** Macros locales *****************************************/

#define foreach(_i, _limit) for (_i = 0; _i < _limit; _i++)
#define forall(_i, _name) for (_i = INICIO_##_name; _i <= FIN_##_name; _i++)

#define STRUCT_INFO(id, label, type, layout, size) \
do { \
   jwObj_string("id", #id);\
   jwObj_string("label", label);\
   jwObj_string("type", #type);\
   jwObj_string("layout", #layout);\
   jwObj_int("size", size);\
} while(0)

#define TABLE_FIELD(id, label) \
do { \
   jwArr_object();\
   jwObj_string("id", #id);\
   jwObj_string("label", label);\
   jwEnd();\
} while(0)

#define DATA_FIELD(id, label, size) \
do { \
   jwArr_object();\
   jwObj_string("id", #id);\
   jwObj_string("label", label);\
   jwObj_int("size", size);\
   jwEnd();\
} while(0)

#define ENTRY_VALUE(section, value, class) \
do { \
   if (class != NULL && strlen(class)) {\
     jwObj_object(section);\
       jwObj_string("text", value);\
       jwObj_string("class", class);\
     jwEnd();\
   } else {\
     jwObj_string(section, value);\
   }\
} while(0)

#define TABLE_FORMAT_ENTRY(id, field, format, row, type, section, value) \
do { \
   char entry_id[80];\
   sprintf(entry_id, #id "-" #field "-" format, row);\
   jwArr_object();\
   jwObj_string("id", entry_id);\
   jwObj_##type(section, value);\
   jwEnd();\
} while(0)

#define TABLE_FORMAT_ENTRY_CLASS(id, field, format, row, section, value, class) \
do { \
   char entry_id[80];\
   sprintf(entry_id, #id "-" #field "-" format, row);\
   jwArr_object();\
   jwObj_string("id", entry_id);\
   ENTRY_VALUE(section, value, class);\
   jwEnd();\
} while(0)

#define TABLE_ENTRY(id, field, row, type, section, value) TABLE_FORMAT_ENTRY(id, field, "%d", row, type, #section, value)
#define TABLE_ENTRY_CLASS(id, field, row, type, section, value, class) TABLE_FORMAT_ENTRY_CLASS(id, field, "%d", row, #section, value, class)

#define ADD_ENTRY_CLASS(id, field, row, text, class) \
do { \
   char entry_id[80];\
   sprintf(entry_id, #id "-" #field "-%d", row);\
   imprime_elemento(entry_id, text, class); \
} while(0)

#define ADD_ENTRY(id, field, row, text) ADD_ENTRY_CLASS(id, field, row, text, "")


#define CRONO_PC(row, value) TABLE_FORMAT_ENTRY(crono, pc, F_CICLO, row, int, "value", value)
#define CRONO_INSTRUC(row, value, class) TABLE_FORMAT_ENTRY_CLASS(crono, instruc, F_CICLO, row, "value", value, class)
#define CRONO_PHASE(col, row, value, exception) \
do {\
   char field[8]; \
   char entry_id[80];\
   sprintf(field, F_CICLO, col); \
   sprintf(entry_id, "crono-" F_CICLO "-" F_CICLO, col, row);\
   jwArr_object();\
   jwObj_string("id", entry_id);\
   ENTRY_VALUE("value", value, ((exception) ? "exception-phase" : ""));\
   jwEnd();\
} while (0)

/***** Variables locales **************************************/

static FILE *f_salida;

static boolean output_active = NO;

/***** Definiciones Internas **********************************/

static void find_field(char *key, char **text, char **class) {
    table_value_t *old;

    old = ht_get(estado_previo, key);
    if (old == NULL) {
        *text = NULL;
        *class = NULL;
    } else {
        *text = old->text;
        *class = old->class;
    }
}

static void imprime_elemento(char *key, char *text, char *class) {

    char *p_text;
    char *p_class;

    find_field(key, &p_text, &p_class);

    p_text = (p_text != NULL) ? p_text : "";
    p_class = (p_class != NULL) ? p_class : "";

    text = (text != NULL) ? text : "";
    class = (class != NULL) ? class : "";

    if (strcmp(p_text, text) != 0 || strcmp(p_class, class) != 0) {
        jwArr_object();
        jwObj_string("id", key);

        if (strlen(p_text) || strlen(p_class)) {
            ENTRY_VALUE("prev", p_text, p_class);
        }
        ENTRY_VALUE("value", text, class);

        jwEnd();

        ht_set(estado_previo, key, text, class);
    }

    // FIRMA_1_1(key, text);
}

/***************************************************************
 *
 * Func: imprime_estado_predictor
 *
 * Description:
 *   Obtiene el estado del predictor
 *
 **************************************************************/

static void imprime_estado_predictor_json
(
        char *nom,
        estado_predic_t e
        ) {
    /*************************************/
    /*  Function body                    */
    /*************************************/

    switch (e) {
        case NO_SALTA:
            sprintf(nom, "<font color=\"orange\">NO Salta</font>");
            break;
        case NO_SALTA_UN_FALLO:
            sprintf(nom, "<font color=\"orange\">NO Salta (1 fallo)</font>");
            break;
        case SALTA_UN_FALLO:
            sprintf(nom, "<font color=\"green\">Salta (1 fallo)</font>");
            break;
        case SALTA:
            sprintf(nom, "<font color=\"green\">Salta</font>");
            break;
        default:
            sprintf(nom, "?");
            break;
    } /* endswitch */

} /* end imprime_estado_predictor */

/***************************************************************
 *
 * Func: imprime_estado_predictor
 *
 * Description:
 *   Obtiene el estado del predictor
 *
 **************************************************************/

static void imprime_estado_predictor_n_json
(
        char *nom,
        int e,
        int n_bits
        ) {
    /*************************************/
    /*  Function body                    */
    /*************************************/

    if (e <= pwrtwo(n_bits - 1) - 1) {
        sprintf(nom, "<font color=\"orange\">(%d) NO Salta</font>", e);
    } else {
        sprintf(nom, "<font color=\"green\">(%d) Salta</font>", e);

    }

}/* end imprime_estado_predictor_n */

/***************************************************************
 *
 * Func: imprime_estado_selector
 *
 * Description:
 *   Obtiene el estado del predictor
 *
 **************************************************************/

static void imprime_estado_selector_n_json
(
        char *nom,
        int e
        ) {
    /*************************************/
    /*  Function body                    */
    /*************************************/

    if (e <= pwrtwo(n_bits_pred_hybrid - 1) - 1) {
        sprintf(nom, "<font color=\"orange\">(%d) Global</font>", e);
    } else {
        sprintf(nom, "<font color=\"green\">(%d) Local</font>", e);

    }

}/* end imprime_estado_predictor_n */

/***************************************************************
 *
 * Func: imprime_config_json
 *
 * Description:
 *
 *
 **************************************************************/
static void imprime_config_json
(
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[256];

#define CONFIG_ENTRY(fila, texto, tipo, valor) \
do { \
    TABLE_ENTRY(config, label, fila, string, value, texto); \
    TABLE_ENTRY(config, value, fila, tipo, value, valor); \
} while (0)

    CONFIG_ENTRY(0, "Número de vías Issue", int, NUM_VIAS_ISSUE);
    CONFIG_ENTRY(1, "Número de buses WB", int, NUM_VIAS_BUS);
    CONFIG_ENTRY(2, "Número vías Commit", int, NUM_VIAS_COMMIT);
    CONFIG_ENTRY(3, "Tamaño ROB", int, TAM_REORDER);
    imprime_tipo_predictor(aux);
    CONFIG_ENTRY(4, "Tipo de predictor", string, aux);
    CONFIG_ENTRY(5, "Tamaño buffer predicción", int, TAM_BUFFER_PREDIC);

#define OPER_ENTRY(fila, operador, numero, latencia, tipo, t_estaciones, estaciones) \
do { \
    TABLE_ENTRY(oper, label, fila, string, value, operador); \
    TABLE_ENTRY(oper, number, fila, int, value, numero); \
    TABLE_ENTRY(oper, latency, fila, int, value, latencia); \
    TABLE_ENTRY(oper, type, fila, string, value, tipo); \
    TABLE_ENTRY(oper, size, fila, t_estaciones, value, estaciones); \
} while (0)

#define OPERADOR(fila, operador, ident, estaciones) \
  OPER_ENTRY(fila, operador, NUM_INICIO_##ident, TEVAL_##ident, (NUM_INICIO_##ident < NUM_OPER_##ident) ? "Segmentado" : "Convencional", string, estaciones)

    sprintf(aux, "%d RS", TAM_RS_ENTEROS);
    OPERADOR(0, "Enteros", ENTEROS, aux);
    sprintf(aux, "%d RS", TAM_RS_SUMREST);
    OPERADOR(1, "Suma/Resta C.F.", SUMREST, aux);
    sprintf(aux, "%d RS", TAM_RS_MULTDIV);
    OPERADOR(2, "Mult/Div. C.F.", MULTDIV, aux);
    sprintf(aux, "%d LB %d SB", TAM_BUFFER_CARGA, TAM_BUFFER_ALMACEN);
    OPERADOR(3, "Memoria Datos", MEMDATOS, aux);

}

/**
 * Imprime información sobre las estructuras que se deben mostrar
 */
static void imprime_estructuras_json() {

    jwObj_array("structures");
    {
        /* Configuración */
        jwArr_object();
        {
            STRUCT_INFO(config, "Configuración", table, vertical, 6);
            jwObj_array("fields");
            {
                TABLE_FIELD(label, "Atributo");
                TABLE_FIELD(value, "Valor");
            }
            jwEnd();
        }
        jwEnd();

        /* Operadores */
        jwArr_object();
        {
            STRUCT_INFO(oper, "Operadores", table, vertical, 6);
            jwObj_array("fields");
            {
                TABLE_FIELD(label, "Operador");
                TABLE_FIELD(number, "Número");
                TABLE_FIELD(latency, "Latencia");
                TABLE_FIELD(type, "Tipo");
                TABLE_FIELD(size, "Estaciones");
            }
            jwEnd();
        }
        jwEnd();

        /* Reorder Buffer */
        jwArr_object();
        {
            STRUCT_INFO(rob, "ROB", table, vertical, TAM_REORDER);
            jwObj_array("fields");
            {
                TABLE_FIELD(id, "Cod");
                TABLE_FIELD(busy, "Ocupado");
                TABLE_FIELD(instruc, "Instrucción");
                TABLE_FIELD(completed, "Completado");
                TABLE_FIELD(dest, "Destino");
                TABLE_FIELD(value, "Valor");
                TABLE_FIELD(pred, "Pred.");
                TABLE_FIELD(excep, "Exc.");
                TABLE_FIELD(pc, "PC");
            }
            jwEnd();
        }
        jwEnd();

        /* Reservation Stations */
        jwArr_object();
        {
            STRUCT_INFO(rs, "RS", table, vertical, TAM_RS_ENTEROS + TAM_RS_SUMREST + TAM_RS_MULTDIV);
            jwObj_array("fields");
            {
                TABLE_FIELD(id, "Cod");
                TABLE_FIELD(busy, "Ocupado");
                TABLE_FIELD(op, "Op");
                TABLE_FIELD(qj, "Q1");
                TABLE_FIELD(vj, "V1");
                TABLE_FIELD(qk, "Q2");
                TABLE_FIELD(vk, "V2");
                TABLE_FIELD(rob, "rob");
                TABLE_FIELD(result, "Resultado");
                TABLE_FIELD(state, "Estado");
            }
            jwEnd();
        }
        jwEnd();

        /* Reservation Stations: Load */
        jwArr_object();
        {
            STRUCT_INFO(tl, "Buffer Lectura", table, vertical, TAM_BUFFER_CARGA);
            jwObj_array("fields");
            {
                TABLE_FIELD(id, "Cod");
                TABLE_FIELD(busy, "Ocupado");
                TABLE_FIELD(op, "Op");
                TABLE_FIELD(qj, "Q1");
                TABLE_FIELD(vj, "V1");
                TABLE_FIELD(desp, "Desp(etiq)");
                TABLE_FIELD(addr, "Direc.");
                TABLE_FIELD(rob, "rob");
                TABLE_FIELD(result, "Resultado");
                TABLE_FIELD(state, "Estado");
            }
            jwEnd();
        }
        jwEnd();

        /* Reservation Stations: Load */
        jwArr_object();
        {
            STRUCT_INFO(te, "Buffer Escritura", table, vertical, TAM_BUFFER_ALMACEN);
            jwObj_array("fields");
            {
                TABLE_FIELD(id, "Cod");
                TABLE_FIELD(busy, "Ocupado");
                TABLE_FIELD(op, "Op");
                TABLE_FIELD(qj, "Q1");
                TABLE_FIELD(vj, "V1");
                TABLE_FIELD(desp, "Desp(etiq)");
                TABLE_FIELD(addr, "Direc.");
                TABLE_FIELD(qk, "Q2");
                TABLE_FIELD(vk, "V2");
                TABLE_FIELD(confirm, "Confirm.");
                TABLE_FIELD(state, "Estado");
            }
            jwEnd();
        }
        jwEnd();

        /* Registers: integer */
        jwArr_object();
        {
            STRUCT_INFO(rint, "Rint", table, horizontal, TAM_REGISTROS);
            jwObj_array("fields");
            {
                TABLE_FIELD(id, "Reg");
                TABLE_FIELD(value, "Value");
                TABLE_FIELD(rob, "rob");
            }
            jwEnd();
        }
        jwEnd();

        /* Registers: F.P. */
        jwArr_object();
        {
            STRUCT_INFO(rfp, "Rfp", table, horizontal, TAM_REGISTROS);
            jwObj_array("fields");
            {
                TABLE_FIELD(id, "Reg");
                TABLE_FIELD(value, "Value");
                TABLE_FIELD(rob, "rob");
            }
            jwEnd();
        }
        jwEnd();

        /* Memory: Datos. */
        for (region_t r = (region_t) 0; r < num_regiones; r++) {
            if (!es_region_datos(r) || tam_region(mmem, r) == 0)
                continue;

            jwArr_object();
            {
                // STRUCT_INFO(m[region], "Mem. Datos ([region]) ", table, vertical, tam_region(r));
                do {
                    char text[64];

                    sprintf(text, "m%s", mmem->regiones[r].nombre);
                    jwObj_string("id", text);

                    sprintf(text, "Mem. Datos (%s)", mmem->regiones[r].nombre);
                    jwObj_string("label", text);
                    jwObj_string("type", "table");
                    jwObj_string("layout", "vertical");
                    jwObj_int("size", tam_region(mmem, r));
                } while (0);

                jwObj_array("fields");
                {
                    TABLE_FIELD(addr, "Dirección");
                    DATA_FIELD(data, "Datos", 8);
                }
                jwEnd();
            }
            jwEnd();
        }

        /* Memory: Instruc. */
        jwArr_object();
        {
            STRUCT_INFO(mi, "Mem. Instrucciones", table, vertical, tam_region(mmem, r_text));
            jwObj_array("fields");
            {
                TABLE_FIELD(pc, "Dirección");
                TABLE_FIELD(instruc, "Instrucciones");
            }
            jwEnd();
        }
        jwEnd();

        /* Cronogram */
        jwArr_object();
        {
            STRUCT_INFO(crono, "Cronograma", crono, xy, 0);
            jwObj_array("fields");
            {
                TABLE_FIELD(pc, "PC");
                TABLE_FIELD(instruc, "Instrucción");
            }
            jwEnd();
        }
        jwEnd();

    }
    jwEnd();

}

static void imprime_mem_instruc_json() {

    int i, index;
    dword PC;
    char str[128];
    char class[64];

    for (i = 0, PC = mmem->regiones[r_text].inicio; PC < mmem->regiones[r_text].final; i++, PC += 4) {
        te_etiqueta(str, PC);
        TABLE_ENTRY(mi, pc, i, string, value, str);
        imprime_instruccion_color(str, PC, NO);
        index = bloque_instruccion(PC) + 1;
        sprintf(class, "instruc-%d", index);
        TABLE_ENTRY_CLASS(mi, instruc, i, string, value, str, class);
    }

}

static void imprime_valores_iniciales() {

    int i;
    char str[64];

    inicio_ciclo_json(0);

    imprime_config_json();

    /* Reorder Buffer */
    foreach(i, TAM_REORDER) {
        TABLE_ENTRY(rob, id, i, int, value, i);
        ADD_ENTRY(rob, busy, i, "no");
    }

    /* Registers: integer */
    foreach(i, TAM_REGISTROS) {
        sprintf(str, "R%d", i);
        TABLE_ENTRY(rint, id, i, string, value, str);
        valor2str(str, Rint[i].valor, Rint_tipo[i]);
        ADD_ENTRY(rint, value, i, str);
    }

    /* Registers: fp */
    foreach(i, TAM_REGISTROS) {
        sprintf(str, "F%d", i);
        TABLE_ENTRY(rfp, id, i, string, value, str);
        valor2str(str, Rfp[i].valor, Rfp_tipo[i]);
        ADD_ENTRY(rfp, value, i, str);
    }

    /* RS */
    forall(i, RS_ENTEROS) {
        imprime_codigo(i, str);
        TABLE_ENTRY(rs, id, i, string, value, str);
        ADD_ENTRY(rs, busy, i, "no");
    }

    forall(i, RS_SUMREST) {
        imprime_codigo(i, str);
        TABLE_ENTRY(rs, id, i, string, value, str);
        ADD_ENTRY(rs, busy, i, "no");
    }

    forall(i, RS_MULTDIV) {
        imprime_codigo(i, str);
        TABLE_ENTRY(rs, id, i, string, value, str);
        ADD_ENTRY(rs, busy, i, "no");
    }

    forall(i, BUFFER_CARGA) {
        imprime_codigo(i, str);
        TABLE_ENTRY(tl, id, i - INICIO_BUFFER_CARGA, string, value, str);
        ADD_ENTRY(tl, busy, i - INICIO_BUFFER_CARGA, "no");
    }

    forall(i, BUFFER_ALMACEN) {
        imprime_codigo(i, str);
        TABLE_ENTRY(te, id, i - INICIO_BUFFER_ALMACEN, string, value, str);
        ADD_ENTRY(te, busy, i - INICIO_BUFFER_ALMACEN, "no");
    }

    // Memoria datos
    for (region_t r = 0; r < num_regiones; r++) {
        if (!es_region_datos(r) || tam_region(mmem, r) == 0)
            continue;

        muestra_datos_json(r, inicio_region(mmem, r), final_region(mmem, r), INITIAL_MEM);
    }

    // Memoria instrucciones
    imprime_mem_instruc_json();

    fin_ciclo_json(0);
}

static void imprime_Registros_json() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    int i;

    char text[128];
    char *class;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

    /* Banco de registros de enteros */

    foreach(i, TAM_REGISTROS) {
        class = "";

        if (Rint[i].rob != MARCA_NULA) {
            sprintf(text, "#%d", Rint[i].rob);
            class = "register-busy";
        } else {
            strcpy(text, "");
        } /* endif */

        FIRMA_2_1("R%d.rob", i, text);

        ADD_ENTRY_CLASS(rint, rob, i, text, class);

        valor2str(text, Rint[i].valor, Rint_tipo[i]);

        FIRMA_2_1("R%d.value", i, text);

        ADD_ENTRY_CLASS(rint, value, i, text, class);

    } /* endfor */

    /* Banco de registros de coma flotante */

    foreach(i, TAM_REGISTROS) {
        class = "";

        if (Rfp[i].rob != MARCA_NULA) {
            sprintf(text, "#%d", Rfp[i].rob);
            class = "register-busy";
        } else {
            strcpy(text, "");
        } /* endif */

        FIRMA_2_1("F%d.rob", i, text);

        ADD_ENTRY_CLASS(rfp, rob, i, text, class);

        valor2str(text, Rfp[i].valor, Rfp_tipo[i]);

        FIRMA_2_1("F%d.value", i, text);

        ADD_ENTRY_CLASS(rfp, value, i, text, class);

    } /* endfor */

}

static void imprime_ROB_json() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    int i;

    char str[128];

    char text[256];
    char *p_text;
    char *class;
    char class2[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

    foreach(i, TAM_REORDER) {
        /*** Previous value ***/
        if (RB[i].ocupado || (!RB[i].ocupado && RB[i].orden > 0)) {
            if (!RB[i].ocupado) {
                class = "entry-old";
            } else {
                class = "entry-busy";
            }

            p_text = (RB[i].ocupado) ? "sí" : "no";
            ADD_ENTRY_CLASS(rob, busy, i, p_text, class);

            imprime_instruccion_color(text, RB[i].PC, NO);
            int index = bloque_instruccion(RB[i].PC) + 1;
            sprintf(class2, "%s instruc-%d", class, index);
            ADD_ENTRY_CLASS(rob, instruc, i, text, class2);

            p_text = (RB[i].completado) ? "SÍ" : "NO";
            ADD_ENTRY_CLASS(rob, completed, i, p_text, class);

            if (inst_store(RB[i].OP)) {
                imprime_codigo(RB[i].dest, text);
                ADD_ENTRY_CLASS(rob, dest, i, text, class);
                ADD_ENTRY_CLASS(rob, value, i, "", class);
                ADD_ENTRY_CLASS(rob, pred, i, "", class);

            } else if (inst_salto(RB[i].OP)) {
                strcpy(text, "");
                if inst_jal(RB[i].OP) {
                    // SI es jal, ponemos "R31,direccion"
                    if (RB[i].completado) {
                        te_etiqueta(str, RB[i].direccion);
                        sprintf(text, "R" F_DWORD ",%s", RB[i].dest, str);
                    } else {
                        sprintf(text, "R" F_DWORD, RB[i].dest);
                    }
                } else if inst_jr(RB[i].OP) {
                    if (RB[i].completado) {
                        te_etiqueta(text, RB[i].direccion);
                    }
                } else { // Resto de saltos
                    te_etiqueta(text, RB[i].direccion);
                }
                ADD_ENTRY_CLASS(rob, dest, i, text, class);

                // [Valor,condicion] prediccion
                strcpy(text, "");
                if (RB[i].OP == OP_JAL) {
                    if (RB[i].completado) {
                        te_etiqueta(str, RB[i].valor.int_d);
                        sprintf(text, "%s,%s", str, (RB[i].condicion) ? "Salta" : "No salta");
                    }
                    ADD_ENTRY_CLASS(rob, value, i, text, class);

                    p_text = (RB[i].prediccion) ? "Salta" : "No salta";
                    ADD_ENTRY_CLASS(rob, pred, i, p_text, class);
                } else if (RB[i].OP == OP_JR) {
                    if (RB[i].completado) {
                        strcpy(text, (RB[i].condicion) ? "Fallo" : "Acierto");
                    }
                    ADD_ENTRY_CLASS(rob, value, i, text, class);

                    te_etiqueta(text, RB[i].pred_data.dir_destino);
                    ADD_ENTRY_CLASS(rob, pred, i, text, class);
                } else if (RB[i].OP == OP_JALR) {
                    if (RB[i].completado) {
                        te_etiqueta(str, RB[i].valor.int_d);
                        sprintf(text, "%s,%s", str, (RB[i].condicion) ? "Fallo" : "Acierto");
                    }
                    ADD_ENTRY_CLASS(rob, value, i, text, class);

                    te_etiqueta(text, RB[i].pred_data.dir_destino);
                    ADD_ENTRY_CLASS(rob, pred, i, text, class);
                } else { // Saltos condicionales
                    if (RB[i].completado) {
                        strcpy(text, (RB[i].condicion) ? "Salta" : "No salta");
                    }
                    ADD_ENTRY_CLASS(rob, value, i, text, class);

                    p_text = (RB[i].prediccion) ? "Salta" : "No salta";
                    ADD_ENTRY_CLASS(rob, pred, i, p_text, class);
                } /* endif */

            } else if (inst_fpsr(RB[i].OP)) {
                ADD_ENTRY_CLASS(rob, dest, i, "fpsr", class);

                if (RB[i].completado) {
                    p_text = (RB[i].valor.int_d) ? "Cierto" : "Falso";
                } else {
                    p_text = "";
                } /* endif */
                ADD_ENTRY_CLASS(rob, value, i, p_text, class);

                ADD_ENTRY_CLASS(rob, pred, i, "", class);
            } else if (RB[i].OP != OP_NOP && RB[i].OP != OP_TRAP) {
                sprintf(text, "%s" F_DWORD, (inst_int(RB[i].OP)) ? "R" : "F", RB[i].dest);

                strcpy(text, "");
                if (RB[i].completado) {
                    valor2str(text, RB[i].valor, obtener_tipo(RB[i].OP));
                }
                ADD_ENTRY_CLASS(rob, value, i, text, class);

                ADD_ENTRY_CLASS(rob, pred, i, "", class);
            } else {
                ADD_ENTRY_CLASS(rob, dest, i, "", class);
                ADD_ENTRY_CLASS(rob, value, i, "", class);
                ADD_ENTRY_CLASS(rob, pred, i, "", class);
            }

            if (RB[i].excepcion != EXC_NONE) {
                sprintf(text, "%d", RB[i].excepcion);
                ADD_ENTRY_CLASS(rob, excep, i, text, class);
            } else {
                ADD_ENTRY_CLASS(rob, excep, i, "", class);
            }

            te_etiqueta(text, RB[i].PC);
            ADD_ENTRY_CLASS(rob, pc, i, text, class);
        } else {
            ADD_ENTRY(rob, busy, i, "no");
            ADD_ENTRY(rob, instruc, i, "");
            ADD_ENTRY(rob, completed, i, "");
            ADD_ENTRY(rob, dest, i, "");
            ADD_ENTRY(rob, value, i, "");
            ADD_ENTRY(rob, pred, i, "");
            ADD_ENTRY(rob, excep, i, "");
            ADD_ENTRY(rob, pc, i, "");
        }
    }
}

static void imprime_RS_json() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    marca_t s;

    char str[128];

    char text[256];
    char *p_text;
    char *class;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

    /*** Estaciones de reserva de enteros ***/

    forall(s, ESTACION_RESERVA) {

        if (RS[s].ocupado || (!RS[s].ocupado && RS[s].orden > 0)) {
            if (!RS[s].ocupado && RS[s].orden > 0) {
                class = "entry-old";
            } else {
                class = "entry-busy";
            }

            p_text = (RS[s].ocupado) ? "sí" : "no";
            ADD_ENTRY_CLASS(rs, busy, s, p_text, class);

            imprime_operacion(RS[s].OP, text);
            ADD_ENTRY_CLASS(rs, op, s, text, class);

            if (RS[s].Q1 != MARCA_NULA) {
                sprintf(text, "#%d", RS[s].Q1);
                ADD_ENTRY_CLASS(rs, qj, s, text, class);
                ADD_ENTRY_CLASS(rs, vj, s, "", class);
            } else {
                ADD_ENTRY_CLASS(rs, qj, s, "", class);
                valor2str(text, RS[s].V1, obtener_tipo(RS[s].OP));
                ADD_ENTRY_CLASS(rs, vj, s, text, class);
            } /* endif */

            if (RS[s].Q2 != MARCA_NULA) {
                sprintf(text, "#%d", RS[s].Q2);
                ADD_ENTRY_CLASS(rs, qk, s, text, class);
                ADD_ENTRY_CLASS(rs, vk, s, "", class);
            } else {
                ADD_ENTRY_CLASS(rs, qk, s, "", class);
                valor2str(text, RS[s].V2, obtener_tipo(RS[s].OP));
                ADD_ENTRY_CLASS(rs, vk, s, text, class);
            } /* endif */

            sprintf(text, "#%d", RS[s].rob);
            ADD_ENTRY_CLASS(rs, rob, s, text, class);

            switch (RS[s].estado) {
                case EN_CURSO:
                    ADD_ENTRY_CLASS(rs, result, s, "", class);
                    sprintf(text, F_ESTADO, Op[RS[s].operador].ciclo, Op[RS[s].operador].Teval);
                    ADD_ENTRY_CLASS(rs, state, s, text, class);
                    break;
                case FINALIZADA:
                    if (!inst_salto(RS[s].OP)) { // Aritméticas
                        valor2str(text, RS[s].resultado, obtener_tipo(RS[s].OP));
                        ADD_ENTRY_CLASS(rs, result, s, text, class);
                    } else if inst_jr(RS[s].OP) {
                        te_etiqueta(str, RS[s].resultado.int_d);
                        sprintf(text, "%s,%s", ((RS[s].condicion) ? "Fallo" : "Acierto"), str);
                        ADD_ENTRY_CLASS(rs, result, s, text, class);
                    } else { // Saltos condicionales
                        p_text = (RS[s].condicion) ? "sí" : "no";
                        ADD_ENTRY_CLASS(rs, result, s, p_text, class);
                    }

                    ADD_ENTRY_CLASS(rs, state, s, "end", class);
                    break;
                default:
                    ADD_ENTRY_CLASS(rs, result, s, "", class);
                    ADD_ENTRY_CLASS(rs, state, s, "", class);
            } // end switch
        } else {
            ADD_ENTRY(rs, busy, s, "no");
            ADD_ENTRY(rs, op, s, "");
            ADD_ENTRY(rs, qj, s, "");
            ADD_ENTRY(rs, vj, s, "");
            ADD_ENTRY(rs, qk, s, "");
            ADD_ENTRY(rs, vk, s, "");
            ADD_ENTRY(rs, rob, s, "");
            ADD_ENTRY(rs, result, s, "");
            ADD_ENTRY(rs, state, s, "");
        } /* endif */

    } /* endfor */

}

static void imprime_LB_json() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    marca_t s;
    int i;

    char str[128];

    char text[256];
    char *p_text;
    char *class;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

        /*** Estaciones de reserva de enteros ***/

    forall(s, BUFFER_CARGA) {
        i = s - INICIO_BUFFER_CARGA;

        if (LB[s].ocupado || (!LB[s].ocupado && LB[s].orden > 0)) {
            if (!LB[s].ocupado && LB[s].orden > 0) {
                class = "entry-old";
            } else {
                class = "entry-busy";
            }

            p_text = (LB[s].ocupado) ? "sí" : "no";
            ADD_ENTRY_CLASS(tl, busy, i, p_text, class);

            imprime_operacion(LB[s].OP, text);
            ADD_ENTRY_CLASS(tl, op, i, text, class);

            if (LB[s].Q1 != MARCA_NULA) {
                sprintf(text, "#%d", LB[s].Q1);
                ADD_ENTRY_CLASS(tl, qj, i, text, class);
                ADD_ENTRY_CLASS(tl, vj, i, "", class);
            } else {
                ADD_ENTRY_CLASS(tl, qj, i, "", class);
                valor2str(text, LB[s].V1, t_dword);
                ADD_ENTRY_CLASS(tl, vj, i, text, class);
            } /* endif */

            sprintf(text, F_DWORD " (%s)", LB[s].desplazamiento, LB[s].etiqueta);
            ADD_ENTRY_CLASS(tl, desp, i, text, class);

            if (LB[s].estado != PENDIENTE) {
                te_etiqueta(text, LB[s].direccion);
                ADD_ENTRY_CLASS(tl, addr, i, text, class);
            } else {
                ADD_ENTRY_CLASS(tl, addr, i, "", class);
            }

            sprintf(text, "#%d", LB[s].rob);
            ADD_ENTRY_CLASS(tl, rob, i, text, class);

            switch (LB[s].estado) {
                case EN_CURSO:
                    ADD_ENTRY_CLASS(tl, result, i, "", class);
                    sprintf(text, F_ESTADO, Op[LB[s].operador].ciclo, Op[LB[s].operador].Teval);
                    ADD_ENTRY_CLASS(tl, state, i, text, class);
                    break;
                case FINALIZADA:
                    valor2str(text, LB[s].resultado, obtener_tipo(LB[s].OP));
                    ADD_ENTRY_CLASS(tl, result, i, text, class);
                    ADD_ENTRY_CLASS(tl, state, i, "end", class);
                    break;
                default:
                    ADD_ENTRY_CLASS(tl, result, i, "", class);
                    ADD_ENTRY_CLASS(tl, state, i, "", class);
            } // end switch
        } else {
            ADD_ENTRY(tl, busy, i, "no");
            ADD_ENTRY(tl, op, i, "");
            ADD_ENTRY(tl, qj, i, "");
            ADD_ENTRY(tl, vj, i, "");
            ADD_ENTRY(tl, desp, i, "");
            ADD_ENTRY(tl, addr, i, "");
            ADD_ENTRY(tl, rob, i, "");
            ADD_ENTRY(tl, result, i, "");
            ADD_ENTRY(tl, state, i, "");
        } /* endif */

    } /* endfor */
}

static void imprime_SB_json() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    marca_t s;
    int i;

    char str[128];

    char text[128];
    char *p_text;
    char *class;
    char class2[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

    /*** Estaciones de reserva de enteros ***/


    forall(s, BUFFER_ALMACEN) {
        i = s - INICIO_BUFFER_ALMACEN;


        if (SB[s].ocupado || (!SB[s].ocupado && SB[s].orden > 0)) {
            if (!SB[s].ocupado && SB[s].orden > 0) {
                class = "entry-old";
            } else {
                class = "entry-busy";
            }

            p_text = (SB[s].ocupado) ? "sí" : "no";
            ADD_ENTRY_CLASS(te, busy, i, p_text, class);

            imprime_operacion(SB[s].OP, text);
            ADD_ENTRY_CLASS(te, op, i, text, class);

            if (SB[s].Q1 != MARCA_NULA) {
                sprintf(text, "#%d", SB[s].Q1);
                ADD_ENTRY_CLASS(te, qj, i, text, class);
                ADD_ENTRY_CLASS(te, vj, i, "", class);
            } else {
                ADD_ENTRY_CLASS(te, qj, i, "", class);
                valor2str(text, SB[s].V1, t_dword);
                ADD_ENTRY_CLASS(te, vj, i, text, class);
            } /* endif */

            sprintf(text, F_DWORD " (%s)", LB[s].desplazamiento, LB[s].etiqueta);
            ADD_ENTRY_CLASS(te, desp, i, text, class);

            if (LB[s].estado != PENDIENTE) {
                te_etiqueta(text, LB[s].direccion);
                ADD_ENTRY_CLASS(te, addr, i, text, class);
            } else {
                ADD_ENTRY_CLASS(te, addr, i, "", class);
            }

            if (SB[s].Q2 != MARCA_NULA) {
                sprintf(text, "#%d", SB[s].Q2);
                ADD_ENTRY_CLASS(te, qk, i, text, class);
                ADD_ENTRY_CLASS(te, vk, i, "", class);
            } else {
                ADD_ENTRY_CLASS(te, qk, i, "", class);
                valor2str(text, SB[s].V2, obtener_tipo(SB[s].OP));
                ADD_ENTRY_CLASS(te, vk, i, text, class);
            } /* endif */

            sprintf(text, "#%d", SB[s].rob);
            ADD_ENTRY_CLASS(te, rob, i, text, class);

            p_text = (SB[s].confirm) ? "sí" : "no";
            ADD_ENTRY_CLASS(te, confirm, i, p_text, class);

            switch (SB[s].estado) {
                case EN_CURSO:
                    sprintf(text, F_ESTADO, Op[SB[s].operador].ciclo, Op[SB[s].operador].Teval);
                    ADD_ENTRY_CLASS(te, state, i, text, class);
                    break;
                case FINALIZADA:
                    ADD_ENTRY_CLASS(te, state, i, "end", class);
                    break;
                default:
                    ADD_ENTRY_CLASS(te, state, i, "", class);
            } // end switch
        } else {
            ADD_ENTRY(te, busy, i, "no");
            ADD_ENTRY(te, op, i, "");
            ADD_ENTRY(te, qj, i, "");
            ADD_ENTRY(te, vj, i, "");
            ADD_ENTRY(te, desp, i, "");
            ADD_ENTRY(te, addr, i, "");
            ADD_ENTRY(te, qk, i, "");
            ADD_ENTRY(te, vk, i, "");
            ADD_ENTRY(te, confirm, i, "");
            ADD_ENTRY(te, state, i, "");
        } /* endif */

    } /* endfor */
}


/***** Funciones Exportables **********************************/

/***************************************************************
 *
 * Func: init_instruc
 *
 * Description:
 *   Inicializa una instrucción en el cronograma
 *
 **************************************************************/

void init_instruc_json
(
        dword PC,
        ciclo_t orden
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    int index;
    char texto[128];
    char clase[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    if (!output_active) return;

    CRONO_PC(orden, PC);

    imprime_instruccion_color(texto, PC, NO);
    index = bloque_instruccion(PC) + 1;
    sprintf(clase, "instruc-%d", index);
    CRONO_INSTRUC(orden, texto, clase);

} /* end init_instruc */

/***************************************************************
 *
 * Func: muestra_fase
 *
 * Description:
 *   Muestra la fase de cada instrucción en el cronograma
 *
 **************************************************************/

void muestra_fase_json(
        char *fase,
        ciclo_t orden,
        boolean exception
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    int o, c;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    if (!output_active) return;

    CRONO_PHASE(Ciclo, orden, fase, exception);

} /* end muestra_fase */

/***************************************************************
 *
 * Func: muestra_datos
 *
 * Description:
 *   Muestra parte del contenido de la memoria de datos.
 *
 **************************************************************/

void muestra_datos_json(
        region_t r,
        dword from,
        dword to,
        mem_status_t status
        ) {
    dword d;
    char *section;
    char text[64];
    char label[64];
    char style[64];
    valor_t valor;
    size_t tam_elem;
    boolean dirty;

    if (!output_active) return;

    if (status == INITIAL_MEM) {
        for (d = from; d < to; d += sizeof (dword)) {
            int index;

            if (d % sizeof (dword) == 0) {
                jwArr_object();
                sprintf(label, "m%s-addr-" F_DWORD, mmem->regiones[r].nombre, indice_dword(dir_a_offset(mmem, d, r)));
                jwObj_string("id", label);

                te_etiqueta(text, d);
                jwObj_string("value", text);

                ts_busca_direccion(programa.simbolos, d, &index);
                index %= NUM_STYLES;
                sprintf(style, "datos-%d", index);

                jwObj_string("class", style);
                jwEnd();
            }
        }
    }

    if (status == OLD_MEM) {
        section = "prev";
    } else {
        section = "value";
    }

    if (status == OLD_MEM || status == INITIAL_MEM) {
        jwArr_object();
        sprintf(label, "m%s-data-" F_DWORD, mmem->regiones[r].nombre, indice_dword(dir_a_offset(mmem, from, r)));
        jwObj_string("id", label);
    }

    jwObj_array(section);

    for (d = from; d < to; d += tam_elem) {
        tipo_t tipo_dato;

        if (d % sizeof (dword) == 0 && d > from) {
            jwEnd();
            jwEnd();

            jwArr_object();
            sprintf(label, "m%s-data-" F_DWORD, mmem->regiones[r].nombre, indice_dword(dir_a_offset(mmem, d, r)));
            jwObj_string("id", label);
            jwObj_array(section);
        }


        tipo_dato = md_tipo[r][dir_a_offset(mmem, d, r)];
        tam_elem = tam_dato(tipo_dato);
        valor = lee_mem_datos(mmem, d, tipo_dato, &dirty);
        valor2str(text, valor, tipo_dato);

        jwArr_object();

        jwObj_int("index", d % sizeof (dword));
        jwObj_string("text", text);
        jwObj_int("size", tam_elem);

        jwEnd();
    }

    jwEnd();

    if (status == NEW_MEM || status == INITIAL_MEM) {
        jwEnd();
    }

} /* end muestra_datos_json */


#if 0

/***************************************************************
 *
 * Func: imprime_DMem
 *
 * Desc: Imprime la memoria de datos
 *
 **************************************************************/

static void imprime_DMem_json() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i;
    int next;

    char aux[128];
    char color[64];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

    fprintf(f_salida, "<table>\n");


    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"50\" align=\"center\">Dir</td>\n");
    for (i = 0; i < 4; i++) {
        fprintf(f_salida, "  <td width=\"20\" align=\"center\">+%d</td>\n", i);
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    next = mmem->regiones[r_data].inicio;
    for (i = mmem->regiones[r_data].inicio; i < mmem->regiones[r_data].final; i++) {

        if (i % sizeof (word) == 0) {
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            te_etiqueta(aux, i);
            color_etiqueta_memdatos(color, i);
            fprintf(f_salida, "  <td align=\"left\" nowrap='nowrap' bgcolor=\"#ddeeff\">"
                    "<span style='color: %s; margin: 0 0.5em'>%s</span></td>\n", color, aux);
        } /* endif */

        if (i == next) {
            switch (md_tipo[i]) {
                case t_byte:
                    FIRMA_2_2("MD%d", i, "%hhX", MD.m_byte[indice_byte(i)]);
                    /* Imprime también el caracter */
                    sprintf(aux, "%s %c", vstr, MD.m_byte[indice_byte(i)]);
                    fprintf(f_salida,
                            "  <td align=\"center\">%s</td>\n", aux);
                    next = i + sizeof (byte);
                    break;
                case t_half:
                    FIRMA_2_2("MD%d", i, "%hd", MD.m_half[indice_half(i)]);
                    fprintf(f_salida,
                            "  <td align=\"center\" colspan=\"2\">%s</td>\n", vstr);
                    next = i + sizeof (half);
                    break;
                case t_word:
                    FIRMA_2_2("MD%d", i, "%d", MD.m_word[indice_word(i)]);
                    fprintf(f_salida,
                            "  <td align=\"center\" colspan=\"4\">%s</td>\n", vstr);
                    next = i + sizeof (word);
                    break;
                case t_dword:
                    FIRMA_2_2("MD%d", i, F_DWORD, MD.m_dword[indice_dword(i)]);
                    fprintf(f_salida,
                            "  <td align=\"center\" rowspan=\"2\" colspan=\"4\">%s</td>\n", vstr);
                    next = i + sizeof (dword);
                    break;
                case t_float:
                    FIRMA_2_2("MD%d", i, "%.2f", MD.m_float[indice_float(i)]);
                    fprintf(f_salida,
                            "  <td align=\"center\" colspan=\"4\">%s</td>\n", vstr);
                    next = i + sizeof (float);
                    break;
                case t_double:
                    FIRMA_2_2("MD%d", i, "%.2f", MD.m_double[indice_double(i)]);
                    fprintf(f_salida,
                            "  <td align=\"center\" rowspan=\"2\" colspan=\"4\">%s</td>\n", vstr);
                    next = i + sizeof (double);
                    break;
                default:
                    fprintf(f_salida, "  <td>&nbsp;</td>\n");
                    next = i + 1;
            } /* endswitch */
        } /* endif */
        if ((i + 1) % sizeof (word) == 0) {
            fprintf(f_salida, " </tr>\n");
        } /* endif */
    } /* endif */

    fprintf(f_salida, "</table><br>\n");
}
#endif

/***************************************************************
 *
 * Func: imprime_inicio
 *
 * Desc: Imprime el cronograma de ejecución
 *
 **************************************************************/

void imprime_inicio_json
(
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i;

    char aux[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    strcpy(aux, "data.js");

    f_salida = fopen(aux, "w");
    if (f_salida == NULL) {
        fprintf(stderr, "Error creando %s\n", aux);
        exit(1);
    } /* endif */

    output_active = SI;

    jwOpen(f_salida, JW_OBJECT, JW_PRETTY);

    /*** Información general ***/

    jwObj_object("info");
    jwObj_string("file", nombre_fich);
    jwObj_string("author", "Arquitectura e Ingeniería de Computadores");
    jwEnd();

    /*** Datos de la implementación ***/

    // imprime_config_json();

    /*** Datos de la visualización ***/

    imprime_estructuras_json();

    /*** Contenidos iniciales ***/

    // Memoria instrucciones

    /*** Comienzo de la lista de valores de la ejecución ***/

    jwObj_array("cycles");

    // Imprime los valores iniciales y los nombres de las estructuras como los valores del ciclo 0

    imprime_valores_iniciales();

} /* end imprime_inicio */

void inicio_ciclo_json(ciclo_t ciclo) {

    if (ciclo > MAX_CICLOS_HTML) {
        output_active = NO;
    }

    if (!output_active) return;

    /*** Inicio del siguiente ciclo de simulación ***/

    jwArr_object();
    jwObj_int("cycle", ciclo);
    jwObj_array("values");
}

void fin_ciclo_json(ciclo_t ciclo) {

    if (!output_active) return;

    /*** Fin del ciclo de simulación ***/

    jwEnd(); // Cierra values

    jwEnd(); // Cierra el ciclo actual
}

/***************************************************************
 *
 * Func: imprime_final
 *
 * Desc: Imprime el cronograma de ejecución
 *
 **************************************************************/

void imprime_final_json
(
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i;
    int next;

    char aux[128];
    char color[64];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /*** Final de la lista de valores de la ejecución ***/

    jwEnd();

    /*** Información estadística ***/


    /*** Cierre del fichero de datos ***/

    jwClose();

    fputc('\n', f_salida);
    fclose(f_salida);

} /* end imprime_final */

/***************************************************************
 *
 * Func: imprime_estado
 *
 * Desc: Imprime el estado de las estructuras de coma flotante
 *
 **************************************************************/

void imprime_estado_json
(
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /*** Bancos de registros ***/

    imprime_Registros_json();

    /*** Reorder buffer ***/

    imprime_ROB_json();

    /*** Estaciones de reserva ***/

    imprime_RS_json();

    /*** Tampones de lectura ***/

    imprime_LB_json();
    imprime_SB_json();

    /*** Contenido de la memoria de datos ***/

    // imprime_DMem_json();

    if (error_firma) {
        fprintf(stderr, "ERROR. "
                "Existen diferencias entre la ejecución actual y el fichero de firmas.\n"
                "Analice el estado en el ciclo " F_CICLO "\n", Ciclo);
        exit(1);
    } /* endif */

} /* end imprime_estado */

#if 0

/***************************************************************
 *
 * Func: imprime_predictor
 *
 * Desc: Imprime el estado del predictor
 *
 **************************************************************/

void imprime_predictor_json
(
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i, j;

    char aux[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    sprintf(aux, "predictor%03ld.json", Ciclo);
    if (json_merge == SI) {
        fprintf(f_salida, "'%s':`", aux);
    } else {
        f_salida = fopen(aux, "w");
        if (f_salida == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */

    nueva_firma(); /*** FIRMAS ***/

    /*** json INTRO ***/

    fprintf(f_salida,
            "<json><head><title>Predictor: %s. Ciclo %lu</title></head>\n",
            nombre_fich, Ciclo);
    fprintf(f_salida,
            "<body bgcolor=\"white\">\n");

    fprintf(f_salida, "<a href=\"index.json\">INICIO</a>&nbsp;\n");
    fprintf(f_salida, "<a href=\"final.json\">FINAL</a>&nbsp;\n");
    if (Ciclo > 10)
        fprintf(f_salida, "<a href=\"predictor%03ld.json\">[-10]</a>&nbsp;\n",
            Ciclo - 10);
    else
        fprintf(f_salida, "[-5]&nbsp;\n");
    if (Ciclo > 5)
        fprintf(f_salida, "<a href=\"predictor%03ld.json\">[-5]</a>&nbsp;\n",
            Ciclo - 5);
    else
        fprintf(f_salida, "[-5]&nbsp;\n");
    if (Ciclo > 1)
        fprintf(f_salida, "<a href=\"predictor%03ld.json\">[-1]</a>&nbsp;\n",
            Ciclo - 1);
    else
        fprintf(f_salida, "[-1]\n");

    if (!final)
        fprintf(f_salida, "<a href=\"predictor%03ld.json\">[+1]</a>\n", Ciclo + 1);
    if (!final)
        fprintf(f_salida, "<a href=\"predictor%03ld.json\">[+5]</a>\n", Ciclo + 5);
    if (!final)
        fprintf(f_salida, "<a href=\"predictor%03ld.json\">[+10]</a>\n", Ciclo + 10);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"estado%03ld.json\">Estado</a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"crono%03ld.json\">Crono</a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<!a href=\"predictor%03ld.json\">BTB<!/a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<tt><b>Programa: %s</b></tt>\n", nombre_fich);
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<b>Ciclo: %lu</b>\n", Ciclo);
    fprintf(f_salida, "<br>\n");

    fprintf(f_salida, "<p>Estado al final del ciclo\n");

    /*  Con propósitos de depuración. Funciona con 1 vía */
    if (NUM_VIAS_ISSUE == 1) {
        if (inst_salto(IF_ISS_1.IR.codop)) {
            imprime_instruccion_color(aux, IF_ISS_1.PC, NO);
            fprintf(f_salida, "<p>IF: PC= "
                    F_DWORD
                    "; Instr=%s; Prediccion=%d; BHR local=%d; BHR global=%d\n",
                    IF_ISS_1.PC, aux, IF_ISS_1.prediccion, IF_ISS_1.pred_data.BHR_local, IF_ISS_1.pred_data.BHR_global);
            fprintf(f_salida, "<p>    Pred local=%d; Pred global=%d\n", IF_ISS_1.pred_data.pred_local,
                    IF_ISS_1.pred_data.pred_global);

        }

        if (inst_salto(RB[RB_inicio].OP) && (RB[RB_inicio].ocupado) && (RB[RB_inicio].completado)) {
            imprime_instruccion_color(aux, RB[RB_inicio].PC, NO);
            fprintf(f_salida,
                    "<p>&rarr;C: PC="
                    F_DWORD
                    "; Instr=%s, Prediccion=%d; Condicion=%d; BHR global=%d; BHR local=%d\n",
                    RB[RB_inicio].PC, aux, RB[RB_inicio].prediccion, RB[RB_inicio].condicion,
                    RB[RB_inicio].pred_data.BHR_global, RB[RB_inicio].pred_data.BHR_local);
            fprintf(f_salida, "<p>    Pred local=%d; Pred global=%d\n", RB[RB_inicio].pred_data.pred_local,
                    RB[RB_inicio].pred_data.pred_global);
        }
    }

    if ((tipo_predictor == CLARIVIDENTE) || (tipo_predictor == BTB_1_BIT) ||
            (tipo_predictor == BTB_2_BIT_H) || (tipo_predictor == BTB_2_BIT_S)) {

        /* Branch Target Buffer 1 o 2 bits */
        /* Mascara global */

        fprintf(f_salida, "<p>BTB\n");
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">BHR local: </td>\n");
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
        itoa(branch_mask_local, aux, 2);
        FIRMA_1_2("Branch Mask Local", "%0x", branch_mask_local);
        fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s(%d)</td>\n", aux, branch_mask_local);
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, "</table>\n");

        /* BTB */
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PC (instr.)</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Estado</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Dir. Destino</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">(Ultimo acceso)</td>\n");
        fprintf(f_salida, " </tr>\n");

        for (i = 0; i < TAM_BUFFER_PREDIC; i++) {
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");

            if (BTB[i].orden > 0) { /* Entrada ocupada */
                te_etiqueta(aux, BTB[i].PC);

                FIRMA_2_1("BTB%d.pc", i, aux);
                imprime_instruccion(aux, BTB[i].PC);
                fprintf(f_salida, "  <td nowrap='nowrap'>%s (%s)</td>\n", vstr, aux);

                imprime_estado_predictor_json(aux, BTB[i].estado);
                FIRMA_2_1("BTB%d.estado", i, aux);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


                te_etiqueta(aux, BTB[i].destino);

                FIRMA_2_1("BTB%d.destino", i, aux);
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

                FIRMA_2_2("BTB%d.orden", i, "%lu", BTB[i].orden);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } else {
                FIRMA_2_1("BTB%d.pc", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("BTB%d.estado", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


                FIRMA_2_1("BTB%d.destino", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("BTB%d.orden", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            fprintf(f_salida, " </tr>\n");
        } /* endfor */

        fprintf(f_salida, "</table>\n");
    } else {

        /* Branch Target Buffer n bits */
        /* Mascara global */

        fprintf(f_salida, "<p>BTB\n");
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">BHR local: </td>\n");
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
        itoa(branch_mask_local, aux, 2);
        FIRMA_1_2("Branch Mask Local", "%0x", branch_mask_local);
        fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s(%d)</td>\n", aux, branch_mask_local);
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, "</table>\n");

        /* BTB */
        fprintf(f_salida, "<table>\n");
        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"150\">PC (instr.)</td>\n");

        /* Vector de estado. No se muestra en el predictor global */
        if ((tipo_predictor != GLOBAL_n_BIT_S) && (tipo_predictor != GLOBAL_n_BIT_H)) {
            for (j = 0; j < pwrtwo(n_bits_branch_mask_local); j++) {
                fprintf(f_salida, "  <td align=\"center\" width=\"150\">V. Estado [%d]</td>\n", j);
            }
        }

        /* Selector. Se muestra en el predictor híbrido */
        if (tipo_predictor == HYBRID_n_BIT_S) {
            fprintf(f_salida, "  <td align=\"center\" width=\"150\">Selector</td>\n");
        }
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">Dir. Destino</td>\n");
        fprintf(f_salida, "  <td align=\"center\" width=\"150\">(Ultimo acceso)</td>\n");
        fprintf(f_salida, " </tr>\n");

        for (i = 0; i < TAM_BUFFER_PREDIC; i++) {
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");

            if (BTB[i].orden > 0) { /* Entrada ocupada */
                te_etiqueta(aux, BTB[i].PC);

                FIRMA_2_1("BTB%d.pc", i, aux);
                imprime_instruccion(aux, BTB[i].PC);
                fprintf(f_salida, "  <td nowrap='nowrap'>%s (%s)</td>\n", vstr, aux);

                //imprime_estado_predictor(aux, BTB[i].estado);
                //FIRMA_2_1("BTB%d.estado", i, aux);
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


                /* Vector de estado. No se muestra en el predictor global */
                if ((tipo_predictor != GLOBAL_n_BIT_S) && (tipo_predictor != GLOBAL_n_BIT_H)) {
                    for (j = 0; j < pwrtwo(n_bits_branch_mask_local); j++) {
                        imprime_estado_predictor_n_json(aux, BTB[i].v_estado[j], n_bits_pred_local);
                        // Faltaria una nueva firma
                        FIRMA_2_1("BTB%d.v_estado", i, aux);
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    }
                }

                /* Selector. Se muestra en el predictor híbrido */
                if (tipo_predictor == HYBRID_n_BIT_S) {
                    imprime_estado_selector_n_json(aux, BTB[i].estado_selector);
                    FIRMA_1_1("Hybrid%d.estado", aux);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                }

                te_etiqueta(aux, BTB[i].destino);
                FIRMA_2_1("BTB%d.destino", i, aux);
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

                FIRMA_2_2("BTB%d.orden", i, "%lu", BTB[i].orden);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } else {
                FIRMA_2_1("BTB%d.pc", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                //FIRMA_2_1("BTB%d.estado", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

                if ((tipo_predictor != GLOBAL_n_BIT_S) && (tipo_predictor != GLOBAL_n_BIT_H)) {
                    for (j = 0; j < pwrtwo(n_bits_branch_mask_local); j++) {
                        // Faltaria una nueva firma
                        FIRMA_2_1("BTB%d.v_estado", i, "&nbsp;");
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    }
                }

                /* Selector. Se muestra en el predictor híbrido */
                if (tipo_predictor == HYBRID_n_BIT_S) {
                    FIRMA_1_1("Hybrid%d.estado", "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                }

                FIRMA_2_1("BTB%d.destino", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("BTB%d.orden", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            fprintf(f_salida, " </tr>\n");
        } /* endfor */

        fprintf(f_salida, "</table>\n");


        if ((tipo_predictor == GLOBAL_n_BIT_H) || (tipo_predictor == GLOBAL_n_BIT_S) ||
                (tipo_predictor == HYBRID_n_BIT_S)) {

            /* Global */
            fprintf(f_salida, "<p>Predictor global\n");
            fprintf(f_salida, "<table>\n");
            fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
            fprintf(f_salida, "  <td width=\"150\">BHR global: </td>\n");
            fprintf(f_salida, " </tr>\n");
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            itoa(branch_mask_global, aux, 2);
            FIRMA_1_2("Branch Mask Global", "%0x", branch_mask_global);
            fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s(%d)</td>\n", aux, branch_mask_global);
            fprintf(f_salida, " </tr>\n");
            fprintf(f_salida, "</table>\n");

            fprintf(f_salida, "<table>\n");
            fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
            for (j = 0; j < pwrtwo(n_bits_branch_mask_global); j++) {
                fprintf(f_salida, "  <td align=\"center\" width=\"150\">V. Estado [%d]</td>\n", j);
            }
            fprintf(f_salida, " </tr>\n");
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            for (j = 0; j < pwrtwo(n_bits_branch_mask_global); j++) {
                imprime_estado_predictor_n_json(aux, Global[j], n_bits_pred_global);

                FIRMA_2_1("Global%d", j, aux);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            }
            fprintf(f_salida, " </tr>\n");
            fprintf(f_salida, "</table>\n");
        }
    }

    fprintf(f_salida, "<address>Arquitectura e Ingenier&iacute;a de Computadores</address>\n");
    fprintf(f_salida, "</body></json>\n");

    if (json_merge == SI) {
        fputs("`,", f_salida);
    } else {
        fclose(f_salida);
    } /* endif */

    if (error_firma) {

        fprintf(stderr, "ERROR. "
                "Existen diferencias entre la ejecución actual y el fichero de firmas.\n"
                "Analice el fichero de estado 'estado%03ld.json'\n", Ciclo);
        exit(1);
    } /* endif */

} /* end imprime_predictor */
#endif
