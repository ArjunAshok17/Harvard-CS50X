/**
 * Program Description
 * Defines functions useful for matrix manipulation in regards to the Paint program canvas
*/

#include "canvas_manipulation.h"

/**
 * Creates/reallocates memory for the canvas given the already specified number
 * @param c: the canvas to adjust
 * @param num_rows: the number of rows to allocate
 * @param num_cols: the number of columns to allocate
 * @modifies: c
 * @returns void
*/
void resize_canvas(Canvas *c, int num_rows, int num_cols)
{
    // update dimensions //
    c->rows = num_rows;
    c->cols = num_cols;

    // allocate space //
    c->matrix = (char**) realloc(c->matrix, num_rows * sizeof(char*));
    for (int i = 0; i < num_rows; i++)
    {
        c->matrix[i] = (char*) realloc(c->matrix[i], num_cols * sizeof(char));
    }

    // end function //
    return;
}

/**
 * Frees all the memory allocated to the canvas
 * @param c: canvas being freed from memory
 * @modifies: c
 * @returns nothing
*/
void free_memory(Canvas *c)
{
    // free matrix //
    for (int i = 0, rows = c->rows; i < rows; i++)
    {
        free(c->matrix[i]);
        c->matrix[i] = NULL;
    }
    free(c->matrix);
    c->matrix = NULL;

    // free canvas //
    free(c);
    c = NULL;

    // end function //
    return;
}

/**
 * Fills the canvas with the empty character
 * @param c: the canvas to fill
 * @modifies: c
 * @returns nothing
*/
void fill_canvas(Canvas *c)
{
    // variable declaration //
    int rows = c->rows;
    int cols = c->cols;

    // fill //
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // empty char //
            c->matrix[i][j] = '*';
        }
    }

    // end function //
    return;
}

/**
 * Prints out the canvas with row and column numbers; prints upside down to avoid input conversion
 * @param c: the canvas to print
 * @modifies: nothing
 * @returns nothing
*/
void print_canvas(Canvas *c)
{
    // variable declaration //
    int rows = c->rows, cols = c->cols;

    // for each row //
    for (int i = rows - 1; i >= 0; i--)
    {
        // print row number //
        printf("%i ", i);
        
        // for each element in the row //
        for (int j = 0; j < cols; j++)
        {
            printf("%c ", c->matrix[i][j]);
        }
        printf("\n");
    }

    // print column numbers //
    printf(" ");
    for (int i = 0; i < cols; i++)
    {
        printf(" %i", i);
    }
    printf("\n");

    // end function //
    return;
}

/**
 * Draws a line given the conditions inferred from user input
 * @param c: canvas to draw on
 * @param row: starting row
 * @param col: start column
 * @param path: character to draw with
 * @param num_paths: number of strokes to perform
 * @param dir_mod_r: direction modifier for rows
 * @param dir_mod_c: direction modifier for columns
 * @modifies: c
 * @returns nothing
*/
void write_canvas(Canvas *c, int row, int col, char path, int num_paths, int dir_mod_r, int dir_mod_c)
{
    // draw line //
    for (int i = 0, cur_r, cur_c; i < num_paths; i++)
    {
        // set vars //
        cur_r = row + dir_mod_r * i;
        cur_c = col + dir_mod_c * i;

        // check intersection //
        if (c->matrix[cur_r][cur_c] != '*' &&
            c->matrix[cur_r][cur_c] != path)
        {
            c->matrix[cur_r][cur_c] = '+';
        }
        else
        {
            c->matrix[cur_r][cur_c] = path;
        }
    }

    // end function //
    return;
}

/**
 * Erases the specified range with the empty character; range is defined with top-left & bottom-right corners
 * @param c: the canvas to erase on
 * @param r1: the top-left anchor point row
 * @param c1: the top-left anchor point column
 * @param r2: the bottom-right anchor point row
 * @param c2: the bottom-right anchor point column
 * @modifies: c
 * @returns nothing
*/
void reset_range(Canvas *c, int r1, int c1, int r2, int c2)
{
    // iterate through range
    for (int i = r1; i <= r2; i++)
    {
        for (int j = c1; j <= c2; j++)
        {
            // delete char
            c->matrix[i][j] = '*';
        }
    }

    // end function //
    return;
}

/**
 * Shifts all values from the specified row position and above/below
 * @param c: canvas to modify
 * @param pos: row position to clear up
 * @param dir: direction to shift (1 for down, -1 for up)
 * @modifies: c
 * @returns nothing
*/
void shift_canvas_row(Canvas *c, int pos, int dir)
{
    if (dir == 1)
    {
        // shift elements down //
        for (int i = c->rows - 1; i > pos; i--)
        {
            for (int j = c->cols - 1; j >= 0; j--)
            {
                // copy from previous row
                c->matrix[i][j] = c->matrix[i - 1][j];
            }
        }
    }
    else
    {
        // shift elements up //
        for (int i = pos; i < c->rows - 1; i++)
        {
            for (int j = c->cols - 1; j >= 0; j--)
            {
                // copy from previous row
                c->matrix[i][j] = c->matrix[i + 1][j];
            }
        }
    }

    // end function //
    return;
}

/**
 * Shifts all values from the specified column position and right/left
 * @param c: canvas to modify
 * @param pos: column position to clear up
 * @param dir: direction to shift (1 for right, -1 for left)
 * @modifies: c
 * @returns nothing
*/
void shift_canvas_col(Canvas *c, int pos, int dir)
{
    if (dir == 1)
    {
        // shift elements right //
        for (int j = c->cols - 1; j >= pos; j--)
        {
            for (int i = c->rows - 1; i >= 0; i--)
            {
                // copy from previous column
                c->matrix[i][j] = c->matrix[i][j - 1];
            }
        }
    }
    else
    {
        // shift elements left //
        for (int j = pos; j < c->cols - 1; j++)
        {
            for (int i = 0; i < c->rows; i++)
            {
                // copy from next column
                c->matrix[i][j] = c->matrix[i][j + 1];
            }
        }
    }

    // end function //
    return;
}

/**
 * Saves the canvas to a file using the format: first line is "rows columns"
 *                                              every row is printed on a new line after
 * @param c: canvas to print to file
 * @param file_name: name of the file to print to
 * @modifies: nothing
 * @returns nothing
*/
void save_canvas(Canvas* c, char* file_name)
{
    // variable declaration //
    FILE* writer = NULL;
    int rows = c->rows, cols = c->cols;

    // file io //
    writer = fopen(file_name, "w");

    // dimensions //
    fprintf(writer, "%i %i\n", rows, cols);

    // canvas matrix //
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols - 1; j++)
        {
            fprintf(writer, "%c ", c->matrix[rows - 1 - i][j]);
        }
        fprintf(writer, "%c\n", c->matrix[rows - 1 - i][cols - 1]);
    }

    // end function //
    fclose(writer);
    return;
}

/**
 * Loads the canvas to the canvas using the save format
 * @param c: canvas to read file data into
 * @param file_name: name of the file to read in
 * @modifies: c
 * @returns nothing
*/
void load_canvas(Canvas* c, char* file_name)
{
    // variable declaration //
    FILE* reader = NULL;
    int rows, cols;

    // file io //
    reader = fopen(file_name, "r");

    // read dimensions //
    fscanf(reader, "%i %i", &rows, &cols);

    // resize & reset canvas //
    resize_canvas(c, rows, cols);
    fill_canvas(c);

    // canvas matrix //
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            fscanf(reader, " %c", &(c->matrix[rows - 1 - i][j]));
        }
    }

    // end function //
    fclose(reader);
    return;
}