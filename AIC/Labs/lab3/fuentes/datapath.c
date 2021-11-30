/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "instrucciones.h"
#include "datapath.h"

#define imprime_valor(valor, maxval, cadena) { \
    if (abs(valor) < maxval) { \
        sprintf(cadena, "%d", valor); \
    } else { \
        if (valor>0) \
            sprintf(cadena, "++"); \
        else \
            sprintf(cadena, "--"); \
    } \
}



#define TAM_DIBUJO 2048

extern FILE * f_salida_html;

typedef struct {
    word ctrl;
    char color[20];
} muxctrl_t;

typedef struct {
    word x;
    word y;
} coord_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t out1;
    char dibujo[TAM_DIBUJO];
} elem_1i1o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t out1;
    coord_t in2;
    coord_t out2;
    char dibujo[TAM_DIBUJO];
} elem_2i2o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t out1;
    coord_t in2;
    coord_t out2;
    coord_t in3;
    coord_t out3;
    char dibujo[TAM_DIBUJO];
} elem_3i3o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t out1;
    coord_t in2;
    coord_t out2;
    coord_t in3;
    coord_t out3;
    coord_t in4;
    coord_t out4;
    char dibujo[TAM_DIBUJO];
} elem_4i4o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t out1;
    coord_t in2;
    coord_t out2;
    coord_t in3;
    coord_t out3;
    coord_t in4;
    coord_t out4;
    coord_t in5;
    coord_t out5;
    char dibujo[TAM_DIBUJO];
} elem_5i5o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t out1;
    coord_t in2;
    coord_t out2;
    coord_t in3;
    coord_t out3;
    coord_t in4;
    coord_t out4;
    coord_t in5;
    coord_t out5;
    coord_t in6;
    coord_t out6;
    char dibujo[TAM_DIBUJO];
} elem_6i6o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t in2;
    coord_t out1;
    char dibujo[TAM_DIBUJO];
} elem_2i1o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t in2;
    coord_t in3;
    coord_t out1;
    char dibujo[TAM_DIBUJO];
} elem_3i1o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t in2;
    coord_t in3;
    coord_t in4;
    coord_t out1;
    char dibujo[TAM_DIBUJO];
} elem_4i1o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t in2;
    coord_t in3;
    coord_t in4;
    coord_t out1;
    coord_t out2;
    char dibujo[TAM_DIBUJO];
} elem_4i2o_t;

typedef struct {
    coord_t pos1;
    coord_t pos2;
    int ancho;
    int alto;
    coord_t in1;
    coord_t in2;
    coord_t in3;
    coord_t in4;
    coord_t in5;
    coord_t out1;
    char dibujo[TAM_DIBUJO];
} elem_5i1o_t;

/***** Macros Locales ******************************************/

#define inicia_dibujo(figura) strcpy(figura, "")

#define dibuja(figura,format,...) do { \
  char line[256];                   \
  sprintf(line, format, __VA_ARGS__); \
  strcat(figura, line);                \
} while(0)

/***************************************************************
 *
 * Func: asigna
 *
 * Desc: Asigna un valor a un punto
 *
 **************************************************************/

/*
static void asigna (coord_t* punto, int x, int y)
{
    (*punto).x = x;
    (*punto).y = y;
}
 */

/***************************************************************
 *
 * Func: escribe
 *
 * Desc: Escribe un texto en el dibujo
 *
 **************************************************************/

static void escribe(int x, int y, char* texto, char* tcolor, int tsize) {

    fprintf(f_salida_html, "<!-- Texto -->\n");
    fprintf(f_salida_html, "<text x=\"%d\" y=\"%d\"\n     style=\"fill:%s;stroke:%s;font-family:Arial;font-size:%dpx;\">\n     %s\n</text>\n\n", x, y, tcolor, "null", tsize, texto);
}

/***************************************************************
 *
 * Func: genera_sum
 *
 * Desc: Dibuja el símbolo de la ALU
 *
 **************************************************************/

static void genera_sum(elem_2i1o_t* alu, int x1, int y1, int ancho, int alto, char* nombre, char* color, char* relleno, char* tcolor, int input1, int input2, int output1) {
    int x2;
    int y2;
    char i1[5];
    char i2[5];
    char o1[5];

    x2 = x1 + ancho;
    y2 = y1 + alto;

    alu->pos1.x = x1;
    alu->pos1.y = y1;
    alu->pos2.x = x2;
    alu->pos2.y = y2;
    alu->ancho = ancho;
    alu->alto = alto;
    alu->in1.x = x1;
    alu->in1.y = y1 + alto / 6;
    alu->in2.x = x1;
    alu->in2.y = y2 - alto / 6;
    alu->out1.x = x2;
    alu->out1.y = y1 + alto / 2;
    inicia_dibujo(alu->dibujo);
    dibuja(alu->dibujo, "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d\" \n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
            x1, y1, x1, y1 + (int) (3 * alto / 8), x1 + (int) (ancho / 4), y1 + (int) alto / 2, x1, y1 + (int) (5 * alto / 8), x1, y1 + alto, x2, y2 - (int) alto / 4, x2, y1 + (int) alto / 4, x1, y1, relleno, color, trazo_grueso);
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n</g>\n",
            x1 + ancho / 2 + 5, alu->out1.y + 15, color, "none", nombre);

    imprime_valor(input1, 10000, i1);
    imprime_valor(input2, 10000, i2);
    imprime_valor(output1, 10000, o1);
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
            alu->in1.x + 5, alu->in1.y + 15, tcolor, "none", i1);
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
            alu->in2.x + 5, alu->in2.y - 5, tcolor, "none", i2);
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n",
            alu->out1.x - 5, alu->out1.y, tcolor, "none", o1);

    fprintf(f_salida_html, "<!-- ALU -->\n");
    fprintf(f_salida_html, "%s\n", alu->dibujo);
}

/***************************************************************
 *
 * Func: genera_cmp
 *
 * Desc: Dibuja el símbolo de la ALU
 *
 **************************************************************/

static void genera_cmp(elem_2i1o_t* alu, int x1, int y1, int ancho, int alto, char* nombre, char* color, char* relleno, char* tcolor, int input1, int input2, int output1) {
    int x2;
    int y2;
    char i1[5];
    char i2[5];
    char o1[5];

    x2 = x1 + ancho;
    y2 = y1 + alto;

    alu->pos1.x = x1;
    alu->pos1.y = y1;
    alu->pos2.x = x2;
    alu->pos2.y = y2;
    alu->ancho = ancho;
    alu->alto = alto;
    alu->in1.x = x1;
    alu->in1.y = y1 + alto / 6;
    alu->in2.x = x1;
    alu->in2.y = y2 - alto / 6;
    alu->out1.x = x2;
    alu->out1.y = y1 + alto / 2;
    inicia_dibujo(alu->dibujo);
    dibuja(alu->dibujo, "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d\" \n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
            x1, y1, x1, y1 + (int) (3 * alto / 8), x1 + (int) (ancho / 4), y1 + (int) alto / 2, x1, y1 + (int) (5 * alto / 8), x1, y1 + alto, x2, y2 - (int) alto / 4, x2, y1 + (int) alto / 4, x1, y1, relleno, color, trazo_grueso);
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n</g>\n",
            x1 + ancho / 2 + 5, alu->out1.y + 15, color, "none", nombre);

    imprime_valor(input1, 100, i1);
    imprime_valor(input2, 100, i2);
    imprime_valor(output1, 100, o1);
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
            alu->in1.x + 5, alu->in1.y + 15, tcolor, "none", i1);
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
            alu->in2.x + 5, alu->in2.y, tcolor, "none", i2);
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n",
            alu->out1.x - 5, alu->out1.y, tcolor, "none", o1);

    fprintf(f_salida_html, "<!-- ALU -->\n");
    fprintf(f_salida_html, "%s\n", alu->dibujo);
}

/***************************************************************
 *
 * Func: genera_alu
 *
 * Desc: Dibuja el símbolo de la ALU
 *
 **************************************************************/

static void genera_ALU(elem_2i1o_t* alu, int x1, int y1, int ancho, int alto, char* nombre, char* color, char* relleno, char* tcolor, int input1, int input2, int output1) {
    int x2;
    int y2;
    char i1[5];
    char i2[5];
    char o1[5];
    char control[12];

    x2 = x1 + ancho;
    y2 = y1 + alto;

    alu->pos1.x = x1;
    alu->pos1.y = y1;
    alu->pos2.x = x2;
    alu->pos2.y = y2;
    alu->ancho = ancho;
    alu->alto = alto;
    alu->in1.x = x1;
    alu->in1.y = y1 + alto / 6;
    alu->in2.x = x1;
    alu->in2.y = y2 - alto / 6;
    alu->out1.x = x2;
    alu->out1.y = y1 + alto / 2;
    inicia_dibujo(alu->dibujo);
    dibuja(alu->dibujo, "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d\" \n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n",
            x1, y1, x1, y1 + (int) (3 * alto / 8), x1 + (int) (ancho / 4), y1 + (int) alto / 2, x1, y1 + (int) (5 * alto / 8), x1, y1 + alto, x2, y2 - (int) alto / 4, x2, y1 + (int) alto / 4, x1, y1, relleno, color, trazo_grueso);
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n</g>\n",
            x1 + ancho / 2 + 5, alu->out1.y + 15, color, "none", nombre);
    imprime_valor(input1, 10000, i1);
    imprime_valor(input2, 10000, i2);
    imprime_valor(output1, 10000, o1);
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
            alu->in1.x + 5, alu->in1.y + 15, tcolor, "none", i1);
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
            alu->in2.x + 5, alu->in2.y - 10, tcolor, "none", i2);
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n",
            alu->out1.x - 5, alu->out1.y, tcolor, "none", o1);
    if (inst_load(ID_EX.IR.codop) || inst_store(ID_EX.IR.codop))
        sprintf(control, "mem addr");
    else if ((latencia_salto2(solucion_riesgos_control) || latencia_salto3(solucion_riesgos_control)) && inst_branch(ID_EX.IR.codop))
        sprintf(control, "branch addr");
    else
        sprintf(control, "alu op");
    dibuja(alu->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
            alu->pos1.x - 10, alu->pos1.y - 5, tcolor, "none", control);
    fprintf(f_salida_html, "<!-- ALU -->\n");
    fprintf(f_salida_html, "%s\n", alu->dibujo);
}

/***************************************************************
 *
 * Func: genera_MI
 *
 * Desc: Dibuja el símbolo de la memoria
 *
 **************************************************************/

static void genera_MI(elem_2i1o_t* mem, int x1, int y1, int ancho, int alto, char* tcolor, char* color, char* relleno) {
    int x2;
    int y2;
    char i1[50];
    char nombre[10] = "Mem Instr";
    char in1[10] = "Dir";
    char in2[10] = "";
    char out1[10] = "Inst";


    x2 = x1 + ancho;
    y2 = y1 + alto;

    mem->pos1.x = x1;
    mem->pos1.y = y1;
    mem->pos2.x = x2;
    mem->pos2.y = y2;
    mem->ancho = ancho;
    mem->alto = alto;
    mem->in1.x = x1;
    mem->in1.y = y1 + alto / 4;
    mem->in2.x = x1;
    mem->in2.y = y2 - alto / 4;
    mem->out1.x = x2;
    mem->out1.y = y1 + alto / 2;
    inicia_dibujo(mem->dibujo);
    dibuja(mem->dibujo, "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", x1, y1, x1, y1 + alto, x1 + ancho, y1 + alto, x1 + ancho, y1, x1, y1, relleno, color, trazo_grueso);
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n", x1 + ancho / 2, (int) y1 + 5 * alto / 8, color, "none", nombre);
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n", mem->in1.x + 5, mem->in1.y, color, "none", in1);
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n", mem->in2.x + 5, mem->in2.y, color, "none", in2);
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n", mem->out1.x - 5, mem->out1.y, color, "none", out1);
    imprime_valor(PC, 10000, i1);
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n", mem->in1.x + 25, mem->in1.y, tcolor, "none", i1);
    fprintf(f_salida_html, "<!-- MEM -->\n");
    fprintf(f_salida_html, "%s\n", mem->dibujo);
}

/***************************************************************
 *
 * Func: genera_MD
 *
 * Desc: Dibuja el símbolo de la memoria
 *
 **************************************************************/

static void genera_MD(elem_2i1o_t* mem, int x1, int y1, int ancho, int alto, char* tcolor, char* color, char* relleno) {
    int x2;
    int y2;
    char i1[50];
    char i2[50];
    char o1[50];
    char nombre[10] = "Mem Datos";
    char in1[10] = "Dir";
    char in2[10] = "W";
    char out1[10] = "Dato";
    char control[10];

    x2 = x1 + ancho;
    y2 = y1 + alto;

    mem->pos1.x = x1;
    mem->pos1.y = y1;
    mem->pos2.x = x2;
    mem->pos2.y = y2;
    mem->ancho = ancho;
    mem->alto = alto;
    mem->in1.x = x1;
    mem->in1.y = y1 + alto / 4;
    mem->in2.x = x1;
    mem->in2.y = y2 - alto / 4;
    mem->out1.x = x2;
    mem->out1.y = y1 + alto / 2;
    inicia_dibujo(mem->dibujo);
    dibuja(mem->dibujo, "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", x1, y1, x1, y1 + alto, x1 + ancho, y1 + alto, x1 + ancho, y1, x1, y1, relleno, color, trazo_grueso);
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n", x1 + ancho / 2, (int) y1 + 7 * alto / 8, color, "none", nombre);
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n", mem->in1.x + 5, mem->in1.y, color, "none", in1);
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n", mem->in2.x + 5, mem->in2.y, color, "none", in2);
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n", mem->out1.x - 5, mem->out1.y, color, "none", out1);
    if (inst_load(EX_MEM.IR.codop) || inst_store(EX_MEM.IR.codop)) {
        imprime_valor(MEM.dir, 10000, i1);
        dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n", mem->in1.x + 25, mem->in1.y, tcolor, "none", i1);
    }
    if (inst_load(EX_MEM.IR.codop)) {
        sprintf(control, "Read");
        imprime_valor(MEM_WBn.MEMout, 10000, o1);
        dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n", mem->out1.x - 5, mem->out1.y + 15, tcolor, "none", o1);
    }
    if (inst_store(EX_MEM.IR.codop)) {
        sprintf(control, "Write");
        imprime_valor(EX_MEM.data, 10000, i2);
        dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n", mem->in2.x + 25, mem->in2.y, tcolor, "none", i2);
    }
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n</g>\n",
            mem->pos1.x, mem->pos1.y - 5, tcolor, "none", control);
    fprintf(f_salida_html, "<!-- MEM -->\n");
    fprintf(f_salida_html, "%s\n", mem->dibujo);
}

/***************************************************************
 *
 * Func: genera_elipse
 *
 * Desc: Dibuja una elipse
 *
 **************************************************************/

static void genera_elipse(elem_1i1o_t* mem, int x1, int y1, int ancho, int alto, char* nombre, char* in1, char* out1, char* tcolor, char* color, char* relleno) {
    int x2;
    int y2;
    int cx, cy, rx, ry;
    char i1[50];
    char o1[5];

    x2 = x1 + ancho;
    y2 = y1 + alto;
    rx = ancho / 2;
    ry = alto / 2;
    cx = x1 + rx;
    cy = y1 + ry;

    mem->pos1.x = x1;
    mem->pos1.y = y1;
    mem->pos2.x = x2;
    mem->pos2.y = y2;
    mem->ancho = ancho;
    mem->alto = alto;
    mem->in1.x = x1;
    mem->in1.y = y1 + alto / 2;
    mem->out1.x = x2;
    mem->out1.y = y1 + alto / 2;
    inicia_dibujo(mem->dibujo);
    dibuja(mem->dibujo, "<g>\n  <ellipse cx=\"%d\" cy=\"%d\" rx=\"%d\" ry=\"%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", cx, cy, rx, ry, relleno, color, trazo_grueso);
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n", x1 + ancho / 2, mem->out1.y + 15, color, "none", nombre);
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %s\n</text>\n", mem->in1.x, mem->in1.y, tcolor, "none", in1);
    dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n", mem->out1.x, mem->out1.y, tcolor, "none", out1);

    if (IF_ID.IR.tipo == FormatoI) {
        imprime_valor(ID_EXn.Imm, 10000, o1);
        dibuja(mem->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n", mem->out1.x - 5, mem->out1.y, tcolor, "none", o1);
    }
    fprintf(f_salida_html, "%s\n", mem->dibujo);
}

/***************************************************************
 *
 * Func: genera_latch
 *
 * Desc: Dibuja un registro de 1 entrada
 *
 **************************************************************/

static void genera_latch(elem_1i1o_t* reg, int x1, int y1, int ancho, int alto, char* nombre, char* in1, char* tcolor, char* color, char * relleno) {
    int x2;
    int y2;

    x2 = x1 + ancho;
    y2 = y1 + alto;

    reg->pos1.x = x1;
    reg->pos1.y = y1;
    reg->pos2.x = x2;
    reg->pos2.y = y2;
    reg->ancho = ancho;
    reg->alto = alto;
    reg->in1.x = x1;
    reg->in1.y = y1 + alto / 2;
    reg->out1.x = x2;
    reg->out1.y = y1 + alto / 2;
    inicia_dibujo(reg->dibujo);
    dibuja(reg->dibujo, "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", x1, y1, x1, y1 + alto, x1 + ancho, y1 + alto, x1 + ancho, y1, x1, y1, relleno, color, trazo_grueso);
    dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n", x1 + ancho / 2, y1 - 10, color, tcolor, nombre);
    dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n</g>\n", reg->in1.x + ancho / 2, reg->in1.y - 2, tcolor, "none", in1);
    fprintf(f_salida_html, "<!-- LATCH -->\n");
    fprintf(f_salida_html, "%s\n", reg->dibujo);
}

/*
 void genera_latch2 (elem_2i2o_t* reg, int x1, int y1, int ancho, int alto, int i1_y, int i2_y)
 {
        int x2;
        int y2;

        x2 = x1+ancho;
        y2 = y1+alto;

        reg->pos1.x = x1;
        reg->pos1.y = y1;
        reg->pos2.x = x2;
        reg->pos2.y = y2;
        reg->ancho = ancho;
        reg->alto = alto;
        reg->in1.x = x1;
        reg->in1.y = i1_y;
        reg->out1.x = x2;
        reg->out1.y = i1_y;
        reg->in2.x = x1;
        reg->in2.y = i2_y;
        reg->out2.x = x2;
        reg->out2.y = i2_y;
 sprintf (reg->dibujo, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:purple;stroke-width:3\"/>\n", x1,y1,x1,y1+alto,x1+ancho,y1+alto,x1+ancho,y1,x1,y1,"none");
        fprintf (f_salida_html,reg->dibujo);
 }


 void genera_latch3 (elem_3i3o_t* reg, int x1, int y1, int ancho, int alto, int i1_y, int i2_y, int i3_y)
 {
        int x2;
        int y2;

        x2 = x1+ancho;
        y2 = y1+alto;

        reg->pos1.x = x1;
        reg->pos1.y = y1;
        reg->pos2.x = x2;
        reg->pos2.y = y2;
        reg->ancho = ancho;
        reg->alto = alto;
        reg->in1.x = x1;
        reg->in1.y = i1_y;
        reg->out1.x = x2;
        reg->out1.y = i1_y;
        reg->in2.x = x1;
        reg->in2.y = i2_y;
        reg->out2.x = x2;
        reg->out2.y = i2_y;
        reg->in3.x = x1;
        reg->in3.y = i3_y;
        reg->out3.x = x2;
        reg->out3.y = i3_y;
 sprintf (reg->dibujo, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:purple;stroke-width:3\"/>\n", x1,y1,x1,y1+alto,x1+ancho,y1+alto,x1+ancho,y1,x1,y1,"none");
        fprintf (f_salida_html,reg->dibujo);
 }

 void genera_latch4 (elem_4i4o_t* reg, int x1, int y1, int ancho, int alto, int i1_y, int i2_y, int i3_y, int i4_y, char* nombre, char* in1, char* in2, char* in3, char* in4, char* tcolor, char* color, char * relleno)
 {
        int x2;
        int y2;

        x2 = x1+ancho;
        y2 = y1+alto;

        reg->pos1.x = x1;
        reg->pos1.y = y1;
        reg->pos2.x = x2;
        reg->pos2.y = y2;
        reg->ancho = ancho;
        reg->alto = alto;
        reg->in1.x = x1;
        reg->in1.y = i1_y;
        reg->out1.x = x2;
        reg->out1.y = i1_y;
        reg->in2.x = x1;
        reg->in2.y = i2_y;
        reg->out2.x = x2;
        reg->out2.y = i2_y;
        reg->in3.x = x1;
        reg->in3.y = i3_y;
        reg->out3.x = x2;
        reg->out3.y = i3_y;
        reg->in4.x = x1;
        reg->in4.y = i4_y;
        reg->out4.x = x2;
        reg->out4.y = i4_y;
 sprintf (reg->dibujo, "<g>  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:3\"/>\n", x1,y1,x1,y1+alto,x1+ancho,y1+alto,x1+ancho,y1,x1,y1,relleno,color);
        dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\"\n    >%s</text>\n", x1+ancho/2, y1-10, color, "none", nombre);
        dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\"\n    >%s</text>\n", reg->in1.x+ancho/2, reg->in1.y-2, tcolor, "none", in1);
        dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\"\n    >%s</text>\n", reg->in2.x+ancho/2, reg->in2.y-2, tcolor, "none", in2);
        dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\"\n    >%s</text>\n", reg->in3.x+ancho/2, reg->in3.y-2, tcolor, "none", in3);
        dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\"\n    >%s</text>\n</g>\n", reg->in4.x+ancho/2, reg->in4.y-2, tcolor, "none", "B");
        fprintf (f_salida_html,reg->dibujo);
 }

 void genera_latch5 (elem_5i5o_t* reg, int x1, int y1, int ancho, int alto, int i1_y, int i2_y, int i3_y, int i4_y, int i5_y)
 {
        int x2;
        int y2;

        x2 = x1+ancho;
        y2 = y1+alto;

        reg->pos1.x = x1;
        reg->pos1.y = y1;
        reg->pos2.x = x2;
        reg->pos2.y = y2;
        reg->ancho = ancho;
        reg->alto = alto;
        reg->in1.x = x1;
        reg->in1.y = i1_y;
        reg->out1.x = x2;
        reg->out1.y = i1_y;
        reg->in2.x = x1;
        reg->in2.y = i2_y;
        reg->out2.x = x2;
        reg->out2.y = i2_y;
        reg->in3.x = x1;
        reg->in3.y = i3_y;
        reg->out3.x = x2;
        reg->out3.y = i3_y;
        reg->in4.x = x1;
        reg->in4.y = i4_y;
        reg->out4.x = x2;
        reg->out4.y = i4_y;
 reg->in5.x = x1;
        reg->in5.y = i5_y;
        reg->out5.x = x2;
        reg->out5.y = i5_y;
 sprintf (reg->dibujo, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:purple;stroke-width:3\"/>\n", x1,y1,x1,y1+alto,x1+ancho,y1+alto,x1+ancho,y1,x1,y1,"none");
        fprintf (f_salida_html,reg->dibujo);
 }
 */

/***************************************************************
 *
 * Func: genera_latch6
 *
 * Desc: Dibuja un registro de 6 entradas y 6 salidas
 *
 **************************************************************/

static void genera_latch6(elem_6i6o_t* reg, int x1, int y1, int ancho, int alto, int i1_y, int i2_y, int i3_y, int i4_y, int i5_y, int i6_y, char* nombre, char* in1, char* in2, char* in3, char* in4, char* in5, char* in6, char* tcolor, char* color, char * relleno) {
    int x2;
    int y2;

    x2 = x1 + ancho;
    y2 = y1 + alto;

    reg->pos1.x = x1;
    reg->pos1.y = y1;
    reg->pos2.x = x2;
    reg->pos2.y = y2;
    reg->ancho = ancho;
    reg->alto = alto;
    reg->in1.x = x1;
    reg->in1.y = i1_y;
    reg->out1.x = x2;
    reg->out1.y = i1_y;
    reg->in2.x = x1;
    reg->in2.y = i2_y;
    reg->out2.x = x2;
    reg->out2.y = i2_y;
    reg->in3.x = x1;
    reg->in3.y = i3_y;
    reg->out3.x = x2;
    reg->out3.y = i3_y;
    reg->in4.x = x1;
    reg->in4.y = i4_y;
    reg->out4.x = x2;
    reg->out4.y = i4_y;
    reg->in5.x = x1;
    reg->in5.y = i5_y;
    reg->out5.x = x2;
    reg->out5.y = i5_y;
    reg->in6.x = x1;
    reg->in6.y = i6_y;
    reg->out6.x = x2;
    reg->out6.y = i6_y;
    inicia_dibujo(reg->dibujo);
    dibuja(reg->dibujo, "<g>  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", x1, y1, x1, y1 + alto, x1 + ancho, y1 + alto, x1 + ancho, y1, x1, y1, relleno, color, trazo_grueso);
    dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n", x1 + ancho / 2, y1 - 10, color, "none", nombre);
    dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n", reg->in1.x + ancho / 2, reg->in1.y - 4, tcolor, "none", in1);
    dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n", reg->in2.x + ancho / 2, reg->in2.y - 4, tcolor, "none", in2);
    dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n", reg->in3.x + ancho / 2, reg->in3.y - 4, tcolor, "none", in3);
    dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n", reg->in4.x + ancho / 2, reg->in4.y - 4, tcolor, "none", in4);
    dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n", reg->in5.x + ancho / 2, reg->in5.y - 4, tcolor, "none", in5);
    dibuja(reg->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n</g>\n", reg->in6.x + ancho / 2, reg->in6.y - 2, tcolor, "none", in6);
    fprintf(f_salida_html, "<!-- LATCH6 -->\n");
    fprintf(f_salida_html, "%s\n", reg->dibujo);
}

/***************************************************************
 *
 * Func: genera_regs
 *
 * Desc: Dibuja un banco de registros
 *
 **************************************************************/


static void genera_regs(elem_4i2o_t* regs, int x1, int y1, int ancho, int alto, int in1, int in2, char* in12_col, int in3, int in4, char* in34_col, char* color, char* relleno) {
    int x2;
    int y2;
    char o1[8];
    char o2[8];

    x2 = x1 + ancho;
    y2 = y1 + alto;

    regs->pos1.x = x1;
    regs->pos1.y = y1;
    regs->pos2.x = x2;
    regs->pos2.y = y2;
    regs->ancho = ancho;
    regs->alto = alto;
    regs->in1.x = x1;
    regs->in1.y = y1 + alto / 6;
    regs->in2.x = x1;
    regs->in2.y = y1 + 2 * alto / 6;
    regs->in3.x = x1;
    regs->in3.y = y1 + 4 * alto / 6;
    regs->in4.x = x1;
    regs->in4.y = y1 + 5 * alto / 6;
    regs->out1.x = x2;
    regs->out1.y = y1 + alto / 4;
    regs->out2.x = x2;
    regs->out2.y = y1 + 3 * alto / 4;
    inicia_dibujo(regs->dibujo);
    dibuja(regs->dibujo, "<g>\n  <polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", x1, y1, x1, y1 + alto, x1 + ancho, y1 + alto, x1 + ancho, y1, x1, y1, relleno, color, trazo_grueso);
    dibuja(regs->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:middle\">\n    %s\n</text>\n", x1 + ancho / 2, (int) y1 + alto / 2, color, "none", "Regs");
    dibuja(regs->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    R%d\n</text>\n", regs->in1.x + 5, regs->in1.y, in12_col, "none", in1);
    dibuja(regs->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    R%d\n</text>\n", regs->in2.x + 5, regs->in2.y, in12_col, "none", in2);
    dibuja(regs->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    R%d\n</text>\n", regs->in3.x + 5, regs->in3.y, in34_col, "none", in3);
    dibuja(regs->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:start\">\n    %d\n</text>\n", regs->in4.x + 5, regs->in4.y, in34_col, "none", in4);
    dibuja(regs->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n", regs->out1.x - 5, regs->out1.y, color, "none", "A");
    dibuja(regs->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n</g>\n", regs->out1.x - 5, regs->out2.y, color, "none", "B");

    imprime_valor(ID_EXn.Ra, 10000, o1);
    imprime_valor(ID_EXn.Rb, 10000, o2);
    dibuja(regs->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n", regs->out1.x - 5, regs->out1.y - 15, in12_col, "none", o1);
    dibuja(regs->dibujo, "<text x=\"%d\" y=\"%d\" \n    style=\"fill:%s;stroke:%s;font-family:Arial;font-size:14;text-anchor:end\">\n    %s\n</text>\n", regs->out2.x - 5, regs->out2.y + 15, in12_col, "none", o2);


    fprintf(f_salida_html, "<!-- REGs -->\n");
    fprintf(f_salida_html, "%s\n", regs->dibujo);
}

/***************************************************************
 *
 * Func: genera_mux2
 *
 * Desc: Dibuja un multiplexor de 2 entradas.
 * Fijamos la posición de la salida
 * El in3 es el de control
 *
 **************************************************************/

static void genera_mux2(elem_3i1o_t* mux, int xi, int yo, int ancho, int alto, int selected, char* scolor, char* color, char* relleno) {
    int x1, x2, y1, y2;
    int radio;
    int lado;
    int xsel, ysel;

    x1 = xi;
    y1 = yo - alto / 2;
    //    x1 = xo;
    //    x1 = x2-ancho;

    //    y1 = yo-alto/2;
    //    y2 = y1+alto;
    x2 = x1 + ancho;
    y2 = y1 + alto;

    radio = ancho / 2;
    lado = alto - ancho;

    mux->pos1.x = x1;
    mux->pos1.y = y1;
    mux->pos2.x = x2;
    mux->pos2.y = y2;
    mux->ancho = ancho;
    mux->alto = alto;
    mux->in1.x = x1;
    mux->in1.y = y1 + radio;
    mux->in2.x = x1;
    mux->in2.y = y1 + radio + lado;
    mux->in3.x = x1 + radio;
    mux->in3.y = y1;
    mux->out1.x = x2;
    mux->out1.y = y1 + radio + lado / 2;

    if (selected == 0) {
        xsel = mux->in1.x;
        ysel = mux->in1.y;
    } else {
        xsel = mux->in2.x;
        ysel = mux->in2.y;
    }
    inicia_dibujo(mux->dibujo);
    dibuja(mux->dibujo, "<g>  <path d=\"M%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", x1, y1 + radio, radio, radio, x1 + ancho, y1 + radio, x1 + ancho, y1 + radio + lado, radio, radio, x1, y1 + radio + lado, x1, y1 + radio, relleno, color, trazo_grueso);
    dibuja(mux->dibujo, "<polyline points=\"%d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n</g>\n", xsel, ysel, mux->out1.x, mux->out1.y, "none", scolor, trazo_resaltado);
    fprintf(f_salida_html, "<!-- MUX2 -->\n");
    fprintf(f_salida_html, "%s\n", mux->dibujo);
}

/***************************************************************
 *
 * Func: genera_mux3
 *
 * Desc: Dibuja un multiplexor de 3 entradas // in4 es la de control
 *
 **************************************************************/

static void genera_mux3(elem_4i1o_t* mux, int xi, int yo, int ancho, int alto, int selected, char* scolor, char* color, char* relleno) {
    /*
     int x2;
     int y2;
     int radio;
     int lado;

     x2 = x1+ancho;
     y2 = y1+alto;
     radio = ancho/2;
     lado = (alto-ancho)/2;
     */
    int x1, x2, y1, y2;
    int radio;
    int lado;
    int xsel = 0, ysel = 0;

    x1 = xi;
    y1 = yo - alto / 2;
    //    x1 = xo;
    //    x1 = x2-ancho;

    //    y1 = yo-alto/2;
    //    y2 = y1+alto;
    x2 = x1 + ancho;
    y2 = y1 + alto;

    radio = ancho / 2;
    lado = alto - ancho;

    mux->pos1.x = x1;
    mux->pos1.y = y1;
    mux->pos2.x = x2;
    mux->pos2.y = y2;
    mux->ancho = ancho;
    mux->alto = alto;
    mux->in1.x = x1;
    mux->in1.y = y1 + radio + 0 * (lado / 2);
    //    mux->in1.y = y1 + radio + lado / 4;
    mux->in2.x = x1;
    mux->in2.y = y1 + radio + 1 * (lado / 2);
    //  mux->in2.y = y1 + radio + 2 * lado / 4;
    mux->in3.x = x1;
    mux->in3.y = y1 + radio + 2 * (lado / 2);
    // mux->in3.y = y1 + radio + 3 * lado / 4;
    mux->in4.x = x1 + radio;
    mux->in4.y = y1;
    mux->out1.x = x2;
    mux->out1.y = y1 + radio + lado / 2;

    if (selected == 0) {
        xsel = mux->in1.x;
        ysel = mux->in1.y;
    } else if (selected == 1) {
        xsel = mux->in2.x;
        ysel = mux->in2.y;
    } else if (selected == 2) {
        xsel = mux->in3.x;
        ysel = mux->in3.y;
    }
    inicia_dibujo(mux->dibujo);
    dibuja(mux->dibujo, "<g>  <path d=\"M%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", x1, y1 + radio, radio, radio, x1 + ancho, y1 + radio, x1 + ancho, y1 + radio + lado, radio, radio, x1, y1 + radio + lado, x1, y1 + radio, relleno, color, trazo_grueso);
    dibuja(mux->dibujo, "<polyline points=\"%d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n</g>\n", xsel, ysel, mux->out1.x, mux->out1.y, "none", scolor, trazo_resaltado);
    fprintf(f_salida_html, "<!-- MUX3 -->\n");
    fprintf(f_salida_html, "%s\n", mux->dibujo);


}

/***************************************************************
 *
 * Func: genera_mux4
 *
 * Desc: Dibuja un multiplexor de 4 entradas // in5 es la de control
 *
 **************************************************************/

static void genera_mux4(elem_5i1o_t* mux, int xi, int yo, int ancho, int alto, int selected, char* scolor, char* color, char* relleno) {
    /*
     int x2;
     int y2;
     int radio;
     int lado;

     x2 = x1+ancho;
     y2 = y1+alto;
     radio = ancho/2;
     lado = (alto-ancho)/2;
     */
    int x1, x2, y1, y2;
    int radio;
    int lado;
    int xsel, ysel;

    x1 = xi;
    y1 = yo - alto / 2;
    //    x1 = xo;
    //    x1 = x2-ancho;

    //    y1 = yo-alto/2;
    //    y2 = y1+alto;
    x2 = x1 + ancho;
    y2 = y1 + alto;

    radio = ancho / 2;
    lado = alto - ancho;

    mux->pos1.x = x1;
    mux->pos1.y = y1;
    mux->pos2.x = x2;
    mux->pos2.y = y2;
    mux->ancho = ancho;
    mux->alto = alto;
    mux->in1.x = x1;
    mux->in1.y = y1 + radio + 0 * (lado / 3);
    //    mux->in1.y = y1 + radio + lado / 5;
    mux->in2.x = x1;
    mux->in2.y = y1 + radio + 1 * (lado / 3);
    mux->in3.x = x1;
    mux->in3.y = y1 + radio + 2 * (lado / 3);
    mux->in4.x = x1;
    mux->in4.y = y1 + radio + 3 * (lado / 3);
    mux->in5.x = x1 + radio;
    mux->in5.y = y1;
    mux->out1.x = x2;
    mux->out1.y = y1 + radio + lado / 2;

    if (selected == 0) {
        xsel = mux->in1.x;
        ysel = mux->in1.y;
    } else if (selected == 1) {
        xsel = mux->in2.x;
        ysel = mux->in2.y;
    } else if (selected == 2) {
        xsel = mux->in3.x;
        ysel = mux->in3.y;
    } else {
        xsel = mux->in4.x;
        ysel = mux->in4.y;
    }
    inicia_dibujo(mux->dibujo);
    dibuja(mux->dibujo, "<g>  <path d=\"M%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", x1, y1 + radio, radio, radio, x1 + ancho, y1 + radio, x1 + ancho, y1 + radio + lado, radio, radio, x1, y1 + radio + lado, x1, y1 + radio, relleno, color, trazo_grueso);
    dibuja(mux->dibujo, "<polyline points=\"%d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n</g>\n", xsel, ysel, mux->out1.x, mux->out1.y, "none", scolor, trazo_resaltado);
    fprintf(f_salida_html, "<!-- MUX4 -->\n");
    fprintf(f_salida_html, "%s\n", mux->dibujo);

    /*
     sprintf (mux->dibujo, "<path d=\"M%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\n     A%d,%d 0 0,1 %d,%d\n     L%d,%d\"\n     style=\"fill:%s;stroke:purple;stroke-width:1\"/>\n", x1, y1+radio, radio, radio, x1+ancho, y1+radio, x1+ancho, y1+alto, radio, radio, x1, y1+alto, x1, y1+radio,"none");
     fprintf (f_salida_html,mux->dibujo); */
}

/***************************************************************
 *
 * Func: conecta
 *
 * Desc: Dibuja la conexión entre dos puntos
 *
 **************************************************************/

static void conecta(coord_t punto1, coord_t punto2, char* color) {

    int x_medio;
    x_medio = punto1.x + (punto2.x - punto1.x) / 2;

    fprintf(f_salida_html, "<!-- conecta -->\n");
    fprintf(f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n\n", punto1.x, punto1.y, x_medio, punto1.y, x_medio, punto2.y, punto2.x, punto2.y, "none", color, trazo_fino);
}

/***************************************************************
 *
 * Func: conecta_p
 *
 * Desc: Dibuja la conexión entre dos puntos
 *
 **************************************************************/

/*
static void conecta_p (coord_t punto1, coord_t punto2, int percent,  char* color)
{
    int ancho;
    int alto;

    ancho = punto2.x-punto1.x;
    alto = punto2.y-punto1.x;

    fprintf (f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", punto1.x,punto1.y,punto1.x+(int) ancho*percent/100,punto1.y,punto1.x+(int) ancho*percent/100,punto2.y, punto2.x, punto2.y,"none", color, trazo_fino) ;
}
 */

/***************************************************************
 *
 * Func: conecta_int
 *
 * Desc: Dibuja la conexión entre dos puntos
 *
 **************************************************************/

/*
static void conecta_int (coord_t punto1, coord_t punto2, int desp_xmedio, char* color)
{
    int x_medio;

    x_medio = punto1.x+desp_xmedio;

    fprintf (f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", punto1.x,punto1.y,x_medio,punto1.y,x_medio,punto2.y, punto2.x, punto2.y,"none", color, trazo_fino) ;
}
 */

/***************************************************************
 *
 * Func: conecta_p1_dx1_dy_dx2_p2
 *
 * Desc: Dibuja la conexión entre dos puntos p1 y p2: p1 -> rel_x, rel_y, rel_x -> p2
 *
 **************************************************************/

/*
static void conecta_p1_dx1_dy_dx2_p2 (coord_t punto1, coord_t punto2, int desp_xmedio1, int desp_ymedio, int desp_xmedio2, char* color)
{
    int x_medio1;
    int x_medio2;
    int y_medio1;

    x_medio1 = punto1.x+desp_xmedio1;
    x_medio2 = punto2.x+desp_xmedio2;
    y_medio1 = punto1.y+desp_ymedio;

    fprintf (f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", punto1.x,punto1.y,x_medio1,punto1.y,x_medio1,y_medio1,x_medio2,y_medio1,x_medio2, punto2.y ,punto2.x, punto2.y,"none", color, trazo_fino) ;
}
 */

/***************************************************************
 *
 * Func: conecta_p1_dx1_y_dx2_p2
 *
 * Desc: Dibuja la conexión entre dos puntos p1 y p2: p1 -> rel_x, abs_y, rel_x -> p2
 *
 **************************************************************/

static void conecta_p1_dx1_y_dx2_p2(coord_t punto1, coord_t punto2, int desp_xmedio1, int y_medio1, int desp_xmedio2, char* color) {

    int x_medio1;
    int x_medio2;
    //    int y_medio1;

    x_medio1 = punto1.x + desp_xmedio1;
    x_medio2 = punto2.x + desp_xmedio2;
    //    y_medio1 = punto1.y+desp_ymedio;

    fprintf(f_salida_html, "<!-- conecta_p1_dx1_y_dx2_p2 -->\n");
    fprintf(f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n\n", punto1.x, punto1.y, x_medio1, punto1.y, x_medio1, y_medio1, x_medio2, y_medio1, x_medio2, punto2.y, punto2.x, punto2.y, "none", color, trazo_fino);
}

/***************************************************************
 *
 * Func: conecta_p1_dx1_y1_dx2_y2_dx3_p2
 *
 * Desc: Dibuja la conexión entre dos puntos p1 y p2:
 * p1 -> rel_x, abs_y, rel_x, abs_y, rel_x -> p2
 *
 **************************************************************/

static void conecta_p1_dx1_y1_dx2_y2_dx3_p2(coord_t punto1, coord_t punto2, int desp_xmedio1, int y_medio1, int desp_xmedio2, int y_medio2, int desp_xmedio3, char* color) {

    int x_medio1;
    int x_medio2;
    int x_medio3;
    //    int y_medio1;

    x_medio1 = punto1.x + desp_xmedio1;
    x_medio2 = x_medio1 + desp_xmedio2;
    x_medio3 = punto2.x + desp_xmedio3;
    //    y_medio1 = punto1.y+desp_ymedio;

    fprintf(f_salida_html, "<!-- conecta_p1_dx1_y_dx2_p2 -->\n");
    fprintf(f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d %d,%d\"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n\n",
            punto1.x, punto1.y, x_medio1, punto1.y, x_medio1, y_medio1, x_medio2, y_medio1, x_medio2, y_medio2, x_medio3, y_medio2, x_medio3, punto2.y, punto2.x, punto2.y, "none", color, trazo_fino);
}

/***************************************************************
 *
 * Func: conecta_x
 *
 * Desc: Dibuja la conexión entre dos puntos p1 y p2 moviendo en x
 *
 **************************************************************/

/*
static void conecta_x (coord_t punto1, coord_t punto2, char* color)
{
    fprintf (f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", punto1.x,punto1.y,punto2.x,punto1.y,punto2.x, punto2.y,"none", color, trazo_fino) ;
}
 */

/***************************************************************
 *
 * Func: conecta_y
 *
 * Desc: Dibuja la conexión entre dos puntos p1 y p2, moviendo en y
 *
 **************************************************************/

/*
static void conecta_y (coord_t punto1, coord_t punto2, char* color)
{
    fprintf (f_salida_html, "<polyline points=\"%d,%d %d,%d %d,%d \"\n     style=\"fill:%s;stroke:%s;stroke-width:%f\"/>\n", punto1.x,punto1.y,punto1.x,punto2.y, punto2.x, punto2.y,"none", color, trazo_fino) ;
}
 */

/***************************************************************
 *
 * Func: dibuja_datapath
 *
 * Desc: Dibuja el datapath. Necesita que este abierto f_salida_html
 *
 **************************************************************/

void dibuja_datapath() {
    /*************************************/
    /*  Local variables                  */
    /*************************************/

    char aux[128];
    elem_1i1o_t RegPC, ext;

    elem_6i6o_t RegIF_ID, RegID_EX, RegEX_M, RegM_WB;
    elem_2i1o_t aluPC, aluEX, aluID, zeroID, zeroEX, mem_instr, mem_datos;
    elem_4i2o_t regs;
    // Mux 4 entradas
    elem_5i1o_t mux4_alu_s, mux4_alu_i;
    // Mux 3 entradas
    elem_4i1o_t mux3_zero_IDs, mux3_zero_IDi, mux3_zero_EXs, mux3_zero_EXi, mux_pc;
    // Mux 2 entradas
    elem_3i1o_t mux_rdst, mux2_alu_s, mux2_alu_i, mux_wb, mux2_W_ex, mux2_W_mem;

    muxctrl_t muxctrl; // para los MUX

    char col_IF[20], col_ID[20], col_EX[20], col_M[20], col_WB[20];

    /*
     char *	registro=
     "<symbol id=\"registro\">\n"
     "    <polygon points=\"0,0 20,0 20,60 0,60\" />\n"
     "</symbol>\n";

     char *	registro_intermedio=
     "<symbol id=\"registro_intermedio\">\n"
     "    <polygon points=\"0,0 20,0 20,200 0,200\" />\n"
     "</symbol>\n";

     char *	bancoreg=
     "<symbol id=\"bancoreg\">\n"
     "    <polygon points=\"0,0 40,0 40,80 0,80\" />\n"
     "</symbol>\n";

     char *	memoria=
     "<symbol id=\"memoria\">\n"
     "    <polygon points=\"0,0 40,0 40,80 0,80\" />\n"
     "</symbol>\n";

     char *	alu=
     "<symbol id=\"alu\">\n"
     "    <polyline points=\"0,0 0,40 10,50 0,60 0,100 40,80 40,30 0,0\" />\n"
     "</symbol>\n";


     char *	triangulo=
     "<symbol id=\"triangulo\">\n"
     "    <polyline points=\"10,2  60,2  35,52  10,2\""
     "    style=\"stroke:#006600; fill: #33cc33;\"/>\n"
     "</symbol>\n";
     */


    /*************************************/
    /*  Function body                    */
    /*************************************/


    /*** Datapath ***/

    // Colores
    color_instruccion(col_IF, PC);
    color_instruccion(col_ID, IF_ID.iPC);
    color_instruccion(col_EX, ID_EX.iPC);
    color_instruccion(col_M, EX_MEM.iPC);
    color_instruccion(col_WB, MEM_WB.iPC);


    /*
     sprintf(col_IF, "red");
     sprintf(col_ID, "blue");
     sprintf(col_EX,"green");
     sprintf(col_M, "magenta");
     sprintf(col_WB,"brown");
     */

    fprintf(f_salida_html, "<!-- Datapath en SVG -->\n");
    fprintf(f_salida_html, "<svg style='width: %s' viewbox=\"0 0 1100 600\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" shape-rendering=\"geometricPrecision\" >\n",
            SVG_WIDTH);
    //fprintf(f_salida_html, "<svg style='max-width: 820px' width=\"100%%\" viewbox=\"0 0 1100 600\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" shape-rendering=\"geometricPrecision\" >\n");
    //fprintf(f_salida_html, "<svg width=\"800\" viewbox=\"0 0 1100 650\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" shape-rendering=\"geometricPrecision\" >\n");
    //fprintf(f_salida_html, "<svg width=\"1100\" height=\"650\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" shape-rendering=\"geometricPrecision\" >\n");
    fprintf(f_salida_html, "<symbol id=\"datapath\">\n");

    imprime_instruccion_color(aux, PC, NO);
    escribe(PC_x + 4 * sep1x, eje_y_texto, aux, col_IF, 18);
    imprime_instruccion_color(aux, IF_ID.iPC, NO);
    escribe(IFID_x + 10 * sep1x, eje_y_texto, aux, col_ID, 18);
    imprime_instruccion_color(aux, ID_EX.iPC, NO);
    escribe(IDEX_x + 4 * sep1x, eje_y_texto, aux, col_EX, 18);
    imprime_instruccion_color(aux, EX_MEM.iPC, NO);
    escribe(EXM_x + 4 * sep1x, eje_y_texto, aux, col_M, 18);
    imprime_instruccion_color(aux, MEM_WB.iPC, NO);
    escribe(MWB_x + 3 * sep1x, eje_y_texto, aux, col_WB, 18);

    /* Etapa IF **/
    genera_latch(&RegPC, PC_x, PC_y, reg_ancho, PC_alto, "", "PC", "black", "black", "lightgray");
    genera_MI(&mem_instr, mem_instr_x, mem_instr_y, mem_ancho, mem_alto, col_IF, "black", "lightgray");
    conecta(RegPC.out1, mem_instr.in1, col_IF);
    genera_sum(&aluPC, aluPC_x, aluPC_y, aluPC_ancho, aluPC_alto, "+", "black", "lightgray", col_IF, 4, PC, IF_IDn.NPC);
    //escribe(aluPC.in1.x - 20, aluPC.in1.y, "4", "black", 14);
    conecta(RegPC.out1, aluPC.in2, col_IF);
    escribe(PC_x, PC_y + PC_alto + 20, (IFstall ? "IFstall" : ""), "black", 14);

    // mux PC
    // muxctrl.ctrl = (SaltoEfectivo ? 0 : 2);
    muxctrl.ctrl = 2; // No salta
    if latencia_salto1(solucion_riesgos_control) {
        if (SaltoEfectivo) {
            if inst_jr(IF_ID.IR.codop)
                muxctrl.ctrl = 1; // JR o JALR
            else
                muxctrl.ctrl = 0;
            sprintf(muxctrl.color, "%s", col_ID);
        } else {
            sprintf(muxctrl.color, "%s", col_IF);
        }
    } else if latencia_salto2(solucion_riesgos_control) {
        if (SaltoEfectivo) {
            muxctrl.ctrl = 0;
            sprintf(muxctrl.color, "%s", col_EX);
        } else {
            sprintf(muxctrl.color, "%s", col_IF);
        }
    } else {
        if (SaltoEfectivo) {
            muxctrl.ctrl = 0;
            sprintf(muxctrl.color, "%s", col_M);
        } else {
            sprintf(muxctrl.color, "%s", col_IF);
        }
    }
    //    genera_mux2(&mux_pc, muxpc_x, muxpc_y, mux2_ancho, mux2_alto, muxctrl.ctrl, muxctrl.color, "black", "lightgray");
    genera_mux3(&mux_pc, muxpc_x, muxpc_y, mux3_ancho, mux3_alto, muxctrl.ctrl, muxctrl.color, "black", "lightgray");

    conecta_p1_dx1_y_dx2_p2(aluPC.out1, mux_pc.in3, sep1x, mux_pc.in3.y, -sep1x, col_IF);

    genera_latch6(&RegIF_ID, IFID_x, IFID_y, reg_ancho, reg_alto, eje_y4_IF, mem_instr.out1.y, 0, 0, 0, 0, "IF_ID", "npc", "IR", "", "", "", "", "red", "gray", "lightgray");
    escribe(IFID_x, IFID_y + reg_alto + 20, (IDstall ? "IDstall" : ""), "black", 14);


    conecta_p1_dx1_y_dx2_p2(aluPC.out1, RegIF_ID.in1, sep1x, RegIF_ID.in1.y, 0, col_IF);
    conecta(mem_instr.out1, RegIF_ID.in2, col_IF);
    conecta_p1_dx1_y_dx2_p2(mux_pc.out1, RegPC.in1, sep1x, aluPC.pos2.y + (RegPC.pos1.y - aluPC.pos2.y) / 2, -sep1x, col_IF);

    /* Etapa ID **/
    genera_regs(&regs, regs_x, regs_y, regs_ancho, regs_alto, IF_ID.IR.Rfuente1, IF_ID.IR.Rfuente2, col_ID, MEM_WB.IR.Rdestino, WB.data, col_WB, "black", "lightgray");
    genera_elipse(&ext, ext_x, ext_y, elipse_ancho, elipse_alto, "Ext Signo", "", "", col_ID, "black", "lightgray");
    // Control
    //genera_elipse (&ctrl, control_x, control_y, elipse_ancho,elipse_alto, "Control", "", "", "black", "black", "lightgray");

    conecta_p1_dx1_y_dx2_p2(RegIF_ID.out2, regs.in1, sep1x, regs.in1.y, 0, col_ID);
    conecta_p1_dx1_y_dx2_p2(RegIF_ID.out2, regs.in2, sep1x, regs.in2.y, 0, col_ID);
    conecta_p1_dx1_y_dx2_p2(RegIF_ID.out2, ext.in1, sep1x, ext.in1.y, 0, col_ID);
    // Control
    // conecta_p1_dx1_y_dx2_p2 (RegIF_ID.out2, ctrl.in1, sep1x, ctrl.in1.y, 0, col_ID);

    if latencia_salto1(solucion_riesgos_control) {
        genera_latch6(&RegID_EX, IDEX_x, IDEX_y, reg_ancho, reg_alto, 0, RegIF_ID.out1.y, regs.out1.y, regs.out2.y, ext.out1.y, ext.pos2.y + 3 * sep1y, "ID_EX", "", "", "A", "B", "Imm", "IR", "red", "gray", "lightgray");
    } else {
        genera_latch6(&RegID_EX, IDEX_x, IDEX_y, reg_ancho, reg_alto, 0, RegIF_ID.out1.y, regs.out1.y, regs.out2.y, ext.out1.y, ext.pos2.y + 3 * sep1y, "ID_EX", "", "npc", "A", "B", "Imm", "IR", "red", "gray", "lightgray");
    }

    // mux Rdst
    muxctrl.ctrl = (IF_ID.IR.tipo == FormatoR ? 0 : 1);
    genera_mux2(&mux_rdst, muxid_x, muxid_y, mux2_ancho, mux2_alto, muxctrl.ctrl, col_ID, "black", "lightgray");
    conecta_p1_dx1_y_dx2_p2(RegIF_ID.out2, mux_rdst.in2, sep1x, mux_rdst.in2.y, 0, col_ID);
    conecta_p1_dx1_y_dx2_p2(RegIF_ID.out2, mux_rdst.in1, sep1x, mux_rdst.in1.y, 0, col_ID);

    conecta(regs.out1, RegID_EX.in3, col_ID);
    conecta(regs.out2, RegID_EX.in4, col_ID);
    conecta(ext.out1, RegID_EX.in5, col_ID);
    // Control
    // conecta (ctrl.out1, RegID_EX.in1, col_ID);
    conecta(mux_rdst.out1, RegID_EX.in6, col_ID);

    // Si ds1
    if latencia_salto1(solucion_riesgos_control) {
        // Cmp Zero
        genera_cmp(&zeroID, zeroid_x, zeroid_y, zero_ancho, zero_alto, "cmp", "black", "lightgray", ((inst_branch(IF_ID.IR.codop)&&!IFstall) ? col_ID : "lightgray"), ID.compin1, ID.compin2, ID.cond_out);
        // genera_latch(&zero_id, zeroid_x, zeroid_y, zero_ancho, zero_alto, "", "zero", "red", "black", "lightgray");
        // Calculador nuevo PC
        genera_sum(&aluID, aluid_x, aluid_y, aluPC_ancho, aluPC_alto, "+", "black", "lightgray", ((inst_branch(IF_ID.IR.codop)&&!IFstall) ? col_ID : "lightgray"), IF_ID.NPC, (word) IF_ID.IR.inmediato, ID.dir);
        conecta_p1_dx1_y_dx2_p2(ext.out1, aluID.in2, sep1x, ext.pos1.y - sep1y, -sep1x, col_ID);
        conecta_p1_dx1_y_dx2_p2(RegIF_ID.out1, aluID.in1, 2 * sep1x, aluID.in1.y, 0, col_ID);
        // conexion con el mux_pc
        conecta_p1_dx1_y_dx2_p2(zeroID.out1, mux_pc.in4, sep1x, RegIF_ID.pos1.y - 3 * sep1y, 0, col_ID);
        conecta_p1_dx1_y_dx2_p2(aluID.out1, mux_pc.in1, sep1x, RegIF_ID.pos1.y - 4 * sep1y, -sep1x, col_ID);
        conecta_p1_dx1_y1_dx2_y2_dx3_p2(regs.out1, mux_pc.in2, sep1x, regs.pos1.y - sep1y, -6 * sep1x, RegIF_ID.pos1.y - 5 * sep1y, -2 * sep1x, col_ID);

        // Si no hay cortocircuitos
        if (solucion_riesgos_datos != cortocircuito) {
            conecta_p1_dx1_y_dx2_p2(regs.out1, zeroID.in1, sep1x, regs.pos1.y - sep1y, -2 * sep1x, col_ID);
            conecta_p1_dx1_y_dx2_p2(regs.out2, zeroID.in2, 2 * sep1x, regs.pos1.y - 2 * sep1y, -sep1x, col_ID);
        }
        if (solucion_riesgos_datos == cortocircuito) {
            // mux ZERO en ID
            if (MEMaIDcomp_s) {
                muxctrl.ctrl = 0;
                sprintf(muxctrl.color, "%s", col_M);
            } else {
                muxctrl.ctrl = 2;
                sprintf(muxctrl.color, "%s", col_ID);
            }
            // mux sup
            genera_mux3(&mux3_zero_IDs, muxZeroID_x, zeroID.in1.y, mux3_ancho, mux3_alto, muxctrl.ctrl, muxctrl.color, "black", "lightgray");
            conecta(mux3_zero_IDs.out1, zeroID.in1, col_ID);
            conecta_p1_dx1_y_dx2_p2(regs.out1, mux3_zero_IDs.in3, sep1x, regs.pos1.y - sep1y, -2 * sep1x, col_ID);

            // mux ZERO en ID
            if (MEMaIDcomp_i) {
                muxctrl.ctrl = 0;
                sprintf(muxctrl.color, "%s", col_M);
            } else {
                muxctrl.ctrl = 2;
                sprintf(muxctrl.color, "%s", col_ID);
            }
            // mux inf
            genera_mux3(&mux3_zero_IDi, muxZeroID_x, zeroID.in2.y, mux3_ancho, mux3_alto, muxctrl.ctrl, muxctrl.color, "black", "lightgray");
            conecta(mux3_zero_IDi.out1, zeroID.in2, col_ID);
            conecta_p1_dx1_y_dx2_p2(regs.out2, mux3_zero_IDi.in3, 2 * sep1x, regs.pos1.y - 2 * sep1y, -sep1x, col_ID);
        }
    }

    if (!latencia_salto1(solucion_riesgos_control)) {
        conecta(RegIF_ID.out1, RegID_EX.in2, col_ID);
    }



    /* Etapa EX **/
    genera_ALU(&aluEX, alu_x, alu_y, alu_ancho, alu_alto, "ALU", "black", "lightgray", col_EX, EX.aluin1, EX.aluin2, EX.aluout);

    // Si ds1
    if latencia_salto1(solucion_riesgos_control) {
        genera_latch6(&RegEX_M, EXM_x, EXM_y, reg_ancho, reg_alto, RegID_EX.out1.y, zeroID.out1.y, aluEX.out1.y, RegID_EX.out5.y - sep1y, RegID_EX.out6.y, 0, "EX_M", "", "", "ALUout", "B", "IR", "", "red", "gray", "lightgray");
    }

    // Si ds2
    if latencia_salto2(solucion_riesgos_control) {
        genera_cmp(&zeroEX, ZeroEX_x, ZeroEX_y, zero_ancho, zero_alto, "cmp", "black", "lightgray", (inst_branch(ID_EX.IR.codop) ? col_EX : "lightgray"), EX.compin1, EX.compin2, EX.cond_out);
        genera_latch6(&RegEX_M, EXM_x, EXM_y, reg_ancho, reg_alto, RegID_EX.out1.y, zeroEX.out1.y, aluEX.out1.y, aluEX.pos2.y + 5 * sep1y, RegID_EX.out6.y, 0, "EX_M", "", "cond", "ALUout", "B", "IR", "", "red", "gray", "lightgray");
        // ctrl
        conecta_p1_dx1_y_dx2_p2(zeroEX.out1, mux_pc.in4, sep1x, RegID_EX.pos1.y - 3 * sep1y, 0, col_EX);
        // dir destino
        conecta_p1_dx1_y_dx2_p2(aluEX.out1, mux_pc.in1, sep1x, RegID_EX.pos1.y - 5 * sep1y, -sep1x, col_EX);
    }

    // Si pnt o ds3
    if latencia_salto3(solucion_riesgos_control) {
        genera_cmp(&zeroEX, ZeroEX_x, ZeroEX_y, zero_ancho, zero_alto, "cmp", "black", "lightgray", (inst_branch(ID_EX.IR.codop) ? col_EX : "lightgray"), EX.compin1, EX.compin2, EX.cond_out);
        genera_latch6(&RegEX_M, EXM_x, EXM_y, reg_ancho, reg_alto, RegID_EX.out1.y, zeroEX.out1.y, aluEX.out1.y, aluEX.pos2.y + 5 * sep1y, RegID_EX.out6.y, 0, "EX_M", "", "cond", "ALUout", "B", "IR", "", "red", "gray", "lightgray");
        // ctrl
        conecta(zeroEX.out1, RegEX_M.in2, col_EX);
    }


    // Si no hay cortocircuitos
    if (solucion_riesgos_datos != cortocircuito) {
        if (!latencia_salto1(solucion_riesgos_control)) {
            // mux ALUs
            if (inst_branch(ID_EX.IR.codop)) {
                muxctrl.ctrl = 0;
            } else {
                muxctrl.ctrl = 1;
            }
            genera_mux2(&mux2_alu_s, mux2s_x, aluEX.in1.y, mux2_ancho, mux2_alto, muxctrl.ctrl, col_EX, "black", "lightgray");
            conecta(mux2_alu_s.out1, aluEX.in1, col_EX);
            // Reg A -> ALU
            conecta_p1_dx1_y_dx2_p2(RegID_EX.out3, mux2_alu_s.in2, RegA_x, mux2_alu_s.in2.y, 0, col_EX);
            // NPC
            conecta_p1_dx1_y_dx2_p2(RegID_EX.out2, mux2_alu_s.in1, NPC_x, mux2_alu_s.in1.y, 0, col_EX);
            // Reg A -> ZERO
            conecta_p1_dx1_y_dx2_p2(RegID_EX.out3, zeroEX.in1, RegA_x, zeroEX.in1.y, 0, col_EX);
            // Reg B -> ZERO
            conecta_p1_dx1_y_dx2_p2(RegID_EX.out4, zeroEX.in2, RegB_x, zeroEX.in2.y, 0, col_EX);
        } else { // Si latencia 1, no hay mux superior
            // conexion de A
            conecta_p1_dx1_y_dx2_p2(RegID_EX.out3, aluEX.in1, sep1x, aluEX.in1.y, 0, col_EX);
        }

        // mux ALUi
        muxctrl.ctrl = (ID_EX.IR.tipo == FormatoR ? 0 : 1);
        genera_mux2(&mux2_alu_i, mux2i_x, aluEX.in2.y, mux2_ancho, mux2_alto, muxctrl.ctrl, col_EX, "black", "lightgray");
        conecta(mux2_alu_i.out1, aluEX.in2, col_EX);
        // Reg B -> ALU
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out4, mux2_alu_i.in1, RegB_x, mux2_alu_i.in1.y, 0, col_EX);
        // Imm -> ALU
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out5, mux2_alu_i.in2, Imm_x, mux2_alu_i.in2.y, 0, col_EX);
        // conexion de B hacia EX/M
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out4, RegEX_M.in4, RegB_x, RegEX_M.in4.y, 0, col_EX);
    }

    // Si hay cortocircuitos
    if (solucion_riesgos_datos == cortocircuito) {
        // mux ALUs
        muxctrl.ctrl = EX.muxALU_sup;
        switch (muxctrl.ctrl) {
            case 0:
            case 3:
                sprintf(muxctrl.color, "%s", col_EX);
                break;
            case 1:
                sprintf(muxctrl.color, "%s", col_M);
                break;
            case 2:
                sprintf(muxctrl.color, "%s", col_WB);
                break;
            default:
                sprintf(muxctrl.color, "%s", "white");

        }
        genera_mux4(&mux4_alu_s, mux4s_x, aluEX.in1.y, mux4_ancho, mux4_alto, muxctrl.ctrl, muxctrl.color, "black", "lightgray");

        // mux ALUi
        muxctrl.ctrl = EX.muxALU_inf;
        switch (muxctrl.ctrl) {
            case 0:
            case 3:
                sprintf(muxctrl.color, "%s", col_EX);
                break;
            case 1:
                sprintf(muxctrl.color, "%s", col_M);
                break;
            case 2:
                sprintf(muxctrl.color, "%s", col_WB);
                break;
            default:
                sprintf(muxctrl.color, "%s", "white");
        }
        genera_mux4(&mux4_alu_i, mux4s_x, aluEX.in2.y, mux4_ancho, mux4_alto, muxctrl.ctrl, muxctrl.color, "black", "lightgray");

        // mux MEM en EX
        muxctrl.ctrl = (WBaEXmem ? 1 : 0);
        if (WBaEXmem) {
            sprintf(muxctrl.color, "%s", col_WB);
        } else {
            sprintf(muxctrl.color, "%s", col_EX);
        }
        genera_mux2(&mux2_W_ex, muxW_x, RegEX_M.in4.y, mux2_ancho, mux2_alto, muxctrl.ctrl, muxctrl.color, "black", "lightgray");

        conecta(mux4_alu_s.out1, aluEX.in1, col_EX);
        conecta(mux4_alu_i.out1, aluEX.in2, col_EX);
        // Reg A -> ALU
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out3, mux4_alu_s.in4, RegA_x, mux4_alu_s.in4.y, 0, col_EX);
        // Reg B -> ALU
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out4, mux4_alu_i.in1, RegB_x, mux4_alu_i.in1.y, 0, col_EX);

        if (!latencia_salto1(solucion_riesgos_control)) {
            // NPC -> mux ALUsup
            conecta_p1_dx1_y_dx2_p2(RegID_EX.out2, mux4_alu_s.in1, 3 * sep1x, mux4_alu_s.in1.y, 0, col_EX);
        }

        // Imm -> ALU
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out5, mux4_alu_i.in4, Imm_x, mux4_alu_i.in4.y, 0, col_EX);
        // Reg B -> Mem
        conecta_p1_dx1_y_dx2_p2(RegID_EX.out4, mux2_W_ex.in1, RegB_x, mux2_W_ex.in1.y, 0, col_EX);
        conecta(mux2_W_ex.out1, RegEX_M.in4, col_EX);

        // mux ZERO
        if (!latencia_salto1(solucion_riesgos_control)) {
            // mux sup
            if (WBaEXcomp_s) {
                muxctrl.ctrl = 2;
                sprintf(muxctrl.color, "%s", col_WB);
            } else if (MEMaEXcomp_s) {
                muxctrl.ctrl = 1;
                sprintf(muxctrl.color, "%s", col_M);
            } else {
                muxctrl.ctrl = 0;
                sprintf(muxctrl.color, "%s", col_EX);
            }
            // mux ZERO s
            genera_mux3(&mux3_zero_EXs, muxZeroEX_x, zeroEX.in1.y, mux3_ancho, mux3_alto, muxctrl.ctrl, muxctrl.color, "black", "lightgray");
            // Reg A -> muxZERO
            conecta_p1_dx1_y_dx2_p2(RegID_EX.out3, mux3_zero_EXs.in1, sep1x, mux3_zero_EXs.in1.y, 0, col_EX);
            conecta(mux3_zero_EXs.out1, zeroEX.in1, col_EX);

            // mux inf
            if (WBaEXcomp_i) {
                muxctrl.ctrl = 2;
                sprintf(muxctrl.color, "%s", col_WB);
            } else if (MEMaEXcomp_i) {
                muxctrl.ctrl = 1;
                sprintf(muxctrl.color, "%s", col_M);
            } else {
                muxctrl.ctrl = 0;
                sprintf(muxctrl.color, "%s", col_EX);
            }
            // mux ZERO s
            genera_mux3(&mux3_zero_EXi, muxZeroEX_x, zeroEX.in2.y, mux3_ancho, mux3_alto, muxctrl.ctrl, muxctrl.color, "black", "lightgray");
            // Reg B -> muxZERO
            conecta_p1_dx1_y_dx2_p2(RegID_EX.out4, mux3_zero_EXi.in1, 2 * sep1x, mux3_zero_EXi.in1.y, 0, col_EX);
            conecta(mux3_zero_EXi.out1, zeroEX.in2, col_EX);

        }
    }

    // ALUout
    conecta(aluEX.out1, RegEX_M.in3, col_EX);

    // Control
    // conecta (RegID_EX.out1, RegEX_M.in1, col_EX);
    conecta(RegID_EX.out6, RegEX_M.in5, col_EX);

    /* Etapa MEM **/

    genera_MD(&mem_datos, mem_datos_x, mem_datos_y, mem_ancho, mem_alto, col_M, "black", "lightgray");
    // Dir
    conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mem_datos.in1, 3 * sep1x, mem_datos.in1.y, 0, col_M);
    genera_latch6(&RegM_WB, MWB_x, MWB_y, reg_ancho, reg_alto, RegEX_M.out1.y, mem_datos.pos1.y - 3 * sep1y, mem_datos.out1.y, RegEX_M.out5.y, 0, 0, "MEM_WB", "", "ALUout", "LMD", "IR", "", "", "red", "gray", "lightgray");
    // Read
    conecta(mem_datos.out1, RegM_WB.in3, col_M);
    // ALUout
    conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, RegM_WB.in2, 3 * sep1x, RegM_WB.in2.y, 0, col_M);
    // Control conecta_p1_dx1_y_dx2_p2 (RegEX_M.out1, RegM_WB.in1, 3*sep1x, RegM_WB.in1.y, 0, col_M);
    // Control    conecta (RegEX_M.out1, RegM_WB.in1, col_M);
    // WBdata
    conecta(RegEX_M.out5, RegM_WB.in4, col_M);

    // Si no hay cortocircuitos
    if (solucion_riesgos_datos != cortocircuito) {
        // Write
        conecta(RegEX_M.out4, mem_datos.in2, col_M);
    } else {
        // mux MEM en MEM
        muxctrl.ctrl = (WBaMEM ? 1 : 0);
        if (WBaMEM) {
            sprintf(muxctrl.color, "%s", col_WB);
        } else {
            sprintf(muxctrl.color, "%s", col_M);
        }
        genera_mux2(&mux2_W_mem, muxmem_x, RegEX_M.in4.y + 5, mux2_ancho, mux2_alto, muxctrl.ctrl, muxctrl.color, "black", "lightgray");

        conecta(mux2_W_mem.out1, mem_datos.in2, col_M);
        conecta(RegEX_M.out4, mux2_W_mem.in1, col_M);
        // Corto MEMaEX
        conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux4_alu_s.in2, 3 * sep1x, RegEX_M.pos1.y - 4 * sep1y, -2 * sep1x, col_M);
        conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux4_alu_i.in2, 3 * sep1x, RegEX_M.pos1.y - 4 * sep1y, -2 * sep1x, col_M);
        if latencia_salto1(solucion_riesgos_control) {
            // MEMaID saltos
            conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux3_zero_IDs.in1, 3 * sep1x, RegEX_M.pos1.y - 4 * sep1y, -sep1x, col_M);
            conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux3_zero_IDi.in1, 3 * sep1x, RegEX_M.pos1.y - 4 * sep1y, -sep1x, col_M);
        } else {
            // MEMaEX saltos
            conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux3_zero_EXs.in2, 3 * sep1x, RegEX_M.pos1.y - 4 * sep1y, -2 * sep1x, col_M);
            conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux3_zero_EXi.in2, 3 * sep1x, RegEX_M.pos1.y - 4 * sep1y, -2 * sep1x, col_M);
        }
    }

    if latencia_salto3(solucion_riesgos_control) {
        // ctrl
        conecta_p1_dx1_y_dx2_p2(RegEX_M.out2, mux_pc.in4, sep1x, RegEX_M.pos1.y - 3 * sep1y, 0, col_M);
        // dir destino
        conecta_p1_dx1_y_dx2_p2(RegEX_M.out3, mux_pc.in1, 3 * sep1x, RegEX_M.pos1.y - 4 * sep1y, -sep1x, col_M);
    }


    /* Etapa WB **/

    // mux WB
    if (inst_load_int(MEM_WB.IR.codop))
        muxctrl.ctrl = 1;
    else
        muxctrl.ctrl = 0;

    genera_mux2(&mux_wb, muxwb_x, mem_datos.out1.y - mux2_alto / 2, mux2_ancho, mux2_alto, muxctrl.ctrl, col_WB, "black", "lightgray");

    conecta(RegM_WB.out2, mux_wb.in1, col_WB);
    conecta(RegM_WB.out3, mux_wb.in2, col_WB);
    // WB -> Reg W
    conecta_p1_dx1_y_dx2_p2(RegM_WB.out4, regs.in3, sep1x, mux_rdst.pos2.y + sep1y, -sep1x, col_WB);
    // WBdata -> W
    conecta_p1_dx1_y_dx2_p2(mux_wb.out1, regs.in4, sep1x, mux_rdst.pos2.y + 2 * sep1y, -2 * sep1x, col_WB);

    if (solucion_riesgos_datos == cortocircuito) {
        // WBaEX
        conecta_p1_dx1_y_dx2_p2(mux_wb.out1, mux4_alu_i.in3, sep1x, mux_rdst.pos2.y + 2 * sep1y, -sep1x, col_WB);
        conecta_p1_dx1_y_dx2_p2(mux_wb.out1, mux4_alu_s.in3, sep1x, mux_rdst.pos2.y + 2 * sep1y, -sep1x, col_WB);
        // WBaMEM
        conecta_p1_dx1_y_dx2_p2(mux_wb.out1, mux2_W_mem.in2, sep1x, mux_rdst.pos2.y + 2 * sep1y, -sep1x, col_WB);
        // WBaEXmem
        conecta_p1_dx1_y_dx2_p2(mux_wb.out1, mux2_W_ex.in2, sep1x, mux_rdst.pos2.y + 2 * sep1y, -sep1x, col_WB);

        /* Corto WB a ID. No es necesario si lectura y escritura simult�neas */
        /*
         if (solucion_riesgos_control == ds1) {
         conecta_p1_dx1_y_dx2_p2 (mux_wb.out1, mux3_zero_ID.in2, sep1x, RegEX_M.pos1.y-6*sep1y, -2*sep1x, col_WB);
         }
         */
        if (!latencia_salto1(solucion_riesgos_control)) {
            // WBaEX Saltos
            conecta_p1_dx1_y_dx2_p2(mux_wb.out1, mux3_zero_EXs.in3, sep1x, mux_rdst.pos2.y + 2 * sep1y, -sep1x, col_WB);
            conecta_p1_dx1_y_dx2_p2(mux_wb.out1, mux3_zero_EXi.in3, sep1x, mux_rdst.pos2.y + 2 * sep1y, -sep1x, col_WB);
        }
    }

    fprintf(f_salida_html, "</symbol> \n");


    fprintf(f_salida_html, "<use xlink:href=\"#datapath\"\n");
    //	fprintf(f_salida_html, "transform=\"translate(0) scale(0.77)\"/>\n");

    fprintf(f_salida_html, "</svg>\n\n");


} /* end dibuja_datapath */
