
Scrieți un program care imprimă toate numerele prime mai mici de N folosind procesele M


#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LIMIT     2500000
#define FIRST     0           /*prima sarcină */

int isprime(int n)
{
    int i, squareroot;
    if (n>10)
    {
        squareroot = (int)sqrt(n);
        for (i = 3; i <= squareroot; i = i + 2)
            if ((n%i) == 0)
                return 0;
        return 1;
    }
    else
        return 0;
}


int main(int argc, char *argv[])
{
    int   ntasks,               /* total number of tasks in partitiion */
          rank,                 /* task identifier */
          n,                    /* loop variable */
          pc,                   /* prime counter */
          pcsum,                /* number of primes found by all tasks */
          foundone,             /* most recent prime found */
          maxprime,             /* largest prime found */
          mystart,              /* where to start calculating */
          stride;               /* calculate every nth number */

    double start_time, end_time;

    MPI_Init(&argc, &argv);
	//Mediul MPI este inițializat prin apelarea funcției MPI_Init. MPI_Init trebuie apelat înainte
	//de orice alt apel pentru funcția MPI și trebuie apelat o singură dată pe program
    
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//Pentru a obține rangul procesului curent, puteți apela:
	
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
	//MPI_COMM_WORLD este un comunicator predefinit care include toate procesele din MPI
	// fiecare proces are un identificator unic, numit Rank, care este un întreg între faza
	// Rangul este necesar atunci când trimiteți / primiți mesaje pentru a identifica în mod unic sursa
			
    if (((ntasks % 2) != 0) || ((LIMIT%ntasks) != 0))
    {
        printf("Sorry - acest exercițiu necesită un număr par de sarcini..\n");
        MPI_Finalize();
        exit(0);
    }

    start_time = MPI_Wtime();   //Pentru a obține timpul scurs pentru procesul curent, apelați:
    mystart = (rank * 2) + 1;       /* Punct de plecare - trebuie să fie un număr impar */
    stride = ntasks * 2;          /*Determinați pașii, sărind chiar și numerele */
    pc = 0;                       /* Inițializați contorul prim */
    foundone = 0;

    if (rank == FIRST)
    {
        printf("Using %d tasks to scan %d numbers\n", ntasks, LIMIT);
        pc = 4;                  /* first four primes are counted here */
        for (n = mystart; n <= LIMIT; n = n + stride)
        {
            if (isprime(n))
            {
                pc++;
                foundone = n;
            }
        }
        MPI_Reduce(&pc, &pcsum, 1, MPI_INT, MPI_SUM, FIRST, MPI_COMM_WORLD);
		// Combină valorile expediate de toate procesele utilizând un operator predefinit, 
		//iar locurile au ca rezultat recepția Tamponul procesului rădăcină.
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
        MPI_Reduce(&foundone, &maxprime, 1, MPI_INT, MPI_MAX, FIRST, MPI_COMM_WORLD);
        end_time = MPI_Wtime();  //Următoarele rutine așteaptă finalizarea solicitărilor:
        printf("Done. Largest prime is %d Total primes %d\n", maxprime, pcsum);
        printf("Wallclock time elapsed: %.2lf seconds\n", end_time - start_time);
    }


    if (rank > FIRST)
    {
        for (n = mystart; n <= LIMIT; n = n + stride)
        {
            if (isprime(n))
            {
                pc++;
                foundone = n;
            }
        }
        MPI_Reduce(&pc, &pcsum, 1, MPI_INT, MPI_SUM, FIRST, MPI_COMM_WORLD);
        MPI_Reduce(&foundone, &maxprime, 1, MPI_INT, MPI_MAX, FIRST, MPI_COMM_WORLD);
    }

	//Un program MPI se termină cu un apel către:
    MPI_Finalize();   
}