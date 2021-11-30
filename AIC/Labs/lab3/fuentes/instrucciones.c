/**************************************************************************
 * 
 * Departamento de Informatica de Sistemas y Computadores (DISCA)
 * Universidad Politecnica de Valencia.                         
 * 
 * Author:	Sergio Sáez (ssaez@disca.upv.es)
 *			Pedro López (plopez@disca.upv.es)
 * 
 * File: instrucciones.c
 *  
 * Description: Contiene las funciones necesarias para la imprimir las
 *    instrucciones
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *************************************************************************/

/***** Includes ***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>


/***** Definiciones Externas **********************************/

#include "main.h"
#include "instrucciones.h"
#include "memoria.h"
#include "enlazador.h"
#include "presentacion-html.h"

/***** Funciones Exportables **********************************/

int bloque_instruccion(
        dword PC
        ) {
    int index;

    if (PC == -1) {
        return -1;
    }

    ts_busca_direccion(programa.simbolos, PC, &index);
    index %= NUM_STYLES;

    return index;
}

/***************************************************************
 *
 * Func: color_instruccion
 *
 * Desc: Devuelve un color
 *
 **************************************************************/

void color_instruccion(
        char *color,
        dword PC
        ) {
    static char *paleta[15] = {
        "darkred", "darkcyan", "limegreen", "orange", "deeppink", "royalblue", "brown", "seagreen",
        "lightcoral", "magenta", "steelblue", "salmon", "mediumseagreen", "darkorange", "orchid"
    };

    //int index = bloque_instruccion(PC);
    int index = PC / 4;

    if (index < 0) {
        sprintf(color, "lightgray");
        return;
    } /* endif */

    strcpy(color, paleta[index % 15]);

} /* end color_instruccion */

#define imprime_R_int(resultado, mnemonic, instruccion) do { \
sprintf(resultado, "%s r%d,r%d,r%d", mnemonic, \
        instruccion.Rdestino, \
        instruccion.Rfuente1, \
        instruccion.Rfuente2  \
); \
} while (0)

#define imprime_I_int(resultado, mnemonic, instruccion) do { \
sprintf(resultado, "%s r%d,r%d,%s", mnemonic, \
            instruccion.Rdestino, \
            instruccion.Rfuente1, \
            instruccion.etiqueta \
            ); \
} while(0)

#define imprime_L_int(resultado, mnemonic, instruccion) do { \
sprintf(resultado, "%s r%d,%s(r%d)", mnemonic, \
        instruccion.Rdestino, \
        instruccion.etiqueta, \
        instruccion.Rfuente1 \
        ); \
} while(0)

#define imprime_L_fp(resultado, mnemonic, instruccion) do { \
sprintf(resultado, "%s f%d,%s(r%d)", mnemonic, \
        instruccion.Rdestino, \
        instruccion.etiqueta, \
        instruccion.Rfuente1 \
        ); \
} while(0)

#define imprime_S_int(resultado, mnemonic, instruccion) do { \
sprintf(resultado, "%s r%d,%s(r%d)", mnemonic, \
        instruccion.Rfuente2, \
        instruccion.etiqueta, \
        instruccion.Rfuente1 \
        ); \
} while(0)

#define imprime_S_fp(resultado, mnemonic, instruccion) do { \
sprintf(resultado, "%s f%d,%s(r%d)", mnemonic, \
        instruccion.Rfuente2, \
        instruccion.etiqueta, \
        instruccion.Rfuente1 \
        ); \
} while(0)

#define imprime_CMP_fp(resultado, mnemonic, instruccion) do { \
sprintf(resultado, "%s fpsr,f%d,f%d", mnemonic, \
        instruccion.Rfuente1, \
        instruccion.Rfuente2 \
); \
} while(0)

#define imprime_R_fp(resultado, mnemonic, instruccion) do { \
sprintf(resultado, "%s f%d,f%d,f%d", mnemonic, \
        instruccion.Rdestino, \
        instruccion.Rfuente1, \
        instruccion.Rfuente2 \
); \
} while(0)

#define imprime_B(resultado, mnemonic, instruccion) do { \
char aux[128]; \
ts_simbolo_direccion(programa.simbolos, \
                      PC + 4 + instruccion.inmediato, aux, f_etiquetas); \
sprintf(resultado, "%s %s", mnemonic, \
        aux); \
} while(0)

#define imprime_Bx(resultado, mnemonic, instruccion) do { \
char aux[128]; \
ts_simbolo_direccion(programa.simbolos, \
                      PC + 4 + instruccion.inmediato, aux, f_etiquetas); \
sprintf(resultado, "%s r%d,%s", mnemonic, \
        instruccion.Rfuente1, \
        aux); \
} while(0)

#define imprime_Bxx(resultado, mnemonic, instruccion) do { \
char aux[128]; \
ts_simbolo_direccion(programa.simbolos, \
                      PC + 4 + instruccion.inmediato, aux, f_etiquetas); \
sprintf(resultado, "%s r%d,r%d,%s", mnemonic, \
        instruccion.Rfuente1, \
        instruccion.Rfuente2, \
        aux); \
} while(0)

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

void imprime_instruccion_color(
        char *instruc,
        dword PC,
        boolean encolor
        ) {

    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char color[128];

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    if (!en_region(mmem, PC, r_text)) {
        sprintf(instruc, "-nop-");
    } else {
        instruccion_t instruccion = lee_mem_instruc(mmem, PC);
        switch (instruccion.codop) {
            case OP_NOP:
                sprintf(instruc, "nop");
                break;
            case OP_TRAP:
                sprintf(instruc, "trap %d",
                        instruccion.inmediato);
                break;
            case OP_SYSCALL:
                sprintf(instruc, "syscall");
                break;
            case OP_LB:
                imprime_L_int(instruc, "lb", instruccion);
                break;
            case OP_LBU:
                imprime_L_int(instruc, "lbu", instruccion);
                break;
            case OP_SB:
                imprime_S_int(instruc, "sb", instruccion);
                break;
            case OP_LH:
                imprime_L_int(instruc, "lh", instruccion);
                break;
            case OP_LHU:
                imprime_L_int(instruc, "lhu", instruccion);
                break;
            case OP_SH:
                imprime_S_int(instruc, "sh", instruccion);
                break;
            case OP_LW:
                imprime_L_int(instruc, "lw", instruccion);
                break;
            case OP_LWU:
                imprime_L_int(instruc, "lwu", instruccion);
                break;
            case OP_SW:
                imprime_S_int(instruc, "sw", instruccion);
                break;
            case OP_LD:
                imprime_L_int(instruc, "ld", instruccion);
                break;
            case OP_SD:
                imprime_S_int(instruc, "sd", instruccion);
                break;
            case OP_DADD:
                imprime_R_int(instruc, "dadd", instruccion);
                break;
            case OP_DSUB:
                imprime_R_int(instruc, "dsub", instruccion);
                break;
            case OP_DMUL:
                imprime_R_int(instruc, "dmul", instruccion);
                break;
            case OP_DDIV:
                imprime_R_int(instruc, "ddiv", instruccion);
                break;
            case OP_LUI:
                sprintf(instruc, "lui r%d, %s",
                        instruccion.Rdestino,
                        instruccion.etiqueta
                        );
                break;
            case OP_AND:
                imprime_R_int(instruc, "and", instruccion);
                break;
            case OP_OR:
                imprime_R_int(instruc, "or", instruccion);
                break;
            case OP_XOR:
                imprime_R_int(instruc, "xor", instruccion);
                break;
            case OP_DSRA:
                imprime_R_int(instruc, "dsrav", instruccion);
                break;
            case OP_DSRL:
                imprime_R_int(instruc, "dsrlv", instruccion);
                break;
            case OP_DSLL:
                imprime_R_int(instruc, "dsllv", instruccion);
                break;
            case OP_SEQ:
                imprime_R_int(instruc, "seq", instruccion);
                break;
            case OP_SNE:
                imprime_R_int(instruc, "sne", instruccion);
                break;
            case OP_SGT:
                imprime_R_int(instruc, "sgt", instruccion);
                break;
            case OP_SGE:
                imprime_R_int(instruc, "sge", instruccion);
                break;
            case OP_SLT:
                imprime_R_int(instruc, "slt", instruccion);
                break;
            case OP_SLE:
                imprime_R_int(instruc, "sle", instruccion);
                break;
            case OP_DADDI:
                imprime_I_int(instruc, "dadd", instruccion);
                break;
            case OP_DSUBI:
                imprime_I_int(instruc, "dsub", instruccion);
                break;
            case OP_ANDI:
                imprime_I_int(instruc, "and", instruccion);
                break;
            case OP_ORI:
                imprime_I_int(instruc, "or", instruccion);
                break;
            case OP_XORI:
                imprime_I_int(instruc, "xor", instruccion);
                break;
            case OP_DSRAI:
                imprime_I_int(instruc, "dsra", instruccion);
                break;
            case OP_DSRLI:
                imprime_I_int(instruc, "dsrl", instruccion);
                break;
            case OP_DSLLI:
                imprime_I_int(instruc, "dsll", instruccion);
                break;
            case OP_SEQI:
                imprime_I_int(instruc, "seq", instruccion);
                break;
            case OP_SNEI:
                imprime_I_int(instruc, "sne", instruccion);
                break;
            case OP_SGTI:
                imprime_I_int(instruc, "sgt", instruccion);
                break;
            case OP_SGEI:
                imprime_I_int(instruc, "sge", instruccion);
                break;
            case OP_SLTI:
                imprime_I_int(instruc, "slt", instruccion);
                break;
            case OP_SLEI:
                imprime_I_int(instruc, "sle", instruccion);
                break;
            case OP_BNEZ:
                imprime_Bx(instruc, "bnez", instruccion);
                break;
            case OP_BEQZ:
                imprime_Bx(instruc, "beqz", instruccion);
                break;
            case OP_BNE:
                imprime_Bxx(instruc, "bne", instruccion);
                break;
            case OP_BEQ:
                imprime_Bxx(instruc, "beq", instruccion);
                break;
            case OP_BC1T:
                imprime_B(instruc, "bc1t", instruccion);
                break;
            case OP_BC1F:
                imprime_B(instruc, "bc1f", instruccion);
                break;
            case OP_MOVZ:
                imprime_R_int(instruc, "movz", instruccion);
                break;
            case OP_MOVN:
                imprime_R_int(instruc, "movn", instruccion);
                break;
            case OP_J:
                imprime_B(instruc, "j", instruccion);
                break;
            case OP_JAL:
                imprime_B(instruc, "jal", instruccion);
                break;
            case OP_JR:
                sprintf(instruc, "jr r%d",
                        instruccion.Rfuente1);
                break;
            case OP_JALR:
                sprintf(instruc, "jalr r%d",
                        instruccion.Rfuente1);
                break;
            case OP_FP_ADD_D:
                imprime_R_fp(instruc, "add.d", instruccion);
                break;
            case OP_FP_SUB_D:
                imprime_R_fp(instruc, "sub.d", instruccion);
                break;
            case OP_FP_MUL_D:
                imprime_R_fp(instruc, "mul.d", instruccion);
                break;
            case OP_FP_DIV_D:
                imprime_R_fp(instruc, "div.d", instruccion);
                break;
            case OP_FP_ADD_S:
                imprime_R_fp(instruc, "add.s", instruccion);
                break;
            case OP_FP_SUB_S:
                imprime_R_fp(instruc, "sub.s", instruccion);
                break;
            case OP_FP_MUL_S:
                imprime_R_fp(instruc, "mul.s", instruccion);
                break;
            case OP_FP_DIV_S:
                imprime_R_fp(instruc, "div.s", instruccion);
                break;
            case OP_FP_ADD_PS:
                imprime_R_fp(instruc, "add.ps", instruccion);
                break;
            case OP_FP_SUB_PS:
                imprime_R_fp(instruc, "sub.ps", instruccion);
                break;
            case OP_FP_MUL_PS:
                imprime_R_fp(instruc, "mul.ps", instruccion);
                break;
            case OP_FP_DIV_PS:
                imprime_R_fp(instruc, "div.ps", instruccion);
                break;
            case OP_FP_L_D:
                imprime_L_fp(instruc, "l.d", instruccion);
                break;
            case OP_FP_S_D:
                imprime_S_fp(instruc, "s.d", instruccion);
                break;
            case OP_FP_L_S:
                imprime_L_fp(instruc, "l.s", instruccion);
                break;
            case OP_FP_S_S:
                imprime_S_fp(instruc, "s.s", instruccion);
                break;
            case OP_FP_L_PS:
                imprime_L_fp(instruc, "l.ps", instruccion);
                break;
            case OP_FP_S_PS:
                imprime_S_fp(instruc, "s.ps", instruccion);
                break;
            case OP_FP_EQ_D:
                imprime_CMP_fp(instruc, "c.eq.d", instruccion);
                break;
            case OP_FP_NE_D:
                imprime_CMP_fp(instruc, "c.ne.d", instruccion);
                break;
            case OP_FP_LT_D:
                imprime_CMP_fp(instruc, "c.eq.d", instruccion);
                break;
            case OP_FP_LE_D:
                imprime_CMP_fp(instruc, "c.le.d", instruccion);
                break;
            case OP_FP_GT_D:
                imprime_CMP_fp(instruc, "c.gt.d", instruccion);
                break;
            case OP_FP_GE_D:
                imprime_CMP_fp(instruc, "c.ge.d", instruccion);
                break;
            case OP_FP_EQ_S:
                imprime_CMP_fp(instruc, "c.eq.s", instruccion);
                break;
            case OP_FP_NE_S:
                imprime_CMP_fp(instruc, "c.ne.s", instruccion);
                break;
            case OP_FP_LT_S:
                imprime_CMP_fp(instruc, "c.eq.s ", instruccion);
                break;
            case OP_FP_LE_S:
                imprime_CMP_fp(instruc, "c.le.s", instruccion);
                break;
            case OP_FP_GT_S:
                imprime_CMP_fp(instruc, "c.gt.s", instruccion);
                break;
            case OP_FP_GE_S:
                imprime_CMP_fp(instruc, "c.ge.s", instruccion);
                break;
            case OP_FP_MOV_D:
                sprintf(instruc, "mov.d f%d,f%d",
                        instruccion.Rdestino,
                        instruccion.Rfuente1
                        );
                break;
            case OP_FP_MOV_S:
                sprintf(instruc, "mov.s f%d,f%d",
                        instruccion.Rdestino,
                        instruccion.Rfuente1
                        );
                break;
                /*
               case OP_MFC1:
                    sprintf(instruc, "mfc1 r%d,f%d",
                            instruccion.Rdestino,
                            instruccion.Rfuente1
                            );
                    break;
               case OP_MTC1:
                    sprintf(instruc, "mtc1 f%d,r%d",
                            instruccion.Rdestino,
                            instruccion.Rfuente1
                            );
                    break;
                 */
            default:
                sprintf(instruc, "Inst desconocida r%d,r%d,r%d,#%d",
                        instruccion.Rdestino,
                        instruccion.Rfuente1,
                        instruccion.Rfuente2,
                        instruccion.inmediato
                        );
                break;
        } /* endswitch */
    } /* endif */

    if (encolor == SI) {
        char aux[128];
        strcpy(aux, instruc);
        color_instruccion(color, PC);
        sprintf(instruc, "<font color=\"%s\">%s</font>", color, aux);
    }

} /* end imprime_instruccion */
