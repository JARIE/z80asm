// File: task.c
// Author: Jarielle Catbagan
// Created: 18, May 2014

// Description: 

/* This file contains all the necessary initializations and clean-ups to be preformed. */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "defines.h"

void init_symboltable(symboltable_t **symboltable_list, symboltable_t *defined_symbols,
                     uint8_t *symboltable_currentsize, uint8_t *symboltable_actualsize) {
        int index = 0, size = 0;
        
        while(defined_symbols[index].name != NULL) {
                ++size;
                ++index;
        }

        index = 0;
        *symboltable_list = malloc(size * sizeof(**symboltable_list));

        *symboltable_currentsize = 0;
        *symboltable_actualsize = size;

        if(*symboltable_list != NULL) {
                while(defined_symbols[index].name != NULL) {
                        (*symboltable_list)[index].name = defined_symbols[index].name;
                        (*symboltable_list)[index].value_type =
                                defined_symbols[index].value_type;
                        (*symboltable_list)[index].value_nbytes =
                                defined_symbols[index].value_nbytes;
                        (*symboltable_list)[index].value[0] =
                                defined_symbols[index].value[0];
                        (*symboltable_list)[index].value[1] =
                                defined_symbols[index].value[1];
                        (*symboltable_list)[index].value_status =
                                defined_symbols[index].value_status;
                        ++index;
                        ++(*symboltable_currentsize);
                }
        }
}

void free_symboltable(symboltable_t **symboltable_list) {
        free(*symboltable_list);
}

void goto_nextline(FILE *file_handle, line_status_t line_status) {
        int c;
        if(line_status == CARRIAGERETURN_DETECTED ||
           line_status == COMMNTDELIM_DETECTED) {
                do {
                        c = fgetc(file_handle);
                } while(c != EOF && c != '\n');
        }
}

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
                program_status = CONTINUE_PARSE;
                break;
        case STOP_ACTION:
                *line_status = ENDOFFILE_DETECTED;
                program_status = STOP_PARSE;
                break;
        }
        
        return program_status;
}

word_type_t parse_wordtype(const char *buffer,
                           instruction_parameters_t **instruction_set) {
        word_type_t word_type = UNKNOWN;
        int index1, index2;

        if(buffer[0] < 'A' || buffer[0] > 'Z')
                return word_type;

        if(buffer[(strlen(buffer)) - 1] == ':') {
                word_type = LABEL;
        }

        if(word_type == UNKNOWN) {
                if((!strcmp("ORG", buffer)) || (!strcmp("EQU", buffer)))
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

void storein_symboltable(char *entry, uint8_t entry_type, uint8_t entry_nbytes,
                         uint8_t entry_value[], symboltable_t **symboltable_list,
                         uint8_t *symboltable_currentsize, 
                         uint8_t *symboltable_actualsize) {
        symboltable_t *symboltable_newlist;
        int index, mainindex, size, i;
        enum entry_status_t {NOT_FOUND = 0, FOUND} entry_status;

        entry_status = NOT_FOUND;

        for(index = 0; index < *symboltable_currentsize; ++index) 
                if(!strcmp((*symboltable_list)[index].name, entry)) {
                        entry_status = FOUND;
                        mainindex = index;
                }

        if(entry_status == FOUND) {
                if((*symboltable_list)[mainindex].value_status == DEFINED) {
                        /* If the symbol is already found and is already defined in the
                           symbol table, the this is an error since storing the symbol
                           table will define the symbol more than once. In that case
                           program execution must be terminated. */
                        free(*symboltable_list);
                        STDERR("the symbol \"%s\" is defined more than once\n", entry);
                        EFAILURE;
                }
                else {
                        (*symboltable_list)[mainindex].value_type = entry_type;
                        (*symboltable_list)[mainindex].value_nbytes = entry_nbytes;
                        
                        for(i = 0; i < (*symboltable_list)[mainindex].value_nbytes; ++i)
                                (*symboltable_list)[mainindex].value[i] = entry_value[i];
                        
                        (*symboltable_list)[mainindex].value_status = DEFINED;
                }
        }
        else {
                /* The following if-block is entered only when there is no more room in
                   the symbol table to store additional symbols. In that case, the symbol
                   table has to be dynamically extended to make room. */
                if(*symboltable_currentsize == *symboltable_actualsize) {
                        symboltable_newlist = realloc(*symboltable_list,
                                                      (*symboltable_actualsize + 10) *
                                                      sizeof(*symboltable_newlist));
                        if(symboltable_newlist == NULL) {
                                free(*symboltable_list);
                                STDERR("the symbol table could not be extended to "
                                       "store additional symbols\n");
                                EFAILURE;
                        }
                        else {
                                *symboltable_list = symboltable_newlist;
                                *symboltable_actualsize += 10;
                        }
                }

                index = (*symboltable_currentsize)++;
                size = strlen(entry) + 1;
                (*symboltable_list)[index].name = malloc(size *
                                             sizeof(*((*symboltable_list)[index].name)));
                if((*symboltable_list)[index].name == NULL) {
                        free(*symboltable_list);
                        STDERR("could not allocate space to store the symbol \"%s\"\n",
                               entry);
                        EFAILURE;
                }

                /* This point will not be reached if space could not be allocated to
                   store the entry string. */

                strcpy((*symboltable_list)[index].name, entry);
                (*symboltable_list)[index].value_type = entry_type;
                (*symboltable_list)[index].value_nbytes = entry_nbytes;
                
                for(i = 0; i < entry_nbytes; ++i)
                        (*symboltable_list)[index].value[i] = entry_value[i];

                (*symboltable_list)[index].value_status = DEFINED;
                
        }
        
}

uint16_t asciistr_to16bitnum(char *buffer) {
        int index, boundary;
        uint16_t value = 0;
        uint8_t exponent, multiplicand;

        index = strlen(buffer) - 1;

        if(buffer[index] == 'H' || buffer[index] == 'h') {
                index = strlen(buffer) - 2;
                boundary = 0;
                exponent = 0;
                for(; index >= boundary; --index) {
                        if(buffer[index] >= '0' && buffer[index] <= '9') 
                                multiplicand = buffer[index] - 48;
                        else if(buffer[index] >= 'A' && buffer[index] <= 'F')
                                multiplicand = buffer[index] - 55;
                        else if(buffer[index] >= 'a' && buffer[index] <= 'f')
                                multiplicand = buffer[index] - 87;
                        
                        value += multiplicand * pow(16, exponent);
                        ++exponent;
                }
        }
        else {
                index = strlen(buffer) - 1;
                boundary = 0;
                exponent = 0;
                for(; index >= boundary; --index) {
                        value += (buffer[index] - 48) * pow(10, exponent);
                        ++exponent;
                }
        }

        return value;
}

void extract_operands(FILE *file_handle, char *operand1, char *operand2,
                      line_status_t *line_status, uint8_t *n_operands) {
        int c, index;
        char buffer[20];

        /* Find the nearest non-whitespace character to determine from what location to
           start the extraction. */
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
                /* Once the first non-whitespace character is found, then store it in a
                   buffer to be later analyzed. Gather all characters up until either a
                   whitespace character, end of file indicator, carriage return, newline,
                   or comment delimiter is encountered. */
                do {
                        buffer[index++] = c;
                        c = fgetc(file_handle);
                } while(c != EOF && c != ' ' && c != '\t' && c != '\n' && c != '\r' &&
                        c != ';');
                buffer[index] = '\0';

// development start area
                if(!strcmp(buffer, "(IX") || !strcmp(buffer, "(IY") ||
                   !strcmp(buffer, "(IX+") || !strcmp(buffer, "(IY+")) {
                        if(c == ' ' || c == '\t')
                                        buffer[index++] = c;
                        do {
                                c = fgetc(file_handle);
                                if(c != EOF && c != '\n' && c != '\r' && c != ';') 
                                        buffer[index++] = c;
                        } while(c != EOF && c != '\n' && c != '\r' && c != ';' &&
                                c != ')');
                        buffer[index] = '\0';

                        if(c == ')') {
                                do {
                                        c = fgetc(file_handle);
                                        if(c != EOF && c != '\n' && c != '\r' &&
                                           c != ';' && c != ' ' && c != '\t')
                                                buffer[index++] = c;
                                } while(c != EOF && c != '\n' && c != '\r' &&
                                        c != ';' && c != ' ' && c != '\t');
                                buffer[index] = '\0';
                        }
                }
                
// devlopment end area
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

        /* Determine whether a comma exists at the end of the characters previously
           extracted. If the comma exists, then this means that 2 operands are being
           supplied and that the second operand must also be extracted as the same way as
           before. */
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

// development start area
                        if(!strcmp(buffer, "(IX") || !strcmp(buffer, "(IY") ||
                           !strcmp(buffer, "(IX+") || !strcmp(buffer, "(IY+")) {
                                if(c == ' ' || c == '\t')
                                        buffer[index++] = c;
                                do {
                                        c = fgetc(file_handle);
                                        if(c != EOF && c != '\n' && c != '\r' && c != ';') 
                                                buffer[index++] = c;
                                } while(c != EOF && c != '\n' && c != '\r' && c != ';' &&
                                        c != ')');
                                buffer[index] = '\0';
                        }
                
// devlopment end area
                        
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

data_status_t testif_numvalid(char *buffer, uint8_t *byte_length) {
        data_status_t data_status;
        int index, boundary;
        uint16_t value = 0;
        uint8_t exponent;
        uint8_t multiplicand;

        index = strlen(buffer) - 1;

        if(strlen(buffer) == 1 && (buffer[index] == 'H' || buffer[index] == 'h')) {
                return INVALID;
        }

        data_status = VALID;

        if(buffer[index] == 'H' || buffer[index] == 'h') {
                boundary = strlen(buffer) - 1;
                for(index = 0; index < boundary; ++index)
                        if((buffer[index] < '0' || buffer[index] > '9') &&
                           (buffer[index] < 'A' || buffer[index] > 'F') &&
                           (buffer[index] < 'a' || buffer[index] > 'f'))
                                data_status = INVALID;

                if(data_status != INVALID) {
                        index = boundary - 1;
                        boundary = 0;
                        exponent = 0;
                        for(; index >= boundary; --index) {
                                if((buffer[index] >= 'A' && buffer[index] <= 'F') ||
                                   (buffer[index] >= 'a' && buffer[index] <= 'f'))
                                        multiplicand = buffer[index] - 55;
                                else
                                        multiplicand = buffer[index] - 48;
                                value += multiplicand * pow(16, exponent);
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
                boundary = strlen(buffer);
                for(index = 0; index < boundary; ++index)
                        if(buffer[index] < '0' || buffer[index] > '9')
                                data_status = INVALID;

                if(data_status != INVALID) {
                        index = boundary - 1;
                        boundary = 0;
                        exponent = 0;
                        for(; index >= boundary; --index) {
                                value += (buffer[index] - 48) * pow(10, exponent);
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

void put8bitval_inhex(FILE *outputfile_handle, uint8_t value) {
        uint8_t lval;
        char c;

        lval = (value >> 4);
        if(lval >= 10)
                c = lval + 55;
        else
                c = lval + 48;

        fputc(c, outputfile_handle);

        lval = value & 0x0F;
        if(lval >= 10)
                c = lval + 55;
        else
                c = lval + 48;

        fputc(c, outputfile_handle);
}

uint8_t hexstr_todecnum(char *hexliteral) {
        uint8_t value;

        if(hexliteral[1] >= 'A' && hexliteral[1] <= 'F') 
                value = (hexliteral[1] - 55);

        else 
                value = (hexliteral[1] - 48);


        if(hexliteral[0] >= 'A' && hexliteral[0] <= 'F') 
                value |= ((hexliteral[0] - 55) << 4);

        else 
                value |= ((hexliteral[0] - 48) << 4);

        return value;
}
