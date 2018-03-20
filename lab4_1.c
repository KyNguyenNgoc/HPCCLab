/*
@student: Nguyen Ngoc Ky
@MSSV: 1511680
*/
#include <mpi.h>
#include <stdio.h>
#include <math.h>
typedef enum { false, true } bool;

// Utility function to do modular exponentiation.
// It returns (x^y) % p
int power(int x, unsigned int y, int p)
{   
    // repeated squaring algorithm
    int res = 1;      // Initialize result
    x = x % p;  // Update x if it is more than or
                // equal to p
    while (y > 0)
    {
        // If y is odd, multiply x with result
        if (y & 1)
            res = (res*x) % p;
 
        // y must be even now
        y = y>>1; // y = y/2
        x = (x*x) % p;
    }
    return res;
}
 
// This function is called for all k trials. It returns
// false if n is composite and returns true if n is
// probably prime.
// d is an odd number such that  d*2^r = n-1
// for some r >= 1
bool millerTest(int d, int n)
{
    // Pick a random number in [2..n-2]
    // Corner cases make sure that n > 4
    int a = 2 + rand() % (n - 4);
 
    // Compute a^d % n --> the 'smallest' starting exponent
    int x = power(a, d, n);
 
    if (x == 1  || x == n-1)
       return true;
 
    // Keep squaring x(i.e., doubling the exponent) 
    // while one of the following doesn't happen
    // (i)   d does not reach n-1
    // (ii)  (x^2) % n is not 1
    // (iii) (x^2) % n is not n-1
    while (d != n-1)
    {
        x = (x * x) % n;
        d *= 2;
 
        if (x == 1)      return false; // Return composite
        if (x == n-1)    return true;   // Return probably prime
    }
 
    // Return composite
    return false;
}
 
// It returns false if n is composite and returns true if n
// is probably prime.  k is an input parameter that determines
// accuracy level. Higher value of k indicates more accuracy.
bool isPrime(int n, int k)
{
    // Corner cases
    if (n <= 1 || n == 4)  return false;
    if (n <= 3) return true;
 
    // Find r such that n = 2^d * r + 1 for some r >= 1
    int d = n - 1;
    while (d % 2 == 0)
        d /= 2;
 
    // Iterate given nber of 'k' times
    int i;
    for (i = 0; i < k; i++)
         if (millerTest(d, n) == false)
              return false;
 
    return true;
}

int main (int argc, char *argv[])
{
	int rank;
	int nTasks;
	long pCount, pCountSum;
	long limit = 1000000000;
	int source = 0;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nTasks);

	long step = nTasks*2;
	long start = rank*2 + 1;
	long i;
	pCount = 0;
	double start_time = MPI_Wtime(); 
	if (rank == source){
		for (i = start; i <= limit; i+=step){
			if (isPrime(i, 10) == true) { // auxiliary input k = 10; i will be tested 10 times
				pCount++;
			}
		}
		MPI_Reduce(&pCount,&pCountSum,1,MPI_INT,MPI_SUM,source,MPI_COMM_WORLD);
		double end_time=MPI_Wtime();
		printf("The number of primes from 1 to 1000000000: %d\n", pCountSum + 1); // Counting 2.
		printf("Time: %.2lf\n", end_time - start_time); 
	} else {
		for (i = start; i <= limit; i+=step){
			if (isPrime(i, 10) == true) {
				pCount++;
			}
		}
		MPI_Reduce(&pCount,&pCountSum,1,MPI_INT,MPI_SUM,source,MPI_COMM_WORLD);
	}
	MPI_Finalize();
}

