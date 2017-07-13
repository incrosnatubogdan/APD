Scrieți un program care caută un element în interiorul unui matrice.
A. Utilizați MPI_Broadcast pentru trimiterea matricei. Dacă elementul este găsit, imprimați maximul
Indice de poziție. 
Pentru a calcula poziția maximă, trebuie să utilizați MPI_Reduce.








#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define SIZE 100
#define value 42


int main(int argc, char *argv[])
{
    int rank, size, segmentSize, startP, endP, found = -1, finalFound = -2;
    int array[SIZE];

    srand(time(NULL) + 1);

    MPI_Init(&argc, &argv);
	//Mediul MPI este inițializat prin apelarea funcției MPI_Init. MPI_Init trebuie apelat înainte
	//Orice alt apel pentru funcția MPI și trebuie apelat o singură dată pe program
	
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//Pentru a obține rangul procesului curent, puteți apela:
	
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	//Parametrul comm- MPI_COMM_WORLD reprezintă comunicatorul, în timp ce &size-dimensiunea parametrului de ieșire va stoca dimensiunea comunicatorului.
	//MPI_COMM_WORLD este un comunicator predefinit care include toate procesele din MPI
	// fiecare proces are un identificator unic, numit Rank, care este un întreg între
	//0 și numărul de procese din comunicator. Fiecare proces va primi un rang în inițiere
	//fază. Rangul este necesar atunci când trimiteți / primiți mesaje pentru a identifica în mod unic sursa
	

    if (rank == 0)
    {
        for (int i = 0; i < SIZE; i++)
        {
            array[i] = rand() % 100;
        }
    }

    segmentSize = SIZE / size;//Fiecare își primește partea din matrice
    if (SIZE % size != 0)
        segmentSize++;

    MPI_Bcast(array, SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	/*
	Transmite un mesaj din proces cu rangul "rădăcină" la toate celelalte procese ale comunicatorului parametri:
	- tampon - datele care trebuie trimise
	- numărul - mărimea tamponului de date
	- tipul de date tip
	- root - expeditor
	- comunicator de comunicare
	- cerere - cerere mâner
	*/

    startP = rank * segmentSize;
    endP = startP + segmentSize - 1;

    for (int i = startP; i <= endP; i++)
    {

        if (array[i] == value)
        {
            found = i;
        }
    }

    MPI_Reduce(&found, &finalFound, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
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

    if (rank == 0)
    {
        if (finalFound == -2)
            printf("\nThe number wasn`t found\n");
        else printf("\nNumber found on position %d", finalFound);
    }

	//Un program MPI se termină cu un apel către:
    MPI_Finalize();

}