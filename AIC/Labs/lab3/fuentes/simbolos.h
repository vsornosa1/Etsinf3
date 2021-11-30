/**************************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * Author: Sergio Sáez (ssaez@disca.upv.es)
 *
 * File: simbolos.c
 *
 * Description:
 *      Contiene las definiciones asociadas a la tabla de simbolos
 *
 *************************************************************************/

#ifndef simbolos_H
#define simbolos_H

/***** Includes ***********************************************/

#include "tipos.h"
#include "memoria.h"

//#define _POSIX_C_SOURCE 1
#include <limits.h>

/***** Definiciones *******************************************/

#define LONG_ETIQUETA    64

/***** Definiciones *******************************************/

// Macro para hacer la migración. Sólo se puede utilizar en ficheros que incluyan main.h
#define te_etiqueta(str, dir) ts_simbolo_direccion(programa.simbolos, (dir), (str), f_etiquetas)

/***** Tipos **************************************************/

// Indica que parte de la dirección de la referencia hay que poner dentro de la instruccion
typedef enum {
    A32_LO, // Los 16 bits bajos de una dirección de 32 bits
    A32_HI, // Los 16 bits altos de una dirección de 32 bits
    PC_REL_16,  // Valor inmediato relativo al PC (16 bits)
    DATA_REL_16 // Valor desplazamiento relativo al comienzo de la zona de datos (_data)
} parte_direccion_t;

typedef struct etiqueta_s {
    char texto[LONG_ETIQUETA];

    dword offset;              /* Dirección de la etiqueta dentro del segmento */
    region_t region;           /* Tipo de memoria a la que apunta la etiqueta */

    struct etiqueta_s *sig;    /* Siguiente etiqueta */
} etiqueta_t;

typedef struct simbolo_s {
    char texto[LONG_ETIQUETA];

    dword direccion;           /* Dirección absoluta del símbolo o offset antes de relocalizarlo */
    region_t region;           /* Tipo de memoria de la referencia: r_text o el resto */
    boolean global;            /* Símbolo global */

    struct simbolo_s *sig;    /* Siguiente símbolo */
} simbolo_t;

/* Referencia sin resolver o pendiente de relocalizar */
typedef struct referencia_s {
    char texto[LONG_ETIQUETA];

    region_t region;        /* Región de código donde se encuentra la instrucción */
    dword offset;            /* Offset de la instrucción dentro de la región de código */
    parte_direccion_t parte_dir; /* Parte de la dirección que nos interesa */
    tipo_region_t tipo_region;    /* Tipo de memoria donde encontrar la referencia: tr_code o tr_data  */

    struct referencia_s *sig; /* Siguiente referencia por resolver */
} referencia_t;

typedef struct {
    char fichero[PATH_MAX];

    etiqueta_t *etiqs;  // Etiquetas dentro de un módulo
    referencia_t *refs; // Referencias pendientes de resolver
    simbolo_t *sims;    // Símbolos ofrecidos por este módulo
} tabla_simbolos_t;

/***** Prototipos *********************************************/

#ifndef CPROTO

#include "simbolos_api.h"

#endif

#endif
