//Tema1_numar prim

//Fiecare proces lansat ia o parte de la 0 la 10; de la 11 la 20; de la 21 la 30; de la 31 la 40 
//si le verifica pe astea daca sunt prime iar daca sunt le printeaza.
// daca sunt in radacina calculez fiecare ce parte sa ia
//daca sunt pe un proces 1-2-3 primesc datele si lep relucrez

#include <iostream>
#include "mpi.h"
#include <string>
#define FirstRank 0
#define N 40

using namespace std;

bool isprime(int n)
{
	if (n <= 1) { 
		return 0;
	}
	if (n == 2) {
		return true;
	}
	if (!(n % 2)) {
		return false;  
	}
	for (int i = 2; i*i <= n; i++) { 
		if (!(n % i)) {  
			return 0;
		}
	}
	return 1;
}


int main(int argc, char *argv[])
{
	int i;

	int  my_rank;       
	int  nrProcesses;   
	int  nameSize;      

	int  parte = 0;      
	int count = 0;     

	int SendData = 0;   
	int ReceiveData = 0; 

	char computerName[MPI_MAX_PROCESSOR_NAME];       //DESKTOP-VE4Q72I

	MPI_Init(&argc, &argv); /*START MPI */

							/*Stabilește dimensiunea grupului asociat unui comunicator(cate procese sa imi fie lansate)*/
	MPI_Comm_size(MPI_COMM_WORLD, &nrProcesses);

	/*Stabilește rangul procesului de apelare în comunicator || Rangul procesului curent*/
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/*Obține numele procesorului si stocheaza dimensiunea acestuia in buffer*/
	MPI_Get_processor_name(computerName, &nameSize);

	if (my_rank == FirstRank) {

		parte = N / nrProcesses;

		cout << "Hello from process " << my_rank << " of " << nrProcesses << " processor on " << computerName << "\n\n";

		for (count = parte*FirstRank;count < parte*(FirstRank + 1);count++) 
		{
			if (isprime(count)) {
				cout << count << "  ";
			}
		}

		SendData = parte;
		for (i = 1; i < nrProcesses; i++)
		{
		//int MPI_Send(void *buf -Datele care trebuie trimise / primite (tampon aplicație). 
		//int count -(Dimensiunea (numărul de elemente) a tamponului care trebuie trimis / primit.) 
		//MPI_Datatype datatype, -Una dintre următoarele valori
		//int dest -(Rangul proceselor receptorului) , 
		//int tag -(eticheta mesajului primire/trimitere(+-), 
		//MPI_Comm comm)
			MPI_Send(&SendData, 1, MPI_INT, i, 1, MPI_COMM_WORLD);   //trimite date
		}

	}

	MPI_Status status;
	if (my_rank > FirstRank) {   //daca sunt pe alt proces
		MPI_Recv(&ReceiveData, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);   
		cout << "Hello from process " << my_rank << " of " << nrProcesses << " processor on " << computerName << "\n\n";
		for (count = ReceiveData*my_rank;count < ReceiveData*(my_rank + 1);count++)
		{
			if (isprime(count)) {
				cout << count << "  ";
			}
		}
	}

	MPI_Finalize(); /* EXIT MPI */

	return 0;
}
