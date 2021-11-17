/* Ping-pong program */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NMAX 1000000
#define NREPS 100

char buf[NMAX];

int main(int argc,char *argv[])
{
  int n, myid, numprocs, i;
  double tPreBucle, tPostBucle;
  
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);

  /* The program takes 1 argument: message size (n), with a default size of 100
     bytes and a maximum size of NMAX bytes*/
  if (argc==2) n = atoi(argv[1]);
  else n = 100;
  if (n<0 || n>NMAX) n=NMAX;

  /* COMPLETE: Get current time, using MPI_Wtime() */
  tPreBucle = MPI_Wtime(); 


  /* COMPLETE: loop of NREPS iterations.
     In each iteration, P0 sends a message of n bytes to P1, and P1 sends the same message back to P0. 
     The data sent is taken from array buf and received into the same array.
     */
  for(i = 0; i < NREPS; i++) {
      if(myid == 0) {
          MPI_Send(&buf, 1, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
          MPI_Recv(&buf, 1, MPI_BYTE, 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(&buf, 1, MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&buf, 1, MPI_BYTE, 0, 1, MPI_COMM_WORLD);
    }
  }

  /* COMPLETE: Get current time, using MPI_Wtime() */
  tPostBucle = MPI_Wtime();

  /* COMPLETE: Only in process 0.
     Compute the time of transmission of a single message (in milliseconds) and print it.
     Take into account there have been NREPS repetitions, and each repetition involves 2
     messages. */
    if(myid == 0) {
        tPostBucle -= tPreBucle;
        printf("Tiempo de ejecución para %d iteraciones: %.5f\n", NREPS, tPostBucle);
    }
        
  MPI_Finalize();
  return 0;
}

