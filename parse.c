// File: parse.c
// Author: Jarielle Catbagan
// Created: 17, May 2014

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "defines.h"
#include "parse.h"

void parse_instruction(FILE *file_handle, char *buffer,
                       line_status_t *line_status,
                       instruction_parameters_t **instruction_set,
                       uint16_t *location_counter, symboltable_t **symboltable_list,
                       uint8_t symboltable_currentsize,
                       char ***symbolstracked_list, uint8_t *symbolstracked_currentsize,
                       uint8_t *symbolstracked_actualsize) {
        char *instruction, operand1[20], operand2[20];
        uint8_t operand1_type = NONE, operand2_type = NONE, n_operands;
        int c, mainindex, subindex;
        status_t status;
        data_status_t data_status;

        instruction = buffer;
        n_operands = 0;

        if(*line_status == ENDOFFILE_DETECTED || *line_status == COMMNTDELIM_DETECTED ||
           *line_status == NEWLINE_DETECTED || *line_status == CARRIAGERETURN_DETECTED) {
                operand1_type = NONE;
                operand2_type = NONE;
        }
        else {
                extract_operands(file_handle, operand1, operand2, line_status,
                                 &n_operands);
                if(n_operands == 0) {
                        operand1_type = NONE;
                        operand2_type = NONE;
                }
                else if(n_operands == 1) {
                        operand1_type = parse_operandtype(operand1, symboltable_list,
                                                          symboltable_currentsize,
                                                          symbolstracked_list,
                                                          symbolstracked_currentsize,
                                                          symbolstracked_actualsize);
                        operand2_type = NONE; 
                }
                else { //n_operands == 2
                        operand1_type = parse_operandtype(operand1, symboltable_list,
                                                          symboltable_currentsize,
                                                          symbolstracked_list,
                                                          symbolstracked_currentsize,
                                                          symbolstracked_actualsize);
                        operand2_type = parse_operandtype(operand2, symboltable_list,
                                                          symboltable_currentsize,
                                                          symbolstracked_list,
                                                          symbolstracked_currentsize,
                                                          symbolstracked_actualsize);
                }
        }

        data_status = testif_instructionexistent(instruction, instruction_set,
                                                 operand1_type, operand2_type,
                                                 &mainindex, &subindex);

        if(data_status != VALID) {
                STDERR("invalid operands detected");
                EFAILURE;
        }
        else {
                *location_counter +=
                        instruction_set[mainindex][subindex].instruction_length;
        }
}



uint8_t parse_operandtype(char *operand, symboltable_t **symboltable_list,
                          uint8_t symboltable_currentsize,
                          char ***symbolstracked_list,
                          uint8_t *symbolstracked_currentsize,
                          uint8_t *symbolstracked_actualsize) {
        uint8_t operand_type;
        data_status_t data_status;
        uint8_t byte_length;
        int8_t index;

        operand_type = NONE;
        
        index = strlen(operand) - 1;
        if(operand[0] == '(' && operand[index] == ')') {
                data_status = testif_memlocvalid(operand);

                if(data_status == VALID)
                        operand_type = MEMORY_16_BIT;
                else {
                        data_status = testif_indexregwoffset(operand, &operand_type);

                        if(data_status == INVALID) 
                                operand_type = NONE;
                }
        }

        if(operand_type == NONE) {
                data_status = testif_numvalid(operand, &byte_length);

                if(data_status == VALID)
                        if(byte_length == 1)
                                operand_type = VALUE_8_BIT;
                        else
                                operand_type = VALUE_16_BIT;

        }

        if(operand_type == NONE) {
                data_status = testif_symbolexistent(operand, *symboltable_list,
                                                    symboltable_currentsize,
                                                    &operand_type);

                if(data_status == INVALID) {
                        data_status = checkif_symbolworthy(operand);

                        /* Must check if the symbol adheres to the rules of containing
                           appropriate characters to be a valid symbol. */                

                        if(data_status == VALID) { //operand is symbol worthy
                                operand_type = MEMORY_16_BIT;
                                data_status = track_symbol(operand, symbolstracked_list,
                                                           symbolstracked_currentsize,
                                                           symbolstracked_actualsize);
                                if(data_status == INVALID) {
                                        free(*symboltable_list);
                                        free(*symbolstracked_list);
                                        STDERR("symbols could not be tracked\n");
                                        EFAILURE;
                                }
                  
                        }
                        else {
                                operand_type = INVALID_TYPE;
                        }
                }
        }

        return operand_type;
}

data_status_t testif_memlocvalid(char* operand) {
        data_status_t data_status;
        int index, boundary;

        data_status = VALIDITY_UNKNOWN;
        
        index = strlen(operand) - 2;
        if(operand[index] == 'H' || operand[index] == 'h') {
                boundary = strlen(operand) - 2;
                
                for(index = 1; index < boundary; ++index)
                        if((operand[index] < '0' || operand[index] > '9') &&
                           (operand[index] < 'A' || operand[index] > 'F') &&
                           (operand[index] < 'a' || operand[index] > 'f'))
                                data_status = INVALID;
        }
        else {
                boundary = strlen(operand) - 1;

                for(index = 1; index < boundary; ++index)
                        if((operand[index] < '0' || operand[index] > '9'))
                                data_status = INVALID;
        }
        
        if(data_status == VALIDITY_UNKNOWN)
                data_status = VALID;
        
        return data_status;
}



data_status_t testif_symbolexistent(char *symbol, symboltable_t *symboltable_list,
                                    uint8_t symboltable_currentsize,
                                    uint8_t *type) {
        int index;
        data_status_t data_status;

        data_status = VALIDITY_UNKNOWN;

        for(index = 0; index < symboltable_currentsize; ++index) {
                if(!strcmp(symboltable_list[index].name, symbol)) {
                        *type = symboltable_list[index].value_type;
                        data_status = VALID;                        
                }
        }

        
        if(data_status == VALIDITY_UNKNOWN)
                data_status = INVALID;
        
        return data_status;
}

data_status_t checkif_symbolworthy(char *operand) {
        data_status_t data_status;
        int index, boundary;

        boundary = strlen(operand);

        data_status = VALIDITY_UNKNOWN;

        if((operand[0] < 'A' || operand[0] > 'Z') &&
           (operand[0] < 'a' || operand[0] > 'z') &&
           operand[0] != '_')
                data_status = INVALID;
        
        for(index = 1; index < boundary; ++index) {
                if((operand[index] < '0' || operand[index] > '9') &&
                   operand[index] != '_' &&
                   (operand[index] < 'A' || operand[index] > 'Z') &&
                   (operand[index] < 'a' || operand[index] > 'z'))
                        data_status = INVALID;
        }

        if(data_status != INVALID)
                data_status = VALID;

        return data_status;
}

data_status_t testif_instructionexistent(char *instruction,
                                         instruction_parameters_t **instruction_set,
                                         uint8_t operand1_type, uint8_t operand2_type,
                                         int *index1, int *index2) {
        int mainindex, subindex;
        loop_status_t loop_status;
        data_status_t data_status;

        loop_status = CONTINUE;
        mainindex = instruction[0] - 65;
        subindex = 0;

        data_status = VALIDITY_UNKNOWN;

        while(instruction_set[mainindex][subindex].instruction_name != NULL &&
              loop_status == CONTINUE) {
                if((!strcmp(instruction_set[mainindex][subindex].instruction_name,
                           instruction)) &&
                   instruction_set[mainindex][subindex].operand_type[0] == operand1_type &&
                   instruction_set[mainindex][subindex].operand_type[1] == operand2_type) {
                        data_status = VALID;
                        loop_status = EXIT;
                        *index1 = mainindex;
                        *index2 = subindex;
                }
                else {
                        ++subindex;
                }
        }

        if(data_status == VALIDITY_UNKNOWN)
                data_status = INVALID;

        return data_status;
}

void handle_label(char *label, symboltable_t **symboltable_list,
                  uint16_t location_counter, uint8_t *symboltable_currentsize,
                  uint8_t *symboltable_actualsize) {
        int index, mainindex;
        uint8_t value[2];

        label[(strlen(label) - 1)] = '\0';

        value[0] = (uint8_t) location_counter;
        value[1] = (uint8_t) (location_counter >> 8);

        storein_symboltable(label, MEMORY_16_BIT, 2, value, symboltable_list,
                            symboltable_currentsize, symboltable_actualsize);
}

void handle_directive(FILE *file_handle, char *directive, symboltable_t **symboltable_list,
                      uint16_t *location_counter, uint8_t *symboltable_currentsize,
                      uint8_t *symboltable_actualsize, line_status_t *line_status) {
        status_t status;
        char dir_arg1[20], dir_arg2[20];
        data_status_t data_status, symbol_status, value_status;
        uint8_t byte_length, value_type;
        char value_toconvert[20];
        uint8_t value[2];
        int index, boundary;
        uint8_t type;
        
        if(!strcmp("ORG", directive)) {
                status = extract_dirarg(file_handle, 1, line_status,
                                             dir_arg1, NULL);
                data_status = testif_numvalid(dir_arg1, &byte_length);
                if(data_status == VALID)
                        *location_counter = asciistr_to16bitnum(dir_arg1);
                else {
                        free(*symboltable_list);
                        STDERR("assigning invalid value to location counter\n");
                        EFAILURE;
                }
        }

        else if(!strcmp("EQU", directive)) {
                status = extract_dirarg(file_handle, 2, line_status, dir_arg1, dir_arg2);
                symbol_status = checkif_symbolworthy(dir_arg1);

                if(symbol_status != VALID) {
                        free(*symboltable_list);
                        STDERR("invalid EQU symbol\n");
                        EFAILURE;
                }

                value_status = parse_equvalue(dir_arg2, &type);
                if(value_status == VALID) {
                        if(type == MEMORY_16_BIT) {
                                index = 1;
                                boundary = strlen(dir_arg2) - 1;
                                for(; index < boundary; ++index)
                                        value_toconvert[index - 1] = dir_arg2[index];
                                value_toconvert[index - 1] = '\0';
                                value[0] = (uint8_t) asciistr_to16bitnum(value_toconvert);
                                value[1] = (uint8_t)
                                        (asciistr_to16bitnum(value_toconvert) >> 8);
                                byte_length = 2;
                        }
                        else if(type == VALUE_16_BIT) {
                                value[0] = (uint8_t) asciistr_to16bitnum(dir_arg2);
                                value[1] = (uint8_t)
                                        (asciistr_to16bitnum(dir_arg2) >> 8);
                                byte_length = 2;
                        }
                        else {
                                value[0] = (uint8_t) asciistr_to16bitnum(dir_arg2);
                                byte_length = 1;
                        }
                        storein_symboltable(dir_arg1, type, byte_length, value,
                                            symboltable_list, symboltable_currentsize,
                                            symboltable_actualsize);
                }
                else {
                        data_status = testif_symbolexistent(dir_arg2,
                                                            *symboltable_list,
                                                            *symboltable_currentsize,
                                                            &type);
                        
                        if(data_status == VALID) {
                                get_symbolparams(dir_arg2, *symboltable_list,
                                                 *symboltable_currentsize,
                                                 &byte_length, value);
                                storein_symboltable(dir_arg1, type, byte_length,
                                                    value, symboltable_list,
                                                    symboltable_currentsize,
                                                    symboltable_actualsize);
                        }
                        else {
                                STDERR("could not store symbol\n");
                        }
                }
        }
}

status_t extract_dirarg(FILE *file_handle, uint8_t extract_ndirargs,
                          line_status_t *line_status, char *dir_arg1, char *dir_arg2) {
        int index, c;
        char buffer[20];
        status_t status;
        uint8_t byte_length;

        do {
                c = fgetc(file_handle);
        } while(c != EOF && (c == ' ' || c == '\t'));

        if(c == EOF || c== '\n' || c== '\r' || c == ';') {
                status = ERROR;
        }
        else {
                index = 0;
                do {
                        buffer[index++] = c;
                        c = fgetc(file_handle);
                } while(c != EOF && c != ' ' && c != '\t' && c != '\n' && c != '\r' &&
                        c != ';');
                buffer[index] = '\0';

                status = NO_ERROR;
                strcpy(dir_arg1, buffer);

                if(c == EOF)
                        *line_status = ENDOFFILE_DETECTED;
                else if(c == '\n')
                        *line_status = NEWLINE_DETECTED;
                else if(c == '\r')
                        *line_status = CARRIAGERETURN_DETECTED;
                else if(c == ';')
                        *line_status = COMMNTDELIM_DETECTED;
                else
                        *line_status = NONE_DETECTED;
        }

        if(status == NO_ERROR) {
                if(extract_ndirargs == 2) {
                        do {
                                c = fgetc(file_handle);
                        } while(c != EOF && (c == ' ' || c == '\t'));

                        if(c == EOF || c == '\n' || c == '\r' || c == ';') {
                                status = ERROR;
                        }
                        else {
                                index = 0;
                                do {
                                        buffer[index++] = c;
                                        c = fgetc(file_handle);
                                } while(c != EOF && c != ' ' && c != '\t' && c != '\n' &&
                                        c != '\r' && c != ';');
                                buffer[index] = '\0';

                                status = NO_ERROR;
                                strcpy(dir_arg2, buffer);

                                if(c == EOF)
                                        *line_status = ENDOFFILE_DETECTED;
                                else if(c == '\n')
                                        *line_status = NEWLINE_DETECTED;
                                else if(c == '\r')
                                        *line_status = CARRIAGERETURN_DETECTED;
                                else if(c == ';')
                                        *line_status = COMMNTDELIM_DETECTED;
                                else
                                        *line_status = NONE_DETECTED;
                        }
                }
                else {
                        status = NO_ERROR;
                }
        }
        return status;
}

data_status_t parse_equvalue(char *value, uint8_t *type) {
        int index;
        data_status_t value_status;
        uint8_t byte_length;

        index = strlen(value) - 1;

        value_status = VALIDITY_UNKNOWN;

        if(value[0] == '(' && value[index] == ')') {
                value_status = testif_memlocvalid(value);

                if(value_status == VALID) 
                        *type = MEMORY_16_BIT;
        }

        if(value_status != VALID) {
                value_status = testif_numvalid(value, &byte_length);

                if(value_status == VALID) 
                        if(byte_length == 1)
                                *type = VALUE_8_BIT;
                        else
                                *type = VALUE_16_BIT;
        }

        return value_status;
}

data_status_t track_symbol(char *symbol, char ***symbolstracked_list,
                           uint8_t *symbolstracked_currentsize,
                           uint8_t *symbolstracked_actualsize) {
        int index, size;
        enum symbol_status_t {NOT_FOUND = 0, FOUND} symbol_status;
        char **symbolstracked_newlist;
        data_status_t data_status;

        symbol_status = NOT_FOUND;
        
        for(index = 0; index < *symbolstracked_currentsize; ++index) {
                if(!strcmp((*symbolstracked_list)[index], symbol))
                        symbol_status = FOUND;
        }

        if(symbol_status == NOT_FOUND) {
                if(*symbolstracked_currentsize == *symbolstracked_actualsize) {
                        symbolstracked_newlist = realloc(*symbolstracked_list,
                                (*symbolstracked_actualsize + 10) *
                                                       sizeof(*symbolstracked_newlist));
                        if(symbolstracked_newlist == NULL)
                                data_status = INVALID;
                        else {
                                *symbolstracked_list = symbolstracked_newlist;
                                *symbolstracked_actualsize += 10;
                        }
                }

                index = (*symbolstracked_currentsize)++;

                size = strlen(symbol) + 1;

                (*symbolstracked_list)[index] = malloc(size *
                                               sizeof(*((*symbolstracked_list)[index])));

                if((*symbolstracked_list)[index] == NULL)
                        data_status = INVALID;
                else {
                        strcpy((*symbolstracked_list)[index], symbol);
                }
        }
        return data_status;
}

status_t validate_symbolstracked(char **symbolstracked_list,
                                 symboltable_t *symboltable_list,
                                 uint8_t symbolstracked_currentsize,
                                 uint8_t symboltable_currentsize) {
        int index1, index2;
        status_t status;
        data_status_t data_status;

        status = NO_ERROR;
        
        for(index1 = 0; index1 < symbolstracked_currentsize; ++index1) {
                data_status = INVALID;
                for(index2 = 0; index2 < symboltable_currentsize; ++index2) {
                        if(!strcmp(symboltable_list[index2].name,
                                   symbolstracked_list[index1]))
                                data_status = VALID;
                }
                if(data_status != VALID)
                        status = ERROR;
        }
        return status;
}

void get_symbolparams(char *symbol, symboltable_t *symboltable_list,
                      uint8_t symboltable_currentsize,
                      uint8_t *byte_length, uint8_t value[]) {
        int index1, index2;

        for(index1 = 0; index1 < symboltable_currentsize; ++index1) {
                if(!strcmp(symbol, symboltable_list[index1].name)) {
                        *byte_length = symboltable_list[index1].value_nbytes;
                        for(index2 = 0; index2 < *byte_length; ++ index2)
                                value[index2] = symboltable_list[index1].value[index2];
                }
        }
}


data_status_t testif_indexregwoffset(char *operand, uint8_t *operand_type) {
        int index, boundary, index2;
        char buffer[10];
        enum indexreg_type_t {NO_INDEXREG = 0, IX, IY} indexreg_type;
        data_status_t data_status;

        index = 1;
        boundary = strlen(operand) - 1;
        index2 = 0;

        while(operand[index] >= 'A' && operand[index] <= 'Z') {
                buffer[index2] = operand[index];
                ++index;
                ++index2;
        }
        buffer[index2] = '\0';

        if(!strcmp(buffer, "IX"))
                indexreg_type = IX;
        else if(!strcmp(buffer, "IY"))
                indexreg_type = IY;
        else indexreg_type = NO_INDEXREG;

        if(indexreg_type != NO_INDEXREG) {
                while(operand[index] == ' ' || operand[index] == '\t')
                        ++index;
                if(operand[index] == '+') {
                        ++index;
                        while(operand[index] == ' ' || operand[index] == '\t')
                                ++index;
                        index2 = 0;

                        while(operand[index] != ')') {
                                buffer[index2] = operand[index];
                                ++index2;
                                ++index;
                        }

                        data_status = VALID;

                        if(buffer[index2 - 1] == 'H' || buffer[index2 - 1] == 'h')
                                index2 = index2 - 1;
                        
                        for(index = 0; index < index2; ++index) {
                                if((buffer[index] < '0' || buffer[index] > '9') &&
                                   (buffer[index] < 'A' || buffer[index] > 'F') &&
                                   (buffer[index] < 'a' || buffer[index] > 'f'))
                                        data_status = INVALID;
                        }

                        if(data_status == VALID) {
                                if(indexreg_type == IX)
                                        *operand_type = IX_REGISTER_WOFFSET;
                                else {
                                        *operand_type = IY_REGISTER_WOFFSET;
                                }
                                
                        }
                }
                else
                        data_status = INVALID;
                
        }
        else
                data_status = INVALID;

        return data_status;
}
