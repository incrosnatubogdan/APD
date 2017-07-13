
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


__global__ void ShellSortKernel( int *a, int N) 
{
    int i = threadIdx.x;  
	int j = threadIdx.y;
 
	int  temp;
	int dim;   

	for (dim = N / 2; dim > 0; dim /= 2)   

		for (i = dim; i < N; i++)  

			for (j = i - dim; j >= 0 && a[j]>a[j + dim]; j -= dim) {

				temp = a[j];   

				a[j] = a[j + dim];

				a[j + dim] = temp;

			}
}

int main()
{
	size_t N = 10;
	int array[] = { 9, 6, 1, 2, 7, 3, 8, 4, 10, 5 };
	int  i;

	int *d_a;  //vectorul buffer alocat 
	cudaMalloc(&d_a, N);  //alocam memorie liniara  // se aloca vectorul in memoria dispozitivului si dimensiunea acestuia
	
	memcpy(d_a, array, sizeof(int)*N);    //transferul de date între memoria gazdă și memoria dispozitivului
	


	int numBlocks = 1;
	
	// Lansați un kernel pe GPU cu fir N pentru fiecare element.
	for (int k = 0; k < N; k++) {

		ShellSortKernel << <numBlocks, N >> >(d_a, N);
		
	}

   
	for (i = 0; i < N; i++)
		printf("%d ", array[i]);

	cudaFree(d_a);
    

    return 0;
}


