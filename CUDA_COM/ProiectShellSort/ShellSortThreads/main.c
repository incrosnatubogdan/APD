#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define size 10000 //numarul de elemente

int arr[size];   //vectorul pe care dorim sa il sortam
int incr = size/2;

void arr_initiate() { //aceasta functie initializeaza vectorul arr cu valori cuprinse intre 0 si 5000. Vectorul are size elemente
    int i;
    srand(time(NULL));
    for(i = 0; i < size; i++)
        arr[i] = rand() % 500000;

    printf("Original array\n");
    for(i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}




void shellsort()
{
    int j, k, tmp;

        for (j = incr; j < size; j++)
        {
            for(k = j - incr; k >= 0; k = k - incr)
            {
                if (arr[k+incr] >= arr[k])
                    break;
                else
                {
                    tmp = arr[k];
                    arr[k] = arr[k+incr];
                    arr[k+incr] = tmp;
                }
            }
        }
        pthread_exit(NULL);
}

int main()
{
    pthread_t thread[size];
    int i;
    arr_initiate();
    int threadCount=10;

    while(incr>0)
    {

        for(i = 0; i < threadCount; i++){
            if(incr>0){
                pthread_create(&thread[i], NULL, &shellsort, NULL);
                incr=incr/2;
            }
            else
                break;

            for(i = 0; i < threadCount; i++)
                pthread_join(thread[i], NULL);
        }


     }


    printf("Sorted array\n");
    for(i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");

    exit(0);
}
