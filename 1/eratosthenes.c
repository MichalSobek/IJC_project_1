   // eratosthenes.c
   // Řešení IJC-DU1, příklad a), 20.3.2026
   // Autor: Michal Sobek, FIT
   // Přeloženo: gcc 13.3.0
   // pomocou eratosthenesovho algoritmu upravuje pole bitov inicializovanych 
   // na 1 tak aby hodnotu 1 mali len prvocisla ako podla za

#include <stdio.h>
#include "bitarray.h"
#include <math.h>


/// @brief sets the bits of bitarray with non prime indexes to zero
/// @param arr ul[] that logically holds the bitarr, arr[0] must have the bitarr length
void eratosthenes(bitarray_t arr) {

    // returning the length of logical bitarray
    unsigned long arr_len = bitarray_size(arr) - 1;

    // 0, 1 are not prime numbers, setting to zero
    bitarray_setbit(arr, 0, 0);
    bitarray_setbit(arr, 1, 0);

    // we only need to iterate till sqrt(n)
    unsigned long opt_arr_len = sqrt(arr_len);

    // iterating through numbers 2....sqrt(n)
    for (unsigned long number = 2; number <= opt_arr_len; number++) {

        // we found a prime number :D, its products are not primes
        if (bitarray_getbit(arr, number) == 1) {

            // setting each product till arr_len to 0
            for (unsigned long product = number * number; product <= arr_len; product += number) {
                bitarray_setbit(arr, product, 0);
            }
        }
    }
}