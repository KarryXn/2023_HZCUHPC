#!/bin/bash -l
#SBATCH --job-name=pivot_first_test
#SBATCH --partition=48core
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --error=%j.err
#SBATCH --output=%j.out
 
gcc -g -O3 -lm -fopenmp  -std=c99  ss.c -o testpara	
./testpara




