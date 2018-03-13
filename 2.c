#include <mpi.h>
#include <stdio.h>
#define SIZE 4

// MPI_reduce to compute the sum of all integers from other processes
int main(int argc, char const *argv[])
{
	int numtasks, rank, sendcount, recvcount;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	// Scatter the data
	int *data = NULL;
	sendcount = recvcount = 1;
	if (rank == 0)
	{
		data = (int *) malloc(sizeof(int) * sendcount * numtasks);
		for (int i = 0; i < numtasks; ++i)
		{
			data[i] = i + 1;
		}
	}

	int *sub_local_elements = malloc(sizeof(int) * sendcount);

	// Scatter the random numbers to all processes
	MPI_Scatter(data, sendcount, MPI_INT, sub_local_elements, recvcount, MPI_INT, 0, MPI_COMM_WORLD);

	// Compute the local sum
	int *sub_sums = NULL;
	int local_sum = 0;
	int i;
	for (i = 0; i < sendcount; ++i)
	{
		local_sum += sub_local_elements[i];
	}

	// If the current proc. is root then store all other sub_local_sums
	if (rank == 0)
	{
		sub_sums = malloc(sizeof(int) * numtasks);
	}

	// The root gathers all data
	MPI_Gather(&local_sum, 1, MPI_INT, sub_sums, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// Compute the total sum
	if (rank == 0)
	{
		int total_sum = 0;
		int i;
		for (i = 0; i < numtasks; ++i)
		{
			printf("Received: %d\n", sub_sums[i]);
			total_sum += sub_sums[i];
		}

		// Print out the total
		printf("The total sum is: %d\n", total_sum);
	}

	MPI_Finalize();
	/* code */
	return 0;
}