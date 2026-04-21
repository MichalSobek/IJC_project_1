   // no-comment.c
   // Řešení IJC-DU1, příklad b), 13.3.2026
   // Autor: Michal Sobek, FIT
   // Přeloženo: gcc 13.3.0
   // no-comment, vracia vstupny subor bez komentarov definovanych stadartmi jazyka c11 a vyssie 
   // podla zadania IJC-DU1, příklad b)

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "error.h"



/// @brief return a valid file & validates input argument, count 
///        also prevents source file destruction
/// @param argc count of arguments in execution command
/// @param argv list of argumets in the execution command
FILE* get_valid_file(int argc, char* argv[]){

    FILE *file;

    // too little arguments printing to stdin
    if (argc == 1) file = stdin;

    // correct case, attempting to open the file
    else if (argc == 2 ) {
        file = fopen(argv[argc-1], "r");
        if (file == NULL) error_exit("Unable to open file");
    }    

    // too much argumetns, terminating program
    else error_exit("Too much arguments");

    struct stat stat_in, stat_out;
    
    // attemting to load input file detes into stat_in
    if (fstat(fileno(file), &stat_in) == -1) {
        error_exit("Error getting input file status");
    }
    // attemting to load output file detes into stat_in
    if (fstat(STDOUT_FILENO, &stat_out) == -1) {
        error_exit("Error getting output file status");
    }
    // comparing the index nodes and device (disk) ids
    if (stat_in.st_ino == stat_out.st_ino && stat_in.st_dev == stat_out.st_dev) {
        error_exit("Input and output files are the same");  // ino and dev match terminating program
    }

    return file;
}



/// @brief state machine to remove comments from the input file
///        no error handling implemented for file opening, make sure the input file is valid beforehand 
///        function handles string literals, inline comments and their continuation & regural comments
/// @param file input file to remove the comments from
void remove_comments(FILE* file){

    int state = 0;
    int character;

    // looping each character
    while ((character = fgetc(file)) != EOF) {

        switch (state) {

            // normal state, printing character to stdout, waiting for chars /,",'
            case 0: if (character == '/') state = 1;
                    else if (character == '"') {
                        state = 5; 
                        putchar('"');
                    }
                    else if (character == '\'') {
                        state = 6; 
                        putchar('\'');
                    }
                    else putchar(character);
                    
                    break;

            // we recieved an /, waiting for and /,*,<c> if <c> print /, <c> move to normal state
            case 1: if (character == '*') {
                        state = 2; 
                        break;
                    }
                    else if (character == '/') {
                        state = 4; 
                        break;
                    }
                    else state = 0;

                    putchar('/');
                    putchar(character);
                    break;

            // we are inside the comment
            case 2: if (character == '*') {
                        state = 3;  break;
                    }
                    break;
            
            // we possibly at then end of the comment
            case 3: if (character =='/') {
                        state = 0; 
                        putchar(' '); 
                        break;
                    }
                    else if (character == '*') {
                        state = 3; 
                        break;
                    }
                    else state = 2;

                    break;
            
            // we are in an inline comment 
            case 4: if (character =='\n') {
                        state = 0;
                        putchar('\n');
                    } else if (character == '\\') {
                        state = 9;}     // an /n is comming but we want to stay in case 4

                    break;

            // we are in a string
            case 5: if (character == '"') state = 0;
                    else if (character == '\\') state = 7;

                    putchar(character);
                    break;
            
            // we are in a char (or what is it called)
            case 6: if (character == '\'') state = 0;
                    else if (character == '\\') state = 8;

                    putchar(character);
                    break;
            
            // we read an \ so now we can print any character, after that returning to string literal
            case 7: putchar(character);
                    state = 5;
                    break;
            
            // we read an \ son now we can print any character, , after that returning to char
            case 8: putchar(character);
                    state = 6;
                    break;
            
            // we read an \ if there is a newline print it and go to the inline comment state
            case 9: if (character == '\n') putchar('\n');
                    else if (character == '\r') break;;
                    state = 4;
                    break;

        }
    }

    // string literal or comment was not ended
    if (state == 2 || state == 3 || state >= 5) error_exit("syntax eror detected in input file");

    // printing the / in case its the last character before EOF (isnt that a syntax errror?)
    if (state == 1) putchar('/');

    if (file != stdin) fclose(file);
}



/// @brief main function :P
/// @param argc count of arguments in execution command
/// @param argv list of argumets in the execution command
int main(int argc, char *argv[]) {

    // validating the input
    FILE* file = get_valid_file(argc, argv);

    // returning file without comments 
    remove_comments(file);

    return 0;
}