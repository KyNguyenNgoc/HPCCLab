#include <mpi.h>
#include <stdio.h>
#define SIZE 4


int main(int argc, char const *argv[])
{
	
	// Sum the numbers locally
	int local_sum = world_rank + 1;

	// Print the random numbers on each process
	printf("Local sum for process %d - %f\n",
	       world_rank, local_sum);

	// Reduce all of the local sums into the global sum
	int global_sum;
	MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0,
	           MPI_COMM_WORLD);

	// Print the result
	if (world_rank == 0) {
	  printf("Total sum = %d\n", global_sum);
	}

	/* code */
	return 0;
}