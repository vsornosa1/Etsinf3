/***************************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politècnica de Valencia.
 *
 * Autor: Pedro López (plopez@disca.upv.es)
 *        Sergio Sáez (ssaez@disca.upv.es)
 *
 *
 * Fichero: mips_fp.c
 *
 * Descripción:
 *   Contiene la microarquitectura de la extensión multiciclo del MIPS
 *
 *
 ****************************************************************************/

#define mips_fp_C

/*** Includes **************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


/*** Definiciones Externas *************************************************/

#include "main.h"
#include "instrucciones.h"
#include "depurar.h"
#include "control.h"
#include "mips_fp.h"
#include "log.h"
#include "memoria.h"


/*** Variables Globales ****************************************************/

/*** Variables Locales *****************************************************/

/*** Funciones Locales *****************************************************/

/*** Funciones Exportables *************************************************/

/***************************************************************
 *
 * Func: operacion_ALU_FP
 *
 * Desc: Realiza la operacion en el operador aritmético de FP
 *
 **************************************************************/
valor_t operacion_ALU_FP(ubyte codop, valor_t in1, valor_t in2) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    switch (codop) {
        case OP_NOP:
            result.fp_d = 0;
        case OP_FP_ADD_D:
            result.fp_d = in1.fp_d + in2.fp_d;
            break;
        case OP_FP_SUB_D:
            result.fp_d = in1.fp_d - in2.fp_d;
            break;
        case OP_FP_MUL_D:
            result.fp_d = in1.fp_d * in2.fp_d;
            break;
        case OP_FP_DIV_D:
            result.fp_d = in1.fp_d / in2.fp_d;
            break;
        case OP_FP_ADD_S:
            result.fp_s = in1.fp_s + in2.fp_s;

            break;
        case OP_FP_SUB_S:
            result.fp_s = in1.fp_s - in2.fp_s;

            break;
        case OP_FP_MUL_S:
            result.fp_s = in1.fp_s * in2.fp_s;

            break;
        case OP_FP_DIV_S:
            result.fp_s = in1.fp_s / in2.fp_s;

            break;
        case OP_FP_ADD_PS:
            result.fp_ps[Lo] = in1.fp_ps[Lo] + in2.fp_ps[Lo];
            result.fp_ps[Hi] = in1.fp_ps[Hi] + in2.fp_ps[Hi];
            break;
        case OP_FP_SUB_PS:
            result.fp_ps[Lo] = in1.fp_ps[Lo] - in2.fp_ps[Lo];
            result.fp_ps[Hi] = in1.fp_ps[Hi] - in2.fp_ps[Hi];
            break;
        case OP_FP_MUL_PS:
            result.fp_ps[Lo] = in1.fp_ps[Lo] * in2.fp_ps[Lo];
            result.fp_ps[Hi] = in1.fp_ps[Hi] * in2.fp_ps[Hi];
            break;
        case OP_FP_DIV_PS:
            result.fp_ps[Lo] = in1.fp_ps[Lo] / in2.fp_ps[Lo];
            result.fp_ps[Hi] = in1.fp_ps[Hi] / in2.fp_ps[Hi];
            break;
        default:
            fnError("Operacion FP %d no implementada\n\n", codop)
                    //fprintf(stderr,
                    //        "ERROR (%d %d): Operacion FP no implementada\n",
                    //        __LINE__, codop);
                    //exit(1);
    } /* endswitch */

    return (result);
} /* end operacion_ALU */

/***************************************************************
 *
 * Func: comparacion_ALU_FP
 *
 * Desc: Realiza la operacion en el operador aritmético de comparacion FP
 *
 **************************************************************/
dword comparacion_ALU_FP(ubyte codop, valor_t in1, valor_t in2) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/


    switch (codop) {
        case OP_NOP:
            result = 0;
        case OP_FP_EQ_D:
            result = (in1.fp_d == in2.fp_d);
            break;
        case OP_FP_NE_D:
            result = (in1.fp_d != in2.fp_d);
            break;
        case OP_FP_LT_D:
            result = (in1.fp_d < in2.fp_d);
            break;
        case OP_FP_LE_D:
            result = (in1.fp_d <= in2.fp_d);
            break;
        case OP_FP_GT_D:
            result = (in1.fp_d > in2.fp_d);
            break;
        case OP_FP_GE_D:
            result = (in1.fp_d >= in2.fp_d);
            break;
        case OP_FP_EQ_S:
            result = (in1.fp_s == in2.fp_s);
            break;
        case OP_FP_NE_S:
            result = (in1.fp_s != in2.fp_s);
            break;
        case OP_FP_LT_S:
            result = (in1.fp_s < in2.fp_s);
            break;
        case OP_FP_LE_S:
            result = (in1.fp_s <= in2.fp_s);
            break;
        case OP_FP_GT_S:
            result = (in1.fp_s > in2.fp_s);
            break;
        case OP_FP_GE_S:
            result = (in1.fp_s >= in2.fp_s);
            break;
        default:
            fnError("Comparación FP %d no implementada\n", codop)
                    //fprintf(stderr,
                    //        "ERROR (%d %d): Comparacion FP no implementada\n",
                    //        __LINE__, codop);
                    //exit(1);
    } /* endswitch */
    //	fprintf(stdout,"DEBUG comparacion_ALU_FP (codop=%d) %f %f result=%d \n",codop,in1,in2,result);

    return (result);
} /* end operacion_ALU */

/***************************************************************
 *
 * Func: mux_FP_LSsup
 *
 * Desc: Implementa el multiplexor inferior del op LS
 *
 **************************************************************/
dword mux_FP_LSsup(dword ra, dword mem, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WBaLS1_s = NO;
    MEMaLS1_s = NO;


    result = ra;

    if (hay_destino_MEM() && hay_fuente1_LS1() && (ID_LS.IR.Rfuente1 == EX_MEM.IR.Rdestino)) {
        MEMaLS1_s = SI;
        result = mem;
        write_log_2("corto MEMaLS1_s", ID_LS.iPC, EX_MEM.iPC, "LS", "MEM");
        goto final;
    } /* endif */


    if (hay_destino_WB() && hay_fuente1_LS1() && (ID_LS.IR.Rfuente1 == MEM_WB.IR.Rdestino)) {
        WBaLS1_s = SI;
        result = wb;
        write_log_2("corto WBaLS1_s", ID_LS.iPC, MEM_WB.iPC, "LS", "WB");

    } /* endif */

final:

    return (result);
} /* end mux_FP_LSsup */

/***************************************************************
 *
 * Func: mux_FP_LSinf
 *
 * Desc: Implementa el multiplexor inferior del op LS
 *
 **************************************************************/
valor_t mux_FP_LSinf(valor_t reg_val, valor_t wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    FP_WBaLS1_i = NO;

    result = reg_val;
    // hay_fuente2_LS1() es true solo en S.D
    // aun así, es redundante comprobarlo, ya que L.D no va a utilizar el valor devuelto
    if (hay_destino_FP_WB() && hay_fuente2_LS1() && (ID_LS.IR.Rfuente2 == FP_WB.IR.Rdestino)) {
        FP_WBaLS1_i = SI;
        result = wb;
        write_log_2("corto FP_WBaLS1_i", ID_LS.iPC, FP_WB.iPC, "FP LS", "FP WB");
    } /* endif */


    return (result);
} /* end mux_FP_LSinf */

/***************************************************************
 *
 * Func: mux_FP_LS2inf
 *
 * Desc: Implementa el multiplexor inferior del op LS en LS2
 *
 **************************************************************/
valor_t mux_FP_LS2inf(valor_t reg_val, valor_t wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    FP_WBaLS2_i = NO;

    result = reg_val;
    if (hay_destino_FP_WB() && hay_fuente2_LS2() && (LS[2].IR.Rfuente2 == FP_WB.IR.Rdestino)) {
        FP_WBaLS2_i = SI;
        result = wb;
        write_log_2("corto FP_WBaLS2_i", LS[2].iPC, FP_WB.iPC, "FP LS", "FP WB");
    } /* endif */


    return (result);
} /* end mux_FP_LS2inf */

/***************************************************************
 *
 * Func: mux_FP_ADDinf
 *
 * Desc: Implementa el multiplexor inferior del FP ADD
 *
 **************************************************************/
valor_t mux_FP_ADDinf(valor_t reg_val, valor_t wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    FP_WBaA1_i = NO;

    result = reg_val;

    if (hay_destino_FP_WB() && hay_fuente2_A1() && (ID_A.IR.Rfuente2 == FP_WB.IR.Rdestino)) {
        FP_WBaA1_i = SI;
        result = wb;
        write_log_2("corto FP_WBaA1_i", ID_A.iPC, FP_WB.iPC, "FP ADD", "FP WB");
        //        fprintf (stdout, "DEBUG cambiazo\n");
    } /* endif */
    //    fprintf (stdout, "DEBUG mux_FP ADD_inf %f\n", result);


    return (result);
} /* end mux_FP_ADDinf */

/***************************************************************
 *
 * Func: mux_FP_ADDsup
 *
 * Desc: Implementa el multiplexor inferior del FP ADD
 *
 **************************************************************/
valor_t mux_FP_ADDsup(valor_t reg_val, valor_t wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    FP_WBaA1_s = NO;

    result = reg_val;
    if (hay_destino_FP_WB() && hay_fuente1_A1() && (ID_A.IR.Rfuente1 == FP_WB.IR.Rdestino)) {
        FP_WBaA1_s = SI;
        result = wb;
        write_log_2("corto FP_WBaA1_s", ID_A.iPC, FP_WB.iPC, "FP ADD", "FP WB");
    } /* endif */

    return (result);
} /* end mux_FP_ADDsup */

/***************************************************************
 *
 * Func: mux_FP_CMPinf
 *
 * Desc: Implementa el multiplexor inferior del FP CMP
 *
 **************************************************************/
valor_t mux_FP_CMPsup(valor_t reg_val, valor_t wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    FP_WBaC1_s = NO;

    result = reg_val;

    if (hay_destino_FP_WB() && hay_fuente1_C1() && (ID_C.IR.Rfuente1 == FP_WB.IR.Rdestino)) {
        FP_WBaC1_s = SI;
        result = wb;
        write_log_2("corto FP_WBaC1_s", ID_C.iPC, FP_WB.iPC, "FP CMP", "FP WB");
        //        fprintf (stdout, "DEBUG cambiazo\n");
    } /* endif */
    //    fprintf (stdout, "DEBUG mux_FP ADD_inf %f\n", result);


    return (result);
} /* end mux_FP_CMPsup */

/***************************************************************
 *
 * Func: mux_FP_CMPinf
 *
 * Desc: Implementa el multiplexor inferior del FP CMP
 *
 **************************************************************/
valor_t mux_FP_CMPinf(valor_t reg_val, valor_t wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    FP_WBaC1_i = NO;

    result = reg_val;

    if (hay_destino_FP_WB() && hay_fuente2_C1() && (ID_C.IR.Rfuente2 == FP_WB.IR.Rdestino)) {
        FP_WBaC1_i = SI;
        result = wb;
        write_log_2("corto FP_WBaC1_i", ID_C.iPC, FP_WB.iPC, "FP CMP", "FP WB");

        //        fprintf (stdout, "DEBUG cambiazo\n");
    } /* endif */
    //    fprintf (stdout, "DEBUG mux_FP ADD_inf %f\n", result);


    return (result);
} /* end mux_FP_CMPinf */

/***************************************************************
 *
 * Func: mux_FP_MULsup
 *
 * Desc: Implementa el multiplexor inferior del FP MUL
 *
 **************************************************************/
valor_t mux_FP_MULsup(valor_t reg_val, valor_t wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    FP_WBaM1_i = NO;

    result = reg_val;
    if (hay_destino_FP_WB() && hay_fuente1_M1() && (ID_M.IR.Rfuente1 == FP_WB.IR.Rdestino)) {
        FP_WBaM1_i = SI;
        result = wb;
        write_log_2("corto FP_WBaM1_s", ID_M.iPC, FP_WB.iPC, "FP MUL", "FP WB");
    } /* endif */

    return (result);
} /* end mux_FP_MULsup */

/***************************************************************
 *
 * Func: mux_FP_MULinf
 *
 * Desc: Implementa el multiplexor inferior del FP MUL
 *
 **************************************************************/
valor_t mux_FP_MULinf(valor_t reg_val, valor_t wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    FP_WBaM1_i = NO;

    result = reg_val;
    if (hay_destino_FP_WB() && hay_fuente2_M1() && (ID_M.IR.Rfuente2 == FP_WB.IR.Rdestino)) {
        FP_WBaM1_i = SI;
        result = wb;
        write_log_2("corto FP_WBaM1_i", ID_M.iPC, FP_WB.iPC, "FP MUL", "FP WB");

    } /* endif */

    return (result);
} /* end mux_FP_MULinf */

/***************************************************************
 *
 * Func: mux_FP_WB
 *
 * Desc: Implementa el multiplexor de la fase WB FP
 *
 **************************************************************/
FP_WB_t mux_FP_WB(FP_WB_t fp_ls, FP_WB_t fp_add, FP_WB_t fp_mul, FP_WB_t id_wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    FP_WB_t result;
    word count;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    count = 0;

    result.iPC = -1;
    result.orden = 0;
    result.IR.codop = OP_NOP;
    result.IR.tipo = FormatoR;
    result.IR.Rdestino = 0;
    result.IR.Rfuente1 = 0;
    result.IR.Rfuente2 = 0;
    result.IR.inmediato = 0;
    result.fpALUout.fp_d = 0.0;

    // fprintf(stderr, "DEBUG muxFPWB (%d %d %d %d)\n",fp_ls.iPC,fp_add.iPC,fp_mul.iPC,id_wb.iPC);

    //    if ((fp_ls.iPC != -1)&&(inst_load_fp(fp_ls.IR.codop))) {
    if (fp_ls.iPC != -1) {
        result.iPC = fp_ls.iPC;
        result.orden = fp_ls.orden;
        result.IR = fp_ls.IR;
        result.fpALUout = fp_ls.fpALUout;
        count++;
    };

    if (fp_add.iPC != -1) {
        result.iPC = fp_add.iPC;
        result.orden = fp_add.orden;
        result.IR = fp_add.IR;
        result.fpALUout = fp_add.fpALUout;
        count++;
    };

    if (fp_mul.iPC != -1) {
        result.iPC = fp_mul.iPC;
        result.orden = fp_mul.orden;
        result.IR = fp_mul.IR;
        result.fpALUout = fp_mul.fpALUout;
        count++;
    };

    if (id_wb.iPC != -1) {
        result.iPC = id_wb.iPC;
        result.orden = id_wb.orden;
        result.IR = id_wb.IR;
        result.fpALUout = id_wb.fpALUout;
        count++;
    };

    if (count > 1) {
        fnError("Dos o más instrucciones FP en WB\n")
                //fprintf(stderr, "ERROR (%d): Dos o mas instrucciones fp en WB\n", __LINE__);
                //exit(1);
    };


    return (result);
} /* end mux_FP_WB */

/***************************************************************
 *
 * Func: fase_LS
 *
 * Desc: Implementa la fase LS
 *
 **************************************************************/
void fase_LS(short i) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t datos_mem, datos_mem_store;
    boolean dirty;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    //FP_WBaLS1_i = NO;



    /* propagar datos */

    if (i == 1) {

        /* copiar operandos */
        LSn[2].Fa = ID_LS.Fa;
        LSn[2].Fb = mux_FP_LSinf(ID_LS.Fb, FP_WB.fpALUout);
        //LSn[2].address = mux_FP_LSsup(ID_LS.Ra, EX_MEM.ALUout, MEM_WB.MEMout) + ID_LS.Imm;
        LSn[2].address = mux_FP_LSsup(ID_LS.Ra, EX_MEM.ALUout, WB.data) + ID_LS.Imm;
        LSn[2].IR = ID_LS.IR;
        LSn[2].iPC = ID_LS.iPC;
        LSn[2].orden = ID_LS.orden;
        //		fprintf (stdout, "DEBUG LS(%i) %d %d %f\n",i,Ciclo,LS[i+1].address,LS[i+1].Fb);
        return;
    }

    /* mux datos store en LS2 */
    datos_mem_store = i == 2 ? mux_FP_LS2inf(LS[i].Fb, FP_WB.fpALUout) : LS[i].Fb;

    if (i == FP_LS_LAT) {
        LS_WBn.IR = LS[i].IR;
        LS_WBn.iPC = LS[i].iPC;
        LS_WBn.orden = LS[i].orden;
        switch (LS[i].IR.codop) {
            case OP_FP_L_D:
                datos_mem = lee_mem_datos(mmem, LS[i].address, t_double, &dirty);
                LS_WBn.fpALUout.fp_d = datos_mem.fp_d;
                break;
            case OP_FP_L_S:
                datos_mem = lee_mem_datos(mmem, LS[i].address, t_float, &dirty);
                LS_WBn.fpALUout.fp_ps[Lo] = 0.0;
                LS_WBn.fpALUout.fp_ps[Hi] = 0.0;
                LS_WBn.fpALUout.fp_s = datos_mem.fp_s;
                break;
            case OP_FP_L_PS:
                datos_mem = lee_mem_datos(mmem, LS[i].address, t_float, &dirty);
                LS_WBn.fpALUout.fp_ps[Lo] = datos_mem.fp_s;
                datos_mem = lee_mem_datos(mmem, LS[i].address + 4, t_float, &dirty);
                LS_WBn.fpALUout.fp_ps[Hi] = datos_mem.fp_s;
                break;
            case OP_FP_S_D:
                //datos_mem.fp_d = LS[i].Fb.fp_d;
                datos_mem.fp_d = datos_mem_store.fp_d;
                escribe_mem_datos(mmem, LS[i].address, datos_mem, t_double, SI);
                // Stores no pasan por WB
                pasar_a_FP_LS_WB_NOP();
                estat.instrucciones++;
                estat.multiciclo++;

                break;
            case OP_FP_S_S:
                //datos_mem.fp_s = LS[i].Fb.fp_s;
                datos_mem.fp_s = datos_mem_store.fp_s;
                escribe_mem_datos(mmem, LS[i].address, datos_mem, t_float, SI);
                // Stores no pasan por WB
                pasar_a_FP_LS_WB_NOP();
                estat.instrucciones++;
                estat.multiciclo++;

                break;
            case OP_FP_S_PS:
                //datos_mem.fp_s = LS[i].Fb.fp_s;
                datos_mem.fp_s = datos_mem_store.fp_s;
                escribe_mem_datos(mmem, LS[i].address, datos_mem, t_float, SI);
                //datos_mem.fp_s = LS[i].Fb.fp_ps[Hi];
                datos_mem.fp_s = datos_mem_store.fp_ps[Hi];
                escribe_mem_datos(mmem, LS[i].address + 4, datos_mem, t_float, SI);
                // Stores no pasan por WB
                pasar_a_FP_LS_WB_NOP();
                estat.instrucciones++;
                estat.multiciclo++;

                break;
            default:
                break;
        } /* endswitch */
        //				LS_WBn.fpALUout = 0.0;
        //				fprintf (stdout, "DEBUG LS(%i) %d %d %f\n",i,Ciclo,LS[i].address,LS[i].Fb);

    } else {

        LSn[i + 1].Fa = LS[i].Fa;
        //LSn[i + 1].Fb = LS[i].Fb;
        LSn[i + 1].Fb = datos_mem_store;
        LSn[i + 1].address = LS[i].address;

        LSn[i + 1].IR = LS[i].IR;
        LSn[i + 1].iPC = LS[i].iPC;
        LSn[i + 1].orden = LS[i].orden;
    }
    //       fprintf (stdout, "A(%i) %f\n",i,A[i].ALUout);

} /* end fase_LS */

/***************************************************************
 *
 * Func: fase_A
 *
 * Desc: Implementa la fase A
 *
 **************************************************************/
void fase_A(short i) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t aluin1, aluin2;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    //FP_WBaA1_s = NO;
    //FP_WBaA1_i = NO;


    /* propagar datos */

    if (i == 1) {
        /* multiplexores */
        aluin1 = mux_FP_ADDsup(ID_A.Fa, FP_WB.fpALUout);
        //        fprintf (stdout, "DEBUG A(%i) aluin1=%f sup=%f inf=%f\n",i,aluin1,ID_A.Fa, FP_WB.ALUout);

        aluin2 = mux_FP_ADDinf(ID_A.Fb, FP_WB.fpALUout);
        //        fprintf (stdout, "DEBUG A(%i) aluin2=%f sup=%f inf=%f\n",i,aluin2,ID_A.Fb, FP_WB.ALUout);

        /* hacemos la operacion */
        An[2].fpALUout = operacion_ALU_FP(ID_A.IR.codop, aluin1, aluin2);

        /* propagar datos */

        An[2].IR = ID_A.IR;
        An[2].iPC = ID_A.iPC;
        An[2].orden = ID_A.orden;
    } else if (i == FP_ADD_LAT) {
        ADD_WBn.fpALUout = A[i].fpALUout;

        ADD_WBn.IR = A[i].IR;
        ADD_WBn.iPC = A[i].iPC;
        ADD_WBn.orden = A[i].orden;
    } else {
        An[i + 1].fpALUout = A[i].fpALUout;

        An[i + 1].IR = A[i].IR;
        An[i + 1].iPC = A[i].iPC;
        An[i + 1].orden = A[i].orden;
    }
    //	      fprintf (stdout, "DEBUG A(%i) %d\n",i,FP_WBn.orden);


} /* end fase_A */

/***************************************************************
 *
 * Func: fase_M
 *
 * Desc: Implementa la fase M
 *
 **************************************************************/
void fase_M(short i) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t aluin1, aluin2;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    //FP_WBaM1_s = NO;
    //FP_WBaM1_i = NO;


    /* propagar datos */

    if (i == 1) {
        /* multiplexores */
        aluin1 = mux_FP_MULsup(ID_M.Fa, FP_WB.fpALUout);
        aluin2 = mux_FP_MULinf(ID_M.Fb, FP_WB.fpALUout);

        /* hacemos la operacion */
        Mn[2].fpALUout = operacion_ALU_FP(ID_M.IR.codop, aluin1, aluin2);

        /* propagar datos */

        Mn[2].IR = ID_M.IR;
        Mn[2].iPC = ID_M.iPC;
        Mn[2].orden = ID_M.orden;
    } else if (i == FP_MUL_LAT) {
        MUL_WBn.fpALUout = M[i].fpALUout;

        MUL_WBn.IR = M[i].IR;
        MUL_WBn.iPC = M[i].iPC;
        MUL_WBn.orden = M[i].orden;
    } else {
        Mn[i + 1].fpALUout = M[i].fpALUout;

        Mn[i + 1].IR = M[i].IR;
        Mn[i + 1].iPC = M[i].iPC;
        Mn[i + 1].orden = M[i].orden;
    }
    //       fprintf (stdout, "A(%i) %f\n",i,A[i].ALUout);


} /* end fase_M */

/***************************************************************
 *
 * Func: fase_C
 *
 * Desc: Implementa la fase c
 *
 **************************************************************/
void fase_C(short i) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    valor_t aluin1, aluin2;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    // FP_WBaC1_s = NO;
    //FP_WBaC1_i = NO;

    // fprintf (stdout, "DEBUG C(%i) codop=%d\n",i,C[i].IR.codop);

    /* propagar datos */

    if (i == 1) {
        /* multiplexores */
        aluin1 = mux_FP_CMPsup(ID_C.Fa, FP_WB.fpALUout);
        //		fprintf (stdout, "DEBUG CMP(%i) codop=%d aluin1=%f in1=%f in2=%f\n",i,ID_C.IR.codop,aluin1,ID_C.Fa, FP_WB.fpALUout);

        aluin2 = mux_FP_CMPinf(ID_C.Fb, FP_WB.fpALUout);
        //		fprintf (stdout, "DEBUG CMP(%i) codop=%d aluin2=%f in1=%f in2=%f\n",i,ID_C.IR.codop,aluin2,ID_C.Fb, FP_WB.fpALUout);

        /* hacemos la operacion */
        Cn[2].intALUout = comparacion_ALU_FP(ID_C.IR.codop, aluin1, aluin2);
        //		fprintf (stdout, "DEBUG C(%i) codop=%d, aluin1=%f aluin2=%f result=%d\n",i,ID_C.IR.codop,aluin1,aluin2,Cn[2].intALUout);
        /* propagar datos */

        Cn[2].IR = ID_C.IR;
        Cn[2].iPC = ID_C.iPC;
        Cn[2].orden = ID_C.orden;
    } else if (i == FP_CMP_LAT) {
        CMP_WBn.ALUout = C[i].intALUout;
        //        fprintf (stdout, "DEBUG C(%i) aluin1=%f aluin2=%f result=%d\n",i,aluin1,aluin2,CMP_WBn.intALUout);

        CMP_WBn.IR = C[i].IR;
        CMP_WBn.iPC = C[i].iPC;
        CMP_WBn.orden = C[i].orden;
    } else {

        Cn[i + 1].intALUout = C[i].intALUout;
        Cn[i + 1].IR = C[i].IR;
        Cn[i + 1].iPC = C[i].iPC;
        Cn[i + 1].orden = C[i].orden;
    }
    //	      fprintf (stdout, "DEBUG A(%i) %d\n",i,FP_WBn.orden);


} /* end fase_C */

/***************************************************************
 *
 * Func: detectar_riesgos_RAW_FP
 *
 * Desc: Detecta los riesgos RAW, activando las señales IDstall, IFstall en su caso
 *
 **************************************************************/
void detectar_riesgos_RAW_FP(void) {
    switch (solucion_riesgos_datos) {
        case parada:
            /* Riesgos RAW con la dirección */
            switch (IF_ID.IR.codop) {
                case OP_FP_L_D:
                case OP_FP_S_D:
                case OP_FP_L_S:
                case OP_FP_S_S:
                case OP_FP_L_PS:
                case OP_FP_S_PS:
                    /* Riesgo entre EX e ID */
                    if (hay_destino_EX() && (IF_ID.IR.Rfuente1 == ID_EX.IR.Rdestino)) {
                        IFstall = SI;
                        IDstall = SI;
                        write_log_2("IDstall, IFstall", IF_ID.iPC, ID_EX.iPC, "ID", "EX");
                        break;
                    } /* endif */

                    /* Riesgo entre MEM e ID */
                    if (hay_destino_MEM() && (IF_ID.IR.Rfuente1 == EX_MEM.IR.Rdestino)) {
                        IFstall = SI;
                        IDstall = SI;
                        write_log_2("IDstall, IFstall", IF_ID.iPC, EX_MEM.iPC, "ID", "MEM");
                    } /* endif */

                    break;
                default:
                    break;
            };

            /* Riesgos RAW  */
            switch (IF_ID.IR.codop) {
                case OP_FP_L_D:
                case OP_FP_L_S:
                case OP_FP_L_PS:
                    break;
                case OP_FP_S_D:
                case OP_FP_S_S:
                case OP_FP_S_PS:
                    if (Rfp[IF_ID.IR.Rfuente2].busy != 0) {
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1("RAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.Rfuente2);
                    };

                    break;
                default:
                    if (Rfp[IF_ID.IR.Rfuente1].busy != 0) {
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1("RAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.Rfuente1);
                    };

                    if (Rfp[IF_ID.IR.Rfuente2].busy != 0) {
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1("RAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.Rfuente2);
                    };
                    break;
            };

        case cortocircuito:
            /* Riesgos RAW */
            //sprintf(_msg, "RAW Ciclo=%d fte1=%d Busy=%d\n", Ciclo, IF_ID.IR.Rfuente1, Rfp[IF_ID.IR.Rfuente1].busy);
            //DEBUG(__func__);

            //                fprintf(stdout, "DEBUG
            /* Riesgos RAW con la dirección */
            switch (IF_ID.IR.codop) {
                case OP_FP_L_D:
                case OP_FP_S_D:
                case OP_FP_L_S:
                case OP_FP_S_S:
                case OP_FP_L_PS:
                case OP_FP_S_PS:
                    /* Riesgo entre EX e ID */
                    if (inst_load_int(ID_EX.IR.codop) && (IF_ID.IR.Rfuente1 == ID_EX.IR.Rdestino)) {
                        IFstall = SI;
                        IDstall = SI;
                        write_log_2("IDstall, IFstall", IF_ID.iPC, ID_EX.iPC, "ID", "EX");
                    } /* endif */
                    break;
                default:
                    break;
            };

            /* Riesgos RAW  */
            switch (IF_ID.IR.codop) {
                case OP_FP_L_D:
                case OP_FP_L_S:
                case OP_FP_L_PS:
                    break;
                case OP_FP_S_D:
                case OP_FP_S_S:
                case OP_FP_S_PS:
                    // if ((int) (Rfp[IF_ID.IR.Rfuente2].busy - Ciclo) > 0) {
                    if ((int) (Rfp[IF_ID.IR.Rfuente2].busy - Ciclo) > 1) { // mux datos store en LS2
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1("RAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.Rfuente2);
                        //						fprintf(stdout, "DEBUG RAW fte2 Ciclo=%d fte2=%d Busy=%d\n",Ciclo,IF_ID.IR.Rfuente2,Rfp[IF_ID.IR.Rfuente2].busy);
                    };
                    break;

                default:
                    if ((int) (Rfp[IF_ID.IR.Rfuente1].busy - Ciclo) > 0) {
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1("RAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.Rfuente1);
                        //						fprintf(stdout, "DEBUG RAW Ciclo=%d fte1=%d Busy=%d\n",Ciclo,IF_ID.IR.Rfuente1,Rfp[IF_ID.IR.Rfuente1].busy);
                    };

                    if ((int) (Rfp[IF_ID.IR.Rfuente2].busy - Ciclo) > 0) {
                        IDstall = SI;
                        IFstall = SI;
                        RAW = SI;
                        write_log_1("RAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.Rfuente2);
                        //						fprintf(stdout, "DEBUG RAW fte2 Ciclo=%d fte2=%d Busy=%d\n",Ciclo,IF_ID.IR.Rfuente2,Rfp[IF_ID.IR.Rfuente2].busy);
                    };
                    break;
            };


        default:
            ;
    } /* end switch */
}

/***************************************************************
 *
 * Func: detectar_riesgos_WAW_FP
 *
 * Desc: Detecta los riesgos WAW, activando las señales IDstall, IFstall en su caso
 *
 **************************************************************/
void detectar_riesgos_WAW_FP(void) {
    switch (IF_ID.IR.codop) {


        case OP_FP_L_D:
        case OP_FP_L_S:
        case OP_FP_L_PS:
            /* Riesgos WAW */
            if (Rfp[IF_ID.IR.Rdestino].busy >= Ciclo + FP_LS_LAT) {
                IDstall = SI;
                IFstall = SI;
                WAW = SI;
                write_log_1("WAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.Rdestino);
            };
            break;

        case OP_FP_ADD_D:
        case OP_FP_SUB_D:
            /* Riesgos WAW */
            if (Rfp[IF_ID.IR.Rdestino].busy >= Ciclo + FP_ADD_LAT) {
                IDstall = SI;
                IFstall = SI;
                WAW = SI;
                write_log_1("WAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.Rdestino);
            };
            break;

        case OP_FP_MUL_D:
        case OP_FP_DIV_D:
            /* Riesgos WAW */
            if (Rfp[IF_ID.IR.Rdestino].busy >= Ciclo + FP_MUL_LAT) {
                IDstall = SI;
                IFstall = SI;
                WAW = SI;
                write_log_1("WAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.Rdestino);
            };
            break;

        case OP_FP_MOV_D:
        case OP_FP_MOV_S:
            /* Riesgos WAW */
            if (Rfp[IF_ID.IR.Rdestino].busy >= Ciclo) {
                IDstall = SI;
                IFstall = SI;
                WAW = SI;
                write_log_1("WAW, IDstall, IFstall", IF_ID.iPC, IF_ID.IR.Rdestino);
            };
            break;
            /* Las comparaciones no tienen WAW, todas escriben en FPSR */

    };
}

/***************************************************************
 *
 * Func: detectar_riesgos_WB_FP
 *
 * Desc: Detecta los riesgos WAW, activando las señales IDstall, IFstall en su caso
 *
 **************************************************************/
void detectar_riesgos_WB_FP(void) {
    int i;

    /* Riesgos estructurales WB, con cualquier registro  */
    switch (IF_ID.IR.codop) {

        case OP_FP_L_D:
        case OP_FP_L_S:
        case OP_FP_L_PS:
            /*
             for (i = 0; i < TAM_REGISTROS; i++) {
                if (Rfp[i].busy == Ciclo + FP_LS_LAT) {
                    IDstall = SI;
                    IFstall = SI;
                    estructural = SI;
                    write_log("Estructural_WB IDstall, IFstall", IF_ID.iPC);
                    //                        fprintf(stdout,"DEBUG FP Rdst %d Rfte1 %d Rfte2 %d\n",IF_ID.IR.Rdestino,IF_ID.IR.Rfuente1,IF_ID.IR.Rfuente2);

                    //                       fprintf (stdout, "DEBUG ID LS Ciclo=%d i=%d WB en %d  \n",Ciclo,i,Rfp[i].busy);
                };
            }; */
            i = 1 << (FP_LS_LAT + 1);
            if ((Rfp_pending_writes & i) == i) {
                IDstall = SI;
                IFstall = SI;
                estructural = SI;
                write_log("Estructural_WB IDstall, IFstall", IF_ID.iPC);
                fnDebug(NO, " WBconflict codop %d i=%d; Rfp_pending_writes=%x\n", IF_ID.IR.codop, i, Rfp_pending_writes);
            }
            break;

        case OP_FP_ADD_D:
        case OP_FP_SUB_D:
        case OP_FP_ADD_S:
        case OP_FP_SUB_S:
        case OP_FP_ADD_PS:
        case OP_FP_SUB_PS:
            //				fprintf (stdout, "DEBUG ID FP_ADD %d \n",Ciclo);
            /*
             for (i = 0; i < TAM_REGISTROS; i++) {
                //					fprintf (stdout, "DEBUG ID FP_ADD  i=%d busy=%d \n",i,Rfp[i].busy);
                if (Rfp[i].busy == Ciclo + FP_ADD_LAT) {
                    IDstall = SI;
                    IFstall = SI;
                    estructural = SI;
                    write_log("Estructural_WB IDstall, IFstall", IF_ID.iPC);
                };
            };*/
            //				fprintf (stdout, "DEBUG ID FP_ADD %d \n",Ciclo);
            i = 1 << (FP_ADD_LAT + 1);
            if ((Rfp_pending_writes & i) == i) {
                IDstall = SI;
                IFstall = SI;
                estructural = SI;
                write_log("Estructural_WB IDstall, IFstall", IF_ID.iPC);
                fnDebug(NO, " WBconflict codop %d i=%d; Rfp_pending_writes=%x\n", IF_ID.IR.codop, i, Rfp_pending_writes);
            }
            break;

        case OP_FP_MUL_D:
        case OP_FP_DIV_D:
        case OP_FP_MUL_S:
        case OP_FP_DIV_S:
        case OP_FP_MUL_PS:
        case OP_FP_DIV_PS:
            /* for (i = 0; i < TAM_REGISTROS; i++) {
                if (Rfp[i].busy == Ciclo + FP_MUL_LAT) {
                    IDstall = SI;
                    IFstall = SI;
                    estructural = SI;
                    write_log("Estructural_WB IDstall, IFstall", IF_ID.iPC);
                    //    fprintf (stdout, "DEBUG ID LS %d \n",i,Ciclo);
                };
            };*/
            i = 1 << (FP_MUL_LAT + 1);
            if ((Rfp_pending_writes & i) == i) {
                IDstall = SI;
                IFstall = SI;
                estructural = SI;
                write_log("Estructural_WB IDstall, IFstall", IF_ID.iPC);
                fnDebug(NO, " WBconflict codop %d i=%d; Rfp_pending_writes=%x\n", IF_ID.IR.codop, i, Rfp_pending_writes);
            }
            break;

        case OP_FP_MOV_D:
        case OP_FP_MOV_S:

            //printf("DEBUG Instruccion %d\n",IF_ID.IR.codop);
            //for (i=0; i<TAM_REGISTROS; i++){
            // fprintf (stdout, "DEBUG ID Busy  i=%d busy=%d \n",i,Rfp[i].busy);
            //}

            /* for (i = 0; i < TAM_REGISTROS; i++) {
                if (Rfp[i].busy == (Ciclo)) {
                    IDstall = SI;
                    IFstall = SI;
                    estructural = SI;
                    write_log("Estructural_WB IDstall, IFstall", IF_ID.iPC);
                    //           fprintf(stdout,"DEBUG FP Rdst %d Rfte1 %d Rfte2 %d\n",IF_ID.IR.Rdestino,IF_ID.IR.Rfuente1,IF_ID.IR.Rfuente2);

                    //                       fprintf (stdout, "DEBUG ID LS Ciclo=%d i=%d WB en %d  \n",Ciclo,i,Rfp[i].busy);
                };
            };*/
            i = 1 << (0 + 1);
            if ((Rfp_pending_writes & i) == i) {
                IDstall = SI;
                IFstall = SI;
                estructural = SI;
                write_log("Estructural_WB IDstall, IFstall", IF_ID.iPC);
                fnDebug(NO, " WBconflict codop %d i=%d; Rfp_pending_writes=%x\n", IF_ID.IR.codop, i, Rfp_pending_writes);
            }
            break;


        default:
            /* Las comparaciones no tienen riesgos estructurales en WB, todas escriben en FPSR */

            ;

    };
}
