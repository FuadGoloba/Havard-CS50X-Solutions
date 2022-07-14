#include "helpers.h"
#include <stdlib.h>
#include <math.h>
#include <cs50.h>

// void swap(RGBTRIPLE *a, RGBTRIPLE *b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels, by row and column

    // for each row
    for (int i = 0; i < height; i++)
    {
        // For each corresponding column
        for (int j = 0; j < width; j++)
        {
            // We declare an integer; average
            int average;
            // Calculate the average values of each pixel colors to determine the grayscale of that pixel
            average = round(((float)image[i][j].rgbtBlue + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtRed)/3);

            // Update the pixel colors with the average
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through all pixels, row by column
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Declaring sepia variable to calculate sepia values
            int sepiaRed;
            int sepiaGreen;
            int sepiaBlue;

            // Calculating each sepia colors
            sepiaRed = round((0.393 * (float)image[i][j].rgbtRed) + (0.769 * (float)image[i][j].rgbtGreen) + (0.189 * (float)image[i][j].rgbtBlue));
            sepiaGreen = round((0.349 * (float)image[i][j].rgbtRed) + (0.686 * (float)image[i][j].rgbtGreen) + (0.168 * (float)image[i][j].rgbtBlue));
            sepiaBlue = round((0.272 * (float)image[i][j].rgbtRed) + (0.534 * (float)image[i][j].rgbtGreen) + (0.131 * (float)image[i][j].rgbtBlue));

            // Handling situation where sepia color is out of range of bytes
            if (sepiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sepiaRed;
            }

            if (sepiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }

            if (sepiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
        }
    }

    return;
}

// void swap(RGBTRIPLE *a, RGBTRIPLE *b)
// {
//     RGBTRIPLE *temp;
//     int height, width

//     temp[0] = *a[height][width].rgbtBlue;
//     temp[1] = *a[height][width].rgtRed;
//     temp[2] = *a[height][width].rgbtGreen;

//     *a[height][width].rgbtBlue = *b[height][height].rgbtBlue;
//     *a[height][width].rgbtRed = *b[height][width].rgbtRed;
//     *a[height][width].rgbtGreen = *b[height][width].rgbtGreen;

//     *b[height][width].rgbtBlue = temp[0];
//     *b[height][width].rgbtRed = temp[1];
//     *b[height][width].rgbtGreen = temp[2];
// }


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    // Create a an array of 3 integers to store temporary values for the swap
    int temp[3];

    // Loop through all pixels
    for (int i = 0; i < height; i++)
    {

        // We loop through hald the size(midpoint) of columns for each row, since we want to swap values at opposite ends of each row
        for (int j = 0; j < width/2; j++)
        {

            // For each of the pixel colors, we store the original values in temp as placeholder
            temp[0] = image[i][j].rgbtBlue;
            temp[1] = image[i][j].rgbtRed;
            temp[2] = image[i][j].rgbtGreen;

            // we swap the last item at each opposite end of the row with the first item
            // i.e [R,G,B,D,A]; then we have [A,G,B,D,R] and continue for each item in the row

            // last item is stored in the address of first item
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;

            // The first item is stored in the address of last item
            image[i][width - j - 1].rgbtBlue = temp[0];
            image[i][width - j - 1].rgbtRed = temp[1];
            image[i][width - j - 1].rgbtGreen = temp[2];

            // swap(&image[i][j].rgbtBlue, &image[i][j+1].rgbtBlue);
            // swap(&image[i][j].rgbtRed, &image[i][j+1].rgbtRed);
            // swap(&image[i][j].rgbtGreen, &image[i][j+1].rgbtGreen);

            //swapped = true;

            // if (swapped == false)
            // {
            //     break;
            // }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary image to copy original image
    RGBTRIPLE newImage[height][width];

    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            newImage[x][y] = image[x][y];
        }
    }

    // Declare variables to calculate average of RGB values for neighbouring pixels
    int averageRed, averageBlue, averageGreen;
    // Loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Handle neighbouring pixels of middle pixel
            if ((i > 0 && j > 0) && (i + 1 < height && j + 1 < width))
            {
                averageRed = round(((float)newImage[i-1][j-1].rgbtRed + (float)newImage[i-1][j].rgbtRed + (float)newImage[i-1][j+1].rgbtRed\
                + (float)newImage[i][j-1].rgbtRed + (float)newImage[i][j].rgbtRed + (float)newImage[i][j+1].rgbtRed\
                + (float)newImage[i+1][j-1].rgbtRed + (float)newImage[i+1][j].rgbtRed + (float)newImage[i+1][j+1].rgbtRed)/9);

                averageBlue = round(((float)newImage[i-1][j-1].rgbtBlue + (float)newImage[i-1][j].rgbtBlue + (float)newImage[i-1][j+1].rgbtBlue\
                + (float)newImage[i][j-1].rgbtBlue + (float)newImage[i][j].rgbtBlue + (float)newImage[i][j+1].rgbtBlue\
                + (float)newImage[i+1][j-1].rgbtBlue + (float)newImage[i+1][j].rgbtBlue + (float)newImage[i+1][j+1].rgbtBlue)/9);

                averageGreen = round(((float)newImage[i-1][j-1].rgbtGreen + (float)newImage[i-1][j].rgbtGreen + (float)newImage[i-1][j+1].rgbtGreen\
                + (float)newImage[i][j-1].rgbtGreen + (float)newImage[i][j].rgbtGreen + (float)newImage[i][j+1].rgbtGreen\
                + (float)newImage[i+1][j-1].rgbtGreen + (float)newImage[i+1][j].rgbtGreen + (float)newImage[i+1][j+1].rgbtGreen)/9);


                //Update the pixel colors with the average
                image[i][j].rgbtBlue = averageBlue;
                image[i][j].rgbtGreen = averageGreen;
                image[i][j].rgbtRed = averageRed;
            }
            // Handling neighbouring pixels of right edge pixels
            else if ((j+1 == width) && (i+1 < height) && (i > 0))
            {
                averageRed = round(((float)newImage[i-1][j-1].rgbtRed + (float)newImage[i-1][j].rgbtRed\
                + (float)newImage[i][j-1].rgbtRed + (float)newImage[i][j].rgbtRed\
                + (float)newImage[i+1][j-1].rgbtRed + (float)newImage[i+1][j].rgbtRed)/6);

                averageBlue = round(((float)newImage[i-1][j-1].rgbtBlue + (float)newImage[i-1][j].rgbtBlue\
                + (float)newImage[i][j-1].rgbtBlue + (float)newImage[i][j].rgbtBlue\
                + (float)newImage[i+1][j-1].rgbtBlue + (float)newImage[i+1][j].rgbtBlue)/6);

                averageGreen = round(((float)newImage[i-1][j-1].rgbtGreen + (float)newImage[i-1][j].rgbtGreen\
                + (float)newImage[i][j-1].rgbtGreen + (float)newImage[i][j].rgbtGreen\
                + (float)newImage[i+1][j-1].rgbtGreen + (float)newImage[i+1][j].rgbtGreen)/6);

                image[i][j].rgbtBlue = averageBlue;
                image[i][j].rgbtGreen = averageGreen;
                image[i][j].rgbtRed = averageRed;
            }
            // Handling neighbouring pixels of Top right corner pixels
            else if ((i == 0) && (j+1 == width))
            {
                averageRed = round(((float)newImage[i][j-1].rgbtRed + (float)newImage[i][j].rgbtRed\
                + (float)newImage[i+1][j-1].rgbtRed + (float)newImage[i+1][j].rgbtRed)/4);

                averageBlue = round(((float)newImage[i][j-1].rgbtBlue + (float)newImage[i][j].rgbtBlue\
                + (float)newImage[i+1][j-1].rgbtBlue + (float)newImage[i+1][j].rgbtBlue)/4);

                averageGreen = round(((float)newImage[i][j-1].rgbtGreen + (float)newImage[i][j].rgbtGreen\
                + (float)newImage[i+1][j-1].rgbtGreen + (float)newImage[i+1][j].rgbtGreen)/4);

                image[i][j].rgbtBlue = averageBlue;
                image[i][j].rgbtGreen = averageGreen;
                image[i][j].rgbtRed = averageRed;
            }
            // Handling neighbouring pixels of bottom right pixels
            else if ((i+1 == height) && (j+1 == width))
            {
                averageRed = round(((float)newImage[i-1][j-1].rgbtRed + (float)newImage[i-1][j].rgbtRed\
                + (float)newImage[i][j-1].rgbtRed + (float)newImage[i][j].rgbtRed)/4);

                averageBlue = round(((float)newImage[i-1][j-1].rgbtBlue + (float)newImage[i-1][j].rgbtBlue\
                + (float)newImage[i][j-1].rgbtBlue + (float)newImage[i][j].rgbtBlue)/4);

                averageGreen = round(((float)newImage[i-1][j-1].rgbtGreen + (float)newImage[i-1][j].rgbtGreen\
                + (float)newImage[i][j-1].rgbtGreen + (float)newImage[i][j].rgbtGreen)/4);

                image[i][j].rgbtBlue = averageBlue;
                image[i][j].rgbtGreen = averageGreen;
                image[i][j].rgbtRed = averageRed;
            }
            // Handling neighbouring pixels of left edge pixels
            else if ((i > 0) && (i + 1 < height) && (j == 0))
            {
                averageRed = round(((float)newImage[i-1][j].rgbtRed + (float)newImage[i-1][j+1].rgbtRed\
                + (float)newImage[i][j].rgbtRed + (float)newImage[i][j+1].rgbtRed\
                + (float)newImage[i+1][j].rgbtRed + (float)newImage[i+1][j+1].rgbtRed)/6);

                averageBlue = round(((float)newImage[i-1][j].rgbtBlue + (float)newImage[i-1][j+1].rgbtBlue\
                + (float)newImage[i][j].rgbtBlue + (float)newImage[i][j+1].rgbtBlue\
                + (float)newImage[i+1][j].rgbtBlue + (float)newImage[i+1][j+1].rgbtBlue)/6);

                averageGreen = round(((float)newImage[i-1][j].rgbtGreen + (float)newImage[i-1][j+1].rgbtGreen\
                + (float)newImage[i][j].rgbtGreen + (float)newImage[i][j+1].rgbtGreen\
                + (float)newImage[i+1][j].rgbtGreen + (float)newImage[i+1][j+1].rgbtGreen)/6);


                //Update the pixel colors with the average
                image[i][j].rgbtBlue = averageBlue;
                image[i][j].rgbtGreen = averageGreen;
                image[i][j].rgbtRed = averageRed;
            }
            // Handling neighbouring pixels of top left corner pixels
            else if (i == 0 && j == 0)
            {
                averageRed = round(((float)newImage[i][j].rgbtRed + (float)newImage[i][j+1].rgbtRed\
                + (float)newImage[i+1][j].rgbtRed + (float)newImage[i+1][j+1].rgbtRed)/4);

                averageBlue = round(((float)newImage[i][j].rgbtBlue + (float)newImage[i][j+1].rgbtBlue\
                + (float)newImage[i+1][j].rgbtBlue + (float)newImage[i+1][j+1].rgbtBlue)/4);

                averageGreen = round(((float)newImage[i][j].rgbtGreen + (float)newImage[i][j+1].rgbtGreen\
                + (float)newImage[i+1][j].rgbtGreen + (float)newImage[i+1][j+1].rgbtGreen)/4);


                //Update the pixel colors with the average
                image[i][j].rgbtBlue = averageBlue;
                image[i][j].rgbtGreen = averageGreen;
                image[i][j].rgbtRed = averageRed;
            }
            // Handling neighbouring pixels of bottom left corner pixels
            else if (i + 1 == height && j == 0)
            {
                averageRed = round(((float)newImage[i-1][j].rgbtRed + (float)newImage[i-1][j+1].rgbtRed\
                + (float)newImage[i][j].rgbtRed + (float)newImage[i][j+1].rgbtRed)/4);

                averageBlue = round(((float)newImage[i-1][j].rgbtBlue + (float)newImage[i-1][j+1].rgbtBlue\
                + (float)newImage[i][j].rgbtBlue + (float)newImage[i][j+1].rgbtBlue)/4);

                averageGreen = round(((float)newImage[i-1][j].rgbtGreen + (float)newImage[i-1][j+1].rgbtGreen\
                + (float)newImage[i][j].rgbtGreen + (float)newImage[i][j+1].rgbtGreen)/4);


                //Update the pixel colors with the average
                image[i][j].rgbtBlue = averageBlue;
                image[i][j].rgbtGreen = averageGreen;
                image[i][j].rgbtRed = averageRed;
            }
            // Handling neighbouring pixels of top edge pixels
            else if ((i == 0) && (j > 0) && (j+1 < width))
            {
                averageRed = round(((float)newImage[i][j-1].rgbtRed + (float)newImage[i][j].rgbtRed + (float)newImage[i][j+1].rgbtRed\
                + (float)newImage[i+1][j-1].rgbtRed + (float)newImage[i+1][j].rgbtRed + (float)newImage[i+1][j+1].rgbtRed)/6);

                averageBlue = round(((float)newImage[i][j-1].rgbtBlue + (float)newImage[i][j].rgbtBlue + (float)newImage[i][j+1].rgbtBlue\
                + (float)newImage[i+1][j-1].rgbtBlue + (float)newImage[i+1][j].rgbtBlue + (float)newImage[i+1][j+1].rgbtBlue)/6);

                averageGreen = round(((float)newImage[i][j-1].rgbtGreen + (float)newImage[i][j].rgbtGreen + (float)newImage[i][j+1].rgbtGreen\
                + (float)newImage[i+1][j-1].rgbtGreen + (float)newImage[i+1][j].rgbtGreen + (float)newImage[i+1][j+1].rgbtGreen)/6);


                //Update the pixel colors with the average
                image[i][j].rgbtBlue = averageBlue;
                image[i][j].rgbtGreen = averageGreen;
                image[i][j].rgbtRed = averageRed;
            }
            // Handling neighbouring pixels of bottom edge pixels
            else if ((i+1 == height) && (j > 0) && (j+1 < width))
            {
                averageRed = round(((float)newImage[i-1][j-1].rgbtRed + (float)newImage[i-1][j].rgbtRed + (float)newImage[i-1][j+1].rgbtRed\
                + (float)newImage[i][j-1].rgbtRed + (float)newImage[i][j].rgbtRed + (float)newImage[i][j+1].rgbtRed)/6);

                averageBlue = round(((float)newImage[i-1][j-1].rgbtBlue + (float)newImage[i-1][j].rgbtBlue + (float)newImage[i-1][j+1].rgbtBlue\
                + (float)newImage[i][j-1].rgbtBlue + (float)newImage[i][j].rgbtBlue + (float)newImage[i][j+1].rgbtBlue)/6);

                averageGreen = round(((float)newImage[i-1][j-1].rgbtGreen + (float)newImage[i-1][j].rgbtGreen + (float)newImage[i-1][j+1].rgbtGreen\
                + (float)newImage[i][j-1].rgbtGreen + (float)newImage[i][j].rgbtGreen + (float)newImage[i][j+1].rgbtGreen)/6);


                //Update the pixel colors with the average
                image[i][j].rgbtBlue = averageBlue;
                image[i][j].rgbtGreen = averageGreen;
                image[i][j].rgbtRed = averageRed;
            }

        }

    }
    return;
}
