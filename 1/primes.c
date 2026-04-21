   // primes.c
   // Řešení IJC-DU1, příklad a), 20.3.2026
   // Autor: Michal Sobek, FIT
   // Přeloženo: gcc 13.3.0
   // vypis 10 najvecsich prvocisel po 444 mil vzostupne aj s casom pomocov 
   // eratosthenosovho sita 

#include <stdio.h>
#include "bitarray.h"
#include "error.h"
#include <time.h>


void eratosthenes(bitarray_t arr);


/// @brief  main prints 10 greatest prime numbers till 444 mil from smallest 
///         to greatest into separate lines
///         main also printes the time of calculation
///         bit_array is saved on heap
int main() {

    // starting the timer
    clock_t start = clock();

    unsigned long N = 444000000;

    // allocating and filling the our array
    bitarray_alloc(arr, N+1);
    bitarray_fill(arr, 1);

    // setting the primes to 1 and nonprimes to 0 in arr
    eratosthenes(arr);
    
    // finding the primes
    bitarray_index_t result[10] = {0};
    int counter = 9;

    // populating our result arr with the greates primes in arr, from end to start
    for (bitarray_index_t idx = N; idx > 0 && counter >= 0; idx--) {
        if (bitarray_getbit(arr, idx) == 1) {
            result[counter] = idx;
            counter--;
        }
    }

    // printing the stored primes from smallest to grates
    for (int prime_idx = 0; prime_idx < 10; prime_idx++){
        printf("%lu\n", result[prime_idx]);
    }


    bitarray_free(arr);
    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);

    return 0;
}


#ifdef USE_INLINE

    extern inline void bitarray_free(bitarray_t name);
    extern inline unsigned long bitarray_size(bitarray_t name);
    extern inline void bitarray_fill(bitarray_t name, int val);
    extern inline void bitarray_setbit(bitarray_t name, bitarray_index_t index, int val);
    extern inline int bitarray_getbit(bitarray_t name, bitarray_index_t index);

#endif