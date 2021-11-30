/*********************************************************************
 * 
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.                         
 * 
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 * 
 * File: presentacion.h
 *  
 * Description: 
 *    Definiciones para el módulo de presentación
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *********************************************************************/

#ifndef presentacion_H
#define presentacion_H

/***** Includes ***********************************************/

#include "tipos.h"
#include "memoria.h"

/***** Constants **********************************************/

/***** Constants **********************************************/

#define FIRMA_DEL_FICHERO "<Fichero de firmas> AIC"
#define MAX_INSTRUC 70 /* Número máximo de instrucciones visibles */
#define MAX_CICLOS_INIC 45 /* Número máximo de ciclos visibles
                                   inicialmente */
#define MAX_CICLOS 50 /* Número máximo de ciclos visibles */

#define NUM_STYLES 16
#define LONG_FIRMA 20480

#define LONG_FIRMA 20480
#define FIRMA_DEL_FICHERO "<Fichero de firmas> AIC"

/***** Types **************************************************/

//typedef enum {
//    INITIAL_MEM,
//    OLD_MEM,
//    NEW_MEM
//} mem_status_t;

typedef struct {
    // void imprime_inicio (void);
    void (*imprime_inicio_f)(void);
    // void imprime_final (void);
    void (*imprime_final_f)(void);
    // void inicio_ciclo (ciclo_t ciclo);
    void (*inicio_ciclo_f)(ciclo_t ciclo);
    // void fin_ciclo (ciclo_t ciclo);
    void (*fin_ciclo_f)(ciclo_t ciclo);
    // void imprime_estado (void);
    void (*imprime_estado_f)(void);
    // void imprime_crono (void);
    void (*imprime_crono_f)(void);
    // void imprime_predictor (void);
    void (*imprime_predictor_f)(void);
    // void init_instruc (dword PC, ciclo_t orden);
    void (*init_instruc_f)(dword PC, ciclo_t orden);
    // void muestra_fase (char *fase, ciclo_t orden, boolean exception);
    void (*muestra_fase_f)(char *fase, ciclo_t orden, boolean exception);
    // void muestra_datos(dword from, dword to, mem_status_t status)
    void (*muestra_datos_f)(region_t r, dword from, dword to, mem_status_t status);
} presentacion_interface_t;

/***** Macros *************************************************/

#ifdef MIN
#undef MIN
#endif
#define MIN(x, y) (((x)<(y)) ? (x) : (y))

#ifdef MAX
#undef MAX
#endif
#define MAX(x, y) (((x)>(y)) ? (x) : (y))

#define F_ESTADO F_CICLO "/" F_CICLO

#define FIRMA_1_1(s1, s2)                       \
    do {                                        \
      char campo[256];				\
      char valor[256];				\
      strcpy(campo, (s1));			\
      strcpy(valor, (s2));			\
      vstr= procesa_firma(campo, valor);	\
    } while (0)

#define FIRMA_1_2(s1, f2, p2)                   \
    do {                                        \
      char campo[256];				\
      char valor[256];				\
      strcpy(campo, (s1));			\
      sprintf(valor, (f2), (p2));		\
      vstr= procesa_firma(campo, valor);	\
    } while (0)

#define FIRMA_2_1(f1, p1, s2)                   \
    do {                                        \
      char campo[256];				\
      char valor[256];				\
      sprintf(campo, (f1),(p1));		\
      strcpy(valor, (s2));			\
      vstr= procesa_firma(campo, valor);	\
    } while (0)

#define FIRMA_2_2(f1, p1, f2, p2)               \
    do {                                        \
      char campo[256];				\
      char valor[256];				\
      sprintf(campo, (f1), (p1));		\
      sprintf(valor, (f2), (p2));		\
      vstr= procesa_firma(campo, valor);	\
    } while (0)

#define FIRMA_2_3(f1, p1, f2, p2a, p2b)         \
    do {                                        \
      char campo[256];				\
      char valor[256];				\
      sprintf(campo, (f1), (p1));		\
      sprintf(valor, (f2), (p2a), (p2b));	\
      vstr= procesa_firma(campo, valor);	\
    } while (0)


#define imprime_instruccion(str, dir) __imprime_instruccion(str, dir, __FILE__, __LINE__)

/***** Aliases ************************************************/

// void imprime_inicio (void);
#define imprime_inicio presentacion.imprime_inicio_f

// void imprime_final (void);
#define imprime_final presentacion.imprime_final_f

// void inicio_ciclo (ciclo_t ciclo);
#define inicio_ciclo(ciclo) presentacion.inicio_ciclo_f(ciclo)

// void fin_ciclo (ciclo_t ciclo);
#define fin_ciclo(ciclo) presentacion.fin_ciclo_f(ciclo)

// void imprime_estado (void);
#define imprime_estado presentacion.imprime_estado_f

// void imprime_crono (void);
#define imprime_crono presentacion.imprime_crono_f

// void imprime_predictor (void);
#define imprime_predictor presentacion.imprime_predictor_f

// void init_instruc (dword PC, ciclo_t orden);
#define init_instruc(PC, orden) presentacion.init_instruc_f(PC, orden)

// void muestra_fase (char *fase, ciclo_t orden);
#define muestra_fase(fase, orden) presentacion.muestra_fase_f(fase, orden, NO)
#define muestra_fase_exception(fase, orden) presentacion.muestra_fase_f(fase, orden, SI)

// void muestra_datos (region_t r, dword from, dword to, mem_status_t status);
#define muestra_datos(r, from, to, status) presentacion.muestra_datos_f(r, from, to, status)

/***** Prototypes *********************************************/

#ifndef CPROTO

#include "presentacion_api.h"

#endif

#ifdef presentacion_C  /*** Privado ***/

char p_str[256]; /* Cadena auxiliar */

/** Variables para las macros de firmas */
char *vstr;

boolean error_firma = NO;

#else    /*** Público ***/

extern char p_str[256]; /* Cadena auxiliar */
extern char *vstr;

extern boolean error_firma;

extern presentacion_interface_t presentacion;

#endif

#endif /* end presentacion.h */

