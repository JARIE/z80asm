// File: parse.h
// Author: Jarielle Catbagan
// Created: 17, May 2014

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

void extract_operands(FILE *file_handle, char *operand1, char *operand2,
                      line_status_t *line_status, uint8_t *n_operands);

uint8_t parse_operandtype(char *operand, symboltable_t **symboltable_list,
                          uint8_t symboltable_currentsize,
                          char ***symbolstracked_list,
                          uint8_t *symbolstracked_currentzie,
                          uint8_t *symbolstracked_actualsize); 

data_status_t testif_memlocvalid(char *operand);

data_status_t testif_numvalid(char *operand, uint8_t *byte_length);

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

#endif
