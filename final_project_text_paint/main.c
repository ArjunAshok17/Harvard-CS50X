/**
 * Program Description
 * Emulates MS Paint using text-input
*/

#include "commands.h"
#include "canvas_manipulation.h"
#include "user_interaction.h"

int main(int argc, char** argv)
{
    // variable declaration //
    Canvas* c = (Canvas*) calloc(1, sizeof(Canvas));
    c->matrix = NULL;

    int num_rows = 10, num_cols = 10;
    char command;

    // canvas set-up //
    cl_input(argc, argv, &num_rows, &num_cols);
    resize_canvas(c, num_rows, num_cols);
    fill_canvas(c);

    // conduct program //
    do
    {
        // print canvas //
        print_canvas(c);

        // user input //
        if (!read_command(&command))
        {
            // need valid input //
            continue;
        }

        // execute input //
        route_command(c, command);
    } while (command != 'q');

    // end program //
    return 0;
}