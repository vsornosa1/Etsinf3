%{
    /**************************************************************************
     *
     * Departamento de Inform·tica de Sistema y Computadores (DISCA)
     * Universidad Politecnica de Valencia.
     *
     * Author: Sergio Sáez (ssaez@disca.upv.es)
     *             Pedro López (plopez@disca.upv.es)
     *
     * File: input.yacc.c
     *
     * Date: Wed Mar 10 20:05:25 GMT 1999
     *
     * Description:
     *    Contiene la descripción sintáctica del lenguaje ensamblador
     *
     * This program is free software; you can redistribute it and/or modify
     * it under the terms of the GNU General Public License as published by
     * the Free Software Foundation; either version 2 of the License, or
     * (at your option) any later version.
     *
     * This program is distributed in the hope that it will be useful,
     * but WITHOUT ANY WARRANTY; without even the implied warranty of
     * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     * GNU General Public License for more details.
     *
     * You should have received a copy of the GNU General Public License
     * along with this program; if not, write to the Free Software
     * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
     *
     *************************************************************************/

    /***** Includes ***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

#include "main.h"
#include "tipos.h"
#include "instrucciones.h"
#include "memoria.h"
#include "registros.h"
#include "enlazador.h"

    /*** Declaraciones externas */

    extern FILE * yyin;
    extern int yyerror(char *);
    extern void free(void*);
    extern int yylex();

    void nueva_instruccion(codop_t codop, formato_t tipo,
    			   int Rdestino, int Rfuente1, int Rfuente2,
    			   half inmediato, char * etiqueta);

    /*** Variables globales */

    char mensaje[256];
    long linenumber = 1;
    char desplazamiento[256];
    int hay_valor=0;
    int hay_etiq=0;

    tipo_t tipo = t_dword;

    dword pos[num_regiones];
    region_t region = r_invalid;
    mapa_memoria_t * mm = NULL;
    tabla_simbolos_t * ts = NULL;

    int idx_ireg = 1;
    int idx_fpreg = 0;
    int idx_btb = 0;

    boolean no_ireg = NO;
    boolean no_fpreg = NO;
    boolean no_btb = NO;

    int i = 0;
    instruccion_t instruccion;
    valor_t valor;
    valor_t reg_fp;

    int pending_label = 0;
    region_t pending_region = r_invalid;
    char etiqueta[256];

    /*****************************************************************************/

    %}

%union {
    double fvalue;
    int64_t ivalue;
    char cvalue[128];
}

%token <ivalue> NL
%token <ivalue> T_INT_LIT
%token <fvalue> T_FP_LIT
%token <cvalue> T_ALPHANUM
%token <cvalue> T_STRING
%token <ivalue> REG_INT REG_FP
%token <ivalue> DE_DATA DE_TEXT DE_SPACE DE_GLOBL
%token <ivalue> DE_IREG DE_FREG DE_ASCIIZ DE_BTB
%token <ivalue> DE_BYTE DE_HALF DE_WORD DE_DWORD
%token <ivalue> DE_FLOAT DE_DOUBLE
%token <ivalue> FORM_INM FORM_INM_L FORM_INM_S FORM_INM_HI FORM_INM_LO FORM_INM_B FORM_INM_B_FI FORM_INM_B_FFI FORM_INM_T
%token <ivalue> INST_NOP INST_RET INST_SYSCALL
%token <ivalue> FORM_REG FORM_REG_FF
%token <ivalue> FORM_INM_FP_L FORM_INM_FP_S
%token <ivalue> FORM_FP_REG FORM_FP_REG_FF FORM_FP_REG_DF
%token <ivalue> FORM_REG_R_FP FORM_REG_FP_R
%token <ivalue> FORM_J FORM_J_F
%token <cvalue> LABEL

%token <ivalue> PINST_LA PINST_LI
%token <ivalue> M_LO M_HI M_DISP

%type <ivalue> r_inmediato_lo r_inmediato_hi r_inmediato_disp r_posicion r_offset

%start r_inicio

/*** Seccion de reglas *******************************************************/

%%

r_inicio : LABEL
{
    if (es_region_datos(region)) {
    	if (pending_label) {
            ts_inserta_etiqueta(ts, etiqueta, pos[pending_region], pending_region);
            pending_label= 0;
        }
        // Las etiquetas en zonas de datos se quedan diferidas por si la dirección se tiene que alinear
        pending_label = 1;
        pending_region = region;
        sprintf (etiqueta, "%s", $1);
    } else if (es_region_codigo(region)) {
        // TODO revisar si esto es necesario
	if (pending_label) {
           ts_inserta_etiqueta(ts, etiqueta, pos[pending_region], pending_region);
           pending_label= 0;
        }
        ts_inserta_etiqueta(ts, $1, pos[region], region);
        break;
    } else {
        yyerror("Etiqueta indefinida");
    } /* endif */}
r_linea_ensamblador
{}

| r_linea_ensamblador
{}

|
{
}
;

r_linea_ensamblador:
r_directiva NL
{}
r_inicio
{}

| r_instruccion NL
{}
r_inicio
{}

| NL r_inicio
{}

| r_directiva
{}

| r_instruccion
{}
;

r_directiva:
DE_DATA
{
    region = r_data;
}

| DE_TEXT
{
    region = r_text;
}

| DE_GLOBL T_ALPHANUM
{
    ts_inserta_simbolo(ts, $2);
}

| DE_BYTE
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
    tipo = t_byte;
}
r_lista_enteros
{}

| DE_HALF
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
    tipo = t_half;
}
r_lista_enteros
{}

| DE_WORD
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
    tipo = t_word;
}
r_lista_enteros
{}

| DE_DWORD
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
    tipo = t_dword;
}
r_lista_enteros
{}

| DE_FLOAT
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
    tipo = t_float;
}
r_lista_reales
{}

| DE_DOUBLE
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
    tipo = t_double;
}
r_lista_reales
{}

| DE_SPACE T_INT_LIT
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }

    if (pending_label) {
        ts_inserta_etiqueta(ts, etiqueta, pos[pending_region], pending_region);
        pending_label= 0;
    }
    pos[region] += $2;

    if (pos[region] > tam_region[region]) {
    	sprintf(mensaje, "Excedido el tamaño de la memoria: %s", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
}

| DE_ASCIIZ T_STRING
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }

    if (!esta_alineada_dir_byte(pos[region]))
        pos[region] = mem_direccion_alineada(pos[region], sizeof(byte));
    if (pending_label) {
        ts_inserta_etiqueta(ts, etiqueta, pos[pending_region], pending_region);
        pending_label=0;
    }

    for (i = 0; i <= (strlen($2) + 1); i++) // Sumo 1 por el null terminated
    {
        valor.int_d = (byte) $2[i];
        mem_escribe_region_datos(mm, region, pos[region] + i, valor, t_byte, NO);
    }
    pos[region] += sizeof (byte) * strlen($2) + 1; // Sumo 1 por el null terminated

    if (pos[region] > tam_region[region]) {
    	sprintf(mensaje, "Excedido el tamaño de la memoria: %s", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
}

| DE_IREG
{
    if (no_ireg) {
    	sprintf(mensaje, "Los registros enteros ya han sido inicializados en otro módulo");
        yyerror(mensaje);
    }
}
r_valores_enteros
{
  init_ireg = SI;
}

| DE_FREG
{
    if (no_fpreg) {
    	sprintf(mensaje, "Los registros de como flotante ya han sido inicializados en otro módulo");
        yyerror(mensaje);
    }
}
r_valores_fp
{
  init_fpreg = SI;
}

| DE_BTB
{
    if (no_btb) {
    	sprintf(mensaje, "La BTB ya ha sido inicializada en otro módulo");
        yyerror(mensaje);
    }
}
r_valores_btb
{
  init_btb = SI;
}
;

r_lista_enteros:
T_INT_LIT
{
    switch (tipo) {
        case t_byte:
            if (!esta_alineada_dir_byte(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(byte));
            if ($1 > MAX_BYTE)
                yyerror("Literal fuera del rango del tipo 'byte'");
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
            valor.int_d = (byte) $1;
            mem_escribe_region_datos(mm, region, pos[region], valor, t_byte, NO);
            pos[region] += sizeof (byte);
            break;
        case t_half:
            if (!esta_alineada_dir_half(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(half));
            if ($1 > MAX_HALF)
                yyerror("Literal fuera del rango del tipo 'half'");
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
            valor.int_d = (half) $1;
            mem_escribe_region_datos(mm, region, pos[region], valor, t_half, NO);
            pos[region] += sizeof (half);
            break;
        case t_word:
            if (!esta_alineada_dir_word(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(word));
            if ($1 > MAX_WORD)
                yyerror("Literal fuera del rango del tipo 'word'");
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
            valor.int_d = (word) $1;
            mem_escribe_region_datos(mm, region, pos[region], valor, t_word, NO);
            pos[region] += sizeof (word);
            break;
        case t_dword:
            if (!esta_alineada_dir_dword(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(dword));
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
            valor.int_d = (dword) $1;
            mem_escribe_region_datos(mm, region, pos[region], valor, t_dword, NO);
            pos[region] += sizeof (dword);
            break;
        default:
            yyerror("PÁNICO");
            break;
    } /* endswitch */

    if (pos[region] > tam_region[region]) {
    	sprintf(mensaje, "Excedido el tamaño de la memoria: %s", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
}

r_lista_enteros_cont
{}
;

r_lista_enteros_cont:
',' r_lista_enteros
{}

|
{
}
;

r_lista_reales:
T_FP_LIT
{
    switch (tipo) {
        case t_float:
            if (!esta_alineada_dir_float(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(float));
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
            valor.fp_s = (float) $1;
            mem_escribe_region_datos(mm, region, pos[region], valor, t_float, NO);
            pos[region] += sizeof (float);
            break;
        case t_double:
            if (!esta_alineada_dir_double(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(double));
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
            valor.fp_d = (double) $1;
            mem_escribe_region_datos(mm, region, pos[region], valor, t_double, NO);
            pos[region] += sizeof (double);
            break;
        default:
            yyerror("PÁNICO");
            break;
    } /* endswitch */

    if (pos[region] > tam_region[region]) {
    	sprintf(mensaje, "Excedido el tamaño de la memoria: %s", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
}

r_lista_reales_cont
{}

| T_INT_LIT
{
    switch (tipo) {
        case t_float:
            if (!esta_alineada_dir_float(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(float));
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
	    valor.fp_s = (float) $1;
	    mem_escribe_region_datos(mm, region, pos[region], valor, t_float, NO);
            pos[region] += sizeof (float);
            break;
        case t_double:
            if (!esta_alineada_dir_double(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(double));
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
	    valor.fp_d = (double) $1;
	    mem_escribe_region_datos(mm, region, pos[region], valor, t_double, NO);
            pos[region] += sizeof (double);
            break;
        default:
            yyerror("PÁNICO");
            break;
    } /* endswitch */

    if (pos[region] > tam_region[region]) {
    	sprintf(mensaje, "Excedido el tamaño de la memoria: %s", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
}

r_lista_reales_cont
{}
;

r_lista_reales_cont:
',' r_lista_reales
{}

|
{
}
;

r_offset:
'+' T_INT_LIT
{
    $$ = $2;
}

|
'-' T_INT_LIT
{
    $$ = - ($2);
}

|
T_INT_LIT
{
    if ($1 >= 0) {
      yyerror("Syntax error. Se esperaba el operador aritmético '+' o '-' o el separador ','.");
    }
    $$ = $1;
}

|
{
    $$ = 0;
}
;

r_indice_ireg:
REG_INT '='
{
    idx_ireg = $1;
}

| {}
;

r_valores_enteros:
r_indice_ireg T_INT_LIT
{
    if (idx_ireg >= TAM_REGISTROS)
        yyerror("Excedido el numero de registros");
    if (idx_ireg <= 0) {
        sprintf(mensaje, "Índice de registro entero incorrecto (%d)", idx_ireg);
        yyerror(mensaje);
    }

    escribe_int_reg(idx_ireg, (dword) $2, t_dword, NO);
    idx_ireg++;}
r_valores_enteros_cont
{}

|
r_indice_ireg T_ALPHANUM r_offset
{
    if (idx_ireg >= TAM_REGISTROS)
        yyerror("Excedido el numero de registros");
    ts_inserta_direccion(ts, $2, idx_ireg, $3, DIR_REG);
    idx_ireg++;}
r_valores_enteros_cont
{}
;

r_valores_enteros_cont:
',' r_valores_enteros
{}

|
{
}
;

r_indice_fpreg:
REG_FP '='
{
    idx_fpreg = $1;
}

| {}
;

r_valores_fp:
r_indice_fpreg T_FP_LIT
{
    if (idx_fpreg >= TAM_REGISTROS)
        yyerror("Excedido el numero de registros");
    reg_fp.fp_d= (double) $2;
    escribe_fp_reg(idx_fpreg, reg_fp, t_double, NO);
    idx_fpreg++;
    }
r_valores_fp_cont
{}
;

r_valores_fp_cont:
',' r_valores_fp
{}

|
{
}
;

r_valores_btb:
T_ALPHANUM r_offset '&' T_INT_LIT '&' T_ALPHANUM r_offset
{
   if (idx_btb >= TAM_BUFFER_PREDIC)
      yyerror("Excedido el número de entradas en la BTB");

   ts_inserta_direccion(ts, $1, idx_btb, $2, DIR_BTB_PC);
   BTB[idx_btb].orden = idx_btb+1;
   BTB[idx_btb].estado = $4;
   ts_inserta_direccion(ts, $6, idx_btb, $7, DIR_BTB_DEST);

   idx_btb++;
   }
;

r_inmediato_disp:
T_INT_LIT
{
    sprintf(desplazamiento, F_DWORD, $1);
    $$ = $1;}

| T_ALPHANUM
{
    hay_valor=0;
    // Por omisión usa %%lo()
    if (f_macros)
        sprintf(desplazamiento,"%%disp[%s]",$1);
    else
        strcpy(desplazamiento, $1);
    ts_inserta_referencia(ts, $1, region, pos[region], DATA_REL_16, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_HI '(' T_ALPHANUM ')'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%hi[%s]",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], A32_HI, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_LO '(' T_ALPHANUM ')'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%lo[%s]",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], A32_LO, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_DISP '(' T_ALPHANUM ')'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%disp[%s]",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], DATA_REL_16, tr_data);
    $$ = 0; // Referencia por resolver
    }
;

r_inmediato_lo:
T_INT_LIT
{
    sprintf(desplazamiento, F_DWORD, $1);
    $$ = $1;}

| T_ALPHANUM
{
    // Por omisión usa %%lo()
    hay_valor=0;
    if (f_macros)
        sprintf(desplazamiento,"%%lo[%s]",$1);
    else
        strcpy(desplazamiento, $1);
    ts_inserta_referencia(ts, $1, region, pos[region], A32_LO, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_HI '(' T_ALPHANUM ')'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%hi[%s]",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], A32_HI, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_LO '(' T_ALPHANUM ')'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%lo[%s]",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], A32_LO, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_DISP '(' T_ALPHANUM ')'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%disp[%s]",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], DATA_REL_16, tr_data);
    $$ = 0; // Referencia por resolver
    }
;

r_inmediato_hi:
T_INT_LIT
{
    sprintf(desplazamiento, F_DWORD, $1);
    $$ = $1;}

| T_ALPHANUM
{
    // Por omisión usa %%hi()
    hay_valor=0;
    if (f_macros)
        sprintf(desplazamiento,"%%hi[%s]",$1);
    else
        strcpy(desplazamiento, $1);
    ts_inserta_referencia(ts, $1, region, pos[region], A32_HI, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_HI '(' T_ALPHANUM ')'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%hi[%s]",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], A32_HI, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_LO '(' T_ALPHANUM ')'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%lo[%s]",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], A32_LO, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_DISP '(' T_ALPHANUM ')'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%disp[%s]",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], DATA_REL_16, tr_data);
    $$ = 0; // Referencia por resolver
    }
;

r_posicion:
T_INT_LIT
{
    $$ = $1;
}

| T_ALPHANUM
{
    ts_inserta_referencia(ts, $1, region, pos[region], PC_REL_16, tr_code);
    $$ = 0; // Referencia por resolver
}
;

r_instruccion:
/*** Pseudo Instrucciones ***/

/* Aquí figuran las pseudo-instrucciones que tienen un formato diferente a
 *  las instrucciones reales que representan (v.g. li, ret)
 */

// LI
PINST_LI REG_INT ',' T_INT_LIT
{
    if ($4 >= INT32_MIN && $4 < INT32_MAX) {
       sprintf(desplazamiento, F_DWORD, $4);
       nueva_instruccion(OP_ORI, FormatoR, $2, 0, 0, $4, desplazamiento);
    } else {
       dword valor;

       sprintf(desplazamiento,"%%hi[" F_DWORD "]",$4);
       valor = $4 >> 16;
       valor &= UINT16_MAX;
       nueva_instruccion(OP_LUI, FormatoI, $2, 0, 0, valor, desplazamiento);

       sprintf(desplazamiento,"%%lo[" F_DWORD "]",$4);
       valor = $4;
       valor &= UINT16_MAX;
       nueva_instruccion(OP_ORI, FormatoI, $2, $2, 0, valor, desplazamiento);
    }
}

// LA
| PINST_LA REG_INT ',' T_ALPHANUM
{
    sprintf(desplazamiento,"%%hi[%s]",$4);
    ts_inserta_referencia(ts, $4, region, pos[region], A32_HI, tr_data);
    nueva_instruccion(OP_LUI, FormatoI, $2, 0, 0, 0, desplazamiento);

    sprintf(desplazamiento,"%%lo[%s]",$4);
    ts_inserta_referencia(ts, $4, region, pos[region], A32_LO, tr_data);
    nueva_instruccion(OP_ORI, FormatoI, $2, $2, 0, 0, desplazamiento);
}

// RET
| INST_RET
{
    nueva_instruccion(OP_JR, FormatoJ, 0, 31, 0, 0, "");
}

/*** Instrucciones reales ***/
| INST_NOP
{
    nueva_instruccion($1, FormatoR, 0, 0, 0, 0, "");
}

| INST_SYSCALL
{
//    nueva_instruccion(OP_TRAP, FormatoR, 0, 0, 0, EXC_SYSCALL-EXC_USER, "");
    nueva_instruccion($1, FormatoR, 0, 0, 0, 0, "");
}

// LOAD INT
| FORM_INM_L REG_INT ',' r_inmediato_disp '(' REG_INT ')'
{
    nueva_instruccion($1, FormatoI, $2, $6, 0, $4, desplazamiento);
}

// LOAD FP
| FORM_INM_FP_L REG_FP ',' r_inmediato_disp '(' REG_INT ')'
{
    nueva_instruccion($1, FormatoI, $2, $6, 0, $4, desplazamiento);
}

// STORE INT
| FORM_INM_S REG_INT ',' r_inmediato_disp '(' REG_INT ')'
{
    nueva_instruccion($1, FormatoI, 0, $6, $2, $4, desplazamiento);
}

// STORE FP
| FORM_INM_FP_S REG_FP ',' r_inmediato_disp '(' REG_INT ')'
{
    nueva_instruccion($1, FormatoI, 0, $6, $2, $4, desplazamiento);
}

// R R R
| FORM_REG REG_INT ',' REG_INT ',' REG_INT
{
    nueva_instruccion($1, FormatoR, $2, $4, $6, 0, "");
}

// FP FP FP
| FORM_FP_REG REG_FP ',' REG_FP ',' REG_FP
{
    nueva_instruccion($1, FormatoR, $2, $4, $6, 0, "");
}

// FP FP
| FORM_FP_REG_DF REG_FP ',' REG_FP
{
    nueva_instruccion($1, FormatoR, $2, $4, 0, 0, "");
}

// R R I
| FORM_REG REG_INT ',' REG_INT ',' r_inmediato_disp
{
    /* El código de operación de las operaciones aritméticas
       enteras con valores inmediatos son iguales a las que usan
       registros más UNO. Ver fichero instrucciones.h */

    nueva_instruccion($1 + 1, FormatoI, $2, $4, 0, $6, desplazamiento);
}

// R R I nativo
| FORM_INM REG_INT ',' REG_INT ',' r_inmediato_disp
{
    nueva_instruccion($1, FormatoI, $2, $4, 0, $6, desplazamiento);
}

// TRAP
| FORM_INM_T T_INT_LIT
{
    nueva_instruccion($1, FormatoI, 0, 0, 0, $2, "");
}

// LUI
| FORM_INM_HI REG_INT ',' r_inmediato_hi
{
    nueva_instruccion($1, FormatoI, $2, 0, 0, $4, desplazamiento);
}

// ORI
| FORM_INM_LO REG_INT ',' r_inmediato_lo
{
    nueva_instruccion($1, FormatoI, $2, 0, 0, $4, desplazamiento);
}

// C.xx.D C.xx.S
| FORM_FP_REG_FF REG_FP ',' REG_FP
{
    nueva_instruccion($1, FormatoR, REG_FPSR, $2, $4, 0, "");
}

// BEQZ, BNEZ
| FORM_INM_B_FI REG_INT ',' r_posicion
{
    nueva_instruccion($1, FormatoI, 0, $2, 0, $4 - pos[region] - 4, "");
}

// BEQ, BNE
| FORM_INM_B_FFI REG_INT ',' REG_INT ',' r_posicion
{
    nueva_instruccion($1, FormatoI, 0, $2, $4, $6 - pos[region] - 4, "");
}

// BC1T BC1F
| FORM_INM_B r_posicion
{
    nueva_instruccion($1, FormatoI, 0, REG_FPSR, 0, $2 - pos[region] - 4, "");
}

// J JAL
| FORM_J r_posicion
{
    nueva_instruccion($1, FormatoJ, 0, 0, 0, $2 - pos[region] - 4, "");
}

// JR JALR
| FORM_J_F REG_INT
{
    nueva_instruccion($1, FormatoJ, 0, $2, 0, 0, "");
}
;

%%

/*** Definición de código ***********************************************/

void nueva_instruccion(
    codop_t codop,
    formato_t tipo,
    int Rdestino,
    int Rfuente1,
    int Rfuente2,
    half inmediato,
    char * etiqueta
) {
    if (!es_region_codigo(region)) {
        yyerror("Se debe abrir una sección de código (v.g. .text) antes de insertar instrucciones");
    }

    bzero(&instruccion, sizeof (instruccion_t));

    instruccion.codop = codop;
    instruccion.tipo = tipo;
    instruccion.Rdestino = Rdestino;
    instruccion.Rfuente1 = Rfuente1;
    instruccion.Rfuente2 = Rfuente2;
    instruccion.inmediato = inmediato;
    strcpy(instruccion.etiqueta, etiqueta);
    mem_escribe_region_instruc(mm, region, pos[region], instruccion);
    pos[region] += 4;
}

int yyerror
(
    char *s
        ) {
    fprintf(stderr, "%s:%ld. Error: %s\n", ts->fichero, linenumber, s);
    exit(1);
} /* end yyerror */

void ensamblador (
    fichero_objeto_t * obj
      ) {

    FILE * f_input;

    /* Inicializamos variables, por si hacemos dos pasadas de ensamblador */
    linenumber = 1;
    hay_valor=0;
    hay_etiq=0;

    /* Si están inicializados en otro módulo no se pueden volver a inicializar */
    no_ireg = init_ireg;
    no_fpreg = init_fpreg;
    no_btb = init_btb;

    idx_ireg = 1;
    idx_fpreg = 0;
    idx_btb = 0;
    i = 0;

    mm = &obj->memoria;
    ts = &obj->ts;

    for (i= 0; i< num_regiones; i++) {
       pos[i] = 0;
    }
    region = r_invalid;

    f_input = fopen(obj->ts.fichero, "r");
    if (f_input == NULL) {
       fprintf(stderr, "Error abriendo el fichero '%s'.\n", obj->ts.fichero);
       exit(1);
    } /* endif */

    yyin = f_input;
    if (yyin == NULL) return;

    /*** Realiza el 'parsing' del fichero de entrada ***/

    yyparse();

    // Ajustando los límites de las regiones utilizadas por el objeto
    for (i= 0; i< num_regiones; i++) {
    	  mm->regiones[i].final = pos[i];
    }

    fclose(f_input);

} /* end ensamblador */

