#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <inttypes.h>
#include <semaphore.h>


// Program's inputs
uint64_t n;
uint64_t k;

// Program's output, holds partial results computed by p1 and p2
volatile uint64_t b;

// Holds the current term of the numerator's product
volatile uint64_t Y1;
// Holds the current term of the denominator's product
volatile uint64_t Y2;

// Mutex for the critical section (mentioned as "r" in the report)
pthread_mutex_t criticalMutex;
// Condition variable for p2's "await" in line m1
pthread_cond_t valueCondition;
// Auxilliary mutex for for the condition variable
pthread_mutex_t valueMutex;
// Semaphore for the critical section
sem_t criticalSemaphore;


// Process p1
void* numerator_process(void* args)
{
    uint64_t t1;

    while (Y1 > n - k)
    {
        sem_wait(&criticalSemaphore);
        pthread_mutex_lock(&criticalMutex);
        t1 = b * Y1;
        b = t1;
        // printf("p1: b=%"PRIu64"\n", b);
        pthread_mutex_unlock(&criticalMutex);
        pthread_mutex_lock(&valueMutex);
        Y1--;
        pthread_mutex_unlock(&valueMutex);
        pthread_cond_signal(&valueCondition);
    }
    return NULL;
}

// Process p2
void* denominator_process(void* args)
{
    uint64_t t2;

    while (Y2 <= k)
    {
        pthread_mutex_lock(&valueMutex);
        while (Y1 + Y2 > n) {
            sem_post(&criticalSemaphore);
            // Await for p1 to signal that it performed a change in Y1's value
            pthread_cond_wait(&valueCondition, &valueMutex);
        }
        pthread_mutex_unlock(&valueMutex);
        pthread_mutex_lock(&criticalMutex);
        t2 = b / Y2;
        b = t2;
        // printf("p2: b=%"PRIu64"\n", b);
        pthread_mutex_unlock(&criticalMutex);
        Y2++;
    }
    return NULL;
}

// Parse a 64-bit unsigned int from its string form (in decimal)
uint64_t parseUint64(const char* S)
{
    char* end;
    uint64_t result = strtoull(S, &end, 10);

    if (*end != '\0') {
        fprintf(stderr, "Parse Error: Insufficient parsing of \"%s\" - Leftover: \"%s\"\n", S, end);
    }
    return result;
}

int main(int argc, char* argv[]) 
{
    pthread_t p1;
    pthread_t p2;

    // Input control (1)
    if (argc < 3) 
    {
        fprintf(stderr, "Input Error: Missing input n and k\n");
        return EXIT_FAILURE;
    }
    n = parseUint64(argv[1]);
    k = parseUint64(argv[2]);

    // Input control (2)
    if (!(0 <= k && k <= n)) 
    {
        fprintf(stderr, "Input Error: Illegal input - Should be 0 <= k <= n\n");
        return EXIT_FAILURE;
    }
    // Initialization as mentioned in report
    Y1 = n;
    Y2 = 1;
    b = 1;

    // Initialization of pthread entities
    pthread_mutex_init(&criticalMutex, NULL);
    pthread_mutex_init(&valueMutex, NULL);
    pthread_cond_init(&valueCondition, NULL);
    sem_init(&criticalSemaphore, 0, 0);

    printf("[COMP] > ");
    if (pthread_create(&p2, NULL, denominator_process, NULL) != 0) {
        perror("Failed to create denominator thread");
    }
    if (pthread_create(&p1, NULL, numerator_process, NULL) != 0) {
        perror("Failed to create numerator thread");
    }

    if (pthread_join(p1, NULL) != 0) {
        perror("Failed to join numerator thread");
    }
    if (pthread_join(p2, NULL) != 0) {
        perror("Failed to join denominator thread");
    }
    // Print result
    printf("bincoef(%"PRIu64", %"PRIu64") = %"PRIu64"\n", n, k, b);

    // Deallocation of pthread entities
    sem_destroy(&criticalSemaphore);
    pthread_cond_destroy(&valueCondition);
    pthread_mutex_destroy(&valueMutex);
    pthread_mutex_destroy(&criticalMutex);

    return EXIT_SUCCESS;
}