// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
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

    // TODO: Copy header from input file to output file
    // Creating an array of bytes to store the data from the input file header
    uint8_t header[HEADER_SIZE];
    // Reading the from the input into the array header created
    // Note we are reading into the address of header
    fread(header, HEADER_SIZE, 1, input);

    // write from the array header into the output file
    fwrite(header, HEADER_SIZE, 1, output);

    // TODO: Read samples from input file and write updated data to output file

    // CReating a buffer to store audio samples to be read from the input file
    int16_t buffer;

    // Reading every audio sample
    while (fread(&buffer, sizeof(int16_t), 1, input))
    {
        // Updating the volume
        buffer = factor * buffer;

        // Writing into the output file
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }


    // Close files
    fclose(input);
    fclose(output);
}
