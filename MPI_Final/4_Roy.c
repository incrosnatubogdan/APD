/*
fiecare proces imi calculeaza o parte din matrice si imi scrie in ea noul cost daca este mai mic.
apoi imi combina valorile de la toate procesele, apoi radacina imi creaza o matrice noua cu noie valori
din radacina se trimite un mesaj la toate celelalte procese
asteapta pana cand toate procesele ajung la acelasi nivel
afiseaza noua matrice
*/



#include "mpi.h"
#include <iostream>
using namespace std;

#define N 6

int main(int argc, char *argv[])
{
	int a[N][N] = {
		{ 1, 2, 5, 979, 999, 999 },
		{ 999, 12, 4, 2, 999, 3 },
		{ 999, 999, 0, 8, 999, 999 },
		{ 999, 999, 999, 0, 3, 999 },
		{ 999, 999, 2, 999, 8, 3 },
		{ 999, 5, 999, 2, 0,8 }
	};
	int result[N][N];
	int nrProcesses, my_rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nrProcesses);

	

	for (int k = 0; k < N; k++)    
	{
		for (int i = my_rank; i < N; i = i + nrProcesses)
		{
			for (int j = 0; j < N; j++)
			{
				if (a[i][j] > a[i][k] + a[k][j])
					a[i][j] = a[i][k] + a[k][j];    
			}
		}
		
		MPI_Reduce(a, result, N*N, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
		if (my_rank == 0) {
			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++)
					a[i][j] = result[i][j];
			MPI_Bcast(a, N*N, MPI_INT, 0, MPI_COMM_WORLD);
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	if (my_rank == 0){
		for (int i = 0; i < N; i++){
			for (int j = 0; j < N; j++)
				cout << a[i][j] << " ";
			cout << "\n";
		}

	}

	MPI_Finalize();


	system("pause");
	return 0;
}