#! /bin/bash
#SBATCH --nodes=2
#SBATCH -n 4
#SBATCH --ntasks-per-node=2
#SBATCH --time=00:03:00

module purge
module load intel
export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so
surn ./a.out
