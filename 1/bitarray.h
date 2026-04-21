   // bitearray.h
   // Řešení IJC-DU1, příklad a), 16.3.2026
   // Autor: Michal Sobek, FIT
   // Přeloženo: gcc 13.3.0
   // definice makier a inline funkcii pre zadanie a

#include <limits.h>
#include <stdlib.h>
#include <assert.h>  
#include <math.h>
#include "error.h"


typedef unsigned long bitarray_t[];
typedef unsigned long bitarray_index_t;


/// @brief returns the the bitcount of ul type on your system
#define ul_bits (sizeof(unsigned long) * 8)


/// @brief returns the minimal needed count of ul instances stored in array
#define ba_size(size) ((((size) + ul_bits - 1) / ul_bits) + 1)


/// @brief creating an bitearray on stack, the first item stores the bit count 
#define bitarray_create(name, size)                                 \
    unsigned long name[ba_size(size)] = {(size), 0};                \
    static_assert(((size) > 0 && (size) <= (ULONG_MAX - ul_bits)), "Velikost pole je mimo povoleny rozsah") 


/// @brief creating an bitearray on heap, the first item stores the bit count 
#define bitarray_alloc(name, size)                                              \
    assert((size) > 0 && (size) <= (ULONG_MAX - ul_bits));                                                         \
    unsigned long *name = calloc(ba_size((size)), sizeof(unsigned long));     \
    if ((name) == NULL) error_exit("bitarray_alloc: Chyba alokace paměti");     \
    (name)[0] = (size)               


#ifndef USE_INLINE                                        

/// @brief freeing the dynamically allocated bitearray
#define bitarray_free(name) free((name))


/// @brief returning the first ul, holding the bitearray size 
#define bitarray_size(name)  (name)[0]


/// @brief setting the whole bitearray (all cells) to 1 or 0
#define bitarray_fill(name, val)                                                                                    \
    do {                                                                                                            \
        for (unsigned long cell = 1; cell <= (((name)[0] + ul_bits - 1) / ul_bits); cell++ )                        \
            { (!val)? ((name)[cell] = 0ul) : ((name)[cell] = ~(0ul)); }                                             \
    } while (0)



#ifdef NO_CHECK

    #define bitarray_setbit(name, index, val)                                                                       \
    (val)                                                                                                           \
    ? ((name)[((index) / ul_bits + 1)] = (name)[((index) / ul_bits + 1)] | 1ul << (index % ul_bits ))               \
    : ((name)[((index) / ul_bits + 1)] = (name)[((index) / ul_bits + 1)] & ~(1ul << (index % ul_bits)))


    /// @brief returning the bit on the given index
    #define bitarray_getbit(name, index)        \
        (((name)[((index) / ul_bits + 1)] & (1ul << ( (index) % ul_bits))) != 0)   

#endif // NO_CHECK


#ifndef NO_CHECK


    /// @brief setting the bit on given index, doin a logical and (val=false) / or (val=true) with a mask of the given bit
    ///        ((index) / ul_bits + 1) returns the index of cell in which the desired bit resides
    ///        (index % ul_bits ) is the index of the bit in the cell
    #define bitarray_setbit(name, index, val)                                                                               \
        ((index) < (name)[0] )                                                                                              \
        ? ( (val)                                                                                                           \
            ? ((name)[((index) / ul_bits + 1)] = (name)[((index) / ul_bits + 1)] | 1ul << (index % ul_bits ))               \
            : ((name)[((index) / ul_bits + 1)] = (name)[((index) / ul_bits + 1)] & ~(1ul << (index % ul_bits))))            \
        : (error_exit("bitarray_setbit: Index %lu mimo rozsah 0..%lu", (unsigned long)(index), (unsigned long)(name)[0]), 0)  


    /// @brief returning the bit on the given index
    #define bitarray_getbit(name, index)                                                    \
        ((index)  < (name)[0] )                                                             \
        ? (((name)[((index) / ul_bits + 1)] & (1ul << ( (index) % ul_bits))) != 0)          \
        : (error_exit("bitarray_getbit: Index %lu mimo rozsah 0..%lu", (unsigned long)(index), (unsigned long)(name)[0]), 0)   


#endif // !NO_CHECK


#endif // !USE_INLINE



#ifdef USE_INLINE


/// @brief freeing the dynamically allocated bitearray
inline void bitarray_free(bitarray_t name) {
    free((name));
}


/// @brief returning the first ul, holding the bitearray size 
inline unsigned long bitarray_size(bitarray_t name) {
    return (name)[0];
}


/// @brief setting the whole bitearray (all cells) to 1 or 0
inline void bitarray_fill(bitarray_t name, int val) {                                                                               
    for (unsigned long cell = 1; cell <= ((name[0] + ul_bits - 1) / ul_bits); cell++ ) { 
        (!val)? (name[cell] = 0ul) : (name[cell] = ~(0ul)); 
    }
}



    #ifdef NO_CHECK

        inline void bitarray_setbit(bitarray_t name, bitarray_index_t index, int val) {
            (val)
                ? ((name)[((index) / ul_bits + 1)] = (name)[((index) / ul_bits + 1)] | 1ul << (index % ul_bits ))
                : ((name)[((index) / ul_bits + 1)] = (name)[((index) / ul_bits + 1)] & ~(1ul << (index % ul_bits)));
        }


        /// @brief returning the bit on the given index
        inline int bitarray_getbit(bitarray_t name, bitarray_index_t index) {
            return (name[((index) / ul_bits + 1)] & (1ul << ( (index) % ul_bits))) != 0; 
        }

    #endif // NO_CHECK


    #ifndef NO_CHECK

    /// @brief setting the bit on given index, doin a logical and (val=false) / or (val=true) with a mask of the given bit
    ///        ((index) / ul_bits + 1) returns the index of cell in which the desired bit resides
    ///        (index % ul_bits ) is the index of the bit in the cell
    inline void bitarray_setbit(bitarray_t name, bitarray_index_t index, int val) {
        if ((index) >= (name)[0] )  error_exit("bitarray_setbit: Index %lu mimo rozsah 0..%lu", (unsigned long)(index), (unsigned long)(name)[0]);
        (val)
            ? ((name)[((index) / ul_bits + 1)] = (name)[((index) / ul_bits + 1)] | 1ul << (index % ul_bits ))
            : ((name)[((index) / ul_bits + 1)] = (name)[((index) / ul_bits + 1)] & ~(1ul << (index % ul_bits)));
    }


    /// @brief returning the bit on the given index
    inline int bitarray_getbit(bitarray_t name, bitarray_index_t index) {
        if ((index) >= (name)[0] )  error_exit("bitarray_getbit: Index %lu mimo rozsah 0..%lu", (unsigned long)(index), (unsigned long)(name)[0]);
        return ((name)[((index) / ul_bits + 1)] & (1ul << ( (index) % ul_bits))) != 0;
    }

#endif // !NO_CHECK


#endif // USE_INLINE