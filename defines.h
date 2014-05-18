// File: defines.h
// Author: Jarielle Catbagan
// Created: 17, May 2014

// Description:

/* This file contains all global defines. */

#ifndef DEFINES_H
#define DEFINES_H

typedef enum loop_status_t {EXIT = 0, CONTINUE} loop_status_t;
typedef enum action_status_t{STOP_ACTION = 0, LOOKFOR_NONWHITESPACE,
                             BEGIN_PARSING} action_status_t;
typedef enum program_status_t {PARSE_SOURCEFILE = 0,
                               ASSEMBLE_OUTPUTFILE} program_status_t;

#endif
