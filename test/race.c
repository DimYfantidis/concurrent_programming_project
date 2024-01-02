#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


long long mails = 0;

pthread_mutex_t mutex;


void* routine() 
{
    for (int i = 0; i < 11111111; ++i) 
    {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[])
{
    pthread_t p[7];
    const size_t plen = sizeof(p) / sizeof(p[0]);

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < plen; ++i)
    {
        if (pthread_create(&p[i], NULL, routine, NULL) != 0) {
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < plen; ++i)
    {
        if (pthread_join(p[i], NULL) != 0) {
            return EXIT_FAILURE;
        }
    }
    pthread_mutex_destroy(&mutex);
    printf("Number of mails: %lld\n", mails);

    return EXIT_SUCCESS;
}