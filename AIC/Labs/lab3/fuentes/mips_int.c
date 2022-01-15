/***************************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politècnica de Valencia.
 *
 * Autor: Pedro López (plopez@disca.upv.es)
 *        Sergio Sáez (ssaez@disca.upv.es)
 *
 * Fichero: mips_int.c
 *
 * Descripción:
 *   Contiene la microarquitectura del MIPS 5 etapas+multiciclo
 *
 ****************************************************************************/


#define mips_int_C

/*** Includes **************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


/*** Definiciones Externas *************************************************/
#define lee_Rfte1 hay_fuente1
#define lee_Rfte2 hay_fuente2
#define escribe_Rdst hay_destino
#define es_load(i) inst_load(i.codop)


#include "main.h"
#include "instrucciones.h"
#include "depurar.h"
#include "control.h"
#include "mips_int.h"
#include "log.h"


/*** Variables Globales ****************************************************/

/*** Variables Locales *****************************************************/

/*** Funciones Locales *****************************************************/

/*** Funciones Exportables *************************************************/

/***************************************************************
 *
 * Func: operacion_ALU
 *
 * Desc: Realiza la operacion en el operador aritm�tico
 *
 **************************************************************/
dword operacion_ALU(ubyte codop, dword in1, dword in2) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result = 0;
    uint64_t aux1, aux2;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/


    switch (codop) {
        case OP_DADD:
        case OP_DADDI:
            result = in1 + in2;
            break;
        case OP_DSUB:
        case OP_DSUBI:
            result = in1 - in2;
            break;
        case OP_DMUL:
            result = in1 * in2;
            break;
        case OP_DDIV:
            result = in1 / in2;
            break;
        case OP_LUI:
            result = in2 << 16;
            break;
        case OP_AND:
        case OP_ANDI:
            result = in1 & in2;
            break;
        case OP_OR:
        case OP_ORI:
            result = in1 | in2;
            break;
        case OP_XOR:
        case OP_XORI:
            result = in1 ^ in2;
            break;
        case OP_DSRA:
        case OP_DSRAI:
            result = in1 >> (in2 & 0x1f);
            break;
        case OP_DSLL:
        case OP_DSLLI:
            result = in1 << (in2 & 0x1f);
            break;
        case OP_DSRL:
        case OP_DSRLI:
            aux1 = (uint64_t) in1;
            aux2 = (uint64_t) in2 & 0x1f;
            result = aux1 >> aux2;
            break;
        case OP_SEQ:
        case OP_SEQI:
            result = (in1 == in2);
            break;
        case OP_SNE:
        case OP_SNEI:
            result = (in1 != in2);
            break;
        case OP_SGT:
        case OP_SGTI:
            result = (in1 > in2);
            break;
        case OP_SLT:
        case OP_SLTI:
            result = (in1 < in2);
            break;
        case OP_SGE:
        case OP_SGEI:
            result = (in1 >= in2);
            break;
        case OP_SLE:
        case OP_SLEI:
            result = (in1 <= in2);
            break;
        case OP_LD:
        case OP_SD:
        case OP_LW:
        case OP_LWU:
        case OP_SW:
        case OP_LH:
        case OP_LHU:
        case OP_SH:
        case OP_LB:
        case OP_LBU:
        case OP_SB:
        case OP_BEQZ:
        case OP_BNEZ:
        case OP_BEQ:
        case OP_BNE:
        case OP_J:
        case OP_JAL:
        case OP_BC1T:
        case OP_BC1F:
            result = in1 + in2;
            break;
        case OP_JR:
        case OP_JALR:
            result = in1; // El Registro Ra
            break;
        case OP_MOVN:
            if (in2 < 0) {
                result = in1;
            } else {// Cambiamos el Rdst, para que no escriba
                //result = 0;
                ID_EX.IR.Rdestino = 0;
            }
            break;
        case OP_MOVZ:
            if (in2 == 0) {
                result = in1;
            } else {// Cambiamos el Rdst, para que no escriba
                //result = 0;
                ID_EX.IR.Rdestino = 0;
            }
            break;
        case OP_NOP:
        case OP_TRAP:
        case OP_SYSCALL:
            result = 0;
            break;

        default:
            fnError("Operación %d no implementada\n", codop)
                    //fprintf(stderr,
                    //        "ERROR (%d %d): Operación no implementada\n",
                    //        __LINE__, codop);
                    //exit(1);
    } /* endswitch */

    return (result);
} /* end operacion_ALU */

/***************************************************************
 *
 * Func: operacion_COMP
 *
 * Desc: Realiza la operacion en el comparador de la fase EX
 *
 **************************************************************/
dword operacion_COMP(ubyte codop, dword in1, dword in2) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    result = 0; /* no salta */


    /*** Operacion en el comparador */
    switch (codop) {
        case OP_BEQZ: /* OJO, no con ds1 */
        case OP_BC1F:
            if (in1 == 0)
                result = 1;
            else
                result = 0;
            break;
        case OP_BNEZ: /* OJO, no con ds1 */
        case OP_BC1T:
            if (in1 != 0)
                result = 1;
            else
                result = 0;
            break;
        case OP_BNE:
            if (in1 != in2)
                result = 1;
            else
                result = 0;
            break;
        case OP_BEQ:
            if (in1 == in2)
                result = 1;
            else
                result = 0;
            break;
        case OP_J:
        case OP_JR:
        case OP_JAL:
        case OP_JALR:
            result = 1;
            break;
        default:
            result = 0; /* No salta */
    } /* endswitch */


    return (result);
} /* end operacion_COMP */

/***************************************************************
 *
 * Func: mux_ALUsup
 *
 * Desc: Implementa el multiplexor superior de la ALU
 *
 **************************************************************/
dword mux_ALUsup(dword npc, dword ra, dword mem, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result = 0;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WBaEXalu_s = NO;
    MEMaEXalu_s = NO;

    switch (ID_EX.IR.codop) {
        case OP_BNEZ: /* si es un salto */
        case OP_BEQZ:
        case OP_BEQ:
        case OP_BNE:
        case OP_BC1T:
        case OP_BC1F:
        case OP_J:
        case OP_JAL:
            result = npc;
            EX.muxALU_sup = 0;
            break;
        default: /* el resto de instrucciones */
            result = ra; /* por defecto */
            EX.muxALU_sup = 3;
            switch (solucion_riesgos_datos) {
                case ninguno:
                case parada:
                    break;
                case cortocircuito:
                    /* Los cortocircuitos desde MEM tienen prioridad sobre WB, ya
                       que la instruccion que hay en MEM es mas moderna */
                                        /* WBtoEX */
                    
              /* INSERTAR CÓDIGO */

                    /* if (...) {
                            ...
                            WBaEXalu_s = SI;
                            result = wb;
                        }
                     */

                    /* MEMtoEX */
                    
              /* INSERTAR CÓDIGO */

                    /* if (...) {
                            ...
                            MEMaEXalu_s = SI;
                            result = mem;
                        }
                     */
                    break;
            }
    }
    //    fprintf(stdout, "DEBUG muxALUsup ID_EX.IR.Rfuente1 %d EX_MEM.IR.Rdestino %d\n",ID_EX.IR.Rfuente1,EX_MEM.IR.Rdestino);
    //    fprintf(stdout, "DEBUG muxALUsup npc %d ra %d mem %d wb %d result %d\n",npc,ra,mem,wb,result);

    return (result);
} /* end mux_ALUsup */

/***************************************************************
 *
 * Func: mux_ALUinf
 *
 * Desc: Implementa el multiplexor inferior de la ALU
 *
 **************************************************************/
dword mux_ALUinf(dword rb, dword imm, dword mem, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result = 0;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WBaEXalu_i = NO;
    MEMaEXalu_i = NO;

    switch (ID_EX.IR.codop) {
        case OP_JR: /* si es un salto J*R */
        case OP_JALR:
            result = 0;
            EX.muxALU_inf = -1;
            break;
        default: /* el resto de instrucciones */
            switch (ID_EX.IR.tipo) {
                case FormatoR:
                    result = rb; /* por defecto */
                    EX.muxALU_inf = 0;
                    if (solucion_riesgos_datos == cortocircuito) {
                        /* Los cortocircuitos desde MEM tienen prioridad sobre WB, ya
                        que la instruccion que hay en MEM es mas moderna */
                        
                        /* WBtoEX */
                        
              /* INSERTAR CÓDIGO */

                        /* if (...) {
                            ...
                            WBaEXalu_i = SI;
                            result = wb;
                        }
                         */

                        /* MEMtoEX */
                        
              /* INSERTAR CÓDIGO */

                        /* if (...) {
                            ...
                            MEMaEXalu_i = SI;
                            result = mem;
                        }
                         */
                        break;
                    }
                    break;
                case FormatoI:
                case FormatoJ:
                    result = imm;
                    EX.muxALU_inf = 3;
                    break;
                default:
                    result = rb;
                    EX.muxALU_inf = 0;
            } /* endswitch */

    }
    //        fprintf(stdout, "DEBUG muxALUinf rb %d imm %d mem %d wb %d result %d\n",rb,imm,mem,wb,result);

    return (result);
} /* end mux_ALUinf */

/***************************************************************
 *
 * Func: mux_COMPsup
 *
 * Desc: Implementa el multiplexor superior del COMP de la fase EX
 *
 **************************************************************/
dword mux_COMPsup(dword ra, dword mem, dword wb, dword fpsr_wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    MEMaEXcomp_s = NO;
    WBaEXcomp_s = NO;
    FP_WBaEXcomp = NO;

    result = ra; /* por defecto */

    if ((latencia_salto3(solucion_riesgos_control) || latencia_salto2(solucion_riesgos_control)) && (solucion_riesgos_datos == cortocircuito)) {
        switch (ID_EX.IR.codop) {
            case OP_BEQZ:
            case OP_BNEZ:
            case OP_BNE:
            case OP_BEQ:
                /* MEMtoEX */
                
                /* WBtoEX */
                                break;
            case OP_BC1F:
            case OP_BC1T:
                /* FP_WBtoEX */
                            default:
                break;
        };
    };

    //	fprintf(stdout,"DEBUG muxCOMP op=%d ra=%d mem=%d wb=%d fpsr_wb=%d result=%d\n",ID_EX.IR.codop,ra,mem,wb,fpsr_wb,result);

    return (result);
} /* end mux_COMPsup */

/***************************************************************
 *
 * Func: mux_COMPinf
 *
 * Desc: Implementa el multiplexor superior del COMP de la fase EX
 *
 **************************************************************/
dword mux_COMPinf(dword rb, dword mem, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    MEMaEXcomp_i = NO;
    WBaEXcomp_i = NO;

    result = rb; /* por defecto */

    if (solucion_riesgos_datos == cortocircuito) {
        switch (ID_EX.IR.codop) {
            case OP_BNE:
            case OP_BEQ:
                /* MEMtoEX */
                
                /* WBtoEX */
                                break;
            default:
                break;
        };
    };

    //	fprintf(stdout,"DEBUG muxCOMP op=%d ra=%d mem=%d wb=%d fpsr_wb=%d result=%d\n",ID_EX.IR.codop,ra,mem,wb,fpsr_wb,result);

    return (result);
} /* end mux_COMPinf */

/***************************************************************
 *
 * Func: mux_EXmem
 *
 * Desc: Implementa el multiplexor hacia memoria de la fase EX
 *
 **************************************************************/
dword mux_EXmem(dword rb, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WBaEXmem = NO;


    result = rb; /* por defecto */

    switch (solucion_riesgos_datos) {
        case cortocircuito:
            /* WBtoEX */
                        /* MEMtoEX */
            /* No hace falta. WBtoMEM lo resuelve */
            break;
        default:
            break;
    }
    return (result);
} /* end mux_EXmem */

/***************************************************************
 *
 * Func: mux_MEMmem
 *
 * Desc: Implementa el multiplexor hacia memoria de la fase MEM
 *
 **************************************************************/
dword mux_MEMmem(dword rb, dword wb) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    WBaMEM = NO;

    result = rb; /* por defecto */

    switch (solucion_riesgos_datos) {
        case cortocircuito:
            /* WBtoMEM */
                        break;
        default:
            break;
    }
    return (result);
} /* end mux_MEMmem */

/***************************************************************
 *
 * Func: mux_COMPidsup
 *
 * Desc: Implementa el multiplexor del COMP de la fase ID
 *
 **************************************************************/
dword mux_COMPidsup(dword ra, dword mem) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    MEMaIDcomp_s = NO;

    //FP_WBaIDcomp = NO;

    result = ra; /* por defecto */

    switch (solucion_riesgos_datos) {
        case cortocircuito:
            switch (IF_ID.IR.codop) {
                case OP_BEQZ:
                case OP_BNEZ:
                case OP_BEQ:
                case OP_BNE:
                    /* MEMtoID */
                                        /* WBtoIDn no hay */

                    break;

                default:
                    break;
            };
        default:
            break;
    };

    return (result);
} /* end mux_COMPidsup */

/***************************************************************
 *
 * Func: mux_COMPidinf
 *
 * Desc: Implementa el multiplexor del COMP de la fase ID
 *
 **************************************************************/
dword mux_COMPidinf(dword rb, dword mem) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    dword result;


    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    MEMaIDcomp_i = NO;

    result = rb; /* por defecto */

    switch (solucion_riesgos_datos) {
        case cortocircuito:
            switch (IF_ID.IR.codop) {
                case OP_BEQ:
                case OP_BNE:
                    /* MEMtoID */
                                        /* WBtoID, no hay */

                    break;
                default:
                    break;
            };
        default:
            break;
    };
    return (result);
} /* end mux_COMPidinf */

/***************************************************************
 *
 * Func: detectar_riesgos_control
 *
 * Desc: Detecta los riesgos de control, activando las señales IDstall, IFstall en su caso
 *
 **************************************************************/
void detectar_riesgos_control(void) {

    /* riesgos de control */

    switch (solucion_riesgos_control) {
        case stall3:
        case stall2:
        case stall1:
            if (inst_salto(IF_ID.IR.codop)) {
                IFstall = SI;
                write_log("IFstall", IF_ID.iPC);
            }
            break;

        default:
            ;
    } /* end switch */
}

/***************************************************************
 *
 * Func: detectar_riesgos_datos
 *
 * Desc: Detecta los riesgos de datos, activando las señales IDstall, IFstall en su caso
 *
 **************************************************************/
void detectar_riesgos_datos(void) {

    /* riesgos de datos: insertar ciclos de parada? */

    switch (solucion_riesgos_datos) {
        case parada:
            /* Riesgo entre EX e ID */
            if (escribe_Rdst(ID_EX.IR) &&
                ((lee_Rfte1(ID_EX.IR)) ||
                (lee_Rfte2(ID_EX.IR))) && 
                ((ID_EX_t.IR.Rdestino == IF_ID_t.IR.Rfuente1) ||
                (ID_EX_t.IR.Rdestino == IF_ID_t.IR.Rfuente2))) {
                    IFstall=SI;
                    IDstall=SI;
            }

            /* Riesgo entre MEM e ID */
            if ((ID_EX.IR.codop = "ALU") &&
                (EX_MEM.IR.codop = "ALU") &&
                ((EX_MEM_t.IR.Rdestino == ID_EX_t.IR.Rfuente1) ||
                (EX_MEM_t.IR.Rdestino == ID_EX_t.IR.Rfuente2))) {
                    IFstall=SI;
                    IDstall=SI;
            }
            
            /* INSERTAR CÓDIGO */


            
            break;



        case cortocircuito:
            /* Riesgo entre LOAD en EX e ID */
            /* if (...) {
                    Señal=SI
                    ...
            }
             */
            
              /* INSERTAR CÓDIGO */


                        break;
        default:
            break;

    } /* endswitch */
}



