// Created: 24, May 2014

void assemble_instruction(FILE *infile_handle, FILE *outfile_handle, char *instruction,
                          instruction_parameters_t **instruction_set,
                          symboltable_t *symboltable_list,
                          uint8_t symboltable_currentsize, line_status_t *line_status);

void retrieve_opcharac(char *operand, uint8_t *operand_type, uint8_t *operand_valuelength,
                       uint8_t operand_value[], symboltable_t *symboltable_list,
                       uint8_t symboltable_currentsize);

void assemble(FILE *outfile, instruction_parameters_t **instruction_set,
              char *instruction, uint8_t operand1_type, uint8_t operand2_type,
              uint8_t operand1_valuelength, uint8_t operand2_valuelength,
              uint8_t operand1_value[], uint8_t operand2_value[]);
