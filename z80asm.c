// File: z80asm.c
// Created: 09, May 2014

/* Copyright (C) 2014 Jarielle Catbagan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Description:

/* This file is the main source file for the Zilog Z80 Assembler (z80asm). 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udgetopt.h"
#include "defines.h"
#include "parse.h"
#include "task.h"
#include "assemble.h"
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
        char **symbolstracked_list = NULL;
        uint8_t symbolstracked_currentsize = 0, symbolstracked_actualsize = 0;

        FILE *outputfile_handle;
        char *outputfile_name;
        char dir_arg[20];
        uint16_t current_address = 0, beginning_address = 0, previous_address = 0;
        enum address_status_t {NOT_INITIALIZED = 0, INITIALIZED} address_status;

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
        
        program_status = CONTINUE_PARSE;

        while(extract_nearestword(sourcefile_handle, buffer, 20,
                                  &line_status) == CONTINUE_PARSE) {

                type = parse_wordtype(buffer, instruction_set);
                
                switch(type) {
                case INSTRUCTION:
                        parse_instruction(sourcefile_handle, buffer,
                                          &line_status, instruction_set,
                                          &location_counter, &symboltable_list,
                                          symboltable_currentsize,
                                          &symbolstracked_list,
                                          &symbolstracked_currentsize,
                                          &symbolstracked_actualsize);

                        break;
                case LABEL:
                        handle_label(buffer, &symboltable_list,
                                     location_counter, &symboltable_currentsize,
                                     &symboltable_actualsize);
                        break;
                case DIRECTIVE:
                        handle_directive(sourcefile_handle, buffer, &symboltable_list,
                                         &location_counter, &symboltable_currentsize,
                                         &symboltable_actualsize, &line_status);
                        break;
                case UNKNOWN:
                        STDERR("invalid symbol encountered\n");
                        EFAILURE;
                        break;

                }
                goto_nextline(sourcefile_handle, line_status);
        }

        status = validate_symbolstracked(symbolstracked_list, symboltable_list,
                                         symbolstracked_currentsize,
                                         symboltable_currentsize);
        if(status == ERROR) {
                free_symboltable(&symboltable_list);
                free(symbolstracked_list);
                STDERR("an invalid symbol was found as an operand\n");
                EFAILURE;
        }

        rewind(sourcefile_handle);

        program_status = CONTINUE_PARSE;
        type = UNKNOWN;
        line_status = NONE_DETECTED;

        outputfile_name = malloc((strlen(sourcefile_name) + 3) *
                                 sizeof(*outputfile_name));
        if(outputfile_name == NULL) {
                free_symboltable(&symboltable_list);
                free(symbolstracked_list);
                STDERR("the output file can not be created\n");
                EFAILURE;
        }

        index = strlen(sourcefile_name) - 1;
        strncpy(outputfile_name, sourcefile_name, index);
        outputfile_name[index++] = 'h';
        outputfile_name[index++] = 'e';
        outputfile_name[index++] = 'x';
        outputfile_name[index] = '\0';

        outputfile_handle = fopen(outputfile_name, "w+");
        if(outputfile_handle == NULL) {
                free_symboltable(&symboltable_list);
                free(symbolstracked_list);
                STDERR("the output file created failed\n");
                EFAILURE;
        }

        address_status = NOT_INITIALIZED;
        
        while(extract_nearestword(sourcefile_handle, buffer, 20, &line_status) ==
              CONTINUE_PARSE) {
                type = parse_wordtype(buffer, instruction_set);

                if(type == DIRECTIVE) {
                        if(!strcmp("ORG", buffer)) {
                                status = extract_dirarg(sourcefile_handle, 1, &line_status,
                                                        dir_arg, NULL);
                                current_address = asciistr_to16bitnum(dir_arg);
                                if(address_status == NOT_INITIALIZED) {
                                        previous_address = current_address;
                                        beginning_address = current_address;
                                        address_status = INITIALIZED;
                                }
                        }
                }
                if(type == INSTRUCTION) {
                        assemble_instruction(sourcefile_handle, outputfile_handle,
                                             buffer, instruction_set,
                                             symboltable_list, symboltable_currentsize,
                                             &line_status, &current_address,
                                             &beginning_address, &previous_address);
                }

                goto_nextline(sourcefile_handle, line_status);
        }

        finish_outputhexfile(outputfile_handle);
 
        fclose(outputfile_handle);
        free(outputfile_name);
        free_symboltable(&symboltable_list);
        free(symbolstracked_list);
        

        if((fclose(sourcefile_handle)) == EOF) {
                STDERR("the source file (%s) could not be closed\n", sourcefile_name);
                EFAILURE;
        }
        ESUCCESS;
}
