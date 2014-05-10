// File: udgetopt.h
// Author: Jarielle Catbagan
// Created: 06, May 2014

// Description:

// Code:

#ifdef UDGETOPT_H
#define UDGETOPT_H

#ifdef WINDOWS

int getopt(int argc, char *const *argv, const char *options);

#else

#include <unistd.h>

#endif

#endif
