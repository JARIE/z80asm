// File: z80instructionset.h
// Author: Jarielle Catbagan
// Created: 11, May 2014

// Description:

/* This file contains all the instructions organized in alphabetical order with
   additional parameters for easy access when needed.  

   Note: RST p is not supported 

   The binary code array has a one-to-one correspondence with the actual binary value
   array

   Binary Code:

   Bit 7:6 - Determines which operands affect the corresponding actual binary value.
             0b00 for no operands/doesn't affect the binary value, 0b01 for operand 1,
             0b10 for operand 2 and 0b11 for both operands.
   Bit 5:4 - 0b00 means the whole byte is affected, 0b01 means 2 continguous bits are
             affected, 0b10 means 3 contiguous bits are afected, and 0b11 means 6
             continguous bits are affected.
             Note: This field is ignored if Bit 7:6 is 0b00 for no operands, meaning that
             this byte code should not affect the corresponding binary value. In that
             case, leave this field to zero.
   Bit 3   - If Bit 5:4 is 0b00, then the definition of this field is as follows:

             '0' for when an 8-bit value is to be stored in the corresponding
             binary value and that this value makes up the whole value itself.
             '1' for when an 8-bit value is to be stored in the corresponding binary
             value and that this value is part of a 16-bit value.

             If Bit 5:4 is 0b11, then the definition of this field is as follows:

             '0' for when operand 1 appears first before operand 2 in the corresponding
             binary value.
             '1' for when operand 2 appears first before operand 1 in the corresponding
             binary value.

             Note: Viewing which appears first is taken from the convention that the
             left-most bit appears first while the right-most bit appears last

             If Bit 5:4 is either 0b01 or 0b10 or if Bit 7:6 is 0b00, then this field
             is ignored and should be zero.

   Bit 2:0 - If Bit 7:6 is 0b00 or Bit 7:6 is not 0b00 and bit 5:4 is 0b11,
             then this field is ignored and should contain a zero.

             If bit 7:6 is not 0b00, Bit 5:4 is 0b00 and Bit 3 is 0b1, then Bit 2
             determines either the low byte gets stored or the high byte;  Bit 1:0 is
             ignored in this case.
                     - '0' for low byte
                     - '1' for hight byte

             If Bit 7:6 is not 0b00 and Bit 5:4 is not 0b00 or 0b11, then these bits
             dictate how far should the 2 or 3 continguous bits be shifted from the
             right. Otherwise leave this field to zero.
*/

#include "defines.h"

#ifndef Z80INSTRUCTIONSET_H
#define Z80INSTRUCTIONSET_H
              
instruction_parameters_t a_instructions[] =
{
        // ADD A, r
        {"ADD", 2, {ACCUMULATOR, ACCUMULATOR}, 1, {0x87, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"ADD", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0x80, NA, NA, NA},
         {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},

        // ADD A, n
        {"ADD", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xC6, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // ADD A, (HL)
        {"ADD", 2, {ACCUMULATOR, HL_REGISTER_MEMREF}, 1, {0x86, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // ADD A, (IX + d)
        {"ADD", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0x86, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // ADD A, (IY + d)
        {"ADD", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0x86, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // ADC A, r
        {"ADC", 2, {ACCUMULATOR, ACCUMULATOR}, 1, {0x8F, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"ADC", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0x88, NA, NA, NA},
         {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},

        // ADC A, n
        {"ADC", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xCE, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // ADC A, (HL)
        {"ADC", 2, {ACCUMULATOR, HL_REGISTER_MEMREF}, 1, {0x8E, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // ADC A, (IX + d)
        {"ADC", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0x8E, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // ADC A, (IY + d)
        {"ADC", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0x8E, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // AND A, r
        {"AND", 2, {ACCUMULATOR, ACCUMULATOR}, 1, {0xA7, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"AND", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0xA0, NA, NA, NA},
         {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},

        // AND A, n
        {"AND", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xE6, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // AND A, (HL)
        {"AND", 2, {ACCUMULATOR, HL_REGISTER_MEMREF}, 1, {0xA6, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // AND A, (IX + d)
        {"AND", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0xA6, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // AND A, (IY + d)
        {"AND", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0xA6, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // ADD HL, ss
        {"ADD", 2, {HL_REGISTER, BC_REGISTER}, 1, {0x09, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"ADD", 2, {HL_REGISTER, DE_REGISTER}, 1, {0x19, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"ADD", 2, {HL_REGISTER, HL_REGISTER}, 1, {0x29, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"ADD", 2, {HL_REGISTER, SP_REGISTER}, 1, {0x39, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // ADC HL, ss
        {"ADC", 2, {HL_REGISTER, BC_REGISTER}, 2, {0xED, 0x4A, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"ADC", 2, {HL_REGISTER, DE_REGISTER}, 2, {0xED, 0x5A, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"ADC", 2, {HL_REGISTER, HL_REGISTER}, 2, {0xED, 0x6A, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"ADC", 2, {HL_REGISTER, SP_REGISTER}, 2, {0xED, 0x7A, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // ADD IX, pp
        {"ADD", 2, {IX_REGISTER, BC_REGISTER}, 2, {0xDD, 0x09, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"ADD", 2, {IX_REGISTER, DE_REGISTER}, 2, {0xDD, 0x19, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"ADD", 2, {IX_REGISTER, IX_REGISTER}, 2, {0xDD, 0x29, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"ADD", 2, {IX_REGISTER, SP_REGISTER}, 2, {0xDD, 0x39, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // ADD IY, pp
        {"ADD", 2, {IY_REGISTER, BC_REGISTER}, 2, {0xFD, 0x09, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"ADD", 2, {IY_REGISTER, DE_REGISTER}, 2, {0xFD, 0x19, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"ADD", 2, {IY_REGISTER, IY_REGISTER}, 2, {0xFD, 0x29, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"ADD", 2, {IY_REGISTER, SP_REGISTER}, 2, {0xFD, 0x39, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}, {0, 0, 0, 0}}
};


instruction_parameters_t b_instructions[] =
{
        // BIT b, r
        {"BIT", 2, {BIT, ACCUMULATOR}, 2, {0xCB, 0x47, NA, NA},
         {NONE_AFFECTED, BOTH_OPS | _6BITS | OP1_OP2 | SHIFT_0X, NA, NA}},        
        {"BIT", 2, {BIT, REGISTER_8_BIT}, 2, {0xCB, 0x40, NA, NA},
         {NONE_AFFECTED, BOTH_OPS | _6BITS | OP1_OP2 | SHIFT_0X, NA, NA}},

        // BIT b, (HL)
        {"BIT", 2, {BIT, HL_REGISTER_MEMREF}, 2, {0xCB, 0x46, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_3X, NA, NA}},

        // BIT b, (IX + d)
        {"BIT", 2, {BIT, IX_REGISTER_WOFFSET}, 4, {0xDD, 0xCB, 0x00, 0x46},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS |
          _8BITVAL, OP1 | _3BITS | SHIFT_3X}},

        // BIT b, (IY + d)
        {"BIT", 2, {BIT, IY_REGISTER_WOFFSET}, 4, {0xFD, 0xCB, 0x00, 0x46},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS |
          _8BITVAL, OP1 | _3BITS | SHIFT_3X}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t c_instructions[] = 
{
        // CPI
        {"CPI", 0, {NONE, NONE}, 2, {0xED, 0xA1, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // CPIR
        {"CPIR", 0, {NONE, NONE}, 2, {0xED, 0xB1, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // CPD
        {"CPD", 0, {NONE, NONE}, 2, {0xED, 0xA9, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // CPDR
        {"CPDR", 0, {NONE, NONE}, 2, {0xED, 0xB9, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // CP A, r
        {"CP", 2, {ACCUMULATOR, ACCUMULATOR}, 1, {0xBF, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"CP", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0xB8, NA, NA, NA},
         {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},

        // CP A, n
        {"CP", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xFE, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // CP A, (HL)
        {"CP", 2, {ACCUMULATOR, HL_REGISTER_MEMREF}, 1, {0xBE, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // CP A, (IX + d)
        {"CP", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0xBE, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // CP A, (IY + d)
        {"CP", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0xBE, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // CPL
        {"CPL", 0, {NONE, NONE}, 1, {0x2F, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},

        // CCF
        {"CCF", 0, {NONE, NONE}, 1, {0x3F, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},

        // CALL nn
        {"CALL", 1, {VALUE_8_BIT, NONE}, 3, {0xCD, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED, NA}},        
        {"CALL", 1, {VALUE_16_BIT, NONE}, 3, {0xCD, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE, OP1 | ALLBITS |
          _16BITVAL | HBYTE, NA}},

        // CALL cc, nn
        {"CALL", 2, {CONDITION, VALUE_8_BIT}, 3, {0xC4, 0x00, 0x00, NA},
         {OP1 | _3BITS | SHIFT_3X, OP2 | ALLBITS | _8BITVAL, NONE_AFFECTED, NA}},        
        {"CALL", 2, {CONDITION, VALUE_16_BIT}, 3, {0xC4, 0x00, 0x00, NA},
         {OP1 | _3BITS | SHIFT_3X, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS |
          _16BITVAL | HBYTE, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t d_instructions[] = 
{
        // DEC r
        {"DEC", 1, {ACCUMULATOR, NONE}, 1, {0x3D, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"DEC", 1, {REGISTER_8_BIT, NONE}, 1, {0x05, NA, NA, NA},
         {OP1 | _3BITS | SHIFT_3X, NA, NA, NA}},

        // DEC (HL)
        {"DEC", 1, {HL_REGISTER_MEMREF, NONE}, 1, {0x35, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // DEC (IX + d)
        {"DEC", 1, {IX_REGISTER_WOFFSET, NONE}, 3, {0xDD, 0x35, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA}},

        // DEC (IY + d)
        {"DEC", 1, {IY_REGISTER_WOFFSET, NONE}, 3, {0xFD, 0x35, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA}},

        // DAA
        {"DAA", 0, {NONE, NONE}, 1, {0x27, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},

        // DI
        {"DI", 0, {NONE, NONE}, 1, {0xF3, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},

        // DEC ss
        {"DEC", 1, {BC_REGISTER, NONE}, 1, {0x0B, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"DEC", 1, {DE_REGISTER, NONE}, 1, {0x1B, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"DEC", 1, {HL_REGISTER, NONE}, 1, {0x2B, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"DEC", 1, {SP_REGISTER, NONE}, 1, {0x3B, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // DEC IX
        {"DEC", 1, {IX_REGISTER, NONE}, 2, {0xDD, 0x2B, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // DEC IY
        {"DEC", 1, {IY_REGISTER, NONE}, 2, {0xFD, 0x2B, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // DINZ n
        {"DJNZ", 1, {VALUE_8_BIT, NONE}, 2, {0x10, 0x00, NA, NA},
         {NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t e_instructions[] = 
{
        // EX DE, HL
        {"EX", 2, {DE_REGISTER, HL_REGISTER}, 1, {0xEB, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // EX AF, AF'
        {"EX", 2, {AF_REGISTER, _AF_REGISTER}, 1, {0x08, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // EX (SP), HL
        {"EX", 2, {SP_REGISTER_MEMREF, HL_REGISTER}, 1, {0xE3, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // EX (SP), IX
        {"EX", 2, {SP_REGISTER_MEMREF, IX_REGISTER}, 2, {0xDD, 0xE3, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // EX (SP), IY
        {"EX", 2, {SP_REGISTER_MEMREF, IY_REGISTER}, 2, {0xFD, 0xE3, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // EXX
        {"EXX", 0, {NONE, NONE}, 1, {0xD9, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},

        // EI
        {"EI", 0, {NONE, NONE}, 1, {0xFB, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t h_instructions[] = 
{
        // HALT
        {"HALT", 0, {NONE, NONE}, 1, {0x76, NA, NA, NA}, {NONE_AFFECTED, NA, NA ,NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t i_instructions[] = 
{
        // INC r
        {"INC", 1, {ACCUMULATOR, NONE}, 1, {0x3C, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"INC", 1, {REGISTER_8_BIT, NONE}, 1, {0x04, NA, NA, NA},
         {OP1 | _3BITS | SHIFT_3X, NA, NA, NA}},

        // INC (HL)
        {"INC", 1, {HL_REGISTER_MEMREF, NONE}, 1, {0x34, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // INC (IX + d)
        {"INC", 1, {IX_REGISTER_WOFFSET, NONE}, 3, {0xDD, 0x34, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA}},

        // INC (IY + d)
        {"INC", 1, {IY_REGISTER_WOFFSET, NONE}, 3, {0xFD, 0x34, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA}},

        // IM0
        {"IM0", 0, {NONE, NONE}, 2, {0xED, 0x46, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // IM1
        {"IM1", 0, {NONE, NONE}, 2, {0xED, 0x56, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // IM2
        {"IM2", 0, {NONE, NONE}, 2, {0xED, 0x5E, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // INC ss
        {"INC", 1, {BC_REGISTER, NONE}, 1, {0x03, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"INC", 1, {DE_REGISTER, NONE}, 1, {0x13, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"INC", 1, {HL_REGISTER, NONE}, 1, {0x23, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"INC", 1, {SP_REGISTER, NONE}, 1, {0x33, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // INC IX
        {"INC", 1, {IX_REGISTER, NONE}, 2, {0xDD, 0x23, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // INC IY
        {"INC", 1, {IY_REGISTER, NONE}, 2, {0xFD, 0x23, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // IN A, n
        {"IN", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xDB, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // IN, r, (C)
        {"IN", 2, {ACCUMULATOR, C_REGISTER_MEMREF}, 2, {0xED, 0x78, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"IN", 2, {REGISTER_8_BIT, C_REGISTER_MEMREF}, 2, {0xED, 0x40, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_3X, NA, NA}},

        // INI
        {"INI", 0, {NONE, NONE}, 2, {0xED, 0xA2, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // INIR
        {"INIR", 0, {NONE, NONE}, 2, {0xED, 0xB2, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // IND
        {"IND", 0, {NONE, NONE}, 2, {0xED, 0xAA, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // INDR
        {"INDR", 0, {NONE, NONE}, 2, {0xED, 0xBA, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t j_instructions[] =
{
        // JP nn
        {"JP", 1, {MEMORY_16_BIT, NONE}, 3, {0xC3, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE, OP1 | ALLBITS |
          _16BITVAL | HBYTE, NA}},

        // JP cc, nn
        {"JP", 2, {CONDITION, MEMORY_16_BIT}, 3, {0xC2, 0x00, 0x00, NA},
         {OP1 | _3BITS | SHIFT_3X, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS |
          _16BITVAL | HBYTE, NA}},

        // JR n
        {"JR", 1, {VALUE_8_BIT, NONE}, 2, {0x18, 0x00, NA, NA},
         {NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA, NA}},

        // JR C, n
        {"JR", 2, {CARRY_SET, VALUE_8_BIT}, 2, {0x38, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // JR NC, n
        {"JR", 2, {CARRY_NOTSET, VALUE_8_BIT}, 2, {0x30, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // JR Z, n
        {"JR", 2, {ZERO_SET, VALUE_8_BIT}, 2, {0x28, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // JR NZ, n
        {"JR", 2, {ZERO_NOTSET, VALUE_8_BIT}, 2, {0x20, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // JP (HL)
        {"JP", 1, {HL_REGISTER_MEMREF, NONE}, 1, {0xE9, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // JP IX
        {"JP", 1, {IX_REGISTER, NONE}, 2, {0xDD, 0xE9, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // JP IY
        {"JP", 1, {IY_REGISTER, NONE}, 2, {0xFD, 0xE9, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};


instruction_parameters_t l_instructions[] = 
{
        // LD r, r
        {"LD", 2, {ACCUMULATOR, ACCUMULATOR}, 1, {0x7F, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"LD", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0x78, NA, NA, NA},
         {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},
        {"LD", 2, {REGISTER_8_BIT, ACCUMULATOR}, 1, {0x47, NA, NA, NA},
         {OP1 | _3BITS | SHIFT_3X, NA, NA, NA}},
        {"LD", 2, {REGISTER_8_BIT, REGISTER_8_BIT}, 1, {0x40, NA, NA, NA},
         {BOTH_OPS | _6BITS | OP1_OP2, NA, NA, NA}},

        // LD r, n
        {"LD", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0x3E, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},
        {"LD", 2, {REGISTER_8_BIT, VALUE_8_BIT}, 2, {0x06, 0x00, NA, NA},
         {OP1 | _3BITS | SHIFT_3X, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // LD r, (HL)
        {"LD", 2, {ACCUMULATOR, HL_REGISTER_MEMREF}, 1, {0x7E, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"LD", 2, {REGISTER_8_BIT, HL_REGISTER_MEMREF}, 1, {0x46, NA, NA, NA},
         {OP1 | _3BITS | SHIFT_3X, NA, NA, NA}},
        
        
        // LD r, (IX + d)
        {"LD", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0x7E, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"LD", 2, {REGISTER_8_BIT, IX_REGISTER_WOFFSET}, 3, {0xDD, 0x46, 0x00, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_3X, OP2 | ALLBITS | _8BITVAL, NA}},
        
        // LD r, (IY + d)
        {"LD", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0x7E, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {"LD", 2, {REGISTER_8_BIT, IY_REGISTER_WOFFSET}, 3, {0xFD, 0x46, 0x00, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_3X, OP2 | ALLBITS | _8BITVAL, NA}},

        // LD (HL), r
        {"LD", 2, {HL_REGISTER_MEMREF, ACCUMULATOR}, 1, {0x77, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"LD", 2, {HL_REGISTER_MEMREF, REGISTER_8_BIT}, 1, {0x70, NA, NA, NA},
         {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},

        // LD (IX + d), r
        {"LD", 2, {IX_REGISTER_WOFFSET, ACCUMULATOR}, 3, {0xDD, 0x77, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA}},
        {"LD", 2, {IX_REGISTER_WOFFSET, REGISTER_8_BIT}, 3, {0xDD, 0x70, 0x00, NA},
         {NONE_AFFECTED, OP2 | _3BITS | SHIFT_0X, OP1 | ALLBITS | _8BITVAL, NA}},
        
        // LD (IY + d), r
        {"LD", 2, {IY_REGISTER_WOFFSET, ACCUMULATOR}, 3, {0xFD, 0x77, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA}},
        {"LD", 2, {IY_REGISTER_WOFFSET, REGISTER_8_BIT}, 3, {0xFD, 0x70, 0x00, NA},
         {NONE_AFFECTED, OP2 | _3BITS | SHIFT_0X, OP1 | ALLBITS | _8BITVAL, NA}},

        // LD (HL), n
        {"LD", 2, {HL_REGISTER_MEMREF, VALUE_8_BIT}, 2, {0x36, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // LD (IX + d), n
        {"LD", 2, {IX_REGISTER_WOFFSET, VALUE_8_BIT}, 4, {0xDD, 0x36, 0x00, 0x00},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, OP2 | ALLBITS |
          _8BITVAL}},

        // LD (IY + d), n
        {"LD", 2, {IY_REGISTER_WOFFSET, VALUE_8_BIT}, 4, {0xFD, 0x36, 0x00, 0x00},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, OP2 | ALLBITS |
          _8BITVAL}},

        // LD A, (BC)
        {"LD", 2, {ACCUMULATOR, BC_REGISTER_MEMREF}, 1, {0x0A, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // LD A, (DE)
        {"LD", 2, {ACCUMULATOR, DE_REGISTER_MEMREF}, 1, {0x1A, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // LD A, (nn)
        {"LD", 2, {ACCUMULATOR, MEMORY_16_BIT}, 3, {0x3A, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS |
          _16BITVAL | HBYTE, NA}},

        // LD, (BC), A
        {"LD", 2, {BC_REGISTER_MEMREF, ACCUMULATOR}, 1, {0x02, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // LD, (DE), A
        {"LD", 2, {DE_REGISTER_MEMREF, ACCUMULATOR}, 1, {0x12, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // LD, (nn), A
        {"LD", 2, {MEMORY_16_BIT, ACCUMULATOR}, 3, {0x32, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE, OP1 | ALLBITS |
          _16BITVAL | HBYTE, NA}},

        // LD A, I
        {"LD", 2, {ACCUMULATOR, INTVECT_REGISTER}, 2, {0xED, 0x57, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // LD A, R
        {"LD", 2, {ACCUMULATOR, MEMREFRSH_REGISTER}, 2, {0xED, 0x5F, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // LD, I, A
        {"LD", 2, {INTVECT_REGISTER, ACCUMULATOR}, 2, {0xED, 0x47, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // LD R, A
        {"LD", 2, {MEMREFRSH_REGISTER, ACCUMULATOR}, 2, {0xED, 0x4F, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // LD dd, nn
        {"LD", 2, {BC_REGISTER, VALUE_8_BIT}, 3, {0x01, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NONE_AFFECTED, NA}},
        {"LD", 2, {DE_REGISTER, VALUE_8_BIT}, 3, {0x11, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NONE_AFFECTED, NA}},
        {"LD", 2, {HL_REGISTER, VALUE_8_BIT}, 3, {0x21, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NONE_AFFECTED, NA}},
        {"LD", 2, {SP_REGISTER, VALUE_8_BIT}, 3, {0x31, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NONE_AFFECTED, NA}},
        {"LD", 2, {BC_REGISTER, VALUE_16_BIT}, 3, {0x01, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS |
          _16BITVAL | HBYTE, NA}},
        {"LD", 2, {DE_REGISTER, VALUE_16_BIT}, 3, {0x11, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS |
          _16BITVAL | HBYTE, NA}},
        {"LD", 2, {HL_REGISTER, VALUE_16_BIT}, 3, {0x21, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS |
          _16BITVAL | HBYTE, NA}},
        {"LD", 2, {SP_REGISTER, VALUE_16_BIT}, 3, {0x31, 0x00, 0x00, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 | ALLBITS | _16BITVAL |
          HBYTE, NA}},

        // LD IX, nn
        {"LD", 2, {IX_REGISTER, VALUE_8_BIT}, 4, {0xDD, 0x21, 0x00, 0x00},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"LD", 2, {IX_REGISTER, VALUE_16_BIT}, 4, {0xDD, 0x21, 0x00, 0x00},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 |
          ALLBITS | _16BITVAL | HBYTE}},

        // LD IY, nn
        {"LD", 2, {IY_REGISTER, VALUE_8_BIT}, 4, {0xFD, 0x21, 0x00, 0x00},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NONE_AFFECTED}},
        {"LD", 2, {IY_REGISTER, VALUE_16_BIT}, 4, {0xFD, 0x21, 0x00, 0x00},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 |
          ALLBITS | _16BITVAL | HBYTE}},

        // LD dd, (nn)
        {"LD", 2, {BC_REGISTER, MEMORY_16_BIT}, 4, {0xED, 0x4B, 0x00, 0x00}, 
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 |
          ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {DE_REGISTER, MEMORY_16_BIT}, 4, {0xED, 0x5B, 0x00, 0x00}, 
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 |
          ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {HL_REGISTER, MEMORY_16_BIT}, 4, {0xED, 0x6B, 0x00, 0x00}, 
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 |
          ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {SP_REGISTER, MEMORY_16_BIT}, 4, {0xED, 0x7B, 0x00, 0x00}, 
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 |
          ALLBITS | _16BITVAL | HBYTE}},
        
        // LD IX, (nn)
        {"LD", 2, {IX_REGISTER, MEMORY_16_BIT}, 4, {0xDD, 0x2A, 0x00, 0x00},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 |
          ALLBITS | _16BITVAL | HBYTE}},

        // LD IY, (nn)
        {"LD", 2, {IY_REGISTER, MEMORY_16_BIT}, 4, {0xFD, 0x2A, 0x00, 0x00},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _16BITVAL | LBYTE, OP2 |
          ALLBITS | _16BITVAL | HBYTE}},

        // LD (nn), dd
        {"LD", 2, {MEMORY_16_BIT, BC_REGISTER}, 4, {0xED, 0x43, 0x00, 0x00}, 
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS |
          _16BITVAL | LBYTE, OP1 | ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {MEMORY_16_BIT, DE_REGISTER}, 4, {0xED, 0x53, 0x00, 0x00}, 
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE,
          OP1 | ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {MEMORY_16_BIT, HL_REGISTER}, 4, {0xED, 0x63, 0x00, 0x00}, 
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE,
          OP1 | ALLBITS | _16BITVAL | HBYTE}},
        {"LD", 2, {MEMORY_16_BIT, SP_REGISTER}, 4, {0xED, 0x73, 0x00, 0x00}, 
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE,
          OP1 | ALLBITS | _16BITVAL | HBYTE}},

        // LD (nn), IX
        {"LD", 2, {MEMORY_16_BIT, IX_REGISTER}, 4, {0xDD, 0x22, 0x00, 0x00},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE, OP1 |
          ALLBITS | _16BITVAL | HBYTE}},

        // LD (nn), IY
        {"LD", 2, {MEMORY_16_BIT, IY_REGISTER}, 4, {0xFD, 0x22, 0x00, 0x00},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _16BITVAL | LBYTE, OP1 |
          ALLBITS | _16BITVAL | HBYTE}},
        
        // LD SP, HL
        {"LD", 2, {SP_REGISTER, HL_REGISTER}, 1, {0xF9, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // LD SP, IX
        {"LD", 2, {SP_REGISTER, IX_REGISTER}, 2, {0xDD, 0xF9, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // LD, SP, IY
        {"LD", 2, {SP_REGISTER, IY_REGISTER}, 2, {0xFD, 0xF9, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // LDI
        {"LDI", 0, {NONE, NONE}, 2, {0xED, 0xA0, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // LDIR
        {"LDIR", 0, {NONE, NONE}, 2, {0xED, 0xB0, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // LDD
        {"LDD", 0, {NONE, NONE}, 2, {0xED, 0xA8, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // LDDR
        {"LDDR", 0, {NONE, NONE}, 2, {0xED, 0xB8, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};


instruction_parameters_t n_instructions[] = 
{
        // NEG
        {"NEG", 0, {NONE, NONE}, 2, {0xED, 0x44, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // NOP
        {"NOP", 0, {NONE, NONE}, 1, {0x00, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t o_instructions[] = 
{
        // OR A, r
        {"OR", 2, {ACCUMULATOR, ACCUMULATOR}, 1, {0xB7, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"OR", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0xB0, NA, NA, NA},
         {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},

        // OR A, n
        {"OR", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xF6, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // OR A, (HL)
        {"OR", 2, {ACCUMULATOR, HL_REGISTER_MEMREF}, 1, {0xB6, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // OR A, (IX + d)
        {"OR", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0xB6, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // OR A, (IX + d)
        {"OR", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0xB6, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // OUT n, A
        {"OUT", 2, {VALUE_8_BIT, ACCUMULATOR}, 2, {0xD3, 0x00, NA, NA},
         {NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NA, NA}},

        // OUT (C), r
        {"OUT", 2, {C_REGISTER_MEMREF, ACCUMULATOR}, 2, {0xED, 0x79, NA, NA},
         {NONE_AFFECTED, OP2 | _3BITS | SHIFT_3X, NA, NA}},        
        {"OUT", 2, {C_REGISTER_MEMREF, REGISTER_8_BIT}, 2, {0xED, 0x41, NA, NA},
         {NONE_AFFECTED, OP2 | _3BITS | SHIFT_3X, NA, NA}},

        // OUTI
        {"OUTI", 0, {NONE, NONE}, 2, {0xED, 0xA3, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // OTIR
        {"OTIR", 0, {NONE, NONE}, 2, {0xED, 0xB3, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // OUTD
        {"OUTD", 0, {NONE, NONE}, 2, {0xED, 0xAB, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // OTDR
        {"OTDR", 0, {NONE, NONE}, 2, {0xED, 0xBB, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};


instruction_parameters_t p_instructions[] = 
{
        // PUSH qq
        {"PUSH", 1, {BC_REGISTER, NONE}, 1, {0xC5, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"PUSH", 1, {DE_REGISTER, NONE}, 1, {0xD5, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"PUSH", 1, {HL_REGISTER, NONE}, 1, {0xE5, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"PUSH", 1, {AF_REGISTER, NONE}, 1, {0xF5, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        
        // PUSH IX
        {"PUSH", 1, {IX_REGISTER, NONE}, 2, {0xDD, 0xE5, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // PUSH IY
        {"PUSH", 1, {IY_REGISTER, NONE}, 2, {0xFD, 0xE5, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // POP qq
        {"POP", 1, {BC_REGISTER, NONE}, 1, {0xC1, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"POP", 1, {DE_REGISTER, NONE}, 1, {0xD1, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"POP", 1, {HL_REGISTER, NONE}, 1, {0xE1, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"POP", 1, {AF_REGISTER, NONE}, 1, {0xF1, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // POP IX
        {"POP", 1, {IX_REGISTER, NONE}, 2, {0xDD, 0xE1, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // POP IY
        {"POP", 1, {IY_REGISTER, NONE}, 2, {0xFD, 0xE1, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t r_instructions[] =
{
        // RLCA
        {"RLCA", 0, {NONE, NONE}, 1, {0x07, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},

        // RLA
        {"RLA", 0, {NONE, NONE}, 1, {0x17, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},

        // RRCA
        {"RRCA", 0, {NONE, NONE}, 1, {0x0F, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},

        // RRA
        {"RRA", 0, {NONE, NONE}, 1, {0x1F, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},

        // RLC r
        {"RLC", 1, {ACCUMULATOR, NONE}, 2, {0xCB, 0x07, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"RLC", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x00, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},

        // RLC (HL)
        {"RLC", 1, {HL_REGISTER_MEMREF, NONE}, 2, {0xCB, 0x06, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // RLC (IX + d)
        {"RLC", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x06},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // RLC (IY + d)
        {"RLC", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x06},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // RL r
        {"RL", 1, {ACCUMULATOR, NONE}, 2, {0xCB, 0x17, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"RL", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x10, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},

        // RL (HL)
        {"RL", 1, {HL_REGISTER_MEMREF, NONE}, 2, {0xCB, 0x16, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // RL (IX + d)
        {"RL", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x16},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // RL (IY + d)
        {"RL", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x16},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // RRC r
        {"RRC", 1, {ACCUMULATOR, NONE}, 2, {0xCB, 0x0F, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"RRC", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x08, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},

        // RRC (HL)
        {"RRC", 1, {HL_REGISTER_MEMREF, NONE}, 2, {0xCB, 0x0E, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // RRC (IX + d)
        {"RRC", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x0E},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // RRC (IY + d)
        {"RRC", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x0E},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // RR r
        {"RR", 1, {ACCUMULATOR, NONE}, 2, {0xCB, 0x1F, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"RR", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x18, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},

        // RR (HL)
        {"RR", 1, {HL_REGISTER_MEMREF, NONE}, 2, {0xCB, 0x1E, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // RR (IX + d)
        {"RR", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x1E},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // RR (IY + d)
        {"RR", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x1E},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // RLD
        {"RLD", 0, {NONE, NONE}, 2, {0xED, 0x6F, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // RRD
        {"RRD", 0, {NONE, NONE}, 2, {0xED, 0x67, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // RES b, r
        {"RES", 2, {BIT, ACCUMULATOR}, 2, {0xCB, 0x87, NA, NA},
         {NONE_AFFECTED, BOTH_OPS | _6BITS | OP1_OP2, NA, NA}},        
        {"RES", 2, {BIT, REGISTER_8_BIT}, 2, {0xCB, 0x80, NA, NA},
         {NONE_AFFECTED, BOTH_OPS | _6BITS | OP1_OP2, NA, NA}},

        // RES b, (HL)
        {"RES", 2, {BIT, HL_REGISTER_MEMREF}, 2, {0xCB, 0x86, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_3X, NA, NA}},

        // RES b, (IX + d)
        {"RES", 2, {BIT, IX_REGISTER_WOFFSET}, 4, {0xDD, 0xCB, 0x00, 0x86},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS |
          _8BITVAL, OP1 | _3BITS | SHIFT_3X}},

        // RES b, (IY + d)
        {"RES", 2, {BIT, IY_REGISTER_WOFFSET}, 4, {0xFD, 0xCB, 0x00, 0x86},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS |
          _8BITVAL, OP1 | _3BITS | SHIFT_3X}},

        // RET
        {"RET", 0, {NONE, NONE}, 1, {0xC9, NA, NA, NA}, {NONE_AFFECTED, NA, NA, NA}},

        // RET cc
        {"RET", 1, {CONDITION, NONE}, 1, {0xC0, NA, NA, NA},
         {OP1 | _3BITS | SHIFT_3X, NA, NA, NA}},

        // RETI
        {"RETI", 0, {NONE, NONE}, 2, {0xED, 0x4D, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // RETN
        {"RETN", 0, {NONE, NONE}, 2, {0xED, 0x45, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // RST p
        {"RST", 0, {VALUE_8_BIT, NONE}, 1, {0xC7, NA, NA, NA},
         {OP1 | _3BITS | SHIFT_3X, NA, NA, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t s_instructions[] = 
{
        // SUB A, r
        {"SUB", 2, {ACCUMULATOR, ACCUMULATOR}, 1, {0x97, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"SUB", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0x90, NA, NA, NA},
         {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},

        // SUB A, n
        {"SUB", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xD6, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // SUB A, (HL)
        {"SUB", 2, {ACCUMULATOR, HL_REGISTER_MEMREF}, 1, {0x96, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // SUB A, (IX + d)
        {"SUB", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0x96, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // SUB A, (IY + d)
        {"SUB", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0x96, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // SBC A, r
        {"SBC", 2, {ACCUMULATOR, ACCUMULATOR}, 1, {0x9F, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"SBC", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0x98, NA, NA, NA},
         {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},

        // SBC A, n
        {"SBC", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xDE, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // SBC A, (HL)
        {"SBC", 2, {ACCUMULATOR, HL_REGISTER_MEMREF}, 1, {0x9E, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // SBC A, (IX + d)
        {"SBC", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0x9E, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // SBC A, (IY + d)
        {"SBC", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0x9E, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // SCF
        {"SCF", 0, {NONE, NONE}, 1, {0x37, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // SBC HL, ss
        {"SBC", 2, {HL_REGISTER, BC_REGISTER}, 2, {0xED, 0x42, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"SBC", 2, {HL_REGISTER, DE_REGISTER}, 2, {0xED, 0x52, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"SBC", 2, {HL_REGISTER, HL_REGISTER}, 2, {0xED, 0x62, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"SBC", 2, {HL_REGISTER, SP_REGISTER}, 2, {0xED, 0x72, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // SLA r
        {"SLA", 1, {ACCUMULATOR, NONE}, 2, {0xCB, 0x27, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},
        {"SLA", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x20, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},

        // SLA (HL)
        {"SLA", 1, {HL_REGISTER_MEMREF, NONE}, 2, {0xCB, 0x26, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // SLA (IX + d)
        {"SLA", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x26},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // SLA (IY + d)
        {"SLA", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x26},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // SRA r
        {"SRA", 1, {ACCUMULATOR, NONE}, 2, {0xCB, 0x2F, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},        
        {"SRA", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x28, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},

        // SRA (HL)
        {"SRA", 1, {HL_REGISTER_MEMREF, NONE}, 2, {0xCB, 0x2E, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // SRA (IX + d)
        {"SRA", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x2E},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // SRA (IY + d)
        {"SRA", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x2E},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // SRL r
        {"SRL", 1, {ACCUMULATOR, NONE}, 2, {0xCB, 0x3F, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},        
        {"SRL", 1, {REGISTER_8_BIT, NONE}, 2, {0xCB, 0x38, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_0X, NA, NA}},

        // SRL (HL)
        {"SRL", 1, {HL_REGISTER_MEMREF, NONE}, 2, {0xCB, 0x3E, NA, NA},
         {NONE_AFFECTED, NONE_AFFECTED, NA, NA}},

        // SRL (IX + d)
        {"SRL", 1, {IX_REGISTER_WOFFSET, NONE}, 4, {0xDD, 0xCB, 0x00, 0x3E},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // SRL (IY + d)
        {"SRL", 1, {IY_REGISTER_WOFFSET, NONE}, 4, {0xFD, 0xCB, 0x00, 0x3E},
         {NONE_AFFECTED, NONE_AFFECTED, OP1 | ALLBITS | _8BITVAL, NONE_AFFECTED}},

        // SET b, r
        {"SET", 2, {BIT, ACCUMULATOR}, 2, {0xCB, 0xC7, NA, NA},
         {NONE_AFFECTED, BOTH_OPS | _6BITS | OP1_OP2, NA, NA}},        
        {"SET", 2, {BIT, REGISTER_8_BIT}, 2, {0xCB, 0xC0, NA, NA},
         {NONE_AFFECTED, BOTH_OPS | _6BITS | OP1_OP2, NA, NA}},

        // SET b, (HL)
        {"SET", 2, {BIT, HL_REGISTER_MEMREF}, 2, {0xCB, 0xC6, NA, NA},
         {NONE_AFFECTED, OP1 | _3BITS | SHIFT_3X, NA, NA}},

        // SET b, (IX + d)
        {"SET", 2, {BIT, IX_REGISTER_WOFFSET}, 4, {0xDD, 0xCB, 0x00, 0xC6},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS |
          _8BITVAL, OP1 | _3BITS | SHIFT_3X}},

        // SET b, (IY + d)
        {"SET", 2, {BIT, IY_REGISTER_WOFFSET}, 4, {0xFD, 0xCB, 0x00, 0xC6},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS |
          _8BITVAL, OP1 | _3BITS | SHIFT_3X}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t x_instructions[] = 
{
        // XOR A, r
        {"XOR", 2, {ACCUMULATOR, ACCUMULATOR}, 1, {0xAF, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},
        {"XOR", 2, {ACCUMULATOR, REGISTER_8_BIT}, 1, {0xA8, NA, NA, NA},
         {OP2 | _3BITS | SHIFT_0X, NA, NA, NA}},

        // XOR A, r
        {"XOR", 2, {ACCUMULATOR, VALUE_8_BIT}, 2, {0xEE, 0x00, NA, NA},
         {NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA, NA}},

        // XOR A, (HL)
        {"XOR", 2, {ACCUMULATOR, HL_REGISTER_MEMREF}, 1, {0xAE, NA, NA, NA},
         {NONE_AFFECTED, NA, NA, NA}},

        // XOR A, (IX + d)
        {"XOR", 2, {ACCUMULATOR, IX_REGISTER_WOFFSET}, 3, {0xDD, 0xAE, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},

        // XOR A, (IY + d)
        {"XOR", 2, {ACCUMULATOR, IY_REGISTER_WOFFSET}, 3, {0xFD, 0xAE, 0x00, NA},
         {NONE_AFFECTED, NONE_AFFECTED, OP2 | ALLBITS | _8BITVAL, NA}},
        {NULL, 0, {0, 0}, 0, {0, 0, 0, 0}}
};

instruction_parameters_t *instruction_set[26] = {a_instructions, b_instructions,
                                                 c_instructions, d_instructions,
                                                 e_instructions, NULL, NULL,
                                                 h_instructions, i_instructions,
                                                 j_instructions, NULL, l_instructions,
                                                 NULL, n_instructions, o_instructions,
                                                 p_instructions, NULL, r_instructions,
                                                 s_instructions,  NULL, NULL, NULL,
                                                 NULL, x_instructions, NULL, NULL};

symboltable_t z80_symbols[] = {
        {"A", ACCUMULATOR, 1, {7, NA}, DEFINED},
        {"B", REGISTER_8_BIT, 1, {0, NA}, DEFINED}, 
        {"C", REGISTER_8_BIT, 1, {1, NA}, DEFINED},
        {"D", REGISTER_8_BIT, 1, {2, NA}, DEFINED},
        {"E", REGISTER_8_BIT, 1, {3, NA}, DEFINED},
        {"H", REGISTER_8_BIT, 1, {4, NA}, DEFINED},
        {"L", REGISTER_8_BIT, 1, {5, NA}, DEFINED},
        {"AF", AF_REGISTER, 1, {3, NA}, DEFINED},
        {"AF'", _AF_REGISTER, 0, {NA, NA}, DEFINED},
        {"BC", BC_REGISTER, 1, {0, NA}, DEFINED},
        {"DE", DE_REGISTER, 1, {1, NA}, DEFINED},
        {"HL", HL_REGISTER, 1, {2, NA}, DEFINED},
        {"(HL)", HL_REGISTER_MEMREF, 0, {NA, NA}, DEFINED},
        {"(BC)", BC_REGISTER_MEMREF, 0, {NA, NA}, DEFINED},
        {"(DE)", DE_REGISTER_MEMREF, 0, {NA, NA}, DEFINED},
        {"(SP)", SP_REGISTER_MEMREF, 0, {NA, NA}, DEFINED},
        {"(C)", C_REGISTER_MEMREF, 0, {NA, NA}, DEFINED},
        {"I", INTVECT_REGISTER, 0, {NA, NA}, DEFINED},
        {"R", MEMREFRSH_REGISTER, 0, {NA, NA}, DEFINED},
        {"SP", SP_REGISTER, 1, {3, NA}, DEFINED},
        {"CC", CARRY_SET, 0, {NA, NA}, DEFINED},
        {"NC", CARRY_NOTSET, 0, {NA, NA}, DEFINED},
        {"Z", ZERO_SET, 0, {NA, NA}, DEFINED},
        {"NZ", ZERO_NOTSET, 0, {NA, NA}, DEFINED},
        {"$NZ", CONDITION, 1, {0, NA}, DEFINED},
        {"$Z", CONDITION, 1, {1, NA}, DEFINED},
        {"$NC", CONDITION, 1, {2, NA}, DEFINED},
        {"$C", CONDITION, 1, {3, NA}, DEFINED},
        {"$PO", CONDITION, 1, {4, NA}, DEFINED},
        {"$PE", CONDITION, 1, {5, NA}, DEFINED},
        {"$P", CONDITION, 1, {6, NA}, DEFINED},
        {"$M", CONDITION, 1, {7, NA}, DEFINED},
        {"IX", IX_REGISTER, 0, {NA, NA}, DEFINED},
        {"IY", IY_REGISTER, 0, {NA, NA}, DEFINED},
        {"B0", BIT, 1, {0, NA}, DEFINED},
        {"B1", BIT, 1, {1, NA}, DEFINED},
        {"B2", BIT, 1, {2, NA}, DEFINED},
        {"B3", BIT, 1, {3, NA}, DEFINED},
        {"B4", BIT, 1, {4, NA}, DEFINED},
        {"B5", BIT, 1, {5, NA}, DEFINED},
        {"B6", BIT, 1, {6, NA}, DEFINED},
        {"B7", BIT, 1, {7, NA}, DEFINED},
        {NULL, 0, 0, 0}
};

symboltable_t *symboltable_list;

#endif 
