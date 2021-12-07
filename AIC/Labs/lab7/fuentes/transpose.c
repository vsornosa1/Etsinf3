#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define NMAX 8192
#define BLOCKSIZE 8


void transpose_blocking( int n, int *dst, int *src ) {
    int bi,bj,i,j;

    for( i = 0; i < n; i += BLOCKSIZE )
        for( j = 0; j < n; j += BLOCKSIZE )
            for( bi = i; bi < i + BLOCKSIZE; bi++ )
                for( bj = j; bj < j + BLOCKSIZE; bj++ ) 
                    dst[bj+bi*n] = src[bi+bj*n];
        
}


void transpose( int n, int *dst, int *src ) {
    int i,j;

    for( i = 0; i < n; i++ )
        for( j = 0; j < n; j++ )
            dst[j+i*n] = src[i+j*n];
}


int main( int argc, char **argv ) {
    int i,j;

    int *A = (int*)malloc( NMAX*NMAX*sizeof(int) );
    int *B = (int*)malloc( NMAX*NMAX*sizeof(int) );

    srand48( time( NULL ) );
    for( i = 0; i < NMAX*NMAX; i++ ) A[i] = lrand48( );
    for( i = 0; i < NMAX*NMAX; i++ ) B[i] = lrand48( );

    struct timeval start, end;

    gettimeofday( &start, NULL );
    transpose( NMAX, B, A );
    gettimeofday( &end, NULL );

    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    printf( "original: %g milisegundos\n", seconds*1e3 );

    gettimeofday( &start, NULL );
    transpose_blocking( NMAX, B, A );
    gettimeofday( &end, NULL );

    seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    printf( "blocking: %g milisegundos\n", seconds*1e3 );

    for( i = 0; i < NMAX; i++ ) {
        for( j = 0; j < NMAX; j++ ) {
            if( B[j+i*NMAX] != A[i+j*NMAX] ) {
                printf("Error!!!! La transposición no se ha realizado correctamente!!\n");
                exit( -1 );
            }
        }
    } 

    free( A );
    free( B );
    return 0;
}

