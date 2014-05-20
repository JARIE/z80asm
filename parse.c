// File: parse.c
// Author: Jarielle Catbagan
// Created: 17, May 2014

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "defines.h"

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

                if(c == ';') 
                        *line_status = COMMNTDELIM_DETECTED;
                else
                        *line_status = NO_COMMNTDELIM;

                program_status = PARSE_SOURCEFILE;
                break;
        case STOP_ACTION:
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

void handle_instruction(FILE *file_handle, char *buffer, line_status_t line_status) {
        char *instruction;
        char operand1[20], operand2[20], temp_buffer[20];
        uint8_t operand1_type, operand2_type;
        int c, index, num_of_operands = 0;
        typedef enum instruction_status_t {ERROR = 0, NO_ERROR} instruction_status_t;;


        instruction_status_t instruction_status = NO_ERROR;
        instruction = buffer;

        do {
                c = fgetc(file_handle);
        } while(c != EOF && c != '\r' && c != '\n' && (c == ' ' || c == '\t'));

        if(c == '\r' || c == ';') {
                do {
                        c = fgetc(file_handle);
                } while(c != EOF && c != '\n');
                operand1_type = NONE;
                operand2_type = NONE;
                num_of_operands = 0;
        }
        else if(c == EOF) {
                operand1_type = NONE;
                operand2_type = NONE;
                num_of_operands = 0;
        }
        else {
                index = 0;
                do {
                        temp_buffer[index++] = c;
                        c = fgetc(file_handle);
                } while(c != EOF && c != ';' && c != ' ' && c != '\n' && c != '\r' &&
                        c != '\t');
                temp_buffer[index] = '\0';

                index = strlen(temp_buffer) - 1;

                ++num_of_operands;

                if(temp_buffer[index] != ',' && c != ';') {
                        strcpy(operand1, temp_buffer);
                }
                else if(temp_buffer[index] != ',' && c == ';') {
                        strcpy(operand1, temp_buffer);
                        
                        do {
                                c = fgetc(file_handle);
                        } while(c != EOF && c != '\n');
                } 
                else if(temp_buffer[index] == ',' && c != ';') {
                        strncpy(operand1, temp_buffer, index);
                        operand1[index] = '\0';

                        do {
                                c = fgetc(file_handle);
                        } while(c != EOF && c != '\r' && c != '\n' && (c == ' ' ||
                                                                       c == '\t'));

                        if(c == '\r' || c == ';') {
                                do {
                                        c = fgetc(file_handle);
                                } while(c != EOF && c != '\n');
                                instruction_status = ERROR;
                        }
                        else if(c == EOF) {
                                instruction_status = ERROR;
                        }
                        else {
                                index = 0;
                                do {
                                        temp_buffer[index++] = c;
                                        c = fgetc(file_handle);
                                } while(c != EOF && c != ';' && c != ' ' && c != '\r' &&
                                        c != '\n' && c != '\t');

                                temp_buffer[index] = '\0';

                                strcpy(operand2, temp_buffer);
                                ++num_of_operands;

                                if(c == ';') {
                                        do {
                                                c = fgetc(file_handle);
                                        } while(c != EOF && c != '\n');
                                }
                        }
                }
                else if(temp_buffer[index] == ',' && c == ';') {
                        do {
                                c = fgetc(file_handle);
                        } while(c != EOF && c != '\n');
                        instruction_status = ERROR;
                }
        }

        

        if(instruction_status == NO_ERROR) {
                if(num_of_operands == 1)
                        printf("%s\n", operand1);
                else if(num_of_operands == 2)
                        printf("%s %s\n", operand1, operand2);
                printf("number of operands detected for \"%s\" is %d\n", instruction,
                       num_of_operands);
        }
        else {
                printf("error detected\n");
        }
}

