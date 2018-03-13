#include <mpi.h>
#include <stdio.h>
#define SIZE 4


int main(int argc, char const *argv[])
{
	int numtasks, rank, sendcount, recvcount, source;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	// Sum the numbers locally
	int local_sum = rank + 1;

	// Print the random numbers on each process
	printf("Local sum for process %d - %f\n",
	       rank, local_sum);

	// Reduce all of the local sums into the global sum
	int global_sum;
	MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0,
	           MPI_COMM_WORLD);

	// Print the result
	if (rank == 0) {
	  printf("Total sum = %d\nx", global_sum);
	}

	/* code */
	return 0;
}