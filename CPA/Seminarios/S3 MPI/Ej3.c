/*
3. El siguiente programa suma los n primeros números naturales, siendo dicho valor
de n proporcionado como un dato de entrada al programa. Paralelizarlo mediante MPI
de manera que el proceso 0 muestre el resultado de la suma por pantalla.
*/

#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int n, i, suma_total, suma_local;
    int myId, nProcesos;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);

    if (argc == 2)
        n = atoi(argv[1]);
    else
        n = 10;

    MPI_Comm_size(MPI_COMM_WORLD, &nProcesos);
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);

    suma_local = 0;
    for (i = myId + 1; i <= n; i += nProcesos) // Schedule: Dynamic,nProcesos
        suma_local += i;

    if (myId == 0)
    {
        for (i = 1; i < nProcesos; i++)
        {
            suma_total = suma_local; /* ¡El proceso 0 tambien hace su suma! */
            MPI_Recv(&suma_local, 1, MPi_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            suma_total += suma_local;
        }
        printf("Valor de la suma = %d\n", suma_total);
    }
    else
    {
        /* MPI_Send(DirMemoriaBuffer, nDatosEnviados, MPI_datatype, idEmisor, etiqueta, DirMemoriaStatus); */
        MPI_Send(&suma_local, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}