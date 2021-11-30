/***************************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politècnica de Valencia.
 *
 * Autor: Pedro López (plopez@disca.upv.es)
 *        Sergio Sáez (ssaez@disca.upv.es)
 *
 * Fichero: mips.c
 *
 * Descripción:
 *   Contiene la descripcion de las fases del MIPS
 *
 ****************************************************************************/


#define mips_C

/*** Includes **************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


/*** Definiciones Externas *************************************************/

#include "main.h"
#include "instrucciones.h"
#include "control.h"
#include "presentacion-html.h"
#include "mips_syscall.h"
#include "depurar.h"
#include "log.h"
#include "mips_int.h"
#include "mips_fp.h"
#include "memoria.h"
#include "registros.h"



/*** Variables Globales ****************************************************/

/*** Variables Locales *****************************************************/

/*** Funciones Locales *****************************************************/

/*** Funciones Exportables *************************************************/

/***************************************************************
 *
 * Func: fase_busqueda
 *
 * Desc: Implementa la fase IF
 *
 **************************************************************/
void fase_busqueda(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    init_instruc(PC, orden); /*** VISUALIZACIÓN ***/

    /* Busca la siguiente instrucción */

    if (en_region(mmem, PC, r_text)) {
        if (esta_alineada_dir_instruc(PC)) {
            IF_IDn.IR = lee_mem_instruc(mmem, PC); /* Busca la siguiente instrucción */
        } else {
            /* Dirección de acceso a memoria de instrucciones desalineada */
            fprintf(stderr, "\nAtención (IF): acceso desalineado al segmento de instrucciones Ciclo=%u PC=%"PRId64"\n", Ciclo + 1, PC);
        }
    } else {
        /*** Excepción en el acceso a la memoria de instrucciones */
        fprintf(stderr, "\nAtención (IF): acceso fuera del segmento de instrucciones Ciclo=%u PC=%"PRId64"\n", Ciclo + 1, PC);

    } /* endif */

    IF_IDn.NPC = PC + 4;

    IF_IDn.iPC = PC; /* Visualización */
    IF_IDn.orden = orden;

    SaltoEfectivo = NO;

    //sprintf(_msg, "IF Ciclo %d Instruccion %d Cond %s Dest %d\n", Ciclo, EX_MEM.IR.codop, (EX_MEM.cond ? "true" : "false"), EX_MEM.ALUout);
    //DEBUG(__func__);


    /* riesgos de control */
    switch (solucion_riesgos_control) {
        case ds3:
            if (EX_MEM.cond) {
                                SaltoEfectivo = SI;
                PCn = EX_MEM.ALUout;
                            } else {
                PCn = PC + 4;
            }
            break;

        case ds2:
                        break;

        case ds1:
                        break;

        case pnt3:
            
              /* INSERTAR CÓDIGO */

            break;

        case pnt2:
                        break;

        case pnt1:
                        break;

        case stall3:
            if (EX_MEM.cond) {
                                SaltoEfectivo = SI;
                PCn = EX_MEM.ALUout;
                IFnop = SI;
                write_log("IFnop", EX_MEM.iPC);
                write_log("Salto Efectivo", EX_MEM.iPC);
            } else {
                PCn = PC + 4;
            }
            break;

        case stall2:
                        break;

        case stall1:
                        break;

        default:
            ;
    } /* end switch */

} /* end fase_busqueda */

/***************************************************************
 *
 * Func: fase_decodificacion
 *
 * Desc: Implementa la fase ID
 *
 **************************************************************/
int fase_decodificacion() {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    word i;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WAW = NO;
    RAW = NO;
    estructural = NO;

    MEMaIDcomp_i = NO;
    MEMaIDcomp_s = NO;

    fnDebug(NO, "Ciclo %d Instruccion %d\n", Ciclo, IF_ID.IR.codop);


    if (inst_excepcion(IF_ID.IR.codop)) {
        IFstall = SI;
        write_log("IFstall", IF_ID.iPC);
    }



    if inst_int(IF_ID.IR.codop) {
        /* Instrucción entera */

        detectar_riesgos_control();
        detectar_riesgos_datos();

        if latencia_salto1(solucion_riesgos_control) {
            if (IDstall == NO) {
                /* Condicion y direccion de salto */
                ID.compin1 = mux_COMPidsup(Rint[IF_ID.IR.Rfuente1].valor, EX_MEM.ALUout);
                ID.compin2 = mux_COMPidinf(Rint[IF_ID.IR.Rfuente2].valor, EX_MEM.ALUout);
                ID.cond_out = operacion_COMP(IF_ID.IR.codop, ID.compin1, ID.compin2);

                if inst_jr(IF_ID.IR.codop) {
                    ID.dir = ID.compin1; // Del mux, por si tiene dependencia
                } else {
                    ID.dir = IF_ID.NPC + (word) IF_ID.IR.inmediato;

                }
            }
        }



        /* alimentar el resto de operadores */
        pasar_a_FP_LS_NOP();
        pasar_a_FP_A_NOP();
        pasar_a_FP_M_NOP();
        pasar_a_FP_C_NOP();
        pasar_a_ID_WB_NOP();

        // Esta parte solo sería necesaria si el banco de registros entero lo pudieran escribir
        // dos instrucciones enteras simultáneamente.
        //
        // Se producirá una escritura en 3 ciclos
        //if inst_int_to_Rint(IF_ID.IR.codop) {
        if (!inst_salto(IF_ID.IR.codop)) {
            //Rint[IF_ID.IR.rd].busy = Ciclo + 3;
            Rint_pending_writes = Rint_pending_writes | (1 << 3);
        }


        /* leer operandos */
        ID_EXn.Ra = Rint[IF_ID.IR.Rfuente1].valor;
        ID_EXn.Rb = Rint[IF_ID.IR.Rfuente2].valor;
        ID_EXn.Imm = (dword) IF_ID.IR.inmediato;

        /* propagar datos */
        ID_EXn.NPC = IF_ID.NPC;
        ID_EXn.IR = IF_ID.IR;

        ID_EXn.iPC = IF_ID.iPC;
        ID_EXn.orden = IF_ID.orden;

        /* si ajustar duracion y salto con latencia 1, pasar nop a EX */
        /* solo aplica a la unidad entera en modo multiciclo */
        //if (AJUSTAR_INT && (hay_float || hay_double) && inst_salto(IF_ID.IR.codop) && latencia_salto1(solucion_riesgos_control)) {
        //    pasar_a_EX_NOP();
        //}

    } else {
        /* Instrucción multiciclo */


        detectar_riesgos_RAW_FP();

        detectar_riesgos_WAW_FP();

        detectar_riesgos_WB_FP();

        for (i = 0; i < TAM_REGISTROS; i++) {
            // fprintf (stdout, "DEBUG ID Busy  i=%d busy=%d \n",i,Rfp[i].busy);
        }

        /* Si hay algun problema, se habra activado IDstall */
        /* A diferencia de la unidad de 5 etapas, no podemos dejar que la instrucción
         * contnúe, ya que contaminaría el campo busy del Rfp */
        if (IDstall == NO) {

            switch (IF_ID.IR.codop) {

                case OP_FP_L_D:
                case OP_FP_L_S:
                case OP_FP_L_PS:
                    ID_LSn.Ra = Rint[IF_ID.IR.Rfuente1].valor;
                    ID_LSn.Rb = Rint[IF_ID.IR.Rfuente2].valor;
                    ID_LSn.Imm = (dword) IF_ID.IR.inmediato;
                    //ID_LSn.Fa = Rfp[IF_ID.IR.Rfuente1].valor;
                    //ID_LSn.Fb = Rfp[IF_ID.IR.Rfuente2].valor;

                    Rfp[IF_ID.IR.Rdestino].busy = Ciclo + FP_LS_LAT;
                    Rfp_pending_writes = Rfp_pending_writes | (1 << (FP_LS_LAT + 1));

                    ID_LSn.NPC = IF_ID.NPC;
                    ID_LSn.IR = IF_ID.IR;

                    ID_LSn.iPC = IF_ID.iPC;
                    ID_LSn.orden = IF_ID.orden;

                    //pasar_a_FP_LS_NOP();
                    pasar_a_FP_A_NOP();
                    pasar_a_FP_M_NOP();
                    pasar_a_FP_C_NOP();
                    pasar_a_EX_NOP();
                    pasar_a_ID_WB_NOP();

                    break;

                case OP_FP_S_D:
                case OP_FP_S_S:
                case OP_FP_S_PS:
                    ID_LSn.Ra = Rint[IF_ID.IR.Rfuente1].valor;
                    ID_LSn.Rb = Rint[IF_ID.IR.Rfuente2].valor;
                    ID_LSn.Imm = (dword) IF_ID.IR.inmediato;
                    ID_LSn.Fa = Rfp[IF_ID.IR.Rfuente1].valor;
                    ID_LSn.Fb = Rfp[IF_ID.IR.Rfuente2].valor;

                    ID_LSn.NPC = IF_ID.NPC;
                    ID_LSn.IR = IF_ID.IR;

                    ID_LSn.iPC = IF_ID.iPC;
                    ID_LSn.orden = IF_ID.orden;

                    //pasar_a_FP_LS_NOP();
                    pasar_a_FP_A_NOP();
                    pasar_a_FP_M_NOP();
                    pasar_a_FP_C_NOP();
                    pasar_a_EX_NOP();
                    pasar_a_ID_WB_NOP();

                    break;

                case OP_FP_ADD_D:
                case OP_FP_SUB_D:
                case OP_FP_ADD_S:
                case OP_FP_SUB_S:
                case OP_FP_ADD_PS:
                case OP_FP_SUB_PS:

                    ID_An.Fa = Rfp[IF_ID.IR.Rfuente1].valor;
                    ID_An.Fb = Rfp[IF_ID.IR.Rfuente2].valor;

                    Rfp[IF_ID.IR.Rdestino].busy = Ciclo + FP_ADD_LAT;
                    Rfp_pending_writes = Rfp_pending_writes | (1 << (FP_ADD_LAT + 1));

                    ID_An.NPC = IF_ID.NPC;
                    ID_An.IR = IF_ID.IR;

                    ID_An.iPC = IF_ID.iPC;
                    ID_An.orden = IF_ID.orden;

                    pasar_a_FP_LS_NOP();
                    // pasar_a_FP_A_NOP();
                    pasar_a_FP_M_NOP();
                    pasar_a_FP_C_NOP();
                    pasar_a_EX_NOP();
                    pasar_a_ID_WB_NOP();

                    break;

                case OP_FP_MUL_D:
                case OP_FP_DIV_D:
                case OP_FP_MUL_S:
                case OP_FP_DIV_S:
                case OP_FP_MUL_PS:
                case OP_FP_DIV_PS:

                    ID_Mn.Fa = Rfp[IF_ID.IR.Rfuente1].valor;
                    ID_Mn.Fb = Rfp[IF_ID.IR.Rfuente2].valor;

                    Rfp[IF_ID.IR.Rdestino].busy = Ciclo + FP_MUL_LAT;
                    //					fprintf (stdout, "DEBUG ID FP_MUL escribira en reg %d en el ciclo %d \n",IF_ID.IR.Rdestino,Rfp[IF_ID.IR.Rdestino].busy);
                    Rfp_pending_writes = Rfp_pending_writes | (1 << (FP_MUL_LAT + 1));

                    ID_Mn.NPC = IF_ID.NPC;
                    ID_Mn.IR = IF_ID.IR;

                    ID_Mn.iPC = IF_ID.iPC;
                    ID_Mn.orden = IF_ID.orden;

                    pasar_a_FP_LS_NOP();
                    pasar_a_FP_A_NOP();
                    // pasar_a_FP_M_NOP();
                    pasar_a_FP_C_NOP();
                    pasar_a_EX_NOP();
                    pasar_a_ID_WB_NOP();

                    break;

                case OP_FP_EQ_D:
                case OP_FP_NE_D:
                case OP_FP_LT_D:
                case OP_FP_LE_D:
                case OP_FP_GT_D:
                case OP_FP_GE_D:
                case OP_FP_EQ_S:
                case OP_FP_NE_S:
                case OP_FP_LT_S:
                case OP_FP_LE_S:
                case OP_FP_GT_S:
                case OP_FP_GE_S:

                    ID_Cn.Fa = Rfp[IF_ID.IR.Rfuente1].valor;
                    ID_Cn.Fb = Rfp[IF_ID.IR.Rfuente2].valor;

                    Rfp[IF_ID.IR.Rdestino].busy = Ciclo + FP_CMP_LAT;
                    // Las comparaciones escriben en el FPSR.
                    // Todas duran lo mismo. No hay riesgos estructurales
                    //Rfp_pending_writes = Rfp_pending_writes | (1 << (FP_CMP_LAT + 1));
                    //         fprintf (stdout, "DEBUG ID FP_CMP escribira en reg %d en el ciclo %d \n",IF_ID.IR.Rdestino,Rfp[IF_ID.IR.Rdestino].busy);

                    ID_Cn.NPC = IF_ID.NPC;
                    ID_Cn.IR = IF_ID.IR;

                    ID_Cn.iPC = IF_ID.iPC;
                    ID_Cn.orden = IF_ID.orden;


                    pasar_a_FP_LS_NOP();
                    pasar_a_FP_A_NOP();
                    pasar_a_FP_M_NOP();
                    // pasar_a_FP_C_NOP();
                    pasar_a_EX_NOP();
                    pasar_a_ID_WB_NOP();

                    break;

                case OP_FP_MOV_D:
                case OP_FP_MOV_S:

                    ID_WBn.IR = IF_ID.IR;
                    ID_WBn.iPC = IF_ID.iPC;
                    ID_WBn.orden = IF_ID.orden;

                    // La ejecutamos ya
                    ID_WBn.fpALUout = Rfp[IF_ID.IR.Rfuente1].valor;
                    Rfp[IF_ID.IR.Rdestino].busy = Ciclo;
                    Rfp_pending_writes = Rfp_pending_writes | (1 << (0 + 1));

                    pasar_a_FP_LS_NOP();
                    pasar_a_FP_A_NOP();
                    pasar_a_FP_M_NOP();
                    pasar_a_FP_C_NOP();
                    pasar_a_EX_NOP();
                    // pasar_a_ID_WB_NOP();

                    break;

                default:
                    fnError("Instrucción %d no implementada\n", IF_ID.IR.codop)
                            // fprintf(stderr,
                            //         "ERROR (%d %d): Instrucción no implementada\n",
                            //         __LINE__, IF_ID.IR.codop);
                            // exit(1);

            };

        }
    };

    /*
    fprintf (stdout, "DEBUG ID: busy");
    for (i=0; i<TAM_REGISTROS; i++){
            fprintf (stdout, "%3d ",Rfp[i].busy);
    }
    fprintf (stdout, "\n");
     */


    return (0);

} /* end fase_decodificacion */

/***************************************************************
 *
 * Func: fase_ejecucion
 *
 * Desc: Implementa la fase EX
 *
 **************************************************************/
void fase_ejecucion(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    if (inst_excepcion(ID_EX.IR.codop)) {
        IFstall = SI;
        write_log("IFstall", ID_EX.iPC);
    }

    /* riesgos de control */

    switch (solucion_riesgos_control) {
        case stall3:
        case stall2:
            if (inst_salto(ID_EX.IR.codop)) {

                IFstall = SI;
                write_log("IFstall", ID_EX.iPC);
            }
            break;

        default:
            ;
    } /* end switch */


    /* multiplexores */

    EX.aluin1 = mux_ALUsup(ID_EX.NPC, ID_EX.Ra, EX_MEM.ALUout, WB.data);
    EX.aluin2 = mux_ALUinf(ID_EX.Rb, ID_EX.Imm, EX_MEM.ALUout, WB.data);


    /* hacemos la operacion */

    EX.aluout = operacion_ALU(ID_EX.IR.codop, EX.aluin1, EX.aluin2);
    EX_MEMn.ALUout = EX.aluout;

    /* saltos */

    EX.compin1 = mux_COMPsup(ID_EX.Ra, EX_MEM.ALUout, WB.data, CMP_WB.ALUout);
    EX.compin2 = mux_COMPinf(ID_EX.Rb, EX_MEM.ALUout, WB.data);
    EX.cond_out = operacion_COMP(ID_EX.IR.codop, EX.compin1, EX.compin2);
    EX_MEMn.cond = EX.cond_out;

    /* propagar datos */

    EX.data = mux_EXmem(ID_EX.Rb, WB.data); /* Para SW */
    EX_MEMn.data = EX.data;

    EX_MEMn.IR = ID_EX.IR;

    EX_MEMn.iPC = ID_EX.iPC;
    EX_MEMn.orden = ID_EX.orden;

    /* si ajustar duracion y salto con latencia 2, pasar nop a MEM */
    /* solo aplica a la unidad entera en modo multicilo */
    //if (AJUSTAR_INT && (hay_float || hay_double) && inst_salto(ID_EX.IR.codop) && latencia_salto2(solucion_riesgos_control)) {
    //    pasar_a_MEM_NOP();
    //}


} /* end fase_ejecucion */

/***************************************************************
 *
 * Func: fase_memoria
 *
 * Desc: Implementa la fase MEM
 *
 **************************************************************/
void fase_memoria(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword data;
    valor_t datos_mem;
    boolean dirty;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WBaMEM = NO;

    if (inst_excepcion(EX_MEM.IR.codop)) {
        IFstall = SI;
        write_log("IFstall", EX_MEM.iPC);
    }

    /* riesgos de control */

    switch (solucion_riesgos_control) {
        case stall3:
            if (inst_salto(EX_MEM.IR.codop)) {
                IFstall = SI;
                write_log("IFstall", EX_MEM.iPC);
            }
            break;

        default:
            ;
    } /* end switch */



    /* multiplexores */
    data = mux_MEMmem(EX_MEM.data, WB.data); /* Para SW */


    /* acceso a memoria */

    switch (EX_MEM.IR.codop) {
        case OP_LD:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_dword, &dirty);
            MEM_WBn.MEMout = datos_mem.int_d;
            break;
        case OP_SD:
            MEM.dir = EX_MEM.ALUout;
            datos_mem.int_d = data;
            escribe_mem_datos(mmem, MEM.dir, datos_mem, t_dword, SI);
            break;
        case OP_LW:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_word, &dirty);
            MEM_WBn.MEMout = datos_mem.int_w;
            break;
        case OP_LWU:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_word, &dirty);
            MEM_WBn.MEMout = 0xFFFFFFFF & datos_mem.int_w;
            break;
        case OP_SW:
            MEM.dir = EX_MEM.ALUout;
            datos_mem.int_w = data;
            escribe_mem_datos(mmem, MEM.dir, datos_mem, t_word, SI);
            break;
        case OP_LH:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_half, &dirty);
            MEM_WBn.MEMout = datos_mem.int_h;
            break;
        case OP_LHU:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_half, &dirty);
            MEM_WBn.MEMout = 0xFFFF & datos_mem.int_h;
            break;
        case OP_SH:
            MEM.dir = EX_MEM.ALUout;
            datos_mem.int_h = data;
            escribe_mem_datos(mmem, MEM.dir, datos_mem, t_half, SI);
            break;
        case OP_LB:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_byte, &dirty);
            MEM_WBn.MEMout = datos_mem.int_b;
            break;
        case OP_LBU:
            MEM.dir = EX_MEM.ALUout;
            datos_mem = lee_mem_datos(mmem, MEM.dir, t_byte, &dirty);
            MEM_WBn.MEMout = 0xFF & datos_mem.int_b;
            break;
        case OP_SB:
            MEM.dir = EX_MEM.ALUout;
            datos_mem.int_b = data;
            escribe_mem_datos(mmem, MEM.dir, datos_mem, t_byte, SI);
            break;
        default:
            MEM.dir = 0;
            MEM_WBn.MEMout = 0;
            ;
    } /* endswitch */


    /* propagar datos */

    MEM_WBn.ALUout = EX_MEM.ALUout;
    MEM_WBn.IR = EX_MEM.IR;

    MEM_WBn.iPC = EX_MEM.iPC;
    MEM_WBn.orden = EX_MEM.orden;

    /* si ajustar duracion y store o salto, pasar nop a WB */
    /* solo aplica a la unidad entera en modo multicilo */
    //if (AJUSTAR_INT && (hay_float || hay_double) && (inst_store_int(EX_MEM.IR.codop) || inst_salto(EX_MEM.IR.codop))) {
    if (AJUSTAR_INT && (hay_float || hay_double) && inst_store_int(EX_MEM.IR.codop)) {
        pasar_a_WB_NOP();
    }

} /* end fase_memoria */

/***************************************************************
 *
 * Func: fase_escritura
 *
 * Desc: Implementa la fase 'WB'
 *
 **************************************************************/
int fase_escritura(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    boolean final,
            libre;
    word i;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    sprintf(CONSOLA, "%s", "");
    sprintf(LOG, "%s", "");

    /* Notese que no se simula el multiplexor que selecciona los bits
       adecuados del IR para seleccionar el registro destino. Simplemente
       se supone que la variable Rdestino ya contiene el valor correcto */

    if (inst_excepcion(MEM_WB.IR.codop)) {
        IFstall = SI;
        write_log("IFstall", MEM_WB.iPC);
    }

    if (MEM_WB.iPC != -1) {
        //Instrucciones++;
        estat.instrucciones++;
        estat.enteras++;
    }

    switch (MEM_WB.IR.codop) {
        case OP_LD:
            if (MEM_WB.IR.Rdestino != 0) {
                escribe_int_reg(MEM_WB.IR.Rdestino, MEM_WB.MEMout, t_dword, SI);
                WB.data = MEM_WB.MEMout;
            }
            break;
        case OP_LW:
        case OP_LWU:

            if (MEM_WB.IR.Rdestino != 0) {
                escribe_int_reg(MEM_WB.IR.Rdestino, MEM_WB.MEMout, t_word, SI);
                WB.data = MEM_WB.MEMout;
            }
            break;
        case OP_LH:
        case OP_LHU:
            if (MEM_WB.IR.Rdestino != 0) {
                escribe_int_reg(MEM_WB.IR.Rdestino, MEM_WB.MEMout, t_half, SI);
                WB.data = MEM_WB.MEMout;
            }
            break;
        case OP_LB:
        case OP_LBU:
            if (MEM_WB.IR.Rdestino != 0) {
                escribe_int_reg(MEM_WB.IR.Rdestino, MEM_WB.MEMout, t_byte, SI);
                WB.data = MEM_WB.MEMout;
            }
            break;

        case OP_NOP:
        case OP_SD:
        case OP_SW:
        case OP_SH:
        case OP_SB:
        case OP_BEQZ:
        case OP_BNEZ:
        case OP_BNE:
        case OP_BEQ:
        case OP_J:
        case OP_JAL:
        case OP_JR:
        case OP_JALR:
            break;
        case OP_TRAP:
            SIG_TRAP0 = SI;
            break;
        case OP_SYSCALL:
            SIG_SYSCALL = SI;
            break;

        default:
            if (MEM_WB.IR.Rdestino != 0) {
                escribe_int_reg(MEM_WB.IR.Rdestino, MEM_WB.ALUout, t_dword, SI);
                WB.data = MEM_WB.ALUout;
            }
            break;


    } /* endswitch */

    final = NO;

    if (SIG_TRAP0) {
        final = SI;
        for (i = 0; i < TAM_REGISTROS; i++) {
            final = final && (Rfp[i].busy == 0);
        };
    };

    if (SIG_SYSCALL) {
        libre = SI;
        for (i = 0; i < TAM_REGISTROS; i++) {
            libre = libre && (Rfp[i].busy == 0);
        }
        if (libre) {
            SIG_SYSCALL = NO;
            final = process_syscall();
        }
    }

    if (final) {
        fprintf(stderr, "\nCiclo %u: El programa está terminando normalmente\n", Ciclo + 1);
    }
    return (final);

} /* end fase_escritura */

/***************************************************************
 *
 * Func: fase_escritura_FP
 *
 * Desc: Implementa la fase 'WB'
 *
 **************************************************************/
void fase_escritura_FP(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/



    /* Selecciona quién pasa a FP WB */
    FP_WB = mux_FP_WB(LS_WB, ADD_WB, MUL_WB, ID_WB);


    //    fprintf(stdout,"DEBUG FP WB %d\n",FP_WB.orden);

    if (FP_WB.iPC != -1) {
        //Instrucciones++;
        estat.instrucciones++;
        estat.multiciclo++;
    }


    switch (FP_WB.IR.codop) {
        case OP_FP_L_D:
            escribe_fp_reg(FP_WB.IR.Rdestino, FP_WB.fpALUout, t_double, SI);
            if (Ciclo == Rfp[FP_WB.IR.Rdestino].busy + 1) { /* Para resolver reutilizacion de registros */
                Rfp[FP_WB.IR.Rdestino].busy = 0;

            } else {
                //		fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.Rdestino].busy);
            }
            break;

        case OP_FP_L_S:
            escribe_fp_reg(FP_WB.IR.Rdestino, FP_WB.fpALUout, t_float, SI);
            if (Ciclo == Rfp[FP_WB.IR.Rdestino].busy + 1) { /* Para resolver reutilizacion de registros */
                Rfp[FP_WB.IR.Rdestino].busy = 0;

            } else {
                //		fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.Rdestino].busy);
            }
            break;
        case OP_FP_L_PS:
            escribe_fp_reg(FP_WB.IR.Rdestino, FP_WB.fpALUout, t_float_ps, SI);
            if (Ciclo == Rfp[FP_WB.IR.Rdestino].busy + 1) { /* Para resolver reutilizacion de registros */
                Rfp[FP_WB.IR.Rdestino].busy = 0;

            } else {
                //		fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.Rdestino].busy);
            }
            break;
        case OP_FP_ADD_D:
        case OP_FP_SUB_D:
        case OP_FP_MUL_D:
        case OP_FP_DIV_D:
            estat.flops++;
            escribe_fp_reg(FP_WB.IR.Rdestino, FP_WB.fpALUout, t_double, SI);
            if (Ciclo == Rfp[FP_WB.IR.Rdestino].busy + 1) { /* Para resolver reutilizacion de registros */
                Rfp[FP_WB.IR.Rdestino].busy = 0;

            } else {
                //			fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.Rdestino].busy);
            }
            break;
        case OP_FP_ADD_S:
        case OP_FP_SUB_S:
        case OP_FP_MUL_S:
        case OP_FP_DIV_S:
            estat.flops++;
            escribe_fp_reg(FP_WB.IR.Rdestino, FP_WB.fpALUout, t_float, SI);
            if (Ciclo == Rfp[FP_WB.IR.Rdestino].busy + 1) { /* Para resolver reutilizacion de registros */
                Rfp[FP_WB.IR.Rdestino].busy = 0;

            } else {
                //			fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.Rdestino].busy);
            }
            break;

        case OP_FP_ADD_PS:
        case OP_FP_SUB_PS:
        case OP_FP_MUL_PS:
        case OP_FP_DIV_PS:
            estat.flops++;
            escribe_fp_reg(FP_WB.IR.Rdestino, FP_WB.fpALUout, t_float_ps, SI);
            if (Ciclo == Rfp[FP_WB.IR.Rdestino].busy + 1) { /* Para resolver reutilizacion de registros */
                Rfp[FP_WB.IR.Rdestino].busy = 0;
            } else {
                //			fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.Rdestino].busy);
            }
            break;
        case OP_FP_MOV_D:
            escribe_fp_reg(FP_WB.IR.Rdestino, FP_WB.fpALUout, t_double, SI);
            if (Ciclo == Rfp[FP_WB.IR.Rdestino].busy + 1) { /* Para resolver reutilizacion de registros */
                Rfp[FP_WB.IR.Rdestino].busy = 0;
            } else {
                //			fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.Rdestino].busy);
            }
            break;

        case OP_FP_MOV_S:
            escribe_fp_reg(FP_WB.IR.Rdestino, FP_WB.fpALUout, t_float, SI);
            if (Ciclo == Rfp[FP_WB.IR.Rdestino].busy + 1) { /* Para resolver reutilizacion de registros */
                Rfp[FP_WB.IR.Rdestino].busy = 0;
            } else {
                //			fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.Rdestino].busy);
            }
            break;
        case OP_NOP:
        case OP_FP_S_D:
        case OP_FP_S_S:
        case OP_FP_S_PS:
            break;
        default:
            fnError("La instrucción %d no debería estar en FP WB\n", FP_WB.IR.codop)
                    //fprintf(stderr,
                    //        "ERROR (%d %d): Esta instrucción no deberia estar en FP WB\n",
                    //        __LINE__, FP_WB.IR.codop);
                    //exit(1);

    } /* endswitch */



    //fprintf (stdout, "DEBUG WB: busy");
    //for (i=0; i<TAM_REGISTROS; i++){
    //	fprintf (stdout, "%3 d ",Rfp[i].busy);
    //}
    //fprintf (stdout, "\n");



} /* end fase_escritura */

/***************************************************************
 *
 * Func: fase_escritura
 *
 * Desc: Implementa la fase 'WB'
 *
 **************************************************************/
void fase_escritura_FPSR(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    //boolean		final;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /* Notese que no se simula el multiplexor que selecciona los bits
         adecuados del IR para seleccionar el registro destino. Simplemente
         se supone que la variable Rdestino ya contiene el valor correcto */


    if (CMP_WB.iPC != -1) {
        //Instrucciones++;
        estat.instrucciones++;
        estat.multiciclo++;
    }

    switch (CMP_WB.IR.codop) {
        case OP_FP_EQ_D:
        case OP_FP_NE_D:
        case OP_FP_LT_D:
        case OP_FP_LE_D:
        case OP_FP_GT_D:
        case OP_FP_GE_D:
            estat.flops++;
            escribe_int_reg(CMP_WB.IR.Rdestino, CMP_WB.ALUout, t_dword, SI);

            if (Ciclo == Rfp[CMP_WB.IR.Rdestino].busy + 1) { /* Para resolver reutilizacion de registros */
                Rfp[CMP_WB.IR.Rdestino].busy = 0;
            } else {
                //			fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.Rdestino].busy);
            }
            break;

        case OP_FP_EQ_S:
        case OP_FP_NE_S:
        case OP_FP_LT_S:
        case OP_FP_LE_S:
        case OP_FP_GT_S:
        case OP_FP_GE_S:
            estat.flops++;
            escribe_int_reg(CMP_WB.IR.Rdestino, CMP_WB.ALUout, t_dword, SI);
            if (Ciclo == Rfp[CMP_WB.IR.Rdestino].busy + 1) { /* Para resolver reutilizacion de registros */
                Rfp[CMP_WB.IR.Rdestino].busy = 0;
            } else {
                //			fprintf (stdout, "DEBUG FP_WB %d %d \n",Ciclo,Rfp[FP_WB.IR.Rdestino].busy);
            }

            break;
        case OP_NOP:
            break;
        default:
            fnError("La instrucción %d no debería estar en FPSR WB\n", CMP_WB.IR.codop)
                    //fprintf(stderr,
                    //        "ERROR (%d %d): Esta instrucción no deberia estar en FPSR WB\n",
                    //        __LINE__, CMP_WB.IR.codop);
                    //exit(1);

    } /* endswitch */

    //    return (final);

} /* end fase_escritura */

/***************************************************************
 *
 * Func: impulso_reloj
 *
 * Desc: Actualiza los registros inter-etapa
 *
 **************************************************************/
void impulso_reloj() {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int i;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /* Si se ha recibido la TRAP #0 no buscamos nada mas */
    /*
    if (SIG_TRAP0) {
        IFstall = SI;
        //write_log("IFstall", MEM_WB.iPC);
        //IFnop = SI;
    };
     */

    /* IFnop se activa para abortar una instruccion. Si esta activado tb IFstall, la ignoramos */
    if (IFnop) {
        pasar_a_ID_NOP();
        IFnop = NO;
        IFstall = NO; // Por si estaba activada

    }

    if (!IFstall) {
        orden = orden + 1; /* VISUALIZACIÓN */
        PC = PCn;
    } else {
        if (SaltoEfectivo) {
            /* está activado IDstall por culpa de IDstall y hay un salto en este ciclo */
            /* No podemos perder el PC */
            orden = orden + 1;
            PC = PCn;
        };
        pasar_a_ID_NOP();
        IFstall = NO;
    }

    if (IDnop) {
        pasar_a_EX_NOP();

        pasar_a_FP_LS_NOP();
        pasar_a_FP_A_NOP();
        pasar_a_FP_C_NOP();
        pasar_a_FP_M_NOP();
        pasar_a_ID_WB_NOP();


        IDnop = NO;
        IDstall = NO; // Por si estaba activada

    }

    if (!IDstall) {
        IF_ID = IF_IDn;
    } else {
        pasar_a_EX_NOP();

        pasar_a_FP_LS_NOP();
        pasar_a_FP_A_NOP();
        pasar_a_FP_C_NOP();
        pasar_a_FP_M_NOP();
        pasar_a_ID_WB_NOP();

        IDstall = NO;
    }


    if (EXnop) {
        pasar_a_MEM_NOP();

        pasar_a_FP_LS2_NOP();
        pasar_a_FP_A2_NOP();
        pasar_a_FP_C2_NOP();
        pasar_a_FP_M2_NOP();

        EXnop = NO;
        EXstall = NO; // Por si estaba activada

    }

    if (!EXstall) {
        ID_EX = ID_EXn;
    } else {
        pasar_a_MEM_NOP();
        EXstall = NO;
    }



    EX_MEM = EX_MEMn;

    MEM_WB = MEM_WBn;

    ID_LS = ID_LSn;

    for (i = 2; i <= FP_LS_LAT; i++) {
        LS[i] = LSn[i];
    }

    ID_A = ID_An;

    for (i = 2; i <= FP_ADD_LAT; i++) {
        A[i] = An[i];
    }

    ID_C = ID_Cn;

    for (i = 2; i <= FP_CMP_LAT; i++) {
        C[i] = Cn[i];
    }

    ID_M = ID_Mn;

    for (i = 2; i <= FP_MUL_LAT; i++) {
        M[i] = Mn[i];
    }

    LS_WB = LS_WBn;
    ADD_WB = ADD_WBn;
    MUL_WB = MUL_WBn;

    FP_WB = FP_WBn;
    CMP_WB = CMP_WBn;

    ID_WB = ID_WBn;

    Rfp_pending_writes = Rfp_pending_writes >> 1;
    Rint_pending_writes = Rint_pending_writes >> 1;



} /* end impulso_reloj */
