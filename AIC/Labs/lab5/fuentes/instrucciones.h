/*********************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad PolitÈcnica de Valencia.
 *
 * Autor: Sergio Sáez (ssaez@disca.upv.es)
 *	      Pedro López (plopez@gap.upv.es)
 *
 * Fichero: instrucciones.h
 *
 * Descripción:
 *   Contiene los cÛdigos de las instrucciones implementadas
 *
 ***********************************************************************/

#ifndef instrucciones_H
#define instrucciones_H

/*** Includes **************************************************************/

#include "tipos.h"

/*** Defines ***************************************************************/

/* El código de operación de las operaciones aritméticas
 enteras con valores inmediatos son iguales a las que usan
 registros MÁS UNO. Ver fichero input.lex.l */


#define OP_NOP    0
#define OP_TRAP   1

#define OP_NOP_FORCED 3

// MIPS 32
#define OP_LB    10
#define OP_LBU   11
#define OP_LH    12
#define OP_LHU   13
#define OP_LW    14
#define OP_LWU   15

#define OP_SB    16
#define OP_SH    17
#define OP_SW    18


/*
// MIPS 32
#define OP_ADD 		 21
#define OP_ADDI 	 22
//#define OP_ADDU 	 23
//#define OP_ADDUI 	 24
#define OP_SUB 		 25
#define OP_SUBI 	 26
//#define OP_SUBU 	 27
//#define OP_SUBUI 	 28
 */

#define OP_AND    20
#define OP_ANDI   21
#define OP_OR     22
#define OP_ORI    23
#define OP_XOR    24
#define OP_XORI   25

/*
// MIPS 32
#define OP_SLL 		 34
#define OP_SLLI 	 35
#define OP_SRA 		 36
#define OP_SRAI 	 37
#define OP_SRL 		 38
#define OP_SRLI 	 39
 */

// Solo existe SLT
#define OP_SEQ    40
#define OP_SEQI   41
//#define OP_SEQU 	 42
//#define OP_SEQUI 	 43
#define OP_SGE    44
#define OP_SGEI   45
//#define OP_SGEU 	 46
//#define OP_SGEUI 	 47
#define OP_SGT    48
#define OP_SGTI   49
//#define OP_SGTU 	 50
//#define OP_SGTUI 	 51
#define OP_SLE    52
#define OP_SLEI   53
//#define OP_SLEU 	 54
//#define OP_SLEUI 	 55
#define OP_SLT    56
#define OP_SLTI   57
//#define OP_SLTU 	 58
//#define OP_SLTUI 	 59

#define OP_SNE    60
#define OP_SNEI   61
//#define OP_SNEU 	 62
//#define OP_SNEUI 	 63


/* MIPS 64 */
#define OP_LD    64
#define OP_SD    65

/* Las versiones xxxU y xxxUI no están implementadas */
#define OP_DADD    70
#define OP_DADDI   71
#define OP_DADDU   72
#define OP_DADDUI  73
#define OP_DSUB    74
// DSUBI no existe
#define OP_DSUBI   75
#define OP_DSUBU   76
// DSUBUI no existe
#define OP_DSUBUI  77

#define OP_DMUL    78
//#define OP_DMULU     79
#define OP_DDIV    80
//#define OP_DDIVU     81
//#define OP_MADD      82

#define OP_LUI     83

#define OP_DSLL    84
#define OP_DSLLI   85
#define OP_DSRA    86
#define OP_DSRAI   87
#define OP_DSRL    88
#define OP_DSRLI   89


#define OP_BEQZ   100
#define OP_BNEZ   101
#define OP_BEQ    102
#define OP_BNE    103

#define OP_BC1T   104 /* El FPSR es un registro de 1 bit */
#define OP_BC1F   105 /* El FPSR es un registro de 1 bit */

#define OP_MOVN   106
#define OP_MOVZ   107

#define OP_J      108
#define OP_JR     109
#define OP_JAL    110
#define OP_JALR   111

#define OP_SYSCALL 120

/* Coma flotante */


#define OP_FP_L_S   128
#define OP_FP_S_S   129

#define FIRST_FP_ALU_S 130
#define OP_FP_ADD_S 130
#define OP_FP_SUB_S 131
#define OP_FP_MUL_S 132
#define OP_FP_DIV_S 133
//#define OP_FP_MADD_S 134
#define LAST_FP_ALU_S 133


#define FIRST_FP_CMP_S 135 /* FIRST C_XX_S */
#define OP_FP_GT_S  135
#define OP_FP_LT_S  136
#define OP_FP_EQ_S  137
#define OP_FP_NE_S  138
#define OP_FP_LE_S  139
#define OP_FP_GE_S  140
#define LAST_FP_CMP_S 140 /* LAST C_XX_S */

#define OP_FP_L_D  141
#define OP_FP_S_D  142

#define FIRST_FP_ALU_D 143
#define OP_FP_ADD_D  143
#define OP_FP_SUB_D  144
#define OP_FP_MUL_D  145
#define OP_FP_DIV_D  146
//#define OP_FP_MADD_D 147
#define LAST_FP_ALU_D 146

#define FIRST_FP_CMP_D 148 /* FIRST C_XX_D */
#define OP_FP_GT_D  148
#define OP_FP_LT_D  149
#define OP_FP_EQ_D  150
#define OP_FP_NE_D  151
#define OP_FP_LE_D  152
#define OP_FP_GE_D  153
#define LAST_FP_CMP_D 153 /* LAST C_XX_D */


#define FIRST_FP_ALU_PS 154
#define OP_FP_ADD_PS  154
#define OP_FP_SUB_PS  155
#define OP_FP_MUL_PS  156
#define OP_FP_DIV_PS  157
//#define OP_FP_MADD_PS 158
#define LAST_FP_ALU_PS 157

/*
 #define OP_CVT_L_W 159
#define OP_CVT_L_D 160
#define OP_CVT_L_S 161
#define OP_CVT_W_L 162
#define OP_CVT_W_D 163
#define OP_CVT_W_S 164
#define OP_CVT_S_W 165
#define OP_CVT_S_L 166
#define OP_CVT_S_D 167
#define OP_CVT_D_W 168
#define OP_CVT_D_L 169
#define OP_CVT_D_S 170
 */

#define OP_FP_L_PS  180 // No existe en MIPS64
#define OP_FP_S_PS  181 // No existe en MIPS64
#define OP_FP_MOV_S 182
#define OP_FP_MOV_D 183

#define OP_MFC1     184
#define OP_MTC1     185



/*** Macros ****************************************************************/

#define inst_int(i) ((i) < 128)
#define inst_fp(i) ((i) >= 128)

// Instrucciones que escriben dobles
#define inst_fp_d(i) ((i)==OP_FP_L_D || (i)==OP_FP_S_D || (i)==OP_FP_MOV_D || ((i)>=FIRST_FP_ALU_D && (i)<=LAST_FP_ALU_D) || ((i)>=FIRST_FP_CMP_D && (i)<=LAST_FP_CMP_D))
// Instrucciones que escriben simples
#define inst_fp_s(i) ((i)==OP_FP_L_S || (i)==OP_FP_S_S || (i)==OP_FP_MOV_S || ((i)>=FIRST_FP_ALU_S && (i)<=LAST_FP_ALU_S) || ((i)>=FIRST_FP_CMP_S && (i)<=LAST_FP_CMP_S))
// Instrucciones que escriben simd
#define inst_simd(i) ((i)== OP_FP_ADD_PS || (i)== OP_FP_SUB_PS || (i)== OP_FP_MUL_PS || (i)== OP_FP_DIV_PS || (i)== OP_FP_L_PS || (i)== OP_FP_S_PS)


#define inst_load_int(i) ((i) == OP_LD || (i) == OP_LW || (i) == OP_LH || (i) == OP_LB || (i) == OP_LHU || (i) == OP_LBU)
#define inst_store_int(i) ((i) == OP_SD || (i) == OP_SW || (i) == OP_SH || (i) == OP_SB)
#define inst_load_fp(i) ((i) == OP_FP_L_D || (i) == OP_FP_L_S || (i) == OP_FP_L_PS)
#define inst_store_fp(i) ((i) == OP_FP_S_D || (i) == OP_FP_S_S || (i) == OP_FP_S_PS )

#define inst_load(i) (inst_load_int(i) || inst_load_fp(i))
#define inst_store(i) (inst_store_int(i) || inst_store_fp(i))

// #define instruccion_aritmetica_entera(i) ((i) != OP_LW) && (i) != OP_SW))

#define inst_fpsr(i) (((i) >= FIRST_FP_CMP_S && (i) <= LAST_FP_CMP_S) || ((i) >= FIRST_FP_CMP_D && (i) <= LAST_FP_CMP_D))


#define inst_branch_int(i) ((i) == OP_BNEZ || (i) == OP_BEQZ || (i) == OP_BNE || (i) == OP_BEQ)
#define inst_branch_fp(i)  ((i) == OP_BC1T || (i) == OP_BC1F)
#define inst_branch(i)  (inst_branch_int(i) || inst_branch_fp(i))
#define inst_salto_PCrel(i) (inst_branch(i) || (i) == OP_J || (i) == OP_JAL)

#define inst_jump_int(i) ((i) == OP_J || (i) == OP_JR || (i) == OP_JAL || (i) == OP_JALR)
#define inst_jr(i) ((i) == OP_JR || (i) == OP_JALR)
#define inst_jal(i) ((i) == OP_JAL || (i) == OP_JALR)
#define inst_j(i) ((i) == OP_J || (i) == OP_JR)
#define inst_salto(i) (inst_branch(i) || inst_jump_int(i))

#define inst_condicion(i) (inst_salto(i) || (i) == OP_MOVZ || (i) == OP_MOVN)
#define inst_excepcion(i) ((i)== OP_TRAP || (i)==OP_SYSCALL)

/***** Prototypes *********************************************/

#ifndef CPROTO
#include "instrucciones_api.h"
#endif

#endif /* Include instrucciones.h */
