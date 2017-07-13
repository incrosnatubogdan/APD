
#include "mpi.h"
#include <time.h>
#include <iostream>
using namespace std;

#define SIZE 12
#define NR 3

int main(int argc, char *argv[])
{
	int rank, numProcs, piece, index;
	int array[SIZE], segment[SIZE], found[SIZE], finalFound[SIZE + 10];
	bool display = false;

	srand((unsigned int)time(NULL));

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

	piece = SIZE / numProcs;
	if (SIZE % numProcs != 0)
		++piece;
	
	
//Blochează procesul curent până cand toate celelalte procese din comunicatorul actual au ajuns la această rutină.
//parametru -comm :Comunicatorul folosit în comunicarea colectivă
	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == 0)
	{
		cout << "Array elements are:\n";
		for (int i = 0; i < SIZE; i++)
		{
			array[i] = rand() % 5;
			cout << array[i] << " ";
			finalFound[i] = -1;
		}
	}
	
	
	//trimite bucati egale de date de la root catre toate procesele din com
	/*
	Procesul rădăcină trimite date de dimensiune egală în memoria tampon 
	de aplicație tuturor celorlalte procese din comunicator. 
	Fiecare proces va fi trimis unui alt segment de date.
	*/
	MPI_Scatter(array, piece, MPI_INT, segment, piece, MPI_INT, 0, MPI_COMM_WORLD);
	cout << "\n\nRank " << rank << " Piece size: " << piece << "\n";

	for (int i = 0; i < piece; i++)
		found[i] = -1;

	index = 0;
	for (int i = 0; i < piece; i++)
	{
		if (segment[i] == NR)
		{
			found[++index] = i + rank * piece;
		}
	}
	
	
	/*
	Procesul rădăcină adună date din toate celelalte procese și îl plasează într-un buffer 
	de primire comandat de către Expeditorului.
	*/
	//ia  rez de la toate procesele si le pune in vectorul finalFound
	MPI_Gather(found, piece, MPI_INT, finalFound, piece, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		for (int i = 0; i < SIZE + 10; i++)
		{
			if (finalFound[i] >= 0)
			{
				if (!display)
					cout << "\n\nNumber found on the follwing position(s): \n";
				display = true;
				cout << finalFound[i] << " ";
			}
		}
		if (!display)
			cout << "\n\nNumber not found.";
	}

	MPI_Finalize();
	cout << endl;
	return 0;
}
