#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <inttypes.h>


void* compute_numerator(void* args)
{
    mpz_ptr n = ((mpz_ptr*)args)[0];
    mpz_ptr k = ((mpz_ptr*)args)[1];

    mpz_t x;
    mpz_ptr prod = (mpz_ptr)malloc(sizeof(MP_INT));

    mpz_init_set_ui(prod, 1);

    mpz_init_set(x, n);
    mpz_sub(x, x, k);
    mpz_add_ui(x, x, 1);

    for (/*x initialized*/; mpz_cmp(x, n) <= 0; mpz_add_ui(x, x, 1)) {
        mpz_mul(prod, prod, x);
    }
    return (void *)prod;
}

void* compute_denominator(void* args)
{
    mpz_ptr n = ((mpz_ptr*)args)[0];
    mpz_ptr k = ((mpz_ptr*)args)[1];

    mpz_t x;
    mpz_ptr prod = (mpz_ptr)malloc(sizeof(MP_INT));

    mpz_init_set_ui(prod, 1);

    for (mpz_init_set_ui(x, 1); mpz_cmp(x, k) <= 0; mpz_add_ui(x, x, 1)) {
        mpz_mul(prod, prod, x);
    }
    return (void *)prod;
}


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Missing input: insert n and k\n");
        return EXIT_FAILURE;
    }
    mpz_t n;
    mpz_t k;

    mpz_init_set_str(n, argv[1], 10);
    mpz_init_set_str(k, argv[2], 10);

    if (mpz_cmp(k, n) > 0 || mpz_cmp_ui(n, 0) < 0 || mpz_cmp_ui(k, 0) < 0) 
    {
        fprintf(stderr, "Wrong input: n, k should be 0 <= k <= n\n");
        mpz_clear(n);
        mpz_clear(k);
        return EXIT_FAILURE;
    }
    
    mpz_ptr inputs[2] = {&n[0], &k[0]};
    mpz_ptr numerator;
    mpz_ptr denominator;

    pthread_t th_numerator;
    pthread_t th_denominator;

    printf("[REAL] > ");
    if (pthread_create(&th_numerator, NULL, compute_numerator, (void*)inputs) != 0) {
        perror("Failed to create thread for numerator\n");
    }
    if (pthread_create(&th_denominator, NULL, compute_denominator, (void*)inputs) != 0) {
        perror("Failed to create thread for denominator\n");
    }

    if (pthread_join(th_numerator, (void **)&numerator) != 0) {
        perror("Failed to join thread for numerator\n");
    }
    if (pthread_join(th_denominator, (void**)&denominator) != 0) {
        perror("Failed to join thread for denominator\n");
    }

    mpz_t bincoef;
    mpz_init(bincoef);
    mpz_div(bincoef, numerator, denominator);

    size_t bc_n_digits = mpz_sizeinbase(bincoef, 2);
    size_t num_bits = mpz_sizeinbase(numerator, 2);
    size_t dnm_bits = mpz_sizeinbase(denominator, 2);
    
    gmp_printf("bincoef(%Zd, %Zd) = %Zd, bits: %"PRIu64", ", n, k, bincoef, bc_n_digits);
    printf("NUM bits: %"PRIu64", ", num_bits);
    printf("DNM bits: %"PRIu64"\n", dnm_bits);

    mpz_clear(n);
    mpz_clear(k);
    mpz_clear(bincoef);
    mpz_clear(numerator);
    mpz_clear(denominator);

    free(numerator);
    free(denominator);

    return EXIT_SUCCESS;
}