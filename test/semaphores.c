#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define THREAD_NUM 5

sem_t semaphore;


void* routine(void* args)
{
    // Checks the semaphores value
    // - if 0: the thread will wait on that semaphore
    // - if > 0: decrements and starts executing
    sem_wait(&semaphore);
    sleep(1);
    size_t id = (size_t)args;
    printf("Hello from thread %zu\n", id);

    // Increments the semaphores value
    sem_post(&semaphore);
    return args;
}

int main(int argc, char*argv[])
{
    pthread_t th[THREAD_NUM];

    sem_init(&semaphore, 0, 1);

    for (int i = 0; i < THREAD_NUM; ++i)
    {
        if (pthread_create(&th[i], NULL, routine, (void*)((size_t)i)) != 0) {
            perror("Failed to create thread");
        }
    }

    for (int i = 0; i < THREAD_NUM; ++i)
    {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semaphore);

    return EXIT_SUCCESS;
}