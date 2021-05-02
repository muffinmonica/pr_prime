#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>

#define MAX 40'000'000
#define MIN 2
#define SIZE MAX - MIN
#define THREADS_NUM 8

// #define SHOW_INFO

long long primes[(int)(SIZE / 2)];
long long primes_count = 0;

int find_primes_naive_static(long long from, long long to) {
    if(to < 2 || to <= from) return 0;

    ++primes_count;
    primes[0] = 2;

    #pragma omp parallel for schedule(static)
    for(long long i = 3; i < to; i += 2) {
        bool is_prime = true;
        long long last_num = sqrt((double) i);
        for(long long j = 3; j <= last_num; j += 2) {
            if((i % j) == 0) {
                is_prime = false;
                break;
            }
        }
        #pragma omp critical
        {
            if(is_prime) {
                primes[primes_count++] = i;
            }
        }
    }
    return primes_count;
}

int main() {
    omp_set_num_threads(THREADS_NUM);
    printf("%d\n", find_primes_naive_static(MIN, MAX));

    #ifdef SHOW_INFO
        printf("%d primes between %d and %d\n", primes_count, MIN, MAX);
        printf("First up to 10 primes are:\n");
        for(int i = 0; i < 10 && i < primes_count; i++) {
            printf("%d\n", primes[i]);
        }
    #endif

    // for(int i = 0; i < primes_count; i++) {
    //     printf("%lld\n", primes[i]);
    // }
}