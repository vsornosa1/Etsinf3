#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#define NMAX 1024

void multMat_ijk( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* ijk */
    for( i = 0; i < n; i++ )
        for( j = 0; j < n; j++ )
            for( k = 0; k < n; k++ )
                C[i+j*n] += A[k+j*n]*B[i+k*n];

}

void multMat_ikj( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* ikj */
    for( i = 0; i < n; i++ )
        for( k = 0; k < n; k++ )
            for( j = 0; j < n; j++ )
                C[i+j*n] += A[k+j*n]*B[i+k*n];

}

void multMat_jik( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* jik */
    for( j = 0; j < n; j++ )
        for( i = 0; i < n; i++ )
            for( k = 0; k < n; k++ )
                C[i+j*n] += A[k+j*n]*B[i+k*n];

}

void multMat_jki( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* jki */
    for( j = 0; j < n; j++ )
        for( k = 0; k < n; k++ )
            for( i = 0; i < n; i++ )
                C[i+j*n] += A[k+j*n]*B[i+k*n];

}

void multMat_kij( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* kij */
    for( k = 0; k < n; k++ )
        for( i = 0; i < n; i++ )
            for( j = 0; j < n; j++ )
                C[i+j*n] += A[k+j*n]*B[i+k*n];

}

void multMat_kji( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* kji */
    for( k = 0; k < n; k++ )
        for( j = 0; j < n; j++ )
            for( i = 0; i < n; i++ )
                C[i+j*n] += A[k+j*n]*B[i+k*n];

}

int main( int argc, char **argv ) {
    int i,n;

    void (*orderings[])(int,float *,float *,float *) = {&multMat_ijk,&multMat_ikj,&multMat_jik,&multMat_jki,&multMat_kij,&multMat_kji};
    char *names[] = {"ijk","ikj","jik","jki","kij","kji"};

    float *A = (float *)malloc( NMAX*NMAX * sizeof(float));
    float *B = (float *)malloc( NMAX*NMAX * sizeof(float));
    float *C = (float *)malloc( NMAX*NMAX * sizeof(float));

    struct timeval start, end;

    for( i = 0; i < NMAX*NMAX; i++ ) A[i] = drand48()*2-1;
    for( i = 0; i < NMAX*NMAX; i++ ) B[i] = drand48()*2-1;
    for( i = 0; i < NMAX*NMAX; i++ ) C[i] = drand48()*2-1;

    for( i = 0; i < 6; i++) {

        gettimeofday( &start, NULL );
        (*orderings[i])( NMAX, A, B, C );
        gettimeofday( &end, NULL );

        double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
        double Gflops = 2e-9*NMAX*NMAX*NMAX/seconds;    
        printf( "%s:\tn = %d, %.3f Gflop/s\n", names[i], NMAX, Gflops );
    }

    free( A );
    free( B );
    free( C );

    printf("\n\n");

    return 0;
}
