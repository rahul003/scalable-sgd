#!/bin/bash
# Don't miss this line!
#----------------------------------------------------
# Generic SLURM script -- MPI
#----------------------------------------------------
#SBATCH -J myjob 			#Job name
#SBATCH -o myjob.%j.out 	#stdout; %j expands to jobid
#SBATCH -e myjob.%j.err 	#stderr; skip to combine stdout and stderr
#SBATCH -p normal		#queue
#SBATCH -N 1				#Number of nodes, not cores (16 cores/node)
#SBATCH -n 16
#SBATCH -t 08:00:00				
#SBATCH --mail-user=h.rahul@utexas.edu
#SBATCH --mail-type=ALL
./bin/runner # Use ibrun for MPI codes. Don’t use mpirun or srun