/**
 * Program Description
 * Defines functions for obtaining & executing user input; acts as routing functions
*/

#include "commands.h"
#include <string.h>

/**
 * Calls the appropriate command depending on the input; routing function
 * @param c: canvas to pass onto the next function
 * @param command: user received command to execute
 * @modifies: nothing
 * @returns nothing
*/
void route_command(Canvas *c, char command)
{
    // route to correct function //
    switch (command)
    {
        case 'h':
            help();
            break;
        
        case 'w':
            write(c);
            break;

        case 'e':
            erase(c);
            break;

        case 'a':
            add(c);
            break;

        case 'd':
            delete(c);
            break;

        case 'r':
            resize(c);
            break;

        case 's':
            save(c);
            break;

        case 'l':
            load(c);
            break;
        
        // default to quit //
        default:
            quit(c);
    }

    // end function //
    return;
}

/**
 * Frees memory, exit will happen automatically in main
 * @param c: canvas to free memory from
 * @modifies: c
 * @returns nothing
*/
void quit(Canvas *c)
{
    // check input //
    if (!clear_stream())
    {
        // end function //
        print_error(1);
        return;
    }

    // free memory //
    free_memory(c);

    // end function //
    return;
}

/**
 * Handles displaying user reference information
 * @modifies: nothing
 * @returns nothing
*/
void help()
{
    // check input stream //
    if (!clear_stream())
    {
        // end function //
        print_error(1);
        return;
    }

    // user output //
    print_help();

    // end function //
    return;
}

/**
 * Gets user input on where to draw a line and determines its validity before executing it
 * @param c: canvas to write on
 * @modifies: c
 * @returns nothing
*/
void write(Canvas* c)
{
    // variable declaration //
    int r1, c1, r2, c2; // endpoints of line segment

    int num_paths = 0; // number of paths to write
    char path = '-'; // char to write with, default to horizontal

    int dir_mod_r; // direction modifier for writing
    int dir_mod_c;

    // get input //
    if (!get_valid_int(&r1, 0, c->rows - 1)) // row coord 1
    {
        // end function //
        print_error(6);
        return;
    }

    if (!get_valid_int(&c1, 0, c->cols - 1)) // col coord 1
    {
        // end function //
        print_error(6);
        return;
    }

    if (!get_valid_int(&r2, 0, c->rows - 1)) // row coord 2
    {
        // end function //
        print_error(6);
        return;
    }

    if (!get_valid_int(&c2, 0, c->cols - 1)) // col coord 2
    {
        // end function //
        print_error(6);
        return;
    }

    // check input stream //
    if (!clear_stream())
    {
        // end function //
        print_error(6);
        return;
    }

    // direction modifiers //
    if (r1 == r2)
    {
        dir_mod_r = 0;
    }
    else
    {
        dir_mod_r = r2 > r1 ? 1 : -1;
    }

    if (c1 == c2)
    {
        dir_mod_c = 0;
    }
    else
    {
        dir_mod_c = c2 > c1 ? 1 : -1;
    }

    // straight-line error check //
    if (dir_mod_r * (r2 - r1) != dir_mod_c * (c2 - c1)
        && r1 != r2
        && c1 != c2)
    {
        // show error //
        print_error(6);
        return;
    }

    // direction //
    if (dir_mod_r == 0)
    {
        path = '-';
    }
    else if (dir_mod_c == 0)
    {
        path = '|';
    }
    else if (dir_mod_r == 1)
    {
        path = dir_mod_c == 1 ? '/' : path;
        path = dir_mod_c == -1 ? '\\' : path;
    }
    else if (dir_mod_r == -1)
    {
        path = dir_mod_c == 1 ? '\\' : path;
        path = dir_mod_c == -1 ? '/' : path;
    }

    // paths //
    num_paths = dir_mod_r * (r2 - r1) >=  dir_mod_c * (c2 - c1) ?
                dir_mod_r * (r2 - r1): dir_mod_c * (c2 - c1);
    num_paths += 1;

    // writing //
    write_canvas(c, r1, c1, path, num_paths, dir_mod_r, dir_mod_c);

    // end function //
    return;
}

/**
 * Gets user input on what character to erase and revert that character to the default state
 * @param c: canvas to erase on
 * @modifies: c
 * @returns nothing
*/
void erase(Canvas* c)
{
    // variable declaration //
    int row, col;

    // user input //
    if (!get_valid_int(&row, 0, c->rows - 1))
    {
        // end function //
        print_error(8);
        return;
    }
    
    if (!get_valid_int(&col, 0, c->cols - 1))
    {
        // end function //
        print_error(8);
        return;
    }

    // check input stream //
    if (!clear_stream())
    {
        // end function //
        print_error(8);
        return;
    }

    // erase coordinate //
    reset_range(c, row, col, row, col);

    // end function //
    return;
}

/**
 * Gets and executes user input on where to insert a column or row; everything will be pushed up/right when inserted
 * @param c: canvas to modify
 * @modifies: c
 * @returns nothing
*/
void add(Canvas *c)
{
    // variable declaration //
    char dim = '\0';
    int pos;

    // user input //
    // get dimension
    if (!get_valid_char(&dim) ||
        (dim != 'c' && dim != 'r'))
    {
        // end function //
        print_error(9);
        return;
    }

    // get position
    if (dim == 'r' && !get_valid_int(&pos, 0, c->rows))
    {
        // end function //
        print_error(9);
        return;
    }

    if (dim == 'c' && !get_valid_int(&pos, 0, c->cols))
    {
        // end function //
        print_error(9);
        return;
    }

    // check input stream //
    if (!clear_stream())
    {
        // end function //
        print_error(9);
        return;
    }
    
    // reallocate space, copy elements, fill new space //
    if (dim == 'r')
    {
        resize_canvas(c, c->rows + 1, c->cols);
        shift_canvas_row(c, pos, 1);
        reset_range(c, pos, 0, pos, c->cols - 1);
    }
    else
    {
        resize_canvas(c, c->rows, c->cols + 1);
        shift_canvas_col(c, pos, 1);
        reset_range(c, 0, pos, c->rows - 1, pos);
    }

    // end function //
    return;
}

/**
 * Gets and executes user input on where to delete a column or row; everything will be pushed down/left when deleted
 * @param c: canvas to modify
 * @modifies: c
 * @returns nothing
*/
void delete(Canvas *c)
{
    // variable declaration //
    char dim = '\0';
    int pos;

    // user input //
    if (!get_valid_char(&dim) ||
        (dim != 'c' && dim != 'r'))
    {
        // end function //
        print_error(10);
        return;
    }

    // get position //
    if (dim == 'r' && !get_valid_int(&pos, 0, c->rows))
    {
        // end function //
        print_error(10);
        return;
    }

    if (dim == 'c' && !get_valid_int(&pos, 0, c->cols))
    {
        // end function //
        print_error(10);
        return;
    }

    // check input stream //
    if (!clear_stream())
    {
        // end function //
        print_error(10);
        return;
    }
    
    // shift elements & reallocate space //
    if (dim == 'r')
    {
        shift_canvas_row(c, pos, -1);
        resize_canvas(c, c->rows - 1, c->cols);
    }
    else
    {
        shift_canvas_col(c, pos, -1);
        resize_canvas(c, c->rows, c->cols - 1);
    }

    // end function //
    return;
}

/**
 * Resizes the canvas to be a user-specified size
 * @param c: canvas to modify
 * @modifies: c
 * @returns nothing
*/
void resize(Canvas *c)
{
    // variables //
    int new_rows, new_cols;

    // user input //
    if (!get_valid_int(&new_rows, 1, 2147483647))
    {
        // end function //
        print_error(7);
        return;
    }

    if (!get_valid_int(&new_cols, 1, 2147483647))
    {
        // end function //
        print_error(7);
        return;
    }

    // check input stream //
    if (!clear_stream())
    {
        // end function //
        print_error(7);
        return;
    }


    // expand & contract canvas //
    for (int i = c->rows; i < new_rows; i++)
    {
        // add row //
        resize_canvas(c, i + 1, c->cols);
        shift_canvas_row(c, i, 1);
        reset_range(c, i, 0, i, c->cols - 1);
    }

    for (int i = c->rows - 1; i >= new_rows; i--)
    {
        // delete row //
        shift_canvas_row(c, i, -1);
        resize_canvas(c, i, c->cols);
    }

    for (int i = c->cols; i < new_cols; i++)
    {
        // add column //
        resize_canvas(c, c->rows, i + 1);
        shift_canvas_col(c, i, 1);
        reset_range(c, 0, i, c->rows - 1, i);
    }

    for (int i = c->cols - 1; i >= new_cols; i--)
    {
        // delete column //
        shift_canvas_col(c, i, -1);
        resize_canvas(c, c->rows, i);
    }

    // end function //
    return;
}

/**
 * Gets and executes user input on where to save the canvas in a file
 * @param c: canvas to save
 * @modifies: nothing
 * @returns nothing
*/
void save(Canvas* c)
{
    // variable declaration //
    char* file_name = NULL;

    // user input //
    get_valid_string(&file_name);

    // check input stream //
    if (!clear_stream())
    {
        // end function //
        print_error(11);
        return;
    }

    // print to file //
    save_canvas(c, file_name);

    // end function //
    free(file_name);
    return;
}

/**
 * Gets a file name from the user and loads into paint
 * @param c: canvas to load into
 * @modifies: c
 * @returns nothing
*/
void load(Canvas* c)
{
    // variable declaration //
    char* file_name = NULL;

    // user input //
    get_valid_string(&file_name);

    // check input stream //
    if (!clear_stream())
    {
        // end function //
        print_error(12);
        return;
    }

    // print to file //
    load_canvas(c, file_name);

    // end function //
    free(file_name);
    return;
}