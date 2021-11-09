/* 
1. Paralelizar el siguiente programa mediante MPI de modo que sea el proceso 0 quien
pida un número al usuario y se lo envíe al proceso 1, el cual lo mostrará por pantalla
tras recibirlo.
*/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    double numero;
    int myID;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);

    if (myID == 0)
    {
        printf("Dame un num: ");
        scanf("%lf", &numero);
        /* MPI_Send(DirMemoriaBuffer, nDatosEnviados, MPI_datatype, idReceptor, etiqueta, comunicador); */
        MPI_Send(&numero, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
    }
    else if (myID == 1)
    {
        /* MPI_Send(DirMemoriaBuffer, nDatosEnviados, MPI_datatype, idEmisor, etiqueta, DirMemoriaStatus); */
        MPI_Recv(&numero, 1, MPI_DOUBLE, 0, 0, &status);
        printf("El num proporcionado es el %f\n", numero);
    }

    MPI_Finalize();
    return 0;
}