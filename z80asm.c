// File: z80asm.c
// Author: Jarielle Catbagan
// Created: 09, May 2014

// Description:

/* This file is the main source file for the Zilog Z80 Assembler (z80asm). 
 */

#include <stdio.h>
#include <stdlib.h>
#include "udgetopt.h"
#include "defines.h"
#include "parse.h"
#include "z80instructionset.h"

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
        word_type_t type;
        uint16_t symboltable_currentsize = 0, symboltable_actualsize = 0;
        
        loop_status_t loop_status;
        action_status_t action_status;
        program_status_t program_status;

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

        init_symboltable(&symboltable_list, z80_symbols, &symboltable_currentsize,
                         &symboltable_actualsize);

        if(symboltable_list == NULL) {
                STDERR("the symbol table could not be created\n");
                EFAILURE;
        }

        for(index = 0; index < symboltable_currentsize; ++index) {
                DEBUG("the symbol is %s\n", symboltable_list[index].name);
        }


        program_status = PARSE_SOURCEFILE;

        while(extract_nearestword(sourcefile_handle, buffer, 20) == PARSE_SOURCEFILE) {

                type = parse_wordtype(buffer, instruction_set);
                DEBUG("the string is %s --- ", buffer);
                if(type == INSTRUCTION) 
                        DEBUG("its type is an instruction\n");
                else if(type == LABEL)
                        DEBUG("its type is a label\n");
                else if(type == DIRECTIVE)
                        DEBUG("its type is a directive\n");
                else
                        DEBUG("its type is unknown\n");


        }

        free_symboltable(&symboltable_list);
        

        if((fclose(sourcefile_handle)) == EOF) {
                STDERR("the source file (%s) could not be closed\n", sourcefile_name);
                EFAILURE;
        }
        ESUCCESS;
}
