/**************************************************************************
 *
 * Departamento de Informática de Sistema y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * Autor: Pedro López (plopez@disca.upv.es)
 *           Sergio Sáez (ssaez@disca.upv.es)
 *
 * File: main.c
 *
 * Description:
 *    Módulo principal del simulador
 *
 *************************************************************************/

#define main_C

#ifndef VERSION
#define VERSION 4.0
#endif

/***** Includes ***********************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <libgen.h>
#include <sys/stat.h>



/*** Definiciones Externas *************************************************/

#include "main.h"
#include "tipos.h"
#include "instrucciones.h"
#include "presentacion-txt.h"
#include "presentacion-html.h"
#include "mips.h"
#include "mips_fp.h"
#include "memoria.h"

extern void ensamblador(fichero_objeto_t *obj);


extern int pos_datos;

/*** Funciones Locales *****************************************************/

/***************************************************************
 *
 * Func: inicializa_estructuras
 *
 * Desc: Inicializa las estructuras del mips
 *
 **************************************************************/

static void inicializa_estructuras(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int i;
    instruccion_t instr;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    /* Registros */

    PC = 0;

    for (i = 0; i < TAM_REGISTROS + 1; i++) // +1 por el FPSR
    {
        Rint[i].valor = 0;
    } /* endfor */

    for (i = 0; i < TAM_REGISTROS + 1; i++) // +1 por el FPSR
    {
        Rfp[i].valor.fp_d = 0;
        Rfp[i].busy = 0;
    } /* endfor */


    for (i = 0; i < TAM_REGISTROS + 1; i++) // +1 por el FPSR
    {
        Rfp_tipo[i] = t_ninguno;
    } /* endfor */

    instr.codop = OP_NOP;
    instr.tipo = FormatoR;
    instr.Rdestino = 0;
    instr.Rfuente1 = 0;
    instr.Rfuente2 = 0;
    instr.inmediato = 0;

    /* Registros intermedios */
    IF_ID.IR = instr;
    IF_ID.NPC = 0;
    IF_ID.iPC = -1;
    IF_ID.orden = 0;

    ID_EX.IR = instr;
    ID_EX.NPC = 0;
    ID_EX.Ra = 0;
    ID_EX.Rb = 0;
    ID_EX.Imm = 0;
    ID_EX.iPC = -1;
    ID_EX.orden = -1;

    EX_MEM.IR = instr;
    EX_MEM.ALUout = 0;
    EX_MEM.data = 0;
    EX_MEM.cond = NO;
    EX_MEM.iPC = -1;
    EX_MEM.orden = -1;

    MEM_WB.IR = instr;
    MEM_WB.ALUout = 0;
    MEM_WB.MEMout = 0;
    MEM_WB.iPC = -1;
    MEM_WB.orden = -1;

    IFstall = NO;
    IDstall = NO;

    ID_LS.IR = instr;
    ID_LS.Fa.fp_d = 0;
    ID_LS.Fb.fp_d = 0;
    ID_LS.Ra = 0;
    ID_LS.Rb = 0;
    ID_LS.Imm = 0;
    ID_LS.iPC = -1;
    ID_LS.orden = -1;

    ID_A.IR = instr;
    ID_A.Fa.fp_d = 0;
    ID_A.Fb.fp_d = 0;
    ID_A.Ra = 0;
    ID_A.Rb = 0;
    ID_A.Imm = 0;
    ID_A.iPC = -1;
    ID_A.orden = -1;

    ID_C.IR = instr;
    ID_C.Fa.fp_d = 0;
    ID_C.Fb.fp_d = 0;
    ID_C.Ra = 0;
    ID_C.Rb = 0;
    ID_C.Imm = 0;
    ID_C.iPC = -1;
    ID_C.orden = -1;

    ID_M.IR = instr;
    ID_M.Fa.fp_d = 0;
    ID_M.Fb.fp_d = 0;
    ID_M.Ra = 0;
    ID_M.Rb = 0;
    ID_M.Imm = 0;
    ID_M.iPC = -1;
    ID_M.orden = -1;

    for (i = 1; i <= FP_ADD_LAT; i++) {
        A[i].fpALUout.fp_d = 0;
        A[i].IR = instr;
        A[i].iPC = -1;
        A[i].orden = -1;
    }

    for (i = 1; i <= FP_CMP_LAT; i++) {
        C[i].intALUout = 0;
        C[i].IR = instr;
        C[i].iPC = -1;
        C[i].orden = -1;
    }


    for (i = 1; i <= FP_LS_LAT; i++) {
        LS[i].fpALUout.fp_d = 0;
        LS[i].IR = instr;
        LS[i].iPC = -1;
        LS[i].orden = -1;
    }

    for (i = 1; i <= FP_MUL_LAT; i++) {
        M[i].fpALUout.fp_d = 0;
        M[i].IR = instr;
        M[i].iPC = -1;
        M[i].orden = -1;
    }

    LS_WB.IR = instr;
    LS_WB.fpALUout.fp_d = 0;
    LS_WB.iPC = -1;
    LS_WB.orden = -1;

    ADD_WB.IR = instr;
    ADD_WB.fpALUout.fp_d = 0;
    ADD_WB.iPC = -1;
    ADD_WB.orden = -1;

    CMP_WB.IR = instr;
    CMP_WB.ALUout = 0;
    CMP_WB.iPC = -1;
    CMP_WB.orden = -1;

    MUL_WB.IR = instr;
    MUL_WB.fpALUout.fp_d = 0;
    MUL_WB.iPC = -1;
    MUL_WB.orden = -1;

    ID_WB.IR = instr;
    ID_WB.fpALUout.fp_d = 0;
    ID_WB.iPC = -1;
    ID_WB.orden = -1;

    FP_WB.IR = instr;
    FP_WB.fpALUout.fp_d = 0;
    FP_WB.iPC = -1;
    FP_WB.orden = -1;


} /* end inicializa_estructuras */

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

static int ejecutar_codigo(
        ) {
    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int RC; /* Valor de retorno */
    int i;


    boolean OVF_HTML = NO;
    //    boolean OVF_TXT = NO;

    /*  boolean	html = NO; */

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    RC = 0;

    /*** Inicializacion del computador */

    Ciclo = 0;
    //Instrucciones = 0;
    estat.instrucciones = 0;
    estat.ciclos = 0;
    estat.enteras = 0;
    estat.multiciclo = 0;
    estat.flops = 0;


    orden = 1;

    if ((!hay_float) && (!hay_double))
        small_screen_flag = 1;

    init_presentacion();

    if (salida == html) {
        imprime_inicio_css();

    }

    if (salida == html_final) {
        imprime_inicio_css();
    }

    /* Directiva de ensamblador .pc */
    PC = PC_inicial;


    /*** Bucle principal */


    while (!final) {

        if ((salida == res_final) || (salida == res_tiempo) || (salida == html_final)) {
            if ((Ciclo > 0)&&(Ciclo % 2000) == 0) {
                printf(".");
                fflush(stdout);
            };
            if ((Ciclo > 0)&&(Ciclo % 10000) == 0) {
                printf("%5uK", Ciclo / 1000);
                fflush(stdout);
            };
            if ((Ciclo > 0)&&(Ciclo % 50000) == 0) {
                printf("\n");
            };
        }

        /*** Enteros ******************************/

        /*** Fase: WB **************/

        final = fase_escritura();

        /*** Fase: MEM **************/

        fase_memoria();

        /*** Fase: EX *************/

        fase_ejecucion();

        /*** Fase: WB **************/

        fase_escritura_FP();

        /*** Fase: WB **************/

        fase_escritura_FPSR();

        /*** Fase: Ejecución multiciclo **************/

        i = FP_LS_LAT;
        while (i > 0) {
            fase_LS(i);
            i = i - 1;
        }

        i = FP_ADD_LAT;
        while (i > 0) {
            fase_A(i);
            i = i - 1;
        }

        i = FP_CMP_LAT;
        while (i > 0) {
            fase_C(i);
            i = i - 1;
        }


        i = FP_MUL_LAT;
        while (i > 0) {
            fase_M(i);
            i = i - 1;
        }


        /*** Fase: ID **************/

        fase_decodificacion();

        /*** Fase: IF ***********/

        fase_busqueda();




        /* Incrementa el número de ciclos ejecutados */

        Ciclo++;
        estat.ciclos++;

        if (Ciclo >= MAX_CICLOS_TOTAL) {
            fprintf(stderr, "\nATENCION: Numero de ciclos (%d) excesivo\n Ejecución detenida\n\n", MAX_CICLOS_TOTAL);
            final = SI;
        }

        if ((salida == html) && (Ciclo >= MAX_CICLOS_HTML) && !OVF_HTML) {
            fprintf(stderr, "\nATENCION: Numero de ciclos (%d) excesivo\n No se generan mas archivos html\n\n", MAX_CICLOS_HTML);
            OVF_HTML = SI;
        };

        if (OVF_HTML) {
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

        /*
        if ((salida != html) && (Ciclo >= MAX_CICLOS_TXT) && !OVF_TXT)
        {
            fprintf(stderr, "\nATENCION: Numero de ciclos excesivo\n No se muestran mas ciclos\n\n");
                        OVF_TXT = SI;
        } ;
         */


        /*** Imprime el estado -- interfaz de texto  */
        /*
        if ((salida == res_ciclo)  && (!OVF_TXT)) {
            imprime_etapas_txt ();
            imprime_reg_txt ();
            imprime_memdatos_txt ();
        }
         */
        /*** Imprime el estado del los operadores, etc.  */

        if ((salida == html) && (!OVF_HTML)) {
            actualiza_crono();
            imprime_css();
        }

        impulso_reloj();


    } /* endwhile */


    if (salida == res_tiempo) {
        //imprime_inicio_txt();
        imprime_final_txt();
        //        imprime_reg_txt ();
        //        imprime_memdatos_txt ();
    }

    if (salida == res_final) {
        printf("Archivo: %s\n\n", nombre_fich);
        imprime_inicio_txt();
        imprime_final_txt();
        imprime_reg_txt();
        imprime_memdatos_txt();
    }

    if (salida == html) {
        imprime_final_txt();
        imprime_final_css();

    }

    if (salida == html_final) {
        imprime_final_txt();
        imprime_final_css();
    }


    return (RC);

} /* end ejecutar_codigo */

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

#ifndef LIBMIPS
#define LIBMIPS 0

/***************************************************************
 *
 * Func: main
 *
 * Parametros:
 *	argc  (int) ; N˙mero de parametros en linea
 *	argv[]  (char*) ; Texto de los parametros en linea
 *
 * Retorna:  RC  (int)
 *
 * Descripción:
 *   Función principal
 *
 **************************************************************/

int main(
        int argc,
        char* argv[]
        ) {

#else
#define LIBMIPS 1

/***************************************************************
 *
 * Func: libmips_main
 *
 * Parametros:
 *      ensamblador_s (char*): Cadena con programa ensamblador a simular
 *      config_en_s (int): Leer la configuracion de la primera linea de ensamblador_s
 *      solucion_riesgos_control_p (int): 0 (stalls) / 1 (predict-not-taken) / 2 (delay slot)
 *      latencia_salto_p (int): Latencia de actualización del PC en caso de salto efectivo
 *      solucion_riesgos_datos_p (int): 0 (ninguno) / 1 (stalls) / 2 (cortocircuitos)
 *      FP_ADD_LAT_p, FP_CMP_LAT_p, FP_LS_LAT_p, FP_MUL_LAT_p (int): Latencias de operadores multiciclo
 *
 * Retorna: resultado_htm (char*): Resultado de la simulacion (malloc). Es responsabilidad del que llama liberar con free.
 *
 * Descripcion:
 *   Funcion principal de la libreria (reemplaza a main)
 *
 **************************************************************/

char* libmips_main
(
        char* ensamblador_s,
        int config_en_s,
        int solucion_riesgos_control_p, int latencia_salto_p,
        int solucion_riesgos_datos_p,
        int FP_ADD_LAT_p, int FP_CMP_LAT_p, int FP_LS_LAT_p, int FP_MUL_LAT_p
        ) {

    /*** Utiliza parametros _p si la configuracion no esta en ensamblador_s */

    if (!config_en_s) {

        switch (solucion_riesgos_control_p) {
            case 0:
                switch (latencia_salto_p) { /* stalls */
                    case 1: solucion_riesgos_control = stall1;
                        break;
                    case 2: solucion_riesgos_control = stall2;
                        break;
                    case 3: solucion_riesgos_control = stall3;
                        break;
                }
                break;
            case 1:
                switch (latencia_salto_p) { /* predict-not-taken */
                    case 1: solucion_riesgos_control = pnt1;
                        break;
                    case 2: solucion_riesgos_control = pnt2;
                        break;
                    case 3: solucion_riesgos_control = pnt3;
                        break;
                }
                break;
            case 2:
                switch (latencia_salto_p) { /* delay slot */
                    case 1: solucion_riesgos_control = ds1;
                        break;
                    case 2: solucion_riesgos_control = ds2;
                        break;
                    case 3: solucion_riesgos_control = ds3;
                        break;
                }
                break;
        }

        switch (solucion_riesgos_datos_p) {
            case 0: solucion_riesgos_datos = ninguno;
                break;
            case 1: solucion_riesgos_datos = parada;
                break;
            case 2: solucion_riesgos_datos = cortocircuito;
                break;
        }

        FP_ADD_LAT = FP_ADD_LAT_p >= MAX_FP_LAT ? MAX_FP_LAT : FP_ADD_LAT_p;
        FP_CMP_LAT = FP_CMP_LAT_p >= MAX_FP_LAT ? MAX_FP_LAT : FP_CMP_LAT_p;
        FP_LS_LAT = FP_LS_LAT_p >= MAX_FP_LAT ? MAX_FP_LAT : FP_LS_LAT_p;
        FP_MUL_LAT = FP_MUL_LAT_p >= MAX_FP_LAT ? MAX_FP_LAT : FP_MUL_LAT_p;

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
                argv[0] = "mips";
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

#endif /* #ifndef LIBMIPS */

    /*************************************/
    /*  Variables locales                */
    /*************************************/

    int RC; /* Valor de retorno */

    char * uso =
            "\n%s [-s resultado] [-d r.datos] [-c r.control] [-l x] [-a x] [-k x] [-m x] [-h] [-j] [-n] -f fichero.s\n"
            //        "\tsalida    ciclo final html(*)\n"
            "\t-s resultado  tiempo final html(*) html-final\n"
            "\t-d r.datos    n:ninguno, p:parada, c:cortocircuito(*)\n"
            "\t-c r. control s3:stalls(3)(*), s2:stalls(2), s1:stalls(1)\n"
            "\t              pnt3:p-n-taken(3), pnt2:p-n-taken(3), pnt1:p-n-taken(1)\n"
            "\t              ds3:s.retardado(ds=3), ds2:s.retardado(ds=2), ds1:s.retardado(ds=1)\n"
            "\t-l,-a,-k,-m x latencia operadores l:LS(2),a:ADD(4),k:CMP(4),m:MUL(7)\n"
            "\t--opt-int     elimina fase WB en store int\n"
            "\t-j            generar un sólo archivo htm con javascript\n"
            "\t--no-show-IT  no mostrar diagrama IT\n"
            "\t--no-show-DP  no mostrar ruta de datos\n"
            "\t--no-show-R   no mostrar registros\n"
            "\t--no-show-M   no mostrar memoria\n"
            "\t--no-show-LOG no mostrar LOG\n"
            "\t--no-show-C   no mostrar CONSOLA\n"
            "\t-n            no borrar archivos html\n"
            "\t-f fichero    nombre del fichero en ensamblador\n";


    boolean borrar_html = 1;



    FILE* f_aux;

    //char aux[128];
    char str[64 * MAX_FICHEROS];
    char fichero_entrada[MAX_FICHEROS][PATH_MAX];
    int num_ficheros = 0;

    /*************************************/
    /*  Cuerpo función                   */
    /*************************************/

    nombre_fich[0] = '\0';

    /*** Lectura de parametros */

    if (!LIBMIPS || (LIBMIPS && argc > 0)) {

        solucion_riesgos_datos = cortocircuito;
        solucion_riesgos_control = pnt3;
        salida = html;

        /* Flag set by ‘--verbose’. */
        // static int verbose_flag;

        int c;

        fprintf(stdout, "Simulador del procesador MIPS multiciclo (v%3.1f)\n\n", VERSION);

        while (1) {
            static struct option long_options[] = {
                /* These options set a flag. */
                //{"no-w3css", no_argument, &w3css_flag, 0},
                {"help", no_argument, 0, '?'},
                {"no-borrar-html", no_argument, 0, 'n'},
                {"opt-int", no_argument, &AJUSTAR_INT, 0},
                {"javascript", no_argument, 0, 'j'},
                {"small-screen", no_argument, &small_screen_flag, 0},
                {"no-show-IT", no_argument, &SHOW_DIT, 0},
                {"no-show-DP", no_argument, &SHOW_DATAPATH, 0},
                {"no-show-R", no_argument, &SHOW_REG, 0},
                {"no-show-M", no_argument, &SHOW_MEM, 0},
                {"no-show-LOG", no_argument, &SHOW_LOG, 0},
                {"no-show-C", no_argument, &SHOW_CONSOLE, 0},
                /* These options don't set a flag.
                 We distinguish them by their indices. */
                {"salida", required_argument, 0, 's'},
                {"rdatos", required_argument, 0, 'd'},
                {"rcontrol", required_argument, 0, 'c'},
                {"FPadd", required_argument, 0, 'a'},
                {"FPmul", required_argument, 0, 'm'},
                {"FPls", required_argument, 0, 'l'},
                {"FPcmp", required_argument, 0, 'k'},
                {"programa", required_argument, 0, 'f'},
                {0, 0, 0, 0}
            };
            /* getopt_long stores the option index here. */
            int option_index = 0;

            c = getopt_long(argc, argv, "?a:c:d:f:k:l:m:s:nj",
                    long_options, &option_index);

            /* Detect the end of the options. */
            if (c == -1)
                break;

            switch (c) {
                case '?':
                    fprintf(stderr, uso, argv[0]);
                    exit(1);
                    break;

                case 0:
                    /* If this option set a flag, do nothing else now. */
                    if (long_options[option_index].flag != 0)
                        break;
                    printf("option %s", long_options[option_index].name);
                    if (optarg)
                        printf(" with arg %s", optarg);
                    printf("\n");
                    break;

                case 'c':
                    if (strncmp(optarg, "p", 4) == 0) { // Por compatibilidad previa
                        solucion_riesgos_control = stall3;
                    } else if (strncmp(optarg, "s3", 4) == 0) {
                        solucion_riesgos_control = stall3;
                    } else if (strncmp(optarg, "s2", 4) == 0) {
                        solucion_riesgos_control = stall2;
                    } else if (strncmp(optarg, "s1", 4) == 0) {
                        solucion_riesgos_control = stall1;
                    } else if (strncmp(optarg, "t", 4) == 0) { // Por compatibilidad previa
                        solucion_riesgos_control = pnt3;
                    } else if (strncmp(optarg, "pnt3", 4) == 0) {
                        solucion_riesgos_control = pnt3;
                    } else if (strncmp(optarg, "pnt2", 4) == 0) {
                        solucion_riesgos_control = pnt2;
                    } else if (strncmp(optarg, "pnt1", 4) == 0) {
                        solucion_riesgos_control = pnt1;
                    } else if (strncmp(optarg, "ds3", 4) == 0) {
                        solucion_riesgos_control = ds3;
                    } else if (strncmp(optarg, "ds2", 4) == 0) {
                        solucion_riesgos_control = ds2;
                    } else if (strncmp(optarg, "ds1", 4) == 0) {
                        solucion_riesgos_control = ds1;
                    } else {
                        fprintf(stderr, "Error en argumento %s %s\n", "-c", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;

                case 'd':
                    if (strncmp(optarg, "n", 1) == 0) {
                        solucion_riesgos_datos = ninguno;
                    } else if (strncmp(optarg, "p", 1) == 0) {
                        solucion_riesgos_datos = parada;
                    } else if (strncmp(optarg, "c", 1) == 0) {
                        solucion_riesgos_datos = cortocircuito;
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-d", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;

                case 'f':
                    strcpy(fichero_entrada[num_ficheros], optarg);
                    if (!file_exists(fichero_entrada[num_ficheros])) {
                        fprintf(stderr, "%s: Fichero de entrada '%s' no encontrado.\n", argv[0], fichero_entrada[num_ficheros]);
                        exit(1);
                    } /* endif */

                    sprintf(str, "%s %s", nombre_fich, basename(optarg));
                    strcpy(nombre_fich, str);
                    num_ficheros++;

                    break;

                case 'a':
                    if (isdigit(*optarg) && (atoi(optarg) > 1)) {
                        if (atoi(optarg) >= MAX_FP_LAT) {
                            fprintf(stderr, "\nError. La latencia máxima del operador es %d\n", MAX_FP_LAT - 1);
                            exit(1);
                        }
                        FP_ADD_LAT = atoi(optarg);
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-a", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;

                case 'k':
                    if (isdigit(*optarg) && (atoi(optarg) > 1)) {
                        if (atoi(optarg) >= MAX_FP_LAT) {
                            fprintf(stderr, "\nError. La latencia máxima del operador es %d\n", MAX_FP_LAT - 1);
                            exit(1);
                        }
                        FP_CMP_LAT = atoi(optarg);
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-k", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;

                case 'l':
                    // Primer caracter del string
                    if (isdigit(*optarg) && (atoi(optarg) > 1)) {
                        if (atoi(optarg) >= MAX_FP_LAT) {
                            fprintf(stderr, "\nError. La latencia máxima del operador es %d\n", MAX_FP_LAT - 1);
                            exit(1);
                        }
                        FP_LS_LAT = atoi(optarg);
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-l", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;


                case 'm':
                    if (isdigit(*optarg) && (atoi(optarg) > 1)) {
                        if (atoi(optarg) >= MAX_FP_LAT) {
                            fprintf(stderr, "\nError. La latencia máxima del operador es %d\n", MAX_FP_LAT - 1);
                            exit(1);
                        }
                        FP_MUL_LAT = atoi(optarg);
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-m", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;


                case 's':
                    if (strncmp(optarg, "tiempo", 6) == 0) {
                        salida = res_tiempo;
                    } else if (strncmp(optarg, "final", 5) == 0) {
                        salida = res_final;
                    } else if (strncmp(optarg, "html", 10) == 0) {
                        salida = html;
                    } else if (strncmp(optarg, "html-final", 10) == 0) {
                        salida = html_final;
                    } else {
                        fprintf(stderr, "\nError en argumento %s %s\n", "-s", optarg);
                        fprintf(stderr, uso, argv[0]);
                        exit(1);
                    }
                    break;

                case 'n':
                    borrar_html = 0;
                    break;

                case 'j':
                    html_merge = SI;
                    break;

                default:
                    abort();
            }
        }

        /* Instead of reporting ‘--verbose’
         and ‘--brief’ as they are encountered,
         we report the final status resulting from them. */
        //        if (verbose_flag)
        //            puts ("w3css flag is set");


        /* Print any remaining command line arguments (not options). */
        if (optind < argc) {
            while (optind < argc)
                fprintf(stderr, "\nError en argumento %s\n", argv[optind++]);
            fprintf(stderr, uso, argv[0]);
            exit(1);
        }

    } /* endif (!LIBMIPS || LIBMIPS && argc > 0) */

    /* Corrige configuracion si se usa libmips */

    if (LIBMIPS) {
        /* Corrige configuracion si se usa libmipsooo */

        strcpy(fichero_entrada[num_ficheros], "fichero.s");
        num_ficheros = 1;
        //f_etiquetas = SI;
        borrar_html = NO;
        html_merge = SI;

        salida = html;
    }

    if (num_ficheros==0) {
        fprintf(stderr, "\nError en argumento -f: faltan nombres de archivos .s\n");
        fprintf(stderr, uso, argv[0]);
        exit(1);
    }

    fprintf(stdout, "Archivos: %s\n\n", nombre_fich);

    if (borrar_html) {
        system("rm *.html 2>/dev/null");
    }

    inicializa_programa(&programa, num_ficheros);

    for (int i = 0; i < num_ficheros; ++i) {
        inicializa_objeto(&programa.l_objs[i], fichero_entrada[i]);
    }


    /* Si existe el archivo en el /tmp, se utilizará su contenido para poner
     un enlace HOME en la salida html */
    if (file_exists("/tmp/_url.txt")) {
        f_aux = fopen("/tmp/_url.txt", "r");
        fgets(HOME_LINK, 128, f_aux);
        home_link_flag = 1;
        fclose(f_aux);
    }



    /*** Inicializacion de estructuras */

    inicializa_estructuras();

    /*** Carga el codigo en lenguaje ensamblador */

    construye_programa(&programa);

    /*** Imprime mapa de memoria */

    imprime_mapa_memoria();


    /*** Llama al bucle principal */

    RC = ejecutar_codigo();

#if !LIBMIPS

    return (RC);

#else

    /*** libmips devuelve una cadena con el contenido del fichero .htm de salida */

    FILE* f_salida = fopen("index.htm", "r");
    long htm_size;
    char* result;

    fseek(f_salida, 0, SEEK_END);
    htm_size = ftell(f_salida);
    rewind(f_salida);

    result = malloc(htm_size + 1);
    fread(result, htm_size, 1, f_salida);
    result[htm_size] = 0;

    fclose(f_salida);

    /*** Libera argv si se construyo al inicio */

    if (argc > 0) {
        free(argv);
    }

    return result;

#endif /* LIBMIPS */

} /* end main */

