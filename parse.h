// File: parse.h
// Created: 17, May 2014

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

/* This file contains all the necessary functions to parse the source file specified
 * from the user.
 */

#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include "defines.h"

void parse_instruction(FILE *file_handle, char *buffer,
                       line_status_t *line_status,
                       instruction_parameters_t **instruction_set,
                       uint16_t *location_counter,
                       symboltable_t **symboltable_list,
                       uint8_t symboltable_currentsize,
                       char ***symbolstracked_list,
                       uint8_t *symbolstracked_currentsize,
                       uint8_t *symbolstracked_actualsize);



uint8_t parse_operandtype(char *operand, symboltable_t **symboltable_list,
                          uint8_t symboltable_currentsize,
                          char ***symbolstracked_list,
                          uint8_t *symbolstracked_currentzie,
                          uint8_t *symbolstracked_actualsize); 

data_status_t testif_memlocvalid(char *operand);



data_status_t testif_symbolexistent(char *operand, symboltable_t *symboltable_list,
                                    uint8_t symboltable_currentsize,
                                    uint8_t *operand_type);

data_status_t checkif_symbolworthy(char *operand);

data_status_t testif_instructionexistent(char *instruction,
                                         instruction_parameters_t **instruction_set,
                                         uint8_t operand1_type, uint8_t operand2_type,
                                         int *index1, int *index2);

void handle_label(char *label, symboltable_t **symboltable_list,
                  uint16_t location_counter, uint8_t *symboltable_currentsize,
                  uint8_t *symboltable_acutalsize);

void handle_directive(FILE *file_handle, char *directive, symboltable_t **symboltable_list,
                      uint16_t *location_counter, uint8_t *symboltable_currentsize,
                      uint8_t *symboltable_actualsize, line_status_t *line_status);

status_t extract_dirarg(FILE *file_handle, uint8_t extract_ndirargs,
                        line_status_t *line_status, char *dir_arg1, char *dir_arg2);

data_status_t parse_equvalue(char *value, uint8_t *type);

data_status_t track_symbol(char *symbol, char ***symbolstracked_list,
                           uint8_t *symbolstracked_currentsize,
                           uint8_t *symbolstracked_actualsize);

status_t validate_symbolstracked(char **symbolstracked_list,
                                 symboltable_t *symboltable_list,
                                 uint8_t symbolstracked_currentsize,
                                 uint8_t symboltable_currentsize);

void get_symbolparams(char *symbol, symboltable_t *symboltable_list,
                      uint8_t symboltable_currentsize,
                      uint8_t *byte_length, uint8_t value[]);

data_status_t testif_indexregwoffset(char *operand, uint8_t *operand_type);

#endif
