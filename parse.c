// File: parse.c
// Author: Jarielle Catbagan
// Created: 17, May 2014

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "string.h"
#include "defines.h"
#include "parse.h"

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

status_t handle_instruction(FILE *file_handle, char *buffer,
                            line_status_t *line_status,
                            instruction_parameters_t **instruction_set,
                            uint16_t *location_counter, symboltable_t **symboltable_list,
                            uint8_t *symboltable_currentsize,
                            uint8_t *symboltable_actualsize) {
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
                        printf("no operands, %d, %d\n", operand1_type, operand2_type);
                }
                else if(n_operands == 1) {
                        printf("%s\n", operand1);
                        operand1_type = parse_operandtype(operand1, symboltable_list,
                                                          symboltable_currentsize,
                                                          symboltable_actualsize);
                        operand2_type = NONE;
                        printf("one operand, %d, %d\n", operand1_type, operand2_type); 
                }
                else { //n_operands == 2
                        printf("%s %s\n", operand1, operand2);
                        operand1_type = parse_operandtype(operand1, symboltable_list,
                                                          symboltable_currentsize,
                                                          symboltable_actualsize);
                        operand2_type = parse_operandtype(operand2, symboltable_list,
                                                          symboltable_currentsize,
                                                          symboltable_actualsize);
                        printf("two operands, %d, %d\n", operand1_type, operand2_type);
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
        
        return status;
}

void extract_operands(FILE *file_handle, char *operand1, char *operand2,
                      line_status_t *line_status, uint8_t *n_operands) {
        int c, index;
        char buffer[20];

        do {
                c = fgetc(file_handle);
        } while(c != EOF && (c == ' ' || c == '\t'));

        if(c == EOF || c == '\n' || c == '\r' || c == ';') {
                *n_operands = 0;
                
                if(c == EOF)
                        *line_status = ENDOFFILE_DETECTED;
                else if(c == '\n')
                        *line_status = NEWLINE_DETECTED;
                else if(c == '\r')
                        *line_status = CARRIAGERETURN_DETECTED;
                else
                        *line_status = COMMNTDELIM_DETECTED;
        }
        else {
                index = 0;
                do {
                        buffer[index++] = c;
                        c = fgetc(file_handle);
                } while(c != EOF && c != ' ' && c != '\t' && c != '\n' && c != '\r' &&
                        c != ';');
                buffer[index] = '\0';

                *n_operands = 1;

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

                strcpy(operand1, buffer);
        }

        index = strlen(buffer) - 1;

        if(*line_status == NONE_DETECTED && buffer[index] == ',') {
                operand1[index] = '\0';
                
                do {
                        c = fgetc(file_handle);
                } while(c != EOF && (c == ' ' || c == '\t'));

                if(c == EOF)
                        *line_status = ENDOFFILE_DETECTED;
                else if(c == '\n')
                        *line_status = NEWLINE_DETECTED;
                else if(c == '\r')
                        *line_status = CARRIAGERETURN_DETECTED;
                else if(c == ';')
                        *line_status = COMMNTDELIM_DETECTED;
                else {
                        index = 0;
                        do {
                                buffer[index++] = c;
                                c = fgetc(file_handle);
                        } while(c != EOF && c != ' ' && c != '\t' && c != '\n' &&
                                c != '\r' && c != ';');
                        buffer[index] = '\0';

                        ++(*n_operands);

                        strcpy(operand2, buffer);

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
}

uint8_t parse_operandtype(char *operand, symboltable_t **symboltable_list,
                          uint8_t *symboltable_currentsize,
                          uint8_t *symboltable_actualsize) {
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
                else
                        operand_type = INVALID_TYPE;
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
                                                    *symboltable_currentsize,
                                                    &operand_type);


                if(data_status == INVALID) {
                        /* This block is entered if the operand was not found in the
                           symbol table. In this case it is safe to assume that it is
                           a future reference symbol and that it is a 16-bit memory
                           location. */

                        /* Must check if the symbol adheres to the rules of containing
                           appropriate characters to be a valid symbol. */
                        data_status = checkif_symbolworthy(operand);

                        if(data_status == VALID) { //operand is symbol worthy
                                operand_type = MEMORY_16_BIT;
                                storein_symboltable(operand, operand_type,
                                                    symboltable_list,
                                                    symboltable_currentsize,
                                                    symboltable_actualsize);
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

        index = strlen(operand) - 2;
        if(operand[index] == 'H' || operand[index] == 'h') 
                boundary = strlen(operand) - 2;
        else
                boundary = strlen(operand) - 1;
        
        data_status = VALIDITY_UNKNOWN;

        for(index = 1; index < boundary; ++index)
                if(operand[index] < '0' || operand[index] > '9')
                        data_status = INVALID;

        if(data_status == VALIDITY_UNKNOWN)
                data_status = VALID;
        
        return data_status;
}

data_status_t testif_numvalid(char *operand, uint8_t *byte_length) {
        data_status_t data_status;
        int index, boundary;
        uint16_t value = 0;
        uint8_t exponent;

        index = strlen(operand) - 1;

        data_status = VALID;

        if(operand[index] == 'H' || operand[index] == 'h') {
                boundary = strlen(operand) - 1;
                for(index = 0; index < boundary; ++index)
                        if(operand[index] < '0' || operand[index] > '9')
                                data_status = INVALID;

                if(data_status != INVALID) {
                        index = boundary - 1;
                        boundary = 0;
                        exponent = 0;
                        for(; index >= boundary; --index) {
                                value += (operand[index] - 48) * pow(16, exponent);
                                ++exponent;
                        }

                        if(value < 256)
                                *byte_length = 1;
                        else
                                *byte_length = 2;

                        data_status = VALID;
                }
        }
        else {
                boundary = strlen(operand);
                for(index = 0; index < boundary; ++index)
                        if(operand[index] < '0' || operand[index] > '9')
                                data_status = INVALID;

                if(data_status != INVALID) {
                        index = boundary - 1;
                        boundary = 0;
                        exponent = 0;
                        for(; index >= boundary; --index) {
                                value += (operand[index] - 48) * pow(10, exponent);
                                ++exponent;
                        }

                        if(value < 256)
                                *byte_length = 1;
                        else
                                *byte_length = 2;

                        data_status = VALID;
                }
        }

        return data_status;
}

data_status_t testif_symbolexistent(char *operand, symboltable_t *symboltable_list,
                                    uint8_t symboltable_currentsize,
                                    uint8_t *operand_type) {
        int index;
        data_status_t data_status;

        data_status = VALIDITY_UNKNOWN;

        for(index = 0; index < symboltable_currentsize; ++index) {
                if(!strcmp(symboltable_list[index].name, operand)) {
                        *operand_type = symboltable_list[index].operand_type;
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

void storein_symboltable(char *operand, uint8_t operand_type,
                         symboltable_t **symboltable_list,
                         uint8_t *symboltable_currentsize,
                         uint8_t *symboltable_actualsize) {
        symboltable_t *symboltable_newlist;
        int index, size;


        /* Firsts tests whether there are no more room in the symbol table. If there is
           room to store a symbol then it is stored and the current size of the symbol
           table is incremented. If there is no more room, then the size of the symbol
           table is extended by a preset value to store the symbol as well as for future
           symbols. */
        if(*symboltable_currentsize == *symboltable_actualsize) {
                symboltable_newlist = realloc(*symboltable_list,
                                              (*symboltable_actualsize + 10) *
                                              sizeof(*symboltable_newlist));


                if(symboltable_newlist == NULL) {
                        STDERR("could not extend the symbol table\n");
                        EFAILURE;
                }
                else {
                        *symboltable_list = symboltable_newlist;
                        *symboltable_actualsize += 10;
                }
        }


        index = (*symboltable_currentsize)++;
        size = strlen(operand) + 1;
        (*symboltable_list)[index].name = malloc(size *
                                              sizeof(*((*symboltable_list)[index].name)));

        if((*symboltable_list)[index].name == NULL) {
                STDERR("storing symbol in the symbol table failed\n");
                EFAILURE;
        }
        strcpy((*symboltable_list)[index].name, operand);
        (*symboltable_list)[index].operand_type = operand_type;
        (*symboltable_list)[index].value_nbytes = 2;
        (*symboltable_list)[index].value_status = UNDEFINED;
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
