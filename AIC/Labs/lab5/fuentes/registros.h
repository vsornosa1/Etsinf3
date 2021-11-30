/*
 * File:   registros.h
 * Author: plopez
 *
 * Created on 19 de noviembre de 2017, 0:37
 */

#ifndef REGISTROS_H
#define REGISTROS_H

/*** Includes ******************************************/

#include "tipos.h"


/*** Definiciones **************************************/

/*** Registros MIPS */
#define _zero 0
#define _at   1
#define _v0   2
#define _v1   3
#define _a0   4
#define _a1   5
#define _a2   6
#define _a3   7
#define _t0  8
#define _t1  9
#define _t2  10
#define _t3  11
#define _t4  12
#define _t5  13
#define _t6  14
#define _t7  15
#define _t8  24
#define _t9  25
#define _s0  16
#define _s1  17
#define _s2  18
#define _s3  19
#define _s4  20
#define _s5  21
#define _s6  22
#define _s7  23
#define _k1  26
#define _k2  27
#define _gp  28
#define _sp  29
#define _fp  30
#define _ra  31


#ifdef __cplusplus
extern "C" {
#endif

#ifndef CPROTO
#include "registros_api.h"
#endif


#ifdef __cplusplus
}
#endif

extern char *nombre_int_reg[];

#endif /* REGISTROS_H */

