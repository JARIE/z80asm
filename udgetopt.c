// File: udgetopt.c
// Created: 06, May 2014

/* Copyright (C) 2014 Jarielle Catbagan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Code:

#include <string.h>

extern char *optarg;
static int args_index = 1;

int udgetopt(int argc, char *const *argv, const char *options) {
        unsigned int options_length = 0, n_options = 0, opt_index;
        int retval;
        enum loop_status_t {EXIT = 0, CONTINUE} loop_status, subloop_status;
        enum option_status_t {INVALID = 0, VALID} option_status;
        enum argument_status_t {NO_OPTION = 0, OPTION_DETECTED} argument_status;

        loop_status = CONTINUE;

        options_length = strlen(options);

        while(loop_status == CONTINUE) {
                if(args_index < argc) 
                        if(argv[args_index][0] == '-')
                                argument_status = OPTION_DETECTED;
                        else
                                argument_status = NO_OPTION;
                else
                        return -1;

                switch(argument_status) {
                case OPTION_DETECTED:
                        subloop_status = CONTINUE;
                        option_status = INVALID;
                        opt_index = 0;
                        while(opt_index < options_length && subloop_status == CONTINUE) {
                                if(argv[args_index][1] == options[opt_index]) {
                                        retval = options[opt_index];
                                        option_status = VALID;
                                        subloop_status = EXIT;
                                }
                                else
                                        ++opt_index;
                        }
                        
                        if(option_status == VALID) {
                                if(options[opt_index + 1] == ':' && (opt_index + 1) < options_length) {
                                        if((args_index + 1) < argc) 
                                                optarg = argv[args_index + 1];
                                        else
                                                optarg = NULL;
                                        ++args_index;
                                }
                                return retval;
                        }
                        else
                                ++args_index;
                                return '?';
                        break;
                case NO_OPTION:
                        ++args_index;
                        break;
                }
                
        }
}
