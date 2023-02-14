#ifndef CANVAS_MANIPULATION_H
    #define CANVAS_MANIPULATION_H

    // libraries //
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>

    // struct & enums //
    typedef struct Canvas_struct {
        int rows; // number of rows
        int cols; // number of columns
        char** matrix; // canvas represented as char matrix
    } Canvas;

    // memory allocation //
    void resize_canvas(Canvas *c, int num_rows, int num_cols);
    void free_memory(Canvas *c);

    // helper functions //
    void fill_canvas(Canvas *c);
    void print_canvas(Canvas *c);

    // command helpers //
    void write_canvas(Canvas *c, int row, int col, char path, int num_paths, int dir_mod_r, int dir_mod_c);

    void shift_canvas_row(Canvas *c, int pos, int dir);
    void shift_canvas_col(Canvas *c, int pos, int dir);
    void reset_range(Canvas *c, int r1, int c1, int r2, int c2);

    void save_canvas(Canvas* c, char* file_name);
    void load_canvas(Canvas* c, char* file_name);

#endif