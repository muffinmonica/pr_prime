#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>
#include <string.h>
#include "range.h"

#define MAX 1000
#define MIN 10
#define SIZE MAX - MIN
#define THREADS_NUM 1

bool sieve_tab[(int) sqrt(MAX)] = {true};
long long primes[(int) (sqrt(MAX) / 2)];

bool sieve_in_range[MAX - MIN];
long long primes_in_range[MAX - MIN];

int count = 0;
int count_in_range = 0;

int find_primes_sieve_dynamic(long long from, long long to) {
    from = from >= 2 ? from : 2;
    int sqrt_max = (int) sqrt(to);

#pragma omp parallel
    {
#pragma omp for schedule(dynamic)
        for(int i = 0; i < sqrt_max - 1; i++) {
            if(!sieve_tab[i]) continue;
            int num = i + 2;
            for(int j = i + num; j < sqrt_max; j += num) {
                sieve_tab[j] = false;
            }
        }

#pragma omp single
        for(int i = 0; i < sqrt_max; i++) {
            if(sieve_tab[i]) {
                primes[count++] = i + 2;
                // printf("%d\n", primes[count - 1]);
            }
        }

#pragma omp for schedule(dynamic) // maybe unnecessary
        for(int i = 0; i < count; i++) {
            long long num = primes[i];
            long long start_from = from - from % num;
            if(start_from == from) {
                sieve_in_range[start_from - from] = false;
            }
            int current_num = 0;
            if(start_from == 0) {
                current_num = start_from + 2 * num; // cause if sqrt(max) > from then bug appears
            } else {
                current_num = start_from + num;
            }
            for(; current_num < to; current_num += num) {
                sieve_in_range[current_num - from] = false;
            }
        }

#pragma omp single
        for(int i = 0; i < to - from; i++) {
            if(sieve_in_range[i]) {
                primes_in_range[count_in_range++] = i + from;
                printf("%d\n", primes_in_range[count_in_range - 1]);
            }
        }
    }

    return 0;
}

int main() {
    memset(sieve_tab, true, sizeof(sieve_tab));
    memset(sieve_in_range, true, sizeof(sieve_in_range));
    find_primes_sieve_dynamic(MIN, MAX);
    printf("\n%d\n", count_in_range);
}

