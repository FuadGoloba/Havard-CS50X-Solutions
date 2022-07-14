#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Declare the type of height variable to be an integer
    int height;
    do
    {
        // while the height input is not in range (1, 8 inclusive); we keep asking the user to input
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // For each row
    for (int row = 0; row < height; row++)
    {
        // For each column, we print the spaces by looping through the height-row-1 and decrementing
        for (int space = height - row - 1; space > 0; space--)
        {
            printf(" ");
        }
        // For each column, print #
        for (int col = 0; col < row + 1; col++)
        {
            printf("#");
        }
        // Go to next line and end the current for loop
        printf("\n");
    }

}