/**************************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politecnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *
 * File: presentacion.c
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

#define presentacion_C

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memoria.h>
#include <enlazador.h>
#include <simbolos.h>

/***** Definiciones Externas **********************************/

#include "tipos.h"
#include "main.h"
#include "instrucciones.h"
#include "presentacion.h"
#include "memoria.h"
#include "registros.h"
#include "depurar.h"

typedef enum {
    MF_OCIOSO = 0,
    MF_ESCRITURA = 1,
    MF_LECTURA = 2
} modo_firmas_t;


/***** Variables Globales **************************************/

presentacion_interface_t presentacion = {
    .imprime_inicio_f = imprime_inicio_null,
    .imprime_final_f = imprime_final_null,
    .inicio_ciclo_f = inicio_ciclo_null,
    .fin_ciclo_f = fin_ciclo_null,
    .imprime_estado_f = imprime_estado_null,
    .imprime_crono_f = imprime_crono_null,
    .imprime_predictor_f = imprime_predictor_null,
    .init_instruc_f = init_instruc_null,
    .muestra_fase_f = muestra_fase_null,
    .muestra_datos_f = muestra_datos_null
};

/***** Variables Locales **************************************/

static FILE *f_firmas;
static modo_firmas_t modo_firmas = MF_OCIOSO;
static char linea_firma[LONG_FIRMA];

/***** Macros *************************************************/

/***** Funciones Locales **************************************/

/* reverse:  reverse string s in place */
void reverse(char s[]) {
    int i, j;
    char c;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* itoa:  convert n to characters in s */
void itoa(int n, char s[], int radix) {
    int i, sign;

    if ((sign = n) < 0) /* record sign */
        n = -n; /* make n positive */
    i = 0;
    do { /* generate digits in reverse order */
        s[i++] = n % radix + '0'; /* get next digit */
    } while ((n /= radix) > 0); /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

/***************************************************************
 *
 * Func: crea_firma
 *
 * Description:
 *   Crea un fichero de firmas
 *
 **************************************************************/

void crea_firma
(
        char *fichero
        ) {
    /*************************************/
    /*  Function body                    */
    /*************************************/

    f_firmas = fopen(fichero, "w");
    if (f_firmas == NULL) {
        fprintf(stderr, "Error creando %s\n", fichero);
        exit(1);
    } /* endif */

    modo_firmas = MF_ESCRITURA;
    fprintf(f_firmas, FIRMA_DEL_FICHERO);

} /* end crea_firma */

/***************************************************************
 *
 * Func: abre_firma
 *
 * Description:
 *   Abre un fichero de firmas ya existente
 *
 **************************************************************/

void abre_firma
(
        char *fichero
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char *lin;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    f_firmas = fopen(fichero, "r");
    if (f_firmas == NULL) {
        fprintf(stderr, "Error abriendo %s\n", fichero);
        exit(1);
    } /* endif */

    modo_firmas = MF_LECTURA;

    /*** Comprueba que es un fichero de firmas ***/

    lin = fgets(linea_firma, LONG_FIRMA, f_firmas);
    if (lin == NULL) {
        fprintf(stderr, "Error leyendo el fichero de firmas\n");
        exit(1);
    } /* endif */

    printf(" Leido %s\n", FIRMA_DEL_FICHERO);
    /* Comprueba que la firma es correcta */
    if (strcmp(linea_firma, FIRMA_DEL_FICHERO "\n")) {
        fprintf(stderr, "ERROR. El fichero NO es un fichero de firmas.\n");
        exit(1);
    } /* endif */

} /* end abre_firma */

/***************************************************************
 *
 * Func: ignora_firma
 *
 * Description:
 *   Pasa a modo ocioso
 *
 **************************************************************/

void  ignora_firma () {
  modo_firmas = MF_OCIOSO;
}

/***************************************************************
 *
 * Func: nueva_firma
 *
 * Description:
 *   Comienza la firma de un nuevo ciclo
 *
 **************************************************************/

void nueva_firma
(
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char *tok;
    char s_ciclo[8];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    sprintf(s_ciclo, F_CICLO, Ciclo);

    switch (modo_firmas) {
        case MF_LECTURA:
        {
            char *lin;
            lin = fgets(linea_firma, LONG_FIRMA, f_firmas);
            if (lin == NULL) {
                fprintf(stderr, "Error leyendo el fichero de firmas\n");
                exit(1);
            } /* endif */

            /* Comprueba que el ciclo es el correcto */
            tok = strtok(linea_firma, "@");
            if (strcmp(tok, s_ciclo)) {
                fprintf(stderr, "ERROR. "
                        "El ciclo actual (%s) no coincide con el ciclo del fichero de firmas (%s).\n"
                        "Seguramente no deberías haber alcanzado este ciclo\n", s_ciclo, tok);
                exit(1);
            } /* endif */
        }
            break; /* endcase */
        case MF_ESCRITURA:
        {
            fprintf(f_firmas, "\n%s@", s_ciclo);
        }
            break; /* endcase */
        default:
            break;
    } /* endswitch */

} /* end nueva_firma */

/***************************************************************
 *
 * Func: procesa_firma
 *
 * Description:
 *   MF_ESCRITURA: Añade un trozo de la firma correspondiente al ciclo actual.
 *   MF_LECTURA: Comprueba si uno de los campos de la firma es correcto.
 *
 *   En cualquier caso devuelve la cadena que se debe insertar
 *   como valor.
 *
 **************************************************************/

char *procesa_firma
(
        char *campo,
        char *valor
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char *tok;
    static char msg[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    strcpy(msg, valor);

    switch (modo_firmas) {
        case MF_LECTURA:
            tok = strtok(NULL, "$");
            if (tok == NULL) {
                fprintf(stderr, "%s: %s\n", campo, valor);
                fprintf(stderr, "Error leyendo el fichero de firmas: %d\n", __LINE__);
                exit(1);
            } /* endif */
            if (strcmp(tok, campo)) {
                fprintf(stderr, "ERROR. "
                        "Los nombres de los campos no coinciden con los del fichero de firmas.\n"
                        "Probablemente exista un error en la configuración del simulador\n");
                exit(1);
            } /* endif */

            tok = strtok(NULL, "@");
            if (tok == NULL) {
                fprintf(stderr, "%s: %s\n", campo, valor);
                fprintf(stderr, "Error leyendo el fichero de firmas: %d\n", __LINE__);
                exit(1);
            } /* endif */
            if (strcmp(tok, valor)) {
                if (!strcmp(valor, "&nbsp;"))
                    strcpy(msg, "<i><font color=\"red\"><b> ??? </b></font></i>");
                    /*
                    else if (!strcmp(valor, "")) // Campos alfanuméricos
                        strcpy(msg, "<i><font color=\"red\"><b> ??? </b></font></i>");
                     */
                else
                    sprintf(msg, "<i><font color=\"red\"><b>%s</b></font></i>", valor);

                error_firma = SI;
                return msg;
            } /* endif */
            break;
        case MF_ESCRITURA:
            fprintf(f_firmas, "%s$%s@", campo, valor);
            break;
        default:
            break;
    } /* endswitch */

    return msg;

} /* end procesa_firma */

/***************************************************************
 *
 * Func: cierra_firma
 *
 * Description:
 *   Cierra el fichero de firmas
 *
 **************************************************************/

void cierra_firma
(
        ) {
    /*************************************/
    /*  Function body                    */
    /*************************************/

    fprintf(f_firmas, "\n");
    fclose(f_firmas);

} /* end cierra_firma */

void color_etiqueta_memdatos(
        char *color,
        dword direccion
        ) {
    int index;

    ts_busca_direccion(programa.simbolos, direccion, &index);

    static char *paleta[15] = {
        "red", "darkcyan", "orange", "limegreen", "deeppink", "royalblue", "brown", "seagreen",
        "lightcoral", "magenta", "steelblue", "salmon", "mediumseagreen", "darkorange", "orchid"
    };

    strcpy(color, paleta[index % 15]);

}

/***************************************************************
 *
 * Func: imprime_instruccion
 *
 * Parametros:
 *	instruc  (char *) ;
 *
 * Descripción:
 *   Imprime la intrucción localizada en el PC sobre la
 *   cadena de caracteres 'instruc'
 *
 **************************************************************/

void __imprime_instruccion
(
        char *instruc,
        dword PC,
        char *file,
        int line
        ) {

    if (PC % 4 != 0) {
        fprintf(stderr, "%s:%d ERROR: Dirección incorrecta al imprimir una instrucción (" F_DWORD ")\n", file, line,
                PC);
    }

    if (!en_region(mmem, PC, r_text)) {
        sprintf(instruc, "&lt;nop&gt;");
        return;
    } /* endif */

    imprime_instruccion_color(instruc, PC, SI);

} /* end imprime_instruccion */

/***************************************************************
 *
 * Func: imprime_codigo
 *
 * Description:
 *   Obtiene el nombre de una estación de reserva
 *
 **************************************************************/

void imprime_codigo
(
        marca_t s,
        char *nom
        ) {
    /*************************************/
    /*  Function body                    */
    /*************************************/

    if (/* s >= INICIO_RS_ENTEROS && */ /* Es siempre cierto en está
                                           implementación */
            s <= FIN_RS_ENTEROS) {
        sprintf(nom, "e%d", s - INICIO_RS_ENTEROS + 1);
    } else if (s >= INICIO_RS_SUMREST &&
            s <= FIN_RS_SUMREST) {
        sprintf(nom, "a%d", s - INICIO_RS_SUMREST + 1);
    } else if (s >= INICIO_RS_MULTDIV &&
            s <= FIN_RS_MULTDIV) {
        sprintf(nom, "m%d", s - INICIO_RS_MULTDIV + 1);
    } else if (s >= INICIO_BUFFER_CARGA &&
            s <= FIN_BUFFER_CARGA) {
        sprintf(nom, "l%d", s - INICIO_BUFFER_CARGA + 1);
    } else if (s >= INICIO_BUFFER_ALMACEN &&
            s <= FIN_BUFFER_ALMACEN) {
        sprintf(nom, "s%d", s - INICIO_BUFFER_ALMACEN + 1);
    } else {
        nom[0] = '\0'; /* Invalido o marca nula */
    } /* endif */

} /* end imprime_codigo */

/***************************************************************
 *
 * Func: imprime_operacion
 *
 * Description:
 *   Obtiene el nombre de una estación de reserva
 *
 **************************************************************/

void imprime_operacion
(
        codop_t c,
        char *nom
        ) {
    /*************************************/
    /*  Function body                    */
    /*************************************/

    switch (c) {
        case OP_SGT:
        case OP_FP_GT_D:
        case OP_FP_GT_S:
            sprintf(nom, ">");
            break;
        case OP_SLT:
        case OP_FP_LT_D:
        case OP_FP_LT_S:
            sprintf(nom, "<");
            break;
        case OP_SGE:
        case OP_SGEI:
        case OP_FP_GE_D:
        case OP_FP_GE_S:
            sprintf(nom, ">=");
            break;
        case OP_SLE:
        case OP_SLEI:
        case OP_FP_LE_D:
        case OP_FP_LE_S:
            sprintf(nom, "<=");
            break;
        case OP_SNE:
        case OP_SNEI:
        case OP_FP_NE_D:
        case OP_FP_NE_S:
            sprintf(nom, "!=");
            break;
        case OP_SEQ:
        case OP_SEQI:
        case OP_FP_EQ_D:
        case OP_FP_EQ_S:
            sprintf(nom, "==");
            break;
        case OP_FP_MOV_D:
        case OP_FP_MOV_S:
            sprintf(nom, "mv");
            break;
        case OP_DMUL:
        case OP_FP_MUL_D:
        case OP_FP_MUL_S:
        case OP_FP_MUL_PS:
            sprintf(nom, "*");
            break;
        case OP_DDIV:
        case OP_FP_DIV_D:
        case OP_FP_DIV_S:
        case OP_FP_DIV_PS:
            sprintf(nom, "/");
            break;
        case OP_FP_ADD_D:
        case OP_FP_ADD_S:
        case OP_FP_ADD_PS:
        case OP_DADD:
        case OP_DADDI:
            sprintf(nom, "+");
            break;
        case OP_FP_SUB_D:
        case OP_FP_SUB_S:
        case OP_FP_SUB_PS:
        case OP_DSUB:
        case OP_DSUBI:
            sprintf(nom, "-");
            break;
        case OP_AND:
        case OP_ANDI:
            sprintf(nom, "&");
            break;
        case OP_OR:
        case OP_ORI:
            sprintf(nom, "|");
            break;
        case OP_XOR:
        case OP_XORI:
            sprintf(nom, "^");
            break;
        case OP_BC1T:
            sprintf(nom, "bc1t");
            break;
        case OP_BC1F:
            sprintf(nom, "bc1f");
            break;
        case OP_BNEZ:
            sprintf(nom, "bnez");
            break;
        case OP_BEQZ:
            sprintf(nom, "beqz");
            break;
        case OP_BNE:
            sprintf(nom, "bne");
            break;
        case OP_BEQ:
            sprintf(nom, "beq");
            break;
        case OP_JR:
            sprintf(nom, "jr");
            break;
        case OP_JALR:
            sprintf(nom, "jalr");
            break;
        case OP_J:
            sprintf(nom, "j");
            break;
        case OP_JAL:
            sprintf(nom, "jal");
            break;
        case OP_LUI:
            sprintf(nom, "lui");
            break;
        case OP_DSLL:
        case OP_DSLLI:
            sprintf(nom, "<<");
            break;
        case OP_DSRA:
        case OP_DSRAI:
        case OP_DSRL:
        case OP_DSRLI:
            sprintf(nom, ">>");
            break;
        case OP_MOVN:
            sprintf(nom, "movn");
            break;
        case OP_MOVZ:
            sprintf(nom, "movz");
            break;
        case OP_LD:
            sprintf(nom, "ld");
            break;
        case OP_LB:
            sprintf(nom, "lb");
            break;
        case OP_LBU:
            sprintf(nom, "lbu");
            break;
        case OP_LH:
            sprintf(nom, "lh");
            break;
        case OP_LHU:
            sprintf(nom, "lhu");
            break;
        case OP_LW:
            sprintf(nom, "lw");
            break;
        case OP_LWU:
            sprintf(nom, "lwu");
            break;
        case OP_FP_L_S:
            sprintf(nom, "l.s");
            break;
        case OP_FP_L_D:
            sprintf(nom, "l.d");
            break;
        case OP_FP_L_PS:
            sprintf(nom, "l.ps");
            break;
        case OP_SD:
            sprintf(nom, "sd");
            break;
        case OP_SB:
            sprintf(nom, "sb");
            break;
        case OP_SH:
            sprintf(nom, "sh");
            break;
        case OP_SW:
            sprintf(nom, "sw");
            break;
        case OP_FP_S_S:
            sprintf(nom, "s.s");
            break;
        case OP_FP_S_D:
            sprintf(nom, "s.d");
            break;
        case OP_FP_S_PS:
            sprintf(nom, "s.ps");
            break;

        default:
            sprintf(nom, "%d", c);
            break;
    } /* endswitch */

} /* end imprime_operacion */

/***************************************************************
 *
 * Func: imprime_tipo_predictor
 *
 * Description:
 *
 *
 **************************************************************/

void imprime_tipo_predictor
(
        char *aux
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/


    switch (tipo_predictor) {
        case BTB_1_BIT:
            sprintf(aux, "BTB 1 bit");
            break;
        case BTB_2_BIT_H:
            sprintf(aux, "BTB 2 bits con histeresis");
            break;
        case BTB_2_BIT_S:
            sprintf(aux, "BTB 2 bits con saturacion");
            break;
        case BTB_n_BIT_H:
            sprintf(aux, "Local, %d bits con histeresis", n_bits_pred_local);
            break;
        case BTB_n_BIT_S:
            sprintf(aux, "Local, %d bits con saturacion", n_bits_pred_local);
            break;
        case GLOBAL_n_BIT_H:
            sprintf(aux, "Global, %d bits con histeresis", n_bits_pred_global);
            break;
        case GLOBAL_n_BIT_S:
            sprintf(aux, "Global, %d bits con saturacion", n_bits_pred_global);
            break;
        case HYBRID_n_BIT_S:
            sprintf(aux, "Hibrido, %d bits con saturacion", n_bits_pred_hybrid);
            break;
        case PERFECTO:
            sprintf(aux, "Perfecto");
            break;
        case CLARIVIDENTE:
            sprintf(aux, "Clarividente");
            break;
        case PREDICT_NOT_TAKEN:
            sprintf(aux, "Predict-Not-Taken");
            break;
    }

} /* end desplazamiento_horizontal */

void valor2str(char *dst, valor_t dato, tipo_t tipo_dato) {
    switch (tipo_dato) {
        case t_double:
            sprintf(dst, "%.2lf", dato.fp_d);
            break;
        case t_float:
            sprintf(dst, "%.2f", dato.fp_s);
            break;
        case t_float_ps:
            sprintf(dst, "%.2f|%.2f", dato.fp_ps[Hi], dato.fp_ps[Lo]);
            break;
        case t_char:
            sprintf(dst, "%c", (char) dato.int_d);
            break;
        case t_ninguno:
            strcpy(dst, "&nbsp;");
            break;
        default:
            if (dato.int_d <= MAX_WORD) {
                sprintf(dst, F_DWORD, dato.int_d);
            } else {
                sprintf(dst, "%" PRIx64, dato.int_d);
            }
            break;
    }
}

void FIRMA_valor(char *cadena, int i, valor_t dato, tipo_t tipo_dato) {

    /*************************************/
    /*  Local variables                  */
    /*************************************/
    char aux[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    valor2str(aux, dato, tipo_dato);

    FIRMA_2_1(cadena, i, aux);
}

/**
 * Obtiene el tipo de una instrucción en función del código de operación
 *
 * @param codop
 * @return
 */
tipo_t obtener_tipo(codop_t codop) {
    if (inst_fp_d(codop))
        return (t_double);
    if (inst_fp_s(codop))
        return (t_float);
    if (inst_simd(codop))
        return (t_float_ps);
    return (t_dword);
}

/***************************************************************
 *
 * Func: imprime_config
 *
 * Desc: Imprime la configuración del simulador
 *
 **************************************************************/

void imprime_config() {

    char aux[128];

    fprintf(stdout, "\n*** CONFIGURACIÓN ***\n\n");

    fprintf(stdout, "  Grado superescalar: \n");
    fprintf(stdout, "        ISSUE:        %d\n", NUM_VIAS_ISSUE);
    fprintf(stdout, "        BUSES:        %d\n", NUM_VIAS_BUS);
    fprintf(stdout, "        COMMIT:       %d\n", NUM_VIAS_COMMIT);
    fprintf(stdout, "  Reorder Buffer:     %d\n", TAM_REORDER);
    fprintf(stdout, "  E.R. Suma/Resta:    %d\n", TAM_RS_SUMREST);
    fprintf(stdout, "  E.R. Mult/Div:      %d\n", TAM_RS_MULTDIV);
    fprintf(stdout, "  E.R. Enteros:       %d\n", TAM_RS_ENTEROS);
    fprintf(stdout, "  Buffer lectura:     %d\n", TAM_BUFFER_CARGA);
    fprintf(stdout, "  Buffer escritura:   %d\n", TAM_BUFFER_ALMACEN);
    imprime_tipo_predictor(aux);
    fprintf(stdout, "  Tipo predictor:     %s\n", aux);
    fprintf(stdout, "  Buffer predictor:   %d\n", TAM_BUFFER_PREDIC);
    fprintf(stdout, "  Registros:          %d\n", TAM_REGISTROS);

    /*** Tiempos de evaluación ***/

    fprintf(stdout, "\n");

#define INFO_OPER(op) NUM_INICIO_##op, TEVAL_##op, ((NUM_INICIO_##op != NUM_OPER_##op) ? "SEGM" : "CONV")

    fprintf(stdout, "                    Cantidad   Latencia  Tipo\n");
    fprintf(stdout, "                    --------   --------  ----\n");
    fprintf(stdout, "  Oper. Suma/Resta:  %3d         %3d     %s\n", INFO_OPER(SUMREST));
    fprintf(stdout, "  Oper. Mult/Div:    %3d         %3d     %s\n", INFO_OPER(MULTDIV));
    fprintf(stdout, "  Oper. Enteros:     %3d         %3d     %s\n", INFO_OPER(ENTEROS));
    fprintf(stdout, "  Oper. Memoria:     %3d         %3d     %s\n", INFO_OPER(MEMDATOS));

    fprintf(stdout, "\n");

} /* end imprime_config */

void imprime_mapa_memoria() {
    int i;

    fprintf(stdout, "*** MEMORIA ***\n\n");

    fprintf(stdout, "begin  end    size  region\n");
    fprintf(stdout, "------ ------ ----- -------------------------\n");
    for (i = 0; i < num_regiones; i++) {
        if (tam_region(mmem, i) > 0) {
            fprintf(stdout, "%06" PRIx64 "-%06" PRIx64 " %05" PRIx64 " [%s]\n",
                    mmem->regiones[i].inicio,
                    mmem->regiones[i].final,
                    tam_region(mmem, i),
                    mmem->regiones[i].nombre);
            for (int i_obj = 0; i_obj < programa.n_objs; ++i_obj) {
                fichero_objeto_t *obj = &programa.l_objs[i_obj];
                if (tam_region(&obj->memoria, i) > 0) {
                    fprintf(stdout, "%06" PRIx64 "-%06" PRIx64 " %05" PRIx64 "     [<%s> : %s]\n",
                            obj->memoria.regiones[i].inicio,
                            obj->memoria.regiones[i].final,
                            tam_region(&obj->memoria, i),
                            obj->ts.fichero,
                            obj->memoria.regiones[i].nombre);
                }
            }
        }
    }
    fprintf(stdout, "\n");
}

#define PRINT_ESTAT(label, campo) \
do { \
    int size = FIN_##campo - INICIO_##campo +1;\
    fprintf(stdout, "  " label " %3d / %3d   (%3u%% - %3u%%)\n", max_util.campo, (size), (max_util.campo * 100 / (size)), (aver_util.campo * 100 / (Ciclo * size)));\
} while(0)

/***************************************************************
 *
 * Func: imprime_estadisticas
 *
 * Desc: Imprime las estadisticas del programa
 *
 **************************************************************/

void imprime_estadisticas() {

    fprintf(stdout, "\n*** ESTADISTICAS ***\n\n");

    fprintf(stdout, "Ciclos:              %4ld\n", estat.ciclos);
    fprintf(stdout, "Instrucciones:       %4ld\n", estat.instrucciones);
    fprintf(stdout, "CPI / IPC:           %1.2f / %1.2f\n", (float) estat.ciclos / estat.instrucciones,
            (float) estat.instrucciones / estat.ciclos);
    fprintf(stdout, "Ops C.F:             %4ld\n", estat.flops);
    fprintf(stdout, "Ops C.F./ciclo:      %1.2f\n", (float) estat.flops / estat.ciclos);
    fprintf(stdout, "Saltos:\n");
    fprintf(stdout, "    Accedidos:       %4ld\n", estat.saltos_accedidos);
    fprintf(stdout, "    Encontrados:     %4ld\n", estat.saltos_encontrados);
    fprintf(stdout, "    Ejecutados:      %4ld\n", estat.saltos_ejecutados);
    fprintf(stdout, "    Acertados:       %4ld\n", estat.saltos_acertados);
    fprintf(stdout, "\n");
    fprintf(stdout, "Ocupación de recursos:\n");
    fprintf(stdout, "               Max / TAM  %% max  %% med\n");

    PRINT_ESTAT("RS Enteros: ", RS_ENTEROS);
    PRINT_ESTAT("RS S/R C.F.:", RS_SUMREST);
    PRINT_ESTAT("RS M/D C.F.:", RS_MULTDIV);
    PRINT_ESTAT("RS Carga:   ", BUFFER_CARGA);
    PRINT_ESTAT("RS Almac.:  ", BUFFER_ALMACEN);

    PRINT_ESTAT("Op Enteros: ", OP_ENTEROS);
    PRINT_ESTAT("Op S/R C.F.:", OP_SUMREST);
    PRINT_ESTAT("Op M/D C.F.:", OP_MULTDIV);
    PRINT_ESTAT("Op Mem (AC):", OP_DIRECCIONES);
    PRINT_ESTAT("Op Mem:     ", OP_MEMDATOS);
    PRINT_ESTAT("RB:         ", REORDER);

    fprintf(stdout, "\n\n");

} /* end imprime_estadisticas */

void imprime_inicio_null(void) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void imprime_final_null(void) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void inicio_ciclo_null(ciclo_t ciclo) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void fin_ciclo_null(ciclo_t ciclo) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void imprime_estado_null(void) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void imprime_crono_null(void) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void imprime_predictor_null(void) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void init_instruc_null(dword PC, ciclo_t orden) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void muestra_fase_null(char *fase, ciclo_t orden, boolean exception) {
    /* Esta función de presentación no hace nada intencionadamente */
}

void muestra_datos_null(region_t r, dword from, dword to, mem_status_t status) {
    /* Esta función de presentación no hace nada intencionadamente */
}

