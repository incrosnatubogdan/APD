
Scrieți un program care caută un element în interiorul unui matrice
b. Utilizați scatter pentru a trimite matrice. Dacă elementul este găsit de mai multe ori, imprimați toate pozițiile sale.
Utilizați MPI_Gather pentru trimiterea înapoi a pozițiilor.







int main(int argc, char *argv[])
{
	int rank, size, segmentSize, index, found[SIZE], finalFound[SIZE + 10], poz;
	int arr[SIZE], segment[SIZE];
	bool ok = false;

	srand(time(NULL));
	
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

	segmentSize = SIZE / size;
	if (SIZE % size != 0) {
		++segmentSize;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	//Blochează procesul curent până la toate celelalte procese din comunicatorul actual au ajuns la această rutină.
	//parametri: - comunicatorul utilizat în comunicarea colectivă

	if (rank == 0) {
		printf("Array elements are:\n");
		for (int i = 0; i < SIZE; i++) {
			arr[i] = rand() % 100;
			printf("%d, ", arr[i]);
			finalFound[i] = -1;
		}
		arr[35] = 95;
		arr[98] = 95;
	}

	MPI_Scatter(arr, segmentSize, MPI_INT, segment, segmentSize, MPI_INT, 0, MPI_COMM_WORLD);
	/*
	Procesul rădăcină trimite date de dimensiune egală în memoria tampon de aplicație tuturor celorlalte procese din comunicator. 
	Fiecare proces va fi trimis unui alt segment de date
	
	parametri:
	- sendbuf - adresa tamponului de trimitere
	- numărul de trimiteri - numărul de elemente trimise fiecărui proces
	- sendtype - tipul de date pentru trimiterea elementelor tampon
	- recvcount - numărul de elemente din bufferul de primire
	- recvtype - tipul de date al elementelor tampon de primire
	- rădăcină - rang al procesului de trimitere
	- comunicator de comunicare
	*/

	for (int i = 0; i < segmentSize; i++)
		found[i] = 0;

	index = 0;
	for (int i = 0; i < segmentSize; i++) {
		if (segment[i] == KEY) {
			found[index++] = i + rank * segmentSize;
		}
	}

	MPI_Gather(found, segmentSize, MPI_INT, finalFound, segmentSize, MPI_INT, 0, MPI_COMM_WORLD);
	/*
	Procesul rădăcină adună date din toate celelalte procese și îl plasează într-un buffer de primire comandat de către Expeditorului.
	parametri:
	- sendbuf - adresa de start a bufferului de trimitere
	- numărul de trimiteri - numărul de elemente din bufferul de trimitere
	- sendtype - tipul de date pentru trimiterea elementelor tampon
	- recvcount - numărul elementelor din bufferul de primire
	- recvtype - tipul de date al elementelor tampon de primire
	- rădăcină - rang al procesului de primire
	- comunicator de comunicare
	- recvbuf - adresa de pornire a tamponului de primire
	- cerere - cerere de comunicare
	*/

	if (rank == 0) {
		for (int i = 0; i < SIZE + 10; i++)
		if (finalFound[i] > 0)
			ok = true;
		if (ok) {
			printf("\n\nNumber found on the follwing position(s): ");
			for (int i = 0; i < SIZE + 10; i++)
			if (finalFound[i] != 0)
				printf("%d ", finalFound[i]);
		}


	}

	//Un program MPI se termină cu un apel către:
	MPI_Finalize();
	return 0;
}