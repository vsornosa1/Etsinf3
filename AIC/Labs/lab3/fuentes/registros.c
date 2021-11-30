/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#define registros_C

#include "registros.h"
#include "tipos.h"
#include "main.h"

char *nombre_int_reg[] = {
    "zero", "at",
    "v0", "v1",
    "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9",
    "k1", "k2",
    "gp", "sp", "fp", "ra"
};

dword lee_int_reg(
        //valor_t lee_int_reg(
        dword n,
        tipo_t *tipo,
        boolean *dirty
        ) {
    *tipo = Rint_tipo[n];
    *dirty = Rint_dirty[n];
    return (Rint[n].valor);

}

void escribe_int_reg(
        dword n,
        dword dato,
        tipo_t tipo_dato,
        boolean activar_dirty
        ) {
    Rint[n].valor = dato;
    Rint_tipo[n] = tipo_dato;
    if (activar_dirty)
        Rint_dirty[n] = SI;
}

valor_t lee_fp_reg(
        dword n,
        tipo_t *tipo,
        boolean *dirty
        ) {
    *tipo = Rfp_tipo[n];
    *dirty = Rfp_dirty[n];
    return (Rfp[n].valor);
}

void escribe_fp_reg(
        dword n,
        valor_t dato,
        tipo_t tipo_dato,
        boolean activar_dirty
        ) {
    Rfp[n].valor = dato;
    Rfp_tipo[n] = tipo_dato;
    if (activar_dirty)
        Rfp_dirty[n] = SI;
}

