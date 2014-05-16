// File: z80instructionset.h
// Author: Jarielle Catbagan
// Created: 11, May 2014

// Description:

/* This file contains all the instructions organized in alphabetical order with additional parameters for easy access when needed.  

   Note: RST p is not supported 

   The binary code array has a one-to-one correspondence with the actual binary value array

   Binary Code:

   Bit 7:6 - Determines which operands affect the corresponding actual binary value. 0b00 for no operands/doesn't affect the binary value, 0b01 for operand 1, 0b10 for operand 2
             and 0b11 for both operands.
   Bit 5:4 - 0b00 means the whole byte is affected, 0b01 means 2 continguous bits are affected, 0b10 means 3 contiguous bits are afected, and 0b11 means 6 continguous bits are affected.
             Note: This field is ignored if Bit 7:6 is 0b00 for no operands, meaning that this byte code should not affect the corresponding binary value. In that case, leave this 
             field to zero.
   Bit 3   - If Bit 5:4 is 0b00, then the definition of this field is as follows:
                     - '0' for when an 8-bit value is to be stored in the corresponding binary value and that this value makes up the whole value itself.
                     - '1' for when an 8-bit value is to be stored in the corresponding binary value and that this value is part of a 16-bit value.
             If Bit 5:4 is 0b11, then the definition of this field is as follows:
                     - '0' for when operand 1 appears first before operand 2 in the corresponding binary value.
                     - '1' for when operand 2 appears first before operand 1 in the corresponding binary value.

                     Note: Viewing which appears first is taken from the convention that the left-most bit appears first while the right-most bit appears last

             If Bit 5:4 is either 0b01 or 0b10 or if Bit 7:6 is 0b00, then this field is ignored and should be zero.
   Bit 2:0 - If Bit 7:6 is 0b00 or Bit 7:6 is not 0b00 and bit 5:4 is 0b11, then this field is ignored and should contain a zero.
             If bit 7:6 is not 0b00, Bit 5:4 is 0b00 and Bit 3 is 0b1, then Bit 2 determines either the low byte gets stored or the high byte;  Bit 1:0 is ignored in this case.
                     - '0' for low byte
                     - '1' for hight byte
             If Bit 7:6 is not 0b00 and Bit 5:4 is not 0b00 or 0b11, then these bits dictate how far should the 2 or 3 continguous bits be shifted from the right. Otherwise leave this
             field to zero.
*/

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
              
typedef struct {
        char *instruction_name;
        unsigned char number_of_operands;
        unsigned char operands[2];
        unsigned char instruction_length;
        unsigned char instruction_value[4];
        unsigned char binary_code[4];
} instruction_parameters_t;

instruction_parameters_t a_instructions[] =
{
        {"ADD", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0x80, NA, NA, NA}, {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},
        {"ADD", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xC6, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"ADD", 2, {ACCUMULATOR, HL_REGISTER}, 1, {0x86, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"ADD", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0x86, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"ADD", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0x86, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"ADC", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0x88, NA, NA, NA}, {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},
        {"ADC", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xCE, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"ADC", 2, {ACCUMULATOR, HL_REGISTER}, 1, {0x8E, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"ADC", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0x8E, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"ADC", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0x8E, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"AND", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0xA0, NA, NA, NA}, {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},
        {"AND", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xE6, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"AND", 2, {ACCUMULATOR, HL_REGISTER}, 1, {0xA6, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"AND", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0xA6, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"AND", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0xA6, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"ADD", 2, {HL_REGISTER, REGISTER_16_BIT}, 1, {0x09, NA, NA, NA}, {OP2 | _2BITS | SHIFT_4X, NA, NA, NA}},
        {"ADC", 2, {HL_REGISTER, REGISTER_16_BIT}, 2, {0xED, 0x4A, NA, NA}, {NONE_AFFECTED, OP2 | _2BITS | SHIFT_4X, NA, NA}},
        {"ADD", 2, {IX_REGISTER, REGISTER_16_BIT}, 2, {0xDD, 0x09, NA, NA}, {NONE_AFFECTED, OP2 | _2BITS | SHIFT_4X, NA, NA}},
        {"ADD", 2, {IY_REGISTER, REGISTER_16_BIT}, 2, {0xFD, 0x09, NA, NA}, {NONE_AFFECTED, OP2 | _2BITS | SHIFT_4X, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}, {0, 0, 0, 0}}
};


instruction_parameters_t b_instructions[] =
{
        {"BIT", 2, {BIT, REGISTER_8_BIT}, 2, {0xCB, 0x40, NA, NA}, {NONE_AFFECTED, BOTH_OPS | _6BITS | OP1_OP2 | SHIFT_0X, NA, NA}},
        {"BIT", 2, {BIT, HL_REGISTER}, 2, {0xCB, 0x46, NA, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_3X, NA, NA}},
        {"BIT", 2, {BIT, IX_REGISTER_WOFFSET}, 4, {0xDD, 0xCB, 0x00, 0x46}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, OP1 | _3BITS | SHIFT_3X}},
        {"BIT", 2, {BIT, IY_REGISTER_WOFFSET}, 4, {0xFD, 0xCB, 0x00, 0x46}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, OP1 | _3BITS | SHIFT_3X}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t c_instructions[] = 
{
        {"CPI", 0, {NONE, NONE}, 2, {0xED, 0xA1, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"CPIR", 0, {NONE, NONE}, 2, {0xED, 0xB1, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"CPD", 0, {NONE, NONE}, 2, {0xED, 0xA9, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"CPDR", 0, {NONE, NONE}, 2, {0xED, 0xB9, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"CP", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0xB8, NA, NA, NA}, {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},
        {"CP", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xFE, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"CP", 2, {ACCUMULATOR, HL_REGISTER}, 1, {0xBE, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"CP", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0xBE, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"CP", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0xBE, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"CPL", 0, {NONE, NONE}, 1, {0x2F, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"CCF", 0, {NONE, NONE}, 1, {0x3F, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"CALL", 1, {VALUE_16_BIT, NONE}, 3, {0xCD, 0x00, 0x00, NA}, {NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE, OP1 | ALLBITS | _16BITVAL | HBYTE, NA}},
        {"CALL", 2, {CONDITION, VALUE_16_BIT}, 3, {0xC4, 0x00, 0x00, NA}, {OP1 | _3BITS | SHIFT_3X, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS | _16BITVAL | HBYTE, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t d_instructions[] = 
{
        {"DEC", 1, {REGISTER_8_BIT, NONE}, 1, {0x05, NA, NA, NA}, {OP1 | _3BITS | SHIFT_3X, NA, NA, NA}},
        {"DEC", 1, {HL_REGISTER, NONE}, 1, {0x35, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"DEC", 1, {IX_REGISTER_WOFFSET, NONE}, 3, {0xDD, 0x35, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA}},
        {"DEC", 1, {IY_REGISTER_WOFFSET, NONE}, 3, {0xFD, 0x35, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA}},
        {"DAA", 0, {NONE, NONE}, 1, {0x27, NA, NA, NA}, {}},
        {"DI", 0, {NONE, NONE}, 1, {0xF3, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"DEC", 1, {REGISTER_16_BIT, NONE}, 1, {0x0B, NA, NA, NA}, {OP1 | _2BITS | SHIFT_4X, NA, NA, NA}},
        {"DEC", 1, {IX_REGISTER, NONE}, 2, {0xDD, 0x2B, NA, NA}, {NONE_AFFECTED | NONE_AFFECTED, NA, NA}},
        {"DEC", 1, {IY_REGISTER, NONE}, 2, {0xFD, 0x2B, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"DINZ", 1, {VALUE_8_BIT, NONE}, 2, {0x10, 0x00, NA, NA}, {NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t e_instructions[] = 
{
        {"EX", 2, {DE_REGISTER, HL_REGISTER}, 1, {0xEB, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"EX", 2, {AF1, AF2}, 1, {0x08, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"EX", 2, {SP_REGISTER, HL_REGISTER}, 1, {0xE3, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"EX", 2, {SP_REGISTER, IX_REGISTER}, 2, {0xDD, 0xE3, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"EX", 2, {SP_REGISTER, IY_REGISTER}, 2, {0xFD, 0xE3, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"EXX", 0, {NONE, NONE}, 1, {0xD9, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"EI", 0, {NONE, NONE}, 1, {0xFB, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t h_instructions[] = 
{
        {"HALT", 0, {NONE, NONE}, 1, {0x76, NA, NA, NA}, {NONE_AFFECTED, NA, NA ,NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t i_instructions[] = 
{
        {"INC", 1, {REGISTER_8_BIT, NA}, 1, {0x04, NA, NA, NA}, {OP1 | _3BITS | SHIFT_3X, NA, NA, NA}},
        {"INC", 1, {HL_REGISTER, NA}, 1, {0x34, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"INC", 1, {IX_REGISTER_WOFFSET, NA}, 3, {0xDD, 0x34, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA}},
        {"INC", 1, {IY_REGISTER_WOFFSET, NA}, 3, {0xFD, 0x34, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA}},
        {"IM0", 0, {NONE, NONE}, 2, {0xED, 0x46, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"IM1", 0, {NONE, NONE}, 2, {0xED, 0x56, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"IM2", 0, {NONE, NONE}, 2, {0xED, 0x5E, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"INC", 1, {REGISTER_16_BIT, NONE}, 1, {0x03, NA, NA, NA}, {OP1 | _2BITS | SHIFT_4X, NA, NA, NA}},
        {"INC", 1, {IX_REGISTER, NONE}, 2, {0xDD, 0x23, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"INC", 1, {IY_REGISTER, NONE}, 2, {0xFD, 0x23, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"IN", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xDB, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"IN", 2, {REGISTER_8_BIT, C_REGISTER}, 2, {0xED, 0x40, NA, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_3X, NA, NA}},
        {"INI", 0, {NONE, NONE}, 2, {0xED, 0xA2, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"INIR", 0, {NONE, NONE}, 2, {0xED, 0xB2, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"IND", 0, {NONE, NONE}, 2, {0xED, 0xAA, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"INDR", 0, {NONE, NONE}, 2, {0xED, 0xBA, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t j_instructions[] =
{
        {"JP", 1, {VALUE_16_BIT, NONE}, 3, {0xC3, 0x00, 0x00, NA}, {NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE, OP1 | ALLBITS | _16BITVAL | HBYTE, NA}},
        {"JP", 2, {CONDITION, VALUE_16_BIT}, 3, {0xC2, 0x00, 0x00, NA}, {OP1 | _3BITS | SHIFT_3X, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS | _16BITVAL | HBYTE, NA}},
        {"JR", 1, {VALUE_8_BIT, NONE}, 2, {0x18, 0x00, NA, NA}, {NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA, NA}},
        {"JR", 2, {CARRY_SET, VALUE_8_BIT}, 2, {0x38, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"JR", 2, {CARRY_NOTSET, VALUE_8_BIT}, 2, {0x30, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"JR", 2, {ZERO_SET, VALUE_8_BIT}, 2, {0x28, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"JR", 2, {ZERO_NOTSET, VALUE_8_BIT}, 2, {0x20, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"JP", 1, {HL_REGISTER, NONE}, 1, {0xE9, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"JP", 1, {IX_REGISTER, NONE}, 2, {0xDD, 0xE9, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"JP", 1, {IY_REGISTER, NONE}, 2, {0xFD, 0xE9, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};


instruction_parameters_t l_instructions[] = 
{
        {"LD", 2, {REGISTER_8_BIT, REGISTER_8_BIT}, 1, {0x40, NA, NA, NA}, {BOTH_OPS | _6BITS | OP1_OP2, NA, NA, NA}},
        {"LD", 2, {REGISTER_8_BIT, VALUE_8_BIT}, 2, {0x06, 0x00, NA, NA}, {OP1 | _3BITS | SHIFT_3X, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"LD", 2, {REGISTER_8_BIT, HL_REGISTER}, 1, {0x46, NA, NA, NA}, {OP1 | _3BITS | SHIFT_3X, NA, NA, NA}},
        {"LD", 2, {REGISTER_8_BIT, IX_REGISTER_WOFFSET}, 3, {0xDD, 0x46, 0x00, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_3X, OP2 | ALLBITS | _8BITVAL, NA}},
        {"LD", 2, {REGISTER_8_BIT, IY_REGISTER_WOFFSET}, 3, {0xFD, 0x46, 0x00, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_3X, OP2 | ALLBITS | _8BITVAL, NA}},
        {"LD", 2, {HL_REGISTER, REGISTER_8_BIT}, 1, {0x70, NA, NA, NA}, {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},
        {"LD", 2, {IX_REGISTER_WOFFSET, REGISTER_8_BIT}, 3, {0xDD, 0x70, 0x00, NA}, {NONE_AFFECTED, OP2 | _3BITS | SHIFT_0X, OP1 | ALLBITS | _8BITVAL, NA}},
        {"LD", 2, {IY_REGISTER_WOFFSET, REGISTER_8_BIT}, 3, {0xFD, 0x70, 0x00, NA}, {NONE_AFFECTED, OP2 | _3BITS | SHIFT_0X, OP1 | ALLBITS | _8BITVAL, NA}},
        {"LD", 2, {HL_REGISTER, VALUE_8_BIT}, 2, {0x36, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"LD", 2, {IX_REGISTER_WOFFSET, VALUE_8_BIT}, 4, {0xDD, 0x36, 0x00, 0x00}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, OP2 | ALLBITS | _8BITVAL}},
        {"LD", 2, {IY_REGISTER_WOFFSET, VALUE_8_BIT}, 4, {0xFD, 0x36, 0x00, 0x00}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, OP2 | ALLBITS | _8BITVAL}},
        {"LD", 2, {ACCUMULATOR, BC_REGISTER}, 1, {0x0A, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"LD", 2, {ACCUMULATOR, DE_REGISTER}, 1, {0x1A, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"LD", 2, {ACCUMULATOR, VALUE_16_BIT}, 3, {0x3A, 0x00, 0x00, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS | _16BITVAL | HBYTE, NA}},
        {"LD", 2, {BC_REGISTER, ACCUMULATOR}, 1, {0x02, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"LD", 2, {DE_REGISTER, ACCUMULATOR}, 1, {0x12, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"LD", 2, {VALUE_16_BIT, ACCUMULATOR}, 3, {0x32, 0x00, 0x00, NA}, {NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE, OP1 | ALLBITS | _16BITVAL | HBYTE, NA}},
        {"LD", 2, {ACCUMULATOR, INTVECT_REGISTER}, 2, {0xED, 0x57, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"LD", 2, {ACCUMULATOR, MEMREFRSH_REGISTER}, 2, {0xED, 0x5F, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"LD", 2, {INTVECT_REGISTER, ACCUMULATOR}, 2, {0xED, 0x47, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"LD", 2, {MEMREFRSH_REGISTER, ACCUMULATOR}, 2, {0xED, 0x4F, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"LD", 2, {REGISTER_16_BIT, VALUE_16_BIT}, 3, {0x01, 0x00, 0x00, NA}, {OP1 | _2BITS | SHIFT_4X, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS | _16BITVAL | HBYTE, NA}},
        {"LD", 2, {IX_REGISTER, VALUE_16_BIT}, 4, {0xDD, 0x21, 0x00, 0x00}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {IY_REGISTER, VALUE_16_BIT}, 4, {0xFD, 0x21, 0x00, 0x00}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {REGISTER_16_BIT, MEMORY_16_BIT}, 4, {0xED, 0x4B, 0x00, 0x00}, 
         {NONE_AFFECTED, OP1 | _2BITS | SHIFT_4X, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {IX_REGISTER, MEMORY_16_BIT}, 4, {0xDD, 0x2A, 0x00, 0x00}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {IY_REGISTER, MEMORY_16_BIT}, 4, {0xFD, 0x2A, 0x00, 0x00}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {MEMORY_16_BIT, REGISTER_16_BIT}, 4, {0xED, 0x43, 0x00, 0x00}, 
         {NONE_AFFECTED, OP2 | _2BITS | SHIFT_4X, OP1 | ALLBITS | _16BITVAL | LBYTE, OP1 | ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {MEMORY_16_BIT, IX_REGISTER}, 4, {0xDD, 0x22, 0x00, 0x00}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE, OP1 | ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {MEMORY_16_BIT, IY_REGISTER}, 4, {0xFD, 0x22, 0x00, 0x00}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE, OP1 | ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {SP_REGISTER, HL_REGISTER}, 1, {0xF9, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"LD", 2, {SP_REGISTER, IX_REGISTER}, 2, {0xDD, 0xF9, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"LD", 2, {SP_REGISTER, IY_REGISTER}, 2, {0xFD, 0xF9, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"LDI", 0, {NONE, NONE}, 2, {0xED, 0xA0, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"LDIR", 0, {NONE, NONE}, 2, {0xED, 0xB0, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"LDD", 0, {NONE, NONE}, 2, {0xED, 0xA8, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"LDDR", 0, {NONE, NONE}, 2, {0xED, 0xB8, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
         {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};


instruction_parameters_t n_instructions[] = 
{
        {"NEG", 0, {NONE, NONE}, 2, {0xED, 0x44, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"NOP", 0, {NONE, NONE}, 1, {0x00, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t o_instructions[] = 
{
        {"OR", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0xB0, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"OR", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xF6, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"OR", 2, {ACCUMULATOR, HL_REGISTER}, 1, {0xB6, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"OR", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0xB6, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"OR", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0xB6, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"OUT", 2, {VALUE_8_BIT, ACCUMULATOR}, 2, {0xD3, 0x00, NA, NA}, {NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA, NA}},
        {"OUT", 2, {C_REGISTER, REGISTER_8_BIT}, 2, {0xED, 0x41, NA, NA}, {NONE_AFFECTED, OP2 | _3BITS | SHIFT_3X, NA, NA}},
        {"OUTI", 0, {NONE, NONE}, 2, {0xED, 0xA3, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"OTIR", 0, {NONE, NONE}, 2, {0xED, 0xB3, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"OUTD", 0, {NONE, NONE}, 2, {0xED, 0xAB, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"OTDR", 0, {NONE, NONE}, 2, {0xED, 0xBB, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};


instruction_parameters_t p_instructions[] = 
{
        {"PUSH", 1, {REGISTER_16_BIT, NONE}, 1, {0xC5, NA, NA, NA}, {OP1 | _2BITS | SHIFT_4X, NA, NA, NA}},
        {"PUSH", 1, {IX_REGISTER, NONE}, 2, {0xDD, 0xE5, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"PUSH", 1, {IY_REGISTER, NONE}, 2, {0xFD, 0xE5, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"POP", 1, {REGISTER_16_BIT, NONE}, 1, {0xC1, NA, NA, NA}, {OP1 | _2BITS | SHIFT_4X, NA, NA, NA}},
        {"POP", 1, {IX_REGISTER, NONE}, 2, {0xDD, 0xE1, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"POP", 1, {IY_REGISTER, NONE}, 2, {0xFD, 0xE1, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t r_instructions[] =
{
        {"RLCA", 0, {NONE, NONE}, 1, {0x07, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"RLA", 0, {NONE, NONE}, 1, {0x17, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"RRCA", 0, {NONE, NONE}, 1, {0x0F, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"RRA", 0, {NONE, NONE}, 1, {0x1F, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"RLC", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x00, NA, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},
        {"RLC", 1, {HL_REGISTER, NONE}, 2, {0xCB, 0x06, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"RLC", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x06}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"RLC", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x06}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"RL", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x10, NA, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},
        {"RL", 1, {HL_REGISTER, NONE}, 2, {0xCB, 0x16, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"RL", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x16}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"RL", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x16}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"RRC", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x08, NA, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},
        {"RRC", 1, {HL_REGISTER, NONE}, 2, {0xCB, 0x0E, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"RRC", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x0E}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"RRC", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x0E}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"RR", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x18, NA, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},
        {"RR", 1, {HL_REGISTER, NONE}, 2, {0xCB, 0x1E, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"RR", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x1E}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"RR", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x1E}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"RLD", 0, {NONE, NONE}, 2, {0xED, 0x6F, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"RRD", 0, {NONE, NONE}, 2, {0xED, 0x67, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"RES", 2, {BIT, REGISTER_8_BIT}, 2, {0xCB, 0x80, NA, NA}, {NONE_AFFECTED, BOTH_OPS | _6BITS | OP1_OP2, NA, NA}},
        {"RES", 2, {BIT, HL_REGISTER}, 2, {0xCB, 0x86, NA, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_3X, NA, NA}},
        {"RES", 2, {BIT, IX_REGISTER_WOFFSET}, 4, {0xDD, 0xCB, 0x00, 0x86}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, OP1 | _3BITS | SHIFT_3X}},
        {"RES", 2, {BIT, IY_REGISTER_WOFFSET}, 4, {0xFD, 0xCB, 0x00, 0x86}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, OP1 | _3BITS | SHIFT_3X}},
        {"RET", 0, {NONE, NONE}, 1, {0xC9, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"RET", 1, {CONDITION, NONE}, 1, {0xC0, NA, NA, NA}, {OP1 | _3BITS | SHIFT_3X, NA, NA, NA}},
        {"RETI", 0, {NONE, NONE}, 2, {0xED, 0x4D, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"RETN", 0, {NONE, NONE}, 2, {0xED, 0x45, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t s_instructions[] = 
{
        {"SUB", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0x90, NA, NA, NA}, {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},
        {"SUB", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xD6, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"SUB", 2, {ACCUMULATOR, HL_REGISTER}, 1, {0x96, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"SUB", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0x96, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"SUB", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0x96, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"SBC", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0x98, NA, NA, NA}, {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},
        {"SBC", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xDE, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"SBC", 2, {ACCUMULATOR, HL_REGISTER}, 1, {0x9E, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"SBC", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0x9E, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"SBC", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0x9E, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"SCF", 0, {NONE, NONE}, 1, {0x37, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"SBC", 2, {HL_REGISTER, REGISTER_16_BIT}, 2, {0xED, 0x42, NA, NA}, {NONE_AFFECTED, OP2 | _2BITS | SHIFT_4X, NA, NA}},
        {"SLA", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x20, NA, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},
        {"SLA", 1, {HL_REGISTER, NONE}, 2, {0xCB, 0x26, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"SLA", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x26}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"SLA", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x26}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"SRA", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x28, NA, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},
        {"SRA", 1, {HL_REGISTER, NONE}, 2, {0xCB, 0x2E, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"SRA", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x2E}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"SRA", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x2E}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"SRL", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x38, NA, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},
        {"SRL", 1, {HL_REGISTER, NONE}, 2, {0xCB, 0x3E, NA, NA}, {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"SRL", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x3E}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"SRL", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x3E}, {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"SET", 2, {BIT, REGISTER_8_BIT}, 2, {0xCB, 0xC0, NA, NA}, {NONE_AFFECTED, BOTH_OPS | _6BITS | OP1_OP2, NA, NA}},
        {"SET", 2, {BIT, HL_REGISTER}, 2, {0xCB, 0xC6, NA, NA}, {NONE_AFFECTED, OP1 | _3BITS | SHIFT_3X, NA, NA}},
        {"SET", 2, {BIT, IX_REGISTER_WOFFSET}, 4, {0xDD, 0xCB, 0x00, 0xC6}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, OP1 | _3BITS | SHIFT_3X}},
        {"SET", 2, {BIT, IY_REGISTER_WOFFSET}, 4, {0xFD, 0xCB, 0x00, 0xC6}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, OP1 | _3BITS | SHIFT_3X}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t x_instructions[] = 
{
        {"XOR", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0xA8, NA, NA, NA}, {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},
        {"XOR", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xEE, 0x00, NA, NA}, {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"XOR", 2, {ACCUMULATOR, HL_REGISTER}, 1, {0xAE, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {"XOR", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0xAE, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"XOR", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0xAE, 0x00, NA}, {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t *instruction_set[26] = {a_instructions, b_instructions, c_instructions, d_instructions, e_instructions, NULL, NULL, h_instructions,
						 i_instructions, j_instructions, NULL, l_instructions, NULL, n_instructions, o_instructions, p_instructions,
						 NULL, r_instructions, s_instructions,  NULL, NULL, NULL, NULL, x_instructions, NULL, NULL};

