// File: assemble.c
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

#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "assemble.h"

void assemble_instruction(FILE *infile_handle, FILE *outfile_handle,  char *instruction,
                          instruction_parameters_t **instruction_set,
                          symboltable_t *symboltable_list,
                          uint8_t symboltable_currentsize, line_status_t *line_status,
                          uint16_t *current_address, uint16_t *beginning_address,
                          uint16_t *previous_address) {
        char operand1[20], operand2[20];
        uint8_t operand1_type, operand2_type;
        uint8_t n_operands;
        uint8_t operand1_valuelength, operand2_valuelength,
                operand1_value[2], operand2_value[2];

        extract_operands(infile_handle, operand1, operand2, line_status, &n_operands);

        if(n_operands == 0) {
                operand1_type = NONE;
                operand1_valuelength = 0;
                operand1_value[0] = operand1_value[1] = 0;
                operand2_type = NONE;
                operand2_valuelength = 0;
                operand2_value[0] = operand2_value[1] = 0;
        }
        else if(n_operands == 1) {
                retrieve_opcharac(operand1, &operand1_type, &operand1_valuelength,
                                  operand1_value, symboltable_list,
                                  symboltable_currentsize);
                operand2_type = NONE;
                operand2_valuelength = 0;
                operand2_value[0] = operand2_value[1] = 0;
        }
        else {
                retrieve_opcharac(operand1, &operand1_type, &operand1_valuelength,
                                  operand1_value, symboltable_list,
                                  symboltable_currentsize);
                retrieve_opcharac(operand2, &operand2_type, &operand2_valuelength,
                                  operand2_value, symboltable_list,
                                  symboltable_currentsize);
        }

        assemble(outfile_handle, instruction_set, instruction, operand1_type,
                 operand2_type, operand1_valuelength, operand2_valuelength, operand1_value,
                 operand2_value, current_address, beginning_address, previous_address);
}


void retrieve_opcharac(char *operand, uint8_t *operand_type, uint8_t *operand_valuelength,
                       uint8_t operand_value[], symboltable_t *symboltable_list,
                       uint8_t symboltable_currentsize) {
        int index, i, boundary, index2;
        uint8_t value_toconvert[20];
        enum operand_status_t {UNKNOWN = 0, DETERMINED} operand_status;
        uint8_t byte_length;
        data_status_t data_status, memory_status, indexreg_status;

        operand_status = UNKNOWN;
        
        index = strlen(operand) - 1;

        if(!strcmp(operand, "(BC)") || !strcmp(operand, "(DE)") ||
           !strcmp(operand, "(HL)") || !strcmp(operand, "(SP)") ||
           !strcmp(operand, "(C)")) {
                if(!strcmp(operand, "(BC)"))
                        *operand_type = BC_REGISTER_MEMREF;
                else if(!strcmp(operand, "(DE)"))
                        *operand_type = DE_REGISTER_MEMREF;
                else if(!strcmp(operand, "(HL)"))
                        *operand_type = HL_REGISTER_MEMREF;
                else if(!strcmp(operand, "(SP)"))
                        *operand_type = SP_REGISTER_MEMREF;
                else
                        *operand_type = C_REGISTER_MEMREF;
                operand_status = DETERMINED;
        }
        
        if(operand[0] == '(' && operand[index] == ')' && operand_status == UNKNOWN) {
                if(operand[index - 1] == 'H' || operand[index - 1] == 'h')
                        boundary = index - 1;
                else
                        boundary = index;
                memory_status = VALID;
                for(index = 1; index < boundary; ++index) {
                        if((operand[index] < '0' || operand[index] > '9') &&
                           (operand[index] < 'A' || operand[index] > 'F') &&
                           (operand[index] < 'a' || operand[index] > 'f'))
                                memory_status = INVALID;
                }
                if(memory_status == VALID) {
                        *operand_type = MEMORY_16_BIT;
                        for(index = 1; index < (strlen(operand) - 1); ++index)
                                value_toconvert[index - 1] = operand[index];
                        value_toconvert[index - 1] = '\0';
                        *operand_valuelength = 2;
                        operand_value[0] = (uint8_t)
                                asciistr_to16bitnum(value_toconvert);
                        operand_value[1] = (uint8_t)
                                (asciistr_to16bitnum(value_toconvert) >> 8);
                        operand_status = DETERMINED;
                }
                else {
                        indexreg_status = testif_indexregwoffset(operand, operand_type);
                        if(indexreg_status == VALID) {
                                *operand_valuelength = 1;
                                index = 0;
                                while((operand[index] < '0' || operand[index] > '9') &&
                                      (operand[index] < 'A' || operand[index] > 'F') &&
                                      (operand[index] < 'a' || operand[index] > 'f'))
                                        ++index;
                                index2 = 0;
                                do {
                                        value_toconvert[index2++] = operand[index++];
                                } while(operand[index] != ')' && operand[index] != ' ' &&
                                        operand[index] != '\t');
                                value_toconvert[index2] = '\0';
                                operand_value[0] = (uint8_t)
                                        asciistr_to16bitnum(value_toconvert);
                                operand_status = DETERMINED;
                        }
                }
               
        }

        if(operand_status == UNKNOWN) {
                data_status = testif_numvalid(operand, &byte_length);

                if(data_status == VALID) {
                        if(byte_length == 1) {
                                *operand_type = VALUE_8_BIT;
                                *operand_valuelength = 1;
                                operand_value[0] = (uint8_t) asciistr_to16bitnum(operand);
                                operand_value[1] = 0;
                        }
                        else {
                                *operand_type = VALUE_16_BIT;
                                *operand_valuelength = 2;
                                operand_value[0] = (uint8_t) asciistr_to16bitnum(operand);
                                operand_value[1] = (uint8_t)
                                        (asciistr_to16bitnum(operand) >> 8);
                        }
                        operand_status = DETERMINED;
                }
                else
                        operand_status = UNKNOWN;
        }

        if(operand_status == UNKNOWN) {
                for(index = 0; index < symboltable_currentsize; ++index) {
                        if(!strcmp(operand, symboltable_list[index].name)) {
                                *operand_type = symboltable_list[index].value_type;
                                *operand_valuelength =
                                        symboltable_list[index].value_nbytes;
                                for(i = 0; i < *operand_valuelength; ++i) {
                                        operand_value[i] =
                                                symboltable_list[index].value[i];
                                }
                        }
                }
        }
}

void assemble(FILE *outputfile_handle, instruction_parameters_t **instruction_set,
              char *instruction,
              uint8_t operand1_type, uint8_t operand2_type, uint8_t operand1_valuelength,
              uint8_t operand2_valuelength, uint8_t operand1_value[],
              uint8_t operand2_value[], uint16_t *current_address,
              uint16_t *beginning_address, uint16_t *previous_address) {
        int index1, index2, i;
        loop_status_t loop_status = CONTINUE;
        uint8_t instruction_length;
        uint8_t bitmask, value_atinterest, value[4], lval, nshift;

        index1 = instruction[0] - 65;

        index2 = 0;
        while(instruction_set[index1][index2].instruction_name != NULL &&
              loop_status == CONTINUE) {
               if(!strcmp(instruction, instruction_set[index1][index2].instruction_name) &&
                  operand1_type == instruction_set[index1][index2].operand_type[0] &&
                  operand2_type == instruction_set[index1][index2].operand_type[1]) {
                       loop_status = EXIT;
               }
               else
                       ++index2;
        }

        instruction_length = instruction_set[index1][index2].instruction_length;

        for(i = 0; i < instruction_length; ++i) {
                value_atinterest = instruction_set[index1][index2].binary_code[i] & 0xC0;

                switch(value_atinterest) {
                case NONE_AFFECTED:
                        value[i] = instruction_set[index1][index2].instruction_value[i];
                        break;
                case OP1:
                        value_atinterest =
                                instruction_set[index1][index2].binary_code[i] & 0x30;
                        switch(value_atinterest) {
                        case ALLBITS:
                                value_atinterest = 0x08 &
                                        instruction_set[index1][index2].binary_code[i];
                                switch(value_atinterest) {
                                case _8BITVAL:
                                        value[i] = operand1_value[0];
                                        break;
                                case _16BITVAL:
                                        value_atinterest = 0x04 &
                                          instruction_set[index1][index2].binary_code[i];
                                        switch(value_atinterest) {
                                        case LBYTE:
                                                value[i] = operand1_value[0];
                                                break;
                                        case HBYTE:
                                                value[i] = operand1_value[1];
                                                break;
                                        }
                                        break;
                                }
                                break;
                        case _2BITS:
                                lval = operand1_value[0] & 0x03;
                                nshift = 0x07 &
                                        instruction_set[index1][index2].binary_code[i];
                                value[i] = (lval << nshift) |
                                    instruction_set[index1][index2].instruction_value[i];
                                break;
                        case _3BITS:
                                lval = operand1_value[0] & 0x07;
                                nshift = 0x07 &
                                        instruction_set[index1][index2].binary_code[i];
                                value[i] = (lval << nshift) |
                                    instruction_set[index1][index2].instruction_value[i];
                                break;
                        }
                        break;
                case OP2:
                        value_atinterest =
                                instruction_set[index1][index2].binary_code[i] & 0x30;
                        switch(value_atinterest) {
                        case ALLBITS:
                                value_atinterest = 0x08 &
                                        instruction_set[index1][index2].binary_code[i];
                                switch(value_atinterest) {
                                case _8BITVAL:
                                        value[i] = operand2_value[0];
                                        break;
                                case _16BITVAL:
                                        value_atinterest = 0x04 &
                                            instruction_set[index1][index2].binary_code[i];
                                        switch(value_atinterest) {
                                        case LBYTE:
                                                value[i] = operand2_value[0];
                                                break;
                                        case HBYTE:
                                                value[i] = operand2_value[1];
                                                break;
                                        }
                                        break;
                                }
                                break;
                        case _2BITS:
                                lval = operand2_value[0] & 0x03;
                                nshift = 0x07 &
                                        instruction_set[index1][index2].binary_code[i];
                                value[i] = (lval << nshift) |
                                        instruction_set[index1][index2].instruction_value[i];
                                break;
                        case _3BITS:
                                lval = operand2_value[0] & 0x07;
                                nshift = 0x07 &
                                        instruction_set[index1][index2].binary_code[i];
                                value[i] = (lval << nshift) |
                                      instruction_set[index1][index2].instruction_value[i];
                                break;
                        }
                        break;
                case BOTH_OPS:
                        value_atinterest =
                                instruction_set[index1][index2].binary_code[i] & 0x30;
                        switch(value_atinterest) {
                        case _6BITS:
                                value_atinterest = 0x08 &
                                        instruction_set[index1][index2].binary_code[i];
                                switch(value_atinterest) {
                                case OP1_OP2:
                                        lval = operand1_value[0] & 0x07;
                                        value[i] = (lval << 3) |
                                                instruction_set[index1][index2].instruction_value[i];
                                        lval = operand2_value[0] & 0x07;
                                        value[i] |= lval;
                                        break;
                                case OP2_OP1:
                                        lval = operand2_value[0] & 0x07;
                                        value[i] = (lval << 3) |
                                                instruction_set[index1][index2].instruction_value[i];
                                        lval = operand1_value[0] & 0x07;
                                        value[i] |= lval;
                                        break;
                                }
                                break;
                        }
                        break;
                }
        }

        output_tofile(outputfile_handle, instruction_length, value, current_address,
                      beginning_address, previous_address);
}

static fpos_t addressfield_filepos;
static fpos_t original_filepos;
static fpos_t bytecountfield_filepos;
static uint8_t nbytes_oncurrentline;

void output_tofile(FILE *outputfile_handle, uint8_t instruction_length, uint8_t value[],
                   uint16_t *current_address, uint16_t *beginning_address,
                   uint16_t *previous_address) {
        uint16_t threshold;
        uint8_t lval;
        int index;

        threshold = *beginning_address + 16;

        if(*current_address == *beginning_address) {
                fputc(':', outputfile_handle);
                fgetpos(outputfile_handle, &bytecountfield_filepos);
                fputs("  ", outputfile_handle);
                lval = (uint8_t) (*current_address >> 8);
                put8bitval_inhex(outputfile_handle, lval);
                lval = (uint8_t) *current_address;
                put8bitval_inhex(outputfile_handle, lval);
                fputs("00", outputfile_handle);
                nbytes_oncurrentline = 0;
        }

        for(index = 0; index < instruction_length; ++index) {
                if(*current_address >= *beginning_address &&
                   *current_address < threshold &&
                   (*previous_address == *current_address ||
                    *previous_address == (*current_address - 1))) {
                        put8bitval_inhex(outputfile_handle, value[index]);
                        *previous_address = *current_address;
                        ++(*current_address);
                        ++nbytes_oncurrentline;
                }
                else {
                        *beginning_address = *current_address;
                        threshold = *beginning_address + 16;
                        fgetpos(outputfile_handle, &original_filepos);
                        fsetpos(outputfile_handle, &bytecountfield_filepos);
                        put8bitval_inhex(outputfile_handle, nbytes_oncurrentline);
                        fsetpos(outputfile_handle, &original_filepos);
                        checksum(outputfile_handle);
                        fputs("\r\n:", outputfile_handle);
                        fgetpos(outputfile_handle, &bytecountfield_filepos);
                        fputs("  ", outputfile_handle);
                        lval = (uint8_t) (*current_address >> 8);
                        put8bitval_inhex(outputfile_handle, lval);
                        lval = (uint8_t) *current_address;
                        put8bitval_inhex(outputfile_handle, lval);
                        fputs("00", outputfile_handle);
                        nbytes_oncurrentline = 0;
                        put8bitval_inhex(outputfile_handle, value[index]);
                        *previous_address = *current_address;
                        ++(*current_address);
                        ++nbytes_oncurrentline;
                }
        }
}

void checksum(FILE *outputfile_handle) {
        int index;
        char c[3];
        uint8_t value = 0, checksum, temp;

        fsetpos(outputfile_handle, &bytecountfield_filepos);
        for(index = bytecountfield_filepos; index < original_filepos; ++index) {
                c[0] = fgetc(outputfile_handle);
                ++index;
                c[1] = fgetc(outputfile_handle);
                c[2] = '\0';
                value += hexstr_todecnum(c);
        }
        checksum = value ^ 0xFF;
        checksum += 1;
        fsetpos(outputfile_handle, &original_filepos);
        put8bitval_inhex(outputfile_handle, checksum);
}

void finish_outputhexfile(FILE *outputfile_handle) {
        fgetpos(outputfile_handle, &original_filepos);
        fsetpos(outputfile_handle, &bytecountfield_filepos);
        put8bitval_inhex(outputfile_handle, nbytes_oncurrentline);
        fsetpos(outputfile_handle, &original_filepos);
        checksum(outputfile_handle);
        fputs("\r\n:00000001FF", outputfile_handle);
}
