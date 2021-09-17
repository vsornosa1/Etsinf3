#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define MAXCAD 100

#define NUM_PASOS 5
#define DIST_RADIO 8
#define IMAGEN_ENTRADA "Lenna.ppm"
#define IMAGEN_SALIDA "lenna-fil.ppm"

struct pixel {
  unsigned char r, g, b;
};

int lee_ppm(char *nomfich, struct pixel ***img, int *nf, int *nc)
{
  FILE *df;
  char tipo[MAXCAD];
  int nivmax, height, width, i, j;

  df = fopen(nomfich, "r");
  if (!df) return -1;   /* fopen ha fallado */

  fscanf(df, "%s", tipo);
  if (strcmp(tipo, "P3") != 0) return -2;   /* formato erróneo */

  fscanf(df, "%d%d", &width, &height);

  if ((*img = (struct pixel **)malloc(sizeof(struct pixel *) * height)) == NULL) {
    return -3;   /* malloc ha fallado */
  }
  if (((*img)[0] = (struct pixel *)malloc(sizeof(struct pixel) * width * height)) == NULL) {
    return -3;   /* malloc ha fallado */
  }
  for (i = 1; i < height; i++)
    (*img)[i] = (*img)[i - 1] + width;

  fscanf(df, "%d", &nivmax);

  for (i = 0; i < height; i++)
    for (j = 0; j < width; j++) {
      fscanf(df, "%hhu", &((*img)[i][j].r));
      fscanf(df, "%hhu", &((*img)[i][j].g));
      fscanf(df, "%hhu", &((*img)[i][j].b));
    }

  *nf = height;
  *nc = width;

  fclose(df);

  return 0;
}


int escribe_ppm(char *nomfich, struct pixel **img, int nf, int nc)
{
  FILE *df;
  int nivmax = 255, i, j;

  df = fopen(nomfich, "w");
  if (!df) return -1;   /* fopen ha fallado */

  fprintf(df, "P3\n");
  fprintf(df, "%d %d\n", nc, nf);
  fprintf(df, "%d\n", nivmax);

  for (i = 0; i < nf; i++) {
    for (j = 0; j < nc; j++) {
      fprintf(df, "%d ", img[i][j].r);
      fprintf(df, "%d ", img[i][j].g);
      fprintf(df, "%d ", img[i][j].b);
    }
    fprintf(df, "\n");
  }

  fclose(df);

  return 0;
}



int Filtro(int pasos, int radio, struct pixel **ppsImagenOrg, struct pixel **ppsImagenDst,
           int n, int m)
{
  int i, j, k, l, p, tot;

  struct { int r, g, b; } resultado;
  int **ppdBloque, v;

  if ((ppdBloque = (int **)malloc(sizeof(int *) * (2*radio + 1))) == NULL) {
    return -3;   /* malloc ha fallado */
  }
  if ((ppdBloque[0] = (int *)malloc(sizeof(int) * (2*radio + 1) * (2*radio + 1))) == NULL) {
    return -3;   /* malloc ha fallado */
  }
  for (i = 1; i < 2*radio + 1; i++)
    ppdBloque[i] = ppdBloque[i - 1] + 2*radio + 1;

  for (i = -radio; i <= radio; i++)
    for (j = -radio; j <= radio; j++)
      ppdBloque[i + radio][j + radio] = (radio - abs(i)) * (radio - abs(i)) + (radio - abs(j)) * (radio - abs(j)) + 1;

  for (p = 0; p < pasos; p++) {
    for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
        resultado.r = 0;
        resultado.g = 0;
        resultado.b = 0;
        tot = 0;
        for (k = max(0, i - radio); k <= min(n - 1, i + radio); k++) {
          for (l = max(0, j - radio); l <= min(m - 1, j + radio); l++) {
            v = ppdBloque[k - i + radio][l - j + radio];
            resultado.r += ppsImagenOrg[k][l].r * v;
            resultado.g += ppsImagenOrg[k][l].g * v;
            resultado.b += ppsImagenOrg[k][l].b * v;
            tot += v;
          }
        }
        resultado.r /= tot;
        resultado.g /= tot;
        resultado.b /= tot;
        ppsImagenDst[i][j].r = resultado.r;
        ppsImagenDst[i][j].g = resultado.g;
        ppsImagenDst[i][j].b = resultado.b;
      }
    }
    if (p+1 < pasos)
      memcpy(ppsImagenOrg[0], ppsImagenDst[0], n * m * sizeof(struct pixel));
  }
  free(ppdBloque[0]);
  free(ppdBloque);
  return 0;
}

int main()
{
  struct pixel **ImgOrg, **ImgDst;
  int n = 2560, m = 1920;
  int i, rc;

  rc = lee_ppm(IMAGEN_ENTRADA, &ImgOrg, &n, &m);
  if (rc) { printf("Error al leer el fichero %s\n", IMAGEN_ENTRADA); return 1; }
  printf("Abierta una imagen de n:%d, m:%d\n", n, m);

  ImgDst = (struct pixel **)malloc(n * sizeof(struct pixel *));
  ImgDst[0] = (struct pixel *)malloc(m * n * sizeof(struct pixel));
  for (i = 1; i < n; i++)
    ImgDst[i] = ImgDst[i - 1] + m;

  rc = Filtro(NUM_PASOS, DIST_RADIO, ImgOrg, ImgDst, n, m);
  if (rc) { printf("Error al aplicar el filtro\n"); return 2; }

  rc = escribe_ppm(IMAGEN_SALIDA, ImgDst, n, m);
  if (rc) { printf("Error al escribir la imagen\n"); return 3; }

  free(ImgOrg[0]);
  free(ImgDst[0]);
  free(ImgOrg);
  free(ImgDst);

  return 0;
}
