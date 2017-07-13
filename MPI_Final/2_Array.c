//Utilizați MPI_Broadcast pentru trimiterea matricei.
//Dacă elementul este găsit, imprimați maximul Indice de poziție. 
//Pentru a calcula poziția maximă, trebuie să utilizați MPI_Reduce.

//se imparte dimensiunea array-ului la procese si fiecare analizeaza numarul. apoi il afiseaza si imprima unde a fost 
//gasit ultima data numarul respectiv, iar daca nu gaseste nimic procesul respectiv afiseaza 0.

//array MPI Reduce and Broadcast

#include <iostream>
#include <mpi.h>
#include <math.h>

using namespace std;

#define MAXSIZE 10   //dimensiune array

int main(int argc, char *argv[])
{
	int  my_rank;      
	int  nrProcesses;   

	int arr[MAXSIZE], i, parte, low, high, myresult = 0, result;
	int pozitie = 0;
	int nrcautat = 3;


	MPI_Init(&argc, &argv); /*START MPI */

							/*Stabilește dimensiunea grupului asociat unui comunicator(cate procese sa imi fie lansate)*/
	MPI_Comm_size(MPI_COMM_WORLD, &nrProcesses);

	/*Stabilește rangul procesului de apelare în comunicator || Rangul procesului curent*/
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


	if (0 == my_rank) {   
		cout << "Enter Array Elements : ";
		for (i = 0; i<MAXSIZE; i++) {
			cin >> arr[i];
		}
	}

	
	MPI_Bcast(arr, MAXSIZE, MPI_INT, 0, MPI_COMM_WORLD);

	
	parte = MAXSIZE / nrProcesses; 
	low = my_rank * parte;
	high = low + parte;
	for (i = low; i<high; i++) {   //imi cauta nr in array
		if (nrcautat == arr[i]) {
			pozitie = i;
		}
	}
	cout << "I got pozition " << pozitie << " from " << my_rank << "\n";

	
	MPI_Reduce(&pozitie, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (0 == my_rank) {
		if (result == 0)
		{
			cout << "Number NOT found..!!\n";
		}
		else {
			cout << "Number FOUND at MAX position " << result << "\n";
		}
	}


	MPI_Finalize(); /* EXIT MPI */

	return 0;
}