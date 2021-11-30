%{
/**************************************************************************
 * 
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politecnica de Valencia.                         
 * 
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *         Pedro López (plopez@disca.upv.es)
 * 
 * File: input.lex.c
 *  
 * Description: 
 *    Contiene la descripción léxica del lenguaje ensamblador
 * 
 *
 *************************************************************************/

/*** Includes */

#include <ctype.h>
#include <string.h>
#include <inttypes.h>
#include "input.yacc.h"
#include "instrucciones.h"
#include "main.h"

/*** Declaraciones externas */

extern int linenumber;
extern int yyerror (char*);

/*** Variables locales */

static char mensaje[128];

%}

STRING   [A-Za-z0-9[:blank:]]*
ALPHANUM [A-Za-z_][A-Za-z0-9_]*
BLANK	 [ \t]+

DEC_LIT	 ([-]?[1-9][0-9]*)|0
OCT_LIT	 0[0-9]+
HEX_LIT	 "0x"[0-9a-fA-F]+
FP_LIT	 [+-]?([0-9]*"."[0-9]+([eE][-+]?[0-9]+)?|[0-9]+[eE][-+]?[0-9]+)

REGI     [$r][0-9]?[0-9] 
REGF     (f|"$f")[0-9]?[0-9]

ALT_0	"$zero"
ALT_AT	"$at"
ALT_V	"$v"[0-1]
ALT_A	"$a"[0-3]
ALT_T1	"$t"[0-7]
ALT_S	"$s"[0-7]
ALT_T2	"$t"[8-9]
ALT_K   "$k"[0-1]
ALT_GP  "$gp"
ALT_SP  "$sp"
ALT_FP  "$fp"
ALT_RA  "$ra"

%option noyywrap

%%

 /*** Comentarios y conversiones **/

 /* añadir un "$" no elimina correctamente el comentario de la ultima linea */
;.*				/* Elimina los comentarios */
{BLANK}			/* Borra los espacios en blanco */
[A-Z] { unput(tolower(yytext[0])); }
				/* Convierte todos las letras a minusculas */

[\n] {	linenumber++; 
	return NL; }

 /*** Elementos del formato de instrucción */

{DEC_LIT} {
	sscanf(yytext, "%" PRId64, &yylval.ivalue);
	return T_INT_LIT;
	}

{OCT_LIT} {
	sscanf(yytext, "%" PRIo64, &yylval.ivalue);
	return T_INT_LIT;
	}

{HEX_LIT} {
	sscanf(yytext, "%" PRIx64, &yylval.ivalue);
	return T_INT_LIT;
	}

"#"{DEC_LIT} {
	sscanf(yytext+1, "%" PRId64, &yylval.ivalue);
	return T_INT_LIT;
	}

"#"{OCT_LIT} {
	sscanf(yytext+1, "%" PRIo64, &yylval.ivalue);
	return T_INT_LIT;
	}

"#"{HEX_LIT} {
	sscanf(yytext+1, "%" PRIx64, &yylval.ivalue);
	return T_INT_LIT;
	}

{FP_LIT} {
	sscanf(yytext, "%lg", &yylval.fvalue);
	return T_FP_LIT;
	}

{REGF} { 
	hay_float =SI;
	yylval.ivalue= atoi(yytext+1);
	if (yylval.ivalue >= TAM_REGISTROS)
	  {
	    sprintf(mensaje, 
		    "El rango de registros de coma flotante es F0-F%d",
		    TAM_REGISTROS-1); 
	    yyerror(mensaje);
	  }
	return REG_FP; 
	}

{REGI} { 
	yylval.ivalue= atoi(yytext+1);
	if (yylval.ivalue >= TAM_REGISTROS)
	  {
	    sprintf(mensaje, 
		    "El rango de registros enteros es R0-R%d",
		    TAM_REGISTROS-1); 
	    yyerror(mensaje);
	  }
	return REG_INT; 
	}

"%hi" {
    return M_HI;
}

"%lo" {
    return M_LO;
}

"%disp" {
    return M_DISP;
}

{ALT_0} { 
	yylval.ivalue= 0;
	return REG_INT; 
	}

{ALT_AT} { 
	yylval.ivalue= 1;
	return REG_INT; 
	}

{ALT_V} { 
	yylval.ivalue= atoi(yytext+1)+2;
	if (yylval.ivalue > 3)
	  {
	    sprintf(mensaje, 
		    "El rango de registros enteros es R0-R%d",
		    TAM_REGISTROS-1); 
	    yyerror(mensaje);
	  }
	return REG_INT; 
	}

{ALT_A} { 
	yylval.ivalue= atoi(yytext+2)+4;
	if (yylval.ivalue > 7)
	  {
	    sprintf(mensaje, 
		    "El rango de registros enteros es R0-R%d",
		    TAM_REGISTROS-1); 
	    yyerror(mensaje);
	  }
	return REG_INT; 
	}

{ALT_T1} { 
	yylval.ivalue= atoi(yytext+2)+8;
	if (yylval.ivalue > 15)
	  {
	    sprintf(mensaje, 
		    "El rango de registros enteros es R0-R%d",
		    TAM_REGISTROS-1); 
	    yyerror(mensaje);
	  }
	return REG_INT; 
	}

{ALT_S} { 
	yylval.ivalue= atoi(yytext+2)+16;
	if (yylval.ivalue > 23)
	  {
	    sprintf(mensaje, 
		    "El rango de registros enteros es R0-R%d",
		    TAM_REGISTROS-1); 
	    yyerror(mensaje);
	  }
	return REG_INT; 
	}

{ALT_T2} { 
	yylval.ivalue= atoi(yytext+2)+16;
	if (yylval.ivalue > 25)
	  {
	    sprintf(mensaje, 
		    "El rango de registros enteros es R0-R%d",
		    TAM_REGISTROS-1); 
	    yyerror(mensaje);
	  }
	return REG_INT; 
	}

{ALT_K} { 
	yylval.ivalue= atoi(yytext+2)+26;
	if (yylval.ivalue > 27)
	  {
	    sprintf(mensaje, 
		    "El rango de registros enteros es R0-R%d",
		    TAM_REGISTROS-1); 
	    yyerror(mensaje);
	  }
	return REG_INT; 
	}
	
{ALT_GP} { 
	yylval.ivalue= 28;
	return REG_INT; 
	}

{ALT_SP} { 
	yylval.ivalue= 29;
	return REG_INT; 
	}

{ALT_FP} { 
	yylval.ivalue= 30;
	return REG_INT; 
	}

{ALT_RA} { 
	yylval.ivalue= 31;
	return REG_INT; 
	}

{ALPHANUM}":" {
	strcpy(yylval.cvalue,yytext);
	yylval.cvalue[yyleng-1]= '\0';
	return LABEL; 
	}

"\""{STRING}"\"" { 
	strncpy(yylval.cvalue,&yytext[1],strlen(yytext)-2);
	yylval.cvalue[yyleng-2]= '\0';
	return T_STRING; 
	}

 /*** Directivas del ensamblador */

".data" {
	return DE_DATA;
	}
".text"	{ 
	return DE_TEXT;
	}
".globl" {
    return DE_GLOBL;
    }
".ireg"	{ 
	return DE_IREG;
	}
".fpreg"	{
    return DE_FREG;
    }
".btb"	{
    return DE_BTB;
    }

".byte"	{ 
	return DE_BYTE;
	hay_byte=SI;
	} 
".half"	{
	hay_half =SI;
	return DE_HALF;
	}
".word"	{ 
    hay_word =SI;
	return DE_WORD;
	}
".dword" {
    hay_dword =SI;
	return DE_DWORD;
	}
".float" {
    hay_float =SI;
	return DE_FLOAT;
	}
".double" {
    hay_double =SI;
	return DE_DOUBLE;
	}
".space" { 
	return DE_SPACE;
	}
".asciiz" { 
	// hay_char= SI;
	return DE_ASCIIZ;
	}


 /*** Pseudo-Instrucciones implementadas ************************************/

"li"	{ yylval.ivalue= 0;
	  return PINST_LI; }
"la"	{ yylval.ivalue= 0;
	  return PINST_LA; }

 /*** Instrucciones implementadas *******************************************/

"ld"	{ yylval.ivalue= OP_LD;
	  return FORM_INM_L; }
"lw"	{ yylval.ivalue= OP_LW;
	  return FORM_INM_L; }
"lh"	{ yylval.ivalue= OP_LH;
	  return FORM_INM_L; }
"lb"	{ yylval.ivalue= OP_LB;
	  return FORM_INM_L; }
"lwu"	{ yylval.ivalue= OP_LWU;
	  return FORM_INM_L; }
"lhu"	{ yylval.ivalue= OP_LHU;
	  return FORM_INM_L; }
"lbu"	{ yylval.ivalue= OP_LBU;
	  return FORM_INM_L; }

"sd"	{ yylval.ivalue= OP_SD;
	  return FORM_INM_S; }
"sw"	{ yylval.ivalue= OP_SW;
	  return FORM_INM_S; }
"sh"	{ yylval.ivalue= OP_SH;
	  return FORM_INM_S; }
"sb"	{ yylval.ivalue= OP_SB;
	  return FORM_INM_S; }
"lui"	{ yylval.ivalue= OP_LUI;
	  return FORM_INM_HI; }


"dadd"	{ yylval.ivalue= OP_DADD;
	  return FORM_REG; }
"dsub"	{ yylval.ivalue= OP_DSUB;
	  return FORM_REG; }
"dmul"	{ yylval.ivalue= OP_DMUL;
	  return FORM_REG; }
"ddiv"	{ yylval.ivalue= OP_DDIV;
	  return FORM_REG; }
	  

"and"	{ yylval.ivalue= OP_AND; 
	  return FORM_REG; }
"or"	{ yylval.ivalue= OP_OR; 
	  return FORM_REG; }
"xor"	{ yylval.ivalue= OP_XOR; 
	  return FORM_REG; }
	  
"dsrav"	{ yylval.ivalue= OP_DSRA;
	  return FORM_REG; }
"dsllv"	{ yylval.ivalue= OP_DSLL;
	  return FORM_REG; }
"dsrlv"	{ yylval.ivalue= OP_DSRL;
	  return FORM_REG; }
	  
"seq"	{ yylval.ivalue= OP_SEQ; 
	  return FORM_REG; }
"sne"	{ yylval.ivalue= OP_SNE; 
	  return FORM_REG; }
"sgt"	{ yylval.ivalue= OP_SGT; 
	  return FORM_REG; }
"slt"	{ yylval.ivalue= OP_SLT; 
	  return FORM_REG; }
"sge"	{ yylval.ivalue= OP_SGE; 
	  return FORM_REG; }
"sle"	{ yylval.ivalue= OP_SLE; 
	  return FORM_REG; }
	  
"daddi"	{ yylval.ivalue= OP_DADDI;
	  return FORM_INM; }
"dsubi"	{ yylval.ivalue= OP_DSUBI;
	  return FORM_INM; }
	  
"andi"	{ yylval.ivalue= OP_ANDI; 
	  return FORM_INM; }
"xori"	{ yylval.ivalue= OP_XORI;
	  return FORM_INM; }
"ori"	{ yylval.ivalue= OP_ORI;
	  return FORM_INM_LO; }

"dsra"	{ yylval.ivalue= OP_DSRAI;
	  return FORM_INM; }
"dsll"	{ yylval.ivalue= OP_DSLLI;
	  return FORM_INM; }
"dsrl"	{ yylval.ivalue= OP_DSRLI;
	  return FORM_INM; }
	  
"seqi"	{ yylval.ivalue= OP_SEQI; 
	  return FORM_INM; }
"snei"	{ yylval.ivalue= OP_SNEI; 
	  return FORM_INM; }
"sgti"	{ yylval.ivalue= OP_SGTI; 
	  return FORM_INM; }
"slti"	{ yylval.ivalue= OP_SLTI; 
	  return FORM_INM; }
"sgei"	{ yylval.ivalue= OP_SGEI; 
	  return FORM_INM; }
"slei"	{ yylval.ivalue= OP_SLEI; 
	  return FORM_INM; }
	  
"bnez"	{ yylval.ivalue= OP_BNEZ; 
	  return FORM_INM_B_FI; }
"beqz"	{ yylval.ivalue= OP_BEQZ; 
	  return FORM_INM_B_FI; }
"beq"	{ yylval.ivalue= OP_BEQ; 
	  return FORM_INM_B_FFI; }
"bne"	{ yylval.ivalue= OP_BNE; 
	  return FORM_INM_B_FFI; }
"movn"	{ yylval.ivalue= OP_MOVN;
	  return FORM_REG; }
"movz"	{ yylval.ivalue= OP_MOVZ;
	  return FORM_REG; }
"j"		{ yylval.ivalue= OP_J; 
	  return FORM_J; }
"jr"	{ yylval.ivalue= OP_JR; 
	  return FORM_J_F; }
"jal"	{ yylval.ivalue= OP_JAL; 
	  return FORM_J; }
"call"	{ yylval.ivalue= OP_JAL; 
	  return FORM_J; }
"jalr"	{ yylval.ivalue= OP_JALR; 
	  return FORM_J_F; }

	  
"ret"	{ yylval.ivalue= OP_NOP; 
	  return INST_RET ; }
"nop"	{ yylval.ivalue= OP_NOP; 
	  return INST_NOP ; }
"trap"	{ yylval.ivalue= OP_TRAP; 
	  return FORM_INM_T; }
"syscall"	{ yylval.ivalue= OP_SYSCALL; 
	  return INST_SYSCALL; }

     
"add.s"	{ yylval.ivalue= OP_FP_ADD_S; 
	  return FORM_FP_REG; }
"sub.s"	{ yylval.ivalue= OP_FP_SUB_S;
      return FORM_FP_REG; }
"mul.s"	{ yylval.ivalue= OP_FP_MUL_S;
      return FORM_FP_REG; }
"div.s"	{ yylval.ivalue= OP_FP_DIV_S;
      return FORM_FP_REG; }

      
"add.d"	{ yylval.ivalue= OP_FP_ADD_D; 
	  return FORM_FP_REG; }
"sub.d"	{ yylval.ivalue= OP_FP_SUB_D;
      return FORM_FP_REG; }
"mul.d"	{ yylval.ivalue= OP_FP_MUL_D;
      return FORM_FP_REG; }
"div.d"	{ yylval.ivalue= OP_FP_DIV_D;
      return FORM_FP_REG; }
      

"add.ps"	{ yylval.ivalue= OP_FP_ADD_PS; 
	  return FORM_FP_REG; }
"sub.ps"	{ yylval.ivalue= OP_FP_SUB_PS;
      return FORM_FP_REG; }
"mul.ps"	{ yylval.ivalue= OP_FP_MUL_PS;
      return FORM_FP_REG; }
"div.ps"	{ yylval.ivalue= OP_FP_DIV_PS;
      return FORM_FP_REG; }

      

"l.s"	{ yylval.ivalue= OP_FP_L_S; 
	  return FORM_INM_FP_L; }
"s.s"	{ yylval.ivalue= OP_FP_S_S; 
	  return FORM_INM_FP_S; }

"l.d"	{ yylval.ivalue= OP_FP_L_D; 
	  return FORM_INM_FP_L; }
"s.d"	{ yylval.ivalue= OP_FP_S_D; 
	  return FORM_INM_FP_S; }

"l.ps"	{ yylval.ivalue= OP_FP_L_PS; 
	  return FORM_INM_FP_L; }
"s.ps"	{ yylval.ivalue= OP_FP_S_PS; 
	  return FORM_INM_FP_S; }
  
"c.eq.s"	{ yylval.ivalue= OP_FP_EQ_S; 
	  return FORM_FP_REG_FF; }
"c.ne.s"	{ yylval.ivalue= OP_FP_NE_S; 
	  return FORM_FP_REG_FF; }
"c.lt.s"	{ yylval.ivalue= OP_FP_LT_S; 
	  return FORM_FP_REG_FF; }
"c.le.s"	{ yylval.ivalue= OP_FP_LE_S; 
	  return FORM_FP_REG_FF; }
"c.gt.s"	{ yylval.ivalue= OP_FP_GT_S; 
	  return FORM_FP_REG_FF; }
"c.ge.s"	{ yylval.ivalue= OP_FP_GE_S; 
	  return FORM_FP_REG_FF; }


"c.eq.d"	{ yylval.ivalue= OP_FP_EQ_D; 
	  return FORM_FP_REG_FF; }
"c.ne.d"	{ yylval.ivalue= OP_FP_NE_D; 
	  return FORM_FP_REG_FF; }
"c.lt.d"	{ yylval.ivalue= OP_FP_LT_D; 
	  return FORM_FP_REG_FF; }
"c.le.d"	{ yylval.ivalue= OP_FP_LE_D; 
	  return FORM_FP_REG_FF; }
"c.gt.d"	{ yylval.ivalue= OP_FP_GT_D; 
	  return FORM_FP_REG_FF; }
"c.ge.d"	{ yylval.ivalue= OP_FP_GE_D; 
	  return FORM_FP_REG_FF; }
	  
"mov.d"	{ yylval.ivalue= OP_FP_MOV_D;
	  return FORM_FP_REG_DF; }
"mov.s"	{ yylval.ivalue= OP_FP_MOV_S;
	  return FORM_FP_REG_DF; }

"mfc1"	{ yylval.ivalue= OP_MFC1;
	  return FORM_REG_R_FP; }
"mtc1"	{ yylval.ivalue= OP_MTC1;
	  return FORM_REG_FP_R; }

	  
"bc1t"	{ yylval.ivalue= OP_BC1T; 
	  return FORM_INM_B; }
"bc1f"	{ yylval.ivalue= OP_BC1F; 
	  return FORM_INM_B; }



{ALPHANUM} {
	strcpy(yylval.cvalue, yytext);
	return T_ALPHANUM;
	}


 /*** Control de errores ****************************************************/

"."{ALPHANUM} { 
	yyerror("Directiva de ensamblador desconocida"); 
	}

. { 
	return yytext[0]; }
