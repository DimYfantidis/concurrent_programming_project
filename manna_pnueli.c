#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <inttypes.h>


uint64_t n;
uint64_t k;
volatile uint64_t b;

volatile uint64_t Y1;
volatile uint64_t Y2;

pthread_mutex_t criticalMutex;
pthread_mutex_t valueMutex;
pthread_cond_t valueCondition;


void* numerator_process(void* args)
{
    uint64_t t1;

    while (Y1 > n - k)
    {
        pthread_mutex_lock(&criticalMutex);
        t1 = b * Y1;
        b = t1;
        pthread_mutex_unlock(&criticalMutex);
        pthread_mutex_lock(&valueMutex);
        Y1--;
        pthread_mutex_unlock(&valueMutex);
        pthread_cond_signal(&valueCondition);
    }
    return NULL;
}

void* denominator_process(void* args)
{
    uint64_t t2;

    while (Y2 <= k)
    {
        pthread_mutex_lock(&valueMutex);
        while (Y1 + Y2 > n) {
            pthread_cond_wait(&valueCondition, &valueMutex);
        }
        pthread_mutex_unlock(&valueMutex);
        pthread_mutex_lock(&criticalMutex);
        t2 = b / Y2;
        b = t2;
        pthread_mutex_unlock(&criticalMutex);
        Y2++;
    }
    return NULL;
}

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

    if (argc < 3) 
    {
        fprintf(stderr, "Input Error: Missing input n and k\n");
        return EXIT_FAILURE;
    }
    n = parseUint64(argv[1]);
    k = parseUint64(argv[2]);

    if (!(0 <= k && k <= n)) 
    {
        fprintf(stderr, "Input Error: Illegal input - Should be 0 <= k <= n\n");
        return EXIT_FAILURE;
    }
    Y1 = n;
    Y2 = 1;
    b = 1;

    pthread_mutex_init(&criticalMutex, NULL);
    pthread_mutex_init(&valueMutex, NULL);
    pthread_cond_init(&valueCondition, NULL);

    printf("[COMP] > ");
    if (pthread_create(&p1, NULL, numerator_process, NULL) != 0) {
        perror("Failed to create numerator thread");
    }
    if (pthread_create(&p2, NULL, denominator_process, NULL) != 0) {
        perror("Failed to create denominator thread");
    }

    if (pthread_join(p1, NULL) != 0) {
        perror("Failed to join numerator thread");
    }
    if (pthread_join(p2, NULL) != 0) {
        perror("Failed to join denominator thread");
    }
    printf("bincoef(%"PRIu64", %"PRIu64") = %"PRIu64"\n", n, k, b);

    pthread_cond_destroy(&valueCondition);
    pthread_mutex_destroy(&valueMutex);
    pthread_mutex_destroy(&criticalMutex);

    return EXIT_SUCCESS;
}