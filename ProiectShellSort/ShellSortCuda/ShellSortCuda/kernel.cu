
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
	int gap;

	for (gap = N / 2; gap > 0; gap /= 2)

		for (i = gap; i < N; i++)

			for (j = i - gap; j >= 0 && a[j]>a[j + gap]; j -= gap) {

				temp = a[j];

				a[j] = a[j + gap];

				a[j + gap] = temp;

			}
}

int main()
{
	size_t N = 10;
	int array[] = { 9, 6, 1, 2, 7, 3, 8, 4, 10, 5 };
	int  i;

	int *d_a;
	cudaMalloc(&d_a, N);
	memcpy(d_a, array, sizeof(int)*N);


	int numBlocks = 1;
	
	// Launch a kernel on the GPU with N thread for each element.
	for (int k = 0; k < N; k++) {

		ShellSortKernel << <numBlocks, N >> >(d_a, N);
	}

   
	for (i = 0; i < N; i++)
		printf("%d ", array[i]);

	cudaFree(d_a);
    

    return 0;
}


