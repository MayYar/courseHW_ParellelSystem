#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int isprime(long long int n) { 
	int i,squareroot; 
	if (n>10) { 
		squareroot = (long long int) sqrt(n); 
		for (i=3; i<=squareroot; i=i+2) 
			if ((n%i)==0) 
				return 0; 
		return 1; 
	} else 
		return 0; 
}

int main(int argc, char *argv[]) {
	
	long long int pc, /* prime counter */ 
		foundone, /* most recent prime found */
	       pcsum;	
	long long int n, limit, maxprime;
	int rank, size;
	
	sscanf(argv[1],"%llu",&limit);
        
	MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);	
	
  	foundone = 0;
  	pc = 0;
	
	if(rank == 0){
		
		printf("Starting. Numbers to be scanned= %lld\n",limit);
		pc=4; /* Assume (2,3,5,7) are counted here */
		for (n=11; n<=limit; n=n+(size*2)) { 
			if (isprime(n)) { 
				pc++; 
				foundone = n; 
			}
		} 
		MPI_Reduce(&pc, &pcsum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	  	MPI_Reduce(&foundone, &maxprime, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
		printf("Done. Largest prime is %d Total primes %d\n",maxprime , pcsum);
	}
	if(rank > 0){ /*Slave*/
		for (n = 11+rank*2; n <= limit; n = n+(size*2)) {
			if (isprime(n)) {
		  		pc++;
		  		foundone = n;
			}
	  	}
	  	MPI_Reduce(&pc, &pcsum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	  	MPI_Reduce(&foundone, &maxprime, 1, MPI_INT, MPI_MAX, 0,MPI_COMM_WORLD);
  	}
  	MPI_Finalize();

	return 0;
}
