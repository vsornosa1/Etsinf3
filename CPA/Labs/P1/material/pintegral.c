#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

/* Funcion f(x) de la cual se quiere calcular la integral */
double f(double x)
{
//   return pow(10*x-14.0/3.0,3)-150*x+120;
   return M_PI/2.0*sin(x*M_PI);
}

/* Calculo de la integral de la funcion f. Variante 1 */
double calcula_integral1(double a, double b, int n)
{
   double h, s=0, result;
   int i;

   h=(b-a)/n;

   for (i=0; i<n; i++) {
      s+=f(a+h*(i+0.5));
   }

   result = h*s;
   return result;
}

/* Calculo de la integral de la funcion f. Variante 2 */
double calcula_integral2(double a, double b, int n)
{
   double x, h, s=0, result;
   int i;

   h=(b-a)/n;

   for (i=0; i<n; i++) {
      x=a;
      x+=h*(i+0.5);

      s+=f(x);
   }

   result = h*s;
   return result;
}

int main(int argc, char *argv[])
{
   double a, b, result;
   int n, nt, variante;
   
   nt = atoi(argv[2]);
   omp_set_num_threads(nt);
   printf("Número de hilos fuera de la región paralela de OpenMP = %d\n", omp_get_num_threads());

   if (argc<2) {
      fprintf(stderr, "Numero de argumentos incorrecto\n");
   }
   if (argc>2) n=atoi(argv[2]);
   else n=100000;
   a=0;
   b=1;

   variante=atoi(argv[1]);
   #pragma omp parallel
   {
      printf("ID del hilo ejecutando región paralela = %d\n", omp_get_thread_num());
      switch (variante) {
         case 1:
	    result = calcula_integral1(a,b,n);
	    break;
         case 2:
	    result = calcula_integral2(a,b,n);
	    break;
         default:
	    fprintf(stderr, "Numero de variante incorrecto\n");
      }
   
   }
   
   printf("Número máximo de hilos en ejecución = %d\n", omp_get_max_threads());
   printf("Valor de la integral = %.12f\n", result);
   
   return 0;
}