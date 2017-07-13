#include "mpi.h"
#include <stdio.h>
#include <cstdlib>
#include <math.h>
#define MAXSIZE 100
#define SNR 10

int main(int argc, char **argv)
{
	int myid, numprocs;
	int data[MAXSIZE], i, x, low, high, myresult = 0, result;
	

	MPI_Init(&argc, &argv);   //Mediul MPI este inițializat prin apelarea funcției. 
								//Parametrii argc și argv sunt argumentele liniei de comandă și vor fi transmise tuturor proceselor.

	//Dimensiunea unui comunicator poate fi obținută apelând:
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
			//MPI_COMM_WORLD este un comunicator predefinit care include toate procesele din MPI
			// fiecare proces are un identificator unic, numit Rank, care este un întreg între
			//0 și numărul de procese din comunicator. Fiecare proces va primi un rang în inițiere
			//fază. Rangul este necesar atunci când trimiteți / primiți mesaje pentru a identifica în mod unic sursa
	
	//Pentru a obține rangul procesului curent, puteți apela:
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);


if (0 == myid) {
	for (i = 0; i < MAXSIZE; i++) {
		data[i] = rand() %20 + 1;
		printf("%d   ", data[i]);
	}
}
printf("\n");
/* broadcast data */
// Transmite un mesaj din proces cu rangul "rădăcină" la toate celelalte procese ale comunicatorului
// Utilizați MPI_Broadcast pentru trimiterea array
MPI_Bcast(data, MAXSIZE, MPI_INT, 0, MPI_COMM_WORLD);

/* add portion of data */
x = MAXSIZE / numprocs; /* must be an integer *///impart in bucati vectorul, fiecare proces face o parte
low = myid * x;
high = low + x;
for (i = low; i<high; i++) {
	if (data[i] == SNR) {
		myresult = i;
	}
}
printf("I got %d from %d\n", myresult, myid);

// Combină valorile trimise de toate procesele folosind un operator predefinit și locurile rezultă în tamponul de primire al procesului rădăcină.
// Pentru a calcula poziția maximă, trebuie să utilizați MPI_Reduce.
MPI_Reduce(&myresult, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
if (0 == myid) {
	printf("The maximum position is %d.\n", result);
}
MPI_Finalize();     //Un program MPI se termină cu un apel către
}
