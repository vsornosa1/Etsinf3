#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <mpi.h>

#define W 1000
#define H 1000

typedef unsigned char Byte;

/* Function: F(z) = z^3 - 1 */
double complex F1(double complex z)
{
  return z*z*z-1.0;
}
/* Derivative: F'(z) = 3*z^2 */
double complex D1(double complex z)
{
  return 3.0*z*z;
}

/* Function: F(z) = z^4 - 1 */
double complex F2(double complex z)
{
  return z*z*z*z-1.0;
}
/* Derivative: F'(z) = 4*z^3 */
double complex D2(double complex z)
{
  return 4.0*z*z*z;
}

/* Function: F(z) = z^6 + z^3 - 1 */
double complex F3(double complex z)
{
  double complex z3, z6;

  z3 = z*z*z;
  z6 = z3*z3;
  return z6+z3-1.0;
}
/* Derivative: F'(z) = 6*z^5 + 3*z^2 */
double complex D3(double complex z)
{
  double complex z2, z5;

  z2 = z*z;
  z5 = z2*z2*z;
  return 6.0*z5+3.0*z2;
}

/* Function: F(z) = z^11 - 1 */
double complex F4(double complex z)
{
  double complex z2, z5;

  z2 = z*z;
  z5 = z2*z2*z;
  return z5*z5*z-1.0;
}
/* Derivative: F'(z) = 11*z^10 */
double complex D4(double complex z)
{
  double complex z2, z5;

  z2 = z*z;
  z5 = z2*z2*z;
  return 11.0*z5*z5;
}

double complex (*fs[])(double complex) = { F1, F2, F3, F4 };
double complex (*ds[])(double complex) = { D1, D2, D3, D4 };
#define CASOS (sizeof(fs)/sizeof(*fs))

void escribe_pgm(char nom[], int ancho, int alto, Byte *imagen, int max, int col)
{
  FILE *F;
  char nombre[100];
  Byte pal[256][3];
  int i, j, c;

  if (strlen(nom) > 80) nom[80] = '\0';
  sprintf(nombre, "%s.p%cm", nom, col?'p':'g');
  F = fopen(nombre, "w");
  if (F == NULL) {
    fprintf(stderr, "ERROR: No se ha podido crear el archivo \"%s\".\n", nombre);
  } else {
    if (max > 255 || col) max = 255;
    fprintf(F, "P%c %d %d %d\n", col?'6':'5', ancho, alto, max);
    if (!col) {
      fwrite(imagen, 1, ancho*alto, F);
    } else {
      if (col < 0) { /* Se utiliza col como semilla para una paleta aleatoria */
        srand(col);
        for (i = 0; i < 256; i++)
          for (j = 0; j < 3; j++)
            pal[i][j] = rand() & 255;
      } else {
        c = 0; /* Se utiliza col como incremento para crear una paleta */
        for (i = 0; i < 768; i++) {
          ((Byte *)pal)[i] = c;
          c = (c + col) & 255;
        }
      }
      for (i = ancho*alto; i > 0; i--)
        fwrite(pal[*imagen++], 1, 3, F);
    }
    fclose(F);
  }
}

#define ABS(a) ((a) >= 0 ? (a) : -(a))

int caso;
#define F (*fs[caso])
#define D (*ds[caso])

/* Newton: z(i+1) = z(i) - f(z(i))/f'(z(i))     */
/* Devuelve el número de iteraciones realizadas */
int newton(double complex z0, double tol, int maxiter)
{
  int i;
  double complex f, d;

  for (i = 0; (f = F(z0), cabs(f) > tol) && i < maxiter; i++) {
    d = D(z0);
    z0 -= f/d;
  }
  return i;
}

#define A(i, j) A[(i)*w + (j)]
int fractal_newton(double x1, double x2, double y1, double y2,
                   int w, int h, Byte *A, double tol, int maxiter,
                   int me, int np)
{
  int j, ni, max;
  double complex z0;
  double ix, iy;
  int next_row, num_row, rows_done, proc;
  Byte B[W]; /* buffer for one row of the image */
  MPI_Status status;

  ix = (x2-x1)/(w-1);
  iy = (y2-y1)/(h-1);
  max = 0;

  if (me == 0) {

    /* CODE FOR THE MASTER */

    /* Initial distribution of work */
    next_row = 0;
    for ( proc = 1 ; proc < np ; proc++ ) {
      MPI_Send(&next_row, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
      next_row++;
    }

    /* While there are rows to be received */
    for ( rows_done = 0 ; rows_done < h ; rows_done++ ) {
      /* Receive a computed row from any process */
      MPI_Recv(B, w, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
               &status);
      /* Get the process index and the row number */
      proc = status.MPI_SOURCE;
      /* The row number is in the message TAG */
      num_row = status.MPI_TAG;
      /* Ask that process to compute another row */
      MPI_Send(&next_row, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
      next_row++;
      /* Copy the row received into its place in the image */
      memcpy(&A(num_row, 0), B, w);
    }

  } else {

    /* CODE FOR WORKERS */

    /* Receive the number of the row to be computed, or an out-of-range number to end */
    MPI_Recv(&num_row, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    while ( num_row < h ) {
      /* Compute the row */
      for ( j = 0 ; j < w ; j++ ) {
        z0 = (x1 + ix*j) + (y1 + iy*num_row)*I;
        ni = newton(z0, tol, maxiter);
        if ( ni > max ) max = ni;
        B[j] = ni;
      }
      /* Send the computed row */
      MPI_Send(B, w, MPI_BYTE, 0, num_row, MPI_COMM_WORLD);
      /* Receive the number of the next row to be computed */
      MPI_Recv(&num_row, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

  }

  /* Compute the maximum of the local maximums */
  ni = max;
  MPI_Reduce(&ni, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  return max;
}

int main(int argc, char *argv[])
{
  int max, maxiter=50, debug=0, col=0;
  Byte *A;
  double x=0, y=0, rango=1, tol=0.1;
  char *salida = "newton";
  int me, np, *max_tag, flag;
  double t;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &me);
  MPI_Comm_size(MPI_COMM_WORLD, &np);

  /* Lo que sigue es para sacar el máximo valor permitido como TAG */
  MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_TAG_UB, &max_tag, &flag);

  if (np < 2 || (flag && H - 1 > *max_tag)) {
    if (np < 2) {
      fprintf(stderr,
        "Al menos tiene que haber un trabajador al que mandar. :P\n");
    } else {
      fprintf(stderr,
        "No se puede usar el TAG para indicar el número de fila (no cabe).\n");
    }
    MPI_Finalize();
    return 1;
  }

  while (argc > 1) {
    if (argv[1][0] == '-') {
      switch (argv[1][1]) {
      case 'c':
        caso = atoi(&argv[1][2]) - 1;
        if (caso < 0 || caso > CASOS) {
          caso = 0;
        } else if (caso == 3) {
          maxiter = 200;
        } else if (caso == CASOS) {
          caso = 3;  maxiter = 255;  y = 0.128;  rango = 0.1;
        }
        break;
      case 'i':
        maxiter = atoi(&argv[1][2]);
        /* ¡Ojo! maxiter no debe valer más de 255 */
        if (maxiter > 255) maxiter = 255;
        break;
      case 'r':
        rango = atof(&argv[1][2]);
        if (rango <= 0) rango = 1;
        break;
      case 't':
        tol = atof(&argv[1][2]);
        break;
      case 'd':
        debug = 1;
        break;
      case 'o':
        salida = &argv[1][2];
        break;
      case 'x':
        x = atof(&argv[1][2]);
        break;
      case 'y':
        y = atof(&argv[1][2]);
        break;
      case 'p':
        col = atoi(&argv[1][2]);
        break;
      case 'h':
      default:
        fprintf(stderr, "Uso: newton -cCASO -iMAXITER -rRANGO -tTOL"
                " -oSALIDA -xX -yY -pPALETA -d -h\n");
        return 1;
      }
    }
    argc--; argv++;
  }

  /*
   Pedimos memoria.
   El maestro (me=0) tiene que almacenar toda la imagen.
   (Asumimos que malloc no falla, al ser poca memoria).
  */
  if (me == 0) {
    A = malloc(W*H*sizeof(Byte));
  }

  t = MPI_Wtime();
  max = fractal_newton(x-rango, x+rango, y+rango, y-rango,
                       W, H, A, tol, maxiter, me, np);
  t = MPI_Wtime() - t;
  if (me == 0) {
    printf("Tiempo: %.2f (%d procesos)\n", t, np);

    if (debug) {
      fprintf(stderr, "(max: %d)\n",max);
    }

    escribe_pgm(salida, W, H, A, max, col);

    /* Liberamos memoria */
    free(A);
  }

  MPI_Finalize();
  return 0;
}
