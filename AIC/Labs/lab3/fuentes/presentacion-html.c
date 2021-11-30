
/**************************************************************************
 *
 * Departamento de Informatica de Sistemas y Computadores (DISCA)
 * Universidad Politecnica de Valencia.
 *
 * Author:	Sergio Sáez (ssaez@disca.upv.es)
 *			Pedro López (plopez@disca.upv.es)
 *
 * File: presentacion-html.c
 *
 * Description: Contiene las funciones necesarias para presentar la
 *    informacion sobre la ejecucion del algoritmo
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
#include <inttypes.h>



/***** Definiciones Externas **********************************/

#include "tipos.h"
#include "main.h"
#include "instrucciones.h"
#include "control.h"

#include "datapath.h"
#include "presentacion-html.h"
#include "depurar.h"
#include "memoria.h"
#include "registros.h"


#ifdef MAX
#undef MAX
#endif
#define MAX(x,y) (((x)>(y)) ? (x) : (y))

#define S_READ "&#8592"
#define S_WRITE "&#8594"
#define S_READWRITE "&#8646"


/***** Variables Globales *************************************/

/***** Variables Locales **************************************/

FILE * f_salida_html;
static FILE * f_consola;
static FILE * f_log;

static ciclo_t primera_instruccion = 1;
static ciclo_t primer_ciclo = 1;
//static short maxima_columna_visible = MAX_CICLOS_INIC;

int MAX_INSTRUC = _MAX_INSTRUC;
int MAX_CICLOS_INIC = _MAX_CICLOS_INIC;
int MAX_CICLOS = _MAX_CICLOS;

static short maxima_columna_visible;

/*** Variables ************************************************/

char p_str[256]; /* Cadena auxiliar */

linea_crono_t crono[_MAX_INSTRUC];
/* Instrucciones visibles en el
   cronograma */


/***** Funciones Locales **************************************/

/***************************************************************
 *
 * Func: init_presentacion()
 *
 * Desc: Modifica algunos parámetros para pantalas pequeñas
 *
 **************************************************************/
void init_presentacion() {
    if (small_screen_flag) {
        MAX_INSTRUC = _MAX_INSTRUC_s;
        MAX_CICLOS_INIC = _MAX_CICLOS_INIC_s;
        MAX_CICLOS = _MAX_CICLOS_s;
        maxima_columna_visible = MAX_CICLOS_INIC;
    }
}

/***************************************************************
 *
 * Func: imprime_intro
 *
 * Desc: Imprime la intro HTML
 *
 **************************************************************/

static void imprime_intro(char* titulo) {

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /*** HTML INTRO ***/

    fprintf(f_salida_html, "<!DOCTYPE html>\n");
    fprintf(f_salida_html,
            "<html lang=\"es-ES\">\n<head>\n<title>%s:%s. Ciclo %u</title>\n",
            titulo, nombre_fich, Ciclo);
    fprintf(f_salida_html,
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n");
    fprintf(f_salida_html,
            "</head>\n<body bgcolor=\"white\">\n\n");

}

/***************************************************************
 *
 * Func: imprime_w3css
 *
 * Desc: Imprime la intro HTML incluyendo el archivo css
 *
 **************************************************************/

static void imprime_w3css(char* titulo) {

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /*** HTML INTRO ***/

    fprintf(f_salida_html, "<!DOCTYPE html>\n");
    fprintf(f_salida_html,
            "<html lang=\"es-ES\">\n<head>\n<title>%s:%s. Ciclo %u</title>\n",
            titulo, nombre_fich, Ciclo);
    fprintf(f_salida_html,
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n");
    fprintf(f_salida_html,
            "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n");
    fprintf(f_salida_html,
            "<link rel=\"stylesheet\" href=\"https://www.w3schools.com/lib/w3.css\">\n");
    fprintf(f_salida_html, "<style>\n");
    fprintf(f_salida_html, "html, body {\n");
    fprintf(f_salida_html, "    font-family:\"%s\", Times, serif;\n", FONT_NAME);
    fprintf(f_salida_html, "    font-size:%dpt;\n", FONT_SIZE);
    fprintf(f_salida_html, "    line-height:%s;\n", LINE_HEIGHT);
    fprintf(f_salida_html, "}\n");
    fprintf(f_salida_html, "</style>\n");
    fprintf(f_salida_html, "</head>\n<body>\n\n");

}

/***************************************************************
 *
 * Func: imprime_footer
 *
 * Desc: Imprime el final HTML
 *
 **************************************************************/
static void imprime_footer() {

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /*** HTML FINAL ***/
    fprintf(f_salida_html, "<!-- Final -->\n");
    //    fprintf(f_salida_html, "<div class=\"w3-row w3-container\">\n");
    fprintf(f_salida_html, "<address>\n");
    fprintf(f_salida_html, "Arquitectura e Ingeniería de Computadores<br>\n");
    fprintf(f_salida_html, "Departamento de Informática de Sistemas y Computadores (DISCA)<br>\n");
    fprintf(f_salida_html, "Universitat Politècnica de València<br>\n");
    fprintf(f_salida_html, "</address>\n");
    //	fprintf(f_salida_html, "</div>\n\n");

    fprintf(f_salida_html, "</body>\n</html>\n");
}

/***************************************************************
 *
 * Func: imprime_footer_css
 *
 * Desc: Imprime el final HTML con css
 *
 **************************************************************/
static void imprime_footer_css() {

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /*** HTML FINAL ***/
    fprintf(f_salida_html, "<!-- Final -->\n");
    fprintf(f_salida_html, "<div class=\"w3-row w3-container w3-light-grey\">\n");
    fprintf(f_salida_html, "<address>\n");
    fprintf(f_salida_html, "Arquitectura e Ingeniería de Computadores<br>\n");
    fprintf(f_salida_html, "Departamento de Informática de Sistemas y Computadores (DISCA)<br>\n");
    fprintf(f_salida_html, "Universitat Politècnica de València<br>\n");
    fprintf(f_salida_html, "</address>\n");
    fprintf(f_salida_html, "</div>\n\n");

    fprintf(f_salida_html, "</body>\n</html>\n");
}

/***************************************************************
 *
 * Func: imprime_nav1
 *
 * Desc: Imprime la barra de navegación
 *
 **************************************************************/

static void imprime_nav1() {

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Barra de navegación */
    fprintf(f_salida_html, "<!-- Barra de navegacion -->\n");


    fprintf(f_salida_html, "<a href=\"index.html\">INICIO</a>&nbsp;\n");
    fprintf(f_salida_html, "<a href=\"final.html\">FINAL</a>&nbsp;\n");

    fprintf(f_salida_html, "<a href=\"estado%03d.html\">Estado</a>\n", 1);

    fprintf(f_salida_html, "&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida_html, "<a href=\"crono%03d.html\">Cronograma</a>\n", 1);

    /*
    fprintf(f_salida_html, "&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida_html, "<a href=\"datapath%03d.html\">Datapath</a>\n", 1);
     */

    fprintf(f_salida_html, "&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida_html, "Programa: %s\n\n", nombre_fich);
}

/***************************************************************
 *
 * Func: imprime_nav2
 *
 * Desc: Imprime la barra de navegación con estado ciclo a ciclo
 *
 **************************************************************/

static void imprime_nav2(char* archivo) {

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Barra de navegación */
    fprintf(f_salida_html, "<!-- Barra de navegacion -->\n");
    fprintf(f_salida_html, "<a href=\"index.html\">INICIO</a>&nbsp;\n");
    fprintf(f_salida_html, "<a href=\"final.html\">FINAL</a>&nbsp;\n");
    if (Ciclo > 5)
        fprintf(f_salida_html, "<a href=\"%s%03u.html\">[-5]</a>&nbsp;\n", archivo,
            Ciclo - 5);
    else
        fprintf(f_salida_html, "[-5]&nbsp;\n");
    if (Ciclo > 1)
        fprintf(f_salida_html, "<a href=\"%s%03u.html\">[-1]</a>&nbsp;\n", archivo,
            Ciclo - 1);
    else
        fprintf(f_salida_html, "[-1]\n");

    fprintf(f_salida_html, "<a href=\"%s%03u.html\">[+1]</a>\n", archivo, Ciclo + 1);
    fprintf(f_salida_html, "<a href=\"%s%03u.html\">[+5]</a>\n", archivo, Ciclo + 5);

    fprintf(f_salida_html, "&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida_html, "<a href=\"estado%03u.html\">Estado</a>\n", Ciclo);

    fprintf(f_salida_html, "&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida_html, "<a href=\"crono%03u.html\">Crono</a>\n", Ciclo);

    //    fprintf(f_salida_html, "&nbsp;&nbsp;&nbsp;\n");
    //    fprintf(f_salida_html, "<a href=\"datapath%03ld.html\">Datapath</a>\n", Ciclo);

    fprintf(f_salida_html, "&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida_html, "Programa: %s\n", nombre_fich);
    fprintf(f_salida_html, "&nbsp;&nbsp;&nbsp;\n");
    fprintf(f_salida_html, "<b>Ciclo: %u</b>\n\n", Ciclo);
}

/***************************************************************
 *
 * Func: imprime_nav1_css
 *
 * Desc: Imprime la barra de navegación, utilizando CSS
 *
 **************************************************************/

static void imprime_nav1_css() {


    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Barra de navegación */
    fprintf(f_salida_html, "<!-- Barra de navegacion CSS -->\n");
    fprintf(f_salida_html, "<div class=\"w3-bar w3-top w3-light-grey\">\n");


    //sprintf(_msg, "Getenv %s\n", getenv("MIPS_HOME"));
    //DEBUG(__func__);

    if (home_link_flag) {
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s\">HOME</a>\n", HOME_LINK);
    }
    fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"index.html\">INICIO</a>\n");
    fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button\" href=\"final.html\">FINAL</a>\n");
    if (salida == html) {
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button\" href=\"result001.html\">Estado</a>\n");
    }

    fprintf(f_salida_html, "</div>\n\n");
}

/***************************************************************
 *
 * Func: imprime_nav2_css
 *
 * Desc: Imprime la barra de navegación con estado ciclo a ciclo, utilizando CSS
 *
 **************************************************************/

static void imprime_nav2_css(char* archivo) {

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Barra de navegación */
    fprintf(f_salida_html, "<!-- Barra de navegacion CSS -->\n");
    fprintf(f_salida_html, "<div class=\"w3-bar w3-top w3-light-grey\">\n");
    if (home_link_flag) {
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s\">HOME</a>\n", HOME_LINK);
    }
    fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"index.html\">INICIO</a>\n");
    fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"final.html\">FINAL</a>\n");

    if (Ciclo > 10)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-10]</a>\n", archivo,
            Ciclo - 15);
    else
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-10]</a>\n", archivo,
            Ciclo);

    if (Ciclo > 5)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-5]</a>\n", archivo,
            Ciclo - 5);
    else
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-5]</a>\n", archivo,
            Ciclo);
    if (Ciclo > 1)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-1]</a>\n", archivo,
            Ciclo - 1);
    else
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[-1]</a>\n", archivo,
            Ciclo);

    if (!final)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[+1]</a>\n", archivo,
            Ciclo + 1);

    if (!final)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[+5]</a>\n", archivo,
            Ciclo + 5);

    if (!final)
        fprintf(f_salida_html, "<a class=\"w3-bar-item w3-button w3-hover-grey\" href=\"%s%03u.html\">[+10]</a>\n", archivo,
            Ciclo + 10);

    fprintf(f_salida_html, "</div>\n\n");
}

/***************************************************************
 *
 * Func: imprime_conf
 *
 * Desc: Imprime la configuraci�n
 *
 **************************************************************/

static void imprime_conf() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/


    /*** Configuración ***/

    fprintf(f_salida_html, "<!-- Configuración -->\n");

    fprintf(f_salida_html, "<table>\n");

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td colspan=\"2\" bgcolor=\"%s\" align=\"center\">Configuración</td>\n", BG_HEADER);
    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">Parámetro</td>\n", BG_HEADER_COL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">Valor</td>\n", BG_HEADER_COL2);
    fprintf(f_salida_html, "</tr>\n");

    /*** Nombre programa ***/
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Programa</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%s</td>\n", BG_VAL2, nombre_fich);
    fprintf(f_salida_html, "</tr>\n");

    /*** Riesgos datos ***/
    switch (solucion_riesgos_datos) {
        case ninguno:
            sprintf(aux, "%s", "Ninguno");
            break;
        case parada:
            sprintf(aux, "%s", "Stalls");
            break;
        case cortocircuito:
            sprintf(aux, "%s", "Forwarding");
    };

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Riesgos de datos</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%s</td>\n", BG_VAL2, aux);
    fprintf(f_salida_html, "</tr>\n");

    switch (solucion_riesgos_control) {
        case ds3:
            sprintf(aux, "%s", "S. Retardado (DS=3)");
            break;
        case ds2:
            sprintf(aux, "%s", "S. Retardado (DS=2)");
            break;
        case ds1:
            sprintf(aux, "%s", "S. Retardado (DS=1)");
            break;
        case pnt3:
            sprintf(aux, "%s", "Predict-not-taken (Lat=3)");
            break;
        case pnt2:
            sprintf(aux, "%s", "Predict-not-taken (Lat=2)");
            break;
        case pnt1:
            sprintf(aux, "%s", "Predict-not-taken (Lat=1)");
            break;
        case stall3:
            sprintf(aux, "%s", "Stalls (Lat=3)");
            break;
        case stall2:
            sprintf(aux, "%s", "Stalls (Lat=2)");
            break;
        case stall1:
            sprintf(aux, "%s", "Stalls (Lat=1)");
    };

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Riesgos de control&nbsp;&nbsp;</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%s</td>\n", BG_VAL2, aux);
    fprintf(f_salida_html, "</tr>\n\n");

    /*** Registros ***/
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Registros</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%d</td>\n", BG_VAL2, TAM_REGISTROS);
    fprintf(f_salida_html, "</tr>\n\n");

    /*** Latencias ***/
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Lat. L/S&nbsp;&nbsp;</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%d</td>\n", BG_VAL2, FP_LS_LAT);
    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Lat. FP ADD&nbsp;&nbsp;</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%d</td>\n", BG_VAL2, FP_ADD_LAT);
    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Lat. FP CMP&nbsp;&nbsp;</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%d</td>\n", BG_VAL2, FP_CMP_LAT);
    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\">Lat. FP MUL&nbsp;&nbsp;</td>\n", BG_VAL1);
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">%d</td>\n", BG_VAL2, FP_MUL_LAT);
    fprintf(f_salida_html, "</tr>\n\n");

    fprintf(f_salida_html, "</table>\n\n");
}

/***************************************************************
 *
 * Func: desplazamiento_horizontal
 *
 * Description:
 *   Desplaza la zona visible un ciclo hacia la izquierda
 *
 **************************************************************/

static void desplazamiento_horizontal() {
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

        /* Borra la última columna */
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

static void desplazamiento_vertical() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i, j, desp;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Averigua si la línea se va a quedar vacia al principio */
    for (desp = 0;
            desp < MAX_CICLOS &&
            !strlen(crono[1].fase[desp]); desp++);
    if (desp == MAX_CICLOS)
        desp = 1; /* La línea está completamente vacia. Avanzamos sólo un
                                 * ciclo */


    for (i = 0; i < MAX_INSTRUC - 1; i++) {
        strcpy(crono[i].inst, crono[i + 1].inst);
        strcpy(crono[i].PC, crono[i + 1].PC);

        for (j = 0; j < MAX_CICLOS - desp; j++)
            strcpy(crono[i].fase[j], crono[i + 1].fase[j + desp]);

        /* Borra las últimas columnas */
        for (j = MAX_CICLOS - desp; j < MAX_CICLOS; j++)
            strcpy(crono[i].fase[j], "");
    } /* endfor */

    /* Borra la última fila */
    crono[MAX_INSTRUC - 1].inst[0] = '\0';
    crono[MAX_INSTRUC - 1].PC[0] = '\0';
    for (j = 0; j < MAX_CICLOS; j++)
        crono[MAX_INSTRUC - 1].fase[j][0] = '\0';

    /*** Actualiza los desplazamientos iniciales ***/

    primer_ciclo += desp;
    primera_instruccion++;
    maxima_columna_visible -= desp;

} /* end desplazamiento_vertical */

/***************************************************************
 *
 * Func: copy_trunc_str
 *
 * Desc: Trunca un string a un numero dado de posiciones y si exced
 * pone un + al final
 *
 **************************************************************/

/*
static void copy_trunc_str(char * result, char * src, int posiciones) {

    memset(result, '\0', strlen(result));
    //result[0] = 0;
    strncpy(result, src, posiciones);
    if (strlen(src) > posiciones) {
        strcat(result, "+");

    }
}
 */

/***************************************************************
 *
 * Func: muestra_senyal
 *
 * Description:
 *   Imprime el nombre de la señal, si está activada
 *
 **************************************************************/

static void muestra_senyal(boolean senyal, char nombre[128]) {
    char aux[128];

    if (senyal == SI)
        sprintf(aux, "%s", nombre);
    else
        sprintf(aux, "&nbsp;");
    fprintf(f_salida_html, "<td width=%d>%s</td>\n", SENYALES_W, aux);
}

/***************************************************************
 *
 * Func: muestra_fase
 *
 * Description:
 *   Muestra la fase de cada instrucción en el cronograma
 *
 **************************************************************/

static void muestra_fase(
        char * fase,
        ciclo_t orden
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
        strcpy(crono[o].fase[c], fase);
        if (c + 1 > maxima_columna_visible)
            maxima_columna_visible = c + 1;
    } /* endif */

} /* end muestra_fase */

/***************************************************************
 *
 * Func: imprime_int_regs
 *
 * Desc: Imprime el estado de los registros enteros
 *
 **************************************************************/

static void imprime_int_regs() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i, j;

    char col_ID[20], col_WB[20];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Banco de registros enteros */

    // Colores
    color_instruccion(col_ID, IF_ID.iPC);
    color_instruccion(col_WB, MEM_WB.iPC);

    fprintf(f_salida_html, "<!-- Registros enteros -->\n");
    fprintf(f_salida_html, "<table>\n");

    j = 0;
    while (j < TAM_REGISTROS) {
        fprintf(f_salida_html, "<tr>\n");
        fprintf(f_salida_html, "<td width=%d bgcolor=\"%s\">Rint</td>\n", REG_W, BG_OPER);

        for (i = 0; i < (NUM_REG_VISIBLES < TAM_REGISTROS - j ? NUM_REG_VISIBLES : TAM_REGISTROS - j); i++) {
            fprintf(f_salida_html, "<td bgcolor=\"%s\" align=center>R%d</td>\n", BG_REGS, i + j);
        } /* endfor */
        fprintf(f_salida_html, "</tr>\n");

        fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_VAL);
        fprintf(f_salida_html, "<td bgcolor=\"%s\">&nbsp</td>\n", "white");
        for (i = 0; i < (NUM_REG_VISIBLES < TAM_REGISTROS - j ? NUM_REG_VISIBLES : TAM_REGISTROS - j); i++) {
            // El registro se escribe
            if (hay_destino_WB() && (MEM_WB.IR.Rdestino == (i + j))) {
                // Y tambien se lee
                if ((hay_fuente1_ID() && (IF_ID.IR.Rfuente1 == (i + j))) ||
                        (hay_fuente2_ID() && (IF_ID.IR.Rfuente2 == (i + j)))) {
                    fprintf(f_salida_html, "<td width=%d align=\"right\" bgcolor=\"%s\"><i>" F_DWORD "</i></td>\n", REG_W, BG_RW,
                            Rint[i + j].valor);
                    // Solo se escribe
                } else {
                    fprintf(f_salida_html, "<td width=%d align=\"right\" bgcolor=\"%s\">" F_DWORD "</td>\n", REG_W, col_WB,
                            Rint[i + j].valor);
                }
                // El registro se lee
            } else if ((hay_fuente1_ID() && (IF_ID.IR.Rfuente1 == (i + j))) ||
                    (hay_fuente2_ID() && (IF_ID.IR.Rfuente2 == (i + j)))) {
                fprintf(f_salida_html, "<td width=%d align=\"right\" bgcolor=\"%s\">" F_DWORD "</td>\n", REG_W, col_ID,
                        Rint[i + j].valor);
                // Ni se lee ni se escribe
            } else {
                fprintf(f_salida_html, "<td width=%d align=\"right\">" F_DWORD "</td>\n", REG_W,
                        Rint[i + j].valor);
            }
            //			fprintf(f_salida_html, "<td align=center>" F_DWORD "</td>\n",
            //					Rint[i+j].valor);
        } /* endfor */
        j = j + NUM_REG_VISIBLES;
    }
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "</table>\n\n");


} /* end imprime_int_regs */

/***************************************************************
 *
 * Func: imprime_int_regs_col
 *
 * Desc: Imprime el estado de los registros enteros en una columna
 *
 **************************************************************/

static void imprime_int_regs_col() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i, j;

    char col_ID[20], col_WB[20];
    char vstr[128];
    char aux2[256];

    char reg_name[128];


    dword valor_reg;
    tipo_t tipo_dato;
    boolean dirty;


    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Banco de registros enteros */

    // Colores
    color_instruccion(col_ID, IF_ID.iPC);
    color_instruccion(col_WB, MEM_WB.iPC);

    fprintf(f_salida_html, "<!-- Registros enteros en columna -->\n");
    fprintf(f_salida_html, "<table>\n");


    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_HEADER);
    fprintf(f_salida_html, "  <td align=\"center\" colspan=2>Registros int</td>\n");
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">Registro</td>\n", BG_DIR1);
    fprintf(f_salida_html, "  <td width=%d bgcolor=\"%s\" align=\"center\">Valor</td>\n", REG_W, BG_DIR2);
    fprintf(f_salida_html, "</tr>\n");

    i = 0; // NO hace falta, pero así reutilizo el código
    j = 0;
    while (j < TAM_REGISTROS) {
        fprintf(f_salida_html, "<tr>\n");
        /*		fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                                                "R%d&nbsp;&nbsp;</td>\n", BG_VAL1, i+j); */
        valor_reg = lee_int_reg(i + j, &tipo_dato, &dirty);
        switch (tipo_dato) {
            case t_byte:
            case t_char:
                // Valor
                sprintf(vstr, F_DWORD "(%c)", valor_reg, (char) valor_reg);
                if (dirty) {
                    sprintf(aux2, "<td align=\"center\" bgcolor=\"%s\">%s</td>\n", BG_VAL2, vstr);
                } else {
                    sprintf(aux2, "<td align=\"center\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                }
                break;
            default:
                if (valor_reg <= MAX_WORD) {
                    sprintf(vstr, F_DWORD, valor_reg);
                } else {
                    sprintf(vstr, "%" PRIx64, valor_reg);
                }
                // Valor
                if (dirty) {
                    sprintf(aux2, "<td align=\"center\" bgcolor=\"%s\">%s</td>\n", BG_VAL2, vstr);
                } else {
                    sprintf(aux2, "<td align=\"center\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                }
                break;
        } /* endswitch */
        // Valor
        /*
sprintf(aux1, "%lld", Rint[i + j].valor);
copy_trunc_str(aux2, aux1, 6);
color_tipo(t_dword, valor, aux2, Rint_tipo[i + j]);
         */

        //sprintf(reg_name, "R%d&nbsp;&nbsp;", i + j);
        sprintf(reg_name, "R%d (%s)&nbsp;&nbsp;", i + j, nombre_int_reg[i + j]);

        // El registro se escribe
        if (hay_destino_WB() && (MEM_WB.IR.Rdestino == (i + j))) {
            // Y tambien se lee
            if ((hay_fuente1_ID() && (IF_ID.IR.Rfuente1 == (i + j))) ||
                    (hay_fuente2_ID() && (IF_ID.IR.Rfuente2 == (i + j)))) {
                // Rx
                //fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                //        S_READWRITE " R%d&nbsp;&nbsp;</td>\n", BG_RW, i + j);
                fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                        S_READWRITE " %s</td>\n", col_WB, reg_name);
                // Valor
                fprintf(f_salida_html, "%s\n", aux2);
                // Solo se escribe
            } else {
                // Rx
                //fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                //        S_WRITE " R%d&nbsp;&nbsp;</td>\n", col_WB, i + j);
                fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                        S_WRITE " %s</td>\n", col_WB, reg_name);
                // Valor
                fprintf(f_salida_html, "%s\n", aux2);
            }
            // El registro se lee
        } else if ((hay_fuente1_ID() && (IF_ID.IR.Rfuente1 == (i + j))) ||
                (hay_fuente2_ID() && (IF_ID.IR.Rfuente2 == (i + j)))) {
            // Rx
            //fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
            //        S_READ " R%d&nbsp;&nbsp;</td>\n", col_ID, i + j);
            fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                    S_READ " %s</td>\n", col_ID, reg_name);
            // Valor
            fprintf(f_salida_html, "%s\n", aux2);
            // Ni se lee ni se escribe
        } else {
            // Rx
            //fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
            //        "R%d&nbsp;&nbsp;</td>\n", BG_VAL1, i + j);
            fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                    " %s</td>\n", BG_VAL1, reg_name);
            // Valor
            fprintf(f_salida_html, "%s\n", aux2);
        }

        fprintf(f_salida_html, "</tr>\n");
        j = j + 1;
    } /* while */

    fprintf(f_salida_html, "</table>\n\n");

} /* end imprime_int_regs_col */

/***************************************************************
 *
 * Func: imprime_fp_regs
 *
 * Desc: Imprime el estado de los registros enteros
 *
 **************************************************************/

static void imprime_fp_regs() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i, j;

    char col_ID[20], col_WB[20], col_CMP_WB[20];


    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Banco de registros enteros */

    // Colores
    color_instruccion(col_ID, IF_ID.iPC);
    color_instruccion(col_WB, FP_WB.iPC);
    color_instruccion(col_CMP_WB, CMP_WB.iPC);

    fprintf(f_salida_html, "<!-- Registros fp -->\n");
    fprintf(f_salida_html, "<table>\n");

    j = 0;
    while (j < TAM_REGISTROS) {
        fprintf(f_salida_html, "<tr>\n");
        fprintf(f_salida_html, "<td width=%d bgcolor=\"%s\">Rfp</td>\n", REG_W, BG_OPER);

        for (i = 0; i < (NUM_REG_VISIBLES < TAM_REGISTROS - j ? NUM_REG_VISIBLES : TAM_REGISTROS - j); i++) {
            fprintf(f_salida_html, "<td bgcolor=\"%s\" align=center>F%d</td>\n", BG_REGS, i + j);
        } /* endfor */
        fprintf(f_salida_html, "</tr>\n");

        fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_VAL);
        fprintf(f_salida_html, "<td bgcolor=\"%s\">&nbsp</td>\n", "white");
        for (i = 0; i < (NUM_REG_VISIBLES < TAM_REGISTROS - j ? NUM_REG_VISIBLES : TAM_REGISTROS - j); i++) {
            // El registro se escribe
            if (hay_destino_FP_WB() && (FP_WB.IR.Rdestino == (i + j))) {
                // Y tambien se lee
                if ((hay_fuente1_FP_ID() && (IF_ID.IR.Rfuente1 == (i + j))) ||
                        (hay_fuente2_FP_ID() && (IF_ID.IR.Rfuente2 == (i + j)))) {
                    fprintf(f_salida_html, "<td width=%d align=\"right\" bgcolor=\"%s\"><i>%6.1lf</i></td>\n", REG_W, BG_RW,
                            Rfp[i + j].valor.fp_d);
                    // Solo se escribe
                } else {
                    fprintf(f_salida_html, "<td width=%d align=\"right\" bgcolor=\"%s\">%6.1lf</td>\n", REG_W, col_WB,
                            Rfp[i + j].valor.fp_d);
                }
                // El registro se lee
            } else if ((hay_fuente1_FP_ID() && (IF_ID.IR.Rfuente1 == (i + j))) ||
                    (hay_fuente2_FP_ID() && (IF_ID.IR.Rfuente2 == (i + j)))) {
                fprintf(f_salida_html, "<td width=%d align=\"right\" bgcolor=\"%s\">%6.1lf</td>\n", REG_W, col_ID,
                        Rfp[i + j].valor.fp_d);
                // Ni se lee ni se escribe
            } else {
                fprintf(f_salida_html, "<td width=%d align=\"right\">%6.1lf</td>\n", REG_W,
                        Rfp[i + j].valor.fp_d);
            }
            //			fprintf(f_salida_html, "<td align=center>" F_DWORD "</td>\n",
            //					Rint[i+j].valor);
        } /* endfor */
        j = j + NUM_REG_VISIBLES;
    }
    // El FPSR
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_REGS);
    fprintf(f_salida_html, "<td width=%d bgcolor=\"%s\">Rfp</td>\n", REG_W, BG_OPER);

    fprintf(f_salida_html, "<td align=center>FPSR</td>\n");
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td width=%d bgcolor=\"%s\">&nbsp</td>\n", REG_W, "white");
    // El registro se escribe
    if (CMP_WB.IR.Rdestino == REG_FPSR) {
        // Y tambien se lee
        if (inst_branch_fp(IF_ID.IR.codop)) {
            fprintf(f_salida_html, "<td align=right bgcolor=\"%s\"><i>" F_DWORD "</i></td>\n",
                    BG_RW, Rint[TAM_REGISTROS].valor);
            // Solo se escribe
        } else {
            fprintf(f_salida_html, "<td align=right bgcolor=\"%s\">" F_DWORD "</td>\n",
                    col_CMP_WB, Rint[TAM_REGISTROS].valor);
        }
        // El registro se lee
    } else if (inst_branch_fp(IF_ID.IR.codop)) {
        fprintf(f_salida_html, "<td align=right bgcolor=\"%s\">" F_DWORD "</td>\n",
                col_ID, Rint[TAM_REGISTROS].valor);
        // Ni se lee ni se escribe
    } else {
        fprintf(f_salida_html, "<td align=right>" F_DWORD "</td>\n", Rint[TAM_REGISTROS].valor);
    }

    //		fprintf(f_salida_html, "<td align=center>" F_DWORD "</td>\n",Rint[TAM_REGISTROS].valor);

    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "</table>\n\n");


} /* end imprime_fp_regs */

/***************************************************************
 *
 * Func: imprime_fp_regs_col
 *
 * Desc: Imprime el estado de los registros enteros en una columna
 *
 **************************************************************/

static void imprime_fp_regs_col() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i, j;

    char col_ID[20], col_WB[20], col_CMP_WB[20];
    char aux1[128],
            aux2[256], aux3[512];

    valor_t valor_fp_reg;
    dword valor_int_reg;
    tipo_t tipo_dato;
    boolean dirty;


    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Banco de registros fp */

    // Colores
    color_instruccion(col_ID, IF_ID.iPC);
    color_instruccion(col_WB, FP_WB.iPC);
    color_instruccion(col_CMP_WB, CMP_WB.iPC);

    fprintf(f_salida_html, "<!-- Registros fp en columna -->\n");
    fprintf(f_salida_html, "<table>\n");


    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_HEADER);
    fprintf(f_salida_html, "  <td align=\"center\" colspan=3>Registros fp</td>\n");
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">Registro</td>\n", BG_DIR1);
    //  fprintf(f_salida_html, "  <td width=%d bgcolor=\"%s\" align=\"center\">double</td>\n", REG_W, BG_DIR2);
    fprintf(f_salida_html, "  <td width=%d bgcolor=\"%s\" align=\"center\">Hi</td>\n", REG_W, BG_DIR2);
    fprintf(f_salida_html, "  <td width=%d bgcolor=\"%s\" align=\"center\">Lo</td>\n", REG_W, BG_DIR2);
    fprintf(f_salida_html, "</tr>\n");

    i = 0; // NO hace falta, pero así reutilizo el código
    j = 0;
    while (j < TAM_REGISTROS) {
        fprintf(f_salida_html, "<tr>\n");
        /*		fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                                                "F%d&nbsp;&nbsp;</td>\n", BG_VAL1, i+j); */
        valor_fp_reg = lee_fp_reg(i + j, &tipo_dato, &dirty);
        switch (tipo_dato) {
            case t_double:
            case t_ninguno:
                // Valor
                sprintf(aux1, "%6.1lf", valor_fp_reg.fp_d);
                if (dirty) {
                    sprintf(aux3, "<td align=\"center\" colspan=\"2\" bgcolor=\"%s\">%s</td>\n", BG_VAL2, aux1);
                } else {
                    sprintf(aux3, "<td align=\"center\" colspan=\"2\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, aux1);
                }
                break;
            case t_float:
                sprintf(aux1, "%6.1f", 0.0);
                if (dirty) {
                    sprintf(aux2, "<td align=\"right\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, aux1);
                } else {
                    sprintf(aux2, "<td align=\"right\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, aux1);
                }
                sprintf(aux1, "%6.1f", valor_fp_reg.fp_s);
                if (dirty) {
                    sprintf(aux3, "%s<td align=\"right\" bgcolor=\"%s\">%s</td>\n", aux2, BG_VAL2, aux1);
                } else {
                    sprintf(aux3, "%s<td align=\"right\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", aux2, BG_VAL2, TEXT_GRAY, aux1);
                }
                break;
            case t_float_ps:
                sprintf(aux1, "%6.1f", valor_fp_reg.fp_ps[Hi]);
                if (dirty) {
                    sprintf(aux2, "<td align=\"right\" bgcolor=\"%s\">%s</td>\n", BG_VAL2, aux1);
                } else {
                    sprintf(aux2, "<td align=\"right\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, aux1);
                }
                sprintf(aux1, "%6.1f", valor_fp_reg.fp_ps[Lo]);
                if (dirty) {
                    sprintf(aux3, "%s<td align=\"right\" bgcolor=\"%s\">%s</td>\n", aux2, BG_VAL2, aux1);
                } else {
                    sprintf(aux3, "%s<td align=\"right\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", aux2, BG_VAL2, TEXT_GRAY, aux1);
                }
                break;
            default:
                fnError("Imprimir FP regs con tipo de datos no FP\n");
        }
        /*
        // Valor
        sprintf(aux1, "%6.1lf", Rfp[i + j].valor.fpdouble);
        copy_trunc_str(aux2, aux1, 6);
        color_tipo(t_double, valor_double, aux2, Rfp_tipo[i + j]);

        sprintf(aux1, "%6.1f", Rfp[i + j].valor.fpsingle[Hi]);
        copy_trunc_str(aux2, aux1, 6);
        color_tipo(t_float_ps, valor_single_hi, aux2, Rfp_tipo[i + j]);

        sprintf(aux1, "%6.1f", Rfp[i + j].valor.fpsingle[Lo]);
        copy_trunc_str(aux2, aux1, 6);
        color_float(valor_single_lo, aux2, Rfp_tipo[i + j]);
         */


        // El registro se escribe
        if (hay_destino_FP_WB() && (FP_WB.IR.Rdestino == (i + j))) {
            // Y tambien se lee
            if ((hay_fuente1_FP_ID() && (IF_ID.IR.Rfuente1 == (i + j))) ||
                    (hay_fuente2_FP_ID() && (IF_ID.IR.Rfuente2 == (i + j)))) {
                // Fx
                fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                        S_READWRITE " F%d&nbsp;&nbsp;</td>\n", col_WB, i + j);

                // Valor

                fprintf(f_salida_html, "%s", aux3);


                // Solo se escribe
            } else {
                // Fx
                fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                        S_WRITE " F%d&nbsp;&nbsp;</td>\n", col_WB, i + j);
                // Valor
                fprintf(f_salida_html, "%s", aux3);

            }
            // El registro se lee
        } else if ((hay_fuente1_FP_ID() && (IF_ID.IR.Rfuente1 == (i + j))) ||
                (hay_fuente2_FP_ID() && (IF_ID.IR.Rfuente2 == (i + j)))) {
            // Fx
            fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                    S_READ " F%d&nbsp;&nbsp;</td>\n", col_ID, i + j);
            // Valor
            fprintf(f_salida_html, "%s", aux3);

            // Ni se lee ni se escribe
        } else {
            // Fx
            fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                    "F%d&nbsp;&nbsp;</td>\n", BG_VAL1, i + j);
            // Valor
            fprintf(f_salida_html, "%s", aux3);

        }

        fprintf(f_salida_html, "</tr>\n");
        j = j + 1;
    } /* while */
    // FPSR
    /*	fprintf(f_salida_html, "  <td align=\"center\" bgcolor=\"%s\">"
                                            "FPSR</td>\n", BG_VAL1); */
    valor_int_reg = lee_int_reg(TAM_REGISTROS, &tipo_dato, &dirty);
    sprintf(aux1, F_DWORD, valor_int_reg);
    if (dirty) {
        sprintf(aux2, "<td align=\"center\" colspan=\"2\" bgcolor=\"%s\">%s</td>\n", BG_VAL2, aux1);
    } else {
        sprintf(aux2, "<td align=\"center\" colspan=\"2\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, aux1);
    }


    // El registro se escribe
    if (CMP_WB.IR.Rdestino == REG_FPSR) {
        // Y tambien se lee
        if (inst_branch_fp(IF_ID.IR.codop)) {
            // FPSR
            fprintf(f_salida_html, "  <td align=\"center\" bgcolor=\"%s\">"
                    S_READWRITE " FPSR</td>\n", BG_RW);
            // Valor
            fprintf(f_salida_html, "%s\n", aux2);
            // Solo se escribe
        } else {
            // FPSR
            fprintf(f_salida_html, "  <td align=\"center\" bgcolor=\"%s\">"
                    S_WRITE " FPSR</td>\n", col_CMP_WB);
            // Valor
            fprintf(f_salida_html, "%s\n", aux2);
        }
        // El registro se lee
    } else if (inst_branch_fp(IF_ID.IR.codop)) {
        // FPSR
        fprintf(f_salida_html, "  <td align=\"center\" bgcolor=\"%s\">"
                S_READ " FPSR</td>\n", col_ID);
        // Valor
        fprintf(f_salida_html, "%s\n", aux2);
        // Ni se lee ni se escribe
    } else {
        // FPSR
        fprintf(f_salida_html, "  <td align=\"center\" bgcolor=\"%s\">"
                "FPSR</td>\n", BG_VAL1);
        // Valor
        fprintf(f_salida_html, "%s\n", aux2);
    }

    fprintf(f_salida_html, "</table>\n\n");
    //fprintf(f_salida_html, "</div>\n");

} /* end imprime_fp_regs_col */

int en_rango(dword dir_palabra, dword dir) {

    return ((dir >= dir_palabra) && (dir <= (dir_palabra + sizeof (word) - 1)));
}

/***************************************************************
 *
 * Func: imprime_Dmem_col
 *
 * Desc: Imprime el estado de la memoria de datos en una columna
 *
 **************************************************************/

static void imprime_Dmem_col() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    dword i;
    dword d;
    dword next;
    char aux[128];

    char col_M[20], col_LS[20];

    valor_t dato;
    tipo_t tipo;
    //char vstr_col[128];
    char vstr[128];
    boolean dirty;



    /*************************************/
    /*  Function body                    */
    /*************************************/

    // Colores
    // Colores
    color_instruccion(col_M, EX_MEM.iPC);
    color_instruccion(col_LS, LS[FP_LS_LAT].iPC);


    /*** Contenido de la memoria de datos ***/

    for (region_t r = 0; r < num_regiones; r++) {
        if (!es_region_datos(r) || tam_region(mmem, r) == 0)
            continue;

        fprintf(f_salida_html, "<table>\n");

        fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_HEADER);
        fprintf(f_salida_html, " <td align=\"center\" colspan=5>Memoria de Datos. Region %d</td>\n", r);
        fprintf(f_salida_html, "</tr>\n");
        fprintf(f_salida_html, "<tr>\n");
        fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">Dirección</td>\n", BG_DIR1);
        for (i = 0; i < 4; i++) {
            fprintf(f_salida_html, "  <td bgcolor=\"%s\" width=\"%d\" align=\"center\">+%lld</td>\n", BG_DIR2, MEM_W, i);
        } /* endif */
        fprintf(f_salida_html, " </tr>\n");

        next = 0;
        for (d = inicio_region(mmem, r); d < final_region(mmem, r); d++) {

            i = dir_a_offset(mmem, d, r);

            if (d % sizeof (word) == 0) {
                // Falta ver si se lee o escribe
                fprintf(f_salida_html, " <tr bgcolor=\"%s\">\n", BG_VAL2);
                te_etiqueta(aux, d);
                //color_etiqueta_memdatos(color, i);

                if (inst_load_int(EX_MEM.IR.codop) && en_rango(d, EX_MEM.ALUout)) {
                    // LOAD
                    fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                            S_READ " %s&nbsp;&nbsp;</td>\n", col_M, aux);
                } else if (inst_store_int(EX_MEM.IR.codop) && en_rango(d, EX_MEM.ALUout)) {
                    // STORE
                    fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                            S_WRITE " %s&nbsp;&nbsp;</td>\n", col_M, aux);
                } else if (inst_load_fp(LS[FP_LS_LAT].IR.codop) && en_rango(d, LS[FP_LS_LAT].address)) {
                    // LOAD
                    fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                            S_READ " %s&nbsp;&nbsp;</td>\n", col_LS, aux);
                } else

                    if (inst_store_fp(LS[FP_LS_LAT].IR.codop) && en_rango(d, LS[FP_LS_LAT].address)) {
                    // STORE
                    fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                            S_WRITE " %s&nbsp;&nbsp;</td>\n", col_LS, aux);
                } else {
                    fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                            "%s&nbsp;&nbsp;</td>\n", BG_VAL1, aux);
                }
            } /* endif */

            // printf("Region %d Abs %d Offset %d \n", r, d, i);
            if (i == next) {
                tipo = md_tipo[r][i];
                //printf("%d,%hhd\n", i, dato.int_b);

                switch (tipo) {
                    case t_char:
                    case t_byte:
                        dato = mem_lee_region_datos(mmem, r, i, tipo, &dirty);
                        sprintf(vstr, F_BYTE"(%c)", dato.int_b, dato.int_b);
                        if (dirty) {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" bgcolor=\"%s\">%s</span></td>\n", BG_VAL2, vstr);
                        } else {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                        }
                        //FIRMA_2_2("MD%d", i, "%hhX", MD.m_byte[dir_byte(i)]);
                        next = i + sizeof (byte);
                        break;
                    case t_half:
                        dato = mem_lee_region_datos(mmem, r, i, tipo, &dirty);

                        sprintf(vstr, F_HALF, dato.int_h);
                        if (dirty) {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" colspan=\"2\" bgcolor=\"%s\">%s</span></td>\n", BG_VAL2, vstr);
                        } else {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" colspan=\"2\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                        }
                        //FIRMA_2_2("MD%d", i, "%hd", MD.m_half[dir_half(i)]);
                        next = i + sizeof (half);
                        break;
                    case t_word:
                        dato = mem_lee_region_datos(mmem, r, i, tipo, &dirty);

                        sprintf(vstr, F_WORD, dato.int_w);
                        if (dirty) {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" colspan=\"4\" bgcolor=\"%s\">%s</span></td>\n", BG_VAL2, vstr);
                        } else {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" colspan=\"4\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                        }
                        //FIRMA_2_2("MD%d", i, "%d", MD.m_word[dir_word(i)]);
                        next = i + sizeof (word);
                        break;
                    case t_dword:
                        dato = mem_lee_region_datos(mmem, r, i, tipo, &dirty);

                        if (dato.int_d <= MAX_WORD) {
                            sprintf(vstr, F_DWORD, dato.int_d);
                        } else {
                            sprintf(vstr, "%" PRIx64, dato.int_d);
                        }

                        if (dirty) {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" rowspan=\"2\" colspan=\"4\" bgcolor=\"%s\">%s</span></td>\n", BG_VAL2, vstr);
                        } else {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" rowspan=\"2\" colspan=\"4\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                        }
                        //FIRMA_2_2("MD%d", i, F_DWORD, MD.m_dword[dir_dword(i)]);
                        next = i + sizeof (dword);
                        break;
                    case t_float:
                        dato = mem_lee_region_datos(mmem, r, i, tipo, &dirty);

                        sprintf(vstr, "%5.1f", dato.fp_s);
                        if (dirty) {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" colspan=\"4\" bgcolor=\"%s\">%s</span></td>\n", BG_VAL2, vstr);
                        } else {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" colspan=\"4\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                        }
                        //FIRMA_2_2("MD%d", i, "%.2f", MD.m_float[dir_float(i)]);
                        next = i + sizeof (float);
                        break;
                    case t_double:
                        dato = mem_lee_region_datos(mmem, r, i, tipo, &dirty);

                        sprintf(vstr, "%5.1lf", dato.fp_d);
                        if (dirty) {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" rowspan=\"2\" colspan=\"4\" bgcolor=\"%s\">%s</span></td>\n", BG_VAL2, vstr);
                        } else {
                            fprintf(f_salida_html,
                                    "  <td align=\"center\" rowspan=\"2\" colspan=\"4\" bgcolor=\"%s\"><span style=\"color: %s;\">%s</span></td>\n", BG_VAL2, TEXT_GRAY, vstr);
                        }

                        //FIRMA_2_2("MD%d", i, "%.2f", MD.m_double[dir_double(i)]);
                        next = i + sizeof (double);
                        break;
                    default:
                        fprintf(f_salida_html, "  <td>&nbsp;</td>\n");
                        next = i + 1;
                } /* endswitch */
            } /* endif */
            if ((i + 1) % sizeof (word) == 0) {
                fprintf(f_salida_html, " </tr>\n");
            } /* endif */
        } /* endif */
        fprintf(f_salida_html, "</table>\n\n");
    }
}

/***************************************************************
 *
 * Func: imprime_Imem_col
 *
 * Desc: Imprime el estado de la memoria de instrucciones en una columna
 *
 **************************************************************/

static void imprime_Imem_col() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i = 0;
    char aux[128];



    /*************************************/
    /*  Function body                    */
    /*************************************/


    /*** Contenido de la memoria de instrucciones ***/

    fprintf(f_salida_html, "<!-- Mem Instr en columna -->\n");
    fprintf(f_salida_html, "<table>\n");

    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_HEADER);
    fprintf(f_salida_html, "  <td align=\"center\" colspan=2>Memoria de Instrucciones</td>\n");
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "  <td bgcolor=\"%s\" align=\"center\">Dirección</td>\n", BG_DIR1);
    fprintf(f_salida_html, "  <td width=%d bgcolor=\"%s\" align=\"center\">Instrucciones</td>\n", INSTR_CRONO_W, BG_DIR2);
    fprintf(f_salida_html, "</tr>\n");

    for (i = inicio_region(mmem, r_text); i < final_region(mmem, r_text); i += 4) {
        fprintf(f_salida_html, " <tr>\n");
        te_etiqueta(aux, i);
        fprintf(f_salida_html, "  <td align=\"right\" bgcolor=\"%s\">"
                "%s&nbsp;&nbsp;</td>\n", BG_VAL1, aux);
        imprime_instruccion_color(aux, i, SI);
        fprintf(f_salida_html, "  <td bgcolor=\"%s\">%s</td>\n", BG_VAL2, aux);
        fprintf(f_salida_html, "</tr>\n");
    } /* endfor */
    fprintf(f_salida_html, "</table>\n\n");
}

/***************************************************************
 *
 * Func: imprime_etapas
 *
 * Desc: Imprime las etapas del procesador
 *
 **************************************************************/

static void imprime_etapas() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short i = 0;
    char aux[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    /*** Muestra etapas ***/

    fprintf(f_salida_html, "<!-- Etapas -->\n");
    fprintf(f_salida_html, "<table>\n");

    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);
    fprintf(f_salida_html, "<td width=%d bgcolor=\"white\">&nbsp;</td>\n", NOMBRE_ETAPAS_W);
    fprintf(f_salida_html, "<td width=%d align=center>--/IF</td>\n", ETAPAS_W);
    fprintf(f_salida_html, "<td width=%d align=center>IF/ID</td>\n", ETAPAS_W);
    fprintf(f_salida_html, "<td width=%d align=center>ID/EX</td>\n", ETAPAS_W);
    fprintf(f_salida_html, "<td width=%d align=center>EX/MEM</td>\n", ETAPAS_W);
    fprintf(f_salida_html, "<td width=%d align=center>MEM/WB</td>\n", ETAPAS_W);
    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"%s\">Instruc.</td>\n", BG_OPER);

    imprime_instruccion_color(aux, PC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    imprime_instruccion_color(aux, IF_ID.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    //imprime_instruccion_color(aux, ID_EX.iPC, SI);
    imprime_instruccion_color(aux, FP_LS_LAT == 2 && ID_EX.iPC == -1 ? ID_LS.iPC : ID_EX.iPC, SI); // Si FP_LS tiene latencia 2, comparte EX ME WB en presentacion
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    //imprime_instruccion_color(aux, EX_MEM.iPC, SI);
    imprime_instruccion_color(aux, FP_LS_LAT == 2 && EX_MEM.iPC == -1 ? LS[2].iPC : EX_MEM.iPC, SI); // Si FP_LS tiene latencia 2, comparte EX ME WB en presentacion
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    //imprime_instruccion_color(aux, MEM_WB.iPC, SI);
    imprime_instruccion_color(aux, FP_LS_LAT == 2 && MEM_WB.iPC == -1 ? LS_WB.iPC : MEM_WB.iPC, SI); // Si FP_LS tiene latencia 2, comparte EX ME WB en presentacion
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    fprintf(f_salida_html, "</tr>\n");


    /* Transfer FP */
    fprintf(f_salida_html, "<!-- Transfer -->\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td width=%d align=center>ID/WB</td>\n", ETAPAS_W);

    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"%s\">Xfer (FP)</td>\n", BG_OPER);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");

    imprime_instruccion_color(aux, ID_WB.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);

    fprintf(f_salida_html, "</tr>\n");


    /* Load/Store FP */
    if (FP_LS_LAT > 2) { // Si FP_LS tiene latencia 2, comparte EX ME WB en presentacion
        fprintf(f_salida_html, "<!-- LOAD/STORE -->\n");
        fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);

        fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
        fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
        fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
        for (i = 1; i <= FP_LS_LAT; i++) {
            fprintf(f_salida_html, "<td width=%d align=center>L/S%d</td>\n", ETAPAS_W, i);
        }
        fprintf(f_salida_html, "<td width=%d align=center>FP/WB</td>\n", ETAPAS_W);
        fprintf(f_salida_html, "</tr>\n");

        fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
        fprintf(f_salida_html, "<td bgcolor=\"%s\">L/S (FP)</td>\n", BG_OPER);

        fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
        fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");

        imprime_instruccion_color(aux, ID_LS.iPC, SI);
        fprintf(f_salida_html, "<td>%s</td>\n", aux);
        for (i = 2; i <= FP_LS_LAT; i++) {
            imprime_instruccion_color(aux, LS[i].iPC, SI);
            fprintf(f_salida_html, "<td>%s</td>\n", aux);
        }

        //if inst_load_fp(LS_WB.IR.codop) {
        imprime_instruccion_color(aux, LS_WB.iPC, SI);
        //} else {
        //    imprime_instruccion_color(aux, -1, SI);
        //}
        fprintf(f_salida_html, "<td>%s</td>\n", aux);

        fprintf(f_salida_html, "</tr>\n");
    } /* endif (FP_LS_LAT > 2) */

    /* Sumador FP */
    fprintf(f_salida_html, "<!-- FP ADD-->\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 1; i <= FP_ADD_LAT; i++) {
        fprintf(f_salida_html, "<td width=%d align=center>A%d</td>\n", ETAPAS_W, i);
    }
    fprintf(f_salida_html, "<td width=%d align=center>FP/WB</td>\n", ETAPAS_W);

    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"%s\">Sum (FP)</td>\n", BG_OPER);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");

    imprime_instruccion_color(aux, ID_A.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    for (i = 2; i <= FP_ADD_LAT; i++) {
        imprime_instruccion_color(aux, A[i].iPC, SI);
        fprintf(f_salida_html, "<td>%s</td>\n", aux);
    }

    imprime_instruccion_color(aux, ADD_WB.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);

    fprintf(f_salida_html, "</tr>\n");

    /* Comparacion FP */

    fprintf(f_salida_html, "<!-- FP CMP -->\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 1; i <= FP_CMP_LAT; i++) {
        fprintf(f_salida_html, "<td width=%d align=center>C%d</td>\n", ETAPAS_W, i);
    }
    fprintf(f_salida_html, "<td width=%d align=center>CMP/WB</td>\n", ETAPAS_W);

    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"%s\">Cmp (FP)</td>\n", BG_OPER);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");

    imprime_instruccion_color(aux, ID_C.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    for (i = 2; i <= FP_CMP_LAT; i++) {
        imprime_instruccion_color(aux, C[i].iPC, SI);
        fprintf(f_salida_html, "<td>%s</td>\n", aux);
    }

    imprime_instruccion_color(aux, CMP_WB.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);

    fprintf(f_salida_html, "</tr>\n");

    /* Multiplicador FP */

    fprintf(f_salida_html, "<!-- FP MUL -->\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_STAGE);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    for (i = 1; i <= FP_MUL_LAT; i++) {
        fprintf(f_salida_html, "<td width=%d align=center>M%d</td>\n", ETAPAS_W, i);
    }
    fprintf(f_salida_html, "<td width=%d align=center>FP/WB</td>\n", ETAPAS_W);

    fprintf(f_salida_html, "</tr>\n");

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"%s\">Mul (FP)</td>\n", BG_OPER);

    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");
    fprintf(f_salida_html, "<td bgcolor=\"white\">&nbsp;</td>\n");

    imprime_instruccion_color(aux, ID_M.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);
    for (i = 2; i <= FP_MUL_LAT; i++) {

        imprime_instruccion_color(aux, M[i].iPC, SI);
        fprintf(f_salida_html, "<td>%s</td>\n", aux);
    }

    imprime_instruccion_color(aux, MUL_WB.iPC, SI);
    fprintf(f_salida_html, "<td>%s</td>\n", aux);

    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "</table>\n\n");
    fprintf(f_salida_html, "<br>\n");

} /* end imprime_etapas */

/***************************************************************
 *
 * Func: imprime_senyales
 *
 * Desc: Imprime las señales activas del procesador
 *
 **************************************************************/

static void imprime_senyales() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/


    /*************************************/
    /*  Function body                    */
    /*************************************/


    /*** Señales ***/

    //    fprintf(f_salida_html, "<tr bgcolor=\"white\"><td colspan=\"6\">&nbsp;</td></tr>\n");

    fprintf(f_salida_html, "<!-- Señales activas -->\n");
    fprintf(f_salida_html, "<table>\n");

    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_VAL); // Contenido de la tabla
    fprintf(f_salida_html, "<td width=%d bgcolor=\"%s\">Señales</td>\n", NOMBRE_ETAPAS_W, BG_SIG);

    muestra_senyal(IFstall, "IFstall");
    muestra_senyal(IDstall, "IDstall");
    muestra_senyal(RAW, "RAW");
    muestra_senyal(WAW, "WAW");
    muestra_senyal(estructural, "WBconflict");
    muestra_senyal(0, " ");


    fprintf(f_salida_html, "</tr>\n");

    /*** Mas Señales ***/

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"#ffffff\">&nbsp;</td>\n");

    muestra_senyal(IFnop, "IFnop");
    muestra_senyal(IDnop, "IDnop");
    muestra_senyal(EXnop, "EXnop");
    muestra_senyal(0, " ");
    muestra_senyal(0, " ");
    muestra_senyal(0, " ");


    fprintf(f_salida_html, "</tr>\n");


    /*** Mas Señales ***/

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"#ffffff\">&nbsp;</td>\n");

    muestra_senyal((MEMaIDcomp_i || MEMaIDcomp_s), "MEMaIDcomp");
    muestra_senyal((MEMaEXalu_i || MEMaEXalu_s), "MEMaEX");
    muestra_senyal((MEMaEXcomp_i || MEMaEXcomp_s), "MEMaEXcomp");
    muestra_senyal(MEMaLS1_s, FP_LS_LAT == 2 ? "MEMaEXs" : "MEMaLS1s");
    muestra_senyal(0, " ");
    muestra_senyal(0, " ");

    fprintf(f_salida_html, "</tr>\n");

    /*** Mas Señales ***/

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"#ffffff\">&nbsp;</td>\n");

    muestra_senyal(0, " ");
    muestra_senyal((WBaEXalu_i || WBaEXalu_s), "WBaEX");
    muestra_senyal((WBaEXcomp_i || WBaEXcomp_s), "WBaEXcomp");
    muestra_senyal(WBaEXmem, "WBaEXmem ");
    muestra_senyal(WBaMEM, "WBaMEM ");
    muestra_senyal(WBaLS1_s, FP_LS_LAT == 2 ? "WBaEX" : "WBaLS1");

    fprintf(f_salida_html, "</tr>\n");

    /*** Mas Señales ***/

    fprintf(f_salida_html, "<tr bgcolor=%s>\n", BG_VAL);
    fprintf(f_salida_html, "<td bgcolor=\"#ffffff\">&nbsp;</td>\n");

    muestra_senyal(FP_WBaLS1_i, FP_LS_LAT == 2 ? "WBFaEXmem" : "WBFaLS1i");
    muestra_senyal(FP_WBaLS2_i, FP_LS_LAT == 2 ? "WBFaMEM" : "WBFaLS2i");
    muestra_senyal((FP_WBaA1_s || FP_WBaA1_i), "WBFaA1");
    muestra_senyal((FP_WBaM1_s || FP_WBaM1_i), "WBFaM1");
    muestra_senyal((FP_WBaC1_s || FP_WBaC1_i), "WBFaC1");
    muestra_senyal(FP_WBaEXcomp, "WBfaEXcomp");
    //    muestra_senyal(FP_WBaIDcomp, "WBfaIDcomp");


    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "</table>\n\n");

} /* end imprime_senyales */

/***************************************************************
 *
 * Func: imprime_dit
 *
 * Desc: Imprime el cronograma de ejecución
 *
 **************************************************************/

static void imprime_dit(int num_instrucciones, char* archivo) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    short x, y;

    /*************************************/
    /*  Function body                    */
    /*************************************/

    fprintf(f_salida_html, "<!-- Diagrama instrucciones - tiempo  -->\n");
    fprintf(f_salida_html, "<table>\n");

    /*** Cronograma de ejecución ***/

    fprintf(f_salida_html, "  <!-- T-Cronograma -->\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\" align=\"center\">\n", BG_HEADER);
    fprintf(f_salida_html, "  <td width=\"%d\">PC</td>\n", PC_CRONO_W);
    fprintf(f_salida_html, "  <td width=\"%d\">Instrucci&oacute;n</td>\n", INSTR_CRONO_W);
    fprintf(f_salida_html, "  <!-- T-Linea -->\n");
    fprintf(f_salida_html, "  <!-- T-PC %s -->\n", "PC");
    fprintf(f_salida_html, "  <!-- T-Instr %s -->\n", "Instruccion");
    for (x = 0; x < MAX(MAX_CICLOS_INIC, maxima_columna_visible); x++) {
        fprintf(f_salida_html, "  <td width=\"%d\"><a href=\"%s%03u.html\">%u</a></td>\n",
                FASE_CRONO_W, archivo, primer_ciclo + x, primer_ciclo + x);
        fprintf(f_salida_html, "  <!-- T-Ciclo-H %u %u -->\n", x + primer_ciclo, x + primer_ciclo);
    }
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "  <!-- T-Fin-Linea -->\n");

    for (y = 0; y < num_instrucciones; y++) {
        if (y % 2 == 0)
            fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_VAL1);
        else
            fprintf(f_salida_html, "<tr bgcolor=\"%s\" align=\"center\">\n", BG_VAL2);
        fprintf(f_salida_html, "  <!-- T-Linea -->\n");

        if (!strlen(crono[y].inst)) {
            if (y % 2 == 0) {
                fprintf(f_salida_html,
                        "  <td bgcolor=\"%s\">&nbsp;</td>\n", BG_DIT1);
                fprintf(f_salida_html,
                        "  <td bgcolor=\"%s\">&nbsp;</td>\n", BG_DIT1);
            } else {
                fprintf(f_salida_html,
                        "  <td bgcolor=\"%s\">&nbsp;</td>\n", BG_DIT2);
                fprintf(f_salida_html,
                        "  <td bgcolor=\"%s\">&nbsp;</td>\n", BG_DIT2);
            }
        } else {
            if (y % 2 == 0) {
                fprintf(f_salida_html,
                        "  <td align=\"left\" bgcolor=\"%s\">%s</td>\n", BG_DIT1,
                        crono[y].PC);
                fprintf(f_salida_html,
                        "  <td align=\"left\" bgcolor=\"%s\">%s</td>\n", BG_DIT1,
                        crono[y].inst);
            } else {
                fprintf(f_salida_html,
                        "  <td align=\"left\" bgcolor=\"%s\">%s</td>\n", BG_DIT2,
                        crono[y].PC);
                fprintf(f_salida_html,
                        "  <td align=\"left\" bgcolor=\"%s\">%s</td>\n", BG_DIT2,
                        crono[y].inst);
            }
            fprintf(f_salida_html, "  <!-- T-PC %s -->\n", crono[y].PC);
            fprintf(f_salida_html, "  <!-- T-Instr  %s -->\n", crono[y].inst);
        }

        for (x = 0; x < MAX(MAX_CICLOS_INIC, maxima_columna_visible); x++) {
            fprintf(f_salida_html, "  <td align=\"center\">");

            if (strlen(crono[y].fase[x])) {
                fprintf(f_salida_html, "%s", crono[y].fase[x]);
                fprintf(f_salida_html, "</td>\n");
                fprintf(f_salida_html, "  <!-- T-Ciclo-D %d %s -->\n", x + 1, crono[y].fase[x]);
            } else {
                fprintf(f_salida_html, "&nbsp;");
                fprintf(f_salida_html, "</td>\n");
                fprintf(f_salida_html, "  <!-- T-Ciclo-D %d %s -->\n", x + 1, "BLANCO");
            }
            fprintf(f_salida_html, "</td>\n");
        } /* endfor */
        fprintf(f_salida_html, "</tr>\n");
        fprintf(f_salida_html, "  <!-- T-Fin-Linea -->\n");
    } /* endfor */

    fprintf(f_salida_html, "</table><br>\n\n");
    fprintf(f_salida_html, "  <!-- T-Fin-Cronograma -->\n");
} /* end imprime_dit */




/***** Funciones Exportables **********************************/

/***************************************************************
 *
 * Func: init_instruc
 *
 * Description:
 *   Inicializa una instrucción en el cronograma
 *
 **************************************************************/

void init_instruc(
        word PC,
        ciclo_t orden
        ) {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    int o;
    char aux[128];
    char dir[128];

    /*************************************/
    /*  Function body                    */

    /*************************************/

    if (orden == primera_instruccion + MAX_INSTRUC)
        desplazamiento_vertical();

    o = orden - primera_instruccion;

    te_etiqueta(dir, PC);
    imprime_instruccion_color(aux, PC, SI);
    // sprintf(crono[o].inst, "%s: %s", PC, aux);
    sprintf(crono[o].PC, "%s", dir);
    sprintf(crono[o].inst, "%s", aux);

} /* end init_instruc */

/***************************************************************
 *
 * Func: actualiza_crono
 *
 * Description:
 *   Actualiza el cronograma con el estado de las fases
 *
 **************************************************************/

void actualiza_crono() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    int i;
    char aux[128];
    char fase[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    sprintf(fase, "%s", "IF");
    if (IFstall) {
        //        sprintf(fase, "<i>%s</i>", "if");
        sprintf(fase, "%s", "if");
    }
    if (IFnop) {
        sprintf(aux, "%s", fase);
        sprintf(fase, "<del>%s</del>", aux);
        //sprintf(fase,"<b>%s</b>",aux);
        //sprintf(fase,"<font color=\"red\">%s</font>", aux);
        //sprintf(fase, "%s", aux);
    }
    muestra_fase(fase, orden);

    if (IF_ID.orden != -1) {
        sprintf(fase, "%s", "ID");
        if (IDstall) {
            //            sprintf(fase, "<i>%s</i>", "id");
            sprintf(fase, "%s", "id");
        }
        if (IDnop) {
            sprintf(aux, "%s", fase);
            sprintf(fase, "<del>%s</del>", aux);
            //sprintf(fase,"<b>%s</b>",aux);
            //sprintf(fase,"<font color=\"red\">%s</font>", aux);
            //sprintf(fase, "%s", aux);
        }
        muestra_fase(fase, IF_ID.orden);
    }

    if (ID_EX.orden != -1) {
         sprintf(fase, "%s", "EX");
        if (EXnop) {
            sprintf(aux, "%s", fase);
            sprintf(fase, "<del>%s</del>", aux);
            //sprintf(fase,"<b>%s</b>",aux);
            //sprintf(fase,"<font color=\"red\">%s</font>", aux);
            //sprintf(fase, "%s", aux);
        }
        muestra_fase(fase, ID_EX.orden);
    }

    if (EX_MEM.orden != -1)
        muestra_fase("ME", EX_MEM.orden);

    if (MEM_WB.orden != -1)
        muestra_fase("WB", MEM_WB.orden);

    if (ID_LS.orden != -1)
        // muestra_fase("L1", ID_LS.orden);
        muestra_fase(FP_LS_LAT == 2 ? "EX" : "L1", ID_LS.orden); // Si FP_LS tiene latencia 2, comparte EX ME WB en presentacion

    for (i = 2; i <= FP_LS_LAT; i++) {
        if (LS[i].orden != -1) {
            sprintf(aux, "L%d", i);
            //muestra_fase(aux, LS[i].orden);
            muestra_fase(FP_LS_LAT == 2 && i == 2 ? "ME" : aux, LS[i].orden); // Si FP_LS tiene latencia 2, comparte EX ME WB en presentacion
        }
    }

    if (ID_A.orden != -1)
        muestra_fase("A1", ID_A.orden);

    for (i = 2; i <= FP_ADD_LAT; i++) {
        if (A[i].orden != -1) {
            sprintf(aux, "A%d", i);
            muestra_fase(aux, A[i].orden);
        }
    }

    if (ID_C.orden != -1)
        muestra_fase("C1", ID_C.orden);

    for (i = 2; i <= FP_CMP_LAT; i++) {
        if (C[i].orden != -1) {
            sprintf(aux, "C%d", i);
            muestra_fase(aux, C[i].orden);
        }
    }


    if (ID_M.orden != -1)
        muestra_fase("M1", ID_M.orden);

    for (i = 2; i <= FP_MUL_LAT; i++) {
        if (M[i].orden != -1) {
            sprintf(aux, "M%d", i);
            muestra_fase(aux, M[i].orden);
        }
    }

    if (FP_WB.orden != -1)
        muestra_fase("WB", FP_WB.orden);

    if (CMP_WB.orden != -1)
        muestra_fase("WB", CMP_WB.orden);


} /* end actualiza_crono */

/***************************************************************
 *
 * Func: imprime_crono
 *
 * Desc: Imprime el cronograma de ejecucin
 *
 **************************************************************/

void imprime_crono() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[128];


    /*************************************/
    /*  Function body                    */
    /*************************************/

    sprintf(aux, "crono%03u.html", Ciclo);
    f_salida_html = fopen(aux, "w");
    if (f_salida_html == NULL) {

        fprintf(stderr, "Error creando %s\n", aux);
        exit(1);
    } /* endif */

    /*** HTML INTRO ***/

    imprime_intro("Cronograma");

    /* Barra de navegación */
    imprime_nav2("crono");
    fprintf(f_salida_html, "<br><br>\n");


    /*** Cronograma de ejecucion ***/

    imprime_dit(MAX_INSTRUC, "crono");
    fprintf(f_salida_html, "<br>\n");

    fprintf(f_salida_html, "<br>\n");


    /*** Final ***/
    imprime_footer();


    fclose(f_salida_html);

} /* end imprime_crono */

/***************************************************************
 *
 * Func: imprime_css
 *
 * Desc: Imprime todo
 *
 **************************************************************/

void imprime_css() {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[MAX_TEXTO_CONSOLA];


    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Para pantallas pequeñas, caben 10 instrucciones en el dit */

    sprintf(aux, "result%03u.html", Ciclo);
    if (html_merge == SI) {
        fprintf(f_salida_html, "'%s':`", aux);
    } else {
        f_salida_html = fopen(aux, "w");
        if (f_salida_html == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */


    /*** HTML INTRO ***/

    imprime_w3css("Resultados");

    /* Barra de navegación */
    imprime_nav2_css("result");
    fprintf(f_salida_html, "<br><br><br>\n");

    /** CSS - responsive general */
    fprintf(f_salida_html, "<div class=\"w3-responsive\">\n");

    fprintf(f_salida_html, "<!-- Cronograma y señales -->\n");

    /** CSS - bloque izquierdo */
    fprintf(f_salida_html, "<div class=\"w3-container w3-cell\">\n");

    /** CSS - Bloque cronograma **/
    fprintf(f_salida_html, "<div class=\"w3-container\">\n");
    //    fprintf(f_salida_html, "<div class=\"w3-col w3-container\" style=\"width:60%%\">\n");

    /*** Cronograma de ejecución ***/

    if (SHOW_DIT) imprime_dit(MAX_INSTRUC, "result");
    fprintf(f_salida_html, "</div>\n");
    fprintf(f_salida_html, "<br>\n");



    /*** CSS - Bloque Datapath ***/
    fprintf(f_salida_html, "<div class=\"w3-container w3-raw\">\n");

    if ((!hay_float) && (!hay_double)) {
        if (SHOW_DATAPATH)
            dibuja_datapath();
    } else {
        if (SHOW_DATAPATH) {
            imprime_etapas();
            imprime_senyales();
        }
    }
    fprintf(f_salida_html, "</div>\n");
    fprintf(f_salida_html, "<br>\n");

    /*** CSS - Consola */
    if (strlen(CONSOLA) > 0) {
        if (SHOW_CONSOLE) {
            fprintf(f_salida_html, "<div class=\"w3-container w3-raw w3-black w3-text-green\">\n");
            fprintf(f_salida_html, "<!-- CONSOLA -->\n");
            fprintf(f_salida_html, "<br>\n");
            fprintf(f_salida_html, "CONSOLA:\n");
            fprintf(f_salida_html, "<br>\n");
            fprintf(f_salida_html, "%s\n\n", CONSOLA);
            fprintf(f_salida_html, "<br><br>\n");
            fprintf(f_salida_html, "</div>\n");
            fprintf(f_salida_html, "<br>\n");
        }
    }

    /* CSS - Log */
    if (strlen(LOG) > 0) {
        if (SHOW_LOG) {
            fprintf(f_salida_html, "<div class=\"w3-raw w3-container w3-light-grey\">\n");
            fprintf(f_salida_html, "<!-- LOG -->\n");
            fprintf(f_salida_html, "<br>\n");
            fprintf(f_salida_html, "LOG:\n<br>");
            fprintf(f_salida_html, "%s\n<br>", LOG);
            fprintf(f_salida_html, "<br>\n");
            fprintf(f_salida_html, "</div>\n\n");
            fprintf(f_salida_html, "<br>\n");
        }
    }

    /*** CSS - Fin bloque izquierdo */
    fprintf(f_salida_html, "</div>\n");
    fprintf(f_salida_html, "<!-- Fin Cronograma y señales -->\n\n");


    /*** CSS - Bloque derecho */

    fprintf(f_salida_html, "<!-- Resto: Reg Int, Reg FP, Memoria -->\n");
    fprintf(f_salida_html, "<div class=\"w3-container w3-cell\">\n");


    /*** Registros int ***/
    //fprintf(f_salida_html, "<div class=\"w3-container w3-cell \">\n");
    fprintf(f_salida_html, "<div class=\"w3-cell\">\n");
    //    fprintf(f_salida_html, "<div class=\"w3-col w3-container\" style=\"width:12%%\">\n");
    if (SHOW_REG)
        imprime_int_regs_col();
    fprintf(f_salida_html, "</div>\n");

    if ((hay_float) || (hay_double)) {
        /*** Registros fp ***/
        //fprintf(f_salida_html, "<div class=\"w3-container w3-cell \">\n");

        fprintf(f_salida_html, "<div class=\"w3-cell\">\n");
        //    fprintf(f_salida_html, "<div class=\"w3-col w3-container\" style=\"width:12%%\">\n");
        if (SHOW_REG)
            imprime_fp_regs_col();
        fprintf(f_salida_html, "</div>\n");
    }

    /*** Memoria ***/
    //   fprintf(f_salida_html, "<div class=\"w3-container w3-cell \">\n");
    fprintf(f_salida_html, "<div class=\"w3-cell\">\n");
    //    fprintf(f_salida_html, "<div class=\"w3-col w3-container\" style=\"width:12%%\">\n");
    if (SHOW_MEM)
        imprime_Dmem_col();
    fprintf(f_salida_html, "</div>\n\n");

    /** CSS - Fin bloque derecho */
    fprintf(f_salida_html, "</div>\n");
    fprintf(f_salida_html, "<!-- Fin Resto -->\n\n");
    fprintf(f_salida_html, "</div>\n");

    /** CSS - Fin */
    //fprintf(f_salida_html, "<br>\n");
    fprintf(f_salida_html, "</div>\n");

    /** CSS - Fin responsive */
    //fprintf(f_salida_html, "<br>\n");
    fprintf(f_salida_html, "</div>\n");


    /*** Final ***/
    imprime_footer_css();
    fprintf(f_salida_html, "</div>\n");

    if (html_merge == SI) {
        fputs("`,", f_salida_html);
    } else {

        fclose(f_salida_html);
    } /* endif */


} /* end imprime_css */

/***************************************************************
 *
 * Func: imprime_inicio
 *
 * Desc: Imprime la configuracion
 *
 **************************************************************/

void imprime_inicio() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    sprintf(aux, "index.html");
    f_salida_html = fopen(aux, "w");
    if (f_salida_html == NULL) {

        fprintf(stderr, "Error creando %s\n", aux);
        exit(1);
    } /* endif */

    /*** HTML INTRO ***/

    imprime_intro("Estado inicial");

    /*** Navegación ***/

    imprime_nav1();
    fprintf(f_salida_html, "<br><br>\n");


    /*** Configuración ***/

    imprime_conf();
    fprintf(f_salida_html, "<br>\n");



    /*** Memorias ***/

    fprintf(f_salida_html, "<table>\n");


    fprintf(f_salida_html, "<tr valign=\"top\"><td>\n");

    /*** Contenido de la memoria de datos ***/

    imprime_Dmem_col();

    fprintf(f_salida_html, "</td><td>\n");

    /*** Contenido de la memoria de instrucciones ***/

    imprime_Imem_col();

    fprintf(f_salida_html, "</td></tr>\n");
    fprintf(f_salida_html, "</table><br>\n");


    /*** Final ***/
    imprime_footer();


    fclose(f_salida_html);

} /* end imprime_inicio */

/***************************************************************
 *
 * Func: imprime_inicio_css
 *
 * Desc: Imprime la configuracion
 *
 **************************************************************/

void imprime_inicio_css() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[128];


    /*************************************/
    /*  Function body                    */
    /*************************************/

    /* Borra archivo de consola */
    f_consola = fopen(console_name, "w");
    fclose(f_consola);

    /* Borra archivo de log */
    f_log = fopen(log_name, "w");
    fclose(f_log);

    /* Si solo hay enteros, ajustar dit, dibujar datapath */

    if (html_merge == SI) {
        sprintf(aux, "%s.htm", "index");
    } else {
        sprintf(aux, "index.html");
    } /* endif */

    f_salida_html = fopen(aux, "w");
    if (f_salida_html == NULL) {

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
                f_salida_html);
    } /* endif */

    /*** HTML INTRO ***/

    imprime_w3css("Inicio");

    /* Barra de navegación */
    imprime_nav1_css();
    fprintf(f_salida_html, "<br><br><br>\n");


    /*** Configuración ***/
    fprintf(f_salida_html, "<div class=\"w3-row w3-container\">\n");
    imprime_conf();
    fprintf(f_salida_html, "<br>\n");
    fprintf(f_salida_html, "</div>\n\n");




    /*** Memorias ***/
    fprintf(f_salida_html, "<div class=\"w3-row\">\n");

    /*** Contenido de la memoria de datos ***/
    fprintf(f_salida_html, "<div class=\"w3-cell w3-container\">\n");

    imprime_Dmem_col();

    fprintf(f_salida_html, "</div>\n");

    /*** Contenido de la memoria de instrucciones ***/
    fprintf(f_salida_html, "<div class=\"w3-cell w3-container\">\n");

    imprime_Imem_col();

    fprintf(f_salida_html, "</div>\n");
    fprintf(f_salida_html, "</div>\n");


    /*** Final ***/
    imprime_footer_css();

    if (html_merge == SI) {
        fputs("`,", f_salida_html);
    } else {

        fclose(f_salida_html);
    } /* endif */


} /* end imprime_inicio */

/***************************************************************
 *
 * Func: imprime_final
 *
 * Desc: Imprime la configuracion y resultados finales
 *
 **************************************************************/

void imprime_final() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[128];

    /*************************************/
    /*  Function body                    */
    /*************************************/

    sprintf(aux, "final.html");
    f_salida_html = fopen(aux, "w");
    if (f_salida_html == NULL) {

        fprintf(stderr, "Error creando %s\n", aux);
        exit(1);
    } /* endif */

    /*** HTML INTRO ***/

    imprime_intro("Resultados");

    /*** Navegación ***/

    imprime_nav1();
    fprintf(f_salida_html, "<br><br>\n");

    /*** Resultados de la ejecucion ***/

    fprintf(f_salida_html, "<table>\n");
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "<td colspan=\"5\" bgcolor=\"%s\" align=\"center\">Resultados</td>\n", BG_HEADER);
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_DIR1);
    fprintf(f_salida_html, "  <td>Ciclos&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "  <td>Instrucciones&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "  <td>CPI&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "  <td>Op. CF&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "  <td>Op. CF/Ciclo&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_DIR2);
    fprintf(f_salida_html, "  <td>%ld</td>\n", estat.ciclos);
    fprintf(f_salida_html, "  <td>%ld</td>\n", estat.instrucciones);
    fprintf(f_salida_html, "  <td>%1.2f</td>\n", (float) estat.ciclos / estat.instrucciones);
    fprintf(f_salida_html, "  <td>%ld</td>\n", estat.flops);
    fprintf(f_salida_html, "  <td>%1.2f</td>\n", (float) estat.flops / estat.ciclos);
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "</table><br>\n");


    /*** Configuración ***/

    imprime_conf();
    fprintf(f_salida_html, "<br>\n");


    /*** Bancos de registros ***/

    fprintf(f_salida_html, "<table>\n");

    /* Banco de registros enteros */

    imprime_int_regs();
    fprintf(f_salida_html, "<br>\n");

    /* Banco de registros fp */

    imprime_fp_regs();
    fprintf(f_salida_html, "<br>\n");

    /*** Memorias ***/

    fprintf(f_salida_html, "<table>\n");

    fprintf(f_salida_html, "<tr valign=\"top\"><td>\n");

    /*** Contenido de la memoria de datos ***/

    imprime_Dmem_col();

    fprintf(f_salida_html, "</td><td>\n");

    /*** Contenido de la memoria de instrucciones ***/

    imprime_Imem_col();

    fprintf(f_salida_html, "</td></tr>\n");
    fprintf(f_salida_html, "</table><br>\n");

    /*** Final ***/
    imprime_footer();


    fclose(f_salida_html);

} /* end imprime_final */

/***************************************************************
 *
 * Func: imprime_final_css
 *
 * Desc: Imprime la configuracion y resultados finales
 *
 **************************************************************/

void imprime_final_css() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[128];


    /*************************************/
    /*  Function body                    */
    /*************************************/

    sprintf(aux, "final.html");
    if (html_merge == SI) {
        fprintf(f_salida_html, "'%s':`", aux);
    } else {
        f_salida_html = fopen(aux, "w");
        if (f_salida_html == NULL) {
            fprintf(stderr, "Error creando %s\n", aux);
            exit(1);
        } /* endif */
    } /* endif */


    /*** HTML INTRO ***/

    imprime_w3css("Final");

    /* Barra de navegación */
    imprime_nav1_css();
    fprintf(f_salida_html, "<br><br><br>\n");


    fprintf(f_salida_html, "<div class=\"w3-row w3-container\">\n");

    /*** Resultados de la ejecucion ***/

    fprintf(f_salida_html, "<table>\n");
    fprintf(f_salida_html, "<tr>\n");
    fprintf(f_salida_html, "<td colspan=\"5\" bgcolor=\"%s\" align=\"center\">Resultados</td>\n", BG_HEADER);
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_DIR1);
    fprintf(f_salida_html, "  <td>Ciclos&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "  <td>Instrucciones&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "  <td>CPI&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "  <td>Op. CF&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "  <td>Op. CF/Ciclo&nbsp;&nbsp;</td>\n");
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "<tr bgcolor=\"%s\">\n", BG_DIR2);
    fprintf(f_salida_html, "  <td>%ld</td>\n", estat.ciclos);
    fprintf(f_salida_html, "  <td>%ld</td>\n", estat.instrucciones);
    fprintf(f_salida_html, "  <td>%1.2f</td>\n", (float) estat.ciclos / estat.instrucciones);
    fprintf(f_salida_html, "  <td>%ld</td>\n", estat.flops);
    fprintf(f_salida_html, "  <td>%1.2f</td>\n", (float) estat.flops / estat.ciclos);
    fprintf(f_salida_html, "</tr>\n");
    fprintf(f_salida_html, "</table><br>\n");
    fprintf(f_salida_html, "</div>\n\n");


    /*** Configuración ***/
    fprintf(f_salida_html, "<div class=\"w3-row w3-container\">\n");
    imprime_conf();
    fprintf(f_salida_html, "<br>\n");
    fprintf(f_salida_html, "</div>\n\n");



    fprintf(f_salida_html, "<div class=\"w3-row w3-container\">\n");


    /* Banco de registros enteros */

    fprintf(f_salida_html, "<div class=\"w3-container w3-cell\">\n");
    imprime_int_regs_col();
    fprintf(f_salida_html, "</div>\n\n");

    /* Banco de registros fp */

    fprintf(f_salida_html, "<div class=\"w3-container w3-cell\">\n");
    imprime_fp_regs_col();
    fprintf(f_salida_html, "</div>\n\n");


    /*** Contenido de la memoria de datos ***/

    fprintf(f_salida_html, "<div class=\"w3-container w3-cell\">\n");
    imprime_Dmem_col();
    fprintf(f_salida_html, "</div>\n\n");


    /*** Contenido de la memoria de instrucciones ***/

    fprintf(f_salida_html, "<div class=\"w3-container w3-cell\">\n");
    imprime_Imem_col();
    fprintf(f_salida_html, "</div>\n\n");

    fprintf(f_salida_html, "</div>\n\n");
    fprintf(f_salida_html, "<br>\n");

    /* Consola */

    if (SHOW_CONSOLE) {
        fprintf(f_salida_html, "<div class=\"w3-raw w3-container w3-black w3-text-green\">\n");
        fprintf(f_salida_html, "<br>\n");
        fprintf(f_salida_html, "CONSOLA:\n<br>");
        f_consola = fopen(console_name, "r");
        while (fgets(aux, MAX_TEXTO_CONSOLA, f_consola) != NULL) {
            fprintf(f_salida_html, "%s<br>\n", aux);
        }
        fclose(f_consola);
        fprintf(f_salida_html, "\n<br>\n");

        fprintf(f_salida_html, "</div>\n\n");
        fprintf(f_salida_html, "<br>\n");
    }

    /*** Final ***/
    imprime_footer_css();

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
                f_salida_html);
    } /* endif */

    fclose(f_salida_html);

} /* end imprime_final */
