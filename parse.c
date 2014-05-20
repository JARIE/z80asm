// File: parse.c
// Author: Jarielle Catbagan
// Created: 17, May 2014

#include <stdio.h>
#include <stdlib.h>
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
                                   uint16_t *location_counter) {
        char *instruction, operand1[20], operand2[20];
        uint8_t operand1_type, operand2_type, n_operands;
        int c, index1, index2;
        status_t status;

        instruction = buffer;
        n_operands = 0;

        printf("%s\n", instruction);

        if(*line_status == ENDOFFILE_DETECTED || *line_status == COMMNTDELIM_DETECTED ||
           *line_status == NEWLINE_DETECTED || *line_status == CARRIAGERETURN_DETECTED) {
                index1 = instruction[0] - 65;
                index2 = 0;

                while(instruction_set[index1][index2].instruction_name != NULL) {
                        if(!strcmp(instruction_set[index1][index2].instruction_name,
                                   instruction) &&
                           (n_operands ==
                            instruction_set[index1][index2].n_operands)) {
                                *location_counter +=
                                        instruction_set[index1][index2].instruction_length;
                                status = NO_ERROR;
                        }
                        ++index2;
                }
                if(status != NO_ERROR)
                        status = ERROR;
        }
        else {
                extract_operands(file_handle, operand1, operand2, line_status,
                                 &n_operands);
                if(n_operands == 0) {
                        printf("no operands\n");
                        // operand1_type = NONE;
                        //operand2_type = NONE;
                }
                else if(n_operands == 1) {
                        printf("one operand\n");
                        //operand1_type = parse_operandtype(operand1);
                        //operand2_type = NONE;
                }
                else { //n_operands == 2
                        printf("two operands\n");
                        //operand1_type = parse_operandtype(operand1);
                        //operand2_type = parse_operandtype(operand2);
                }
                status = NO_ERROR;
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
                                buffer[index] = c;
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

/*
uint8_t parse_operandtype(char *buffer, symboltable_t *symboltable_list,
                          uint16_t *symboltable_currentsize) {
        
}
*/
