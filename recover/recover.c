#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
#define BLOCK_SIZE 512


int main(int argc, char *argv[])
{
    // Check that only one command-line argument is passed; the name of the forensic image from which to recover JPEGs
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw[name of forensic image to recover JPEGs]\n");
        return 1;
    }
    // Open memory card or file
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("could not open file\n");
        return 2;
    }
    // Imitialise image output file
    FILE *img = NULL;

    // Create buffer to read in 512 bytes
    BYTE buffer[BLOCK_SIZE];

    // Initialise count of jpeg files found
    int jpeg_found = 0;
    // Initialise string to hold each jpeg filename
    char filename[8];

    // Reading until the end of file/ mempory card
    while(fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        // If start of a new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 & buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Create JPEG file name starting with 000.jpg and open file
            sprintf(filename, "%03i.jpg", jpeg_found);
            img = fopen(filename, "w");

            // If first JPEG, recover/write into 000.jpg
            if (jpeg_found == 0)
            {
                fwrite(buffer, 1, BLOCK_SIZE, img);
            }
            // if not first JPEG, close the previous img and open a new image to write into
            else
            {
                fclose(img);
                img = fopen(filename, "w");
                fwrite(buffer, 1, BLOCK_SIZE, img);
            }
            // increment the number of jpegs found
            jpeg_found++;

        // If already found JPEG and been writing into it, continue writing
        }
        else
        {
            if (!(jpeg_found == 0))
            {
                fwrite(buffer, 1, BLOCK_SIZE, img);
            }
        }
    }
    // close final jpeg if we get to the end of the file/mwmory card
    fclose(img);
}