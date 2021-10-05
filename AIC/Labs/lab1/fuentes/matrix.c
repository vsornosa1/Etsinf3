#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <xmmintrin.h>
#define MSIZE 2048 /* Must be multiple of 4 */

float matrix_in[MSIZE*MSIZE], matrix_in2[MSIZE*MSIZE], matrix_out[MSIZE*MSIZE];

//#define __SCALAR_PROD(v1, v2, s)  Scalar(v1, v2, s);
//#define __SCALAR_PROD(v1, v2, s)  ScalarSSE(v1, v2, s);
#define __SCALAR_PROD(v1, v2, s)  


//////////
// code //
//////////

void init_vector(float *t, int size) {
 
  int i;
  float num, dnum;
  
  num = rand()/(RAND_MAX+1.0);
  dnum = rand()/(RAND_MAX+1.0);
  for(i=0; i<size; i++) {
    t[i] = num;
    num += dnum;
  } // end for
 
} // end init_vector()

 
void init_matrix(float *t, int size) {
 
    int i;
    float num, dnum;

    num = rand()/(RAND_MAX+1.0);
    dnum = rand()/(RAND_MAX+1.0);
    for(i=0; i<size*size; i++) {
      t[i] = num;
      num += dnum;
    } // end for

} // end init_matrix()

 
void print_matrix(
		  FILE   *fd,
		  float  *m,
		  int     size
		  )
{
  int i, j;
  
  for (i=0; i<size; i++) {
    for (j=0; j<size; j++) {
      fprintf(fd, "%.1f ", m[i*size+j]);
      fflush(fd);
    } // end for
    fprintf(fd, "\n");
    fflush(fd);
  } // end for
} // end printMat()


float Scalar(float *s1, float *s2, int size) {
 
    int i;
    float prod = 0.0;
 
    for(i=0; i<size; i++) {
      prod += s1[i] * s2[i];
    }
 
    return prod;
 
} // end Scalar()


float ScalarSSE(float *m1, float *m2, int size) {
 
    float prod = 0.0;
    int i;
    __m128 X, Y, Z;
    
    Z = _mm_setzero_ps(); /* Z needs to be initialized to all 0.0 */
    for(i=0; i<size; i+=4) {
      X = _mm_load_ps(&m1[i]);
      Y = _mm_load_ps(&m2[i]);
      X = _mm_mul_ps(X, Y);
      Z = _mm_add_ps(X, Z);
    }
    
    for(i=0; i<4; i++) {
      prod += Z[i];
    }
 
    return prod;
 
} // end ScalarSSE()


void MatMult(
	     float *m1,
	     float *m2,
	     int    size,
	     float *res
	     )
{
  float tmp[size]; 
  int   i, j, n;
  
  for (j=0; j<size; j++) { // columnas
    // trasponer vector columna: m2[*][j]
    for (n=0; n<size; n++) {
      tmp[n] = m2[n*size+j];
    } // end for
    for (i=0; i<size; i++) {  // filas
      __SCALAR_PROD(&m1[i*size], tmp, size);
    } // end for
  } // end for
  
} // end MatMult()


void MatCompute(
		float *m,
		int    size
		)
{
  int  i, j, n;

  for (n=0; n<size/16; n++) {
    for (i=0; i<size; i++) {
      for (j=0; j<size; j++) {
	m[i*size+j] = cos(m[i*size+j]);
      } // end for
    } // end for
  } // end for

} // end MatCos()


int main(int argc, char * argv[]) {
 
    int     i;
    time_t  start, stop;
    double  avg_time;
    double  cur_time;
    int     rep=1;
    int     msize=MSIZE;
    float   fvalue;
    char   *cmd;

    if (argc == 2) { 
      rep = atoi(argv[1]);
    } else if (argc == 3) {
      rep = atoi(argv[1]);
      msize = atoi(argv[2]);
    } // end if/else
    fprintf(stderr, "Rep = %d / size = %d\n", rep, msize);
    
    for(i=0; i<rep; i++) {
      init_matrix(matrix_in, msize);
      init_matrix(matrix_in2, msize);
      MatMult(matrix_in, matrix_in2, msize, matrix_out);
      MatCompute(matrix_out, msize);
    } // end for

    exit(0);
    
} // end main()
