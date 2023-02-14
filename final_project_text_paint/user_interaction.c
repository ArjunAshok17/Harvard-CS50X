/**
 * Program Description
 * Defines functions useful for use interactions, especially with receiving user input
*/

#include "user_interaction.h"

/**
 * This function prints the help menu for users to reference
 * @modifies: nothing
 * @returns nothing
*/
void print_help(void)
{
    printf("Commands:\n");
    printf("Help: h\n");
    printf("Quit: q\n");
    printf("Draw line: w row_start col_start row_end col_end\n");
    printf("Resize: r num_rows num_cols\n");
    printf("Add row or column: a [r | c] pos\n");
    printf("Delete row or column: d [r | c] pos\n");
    printf("Erase: e row col\n");
    printf("Save: s file_name\n");
    printf("Load: l file_name\n");
}

/**
 * This function prints the corresponding error message for each error code
 * @param error_code: error code; 0 => argc error, 1 => command error, 2 => non-integer row, 3 => invalid row
 *                                4 => non-integer column, 5 => invalid column, 6 => invalid draw command
 *                                7 => invalid resize command, 8 => invalid erase command, 9 => invalid add command
 *                                10 => invalid delete command, 11 => failed to save, 12 => failed to load
 * @modifies: nothing
 * @returns nothing
*/
void print_error(int error_code)
{
    // route error code //
    switch (error_code)
    {
        case 0:
            printf("Wrong number of command line arguments entered.\n");
            printf("Usage: ./paint.out [num_rows num_cols]\n");
            printf("Making default board of 10 X 10.\n");
            break;
        
        case 1:
            printf("Unrecognized command. Type h for help.\n");
            break;
        
        case 2:
            printf("The number of rows is not an integer.\n");
            break;
            
        case 3:
            printf("The number of rows is less than 1.\n");
            printf("Making default board of 10 X 10.\n");
            break;
        
        case 4:
            printf("The number of columns is not an integer.\n");
            break;
        
        case 5:
            printf("The number of columns is less than 1.\n");
            printf("Making default board of 10 X 10.\n");
            break;
        
        case 6:
            printf("Improper draw command.\n");
            break;
        
        case 7:
            printf("Improper resize command.\n");
            break;
        
        case 8:
            printf("Improper erase command.\n");
            break;
        
        case 9:
            printf("Improper add command.\n");
            break;
        
        case 10:
            printf("Improper delete command.\n");
            break;

        case 11:
            printf("Improper save command or file could not be created.\n");
            break;

        case 12:
            printf("Improper load command or file could not be opened.\n");
            break;

        default:
            printf("INVALID ERROR CODE.\n");
    }

    // end function //
    return;
}

/**
 * This function reads in a command from the user w/ input validation
 * @modifies: nothing
 * @returns true if valid command, false if otherwise
*/
bool read_command(char *command)
{
    // variable declaration //
    *command = '\0'; // default value

    // read input //
    printf("Enter your command: ");
    scanf(" %c", command);

    // error handling //
    if (is_valid_command(*command))
    {
        return true;
    }
    
    // error message & clear buffer
    print_error(1);
    while((getchar()) != '\n');

    // end function //
    return false;
}

/**
 * This function takes in command-line arguments and outputs the rows and columns wanted (or defaults to ten)
 * @param argc: number of command-line args
 * @param argv: vector of command-line arguments
 * @param rows: pointer to space for number of canvas rows
 * @param cols: pointer to space for number of canvas cols
 * @modifies: rows, cols
 * @returns nothing
*/
void cl_input(int argc, char** argv, int *rows, int *cols)
{
    // number of inputs check //
    if (argc == 1)
    {
        // default value //
        *rows = 10;
        *cols = 10;

        // end function //
        return;
    }

    if (argc != 3)
    {
        // user output //
        print_error(0);

        // default value //
        *rows = 10;
        *cols = 10;

        // end function //
        return;
    }

    // read input //
    sscanf(argv[1], " %i", rows);
    sscanf(argv[2], " %i", cols);

    // error check //
	if(*rows < 1)
    {
	    print_error(3);
        *rows = 10;
	    *cols = 10;
	}
	else if(*cols < 1)
    {
        print_error(5);
        *rows = 10;
	    *cols = 10;
	}

    // end function //
    return;
}

/**
 * This function determines whether a command is valid or not
 * @param command: char representing the user input
 * @modifies: nothing
 * @returns true if valid, false otherwise
*/
bool is_valid_command(char command)
{
    // route to correct function //
    switch (command)
    {
        // if valid //
        case 'q':
        case 'h':
        case 'e':
        case 'w':
        case 'a':
        case 'd':
        case 'r':
        case 's':
        case 'l':
            return true;
            break;
        
        // otherwise //
        default:
            return false;
    }

    // end function //
    return false;
}

/**
 * Gets a string to read input from between whitespace; similar return to argv
 * @param str: string to modify with the argumet read
 * @modifies: str
 * @returns nothing
*/
void get_valid_string(char* *str)
{
    // variable declaration //
    int num_char_read = 1;
    int count = 0;
    char c = '\0';

    *str = (char*) calloc(num_char_read, sizeof(char));

    // read until whitespace //
    scanf(" %c", &c);
    while (!isspace(c))
    {
        // update char array //
        num_char_read++;
        *str = realloc(*str, num_char_read);
        (*str)[count] = c;
        count++;

        // read next char //
	    scanf("%c", &c);
    }

    // replace whitespace //
    ungetc(c, stdin);

    // make string //
    (*str)[count] = '\0';

    // end function //
    return;
}

/**
 * Reads in a singular input and determines if a valid int
 * @param num: array to modify with inputs read
 * @param lower_bound: lowest acceptable value
 * @param upper_bound: highest acceptable value
 * @modifies: num
 * @returns true if valid input, false otherwise
*/
bool get_valid_int(int *num, int lower_bound, int upper_bound)
{
    // variable handling //
    int args_read;
    char whitespace;

    // get input //
    args_read = scanf(" %i%c", num, &whitespace);

    // update return //
    if (args_read == 2 && isspace(whitespace) &&
        (*num) >= lower_bound && (*num) <= upper_bound)
    {
        // replace whitespace //
        ungetc(whitespace, stdin);

        // end function //
        return true;
    }

    // clear input buffer //
    fflush(stdin);

    // end function //
    return false;
}

/**
 * Reads in a singular input and returns a valid output char if applicable
 * @param ch: char to modify if valid
 * @modifies: ch
 * @returns true if valid input, false otherwise
*/
bool get_valid_char(char *ch)
{
    // variable handling //
    int args_read;
    char whitespace;
    *ch = '\0';

    // get argument //
    args_read = scanf(" %c%c", ch, &whitespace);

    // check arg //
    if (args_read == 2 && isalpha(*ch) && isspace(whitespace))
    {
        // replace whitespace //
        ungetc(whitespace, stdin);
        
        // end function //
        return true;
    }

    // clear input buffer //
    fflush(stdin);

    // end function //
    return false;
}

/**
 * This function checks the input stream to ensure it's clear
 * @modifies: input buffer
 * @returns true if clean stream, false otherwise
*/
bool clear_stream()
{
    // variable declaration //
    bool clean = true;
    char c;

    do
    {
        // get char //
        c = getchar();

        // check space //
        if (!isspace(c))
        {
            // not clean //
            clean = false;
        }
    } while (c != '\n');

    // end function //
    return clean;
}