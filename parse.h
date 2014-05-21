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

program_status_t extract_nearestword(FILE *file_handle, char *buffer,
                                     unsigned char max_buffersize,
                                     line_status_t *line_status);

word_type_t parse_wordtype(const char *buffer, instruction_parameters_t **instruction_set);


status_t handle_instruction(FILE *file_handle, char *buffer,
                            line_status_t *line_status,
                            instruction_parameters_t **instruction_set,
                            uint16_t *location_counter,
                            symboltable_t *symboltable_list,
                            uint8_t *symboltable_currentsize);

void extract_operands(FILE *file_handle, char *operand1, char *operand2,
                      line_status_t *line_status, uint8_t *n_operands);

uint8_t parse_operandtype(char *operand, symboltable_t *symboltable_list,
                          uint8_t *symboltable_currentsize); 

data_status_t testif_memlocvalid(char *operand);

data_status_t testif_numvalid(char *operand, uint8_t *byte_length);

data_status_t testif_symbolexistent(char *operand, symboltable_t *symboltable_list,
                                    uint8_t symboltable_currentsize,
                                    uint8_t *operand_type);

data_status_t checkif_symbolworthy(char *operand);

void storein_symboltable(char *operand, uint8_t operand_type,
                         symboltable_t *symboltable_list,
                         uint8_t *symboltable_currentsize);

#endif
