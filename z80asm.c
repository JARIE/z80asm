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

int main(int argc, char **argv) {
        FILE *sourcefile_handle;
        char *sourcefile_name, buffer[20];
        int c;
        unsigned char index;
        enum flag_t {NOT_SET = 0, SET} s_flag, err_flag;
        word_type_t type;
        uint8_t symboltable_currentsize = 0, symboltable_actualsize = 0;
        int16_t mainindex, subindex;
        
        line_status_t line_status;
        loop_status_t loop_status;
        action_status_t action_status;
        program_status_t program_status;
        status_t status;
        uint16_t location_counter = 0;

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

        program_status = PARSE_SOURCEFILE;

        while(extract_nearestword(sourcefile_handle, buffer, 20,
                                  &line_status) == PARSE_SOURCEFILE) {

                type = parse_wordtype(buffer, instruction_set);
                
                switch(type) {
                case INSTRUCTION:
                        status = handle_instruction(sourcefile_handle, buffer,
                                                    &line_status, instruction_set,
                                                    &location_counter, &symboltable_list,
                                                    &symboltable_currentsize,
                                                    &symboltable_actualsize);
                        printf("the location counter value is %d\n", location_counter);

                        break;
#ifdef j
                case LABEL:
                        handle_label();
                        break;
                case DIRECTIVE:
                        handle_directive();
                        break;
                case UNKNOWN:
                        break;

#endif
                }
                goto_nextline(sourcefile_handle, line_status);
        }

        free_symboltable(&symboltable_list);
        

        if((fclose(sourcefile_handle)) == EOF) {
                STDERR("the source file (%s) could not be closed\n", sourcefile_name);
                EFAILURE;
        }
        ESUCCESS;
}
