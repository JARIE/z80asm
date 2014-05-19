// File: task.h
// Author: Jarielle Catbagan
// Created: 18, May 2014

#include <stdint.h>
#include "defines.h"

#ifndef TASK_H
#define TASK_H

void init_symboltable(symboltable_t **symboltable_list, symboltable_t *defined_symbols,
                      uint16_t *symboltable_currentsize, uint16_t *symboltable_actualsize);

void free_symboltable(symboltable_t **symboltable_list);

#endif
