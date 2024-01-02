#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};


void* routine(void* arg)
{
    int index = (int)((size_t)arg);
    printf("%d: %d\n", index, primes[index]);
}

int main(int argc, char* argv[])
{
    pthread_t th[10];
    const size_t plen = sizeof(th) / sizeof(th[0]);

    for (int i = 0; i < plen; ++i) 
    {
        if (pthread_create(&th[i], NULL, routine, (void*)((size_t)i)) != 0) {
            perror("Failed to create thread");
        }
    }

    for (int i = 0; i < plen; ++i) 
    {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to create thread");
        }
    }


    return EXIT_SUCCESS;
}