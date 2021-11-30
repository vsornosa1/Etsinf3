/***************************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politècnica de Valencia.
 *
 * Autor: Sergio Sáez (ssaez@disca.upv.es)
 *	  Pedro López (plopez@gap.upv.es)
 *
 * Fichero: control.c
 *
 * Descripción:
 *   Contiene la funciones XXnop y las comprobaciones de campos válidos en el IR
 *
 ****************************************************************************/


/*** Includes **************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


/*** Definiciones Externas *************************************************/

#include "tipos.h"
#include "main.h"
#include "instrucciones.h"
#include "presentacion-html.h"


/*** Variables Globales ****************************************************/

/*** Variables Locales *****************************************************/

/*** Funciones Exportables *************************************************/

/***************************************************************
 *
 * Func: pasar_a_ID_NOP
 *
 * Desc: La fase IF entrega un NOP a la fase ID al pr�ximo ciclo
 *
 **************************************************************/
void pasar_a_ID_NOP(
        ) {

    //    IF_IDn.NPC = ;
    IF_IDn.IR.codop = OP_NOP;
    IF_IDn.IR.tipo = FormatoR;
    IF_IDn.IR.Rfuente1 = 0;
    IF_IDn.IR.Rfuente2 = 0;
    IF_IDn.IR.Rdestino = 0;
    IF_IDn.IR.inmediato = 0;
    IF_IDn.iPC = -1;
    IF_IDn.orden = -1;
} /* pasar_ID_NOP */

/***************************************************************
 *
 * Func: pasar_a_EX_NOP
 *
 * Desc: La fase ID entrega un NOP a la fase EX al pr�ximo ciclo
 *
 **************************************************************/
void pasar_a_EX_NOP(
        ) {
    word nmask;

    ID_EXn.Ra = 0;
    ID_EXn.Rb = 0;
    ID_EXn.Imm = 0;
    //    ID_EXn.NPC = ; no lo toco
    ID_EXn.IR.codop = OP_NOP;
    ID_EXn.IR.tipo = FormatoR;
    ID_EXn.IR.Rfuente1 = 0;
    ID_EXn.IR.Rfuente2 = 0;
    ID_EXn.IR.Rdestino = 0;
    ID_EXn.IR.inmediato = 0;
    ID_EXn.iPC = -1;
    ID_EXn.orden = -1;
    // Anular la escritura dentro de dos ciclos:
    // x = x & mask = ~(~x | ~masc) = ~(~x | nmask)
    nmask = 1 << 2;
    Rint_pending_writes = ~(~Rint_pending_writes | nmask);

} /* pasar_a_EX_NOP */

/***************************************************************
 *
 * Func: pasar_a_MEM_NOP
 *
 * Desc: La fase EX entrega un NOP a la fase MEM al pr�ximo ciclo
 *
 **************************************************************/
void pasar_a_MEM_NOP(
        ) {
    word nmask;

    EX_MEMn.ALUout = 0;
    EX_MEMn.data = 0;
    EX_MEMn.cond = 0;
    //    IF_IDn.NPC = ; no lo toco
    EX_MEMn.IR.codop = OP_NOP;
    EX_MEMn.IR.tipo = FormatoR;
    EX_MEMn.IR.Rfuente1 = 0;
    EX_MEMn.IR.Rfuente2 = 0;
    EX_MEMn.IR.Rdestino = 0;
    EX_MEMn.IR.inmediato = 0;
    EX_MEMn.iPC = -1;
    EX_MEMn.orden = -1;

    // Anular la escritura el próximo ciclo:
    // x = x & masc = ~(~x | ~masc)
    // Ej: x = x & 1..101 = ~(~x | ~1..101) = ~(~x | 0..010)
    nmask = 1 << 1;
    Rint_pending_writes = ~(~Rint_pending_writes | nmask);
} /* pasar_a_MEM_NOP */

/***************************************************************
 *
 * Func: pasar_a_WB_NOP
 *
 * Desc: La fase MEM entrega un NOP a la fase WB al pr�ximo ciclo
 *
 **************************************************************/
void pasar_a_WB_NOP(
        ) {


    MEM_WBn.ALUout = 0;
    MEM_WBn.MEMout = 0;
    //    IF_IDn.NPC = ; no lo toco
    MEM_WBn.IR.codop = OP_NOP;
    MEM_WBn.IR.tipo = FormatoR;
    MEM_WBn.IR.Rfuente1 = 0;
    MEM_WBn.IR.Rfuente2 = 0;
    MEM_WBn.IR.Rdestino = 0;
    MEM_WBn.IR.inmediato = 0;
    MEM_WBn.iPC = -1;
    MEM_WBn.orden = -1;
} /* pasar_a_MEM_NOP */

/***************************************************************
 *
 * Func: pasar_a_FP_LS_NOP
 *
 * Desc: La fase ID entrega un NOP a la fase EX al pr�ximo ciclo
 *
 **************************************************************/
void pasar_a_FP_LS_NOP(
        ) {

    ID_LSn.Fa.fp_d = 0;
    ID_LSn.Fb.fp_d = 0;
    //    ID_EXn.NPC = ; no lo toco
    ID_LSn.IR.codop = OP_NOP;
    ID_LSn.IR.tipo = FormatoR;
    ID_LSn.IR.Rfuente1 = 0;
    ID_LSn.IR.Rfuente2 = 0;
    ID_LSn.IR.Rdestino = 0;
    ID_LSn.IR.inmediato = 0;
    ID_LSn.iPC = -1;
    ID_LSn.orden = -1;
} /* pasar_a_EX_NOP */

/***************************************************************
 *
 * Func: pasar_a_FP_A_NOP
 *
 * Desc: La fase ID entrega un NOP a la fase EX al pr�ximo ciclo
 *
 **************************************************************/
void pasar_a_FP_A_NOP(
        ) {
    ID_An.Fa.fp_d = 0;
    ID_An.Fb.fp_d = 0;
    //    ID_EXn.NPC = ; no lo toco
    ID_An.IR.codop = OP_NOP;
    ID_An.IR.tipo = FormatoR;
    ID_An.IR.Rfuente1 = 0;
    ID_An.IR.Rfuente2 = 0;
    ID_An.IR.Rdestino = 0;
    ID_An.IR.inmediato = 0;
    ID_An.iPC = -1;
    ID_An.orden = -1;
} /* pasar_a_EX_NOP */

/***************************************************************
 *
 * Func: pasar_a_FP_M_NOP
 *
 * Desc: La fase ID entrega un NOP a la fase EX al pr�ximo ciclo
 *
 **************************************************************/
void pasar_a_FP_M_NOP(
        ) {
    ID_Mn.Fa.fp_d = 0;
    ID_Mn.Fb.fp_d = 0;
    //    ID_EXn.NPC = ; no lo toco
    ID_Mn.IR.codop = OP_NOP;
    ID_Mn.IR.tipo = FormatoR;
    ID_Mn.IR.Rfuente1 = 0;
    ID_Mn.IR.Rfuente2 = 0;
    ID_Mn.IR.Rdestino = 0;
    ID_Mn.IR.inmediato = 0;
    ID_Mn.iPC = -1;
    ID_Mn.orden = -1;
} /* pasar_a_EX_NOP */

/***************************************************************
 *
 * Func: pasar_a_FP_C_NOP
 *
 * Desc: La fase ID entrega un NOP a la fase EX al pr�ximo ciclo
 *
 **************************************************************/
void pasar_a_FP_C_NOP(
        ) {
    ID_Cn.Fa.fp_d = 0;
    ID_Cn.Fb.fp_d = 0;
    //    ID_EXn.NPC = ; no lo toco
    ID_Cn.IR.codop = OP_NOP;
    ID_Cn.IR.tipo = FormatoR;
    ID_Cn.IR.Rfuente1 = 0;
    ID_Cn.IR.Rfuente2 = 0;
    ID_Cn.IR.Rdestino = 0;
    ID_Cn.IR.inmediato = 0;
    ID_Cn.iPC = -1;
    ID_Cn.orden = -1;
} /* pasar_a_EX_NOP */

/***************************************************************
 *
 * Func: pasar_a_FP_LS2_NOP
 *
 * Desc: entrega un NOP
 *
 **************************************************************/
void pasar_a_FP_LS2_NOP(
        ) {
    LSn[2].fpALUout.fp_d = 0;
    LSn[2].Fa.fp_d = 0;
    LSn[2].Fb.fp_d = 0;
    LSn[2].address = 0;
    //    IF_IDn.NPC = ; no lo toco
    LSn[2].IR.codop = OP_NOP;
    LSn[2].IR.tipo = FormatoR;
    LSn[2].IR.Rfuente1 = 0;
    LSn[2].IR.Rfuente2 = 0;
    // Liberar registro, si es nuestro
    if (Rfp[LSn[2].IR.Rdestino].busy == Ciclo + FP_LS_LAT - 2) {
        Rfp[LSn[2].IR.Rdestino].busy = 0;
    }
    LSn[2].IR.Rdestino = 0;
    LSn[2].IR.inmediato = 0;
    LSn[2].iPC = -1;
    LSn[2].orden = -1;
} /* pasar_a_MEM_NOP */

/***************************************************************
 *
 * Func: pasar_a_FP_A2_NOP
 *
 * Desc: entrega un NOP
 *
 **************************************************************/
void pasar_a_FP_A2_NOP(
        ) {
    An[2].fpALUout.fp_d = 0;
    An[2].Fa.fp_d = 0;
    An[2].Fb.fp_d = 0;
    An[2].address = 0;
    //    IF_IDn.NPC = ; no lo toco
    An[2].IR.codop = OP_NOP;
    An[2].IR.tipo = FormatoR;
    An[2].IR.Rfuente1 = 0;
    An[2].IR.Rfuente2 = 0;
    // Liberar registro, si es nuestro
    if (Rfp[An[2].IR.Rdestino].busy == Ciclo + FP_ADD_LAT - 2) {
        Rfp[An[2].IR.Rdestino].busy = 0;
    }
    An[2].IR.Rdestino = 0;
    An[2].IR.inmediato = 0;
    An[2].iPC = -1;
    An[2].orden = -1;
} /* pasar_a_MEM_NOP */

/***************************************************************
 *
 * Func: pasar_a_FP_M2_NOP
 *
 * Desc:  entrega un NOP
 *
 **************************************************************/
void pasar_a_FP_M2_NOP(
        ) {
    Mn[2].fpALUout.fp_d = 0;
    Mn[2].Fa.fp_d = 0;
    Mn[2].Fb.fp_d = 0;
    Mn[2].address = 0;
    //    IF_IDn.NPC = ; no lo toco
    Mn[2].IR.codop = OP_NOP;
    Mn[2].IR.tipo = FormatoR;
    Mn[2].IR.Rfuente1 = 0;
    Mn[2].IR.Rfuente2 = 0;
    // Liberar registro, si es nuestro
    if (Rfp[Mn[2].IR.Rdestino].busy == Ciclo + FP_MUL_LAT - 2) {
        Rfp[Mn[2].IR.Rdestino].busy = 0;
    }

    Mn[2].IR.Rdestino = 0;
    Mn[2].IR.inmediato = 0;
    Mn[2].iPC = -1;
    Mn[2].orden = -1;
} /* pasar_a_MEM_NOP */

/***************************************************************
 *
 * Func: pasar_a_FP_C2_NOP
 *
 * Desc: entrega un NOP
 *
 **************************************************************/
void pasar_a_FP_C2_NOP(
        ) {
    Cn[2].fpALUout.fp_d = 0;
    Cn[2].Fa.fp_d = 0;
    Cn[2].Fb.fp_d = 0;
    Cn[2].address = 0;
    //    IF_IDn.NPC = ; no lo toco
    Cn[2].IR.codop = OP_NOP;
    Cn[2].IR.tipo = FormatoR;
    Cn[2].IR.Rfuente1 = 0;
    Cn[2].IR.Rfuente2 = 0;
    // Liberar registro, si es nuestro
    if (Rfp[Cn[2].IR.Rdestino].busy == Ciclo + FP_CMP_LAT - 2) {
        Rfp[Cn[2].IR.Rdestino].busy = 0;
    }
    Cn[2].IR.Rdestino = 0;
    Cn[2].IR.inmediato = 0;
    Cn[2].iPC = -1;
    Cn[2].orden = -1;
} /* pasar_a_MEM_NOP */

/***************************************************************
 *
 * Func: pasar_a_ID_WB_NOP
 *
 * Desc: Se entrega un NOP a la fase ID_WB al proximo ciclo
 *
 **************************************************************/
void pasar_a_ID_WB_NOP(
        ) {
    ID_WBn.fpALUout.fp_d = 0.0;
    ID_WBn.IR.codop = OP_NOP;
    ID_WBn.IR.tipo = FormatoR;
    ID_WBn.IR.Rfuente1 = 0;
    ID_WBn.IR.Rfuente2 = 0;
    ID_WBn.IR.Rdestino = 0;
    ID_WBn.IR.inmediato = 0;
    ID_WBn.iPC = -1;
    ID_WBn.orden = -1;
} /* pasar_a_ID_WB_NOP */

/***************************************************************
 *
 * Func: pasar_a_FP_LS_WB_NOP
 *
 * Desc: Se entrega un NOP a la WB al proximo ciclo. Para los stores
 *
 **************************************************************/
void pasar_a_FP_LS_WB_NOP(
        ) {
    LS_WBn.fpALUout.fp_d = 0.0;
    LS_WBn.IR.codop = OP_NOP;
    LS_WBn.IR.tipo = FormatoR;
    LS_WBn.IR.Rfuente1 = 0;
    LS_WBn.IR.Rfuente2 = 0;
    LS_WBn.IR.Rdestino = 0;
    LS_WBn.IR.inmediato = 0;
    LS_WBn.iPC = -1;
    LS_WBn.orden = -1;
} /* pasar_a_ID_WB_NOP */

/***************************************************************
 *
 * Func: hay_fuente1
 *
 * Desc: Indica si hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente1(instruccion_t inst) {
    return (
            (inst_int(inst.codop) &&
            (inst.Rfuente1 != 0) &&
            (inst.codop != OP_NOP) && (inst.codop != OP_TRAP) &&
            (inst.codop != OP_J) && (inst.codop != OP_JAL) &&
            (inst.codop != OP_LUI)) ||
            (inst_load_fp(inst.codop) || inst_store_fp(inst.codop))
            );
} /* hay_fuente1 */

/***************************************************************
 *
 * Func: hay_fuente2
 *
 * Desc: Indica si hay una instrucción que
 *       tiene Rfte2
 *
 **************************************************************/
boolean hay_fuente2(instruccion_t inst) {
    return (
            (inst_int(inst.codop) &&
            (inst.Rfuente2 != 0) &&
            (inst.codop != OP_NOP) && (inst.codop != OP_TRAP) &&
            (inst.codop != OP_J) && (inst.codop != OP_JAL) &&
            (inst.codop != OP_LUI)) &&
            ((inst.tipo == FormatoR) ||
            inst_store_int(inst.codop) ||
            (inst.codop = OP_BNE) || (inst.codop = OP_BEQ)
            )
            );
} /* hay_fuente2 */

/***************************************************************
 *
 * Func: hay_destino
 *
 * Desc: Indica si hay una instrucción que
 *       tiene Rdst
 *
 **************************************************************/
boolean hay_destino(instruccion_t inst) {
    return (
            (inst_int(inst.codop) &&
            (inst.Rdestino != 0) &&
            (inst.codop != OP_NOP) && (inst.codop != OP_TRAP) &&
            !inst_jump_int(inst.codop) &&
            !inst_branch(inst.codop) &&
            !inst_store_int(inst.codop)
            )
            );
} /* hay_destino */

/***************************************************************
 *
 * Func: hay_fuente1_ID
 *
 * Desc: Indica si en la fase ID hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente1_ID(
        ) {
    return (
            hay_fuente1(IF_ID.IR)
            );
} /* hay_fuente1_ID */

/***************************************************************
 *
 * Func: hay_fuente2_ID
 *
 * Desc: Indica si en la fase ID hay una instrucción que
 *       tiene Rfte2
 *
 **************************************************************/
boolean hay_fuente2_ID(
        ) {
    return (
            hay_fuente2(IF_ID.IR)
            );
} /* hay_fuente2_ID */

/***************************************************************
 *
 * Func: hay_fuente1_EX
 *
 * Desc: Indica si en la fase EX hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente1_EX(
        ) {
    return (
            hay_fuente1(ID_EX.IR)
            );
} /* hay_fuente1_EX */

/***************************************************************
 *
 * Func: hay_fuente2_EX
 *
 * Desc: Indica si en la fase EX hay una instrucción que
 *       tiene Rfte2
 *
 **************************************************************/
boolean hay_fuente2_EX(
        ) {
    return (
            hay_fuente2(ID_EX.IR)
            );
} /* hay_fuente2_EX */

/***************************************************************
 *
 * Func: hay_destino_EX
 *
 * Desc: Indica si en la fase EX hay una instrucción que
 *       tiene Rdst
 *
 **************************************************************/
boolean hay_destino_EX(
        ) {
    return (
            hay_destino(ID_EX.IR)
            );
} /* hay_destino_EX */

/***************************************************************
 *
 * Func: hay_destino_MEM
 *
 * Desc: Indica si en la fase MEM hay una instrucción que
 *       tiene Rdst
 *
 **************************************************************/
boolean hay_destino_MEM(
        ) {
    return (
            hay_destino(EX_MEM.IR)
            );
} /* hay_destino_MEM */

/***************************************************************
 *
 * Func: hay_destino_WB
 *
 * Desc: Indica si en la fase WB hay una instrucción que
 *       tiene Rdst
 *
 **************************************************************/
boolean hay_destino_WB(
        ) {
    return (
            hay_destino(MEM_WB.IR)
            );
} /* hay_destino_WB */

/***************************************************************
 *
 * Func: hay_fuente1_FP_ID
 *
 * Desc: Indica si en la fase ID hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente1_FP_ID(
        ) {
    return (
            inst_fp(IF_ID.IR.codop) &&
            (!inst_load_fp(IF_ID.IR.codop) &&
            !inst_store_fp(IF_ID.IR.codop))
            );
} /* hay_fuente1_ID */

/***************************************************************
 *
 * Func: hay_fuente2_ID
 *
 * Desc: Indica si en la fase ID hay una instrucción que
 *       tiene Rfte2
 *
 **************************************************************/
boolean hay_fuente2_FP_ID(
        ) {
    return (
            inst_fp(IF_ID.IR.codop) &&
            (IF_ID.IR.tipo == FormatoR)
            );
} /* hay_fuente2_ID */

/***************************************************************
 *
 * Func: hay_destino_FP_WB
 *
 * Desc: Indica si en la fase WB hay una instrucción que
 *       tiene Rdst
 *
 **************************************************************/
boolean hay_destino_FP_WB(
        ) {
    return (
            (inst_fp(FP_WB.IR.codop) &&
            (FP_WB.IR.codop != OP_NOP) &&
            !inst_store_fp(FP_WB.IR.codop))
            );
} /* hay_destino_WB */

/***************************************************************
 *
 * Func: hay_fuente1_A1
 *
 * Desc: Indica si en la fase EX hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente1_A1(
        ) {
    return (
            (ID_A.IR.codop != OP_NOP)
            );
} /* hay_fuente1_EX */

/***************************************************************
 *
 * Func: hay_fuente1_M1
 *
 * Desc: Indica si en la fase EX hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente1_M1(
        ) {
    return (
            (ID_M.IR.codop != OP_NOP)
            );
} /* hay_fuente1_EX */

/***************************************************************
 *
 * Func: hay_fuente1_C1
 *
 * Desc: Indica si en la fase EX hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente1_C1(
        ) {
    return (
            (ID_C.IR.codop != OP_NOP)
            );
} /* hay_fuente1_EX */

/***************************************************************
 *
 * Func: hay_fuente2_A1
 *
 * Desc: Indica si en la fase EX hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente2_A1(
        ) {
    return (
            (ID_A.IR.codop != OP_NOP)
            );
} /* hay_fuente1_EX */

/***************************************************************
 *
 * Func: hay_fuente2_M1
 *
 * Desc: Indica si en la fase EX hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente2_M1(
        ) {
    return (
            (ID_M.IR.codop != OP_NOP)
            );
} /* hay_fuente1_EX */

/***************************************************************
 *
 * Func: hay_fuente2_C1
 *
 * Desc: Indica si en la fase EX hay una instrucción que
 *       tiene Rfte1
 *
 **************************************************************/
boolean hay_fuente2_C1(
        ) {
    return (
            (ID_C.IR.codop != OP_NOP)
            );
} /* hay_fuente1_EX */

/***************************************************************
 *
 * Func: hay_fuente1_LS
 *
 * Desc: Indica si en la fase EX hay una instrucción que
 *       tiene Rfte2
 *
 **************************************************************/
boolean hay_fuente1_LS1(
        ) {
    return (
            (ID_LS.IR.codop != OP_NOP)
            );
} /* hay_fuente2_EX */

/***************************************************************
 *
 * Func: hay_fuente2_LS
 *
 * Desc: Indica si en la fase EX hay una instrucción que
 *       tiene Rfte2
 *
 **************************************************************/
boolean hay_fuente2_LS1(
        ) {
    return (
            (ID_LS.IR.codop != OP_NOP) &&
            inst_store_fp(ID_LS.IR.codop)
            );
} /* hay_fuente2_EX */

boolean hay_fuente2_LS2(
        ) {
    return (
            (LSn[2].IR.codop != OP_NOP) &&
            inst_store_fp(LSn[2].IR.codop)
            );
} /* hay_fuente2_LS2 */

