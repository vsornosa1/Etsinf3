/*
2. Dado el siguiente programa, paralelizarlo mediante MPI de manera que el proceso 0
inicialice el vector a y se lo envíe al proceso 1, quien lo mostrará por pantalla tras
recibirlo.
*/

#include <stdio.h>
#include <mpi.h>

#define N 5

int main(int argc, char **argv)
{
    int a[N], i;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);

    if (myId == 0)
    {
        for (i = 0; i < N; i++)
            a[i] = i;
        /* MPI_Send(DirMemoriaBuffer, nDatosEnviados, MPI_datatype, idReceptor, etiqueta, comunicador); */
        MPI_Send(&a[0], N, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if (myId == 1)
    {
        /* MPI_Send(DirMemoriaBuffer, nDatosEnviados, MPI_datatype, idEmisor, etiqueta, DirMemoriaStatus); */

        /* OPCION 1: 
        MPI_Recv(a, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); */

        /* OPCION 2 - Igual de valida pero con el any */
        MPI_Recv(a, N, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        printf("Los elementos del vector son: \n");
        for (i = 0; i < N; i++)
            printf("%d\n", a[i]);
    }

    MPI_Finalize();
    return 0;
}