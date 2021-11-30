/**************************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * Author:  Sergio Sáez (ssaez@disca.upv.es)
 *              Pedro López (plopez@disca.upv.es)
 *              Salva Petit (spetit@disca.upv.es)
 *
 * File: main.c
 *
 * Description:
 *    Módulo principal del simulador
 *
 *************************************************************************/

#define main_C

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include <sys/stat.h>
#include <libgen.h>

/*** Definiciones Externas *************************************************/

#include "tipos.h"
#include "memoria.h"
#include "prediccion.h"
#include "presentacion.h"
#include "presentacion-html.h"
#include "presentacion-json.h"
#include "instrucciones.h"
#include "algoritmo.h"
#include "enlazador.h"
#include "mips_syscall.h"
#include "depurar.h"

extern void ensamblador(fichero_objeto_t *obj);

/*** Variables Globales ****************************************************/

extern int pos_datos;

/*** Funciones Locales *****************************************************/

/***************************************************************
 *
 * Func: inicializa_estructuras
 *
 * Desc: Inicializa las estructuras de la unidad de gestión de
 * instrucciones
 *
 **************************************************************/

static void inicializa_estructuras() {

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int i, oper;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /* Estadisticas */

    bzero(&estat, sizeof (estat));
    bzero(&max_util, sizeof (max_util));
    bzero(&aver_util, sizeof (max_util));

    /* Registros inter-etapas */

    IF_ISS_1.IR.codop = OP_NOP;
    IF_ISS_1.PC = -1;
    IF_ISS_1.orden = 0;
    IF_ISS_1.excepcion = EXC_NONE;

    IF_ISS_2 = IF_ISS_1;

    Control_1.Parar = NO;
    Control_1.Cancelar = NO;
    Control_1.NPC = 0;
    Control_1.Excepcion = NO;
    Control_1.orden = 1;

    Control_2 = Control_1;

    SIG_SYSCALL = NO;

    for (i = 0; i < NUM_VIAS_ISSUE; i++) {
        M_IF_ISS_1[i] = IF_ISS_1;
        M_IF_ISS_2[i] = IF_ISS_2;
    }

    /* Registros */

    for (i = 0; i < TAM_REGISTROS; i++) {
        Rfp[i].valor.fp_d = 0.0;
        Rfp[i].rob = MARCA_NULA;
        Rint[i].valor.int_d = 0;
        Rint[i].rob = MARCA_NULA;
        Rint_tipo[i] = t_ninguno;
        Rfp_tipo[i] = t_ninguno;
        Rint_dirty[i] = NO;
        Rfp_dirty[i] = NO;
    }

    Rint[REG_FPSR].rob = MARCA_NULA; /* FPSR */
    Rint[REG_FPSR].valor.int_d = 0; /* FPSR */
    Rint_tipo[REG_FPSR] = t_ninguno;
    Rint_dirty[REG_FPSR] = NO;

    /* Predictor */

    inicializa_prediccion();

    /* Estaciones de reserva */

    TAM_ESTACIONES = TAM_RS_ENTEROS + TAM_RS_SUMREST + TAM_RS_MULTDIV +
            TAM_BUFFER_CARGA + TAM_BUFFER_ALMACEN;

    for (i = 0; i < TAM_ESTACIONES; i++) {
        RS[i].OP = OP_NOP;
        RS[i].ocupado = NO;
        RS[i].Q1 = MARCA_NULA;
        RS[i].V1.int_d = 0;
        RS[i].Q2 = MARCA_NULA;
        RS[i].V2.int_d = 0;
        RS[i].direccion = 0;
        RS[i].resultado.int_d = 0;
        RS[i].condicion = NO;
        RS[i].estado = PENDIENTE;
        RS[i].confirm = NO;
        RS[i].rob = MARCA_NULA;
        RS[i].orden = 0;
    } /* endfor */

    /* Reorder Buffer */

    RB_inicio = 0;
    RB_fin = 0;
    RB_long = 0;

    INICIO_REORDER = 0;
    for (i = 0; i < TAM_REORDER; i++) {
        RB[i].OP = OP_NOP;
        RB[i].PC = -1;
        RB[i].ocupado = NO;
        RB[i].completado = NO;
        RB[i].dest = MARCA_NULA;
        RB[i].valor.int_d = 0;
        RB[i].condicion = NO;
        RB[i].prediccion = NO;
        RB[i].direccion = NO;
        RB[i].orden = 0;
    } /* endfor */
    FIN_REORDER = TAM_REORDER - 1;

    /* Operadores */

    if (NUM_OPER_ENTEROS + NUM_OPER_SUMREST + NUM_OPER_MULTDIV +
            NUM_OPER_DIRECCIONES + NUM_OPER_MEMDATOS > MAX_OPERADORES) {
        fprintf(stderr, "ERROR: Número de operadores excesiv. Actualice la constante MAX_OPERADORES (mínimo %d)\n",
                NUM_OPER_ENTEROS + NUM_OPER_SUMREST + NUM_OPER_MULTDIV +
                NUM_OPER_DIRECCIONES + NUM_OPER_MEMDATOS);
        exit(1);
    }

    if (TAM_RS_ENTEROS + TAM_RS_SUMREST + TAM_RS_MULTDIV +
            TAM_BUFFER_CARGA + TAM_BUFFER_ALMACEN > MAX_ESTACIONES) {
        fprintf(stderr,
                "ERROR: Número de estaciones de reserva excesivo. Actualice la constante MAX_ESTACIONES (mínimo %d)\n",
                TAM_RS_ENTEROS + TAM_RS_SUMREST + TAM_RS_MULTDIV +
                TAM_BUFFER_CARGA + TAM_BUFFER_ALMACEN);
        exit(1);
    }
    oper = 0;
    INICIO_OP_ENTEROS = oper;
    for (i = 0; i < NUM_OPER_ENTEROS; i++, oper++) {
        Op[oper].tipo = OPER_ENTEROS;
        Op[oper].ocupado = NO;
        Op[oper].codigo = 0;
        Op[oper].ciclo = 0;
        Op[oper].Teval = TEVAL_ENTEROS;
    }
    FIN_OP_ENTEROS = oper - 1;

    INICIO_OP_SUMREST = oper;
    for (i = 0; i < NUM_OPER_SUMREST; i++, oper++) {
        Op[oper].tipo = OPER_SUMREST;
        Op[oper].ocupado = NO;
        Op[oper].codigo = 0;
        Op[oper].ciclo = 0;
        Op[oper].Teval = TEVAL_SUMREST;
    }
    FIN_OP_SUMREST = oper - 1;

    INICIO_OP_MULTDIV = oper;
    for (i = 0; i < NUM_OPER_MULTDIV; i++, oper++) {
        Op[oper].tipo = OPER_MULTDIV;
        Op[oper].ocupado = NO;
        Op[oper].codigo = 0;
        Op[oper].ciclo = 0;
        Op[oper].Teval = TEVAL_MULTDIV;
    }
    FIN_OP_MULTDIV = oper - 1;

    INICIO_OP_DIRECCIONES = oper;
    for (i = 0; i < NUM_OPER_DIRECCIONES; i++, oper++) {
        Op[oper].tipo = OPER_DIRECCIONES;
        Op[oper].ocupado = NO;
        Op[oper].codigo = 0;
        Op[oper].ciclo = 0;
        Op[oper].Teval = TEVAL_DIRECCIONES;
    }
    FIN_OP_DIRECCIONES = oper - 1;

    INICIO_OP_MEMDATOS = oper;
    for (i = 0; i < NUM_OPER_MEMDATOS; i++, oper++) {
        Op[oper].tipo = OPER_MEMDATOS;
        Op[oper].ocupado = NO;
        Op[oper].codigo = 0;
        Op[oper].ciclo = 0;
        Op[oper].Teval = TEVAL_MEMDATOS;
    }
    FIN_OP_MEMDATOS = oper - 1;

    NUM_OPERADORES = oper;

    /* Bus común */

    BUS.ocupado = NO;
    BUS.codigo = MARCA_NULA;
    BUS.valor.int_d = 0;

    estado_previo = ht_create(TAM_ESTADO);

} /* end inicializa_estructuras */

/***************************************************************
 *
 * Func: mira_si_todo_vacio
 *
 * Retorna:  (boolean)
 *
 * Desc: Mira si queda alguna operación por acabar
 *
 **************************************************************/

static boolean mira_si_todo_vacio(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int i;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    for (i = INICIO_BUFFER_ALMACEN; i <= FIN_BUFFER_ALMACEN; i++) {
        if (SB[i].ocupado & SB[i].confirm)
            return (NO);
    } /* endfor */

    return (SI);

} /* end mira_si_todo_vacio */

#define obtiene_util(estr, campo) \
do { \
    int i;\
    int ocup = 0; \
    for (i = INICIO_##campo; i <= FIN_##campo; i++) { \
        if (estr[i].ocupado == SI) \
            ocup++; \
    } \
    if (ocup > max_util.campo) \
        max_util.campo = ocup; \
    aver_util.campo += ocup; \
} while(0)

/***************************************************************
 *
 * Func: analiza_ocupacion
 *
 * Retorna:  (boolean)
 *
 * Desc: Mira si queda alguna operación por acabar
 *
 **************************************************************/

static void analiza_ocupacion(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /* Estaciones de reserva */

    obtiene_util(RS, RS_ENTEROS);
    obtiene_util(RS, RS_SUMREST);
    obtiene_util(RS, RS_MULTDIV);
    obtiene_util(RS, BUFFER_CARGA);
    obtiene_util(RS, BUFFER_ALMACEN);

    obtiene_util(Op, OP_ENTEROS);
    obtiene_util(Op, OP_SUMREST);
    obtiene_util(Op, OP_MULTDIV);
    obtiene_util(Op, OP_DIRECCIONES);
    obtiene_util(Op, OP_MEMDATOS);

    obtiene_util(RB, REORDER);

} /* end analiza_ocupacion */

static void construye_programa(
        programa_t *prog
        ) {
			
    /* Fichero objeto */
    for (int i = 0; i < prog->n_objs; ++i) {
        ensamblador(&prog->l_objs[i]);
    }

    /*** Enlazar ***/

    inicializa_memoria(&prog->memoria);

    // Variable global para simplificar la implementación
    mmem = &prog->memoria;

    /*** Enlazar ***/

    enlazador(prog);

    /*** Cargar ***/

    cargador(prog);

}

/***************************************************************
 *
 * Func: ejecutar_codigo
 *
 *
 * Retorna:  RC  (int)
 *
 * Descripción:
 *   Ejecuta el bucle principal del computador
 *
 **************************************************************/

static void ejecutar_codigo
(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    //boolean final;
    boolean OVF_HTML = NO;

    unsigned int m;
    dword validNPC;
    boolean Ignorar;
    boolean Lanzada[MAX_VIAS_SUPERESCALAR];

    reg_control_t Bloque;

    int i;

    static FILE *f_consola;
    static FILE *f_log;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /*** Inicializacion del computador */

    Ciclo = 0;
    terminando = NO;
    final = NO;

    /* Borra archivo de consola */
    f_consola = fopen(console_name, "w");
    fclose(f_consola);

    /* Borra archivo de log */
    f_log = fopen(log_name, "w");
    fclose(f_log);

    fprintf(stdout, "*** EJECUCIÓN ***\n\n");

    imprime_inicio();

    /* Directiva de ensamblador .pc */
    Control_2.NPC = PC_inicial;

    /*** Bucle principal */

    for (m = 0; m < NUM_VIAS_ISSUE; m++) {
        Lanzada[m] = NO;
    }

    while (!final) {
        Control_1.Parar = NO;
        Control_1.Cancelar = NO;
        Ignorar = NO;

        Ciclo++; /* Incrementa el número de ciclos
				   ejecutados */

        inicio_ciclo(Ciclo);

        //printf("Ciclo %d\n", Ciclo);

        /*** Limpieza fuera de la vista ***/

        for (i = 0; i < TAM_ESTACIONES; i++) {
            if (!RS[i].ocupado) {
                RS[i].excepcion = EXC_NONE;
            }
        } /* endfor */

        /*** Fase: EX (1) **********/

        fase_EX_1();

        /*** Fase: COMMIT ***********/

        for (m = 0; m < NUM_VIAS_COMMIT; m++) {
            fase_COM();
        }

        /*** Fase: WB **************/

        for (m = 0; m < NUM_VIAS_BUS; m++) {
            fase_WB();
        }

        /*** Fase: ISSUE ***********/

        IFstall = NO;
        for (m = 0; m < NUM_VIAS_ISSUE; m++) {
            IF_ISS_2 = M_IF_ISS_2[m];
            IF_ISS_2.ignorar = Lanzada[m];
            fase_ISS();
            Lanzada[m] = (!Control_1.Parar || IF_ISS_2.cancelar);
            if (!Lanzada[m] || Control_1.Excepcion) {
                IFstall = SI;
            }
        }

        /** Estadísticas de ocupación */

        analiza_ocupacion();

        /*** Fase: EX (2) **********/

        fase_EX_2();

        /*** Fase: IF **************/

        Bloque = Control_2;
        validNPC = Bloque.NPC; // Sólo para inicializarlo

        for (m = 0; m < NUM_VIAS_ISSUE; m++) {
            // Engañando a la fase de búsqueda
            Control_2.NPC = Bloque.NPC - (Bloque.NPC % (NUM_VIAS_ISSUE * 4)) + (m * 4);

            fase_IF();

            Control_2.orden = Control_1.orden;

            if (!IFstall) {
                M_IF_ISS_1[m] = IF_ISS_1;

                if (Control_2.NPC < Bloque.NPC || Ignorar) {
                    // Esta instrucción no la queremos ejecutar
                    M_IF_ISS_1[m].cancelar = SI;
                } else if (IF_ISS_1.prediccion) {
                    // Ignorar el resto de instrucciones
                    Ignorar = SI;
                    validNPC = Control_1.NPC;
                } else {
                    // Instrucción normal
                    validNPC = Control_1.NPC;
                }
            }
        }

        if (IFstall) {
            Control_1.orden = Bloque.orden;
        }

        Control_1.NPC = validNPC;

        /*** Estadisticas ***/

        estat.ciclos++;

        if (Ciclo >= MAX_CICLOS_TOTAL) {
            fprintf(stderr, "\nATENCION: Numero de ciclos (%d) excesivo"
                    "\n          Ejecución detenida\n\n", MAX_CICLOS_TOTAL);
            final = SI;
        }

        if ((f_depuracion) && (Ciclo >= MAX_CICLOS_HTML) && !OVF_HTML) {
            fprintf(stderr, "\nATENCION: Numero de ciclos (%d) excesivo"
                    "\n          No se generan más ciclos para visualizar\n\n", MAX_CICLOS_HTML);
            OVF_HTML = SI;
	    ignora_firma();
        };

        if (f_depuracion == NO) {
            if ((Ciclo > 0) && (Ciclo % 2000) == 0) {
                printf(".");
                fflush(stdout);
            };
            if ((Ciclo > 0) && (Ciclo % 10000) == 0) {
                printf("%5uK", Ciclo / 1000);
                fflush(stdout);
            };
            if ((Ciclo > 0) && (Ciclo % 50000) == 0) {
                printf("\n");
            };
        }

        /*** Control del simulador ****************/

        if (terminando) {
            if (mira_si_todo_vacio()) {
                final = SI; /* Termina */
            } else {
                Control_2 = Bloque; /* Repetimos bloque */
                Control_2.Parar = SI;
            }
        } else if (!IFstall && !Control_1.Cancelar) { /* Ejecución normal */
            Control_2 = Control_1;
            for (m = 0; m < NUM_VIAS_ISSUE; m++) {
                M_IF_ISS_2[m] = M_IF_ISS_1[m];
                Lanzada[m] = NO;
            }
        } else if (IFstall) { /* Búsqueda parada */
            Control_2 = Bloque; /* Repetimos bloque */
            Control_2.Parar = NO;
        } else { /* Búsqueda cancelada */
            Control_2 = Control_1;
            IF_ISS_2.orden = IF_ISS_1.orden;
            IF_ISS_2.PC = IF_ISS_1.PC;
            for (m = 0; m < NUM_VIAS_ISSUE; m++) {
                M_IF_ISS_2[m].orden = M_IF_ISS_1[m].orden;
                M_IF_ISS_2[m].PC = M_IF_ISS_1[m].PC;
            }
        } /* endif */

        /*** Imprime el estado del los operadores, etc.  */

        if ((f_depuracion) && !OVF_HTML) {
            imprime_crono();
            imprime_estado();
            imprime_predictor();
        } /* endif */

        fin_ciclo(Ciclo);

    } /* endwhile */

    imprime_final();

} /* end ejecutar_codigo */

/***************************************************************
 *
 * Func: error_fatal
 *
 * Descripción:
 *   Muestra el mensaje y sale
 *
 **************************************************************/

static void error_fatal
(
        char *msg_format,
        char *msg_value
        ) {
    fprintf(stderr, msg_format, msg_value);
    exit(1);
} /* end error_fatal */

/***************************************************************
 *
 * Func: file_exists
 *
 *
 * Retorna:  (int)
 *
 * Descripción:
 *   Comprueba si existe el archivo
 *
 **************************************************************/

static int file_exists(char *filename) {
    struct stat buffer;

    return (stat(filename, &buffer) == 0);
}

/*** Funciones Exportables *************************************************/

#ifndef LIBMIPSOOO
#define LIBMIPSOOO 0

/***************************************************************
 *
 * Func: main
 *
 * Parametros:
 *	argc  (int) ; Número de parametros en linea
 *	argv[]  (char*) ; Texto de los parametros en linea
 *
 * Retorna:  RC  (int)
 *
 * Descripción:
 *   Función principal
 *
 **************************************************************/

int main
(
        int argc,
        char *argv[]
        ) {

#else
#define LIBMIPSOOO 1

/***************************************************************
 *
 * Func: libmipsooo_main
 *
 * Parametros:
 *      ensamblador_s (char*): Cadena con programa ensamblador a simular
 *      config_en_s (int): Leer la configuracion de la primera linea de ensamblador_s
 *      devuelve_json (int): Devolver el resultado de la simulacion representado en json
 *      NUM_INICIO_ENTEROS_p, TEVAL_ENTEROS_p, TAM_RS_ENTEROS_p, SEGMENTADO_ENTEROS_p (int): Config. operador enteros
 *      NUM_INICIO_SUMREST_p, TEVAL_SUMREST_p, TAM_RS_SUMREST_p, SEGMENTADO_SUMREST_p (int): Config. operador suma/resta c.f.
 *      NUM_INICIO_MULTDIV_p, TEVAL_MULTDIV_p, TAM_RS_MULTDIV_p, SEGMENTADO_MULTDIV_p (int): Config. operador mult/div
 *      NUM_INICIO_MEMDATOS_p, TEVAL_MEMDATOS_p, TAM_BUFFER_CARGA_p, TAM_BUFFER_ALMACEN_p, SEGMENTADO_MEMDATOS_p (int): Config. unidad memoria
 *      NUM_VIAS_ISSUE_p, NUM_VIAS_BUS_p, NUM_VIAS_COMMIT_p (int): Config. vias superescalar
 *      tipo_predictor_p, TAM_BUFFER_PREDIC_p (int): Config. predictor de saltos
 *      TAM_REORDER_p (int): Numero de entradas del ROB
 *      load_forwarding_p (int): 0: Load bypassing, !0: Load forwarding
 *
 * Retorna: resultado_htm (char*): Resultado de la simulacion (malloc). Es responsabilidad del que llama liberar con free.
 *
 * Descripcion:
 *   Funcion principal de la libreria (reemplaza a main)
 *
 **************************************************************/

char* libmipsooo_main
(
        char* ensamblador_s,
        int config_en_s,
        int devuelve_json,
        int NUM_INICIO_ENTEROS_p, int TEVAL_ENTEROS_p, int TAM_RS_ENTEROS_p, int SEGMENTADO_ENTEROS_p,
        int NUM_INICIO_SUMREST_p, int TEVAL_SUMREST_p, int TAM_RS_SUMREST_p, int SEGMENTADO_SUMREST_p,
        int NUM_INICIO_MULTDIV_p, int TEVAL_MULTDIV_p, int TAM_RS_MULTDIV_p, int SEGMENTADO_MULTDIV_p,
        int NUM_INICIO_MEMDATOS_p, int TEVAL_MEMDATOS_p, int TAM_BUFFER_CARGA_p, int TAM_BUFFER_ALMACEN_p, int SEGMENTADO_MEMDATOS_p,
        int NUM_VIAS_ISSUE_p, int NUM_VIAS_BUS_p, int NUM_VIAS_COMMIT_p,
        int tipo_predictor_p, int TAM_BUFFER_PREDIC_p,
        int TAM_REORDER_p,
        int load_forwarding_p
        ) {

    /*** Utiliza parametros _p si la configuracion no esta en ensamblador_s */

    if (!config_en_s) {

        NUM_INICIO_ENTEROS = NUM_INICIO_ENTEROS_p;
        TEVAL_ENTEROS = TEVAL_ENTEROS_p;
        TAM_RS_ENTEROS = TAM_RS_ENTEROS_p;
        CHECK_DEFAULT_VALUE(NUM_INICIO_ENTEROS);
        CHECK_DEFAULT_VALUE(TEVAL_ENTEROS);
        CHECK_DEFAULT_VALUE(TAM_RS_ENTEROS);
        NUM_OPER_ENTEROS = (SEGMENTADO_ENTEROS_p) ? NUM_INICIO_ENTEROS * TEVAL_ENTEROS : NUM_INICIO_ENTEROS;

        NUM_INICIO_SUMREST = NUM_INICIO_SUMREST_p;
        TEVAL_SUMREST = TEVAL_SUMREST_p;
        TAM_RS_SUMREST = TAM_RS_SUMREST_p;
        CHECK_DEFAULT_VALUE(NUM_INICIO_SUMREST);
        CHECK_DEFAULT_VALUE(TEVAL_SUMREST);
        CHECK_DEFAULT_VALUE(TAM_RS_SUMREST);
        NUM_OPER_SUMREST = (SEGMENTADO_SUMREST_p) ? NUM_INICIO_SUMREST * TEVAL_SUMREST : NUM_INICIO_SUMREST;

        NUM_INICIO_MULTDIV = NUM_INICIO_MULTDIV_p;
        TEVAL_MULTDIV = TEVAL_MULTDIV_p;
        TAM_RS_MULTDIV = TAM_RS_MULTDIV_p;
        CHECK_DEFAULT_VALUE(NUM_INICIO_MULTDIV);
        CHECK_DEFAULT_VALUE(TEVAL_MULTDIV);
        CHECK_DEFAULT_VALUE(TAM_RS_MULTDIV);
        NUM_OPER_MULTDIV = (SEGMENTADO_MULTDIV_p) ? NUM_INICIO_MULTDIV * TEVAL_MULTDIV : NUM_INICIO_MULTDIV;

        NUM_INICIO_MEMDATOS = NUM_INICIO_MEMDATOS_p;
        NUM_INICIO_DIRECCIONES = NUM_INICIO_MEMDATOS;
        TEVAL_MEMDATOS = TEVAL_MEMDATOS_p;
        TAM_BUFFER_CARGA = TAM_BUFFER_CARGA_p;
        TAM_BUFFER_ALMACEN = TAM_BUFFER_ALMACEN_p;
        CHECK_DEFAULT_VALUE(NUM_INICIO_MEMDATOS);
        CHECK_DEFAULT_VALUE(TEVAL_MEMDATOS);
        CHECK_DEFAULT_VALUE(TAM_BUFFER_CARGA);
        CHECK_DEFAULT_VALUE(TAM_BUFFER_ALMACEN);
        NUM_OPER_MEMDATOS = (SEGMENTADO_MEMDATOS_p) ? NUM_INICIO_MEMDATOS * TEVAL_MEMDATOS : NUM_INICIO_MEMDATOS;
        NUM_OPER_DIRECCIONES = NUM_INICIO_MEMDATOS;

        NUM_VIAS_ISSUE = NUM_VIAS_ISSUE_p;
        NUM_VIAS_BUS = NUM_VIAS_BUS_p;
        NUM_VIAS_COMMIT = NUM_VIAS_COMMIT_p;
        CHECK_DEFAULT_VALUE(NUM_VIAS_ISSUE);
        CHECK_DEFAULT_VALUE(NUM_VIAS_BUS);
        CHECK_DEFAULT_VALUE(NUM_VIAS_COMMIT);

        switch (tipo_predictor_p) {
            case 0:
                tipo_predictor = BTB_1_BIT;
                break;
            case 1:
                tipo_predictor = BTB_2_BIT_H;
                break;
            case 2:
                tipo_predictor = BTB_2_BIT_S;
                break;
            case 3:
                tipo_predictor = PERFECTO;
                break;
            case 4:
                tipo_predictor = CLARIVIDENTE;
                break;
            case 5:
                tipo_predictor = PREDICT_NOT_TAKEN;
                break;
        }
        TAM_BUFFER_PREDIC = TAM_BUFFER_PREDIC_p;

        TAM_REORDER = TAM_REORDER_p;

        dep_mem = load_forwarding_p ? l_forwarding : l_bypassing;
    }

    char** argv;
    int argc = 0;

    /*** Si la configuracion esta en ensamblador_s construye argc y argv a partir de la primera linea de ensamblador_s */

    if (config_en_s) {

        char* i;

        for (i = ensamblador_s; *i != ';' && *i != '\n' && *i != 0; i++);

        if (*i == ';') { /* La primera linea debe ser un comentario */

            char* f;

            i++;
            for (f = i; *f != '\n' && *f != 0; f++);

            if (*f == '\n') { /* No continua si ensamblador_s no tiene al menos una linea */

                /* Aisla la primera linea */
                *f = 0;
                ensamblador_s = f + 1;

                /* main espera que argv tenga como primer componente el nombre del ejecutable */
                argv = malloc(sizeof (char*));
                argv[0] = "mips-ooo";
                argc++;

                /* Separa y anade a argv los argumentos encontrados en la primera linea */
                char* p;

                for (p = strtok(i, " \t"); p != 0; p = strtok(0, " \t")) {
                    argv = realloc(argv, sizeof (char*) * (argc + 1));
                    argv[argc] = p;
                    argc++;
                }
                argv = realloc(argv, sizeof (char*) * (argc + 1));
                argv[argc] = 0;
            }
        }
    } /* endif (config_en_s) */

    /*** Crea el fichero de entrada para el simulador a partir de ensamblador_s */

    {
        FILE* f_entrada;

        f_entrada = fopen("fichero.s", "w");
        fputs(ensamblador_s, f_entrada);
        fclose(f_entrada);
    }

#endif /* #ifndef LIBMIPSOOO */

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int RC = 0; /* Valor de retorno */
    int np;
    boolean parametroM = NO;

    int argn;
    char *uso =
            "\n%s [OPCIONES] {-f <fichero.s>}+\n"
            "\n\t----- Ejecución -----\n\n"
            "\t-t <fichero.sign>\n"
            "\t          Comprueba que la ejecución corresponde con la que hay en el fichero de firmas.\n"
            "\t-g <fichero.sign>\n"
            "\t          Genera un fichero de firmas correspondiente a la ejecución actual.\n"
            "\t-j\n"
            "\t          Genera un sólo archivo .htm con todos los ficheros de estado y cronogramas.\n"
            "\t-J\n"
            "\t          Genera un sólo archivo .js con todos los datos.\n"
            "\t-w\n"
            "\t          La salida utiliza direcciones simbólicas\n"
            "\t-D\n"
            "\t          Se muestran las macros para el cálculo del valor inmediato a partir de las etiquetas.\n"
            "\t          Afecta solamente a las macros predeterminadas.\n"
            "\t-s\n"
            "\t          Ejecuta sin imprimir los ficheros de estado y los cronogramas.\n"
            "\t          Se deshabilita la comprobación de firmas.\n"
            "\t-n\n"
            "\t          No borra archivos html antes de la ejecución.\n"
            "\n\t----- Configuración -----\n\n"
            "\t NOTA: Un valor 0 en cualquier parámetro indica que se mantiene el valor por omisión.\n\n"
            "\t-e <num>:<lat>:<segm>:<er>\n"
            "\t          Número y latencia de los operadores de enteros/saltos, tipo ([s]egmentado/[c]onvencional), y estaciones de reserva\n"
            "\t-l <num>:<lat>:<segm>:<tl>:<te>\n"
            "\t          Número y latencia de los operadores de memoria, tipo ([s]egmentado/[c]onvencional), y tampones de lectura y escritura\n"
            "\t-a <num>:<lat>:<segm>:<er>\n"
            "\t          Número y latencia de los operadores de suma/resta/comparación, tipo ([s]egmentado/[c]onvencional), y estaciones de reserva\n"
            "\t-m <num>:<lat>:<segm>:<er>\n"
            "\t          Número y latencia de los operadores de multiplicación/división, tipo ([s]egmentado/[c]onvencional), y estaciones de reserva\n"
            "\t-r <num>\n"
            "\t          Número de entradas del Reorder Buffer\n"
            "\t-v <issue>:<buses>:<commits>\n"
            "\t          Número de vías del procesador superescalar en ISSUE, BUSES y COMMIT\n"
            "\t-M\n"
            "\t          Número de operadores y estaciones de reserva en función del número de vías\n"
            "\t-p {1|2h|2s|p|c}\n"
            "\t          Tipo de predictor.\n"
            "\t            [1] : 1 bit (opción por omisión)\n"
            "\t            [2h]: 2 bits con histéresis\n"
            "\t            [2s]: 2 bits con saturación\n"
            "\t            [p] : Perfecto\n"
            "\t            [c] : BTB con predictor perfecto\n"
            "\t            [pnt] : Predict-not-taken\n"
            "\t-b <num>\n"
            "\t          Número de entradas del BTB\n"
            "\t-L <bits>:<tipo>:<entradas>:<bits-global>\n"
            "\t          Predictor BTB: n. bits, tipo ([h]isteresis/[s]aturación), n. entradas y n. bits historia global\n"
            "\t-G <bits>:<tipo>:<bits-global>\n"
            "\t          Predictor Global: n. bits, tipo ([h]isteresis/[s]aturación), y n. bits historia global\n"
            "\t-H <bits>\n"
            "\t          Predictor Híbrido: n. bits selector\n"
            "\t-c\n"
            "\t          Muestra la configuración y termina.\n"
            "\t-d {b|f}\n"
            "\t          Determina el mecanismo de resolución de dependencias en memoria.\n"
            "\t            [b]: Load Bypassing (opción por omisión)\n"
            "\t            [f]: Load Forwarding\n"
            "\n\t----- Entrada -----\n\n"
            "\t-f <fichero.s> (permite múltiples valores)\n"
            "\t          Nombre del fichero en ensamblador\n"
            "";

    boolean f_firmas = NO;
    boolean borrar_html = SI;

    char tipo_oper;

    char * ficheros_entrada[MAX_FICHEROS];
    int num_ficheros = 0;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    presentacion = presentacion_html;
    nombre_fich[0] = '\0';

    /*** Lectura de parametros */

    if (!LIBMIPSOOO || (LIBMIPSOOO && argc > 0)) {

        f_depuracion = SI;

        argn = 1;
        if (argc < 2) {
            fprintf(stderr, uso, argv[0]);
            exit(1);
        } /* endif */

        while (argn < argc) {
            if (!strncmp(argv[argn], "-f", 2)) {
                char str[64 * MAX_FICHEROS];

                argn++;
                ficheros_entrada[num_ficheros] = argv[argn];
                if (!file_exists(ficheros_entrada[num_ficheros])) {
                    fprintf(stderr, "%s: Fichero de entrada '%s' no encontrado.\n", argv[0], ficheros_entrada[num_ficheros]);
                    error_fatal(uso, argv[0]);
                } /* endif */

                sprintf(str, "%s %s", nombre_fich, basename(argv[argn]));
                strcpy(nombre_fich, str);
                num_ficheros++;
            } else if (!strncmp(argv[argn], "-t", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);
                abre_firma(argv[argn]);
                f_firmas = SI;
            } else if (!strncmp(argv[argn], "-g", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);
                crea_firma(argv[argn]);
                f_firmas = SI;
            } else if (!strncmp(argv[argn], "-w", 2)) {
                f_etiquetas = SI;
            } else if (!strncmp(argv[argn], "-D", 2)) {
                f_macros = SI;
            } else if (!strncmp(argv[argn], "-s", 2)) {
                f_depuracion = NO;
            } else if (!strncmp(argv[argn], "-e", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);

                np = sscanf(argv[argn], "%d:%d:%c:%d", &NUM_INICIO_ENTEROS, &TEVAL_ENTEROS, &tipo_oper,
                        &TAM_RS_ENTEROS);
                if (np != 4) {
                    fprintf(stderr, "%s: Formato erroneo en el parámetro 'e'\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }

                if (tipo_oper != 's' && tipo_oper != 'c') {
                    fprintf(stderr, "%s: Formato erroneo en el tipo del operador del parámetro 'e'\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }

                CHECK_DEFAULT_VALUE(NUM_INICIO_ENTEROS);
                CHECK_DEFAULT_VALUE(TEVAL_ENTEROS);
                CHECK_DEFAULT_VALUE(TAM_RS_ENTEROS);

                NUM_OPER_ENTEROS = (tipo_oper == 'c') ? NUM_INICIO_ENTEROS : NUM_INICIO_ENTEROS * TEVAL_ENTEROS;

            } else if (!strncmp(argv[argn], "-l", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);

                np = sscanf(argv[argn], "%d:%d:%c:%d:%d", &NUM_INICIO_MEMDATOS, &TEVAL_MEMDATOS, &tipo_oper,
                        &TAM_BUFFER_CARGA, &TAM_BUFFER_ALMACEN);
                if (np != 5) {
                    fprintf(stderr, "%s: Formato erroneo en el parámetro 'l'\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }

                if (tipo_oper != 's' && tipo_oper != 'c') {
                    fprintf(stderr, "%s: Formato erroneo en el tipo del operador del parámetro 'l'\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }

                NUM_OPER_DIRECCIONES = NUM_INICIO_MEMDATOS;
                NUM_INICIO_DIRECCIONES = NUM_INICIO_MEMDATOS;

                CHECK_DEFAULT_VALUE(NUM_INICIO_MEMDATOS);
                CHECK_DEFAULT_VALUE(TEVAL_MEMDATOS);
                CHECK_DEFAULT_VALUE(TAM_BUFFER_CARGA);
                CHECK_DEFAULT_VALUE(TAM_BUFFER_ALMACEN);

                NUM_OPER_MEMDATOS = (tipo_oper == 'c') ? NUM_INICIO_MEMDATOS : NUM_INICIO_MEMDATOS * TEVAL_MEMDATOS;

            } else if (!strncmp(argv[argn], "-a", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);

                np = sscanf(argv[argn], "%d:%d:%c:%d", &NUM_INICIO_SUMREST, &TEVAL_SUMREST, &tipo_oper,
                        &TAM_RS_SUMREST);
                if (np != 4) {
                    fprintf(stderr, "%s: Formato erroneo en el parámetro 'a'\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }

                if (tipo_oper != 's' && tipo_oper != 'c') {
                    fprintf(stderr, "%s: Formato erroneo en el tipo del operador del parámetro 'a'\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }

                CHECK_DEFAULT_VALUE(NUM_INICIO_SUMREST);
                CHECK_DEFAULT_VALUE(TEVAL_SUMREST);
                CHECK_DEFAULT_VALUE(TAM_RS_SUMREST);

                NUM_OPER_SUMREST = (tipo_oper == 'c') ? NUM_INICIO_SUMREST : NUM_INICIO_SUMREST * TEVAL_SUMREST;

            } else if (!strncmp(argv[argn], "-m", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);

                np = sscanf(argv[argn], "%d:%d:%c:%d", &NUM_INICIO_MULTDIV, &TEVAL_MULTDIV, &tipo_oper,
                        &TAM_RS_MULTDIV);
                if (np != 4) {
                    fprintf(stderr, "%s: Formato erroneo en el parámetro 'm'\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }

                if (tipo_oper != 's' && tipo_oper != 'c') {
                    fprintf(stderr, "%s: Formato erroneo en el tipo del operador del parámetro 'm'\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }

                CHECK_DEFAULT_VALUE(NUM_INICIO_MULTDIV);
                CHECK_DEFAULT_VALUE(TEVAL_MULTDIV);
                CHECK_DEFAULT_VALUE(TAM_RS_MULTDIV);

                NUM_OPER_MULTDIV = (tipo_oper == 'c') ? NUM_INICIO_MULTDIV : NUM_INICIO_MULTDIV * TEVAL_MULTDIV;

            } else if (!strncmp(argv[argn], "-r", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);

                TAM_REORDER = atoi(argv[argn]);
                if (TAM_REORDER <= 0 || TAM_REORDER > MAX_REORDER) {
                    fprintf(stderr, "%s: Tamaño del reorder buffer inválido\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
            } else if (!strncmp(argv[argn], "-v", 2)) {
                if (parametroM) {
                    fprintf(stderr, "%s: La opción -M debe ir tras la opción - v\n", argv[0]);
                    error_fatal("", argv[0]);
                }
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);

                np = sscanf(argv[argn], "%d:%d:%d", &NUM_VIAS_ISSUE, &NUM_VIAS_BUS, &NUM_VIAS_COMMIT);
                if (np != 3) {
                    fprintf(stderr, "%s: Formato erroneo en el parámetro 'v'\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }

                CHECK_DEFAULT_VALUE(NUM_VIAS_ISSUE);
                CHECK_DEFAULT_VALUE(NUM_VIAS_BUS);
                CHECK_DEFAULT_VALUE(NUM_VIAS_COMMIT);

                if (NUM_VIAS_ISSUE <= 0 || NUM_VIAS_ISSUE > MAX_VIAS_SUPERESCALAR) {
                    fprintf(stderr, "%s: Número de vías en ISSUE inválido\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
                if (NUM_VIAS_BUS <= 0 || NUM_VIAS_BUS > MAX_VIAS_SUPERESCALAR) {
                    fprintf(stderr, "%s: Número de vías en el BUS inválido\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
                if (NUM_VIAS_COMMIT <= 0 || NUM_VIAS_COMMIT > MAX_VIAS_SUPERESCALAR) {
                    fprintf(stderr, "%s: Número de vías en COMMIT inválido\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
            } else if (!strncmp(argv[argn], "-b", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);

                TAM_BUFFER_PREDIC = atoi(argv[argn]);
                if (TAM_BUFFER_PREDIC < 0 || TAM_BUFFER_PREDIC > MAX_BUFFER_PREDIC) {
                    fprintf(stderr, "\n%s: Tamaño del BTB inválido\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
            } else if (!strncmp(argv[argn], "-d", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);
                if (!strncmp(argv[argn], "b", 1)) {
                    dep_mem = l_bypassing;
                } else if (!strncmp(argv[argn], "f", 1)) {
                    dep_mem = l_forwarding;
                } else {
                    error_fatal(uso, argv[0]);
                }
            } else if (!strncmp(argv[argn], "-p", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);
                if (!strncmp(argv[argn], "1", 3)) {
                    tipo_predictor = BTB_1_BIT;
                } else if (!strncmp(argv[argn], "2h", 3)) {
                    tipo_predictor = BTB_2_BIT_H;
                } else if (!strncmp(argv[argn], "2s", 3)) {
                    tipo_predictor = BTB_2_BIT_S;
                } else if (!strncmp(argv[argn], "p", 3)) {
                    tipo_predictor = PERFECTO;
                } else if (!strncmp(argv[argn], "c", 3)) {
                    tipo_predictor = CLARIVIDENTE;
                } else if (!strncmp(argv[argn], "pnt", 3)) {
                    tipo_predictor = PREDICT_NOT_TAKEN;
                } else {
                    error_fatal(uso, argv[0]);
                }
                /* Predictor local */
            } else if (!strncmp(argv[argn], "-L", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);


                np = sscanf(argv[argn], "%d:%c:%d:%d", &n_bits_pred_local, &subtipo_pred_local, &TAM_BUFFER_PREDIC,
                        &n_bits_branch_mask_local);
                if (np != 4) {
                    fprintf(stderr, "%s: Formato erroneo en el parámetro 'L'\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
                if (subtipo_pred_local == 's') {
                    tipo_predictor = BTB_n_BIT_S;
                } else if (subtipo_pred_local == 'h') {
                    tipo_predictor = BTB_n_BIT_H;
                } else {
                    fprintf(stderr, "%s: Formato erroneo en el subtipo de predictor (h ó s)\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
                if (TAM_BUFFER_PREDIC < 0 || TAM_BUFFER_PREDIC > MAX_BUFFER_PREDIC) {
                    fprintf(stderr, "\n%s: Tamaño del BTB inválido\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
                if (n_bits_pred_local < 1) {
                    fprintf(stderr, "\n%s: Num bits predictor local inválido\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
                if (n_bits_branch_mask_local < 1 || n_bits_branch_mask_local > MAX_BITS_BRANCH_MASK_LOCAL) {
                    fprintf(stderr, "\n%s: Num bits mascara global inválido\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
                /* Predictor global */
            } else if (!strncmp(argv[argn], "-G", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);

                np = sscanf(argv[argn], "%d:%c:%d", &n_bits_pred_global, &subtipo_pred_global,
                        &n_bits_branch_mask_global);
                if (np != 3) {
                    fprintf(stderr, "%s: Formato erroneo en el parámetro 'G'\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
                if (subtipo_pred_global == 's') {
                    tipo_predictor = GLOBAL_n_BIT_S;
                } else if (subtipo_pred_global == 'h') {
                    tipo_predictor = GLOBAL_n_BIT_H;
                } else {
                    fprintf(stderr, "%s: Formato erroneo en el subtipo de predictor (h ó s)\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
                if (n_bits_pred_global < 1) {
                    fprintf(stderr, "\n%s: Num bits predictor global inválido\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
                if (n_bits_branch_mask_global < 1 || n_bits_branch_mask_global > MAX_BITS_BRANCH_MASK_GLOBAL) {
                    fprintf(stderr, "\n%s: Num bits mascara global inválido\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
            } else if (!strncmp(argv[argn], "-H", 2)) {
                argn++;
                if (argn == argc)
                    error_fatal(uso, argv[0]);

                np = sscanf(argv[argn], "%d", &n_bits_pred_hybrid);
                if (np != 1) {
                    fprintf(stderr, "%s: Formato erroneo en el parámetro 'H'\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
                if (n_bits_pred_hybrid < 1) {
                    fprintf(stderr, "\n%s: Num bits predictor inválido\n", argv[0]);
                    error_fatal(uso, argv[0]);
                }
                tipo_predictor = HYBRID_n_BIT_S;
            } else if (!strncmp(argv[argn], "-c", 2)) {
                imprime_config();
                exit(0);
            } else if (!strncmp(argv[argn], "-n", 2)) {
                borrar_html = NO;
            } else if (!strncmp(argv[argn], "-j", 2)) {
                html_merge = SI;
            } else if (!strncmp(argv[argn], "-J", 2)) {
                presentacion = presentacion_json;
            } else if (!strncmp(argv[argn], "-M", 2)) {
                parametroM = SI;
                // Máximo de operadores y RS
                NUM_INICIO_ENTEROS = NUM_VIAS_ISSUE;
                NUM_OPER_ENTEROS = NUM_INICIO_ENTEROS;
                TAM_RS_ENTEROS = NUM_VIAS_ISSUE * 32;

                NUM_INICIO_SUMREST = NUM_VIAS_ISSUE;
                NUM_OPER_SUMREST = NUM_INICIO_SUMREST;
                TAM_RS_SUMREST = NUM_VIAS_ISSUE * 16;

                NUM_INICIO_MULTDIV = NUM_VIAS_ISSUE;
                NUM_OPER_MULTDIV = NUM_INICIO_MULTDIV;
                TAM_RS_MULTDIV = NUM_VIAS_ISSUE * 16;

                NUM_INICIO_MEMDATOS = NUM_VIAS_ISSUE;
                NUM_OPER_MEMDATOS = NUM_INICIO_MEMDATOS;
                NUM_OPER_DIRECCIONES = NUM_OPER_MEMDATOS;
                TAM_BUFFER_CARGA = NUM_VIAS_ISSUE * 16;
                TAM_BUFFER_ALMACEN = NUM_VIAS_ISSUE * 16;
            } else {
                error_fatal(uso, argv[0]);
            } /* endif */

            argn++;
        } /* endwhile */

    } /* endif (!LIBMIPSOOO || LIBMIPSOOO && argc > 0) */

#if LIBMIPSOOO

    /* Corrige configuracion si se usa libmipsooo */

    strcpy(ficheros_entrada[num_ficheros], "fichero.s");
    num_ficheros = 1;
    f_depuracion = SI;
    //f_etiquetas = SI;
    borrar_html = NO;
    html_merge = SI;
    if (devuelve_json) {
        presentacion = presentacion_json;
    }

#endif /* LIBMIPSOOO */

    /*** Borrar html */

    if (borrar_html) {
        system("rm *.html 2>/dev/null");
    }

    /* Si el predictor es perfecto, tenemos que obtener la traza */
    if ((tipo_predictor == PERFECTO) || (tipo_predictor == CLARIVIDENTE)) {
        fprintf(stdout, "*** Generando traza para predictor perfecto/clarividente ... ***\n");

        fase_ejecucion = TRAZA;
        f_trace = fopen(trace_filename, "w");

        /*** Inicializacion del programa */

		inicializa_programa(&programa, num_ficheros, ficheros_entrada);

        /*** Inicializacion de estructuras */

        inicializa_estructuras();

        /*** Carga el codigo en lenguaje ensamblador */

        construye_programa(&programa);

        /*** Llama al bucle principal */

        ejecutar_codigo();

        /* Esta línea la leerá */
        fprintf(f_trace, "%d %d %d %d\n", -1, -1, -1, -1);
        /* Esta ya no debe leerse */
        fprintf(f_trace, "%d %d %d %d\n", -2, -2, -2, -2);
        fclose(f_trace);
        fase_ejecucion = FINAL;

        fprintf(stdout, "*** Traza generada... ***\n");

        system("rm *.html 2>/dev/null");
    }

    if ((tipo_predictor == PERFECTO) || (tipo_predictor == CLARIVIDENTE)) {
        f_trace = fopen(trace_filename, "r");
    }

    /*** Inicializacion del programa */

	inicializa_programa(&programa, num_ficheros, ficheros_entrada);

    /*** Inicializacion de estructuras */

    inicializa_estructuras();

    /*** Imprime configuración */

    imprime_config();

    /*** Carga el codigo en lenguaje ensamblador */

    construye_programa(&programa);

    /*** Imprime mapa de memoria */

    imprime_mapa_memoria();

    /*** Llama al bucle principal */

    ejecutar_codigo();

    if (f_firmas)
        cierra_firma();

    /*** Destruye los objetos que conformaban el programa */

    destruye_objetos_programa(&programa);

    /*** Imprime estadisticas */

    imprime_estadisticas();

    if ((tipo_predictor == PERFECTO) || (tipo_predictor == CLARIVIDENTE)) {
        fclose(f_trace);
    }


#if !LIBMIPSOOO

    return RC;

#else

    /*** libmipsooo devuelve una cadena con el contenido del fichero .htm o .json de salida */

    FILE* f_salida = fopen(devuelve_json ? "data.js" : "index.htm", "r");
    long file_size;
    char* result;

    fseek(f_salida, 0, SEEK_END);
    file_size = ftell(f_salida);
    rewind(f_salida);

    result = malloc(file_size + 1);
    fread(result, file_size, 1, f_salida);
    result[file_size] = 0;

    fclose(f_salida);

    /*** Libera argv si se construyo al inicio */

    if (argc > 0) {
        free(argv);
    }

    return result;

#endif /* LIBMIPSOOO */

} /* end main */
