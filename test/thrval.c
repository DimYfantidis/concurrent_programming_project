#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void* roll_dice() 
{
    int value = (rand() % 6) + 1;
    int* result = (int*)malloc(sizeof(int));
    *result = value;
    printf("Thread result: %p\n", result);
    return (void *)result;
}

int main(int argc, char* argv[])
{
    int* result;
    pthread_t th;
    srand(time(NULL));

    if (pthread_create(&th, NULL, roll_dice, NULL) != 0) {
        return EXIT_FAILURE;
    }
    if (pthread_join(th, (void**)&result) > 0) {
        return EXIT_FAILURE;
    }
    printf("Main result: %p\n", result);
    printf("Result: %d\n", *result);

    free(result);
    return EXIT_SUCCESS;
}