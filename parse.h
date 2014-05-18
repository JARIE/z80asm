// File: parse.h
// Author: Jarielle Catbagan
// Created: 17, May 2014

// Description:

/* This file contains all the necessary functions to parse the source file specified
 * from the user.
 */

#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include "defines.h"

program_status_t extract_nearestword(FILE *file_handle, char *buffer,
                                     unsigned char max_buffersize);

#endif
