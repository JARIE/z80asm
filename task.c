// File: task.c
// Author: Jarielle Catbagan
// Created: 18, May 2014

// Description: 

/* This file contains all the necessary initializations and clean-ups to be preformed. */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "defines.h"

void init_symboltable(symboltable_t **symboltable_list, symboltable_t *defined_symbols,
                      uint8_t *symboltable_currentsize, uint8_t *symboltable_actualsize) {
        int index = 0;
        *symboltable_list = malloc(25 * sizeof(**symboltable_list));

        *symboltable_currentsize = 0;
        *symboltable_actualsize = 25;

        if(*symboltable_list != NULL) {
                while(defined_symbols[index].name != NULL) {
                        (*symboltable_list)[index].name = defined_symbols[index].name;
                        (*symboltable_list)[index].operand_type =
                                defined_symbols[index].operand_type;
                        (*symboltable_list)[index].value_nbytes =
                                defined_symbols[index].value_nbytes;
                        (*symboltable_list)[index].value[0] =
                                defined_symbols[index].value[0];
                        (*symboltable_list)[index].value[1] =
                                defined_symbols[index].value[1];
                        (*symboltable_list)[index].value_status =
                                defined_symbols[index].value_status;
                        ++index;
                        ++(*symboltable_currentsize);
                }
        }
}

void free_symboltable(symboltable_t **symboltable_list) {
        free(*symboltable_list);
}

void goto_nextline(FILE *file_handle, line_status_t line_status) {
        int c;
        if(line_status == CARRIAGERETURN_DETECTED ||
           line_status == COMMNTDELIM_DETECTED) {
                do {
                        c = fgetc(file_handle);
                } while(c != EOF && c != '\n');
        }
}

program_status_t extract_nearestword(FILE *file_handle, char *buffer,
                                     unsigned char max_buffersize,
                                     line_status_t *line_status) {
        action_status_t action_status;
        program_status_t program_status;
        int c;
        unsigned char index;
        
        /* Look for the first non-whitespace character on the current line. If the first
           non-whitespace character is a semicolon, then the rest of the current line is
           part of a comment and the program should analyze immediately after the
           current line. This continues until something can be analyzed that is not part
           of a comment. When that happens, the following do-loop terminates and program
           execution continues thereafter. */
        action_status = LOOKFOR_NONWHITESPACE;

        do {
                do {
                        c = fgetc(file_handle);
                } while(c != EOF && (c == ' ' || c == '\r' || c == '\n' || c == '\t'));

                if(c == ';') {
                        do {
                                c = fgetc(file_handle);
                        } while(c != EOF && c != '\n');
                        if(c == EOF)
                                action_status = STOP_ACTION;
                        else
                                action_status = LOOKFOR_NONWHITESPACE;
                }
                else if(c == EOF)
                        action_status = STOP_ACTION;
                else
                        action_status = BEGIN_PARSING;
        } while(action_status == LOOKFOR_NONWHITESPACE);

        switch(action_status) {
        case BEGIN_PARSING:
                index = 0;
                do {
                        buffer[index++] = c;
                        c = fgetc(file_handle);
                } while(c != EOF && c != ';' && c != ' ' && c != '\n' && c != '\r' &&
                        c != '\t');
                buffer[index] = '\0';

                if(c == EOF)
                        *line_status = ENDOFFILE_DETECTED;
                else if(c == ';')
                        *line_status = COMMNTDELIM_DETECTED;
                else if(c == ' ' || c == '\t')
                        *line_status = NONE_DETECTED;
                else if(c == '\n')
                        *line_status = NEWLINE_DETECTED;
                else if(c == '\r')
                        *line_status = CARRIAGERETURN_DETECTED;
                program_status = PARSE_SOURCEFILE;
                break;
        case STOP_ACTION:
                *line_status = ENDOFFILE_DETECTED;
                program_status = ASSEMBLE_OUTPUTFILE;
                break;
        }
        
        return program_status;
}

word_type_t parse_wordtype(const char *buffer,
                           instruction_parameters_t **instruction_set) {
        word_type_t word_type = UNKNOWN;
        int index1, index2;


        if(buffer[(strlen(buffer)) - 1] == ':') {
                word_type = LABEL;
        }

        if(word_type == UNKNOWN) {
                if((!strcmp("ORG", buffer)) || (!strcmp("EQU", buffer)) ||
                   (!strcmp("END", buffer)))
                        word_type = DIRECTIVE;
        }
        

        if(word_type == UNKNOWN) {
                index1 = buffer[0] - 65;
                index2 = 0;
                if(instruction_set[index1] != NULL) {
                        while(instruction_set[index1][index2].instruction_name !=
                              NULL) {
                                if(!strcmp(instruction_set[index1][index2].instruction_name,
                                           buffer)) {
                                        word_type = INSTRUCTION;
                                }
                                ++index2;
                        }
                }
        }
        
        return word_type;
}
