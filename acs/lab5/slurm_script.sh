#!/bin/bash
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=4

mpirun ./main
