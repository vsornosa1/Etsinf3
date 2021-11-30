/***************************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politècnica de Valencia.                         
 *
 * Autor: Pedro López (plopez@disca.upv.es)
 *        Sergio Sáez (ssaez@disca.upv.es)
 *  *
 * Fichero: log.c
 * 
 * Descripción: 
 *   Contiene las funciones syscall
 *
 ****************************************************************************/


/*** Includes **************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/*** Definiciones Externas *************************************************/

#include "tipos.h"
#include "main.h"
#include "depurar.h"
#include "instrucciones.h"
#include "log.h"


/*** Macros ****************************************************************/

#define print_log(format,...) do { \
  char line[256];                   \
  sprintf(line, format, __VA_ARGS__); \
  strcat(LOG, line);                \
} while(0)

/*** Variables Globales ****************************************************/

/*** Variables Locales *****************************************************/

FILE * f_log;

/*** Funciones Locales *************************************************/

/*** Funciones Exportables *************************************************/

void write_log_2(char* senyal, dword pc_read, dword pc_write, char* fase_read, char* fase_write) {
    char instr_dst[64] = "";
    char instr_fte[64] = "";

    f_log = fopen(log_name, "a");
    print_log("Ciclo: %u, Señal: %s; ", Ciclo + 1, senyal);
    fprintf(f_log, "Ciclo: %u, Señal: %s; ", Ciclo + 1, senyal);
    imprime_instruccion_color(instr_dst, pc_write, NO);
    imprime_instruccion_color(instr_fte, pc_read, NO);
    print_log("Rdst %s(en %s) == Rfte %s(en %s)<br>\n", instr_dst, fase_write, instr_fte, fase_read);
    fprintf(f_log, "Rdst %s(en %s) == Rfte %s(en %s)\n", instr_dst, fase_write, instr_fte, fase_read);
    fclose(f_log);

}

void write_log_1(char* senyal, dword pc, ubyte reg) {
    //char instr_dst[128] = "";
    char instr_fte[128] = "";

    f_log = fopen(log_name, "a");
    print_log("Ciclo: %u, Señal: %s; ", Ciclo + 1, senyal);
    fprintf(f_log, "Ciclo: %u, Señal: %s; ", Ciclo + 1, senyal);
    imprime_instruccion_color(instr_fte, pc, NO);
    print_log("Instrucción %s, Reg F%d<br>\n", instr_fte, reg);
    fprintf(f_log, "Instrucción %s, Reg F%d\n", instr_fte, reg);
    fclose(f_log);
}

void write_log(char* senyal, dword pc) {
    //char instr_dst[128] = "";
    char instr_fte[128] = "";

    f_log = fopen(log_name, "a");
    print_log("Ciclo: %u, Señal: %s; ", Ciclo + 1, senyal);
    fprintf(f_log, "Ciclo: %u, Señal: %s; ", Ciclo + 1, senyal);
    imprime_instruccion_color(instr_fte, pc, NO);
    print_log("Instrucción %s<br>\n", instr_fte);
    fprintf(f_log, "Instrucción %s\n", instr_fte);
    fclose(f_log);
}

