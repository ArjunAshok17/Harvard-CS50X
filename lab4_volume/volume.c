// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

// structs
typedef uint8_t BYTE;
typedef int16_t SAMPLE;

int main(int argc, char *argv[])
{
    // check cl-args //
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // file io & header info //
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // copy header to output //
    BYTE *header = (BYTE *) malloc(HEADER_SIZE * sizeof(BYTE));
    fread(header, 1, 44, input);
    fwrite(header, 1, 44, output);

    // adjust and transfer sample //
    SAMPLE buffer;
    while (fread(&buffer, sizeof(SAMPLE), 1, input) == 1)
    {
        // adjust output //
        buffer *= factor;

        // export to file //
        fwrite(&buffer, sizeof(SAMPLE), 1, output);
    }

    // end program //
    fclose(input);
    fclose(output);
    free(header);

    return 0;
}
