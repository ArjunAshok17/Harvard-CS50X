#ifndef USER_INTERACTION_H
    #define USER_INTERACTION_H

    // libraries //
    #include <ctype.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    // user output //
    void print_help(void);
    void print_error(int error_code);

    // validation //
    void cl_input(int argc, char** argv, int *rows, int *cols);
    bool read_command(char *command);
    bool is_valid_command(char command);

    // get input //
    void get_valid_string(char* *str);
    bool get_valid_int(int *num, int lower_bound, int upper_bound);
    bool get_valid_char(char *ch);

    bool clear_stream(void);

#endif
