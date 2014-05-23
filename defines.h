// File: defines.h
// Author: Jarielle Catbagan
// Created: 17, May 2014

// Description:

/* This file contains all global defines. */

#include <stdint.h>

#define STDERR(str, ...) fprintf(stderr, "error: " str, ##__VA_ARGS__)
#define EFAILURE exit(EXIT_FAILURE);
#define ESUCCESS exit(EXIT_SUCCESS);
#define DEBUG(str, ...) fprintf(stdout, str, ##__VA_ARGS__)

#ifndef DEFINES_H
#define DEFINES_H

#define NA 0x00
#define NONE_AFFECTED 0x00
#define OP1 0x40
#define OP2 0x80
#define BOTH_OPS 0xC0

// if ALL_BITS is selected, then either _8_BITVAL or _16_BITVAL must be selected
#define ALLBITS 0x00
#define _2BITS 0x10
#define _3BITS 0x20
// if _6BITS is selected, then either OP1_OP2 or OP2_OP1 must be selected
#define _6BITS 0x30

#define _8BITVAL 0x00
#define _16BITVAL 0x08

#define OP1_OP2 0x00
#define OP2_OP1 0x08

#define LBYTE 0x00
#define HBYTE 0x04

#define SHIFT_0X 0x00
#define SHIFT_1X 0x01
#define SHIFT_2X 0x02
#define SHIFT_3X 0x03
#define SHIFT_4X 0x04
#define SHIFT_5X 0x05
#define SHIFT_6X 0x06
#define SHIFT_7X 0X07

#define BIT  1
#define C_REGISTER  2
#define VALUE_8_BIT 3
#define REGISTER_8_BIT 4
#define ACCUMULATOR 5
#define IX_REGISTER 6
#define IY_REGISTER 7
#define IX_REGISTER_WOFFSET 8
#define IY_REGISTER_WOFFSET 9
#define HL_REGISTER 10
#define DE_REGISTER 11
#define BC_REGISTER 26
#define MEMORY_16_BIT 12
#define VALUE_16_BIT 13
#define REGISTER_16_BIT 14
#define SP_REGISTER 15
#define AF1 16
#define AF2 17
#define INTVECT_REGISTER 18
#define MEMREFRSH_REGISTER 19
#define CONDITION 20
#define CARRY_SET 21
#define CARRY_NOTSET 22
#define ZERO_SET 23
#define ZERO_NOTSET 24
#define NONE 25
#define INVALID_TYPE 255

#define UNDEFINED 0
#define DEFINED 1

typedef enum loop_status_t {EXIT = 0, CONTINUE} loop_status_t;
typedef enum action_status_t{STOP_ACTION = 0, LOOKFOR_NONWHITESPACE,
                             BEGIN_PARSING} action_status_t;
typedef enum program_status_t {PARSE_SOURCEFILE = 0,
                               ASSEMBLE_OUTPUTFILE} program_status_t;
typedef enum word_type_t {UNKNOWN = 0, LABEL, DIRECTIVE, INSTRUCTION} word_type_t;
typedef enum line_status_t {ENDOFFILE_DETECTED = 0, NEWLINE_DETECTED,
                            CARRIAGERETURN_DETECTED, COMMNTDELIM_DETECTED,
                            NONE_DETECTED} line_status_t;
typedef enum status_t {ERROR = 0, NO_ERROR} status_t;
typedef enum data_status_t {VALIDITY_UNKNOWN = 0, INVALID, VALID} data_status_t;

typedef struct instruction_parameters_t {
        char *instruction_name;
        uint8_t n_operands;
        uint8_t operand_type[2];
        uint8_t instruction_length;
        uint8_t instruction_value[4];
        uint8_t binary_code[4];
} instruction_parameters_t;

typedef struct symboltable_t {
        char *name;
        uint8_t operand_type;
        uint8_t value_nbytes;
        uint8_t value[2];
        uint8_t value_status;
} symboltable_t; 

#endif
