// File: z80asm.c
// Author: Jarielle Catbagan
// Created: 09, May 2014

// Description:

/* This file is the main source file for the Zilog Z80 Assembler (z80asm). */

// Code:

#include <stdio.h>
#include <stdlib.h>
#include "udgetopt.h"

#define STDERR(x) fprintf(stderr, "error: " x)
#define EFAILURE exit(EXIT_FAILURE);
#define ESUCCESS exit(EXIT_SUCCESS);


int main(int argc, char **argv) {
        FILE *sourcefile_handle;
        char *sourcefile_name;
        int c;
        enum flag_t {NOT_SET = 0, SET} s_flag, err_flag;

        s_flag = err_flag = NOT_SET;

        if(argc == 1) {
                STDERR("invalid number of arguments\n");
                EFAILURE;
        }

        while((c = udgetopt(argc, argv, "s:")) != -1) {
                printf("%c ", c);
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
        else {
                printf("the source file specified is %s\n", sourcefile_name);
        }

        if(optarg == NULL) {
                STDERR("no source file specified\n");
                EFAILURE;
        }

        

        ESUCCESS;
}
