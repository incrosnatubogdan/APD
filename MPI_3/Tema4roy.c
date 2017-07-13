Matricea costurilor optime



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
	int numprocs, rank;

	MPI_Init(&argc, &argv);
	//Mediul MPI este inițializat prin apelarea funcției MPI_Init. MPI_Init trebuie apelat înainte
	//Orice alt apel pentru funcția MPI și trebuie apelat o singură dată pe program
	
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//Pentru a obține rangul procesului curent, puteți apela:
	
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	//MPI_COMM_WORLD este un comunicator predefinit care include toate procesele din MPI
	// fiecare proces are un identificator unic, numit Rank, care este un întreg între
	//0 și numărul de procese din comunicator. Fiecare proces va primi un rang în inițiere
	//fază. Rangul este necesar atunci când trimiteți / primiți mesaje pentru a identifica în mod unic sursa
	

	for (int k = 0; k < N; k++)
	{
		for (int i = rank; i < N; i = i + numprocs)
		{
			for (int j = 0; j < N; j++)
			{
				if (a[i][j] > a[i][k] + a[k][j])
					a[i][j] = a[i][k] + a[k][j];
			}
		}
		//min a=result, matricea costurilor minime
		
		MPI_Reduce(a, result, N*N, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
	// Combină valorile expediate de toate procesele utilizând un operator predefinit, iar locurile au ca rezultat recepția Tamponul procesului rădăcină.
	/*
	Sendbuf - adresa tamponului de trimitere
	- count - numărul elementelor din bufferul de trimitere
	- datatype - tipul de date al elementelor de tampon de trimitere
	- op - reduce operațiunea
	- rădăcină - rang de proces rădăcină
	- comunicator de comunicare
	- recvbuf - adresa tamponului de primire
	- cerere - cerere de comunicare
	*/
		
		if (rank == 0) {
			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++)
					a[i][j] = result[i][j];
			MPI_Bcast(a, N*N, MPI_INT, 0, MPI_COMM_WORLD);
			/*
			Transmite un mesaj din proces cu rangul "rădăcină" la toate celelalte procese ale comunicatorului parametri:
			- tampon - datele care trebuie trimise
			- numărul - mărimea tamponului de date
			- tipul de date tip
			- root - expeditor
			- comunicator de comunicare
			- cerere - cerere mâner
			*/
		}
		MPI_Barrier(MPI_COMM_WORLD);
		//Blochează procesul curent până la toate celelalte procese din comunicatorul actual au ajuns la această rutină.
		//parametri: - comunicatorul utilizat în comunicarea colectivă
	}

	if (rank == 0){
		for (int i = 0; i < N; i++){
			for (int j = 0; j < N; j++)
				cout << a[i][j] << " ";
			cout << "\n";
		}

	}
	//Un program MPI se termină cu un apel către:
	MPI_Finalize();


	system("pause");
	return 0;
}