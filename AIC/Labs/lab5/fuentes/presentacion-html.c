/**************************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politecnica de Valencia.
 *
 * Author:  Sergio Sáez (ssaez@disca.upv.es)
 *              Pedro López (plopez@disca.upv.es)
 *
 * File: presentacion-html.c
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

#define presentacion_html_C

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memoria.h>
#include <tipos.h>

#include "main.h"
#include "tipos.h"
#include "instrucciones.h"
#include "memoria.h"
#include "registros.h"
#include "depurar.h"

#include "presentacion.h"
#include "presentacion-html.h"


/***** Variables globales *************************************/

presentacion_interface_t presentacion_html = {
    .imprime_inicio_f = imprime_inicio_html,
    .imprime_final_f = imprime_final_html,
    .inicio_ciclo_f = inicio_ciclo_null,
    .fin_ciclo_f = fin_ciclo_null,
    .imprime_estado_f = imprime_estado_html,
    .imprime_crono_f = imprime_crono_html,
    .imprime_predictor_f = imprime_predictor_html,
    .init_instruc_f = init_instruc_html,
    .muestra_fase_f = muestra_fase_html,
    .muestra_datos_f = muestra_datos_null
};

/***** Variables locales **************************************/

static ciclo_t primera_instruccion = 1;
static ciclo_t primer_ciclo = 1;
static short maxima_columna_visible = MAX_CICLOS_INIC;

static FILE *f_salida;

/***** Definiciones Externas **********************************/

static void itobin(int n, char* buffer, int digits) {
    int i;
    int q;
    char aux1[128];
    char aux2[256];

    if (digits == 0) {
        strcpy(aux2, "--");
    } else {
        q = n;
        strcpy(aux1, "");
        for (i = 0; i < digits; i++) {
            sprintf(aux2, "%d%s", q % 2, aux1);
            q = q / 2;
            strcpy(aux1, aux2);
        }
    }
    strcpy(buffer, aux2);
}

/***************************************************************
 *
 * Func: imprime_estado_predictor
 *
 * Description:
 *   Obtiene el estado del predictor
 *
 **************************************************************/

static void imprime_estado_predictor_html
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

static void imprime_estado_predictor_n_html
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

static void imprime_estado_selector_n_html
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
 * Func: desplazamiento_horizontal
 *
 * Description:
 *   Desplaza la zona visible un ciclo hacia la izquierda
 *
 **************************************************************/

static void desplazamiento_horizontal
(
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i, j;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    for (i = 0; i < MAX_INSTRUC; i++) {
        for (j = 0; j < MAX_CICLOS - 1; j++)
            strcpy(crono[i].fase[j], crono[i].fase[j + 1]);

        /* Borra la Última columna */
        strcpy(crono[i].fase[j], "");
    } /* endfor */

    /*** Actualiza los desplazamientos iniciales ***/

    primer_ciclo++;

} /* end desplazamiento_horizontal */

/***************************************************************
 *
 * Func: desplazamiento_vertical
 *
 * Description:
 *   Desplaza la zona visible un ciclo hacia la izquierda
 *
 **************************************************************/

static void desplazamiento_vertical
(
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i, j, desp;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Averigua si la linea se va a quedar vacia al principio */
    for (desp = 0;
            desp < MAX_CICLOS &&
            !strlen(crono[1].fase[desp]); desp++);
    if (desp == MAX_CICLOS)
        desp = 1; /* La línea está completamente vacia. Avanzamos sólo un
                                   * ciclo */

    for (i = 0; i < MAX_INSTRUC - 1; i++) {
        crono[i].PC = crono[i + 1].PC;
        strcpy(crono[i].inst, crono[i + 1].inst);

        for (j = 0; j < MAX_CICLOS - desp; j++)
            strcpy(crono[i].fase[j], crono[i + 1].fase[j + desp]);

        /* Borra las últimas columnas */
        for (j = MAX_CICLOS - desp; j < MAX_CICLOS; j++)
            strcpy(crono[i].fase[j], "");
    } /* endfor */

    /* Borra la última fila */
    strcpy(crono[MAX_INSTRUC - 1].inst, "");
    for (j = 0; j < MAX_CICLOS; j++)
        strcpy(crono[MAX_INSTRUC - 1].fase[j], "");

    /*** Actualiza los desplazamientos iniciales ***/

    primer_ciclo += desp;
    primera_instruccion++;
    maxima_columna_visible -= desp;

} /* end desplazamiento_vertical */

/***************************************************************
 *
 * Func: imprime_config_html
 *
 * Description:
 *
 *
 **************************************************************/

static void imprime_config_html
(
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/
    char aux[128];

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr valign=\"top\"><td>\n");

    /*** Estructuras ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td>Estructura</td>\n");
    fprintf(f_salida, "  <td align=\"center\">N&uacute;mero</td>\n");
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Num v&iacuteas Issue</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", NUM_VIAS_ISSUE);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Num v&iacuteas Writeback</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", NUM_VIAS_BUS);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Num v&iacuteas Commit</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", NUM_VIAS_COMMIT);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Registros</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", TAM_REGISTROS);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Reorder&nbsp;Buffer</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", TAM_REORDER);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">E.R.&nbsp;Suma/Resta</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", TAM_RS_SUMREST);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">E.R.&nbsp;Multiplicaci&oacute;n/Divisi&oacute;n</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", TAM_RS_MULTDIV);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">E.R.&nbsp;Enteros</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", TAM_RS_ENTEROS);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Buffers de carga</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", TAM_BUFFER_CARGA);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Buffers de almacen.</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", TAM_BUFFER_ALMACEN);
    fprintf(f_salida, " </tr>\n");

    imprime_tipo_predictor(aux);
    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Tipo&nbsp;de&nbsp;predictor</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%s</td>\n", aux);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Buffer&nbsp;predictor</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", TAM_BUFFER_PREDIC);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, "</table><br>\n");


    fprintf(f_salida, "  </td><td>\n");

    /*** Tiempos de evaluación ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td>Operador</td>\n");
    fprintf(f_salida, "  <td align=\"center\">Cantidad</td>\n");
    fprintf(f_salida, "  <td align=\"center\">Latencia</td>\n");
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Suma/Resta</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", NUM_OPER_SUMREST);
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", TEVAL_SUMREST);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Multiplicaci&oacute;n/Divisi&oacute;n</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", NUM_OPER_MULTDIV);
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", TEVAL_MULTDIV);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Enteros</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", NUM_OPER_ENTEROS);
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", TEVAL_ENTEROS);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr>\n");
    fprintf(f_salida, "  <td bgcolor=\"#ddeeff\">Memoria de datos</td>\n");
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", NUM_OPER_MEMDATOS);
    fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" align=\"center\">%d</td>\n", TEVAL_MEMDATOS);
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, "</table><br>\n");

    fprintf(f_salida, "  </td></tr>\n");
    fprintf(f_salida, "</table><br>\n");

} /* end desplazamiento_horizontal */


/***** Funciones Exportables **********************************/

/***************************************************************
 *
 * Func: init_instruc
 *
 * Description:
 *   Inicializa una instrucción en el cronograma
 *
 **************************************************************/

void init_instruc_html
(
        dword PC,
        ciclo_t orden
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    int o;
    char aux[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    if (orden == primera_instruccion + MAX_INSTRUC)
        desplazamiento_vertical();

    o = orden - primera_instruccion;

    imprime_instruccion(aux, PC);
    crono[o].PC = PC;
    sprintf(crono[o].inst, "%s", aux);

} /* end init_instruc */

/***************************************************************
 *
 * Func: muestra_fase
 *
 * Description:
 *   Muestra la fase de cada instrucción en el cronograma
 *
 **************************************************************/

void muestra_fase_html
(
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

    if (Ciclo >= primer_ciclo + MAX_CICLOS)
        desplazamiento_horizontal();

    o = orden - primera_instruccion;
    c = Ciclo - primer_ciclo;

    if (o >= 0 && c >= 0) {
        if (!exception) {
            strcpy(crono[o].fase[c], fase);
        } else {
            sprintf(crono[o].fase[c], "<font color='red'>%s</font>", fase);
        }
        if (c + 1 > maxima_columna_visible)
            maxima_columna_visible = c + 1;
    } /* endif */

} /* end muestra_fase */

/***************************************************************
 *
 * Func: imprime_DMem
 *
 * Desc: Imprime la memoria de datos
 *
 **************************************************************/

static void imprime_DMem_html() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    dword i;
    dword d;
    dword next;

    char aux[128];
    char color[64];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

    for (region_t r = 0; r < num_regiones; r++) {
        if (!es_region_datos(r) || tam_region(mmem, r) == 0)
            continue;

        fprintf(f_salida, "<table>\n");

        fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
        fprintf(f_salida, "  <td width=\"50\" align=\"center\">Dir</td>\n");
        for (i = 0; i < 4; i++) {
            fprintf(f_salida, "  <td width=\"20\" align=\"center\">+" F_DWORD "</td>\n", i);
        } /* endif */
        fprintf(f_salida, " </tr>\n");

        next = 0;
        for (d = inicio_region(mmem, r); d < final_region(mmem, r); d++) {

            i = dir_a_offset(mmem, d, r);

            if (d % sizeof (word) == 0) {
                fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
                te_etiqueta(aux, d);
                color_etiqueta_memdatos(color, d);
                fprintf(f_salida, "  <td align=\"left\" nowrap='nowrap' bgcolor=\"#ddeeff\">"
                        "<span style='color: %s; margin: 0 0.5em'>%s</span></td>\n", color, aux);
            } /* endif */

            if (i == next) {
                switch (md_tipo[r][i]) {
                    case t_byte:
                        FIRMA_2_2("MD"
                                F_DWORD, i, "%hhX", MD[r].m_byte[indice_byte(i)]);
                        /* Imprime también el caracter */
                        sprintf(aux, "%s %c", vstr, MD[r].m_byte[indice_byte(i)]);
                        fprintf(f_salida,
                                "  <td align=\"center\">%s</td>\n", aux);
                        next = i + sizeof (byte);
                        break;
                    case t_half:
                        FIRMA_2_2("MD"
                                F_DWORD, i, "%hd", MD[r].m_half[indice_half(i)]);
                        fprintf(f_salida,
                                "  <td align=\"center\" colspan=\"2\">%s</td>\n", vstr);
                        next = i + sizeof (half);
                        break;
                    case t_word:
                        FIRMA_2_2("MD"
                                F_DWORD, i, "%d", MD[r].m_word[indice_word(i)]);
                        fprintf(f_salida,
                                "  <td align=\"center\" colspan=\"4\">%s</td>\n", vstr);
                        next = i + sizeof (word);
                        break;
                    case t_dword:
                        FIRMA_2_2("MD"
                                F_DWORD, i, F_DWORD, MD[r].m_dword[indice_dword(i)]);
                        fprintf(f_salida,
                                "  <td align=\"center\" rowspan=\"2\" colspan=\"4\">%s</td>\n", vstr);
                        next = i + sizeof (dword);
                        break;
                    case t_float:
                        FIRMA_2_2("MD"
                                F_DWORD, i, "%.2f", MD[r].m_float[indice_float(i)]);
                        fprintf(f_salida,
                                "  <td align=\"center\" colspan=\"4\">%s</td>\n", vstr);
                        next = i + sizeof (float);
                        break;
                    case t_double:
                        FIRMA_2_2("MD"
                                F_DWORD, i, "%.2f", MD[r].m_double[indice_double(i)]);
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
}

/***************************************************************
 *
 * Func: imprime_inicio
 *
 * Desc: Imprime el cronograma de ejecución
 *
 **************************************************************/

void imprime_inicio_html
(
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i;

    char aux[128];
    char color[64];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    primera_instruccion = 1;
    primer_ciclo = 1;
    maxima_columna_visible = MAX_CICLOS_INIC;
    for (i = 0; i < MAX_INSTRUC; i++) {
        bzero(&crono[i], sizeof (linea_crono_t));
    } /* endfor */

    if (html_merge == SI) {
        //        sprintf(aux, "%s.htm", nombre_fich);
        sprintf(aux, "%s.htm", "index");
    } else {
        sprintf(aux, "index.html");
    } /* endif */

    f_salida = fopen(aux, "w");
    if (f_salida == NULL) {
        fprintf(stderr, "Error creando %s\n", aux);
        exit(1);
    } /* endif */

    if (html_merge == SI) {
        fputs("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
                "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
                "<head>\n"
                "<title>HTML merge</title>\n"
                "<!--\n"
                "     Iframe occupies the whole body\n"
                "-->\n"
                "<style type=\"text/css\">\n"
                "  body, html\n"
                "  {\n"
                "     margin: 0; padding: 0; height: 100%; overflow: hidden;\n"
                "  }\n"
                "\n"
                "  #content\n"
                "  {\n"
                "     position:absolute; left: 0; right: 0; bottom: 0; top: 0px;\n"
                "  }\n"
                "\n"
                "</style>\n"
                "\n"
                "<script>\n"
                "\n"
                "var files={\n"
                "'index.html':`",
                f_salida);
    } /* endif */

    /*** HTML INTRO ***/

    fprintf(f_salida,
            "<html><head><title>mips-especulacion: %s. Ciclo " F_CICLO "</title>\n",
            nombre_fich, Ciclo);
    fprintf(f_salida,
            "<style type=\"text/css\">\n"
            " td .address {\n"
            "     text-align: right; white-space: nowrap; padding: 0 0.4em;\n"
            "     background-color: #ddeeff;\n"
            "}\n"
            "</style>\n");
    fprintf(f_salida, "</head>\n");
    fprintf(f_salida,
            "<body bgcolor=\"white\">\n");

    fprintf(f_salida, "<a href=\"index.html\">INICIO</a>&nbsp;\n");
    fprintf(f_salida, "<a href=\"final.html\">FINAL</a>&nbsp;\n");
    if (f_depuracion) {
        fprintf(f_salida, "<a href=\"estado%03d.html\">Estado</a>\n", 1);

        fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
        fprintf(f_salida, "<a href=\"crono%03d.html\">Cronograma</a>\n", 1);

        fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
        fprintf(f_salida, "<a href=\"predictor%03d.html\">BTB</a>\n", 1);
    }
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<tt><b>Programa: %s</b></tt>\n", nombre_fich);
    fprintf(f_salida, "<br>\n");
    fprintf(f_salida, "<br>\n");

    /*** Datos de la implementación ***/

    imprime_config_html();

    /*** Memorias ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr  valign=\"top\" bgcolor=\"#ddeeff\">\n");
    fprintf(f_salida, "  <td>Memoria&nbsp;de&nbsp;datos</td>\n");
    fprintf(f_salida, "  <td>Memoria&nbsp;de&nbsp;instrucciones</td>\n");
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr valign=\"top\"><td>\n");

    /*** Contenido de la memoria de datos ***/

    imprime_DMem_html();

    fprintf(f_salida, "  </td><td>\n");

    /*** Contenido de la memoria de instrucciones ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"75\" aling='center'>Direcci&oacute;n</td>\n");
    fprintf(f_salida, "  <td width=\"200\" aling='center'>Instrucciones</td>\n");
    fprintf(f_salida, " </tr>\n");

    for (i = inicio_region(mmem, r_text); i < final_region(mmem, r_text); i += 4) {
        fprintf(f_salida, " <tr>\n");
        te_etiqueta(aux, i);
        fprintf(f_salida, "  <td class='address'>%s</td>\n", aux);
        imprime_instruccion(aux, i);
        fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" nowrap='nowrap'><code>%s</code></td>\n", aux);
        fprintf(f_salida, " </tr>\n");
    }

    fprintf(f_salida, "</table>\n");

    fprintf(f_salida, "  </td></tr>\n");
    fprintf(f_salida, "</table><br>\n");

    fprintf(f_salida, "<address>Arquitectura e Ingenier&iacute;a de Computadores</address>\n");
    fprintf(f_salida, "</body></html>\n");

    if (html_merge == SI) {
        fputs("`,", f_salida);
    } else {
        fclose(f_salida);
    } /* endif */

} /* end imprime_inicio */

/***************************************************************
 *
 * Func: imprime_final
 *
 **************************************************************/

void imprime_final_html
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

    sprintf(aux, "final.html");
    if (html_merge == SI) {
        fprintf(f_salida, "'%s':`", aux);
    } else {
        f_salida = fopen(aux, "w");
        if (f_salida == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */

    /*** HTML INTRO ***/

    fprintf(f_salida,
            "<html><head><title>mips-especulacion: %s. Ciclo " F_CICLO "</title></head>\n",
            nombre_fich, Ciclo);
    fprintf(f_salida,
            "<body bgcolor=\"white\">\n");

    fprintf(f_salida, "<a href=\"index.html\">INICIO</a>&nbsp;\n");
    fprintf(f_salida, "<a href=\"final.html\">FINAL</a>&nbsp;\n");
    if (f_depuracion) {
        fprintf(f_salida, "<a href=\"estado%03d.html\">Estado</a>\n", 1);

        fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
        fprintf(f_salida, "<a href=\"crono%03d.html\">Cronograma</a>\n", 1);

        fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
        fprintf(f_salida, "<a href=\"predictor%03d.html\">BTB</a>\n", 1);

    }
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<tt><b>Programa: %s</b></tt>\n", nombre_fich);
    fprintf(f_salida, "<br>\n");
    fprintf(f_salida, "<br>\n");

    /*** Resultados de la ejecucion ***/

    fprintf(f_salida, "<table>\n");
    fprintf(f_salida, "<tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td>Ciclos</td>\n");
    fprintf(f_salida, "  <td>Instrucciones</td>\n");
    fprintf(f_salida, "  <td>CPI / IPC</td>\n");
    fprintf(f_salida, "  <td>Op. CF</td>\n");
    fprintf(f_salida, "  <td>Op. CF/ciclo</td>\n");
    fprintf(f_salida, "  <td>Saltos buscados</td>\n");
    fprintf(f_salida, "  <td>Saltos en BTB</td>\n");
    fprintf(f_salida, "  <td>Saltos ejecutados (C)</td>\n");
    fprintf(f_salida, "  <td>Saltos acertados</td>\n");

    fprintf(f_salida, "</tr>\n");
    fprintf(f_salida, "<tr bgcolor=\"#eeeeee\">\n");
    fprintf(f_salida, "  <td>%ld</td>\n", estat.ciclos);
    fprintf(f_salida, "  <td>%ld</td>\n", estat.instrucciones);
    fprintf(f_salida, "  <td>%1.2f / %1.2f</td>\n", (float) estat.ciclos / estat.instrucciones,
            (float) estat.instrucciones / estat.ciclos);
    fprintf(f_salida, "  <td>%ld</td>\n", estat.flops);
    fprintf(f_salida, "  <td>%1.2f</td>\n", (float) estat.flops / estat.ciclos);
    fprintf(f_salida, "  <td>%ld </td>\n", estat.saltos_accedidos);
    fprintf(f_salida, "  <td>%ld (%2.1f%%)</td>\n", estat.saltos_encontrados,
            (float) (estat.saltos_encontrados * 100) / estat.saltos_accedidos);
    fprintf(f_salida, "  <td>%ld (%2.1f%%)</td>\n", estat.saltos_ejecutados,
            (float) (estat.saltos_ejecutados * 100) / estat.instrucciones);
    fprintf(f_salida, "  <td>%ld (%2.1f%%)</td>\n", estat.saltos_acertados,
            (float) (estat.saltos_acertados * 100) / estat.saltos_ejecutados);
    fprintf(f_salida, "</tr>\n");
    fprintf(f_salida, "</table><br>\n");

    /*** Resultados de la ejecucion: Ocupación ***/

    fprintf(f_salida, "<table>\n");
    fprintf(f_salida, "<tr bgcolor=\"#ccddff\">\n");

    fprintf(f_salida, "  <td colspan=\"11\" align=\"center\">Ocupacion de recursos </td>\n");
    fprintf(f_salida, "</tr>\n");
    fprintf(f_salida, "<tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td>RS Ent.</td>\n");
    fprintf(f_salida, "  <td>RS S/R</td>\n");
    fprintf(f_salida, "  <td>RS M/D</td>\n");
    fprintf(f_salida, "  <td>RS Carga</td>\n");
    fprintf(f_salida, "  <td>RS Almac.</td>\n");
    fprintf(f_salida, "  <td>Op Ent.</td>\n");
    fprintf(f_salida, "  <td>Op S/R</td>\n");
    fprintf(f_salida, "  <td>Op M/D</td>\n");
    fprintf(f_salida, "  <td>Op AC</td>\n");
    fprintf(f_salida, "  <td>Op Mem.</td>\n");
    fprintf(f_salida, "  <td>RB</td>\n");
    fprintf(f_salida, "</tr>\n");
    fprintf(f_salida, "<tr bgcolor=\"#eeeeee\">\n");

    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.RS_ENTEROS, (max_util.RS_ENTEROS * 100 / TAM_RS_ENTEROS));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.RS_SUMREST, (max_util.RS_SUMREST * 100 / TAM_RS_SUMREST));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.RS_MULTDIV, (max_util.RS_MULTDIV * 100 / TAM_RS_MULTDIV));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.BUFFER_CARGA, (max_util.BUFFER_CARGA * 100 / TAM_BUFFER_CARGA));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.BUFFER_ALMACEN, (max_util.BUFFER_ALMACEN * 100 / TAM_BUFFER_ALMACEN));

    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.OP_ENTEROS, (max_util.OP_ENTEROS * 100 / NUM_OPER_ENTEROS));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.OP_SUMREST, (max_util.OP_SUMREST * 100 / NUM_OPER_SUMREST));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.OP_MULTDIV, (max_util.OP_MULTDIV * 100 / NUM_OPER_MULTDIV));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.OP_DIRECCIONES,
            (max_util.OP_DIRECCIONES * 100 / NUM_OPER_DIRECCIONES));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.OP_MEMDATOS, (max_util.OP_MEMDATOS * 100 / NUM_OPER_MEMDATOS));
    fprintf(f_salida, "  <td>%d (%d%%)</td>\n", max_util.REORDER, (max_util.REORDER * 100 / TAM_REORDER));

    fprintf(f_salida, "</tr>\n");
    fprintf(f_salida, "</table><br>\n");


    /*** Datos de la implementación ***/

    imprime_config_html();

    /*** Memorias ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr  valign=\"top\" bgcolor=\"#ddeeff\">\n");
    fprintf(f_salida, "  <td>Memoria&nbsp;de&nbsp;datos</td>\n");
    fprintf(f_salida, "  <td>Memoria&nbsp;de&nbsp;instrucciones</td>\n");
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr valign=\"top\"><td>\n");

    /*** Contenido de la memoria de datos ***/

    imprime_DMem_html();

    fprintf(f_salida, "  </td><td>\n");

    /*** Contenido de la memoria de instrucciones ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"75\">Direcci&oacute;n</td>\n");
    fprintf(f_salida, "  <td width=\"200\">Instrucciones</td>\n");
    fprintf(f_salida, " </tr>\n");

    for (i = inicio_region(mmem, r_text); i < final_region(mmem, r_text); i += 4) {
        fprintf(f_salida, " <tr>\n");
        te_etiqueta(aux, i);
        fprintf(f_salida, "  <td align=\"left\" nowrap='nowrap' bgcolor=\"#ddeeff\">"
                "%s&nbsp;&nbsp;&nbsp;&nbsp;</td>\n", aux);
        imprime_instruccion(aux, i);
        fprintf(f_salida, "  <td bgcolor=\"#eeeeee\" nowrap='nowrap'><code>%s</code></td>\n", aux);
        fprintf(f_salida, " </tr>\n");
    }

    fprintf(f_salida, "</table>\n");

    fprintf(f_salida, "  </td></tr>\n");
    fprintf(f_salida, "</table><br>\n");

    fprintf(f_salida, "<address>Arquitectura e Ingenier&iacute;a de Computadores</address>\n");
    fprintf(f_salida, "</body></html>\n");

    if (html_merge == SI) {
        fputs("`};\n"
                "\n"
                "function get_iframedoc()\n"
                "{\n"
                "   var iframe = document.getElementById('iframe');\n"
                "   var iframedoc = iframe.document;\n"
                "      if (iframe.contentDocument)\n"
                "      {\n"
                "         iframedoc = iframe.contentDocument;\n"
                "      }\n"
                "      else if (iframe.contentWindow)\n"
                "      {\n"
                "         iframedoc = iframe.contentWindow.document;\n"
                "      }\n"
                "   return iframedoc;\n"
                "}\n"
                "\n"
                "//\n"
                "// loads file on iframe\n"
                "//\n"
                "function load_file(file)\n"
                "{\n"
                "   if (!files[file])\n"
                "   {\n"
                "      return;\n"
                "   }\n"
                "\n"
                "   var iframedoc = get_iframedoc();\n"
                "\n"
                "   if (iframedoc)\n"
                "   {\n"
                "      iframedoc.open();\n"
                "      iframedoc.writeln(files[file]);\n"
                "      iframedoc.close();\n"
                "   }\n"
                "   else\n"
                "   {\n"
                "      alert('Cannot inject dynamic contents into iframe.');\n"
                "   }\n"
                "}\n"
                "\n"
                "function check_anchor_click(event)\n"
                "{\n"
                "   if (event.target.tagName !== 'A')\n"
                "   {\n"
                "      return;\n"
                "   }\n"
                "   event.preventDefault();\n"
                "\n"
                "   var iframedoc = get_iframedoc();\n"
                "\n"
                "   iframedoc.removeEventListener('click', check_anchor_click);\n"
                "   load_file(event.target.href.split('/').pop());\n"
                "   iframedoc.addEventListener('click', check_anchor_click);\n"
                "}\n"
                "\n"
                "function init_page()\n"
                "{\n"
                "   var iframedoc = get_iframedoc();\n"
                "\n"
                "   document.title = iframedoc.title;\n"
                "   iframedoc.addEventListener('click', check_anchor_click);\n"
                "}\n"
                "\n"
                "</script>\n"
                "</head>\n"
                "<!--\n"
                "     loads 'index.html' on first load\n"
                "-->\n"
                "<body onLoad='load_file(\"index.html\")'>\n"
                "\n"
                "<div id=\"content\">\n"
                "   <iframe id=\"iframe\" width=\"100%\" height=\"100%\" frameborder=\"0\" src=\"about:blank\" onLoad=\"init_page()\" />\n"
                "</div>\n"
                "\n"
                "</body>\n"
                "</html>\n",
                f_salida);
    } /* endif */

    fclose(f_salida);

} /* end imprime_final */

static void imprime_Registros_html() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    marca_t s;
    int i, next;
    boolean dirty;
    valor_t valor_int_reg, valor_fp_reg;
    tipo_t tipo_dato;

    char aux[128];
    char color[64];
    char *clase;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

    fprintf(f_salida, "<table>\n");

    /* Banco de registros de coma flotante */

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"50\" bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 0; i < TAM_REGISTROS; i++) {
        fprintf(f_salida, "  <td width=\"75\" align=\"center\">F%d</td>\n", i);
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
    fprintf(f_salida, "  <td width=\"50\" bgcolor=\"#ddeeff\">rob</td>\n");
    for (i = 0; i < TAM_REGISTROS; i++) {
        if (Rfp[i].rob != MARCA_NULA) {
            FIRMA_2_2("F%d.rob", i, "#&nbsp;%d", Rfp[i].rob);
            fprintf(f_salida,
                    "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);
        } else {
            FIRMA_2_1("F%d.rob", i, "&nbsp;");
            fprintf(f_salida, "  <td>%s</td>\n", vstr);
        } /* endif */
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
    fprintf(f_salida, "  <td width=\"50\" bgcolor=\"#ddeeff\">valor</td>\n");
    for (i = 0; i < TAM_REGISTROS; i++) {
        valor_fp_reg = lee_fp_reg(i, &tipo_dato, &dirty);
        FIRMA_valor("F%d.valor", i, valor_fp_reg, tipo_dato);
        if (Rfp[i].rob != MARCA_NULA) {
            fprintf(f_salida,
                    "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);
        } else {
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        } /* endif */
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    /* Banco de registros de enteros */

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"50\" bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 0; i < TAM_REGISTROS; i++) {
        fprintf(f_salida, "  <td width=\"75\" align=\"center\">R%d<br>(%s)</td>\n", i, nombre_int_reg[i]);
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
    fprintf(f_salida, "  <td width=\"50\" bgcolor=\"#ddeeff\">rob</td>\n");
    for (i = 0; i < TAM_REGISTROS; i++) {
        if (Rint[i].rob != MARCA_NULA) {
            FIRMA_2_2("R%d.rob", i, "#&nbsp;%d", Rint[i].rob);
            fprintf(f_salida,
                    "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);
        } else {
            FIRMA_2_1("R%d.rob", i, "&nbsp;");
            fprintf(f_salida, "  <td>%s</td>\n", vstr);
        } /* endif */
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
    fprintf(f_salida, "  <td width=\"50\" bgcolor=\"#ddeeff\">valor</td>\n");
    for (i = 0; i < TAM_REGISTROS; i++) {
        valor_int_reg = lee_int_reg(i, &tipo_dato, &dirty);
        FIRMA_valor("R%d.valor", i, valor_int_reg, tipo_dato);
        if (Rint[i].rob != MARCA_NULA) {
            fprintf(f_salida,
                    "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);
        } else {
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        } /* endif */
    } /* endif */
    fprintf(f_salida, " </tr>\n");

    fprintf(f_salida, "</table><br>\n");
}

static void imprime_ROB_html() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    marca_t s;
    int i, next;

    char aux[128];
    char aux2[256];
    char color[64];
    char *clase;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"25\">Cod</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"40\">Ocup.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"125\">Instrucci&oacute;n</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"50\">Complet.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"100\">Destino</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"75\">Valor</td>\n");
    //fprintf(f_salida, "  <td align=\"center\" width=\"75\">Cond.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"75\">Pred.</td>\n");
    //fprintf(f_salida, "  <td align=\"center\" width=\"75\">Direc.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Exc.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">PC</td>\n");
    fprintf(f_salida, " </tr>\n");

    for (i = 0; i < TAM_REORDER; i++) {
        if (!RB[i].ocupado && RB[i].orden > 0) {
            clase = "old";
        } else {
            clase = "normal";
        }

        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\" class=\"%s\">\n", clase);

        if (i == RB_inicio)
            fprintf(f_salida,
                "  <td bgcolor=\"#ffddaa\" align=\"center\">%d</td>\n", i);
        else
            fprintf(f_salida,
                "  <td bgcolor=\"#ddeeff\" align=\"center\">%d</td>\n", i);

        if (RB[i].ocupado || (!RB[i].ocupado && RB[i].orden > 0)) {
            FIRMA_2_1("ROB%d.ocupado", i, (RB[i].ocupado) ? "SÍ" : "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            imprime_instruccion(aux, RB[i].PC);
            FIRMA_2_1("ROB%d.inst", i, aux);
            fprintf(f_salida, "  <td align=\"left\" nowrap=\"nowrap\"><code>%s</code></td>\n", vstr);

            FIRMA_2_1("ROB%d.completado", i, (RB[i].completado) ? "SÍ" : "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


            if (inst_store(RB[i].OP)) {
                imprime_codigo(RB[i].dest, aux);
                FIRMA_2_1("ROB%d.dest", i, aux);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

                FIRMA_2_1("ROB%d.valor", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                //FIRMA_2_1("ROB%d.condicion", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("ROB%d.pred", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                // FIRMA_2_1("ROB%d.dir", i, "&nbsp;");
                // fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            } else if (inst_salto(RB[i].OP)) {
                // Dest
                if inst_jal(RB[i].OP) {
                    // SI es jal, ponemos "R31,direccion"
                    if (RB[i].completado) {
                        te_etiqueta(aux, RB[i].direccion);
                        sprintf(aux2, "R"
                                F_DWORD
                                ",%s", RB[i].dest, aux);
                    } else {
                        sprintf(aux2, "R"
                                F_DWORD, RB[i].dest);
                    }
                    FIRMA_2_1("ROB%d.dir", i, aux2);
                    //FIRMA_2_2("ROB%d.dest", i, F_DWORD, 31);
                    //fprintf(f_salida, "  <td align=\"center\">%s%s</td>\n", (inst_int(RB[i].OP)) ? "R" : "F", vstr);
                } else if inst_jr(RB[i].OP) {
                    if (RB[i].completado) {
                        te_etiqueta(aux, RB[i].direccion);
                        FIRMA_2_1("ROB%d.dir", i, aux);
                    } else {
                        FIRMA_2_1("ROB%d.dir", i, "&nbsp;");
                    }
                } else { // Resto de saltos
                    te_etiqueta(aux, RB[i].direccion);
                    FIRMA_2_1("ROB%d.dir", i, aux);
                }
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

                // [Valor,condicion] prediccion
                if (RB[i].OP == OP_JAL) {
                    if (RB[i].completado) {
                        te_etiqueta(aux, RB[i].valor.int_d);
                        sprintf(aux2, "%s,%s", aux, (RB[i].condicion) ? "Salta" : "No salta");
                        FIRMA_2_1("ROB%d.cond", i, aux2);
                    } else {
                        FIRMA_2_1("ROB%d.cond", i, "&nbsp;");
                    }
                    fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);
                    FIRMA_2_1("ROB%d.pred", i, (RB[i].prediccion) ? "Salta" : "No salta");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                } else if (RB[i].OP == OP_JR) {
                    if (RB[i].completado) {
                        FIRMA_2_1("ROB%d.cond", i, (RB[i].condicion) ? "Fallo" : "Acierto");
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    } else {
                        FIRMA_2_1("ROB%d.cond", i, "&nbsp;");
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    }
                    te_etiqueta(aux, RB[i].pred_data.dir_destino);
                    FIRMA_2_1("ROB%d.pred", i, aux);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                } else if (RB[i].OP == OP_JALR) {
                    if (RB[i].completado) {
                        te_etiqueta(aux, RB[i].valor.int_d);
                        sprintf(aux2, "%s,%s", aux, (RB[i].condicion) ? "Fallo" : "Acierto");
                        FIRMA_2_1("ROB%d.cond", i, aux2);
                    } else {
                        FIRMA_2_1("ROB%d.cond", i, "&nbsp;");
                    }
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    te_etiqueta(aux, RB[i].pred_data.dir_destino);
                    FIRMA_2_1("ROB%d.pred", i, aux);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                } else { // Saltos condicionales
                    if (RB[i].completado) {
                        FIRMA_2_1("ROB%d.cond", i, (RB[i].condicion) ? "Salta" : "No salta");
                    } else {
                        FIRMA_2_1("ROB%d.cond", i, "&nbsp;");
                    }
                    fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);
                    FIRMA_2_1("ROB%d.pred", i, (RB[i].prediccion) ? "Salta" : "No salta");
                    fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);
                } /* endif */

            } else if (inst_fpsr(RB[i].OP)) {
                FIRMA_2_1("ROB%d.dest", i, "fpsr");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

                if (RB[i].completado) {
                    FIRMA_2_1("ROB%d.valor", i, (RB[i].valor.int_d) ? "Cierto" : "Falso");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                } else {
                    FIRMA_2_1("ROB%d.valor", i, "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                } /* endif */

                //FIRMA_2_1("ROB%d.cond", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("ROB%d.pred", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                //FIRMA_2_1("ROB%d.dir", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } else if (RB[i].OP == OP_MOVZ || RB[i].OP == OP_MOVN) {
                FIRMA_2_2("ROB%d.dest", i, F_DWORD, RB[i].dest);
                fprintf(f_salida, "  <td align=\"center\">%s%s</td>\n", (inst_int(RB[i].OP)) ? "R" : "F", vstr);

                if (RB[i].completado) {
                    sprintf(aux, F_DWORD, RB[i].valor.int_d);
                    sprintf(aux2, "%s,%s", aux, (RB[i].condicion) ? "Cierto" : "Falso");
                    FIRMA_2_1("ROB%d.valor", i, aux2);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                } else {
                    FIRMA_2_1("ROB%d.valor", i, "&nbsp");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                }

                FIRMA_2_1("ROB%d.pred", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                //FIRMA_2_1("ROB%d.dir", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } else if (RB[i].OP != OP_NOP && RB[i].OP != OP_TRAP) {
                FIRMA_2_2("ROB%d.dest", i, F_DWORD, RB[i].dest);
                fprintf(f_salida, "  <td align=\"center\">%s%s</td>\n", (inst_int(RB[i].OP)) ? "R" : "F", vstr);

                if (RB[i].completado) {
                    //if (inst_int(RB[i].OP)) {
                    //    if (RB[i].valor.int_d <= MAX_WORD)
                    //        FIRMA_2_2("ROB%d.valor", i, F_DWORD, RB[i].valor.int_d);
                    //    else
                    //        FIRMA_2_2("ROB%d.valor", i, "%" PRIx64, RB[i].valor.int_d);
                    //} else
                    FIRMA_valor("ROB%d.valor", i, RB[i].valor, obtener_tipo(RB[i].OP));
                    //FIRMA_2_2("ROB%d.valor", i, "%.2f", RB[i].valor.fp_d);
                } else {
                    FIRMA_2_1("ROB%d.valor", i, "&nbsp");
                }
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

                //FIRMA_2_1("ROB%d.cond", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("ROB%d.pred", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                //FIRMA_2_1("ROB%d.dir", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } else {
                FIRMA_2_1("ROB%d.dest", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("ROB%d.valor", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                //FIRMA_2_1("ROB%d.cond", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("ROB%d.pred", i, "&nbsp;");
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                //FIRMA_2_1("ROB%d.dir", i, "&nbsp;");
                //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            if (RB[i].excepcion != EXC_NONE)
                FIRMA_2_2("ROB%d.exc", i, "%d", RB[i].excepcion);
            else
                FIRMA_2_1("ROB%d.exc", i, "&nbsp;");

            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            te_etiqueta(aux, RB[i].PC);
            FIRMA_2_1("ROB%d.pc", i, aux);
            fprintf(f_salida, "  <td align=\"left\" nowrap='nowrap'>%s</td>\n", vstr);
        } else {
            FIRMA_2_1("ROB%d.ocupado", i, "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.inst", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.completado", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.dest", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.valor", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            //FIRMA_2_1("ROB%d.cond", i, "&nbsp;");
            //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.pred", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            //FIRMA_2_1("ROB%d.dir", i, "&nbsp;");
            //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.exc", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("ROB%d.pc", i, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        } /* endif */

        fprintf(f_salida, " </tr>\n");
    } /* endfor */

    fprintf(f_salida, "</table>\n");

}

static void imprime_RS_enteros_html() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    marca_t s;
    int i, next;

    char aux[256];
    char result[128];
    char cond[128];
    char dir[128];
    char color[64];
    char *clase;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"25\">Cod</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"40\">Ocup.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Op</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Q1</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"75\">V1</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Q2</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"75\">V2</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">rob</td>\n");
    fprintf(f_salida, "  <td align=\"center\">Resultado</td>\n");
    //fprintf(f_salida, "  <td align=\"center\">Cond.</td>\n");
    //fprintf(f_salida, "  <td align=\"center\">Direc.</td>\n");
    fprintf(f_salida, "  <td align=\"center\">Estado</td>\n");
    fprintf(f_salida, " </tr>\n");

    /*** Estaciones de reserva de enteros ***/

    for (s = INICIO_RS_ENTEROS;
            s <= FIN_RS_ENTEROS; s++) {

        if (!RS[s].ocupado && RS[s].orden > 0) {
            clase = "old";
        } else {
            clase = "normal";
        }

        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\" class=\"%s\">\n", clase);

        imprime_codigo(s, aux);
        FIRMA_2_1("RS%d.cod", s, aux);
        fprintf(f_salida, "  <td bgcolor=\"#ddeeff\" align=\"center\">%s</td>\n", vstr);

        if (RS[s].ocupado || (!RS[s].ocupado && RS[s].orden > 0)) {
            FIRMA_2_1("RS%d.ocupado", s, (RS[s].ocupado) ? "SI" : "NO");
            fprintf(f_salida, "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);

            imprime_operacion(RS[s].OP, aux);
            FIRMA_2_1("RS%d.op", s, aux);
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            if (RS[s].Q1 != MARCA_NULA) {
                FIRMA_2_2("RS%d.Q1", s, "#&nbsp;%d", RS[s].Q1);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.V1", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
            } else {
                FIRMA_2_1("RS%d.Q1", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
                FIRMA_2_2("RS%d.V1", s, F_DWORD, RS[s].V1.int_d);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            if (RS[s].Q2 != MARCA_NULA) {
                FIRMA_2_2("RS%d.Q2", s, "#&nbsp;%d", RS[s].Q2);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.V2", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
            } else {
                FIRMA_2_1("RS%d.Q2", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
                FIRMA_2_2("RS%d.V2", s, F_DWORD, RS[s].V2.int_d);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            FIRMA_2_2("RS%d.rob", s, "#&nbsp;%d", RS[s].rob);
            fprintf(f_salida, "  <td>%s</td>\n", vstr);

            switch (RS[s].estado) {
                case EN_CURSO:
                    FIRMA_2_1("RS%d.result", s, "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_3("RS%d.est", s, F_ESTADO, Op[RS[s].operador].ciclo, Op[RS[s].operador].Teval);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    break;
                case FINALIZADA:
                    if (!inst_salto(RS[s].OP)) { // Aritméticas
                        FIRMA_valor("RS%d.result", s, RS[s].resultado, obtener_tipo(RS[s].OP));
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    } else if inst_jr(RS[s].OP) {
                        te_etiqueta(dir, RS[s].resultado.int_d);
                        sprintf(aux, "%s,%s", ((RS[s].condicion) ? "Fallo" : "Acierto"), dir);
                        FIRMA_2_1("RS%d.dir.cond", s, aux);
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    } else { //  //Saltos condicionales
                        FIRMA_2_1("RS%d.cond", s, (RS[s].condicion) ? "SI" : "NO");
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    }


                    FIRMA_2_1("RS%d.est", s, "end");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    break;
                default:
                    FIRMA_2_1("RS%d.result", s, "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    //FIRMA_2_1("RS%d.cond", s, "&nbsp;");
                    //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    //FIRMA_2_1("RS%d.dir", s, "&nbsp;");
                    //fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_1("RS%d.est", s, "&nbsp;");
                    fprintf(f_salida, "  <td>%s</td>\n", vstr);

            }
        } else {

            FIRMA_2_1("RS%d.ocupado", s, "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.op", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.Q1", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.V1", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.Q2", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.V2", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.rob", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.result", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.est", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        } /* endif */

        fprintf(f_salida, " </tr>\n");

    } /* endfor */

    fprintf(f_salida, "</table>\n");
}

static void imprime_RS_FP_html() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    marca_t s;
    int i, next;

    char aux[128];
    char color[64];
    char *clase;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td width=\"25\">Cod</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"40\">Ocup.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Op</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Q1</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"75\">V1</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Q2</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"75\">V2</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">rob</td>\n");
    fprintf(f_salida, "  <td align=\"center\">Resultado</td>\n");
    fprintf(f_salida, "  <td align=\"center\">Estado</td>\n");
    fprintf(f_salida, " </tr>\n");


    /*** Estaciones de reserva de suma/resta ***/

    for (s = INICIO_RS_SUMREST;
            s <= FIN_RS_SUMREST; s++) {
        if (!RS[s].ocupado && RS[s].orden > 0) {
            clase = "old";
        } else {
            clase = "normal";
        }

        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\" class=\"%s\">\n", clase);

        imprime_codigo(s, aux);
        FIRMA_2_1("RS%d.cod", s, aux);
        fprintf(f_salida, "  <td bgcolor=\"#ddeeff\" align=\"center\">%s</td>\n", vstr);

        if (RS[s].ocupado || (!RS[s].ocupado && RS[s].orden > 0)) {
            FIRMA_2_1("RS%d.ocupado", s, (RS[s].ocupado) ? "SI" : "NO");
            fprintf(f_salida, "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);

            imprime_operacion(RS[s].OP, aux);
            FIRMA_2_1("RS%d.op", s, aux);
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            if (RS[s].Q1 != MARCA_NULA) {
                FIRMA_2_2("RS%d.Q1", s, "#&nbsp;%d", RS[s].Q1);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.V1", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
            } else {
                FIRMA_2_1("RS%d.Q1", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
                FIRMA_valor("RS%d.V1", s, RS[s].V1, obtener_tipo(RS[s].OP));
                //FIRMA_2_2("RS%d.V1", s, "%.2f", RS[s].V1.fp_d);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            if (RS[s].Q2 != MARCA_NULA) {
                FIRMA_2_2("RS%d.Q2", s, "#&nbsp;%d", RS[s].Q2);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.V2", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
            } else {
                FIRMA_2_1("RS%d.Q2", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
                FIRMA_valor("RS%d.V2", s, RS[s].V2, obtener_tipo(RS[s].OP));
                //FIRMA_2_2("RS%d.V2", s, "%.2f", RS[s].V2.fp_d);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            FIRMA_2_2("RS%d.rob", s, "#&nbsp;%d", RS[s].rob);
            fprintf(f_salida, "  <td>%s</td>\n", vstr);

            switch (RS[s].estado) {
                case EN_CURSO:
                    FIRMA_2_1("RS%d.result", s, "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_3("RS%d.est", s, F_ESTADO, Op[RS[s].operador].ciclo, Op[RS[s].operador].Teval);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    break;
                case FINALIZADA:
                    FIRMA_valor("RS%d.result", s, RS[s].resultado, obtener_tipo(RS[s].OP));
                    //FIRMA_2_2("RS%d.result", s, "%.2f", RS[s].resultado.fp_d);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_1("RS%d.est", s, "end");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    break;
                default:
                    FIRMA_2_1("RS%d.result", s, "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_1("RS%d.est", s, "&nbsp;");
                    fprintf(f_salida, "  <td>%s</td>\n", vstr);
            }
        } else {
            FIRMA_2_1("RS%d.ocupado", s, "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.op", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.Q1", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.V1", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.Q2", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.V2", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.rob", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.result", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.est", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        } /* endif */

        fprintf(f_salida, " </tr>\n");

    } /* endfor */

    /*** Estaciones de reserva de multiplicación/división ***/

    for (s = INICIO_RS_MULTDIV;
            s <= FIN_RS_MULTDIV; s++) {

        if (!RS[s].ocupado && RS[s].orden > 0) {
            clase = "old";
        } else {
            clase = "normal";
        }

        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\" class=\"%s\">\n", clase);

        imprime_codigo(s, aux);
        FIRMA_2_1("RS%d.cod", s, aux);
        fprintf(f_salida, "  <td bgcolor=\"#ddeeff\" align=\"center\">%s</td>\n", vstr);

        if (RS[s].ocupado || (!RS[s].ocupado && RS[s].orden > 0)) {
            FIRMA_2_1("RS%d.ocupado", s, (RS[s].ocupado) ? "SI" : "NO");
            fprintf(f_salida, "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);

            imprime_operacion(RS[s].OP, aux);
            FIRMA_2_1("RS%d.op", s, aux);
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            if (RS[s].Q1 != MARCA_NULA) {
                FIRMA_2_2("RS%d.Q1", s, "#&nbsp;%d", RS[s].Q1);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.V1", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
            } else {
                FIRMA_2_1("RS%d.Q1", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
                FIRMA_valor("RS%d.V1", s, RS[s].V1, obtener_tipo(RS[s].OP));
                //FIRMA_2_2("RS%d.V1", s, "%.2f", RS[s].V1.fp_d);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            if (RS[s].Q2 != MARCA_NULA) {
                FIRMA_2_2("RS%d.Q2", s, "#&nbsp;%d", RS[s].Q2);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.V2", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
            } else {
                FIRMA_2_1("RS%d.Q2", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
                FIRMA_valor("RS%d.V2", s, RS[s].V2, obtener_tipo(RS[s].OP));
                //FIRMA_2_2("RS%d.V2", s, "%.2f", RS[s].V2.fp_d);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            FIRMA_2_2("RS%d.rob", s, "#&nbsp;%d", RS[s].rob);
            fprintf(f_salida, "  <td>%s</td>\n", vstr);

            switch (RS[s].estado) {
                case EN_CURSO:
                    FIRMA_2_1("RS%d.result", s, "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_3("RS%d.est", s, F_ESTADO, Op[RS[s].operador].ciclo, Op[RS[s].operador].Teval);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    break;
                case FINALIZADA:
                    FIRMA_valor("RS%d.result", s, RS[s].resultado, obtener_tipo(RS[s].OP));
                    //FIRMA_2_2("RS%d.result", s, "%.2f", RS[s].resultado.fp_d);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_1("RS%d.est", s, "end");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    break;
                default:
                    FIRMA_2_1("RS%d.result", s, "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_1("RS%d.est", s, "&nbsp;");
                    fprintf(f_salida, "  <td>%s</td>\n", vstr);
            }
        } else {

            FIRMA_2_1("RS%d.ocupado", s, "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.op", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.Q1", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.V1", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.Q2", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.V2", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.rob", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.result", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.est", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        } /* endif */

        fprintf(f_salida, " </tr>\n");

    } /* endfor */

    fprintf(f_salida, "</table>\n");
}

static void imprime_LB_html() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    marca_t s;
    int i, next;

    char aux[128];
    char color[64];
    char *clase;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Cod</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"40\">Ocup.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Op</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Q1</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"50\">V1</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"50\">Desp(etiq)</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"50\">Direc.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">rob</td>\n");
    fprintf(f_salida, "  <td align=\"center\">Resultado</td>\n");
    fprintf(f_salida, "  <td align=\"center\">Estado</td>\n");
    fprintf(f_salida, " </tr>\n");


    for (s = INICIO_BUFFER_CARGA;
            s <= FIN_BUFFER_CARGA; s++) {

        if (!RS[s].ocupado && RS[s].orden > 0) {
            clase = "old";
        } else {
            clase = "normal";
        }

        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\" class=\"%s\">\n", clase);

        imprime_codigo(s, aux);
        FIRMA_2_1("RS%d.cod", s, aux);
        fprintf(f_salida, "  <td bgcolor=\"#ddeeff\" align=\"center\">%s</td>\n", vstr);

        if (LB[s].ocupado || (!LB[s].ocupado && LB[s].orden > 0)) {
            FIRMA_2_1("RS%d.ocupado", s, LB[s].ocupado ? "SI" : "NO");
            fprintf(f_salida, "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);

            imprime_operacion(LB[s].OP, aux);
            FIRMA_2_1("RS%d.op", s, aux);
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            if (LB[s].Q1 != MARCA_NULA) {
                FIRMA_2_2("RS%d.Q1", s, "#&nbsp;%d", LB[s].Q1);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.V1", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
            } else {
                FIRMA_2_1("RS%d.Q1", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
                FIRMA_2_2("RS%d.V1", s, F_DWORD, LB[s].V1.int_d);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */
            //            FIRMA_2_2("RS%d.desp", s, F_DWORD, LB[s].desplazamiento);
            FIRMA_2_3("RS%d.desp", s, F_DWORD
                    " (%s)", LB[s].desplazamiento, LB[s].etiqueta);
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            //                if (LB[s].estado == PREPARADA) {
            if (LB[s].estado != PENDIENTE) {
                te_etiqueta(aux, LB[s].direccion);

                FIRMA_2_1("RS%d.dir", s, aux);
            } else {
                FIRMA_2_1("RS%d.dir", s, "&nbsp;");
            }
            fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

            FIRMA_2_2("RS%d.rob", s, "#&nbsp;%d", LB[s].rob);
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            switch (RS[s].estado) {
                case EN_CURSO:
                    FIRMA_2_1("RS%d.result", s, "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_3("RS%d.est", s, F_ESTADO, Op[RS[s].operador].ciclo, Op[RS[s].operador].Teval);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    break;
                case FINALIZADA:
                    FIRMA_valor("RS%d.result", s, RS[s].resultado, obtener_tipo(RS[s].OP));
                    //if inst_load_int(RS[s].OP) {
                    //    FIRMA_2_2("RS%d.result", s, F_DWORD, RS[s].resultado.int_d);
                    //} else {
                    //    FIRMA_2_2("RS%d.result", s, "%.2f", RS[s].resultado.fp_d);
                    //}
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_1("RS%d.est", s, "end");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    break;
                default:
                    FIRMA_2_1("RS%d.result", s, "&nbsp;");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    FIRMA_2_1("RS%d.est", s, "&nbsp;");
                    fprintf(f_salida, "  <td>%s</td>\n", vstr);
            }

        } else {

            FIRMA_2_1("RS%d.ocupado", s, "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            FIRMA_2_1("RS%d.op", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.V1", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.Q1", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.desp", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.dir", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.rob", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.result", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.est", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        } /* endif */

        fprintf(f_salida, " </tr>\n");

    } /* endfor */

    fprintf(f_salida, "</table>\n");
}

static void imprime_SB_html() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    marca_t s;
    int i, next;

    char aux[128];
    char color[64];
    char *clase;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    nueva_firma(); /*** FIRMAS ***/

    fprintf(f_salida, "<table>\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Cod</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"40\">Ocup.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Op</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Q1</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"50\">V1</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"50\">Desp(etiq)</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"50\">Direc.</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">rob</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"25\">Q2</td>\n");
    fprintf(f_salida, "  <td align=\"center\" width=\"50\">V2</td>\n");
    fprintf(f_salida, "  <td align=\"center\">Confirm.</td>\n");
    fprintf(f_salida, "  <td align=\"center\">Estado</td>\n");
    fprintf(f_salida, " </tr>\n");

    for (s = INICIO_BUFFER_ALMACEN;
            s <= FIN_BUFFER_ALMACEN; s++) {

        if (!RS[s].ocupado && RS[s].orden > 0) {
            clase = "old";
        } else {
            clase = "normal";
        }

        fprintf(f_salida, " <tr bgcolor=\"#eeeeee\" class=\"%s\">\n", clase);

        imprime_codigo(s, aux);
        FIRMA_2_1("RS%d.cod", s, aux);
        fprintf(f_salida, "  <td bgcolor=\"#ddeeff\" align=\"center\">%s</td>\n", vstr);

        if (SB[s].ocupado || (!SB[s].ocupado && SB[s].orden > 0)) {
            FIRMA_2_1("RS%d.ocupado", s, SB[s].ocupado ? "SI" : "NO");
            fprintf(f_salida, "  <td bgcolor=\"#ffddaa\" align=\"center\">%s</td>\n", vstr);

            imprime_operacion(SB[s].OP, aux);
            FIRMA_2_1("RS%d.op", s, aux);
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            if (SB[s].Q1 != MARCA_NULA) {
                FIRMA_2_2("RS%d.Q1", s, "#&nbsp;%d", SB[s].Q1);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.V1", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
            } else {
                FIRMA_2_1("RS%d.Q1", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
                FIRMA_2_2("RS%d.V1", s, F_DWORD, SB[s].V1.int_d);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */

            //FIRMA_2_2("RS%d.desp", s, F_DWORD, SB[s].desplazamiento);
            FIRMA_2_3("RS%d.desp", s, F_DWORD
                    " (%s)", SB[s].desplazamiento, SB[s].etiqueta);
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            //                if (SB[s].estado == PREPARADA) {
            if (LB[s].estado != PENDIENTE) {
                te_etiqueta(aux, SB[s].direccion);

                FIRMA_2_1("RS%d.dir", s, aux);
            } else {
                FIRMA_2_1("RS%d.dir", s, "&nbsp;");
            }
            fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

            FIRMA_2_2("RS%d.rob", s, "#&nbsp;%d", LB[s].rob);
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            if (SB[s].Q2 != MARCA_NULA) {
                FIRMA_2_2("RS%d.Q2", s, "#&nbsp;%d", SB[s].Q2);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                FIRMA_2_1("RS%d.V2", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
            } else {
                FIRMA_2_1("RS%d.Q2", s, "&nbsp;");
                fprintf(f_salida, "  <td>%s</td>\n", vstr);
                FIRMA_valor("RS%d.V2", s, RS[s].V2, obtener_tipo(RS[s].OP));
                //if inst_store_int(SB[s].OP) {
                //    FIRMA_2_2("RS%d.V2", s, F_DWORD, SB[s].V2.int_d);
                //} else {
                //    FIRMA_2_2("RS%d.V2", s, "%.2f", SB[s].V2.fp_d);
                //}
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            } /* endif */


            FIRMA_2_2("RS%d.conf", s, "%s", (SB[s].confirm) ? "SI" : "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);

            switch (RS[s].estado) {
                case EN_CURSO:
                    FIRMA_2_3("RS%d.est", s, F_ESTADO, Op[RS[s].operador].ciclo, Op[RS[s].operador].Teval);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    break;
                case FINALIZADA:
                    FIRMA_2_1("RS%d.est", s, "end");
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    break;
                default:
                    FIRMA_2_1("RS%d.est", s, "&nbsp;");
                    fprintf(f_salida, "  <td>%s</td>\n", vstr);
            }

        } else {

            FIRMA_2_1("RS%d.ocupado", s, "NO");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.busy", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.V1", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.Q1", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.desp", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.dir", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.rob", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.Q2", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.V2", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.conf", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            FIRMA_2_1("RS%d.est", s, "&nbsp;");
            fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
        } /* endif */

        fprintf(f_salida, " </tr>\n");

    } /* endfor */

    fprintf(f_salida, "</table>\n");
}

/***************************************************************
 *
 * Func: imprime_estado
 *
 * Desc: Imprime el estado de las estructuras de coma flotante
 *
 **************************************************************/

void imprime_estado_html
(
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    sprintf(aux, "estado" F_LCICLO ".html", Ciclo);
    if (html_merge == SI) {
        fprintf(f_salida, "'%s':`", aux);
    } else {
        f_salida = fopen(aux, "w");
        if (f_salida == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */

    //nueva_firma(); /*** FIRMAS ***/

    /*** HTML INTRO ***/

    fprintf(f_salida,
            "<html><head><title>Estado: %s. Ciclo " F_CICLO "</title>\n",
            nombre_fich, Ciclo);
    fprintf(f_salida, "<style type='text/css'>");
    fprintf(f_salida, ".old td {color:#aaa; font-style: italic;}");
    fprintf(f_salida, "</style>");
    fprintf(f_salida, "</head>");
    fprintf(f_salida,
            "<body bgcolor=\"white\">\n");

    fprintf(f_salida, "<a href=\"index.html\">INICIO</a>&nbsp;\n");
    fprintf(f_salida, "<a href=\"final.html\">FINAL</a>&nbsp;\n");
    if (Ciclo > 10)
        fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">[-10]</a>&nbsp;\n",
            Ciclo - 10);
    else
        fprintf(f_salida, "[-10]&nbsp;\n");
    if (Ciclo > 5)
        fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">[-5]</a>&nbsp;\n",
            Ciclo - 5);
    else
        fprintf(f_salida, "[-5]&nbsp;\n");
    if (Ciclo > 1)
        fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">[-1]</a>&nbsp;\n",
            Ciclo - 1);
    else
        fprintf(f_salida, "[-1]\n");

    if (!final)
        fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">[+1]</a>\n", Ciclo + 1);
    else
        fprintf(f_salida, "[+1]&nbsp;\n");
    if (!final)
        fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">[+5]</a>\n", Ciclo + 5);
    else
        fprintf(f_salida, "[+5]&nbsp;\n");
    if (!final)
        fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">[+10]</a>\n", Ciclo + 10);
    else
        fprintf(f_salida, "[+10]&nbsp;\n");

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<!a href=\"estado" F_LCICLO ".html\">Estado<!/a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">Crono</a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">BTB</a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<tt><b>Programa: %s</b></tt>\n", nombre_fich);
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<b>Ciclo: " F_CICLO "</b>\n", Ciclo);
    fprintf(f_salida, "<br>\n");

    fprintf(f_salida, "<p>Estado al final del ciclo\n");


    /*** ORGANIZACIÖN EN DOS COLUMNAS  */
    fprintf(f_salida, "<table>\n");
    fprintf(f_salida, "<tr valign=\"top\">\n");
    fprintf(f_salida, "<td>\n");

    /*** Bancos de registros ***/

    imprime_Registros_html();

    fprintf(f_salida, "<table bgcolor=\"white\">\n");
    fprintf(f_salida, " <tr valign=\"top\"><td>\n");

    /*** Reorder buffer ***/
    imprime_ROB_html();

    fprintf(f_salida, "  </td><td>\n");

    /*** Estaciones de reserva ***/

    /* Contenido de las estaciones de reserva */

    imprime_RS_enteros_html();
    imprime_RS_FP_html();


    fprintf(f_salida, "  </td></tr>\n");
    fprintf(f_salida, "</table><br>\n");

    fprintf(f_salida, "<table bgcolor=\"white\">\n");
    fprintf(f_salida, " <tr valign=\"top\"><td>\n");

    /*** Tampones de lectura ***/

    imprime_LB_html();

    fprintf(f_salida, "  </td>\n");
    fprintf(f_salida, "  <td>\n");

    imprime_SB_html();

    fprintf(f_salida, "  </td></tr>\n");
    fprintf(f_salida, "</table>\n");


    /*** SEGUNDA COLUMNA **********************************************/

    fprintf(f_salida, "</td>\n");
    fprintf(f_salida, "<td>\n");

    /*** Contenido de la memoria de datos ***/

    imprime_DMem_html();

    fprintf(f_salida, "</td>\n");
    fprintf(f_salida, "</tr>\n");
    fprintf(f_salida, "</table>\n");

    fprintf(f_salida, "<address>Arquitectura e Ingenier&iacute;a de Computadores</address>\n");
    fprintf(f_salida, "</body></html>\n");

    if (html_merge == SI) {
        fputs("`,", f_salida);
    } else {
        fclose(f_salida);
    } /* endif */

    if (error_firma) {

        fprintf(stderr, "ERROR. "
                "Existen diferencias entre la ejecución actual y el fichero de firmas.\n"
                "Analice el fichero de estado 'estado" F_LCICLO ".html'\n", Ciclo);
        exit(1);
    } /* endif */

} /* end imprime_estado */

/***************************************************************
 *
 * Func: imprime_crono
 *
 * Desc: Imprime el cronograma de ejecución
 *
 **************************************************************/

void imprime_crono_html
(
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[128];

    short x, y;
    char *color;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    sprintf(aux, "crono" F_LCICLO ".html", Ciclo);
    if (html_merge == SI) {
        fprintf(f_salida, "'%s':`", aux);
    } else {
        f_salida = fopen(aux, "w");
        if (f_salida == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */

    /*** HTML INTRO ***/

    fprintf(f_salida, "<html>\n");
    fprintf(f_salida, "<head>\n");
    fprintf(f_salida, "<title>Cronograma: %s. Ciclo " F_CICLO "</title>\n",
            nombre_fich, Ciclo);
    fprintf(f_salida,
            "<style type=\"text/css\">\n"
            " td.address {\n"
            "     text-align: right; white-space: nowrap; padding: 0 0.2em;\n"
            "}\n"
            "</style>\n");
    fprintf(f_salida, "</head>\n");

    fprintf(f_salida,
            "<body bgcolor=\"white\">\n");

    fprintf(f_salida, "<a href=\"index.html\">INICIO</a>&nbsp;\n");
    fprintf(f_salida, "<a href=\"final.html\">FINAL</a>&nbsp;\n");
    if (Ciclo > 10)
        fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">[-10]</a>&nbsp;\n",
            Ciclo - 10);
    else
        fprintf(f_salida, "[-10]&nbsp;\n");
    if (Ciclo > 5)
        fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">[-5]</a>&nbsp;\n",
            Ciclo - 5);
    else
        fprintf(f_salida, "[-5]&nbsp;\n");
    if (Ciclo > 1)
        fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">[-1]</a>&nbsp;\n",
            Ciclo - 1);
    else
        fprintf(f_salida, "[-1]\n");

    if (!final)
        fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">[+1]</a>\n", Ciclo + 1);
    else
        fprintf(f_salida, "[+1]&nbsp;\n");
    if (!final)
        fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">[+5]</a>\n", Ciclo + 5);
    else
        fprintf(f_salida, "[+5]&nbsp;\n");
    if (!final)
        fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">[+10]</a>\n", Ciclo + 10);
    else
        fprintf(f_salida, "[+10]&nbsp;\n");
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">Estado</a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<!a href=\"crono" F_LCICLO ".html\">Crono<!/a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">BTB</a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<tt><b>Programa: %s</b></tt>\n", nombre_fich);
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<b>Ciclo: " F_CICLO "</b>\n", Ciclo);
    fprintf(f_salida, "<br>\n");

    fprintf(f_salida, "<p>Estado al final del ciclo\n");

    /*** Cronograma de ejecución ***/

    fprintf(f_salida, "<style>th.sticky{position: sticky; top: 0; z-index: 10; background-color: #ccddff;}</style>\n");

    fprintf(f_salida, "<table style='font-family: monospace'><thead>\n");
    fprintf(f_salida, "  <!-- T-Cronograma -->\n");

    fprintf(f_salida, " <tr bgcolor=\"#ccddff\" align=\"center\">\n");
    fprintf(f_salida, "  <th width=\"20\" class=\"sticky\">PC</th>\n");
    fprintf(f_salida, "  <th width=\"150\" class=\"sticky\">Instruc.</th>\n");
    fprintf(f_salida, "  <!-- T-Linea -->\n");
    fprintf(f_salida, "  <!-- T-PC %s -->\n", "PC");
    fprintf(f_salida, "  <!-- T-Instr %s -->\n", "Instruccion");
    for (x = 0; x < MAX(MAX_CICLOS_INIC, maxima_columna_visible); x++) {
        fprintf(f_salida,
                "  <th width=\"25\" class=\"sticky\"><a href=\"crono" F_LCICLO ".html\">" F_CICLO "</a></th>\n",
                x + primer_ciclo, x + primer_ciclo);
        fprintf(f_salida, "  <!-- T-Ciclo-H " F_CICLO " " F_CICLO " -->\n", x + primer_ciclo, x + primer_ciclo);
    }
    fprintf(f_salida, " </tr></thead><tbody>\n");
    fprintf(f_salida, "  <!-- T-Fin-Linea -->\n");

    /* Busca la primera línea del crono con una etapa activa */
    for (y = 0; y < MAX_INSTRUC; y++)
        for (x = 0; x < MAX(MAX_CICLOS_INIC, maxima_columna_visible); x++)
            if (strlen(crono[y].fase[x]))
                goto etapa_encontrada;
etapa_encontrada:

    for (; y < MAX_INSTRUC; y++) {
        if (y % 2 == 0)
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\" align=\"center\">\n");
        else
            fprintf(f_salida, " <tr bgcolor=\"#dddddd\" align=\"center\">\n");
        fprintf(f_salida, "  <!-- T-Linea -->\n");


        color = (y % 2 == 0) ? "#ddeeff" : "#ccddff";
        if (!strlen(crono[y].inst)) {
            // PC
            fprintf(f_salida,
                    "  <td bgcolor=\"%s\">&nbsp;</td>\n", color);
            // Instruc
            fprintf(f_salida,
                    "  <td bgcolor=\"%s\">&nbsp;</td>\n", color);
        } else {
            te_etiqueta(aux, crono[y].PC);

            fprintf(f_salida,
                    "  <td class=\"address\" bgcolor=\"%s\">%s</td>\n",
                    color, aux);
            fprintf(f_salida, "  <!-- T-PC %s -->\n", aux);

            fprintf(f_salida,
                    "  <td nowrap=\"nowrap\" align=\"left\" bgcolor=\"%s\">%s</td>\n",
                    color, crono[y].inst);
            fprintf(f_salida, "  <!-- T-Instr  %s -->\n", crono[y].inst);
        }

        for (x = 0; x < MAX(MAX_CICLOS_INIC, maxima_columna_visible); x++) {
            fprintf(f_salida, "  <td>");

            if (strlen(crono[y].fase[x])) {
                fprintf(f_salida, "%s", crono[y].fase[x]);
                fprintf(f_salida, "  </td>\n");
                fprintf(f_salida, "  <!-- T-Ciclo-D %d %s -->\n", x + 1, crono[y].fase[x]);
            } else {
                fprintf(f_salida, "&nbsp;");
                fprintf(f_salida, "  </td>\n");
                fprintf(f_salida, "  <!-- T-Ciclo-D %d %s -->\n", x + 1, "BLANCO");
            }
        } /* endfor */
        fprintf(f_salida, " </tr>\n");
        fprintf(f_salida, "  <!-- T-Fin-Linea -->\n");
    } /* endfor */
    fprintf(f_salida, "  <!-- T-Fin-Cronograma -->\n");
    fprintf(f_salida, "</tbody></table>\n");

    fprintf(f_salida, "<address>Arquitectura e Ingenier&iacute;a de Computadores</address>\n");
    fprintf(f_salida, "</body></html>\n");

    if (html_merge == SI) {
        fputs("`,", f_salida);
    } else {

        fclose(f_salida);
    } /* endif */

} /* end imprime_crono */

/***************************************************************
 *
 * Func: imprime_predictor
 *
 * Desc: Imprime el estado del predictor
 *
 **************************************************************/

void imprime_predictor_html
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

    sprintf(aux, "predictor" F_LCICLO ".html", Ciclo);
    if (html_merge == SI) {
        fprintf(f_salida, "'%s':`", aux);
    } else {
        f_salida = fopen(aux, "w");
        if (f_salida == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */

    nueva_firma(); /*** FIRMAS ***/

    /*** HTML INTRO ***/

    fprintf(f_salida,
            "<html><head><title>Predictor: %s. Ciclo " F_CICLO "</title></head>\n",
            nombre_fich, Ciclo);
    fprintf(f_salida,
            "<body bgcolor=\"white\">\n");

    fprintf(f_salida, "<a href=\"index.html\">INICIO</a>&nbsp;\n");
    fprintf(f_salida, "<a href=\"final.html\">FINAL</a>&nbsp;\n");
    if (Ciclo > 10)
        fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">[-10]</a>&nbsp;\n",
            Ciclo - 10);
    else
        fprintf(f_salida, "[-5]&nbsp;\n");
    if (Ciclo > 5)
        fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">[-5]</a>&nbsp;\n",
            Ciclo - 5);
    else
        fprintf(f_salida, "[-5]&nbsp;\n");
    if (Ciclo > 1)
        fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">[-1]</a>&nbsp;\n",
            Ciclo - 1);
    else
        fprintf(f_salida, "[-1]\n");

    if (!final)
        fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">[+1]</a>\n", Ciclo + 1);
    if (!final)
        fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">[+5]</a>\n", Ciclo + 5);
    if (!final)
        fprintf(f_salida, "<a href=\"predictor" F_LCICLO ".html\">[+10]</a>\n", Ciclo + 10);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"estado" F_LCICLO ".html\">Estado</a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<a href=\"crono" F_LCICLO ".html\">Crono</a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<!a href=\"predictor" F_LCICLO ".html\">BTB<!/a>\n", Ciclo);

    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<tt><b>Programa: %s</b></tt>\n", nombre_fich);
    fprintf(f_salida, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida, "<b>Ciclo: " F_CICLO "</b>\n", Ciclo);
    fprintf(f_salida, "<br>\n");

    fprintf(f_salida, "<p>Estado al final del ciclo\n");

    /*  Con propósitos de depuración. Funciona con 1 vía */
    if (NUM_VIAS_ISSUE == 1) {
        if (inst_salto(IF_ISS_1.IR.codop)) {
            imprime_instruccion_color(aux, IF_ISS_1.PC, NO);
            fprintf(f_salida, "<p>IF: PC= "
                    F_DWORD
                    "; Instr=%s; Pred local=%d Pred global=%d -> Prediccion=%d; BHR local=%d; BHR global=%d\n",
                    IF_ISS_1.PC, aux, IF_ISS_1.pred_data.pred_local,
                    IF_ISS_1.pred_data.pred_global, IF_ISS_1.prediccion, IF_ISS_1.pred_data.BHR_local, IF_ISS_1.pred_data.BHR_global);
            //fprintf(f_salida, "<p>    Pred local=%d; Pred global=%d\n", IF_ISS_1.pred_data.pred_local,
            //        IF_ISS_1.pred_data.pred_global);

        }

        if (inst_salto(RB[RB_inicio].OP) && (RB[RB_inicio].ocupado) && (RB[RB_inicio].completado)) {
            imprime_instruccion_color(aux, RB[RB_inicio].PC, NO);
            fprintf(f_salida,
                    "<p>&rarr;C: PC="
                    F_DWORD
                    "; Instr=%s, Prediccion=%d; Condicion=%d; BHR local=%d; BHR global=%d\n",
                    RB[RB_inicio].PC, aux, RB[RB_inicio].prediccion, RB[RB_inicio].condicion,
                    RB[RB_inicio].pred_data.BHR_local, RB[RB_inicio].pred_data.BHR_global);
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
        itobin(branch_mask_local, aux, n_bits_branch_mask_local);
        //itoa(branch_mask_local, aux, 2);
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

                imprime_estado_predictor_html(aux, BTB[i].estado);
                FIRMA_2_1("BTB%d.estado", i, aux);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);


                te_etiqueta(aux, BTB[i].destino);

                FIRMA_2_1("BTB%d.destino", i, aux);
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

                FIRMA_2_2("BTB%d.orden", i, F_CICLO, BTB[i].orden);
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

        fprintf(f_salida, "<p>BTB\n");

        /* Mascara global del predictor correlacionado */
        if (n_bits_branch_mask_local > 0) {
            fprintf(f_salida, "<table>\n");
            fprintf(f_salida, " <tr bgcolor=\"#ccddff\">\n");
            fprintf(f_salida, "  <td width=\"150\">BHR local: </td>\n");
            fprintf(f_salida, " </tr>\n");
            fprintf(f_salida, " <tr bgcolor=\"#eeeeee\">\n");
            itobin(branch_mask_local, aux, n_bits_branch_mask_local);
            //itoa(branch_mask_local, aux, 2);
            FIRMA_1_2("Branch Mask Local", "%0x", branch_mask_local);
            fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s(%d)</td>\n", aux, branch_mask_local);
            fprintf(f_salida, " </tr>\n");
            fprintf(f_salida, "</table>\n");
        }

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
                        imprime_estado_predictor_n_html(aux, BTB[i].v_estado[j], n_bits_pred_local);
                        // Faltaria una nueva firma
                        FIRMA_2_1("BTB%d.v_estado", i, aux);
                        fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                    }
                }

                /* Selector. Se muestra en el predictor híbrido */
                if (tipo_predictor == HYBRID_n_BIT_S) {
                    imprime_estado_selector_n_html(aux, BTB[i].estado_selector);
                    FIRMA_1_1("Hybrid.estado", aux);
                    fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
                }

                te_etiqueta(aux, BTB[i].destino);
                FIRMA_2_1("BTB%d.destino", i, aux);
                fprintf(f_salida, "  <td align=\"center\" nowrap='nowrap'>%s</td>\n", vstr);

                FIRMA_2_2("BTB%d.orden", i, F_CICLO, BTB[i].orden);
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
                    FIRMA_1_1("Hybrid.estado", "&nbsp;");
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
            itobin(branch_mask_global, aux, n_bits_branch_mask_global);
            //itoa(branch_mask_global, aux, 2);
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
                imprime_estado_predictor_n_html(aux, Global[j], n_bits_pred_global);

                FIRMA_2_1("Global%d", j, aux);
                fprintf(f_salida, "  <td align=\"center\">%s</td>\n", vstr);
            }
            fprintf(f_salida, " </tr>\n");
            fprintf(f_salida, "</table>\n");
        }
    }

    fprintf(f_salida, "<address>Arquitectura e Ingenier&iacute;a de Computadores</address>\n");
    fprintf(f_salida, "</body></html>\n");

    if (html_merge == SI) {
        fputs("`,", f_salida);
    } else {
        fclose(f_salida);
    } /* endif */

    if (error_firma) {

        fprintf(stderr, "ERROR. "
                "Existen diferencias entre la ejecución actual y el fichero de firmas.\n"
                "Analice el fichero de estado 'estado" F_LCICLO ".html'\n", Ciclo);
        exit(1);
    } /* endif */

} /* end imprime_predictor */
