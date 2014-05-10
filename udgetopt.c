// File: udgetopt.c
// Author: Jarielle Catbagan
// Created: 06, May 2014

// Code:

int getopt(int argc, char *const *argv, const char *options) {
        unsigned int options_length = 0, n_options = 0, opt_index;
        int retval;
        enum status_t {EXIT = 0, CONTINUE} status;

        status = CONTINUE;

        options_length = strlen(options);

        while(status == CONTINUE) {
                if(args_index < argc) {
                        if(*argv[args_index] == '-') {
                                for(opt_index = 0; opt_index < options_length; 
                                    ++opt_index) {
                                        if(argv[args_index][1] == options[opt_index] &&
                                           status != EXIT) {
                                                retval = options[opt_index];
                                                if((opt_index + 1) < options_length) {
                                                        if(options[opt_index + 1] == ':') {
                                                                if((args_index + 1) < argc) {
                                                                        optarg = argv[args_index + 1];
                                                                        ++args_index;
                                                                }
                                                                ++opt_index;
                                                        }
                                                }
                                                status = EXIT;
                                        }
                                }
                                if(status != EXIT) {
                                        retval = '?';
                                        status = EXIT;
                                }
                        }
                        ++args_index;
                }
                else {
                        retval = -1;
                        status = EXIT;
                }
        }
        return retval;
}
