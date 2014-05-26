// File: task.h
// Author: Jarielle Catbagan
// Created: 18, May 2014

#include <stdint.h>
#include "defines.h"

#ifndef TASK_H
#define TASK_H

void init_symboltable(symboltable_t **symboltable_list, symboltable_t *defined_symbols,
                      uint8_t *symboltable_currentsize, uint8_t *symboltable_actualsize);

void free_symboltable(symboltable_t **symboltable_list);

void goto_nextline(FILE *file_handle, line_status_t);

program_status_t extract_nearestword(FILE *file_handle, char *buffer,
                                     unsigned char max_buffersize,
                                     line_status_t *line_status);

word_type_t parse_wordtype(const char *buffer, instruction_parameters_t **instruction_set);

void storein_symboltable(char *entry, uint8_t entry_type, uint8_t entry_nbytes,
                         uint8_t entry_value[], symboltable_t **symboltable_list,
                         uint8_t *symboltable_currentsize,
                         uint8_t *symboltable_actualsize);

uint16_t asciistr_to16bitnum(char *buffer);

void extract_operands(FILE *file_handle, char *operand1, char *operand2,
                      line_status_t *line_status, uint8_t *n_operands);

data_status_t testif_numvalid(char *operand, uint8_t *byte_length);

void put8bitval_inhex(FILE *outputfile_handle, uint8_t value);

uint8_t hexstr_todecnum(char *hexliteral);

#endif
