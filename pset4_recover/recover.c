#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t BYTE;

void get_file_name(char *file_name, int *file_num);
bool check_bytes(BYTE *buffer);

int main(int argc, char *argv[])
{
    // command-line args checking //
    // number of args checking
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // file i/o //
    FILE *raw_file = fopen(argv[1], "r");

    // error checking for command-line args
    if (raw_file == NULL)
    {
        printf("INVALID IMAGE NAME\n");
        return 2;
    }

    // variable handling //
    const int BLOCK_SIZE = 512; // define memory block size in the forensic image
    BYTE *buffer = (BYTE *) malloc(BLOCK_SIZE * sizeof(BYTE));

    int file_num = 0;
    FILE *writer = NULL;
    bool one_file = true;

    char *file_name = (char *) malloc(8 * sizeof(char));
    file_name[7] = '\0';

    // for every file in the forensic image //
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {

        // new image
        if (check_bytes(buffer))
        {
            // name control
            get_file_name(file_name, &file_num);

            // file io
            if (writer != NULL)
            {
                fclose(writer);
            }
            writer = fopen(file_name, "w");
        }

        // write to file
        if (writer != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, writer);
        }
    }

    // end program //
    free(file_name);
    free(buffer);
    fclose(writer);
    fclose(raw_file);
    return 0;
}

/**
 * Creates file_name string to store the next file and updates file number
 * @param file_name: string to store name in
 * @param file_num: file number
 * @modifies: file_name, file_num
 * @returns nothing
*/
void get_file_name(char *file_name, int *file_num)
{
    // variable handling //
    int num = *file_num;

    // print to string //
    sprintf(file_name, "%i%i%i.jpg", num / 100, num / 10 % 10, num % 10);

    // update number //
    *file_num += 1;

    // end function //
    return;
}

/**
 * Checks if the next block is a jpeg or not
 * @param buffer: buffer to read from
 * @modifies: nothing
 * @returns nothing
*/
bool check_bytes(BYTE *buffer)
{
    // byte 1 - 3 //
    if (buffer[0] != 0xff ||
        buffer[1] != 0xd8 ||
        buffer[2] != 0xff)
    {
        return false;
    }

    // byte 4 //
    if (buffer[3] - 224 < 0)
    {
        return false;
    }

    // default //
    return true;
}