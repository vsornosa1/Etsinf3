/***************************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politècnica de Valencia.                         
 *
 * Autor: Pedro López (plopez@disca.upv.es)
 *        Sergio Sáez (ssaez@disca.upv.es)
 *  *
 * Fichero: mips_syscall.c
 * 
 * Descripción: 
 *   Contiene las funciones syscall del SPIM
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
#include "mips_syscall.h"
#include "depurar.h"
#include "registros.h"
#include "memoria.h"


/*** Variables Globales ****************************************************/

/*** Variables Locales *****************************************************/

FILE * f_consola;

/*** Funciones Locales *************************************************/

#define console_out(formato, args...)  { \
    f_consola = fopen(console_name, "a"); \
    sprintf(CONSOLA,formato,##args); \
    fprintf(f_consola, formato, ##args); \
    fclose(f_consola);}

/*** Funciones Exportables *************************************************/

boolean process_syscall() {
    dword call_nr,
            param1,
            param2;

    valor_t ivalue;
    valor_t fvalue;


    //char src[128] = "";
    char dst[128] = "";
    char achar;

    boolean result = NO;
    int i;

    boolean dummy = NO;

    call_nr = Rint[_v0].valor;
    param1 = Rint[_a0].valor;
    param2 = Rint[_a1].valor;


    //printf("DEBUG syscall %d %d %d\n", call_nr, param1, param2);

    switch (call_nr) {
        case 1: // print integer
            printf("%" PRId64, param1);
            console_out("(print int) %" PRId64 "\n", param1);
            break;
        case 2: // print float
            printf("%f", Rfp[12].valor.fp_s);
            console_out("(print float) %f\n", Rfp[12].valor.fp_s);
            break;
        case 3: // print double
            printf("%lf", Rfp[12].valor.fp_d);
            console_out("(print double) %lf\n", Rfp[12].valor.fp_d);
            break;
        case 4: // print string
            i = 0;
            do {
                ivalue = lee_mem_datos(mmem, param1 + i, t_byte, &dummy);
                achar = ivalue.int_b;
                //achar = MD.m_byte[dir_byte(param1 + i)];
                dst[i] = achar;
                //sprintf(src, "%c", achar);
                //strcat(dst,src);
                i++;
            } while ((achar != 0)&&(i < 128));
            printf("%s", dst);
            console_out("(print str) %s\n", dst);
            break;
        case 5: // read integer
            scanf("%" PRId64, &ivalue.int_d);
            escribe_int_reg(_v0, ivalue.int_d, t_dword, SI);
            //Rint[_v0].valor = ivalue;
            //printf("\n");
            console_out("(read int) %" PRId64 "\n", ivalue.int_d);
            break;
        case 6: // read float
            scanf("%f", &fvalue.fp_s);
            escribe_fp_reg(0, fvalue, t_float, SI);
            //Rfp[0].valor.fp_s = fvalue;
            //printf("\n");
            console_out("(read float) %f\n", fvalue.fp_s);
            break;
        case 7: // read double
            scanf("%lf", &fvalue.fp_d);
            escribe_fp_reg(0, fvalue, t_double, SI);
            //Rfp[0].valor.fp_d = dvalue;
            //printf("\n");
            console_out("(read double) %lf\n", fvalue.fp_d);
            break;
        case 8: // read string
            scanf("%s", dst);
            //printf("%s", dst);
            i = 0;
            do {
                achar = dst[i];
                ivalue.int_b = achar;
                escribe_mem_datos(mmem, param1 + i, ivalue, t_byte, SI);
                //MD.m_byte[dir_byte(param1 + i)] = achar;
                //MD_tipo[param1 + i] = t_byte;
                i++;
            } while ((achar != 0)&&(i < param2));
            ivalue.int_b = 0;
            escribe_mem_datos(mmem, param1 + i, ivalue, t_byte, SI);
            //MD.m_byte[dir_byte(param1 + i)] = 0; // Null terminated
            //printf("\n");
            console_out("(read str) %s\n", dst);
            break;
        case 10: // exit
            printf("%s\n", "syscall 10: Programa terminado");
            console_out("%s\n", "syscall 10: Programa terminado\n");
            result = SI;
            break;
        case 11: // print character
            printf("%c", (char) param1);
            console_out("(print char) %c\n", (char) param1);
            break;
        case 12: // read character
            scanf(" %c", &achar);
            Rint[_v0].valor = (dword) achar;
            console_out("(read char) %d\n", achar);
            break;
        default:
            printf("\nsyscall %" PRId64 " no implementado: Programa terminado\n", call_nr);
            result = SI;
    }


    return (result);
}
