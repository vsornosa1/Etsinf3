#!/bin/bash                                                                     
#SBATCH --nodes=1                                                               
#SBATCH --time=5:00                                                             
#SBATCH --partition=cpa
#SBATCH --output=res_numeros.txt                                                         

OMP_NUM_THREADS=32 OMP_SCHEDULE=static,0 ./pprimo_numeros
OMP_NUM_THREADS=32 OMP_SCHEDULE=static,1 ./pprimo_numeros
OMP_NUM_THREADS=32 OMP_SCHEDULE=static,2 ./pprimo_numeros
OMP_NUM_THREADS=32 OMP_SCHEDULE=dynamic,1 ./pprimo_numeros
OMP_NUM_THREADS=32 OMP_SCHEDULE=dynamic,2 ./pprimo_numeros
