// File: z80asm.c
// Author: Jarielle Catbagan
// Created: 09, May 2014

// Description:

/* This file is the main source file for the Zilog Z80 Assembler (z80asm). */

// Code:

#include <stdio.h>
#include <stdlib.h>
#include "udgetopt.h"

#define STDERR(str, ...) fprintf(stderr, "error: " str, ##__VA_ARGS__)
#define EFAILURE exit(EXIT_FAILURE);
#define ESUCCESS exit(EXIT_SUCCESS);
#define DEBUG(str, ...) fprintf(stdout, str, ##__VA_ARGS__)

int main(int argc, char **argv) {
        FILE *sourcefile_handle;
        char *sourcefile_name, buffer[20];
        int c;
        unsigned char index;
        enum flag_t {NOT_SET = 0, SET} s_flag, err_flag;
        enum loop_status_t {EXIT = 0, CONTINUE} loop_status;
        enum action_status_t {LOOKFOR_NONWHITESPACE = 0, BEGIN_PARSING} action_status;

        s_flag = err_flag = NOT_SET;

        if(argc == 1) {
                STDERR("invalid number of arguments\n");
                EFAILURE;
        }

        while((c = udgetopt(argc, argv, "s:")) != -1) {
                switch(c) {
                case 's':
                        sourcefile_name = optarg;
                        s_flag = SET;
                        break;
                case '?':
                        err_flag = SET;
                        break;
                }
        }


        if(err_flag == SET) {
                STDERR("an invalid option was specified\n");
                EFAILURE;
        }

        if(optarg == NULL) {
                STDERR("no source file specified\n");
                EFAILURE;
        }

        /* Open the source file specified on the command-line for reading only. This is
           the source file that will be parsed and converted into machine code for the
           Zilog Z80 CPU. */
        sourcefile_handle = fopen(sourcefile_name, "r");
        if(sourcefile_handle == NULL) {
                STDERR("the specified file (%s) could not be opened\n", sourcefile_name);
                EFAILURE;
        }

        /* Look for the first non-whitespace character on the current line. If the first
           non-whitespace character is a semicolon, then the rest of the current line is
           part of a comment and the program should analyze immediately after the
           current line. This continues until something can be analyzed that is not part
           of a comment. When that happens, the following do-loop terminates and program
           execution continues thereafter. */
        action_status = LOOKFOR_NONWHITESPACE;
        
        do {
                loop_status = CONTINUE;
                do {
                        c = fgetc(sourcefile_handle);
                        if(c != ' ')
                                loop_status = EXIT;
                } while(c != EOF && loop_status == CONTINUE);

                if(c == ';') {
                        loop_status = CONTINUE;
                        do {
                                c = fgetc(sourcefile_handle);
                                if(c == '\n') {
                                        loop_status = EXIT;
                                        action_status = LOOKFOR_NONWHITESPACE;
                                }
                        } while(c != EOF && loop_status == CONTINUE);
                }
                else
                        action_status = BEGIN_PARSING;
        } while(action_status == LOOKFOR_NONWHITESPACE);

        /* Once the first non-whitespace character has been found, extract all the
           characters from this position all the way until either a whitespace character
           or a semicolon is encountered, whichever occurs first. After, parse the
           characters extracted to determine the next course of action. */
        loop_status = CONTINUE;
        index = 0;
        
        do {
                buffer[index++] = c;
                c = fgetc(sourcefile_handle);
                if(c == ' ' || c == ';') {
                        buffer[index] = '\0';
                        loop_status = EXIT;
                }
        } while(c != EOF && loop_status == CONTINUE);
        
        DEBUG("the string obtained is %s\n", buffer);

        if((fclose(sourcefile_handle)) == EOF) {
                STDERR("the source file (%s) could not be closed\n", sourcefile_name);
                EFAILURE;
        }
        ESUCCESS;
}
