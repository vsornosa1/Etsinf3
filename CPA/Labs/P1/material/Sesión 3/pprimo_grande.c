#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <omp.h>

typedef unsigned long long Entero_grande;
#define ENTERO_MAS_GRANDE  ULLONG_MAX

int primo(Entero_grande n)
{
  volatile int p;
  Entero_grande i, s;

  p = (n % 2 != 0 || n == 2);

  if (p) {
    s = sqrt(n);
  }
  
  #pragma omp parallel private(id_hilo)
  {  
  int id_hilo = omp_get_thread_num();
  int n_hilos = omp_get_num_threads();
  
  for (i = 2*id_hilo + 3; p && i <= s; i += 2*n_hilos)
    if (n % i == 0) p = 0;
  }

  return p;
}
int main()
{
  Entero_grande n;

  for (n = ENTERO_MAS_GRANDE; !primo(n); n -= 2) {
    /* NADA */
  }

  printf("El mayor primo que cabe en %lu bytes es %llu.\n",
         sizeof(Entero_grande), n);

  return 0;
}
