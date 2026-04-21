   // error.c
   // Řešení IJC-DU1, příklad b), 13.3.2026
   // Autor: Michal Sobek, FIT
   // Přeloženo: gcc 13.3.0
   // modul error.c definujuci pomocne funkcie pre: IJC-DU1, příklad b)

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"


/// @brief printing a formated warning
/// @param fmt formatted message for log
void warning(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // warning print to stderr
    fprintf(stderr, "Warning: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);
}


/// @brief printing and error message and terminating the program with 1
/// @param fmt formatted message for log
void error_exit(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // error print to stderr
    fprintf(stderr, "Error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);
    exit(1);
}