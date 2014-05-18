// File: parse.c
// Author: Jarielle Catbagan
// Created: 17, May 2014

#include <stdio.h>
#include "defines.h"

program_status_t extract_nearestword(FILE *file_handle, char *buffer,
                                     unsigned char max_buffersize) {
        loop_status_t loop_status;
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
                } while(c != EOF && (c == ' ' || c == '\r' || c == '\n'));

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
                } while(c != EOF && c != ';' && c != ' ' && c != '\n' && c != '\r');
                buffer[index] = '\0';

                if(c == ';') {
                        do {
                                c = fgetc(file_handle);
                        } while(c != EOF && c != '\n');
                }

                program_status = PARSE_SOURCEFILE;
                
                break;
        case STOP_ACTION:
                program_status = ASSEMBLE_OUTPUTFILE;
                break;
        }
        
        return program_status;
}
