#ifndef COMMANDS_H
    #define COMMANDS_H

    // libraries //
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "canvas_manipulation.h"
    #include "user_interaction.h"

    // command routing //
    void route_command(Canvas *c, char command);

    // command defining //
    // user //
    void quit(Canvas *c);
    void help();

    // painting //
    void write(Canvas* c);
    void erase(Canvas* c);

    // memory allocation //
    void add(Canvas *c);
    void delete(Canvas *c);
    void resize(Canvas *c);

    // file i/o //
    void save(Canvas* c);
    void load(Canvas* c);

#endif