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
                      uint16_t *symboltable_currentsize, uint16_t *symboltable_actualsize) {
        int index = 0;
        *symboltable_list = malloc(30 * sizeof(**symboltable_list));

        *symboltable_currentsize = 0;
        *symboltable_actualsize = 30;

        if(*symboltable_list != NULL) {
                while(defined_symbols[index].name != NULL) {
                        (*symboltable_list)[index].name = defined_symbols[index].name;
                        (*symboltable_list)[index].operand_type =
                                defined_symbols[index].operand_type;
                        (*symboltable_list)[index].value =
                                defined_symbols[index].value;
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
