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
                        ++index;
                        ++(*symboltable_currentsize);
                }
        }
}

void free_symboltable(symboltable_t **symboltable_list) {
        free(*symboltable_list);
}
