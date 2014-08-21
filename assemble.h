// File: assemble.h
// Created: 24, May 2014

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
        
#ifndef ASSEMBLE_H
#define ASSEMBLE_H

void assemble_instruction(FILE *infile_handle, FILE *outfile_handle, char *instruction,
                          instruction_parameters_t **instruction_set,
                          symboltable_t *symboltable_list,
                          uint8_t symboltable_currentsize, line_status_t *line_status,
                          uint16_t *current_address, uint16_t *beginning_address,
                          uint16_t *previous_address);

void retrieve_opcharac(char *operand, uint8_t *operand_type, uint8_t *operand_valuelength,
                       uint8_t operand_value[], symboltable_t *symboltable_list,
                       uint8_t symboltable_currentsize);

void assemble(FILE *outputfile, instruction_parameters_t **instruction_set,
              char *instruction, uint8_t operand1_type, uint8_t operand2_type,
              uint8_t operand1_valuelength, uint8_t operand2_valuelength,
              uint8_t operand1_value[], uint8_t operand2_value[],
              uint16_t *current_address, uint16_t *beginning_address,
              uint16_t *previous_address);
 
void output_tofile(FILE *outputfile_handle, uint8_t instruction_length,
                   uint8_t value[], uint16_t *current_address,
                   uint16_t *beginning_address, uint16_t *previous_address);

void checksum(FILE *outputfile_handle);

void finish_outputhexfile(FILE *outputfile_handle);

#endif
