#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// Always enter the -pthread compiler flag.
#include <pthread.h>


int x = 2;

void* routine()
{
    x++;
    sleep(2);
    printf("Process id %d\n", getpid());
    printf("Value of x: %d\n", x);
}

void* routine2()
{
    sleep(2);
    printf("Process id %d\n", getpid());
    printf("Value of x: %d\n", x);
}


int main(int argc, char* argv[])
{
    pthread_t t1;
    pthread_t t2;

    if (pthread_create(&t1, NULL, routine, NULL) != 0) {
        return EXIT_FAILURE;
    }
    if (pthread_create(&t2, NULL, routine2, NULL) != 0) {
        return EXIT_FAILURE;
    }


    if (pthread_join(t1, NULL) != 0) {
        return EXIT_FAILURE;
    }
    if (pthread_join(t2, NULL) != 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}