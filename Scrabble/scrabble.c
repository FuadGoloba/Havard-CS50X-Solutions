#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Determine which if two scrabble words is worth more

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// calling the function defined below
int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    // Compare the points between each player's word and prints the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins\n!");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string

    // Create an array of 26 alphabets
    char ALPHABET[] = {'A','B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X','Y','Z'};

    // Create a variable counter to sum the points
    int sum = 0;

    // Looping through each letter in the word
    for (int i=0, n = strlen(word); i < n; i++)
    {
        // Loop through each letter in the alphabet to get the scrabble point associated to thatletter
        for (int j=0; j < 26; j++)
        {
            if (toupper(word[i]) == ALPHABET[j]) // Handling lower case
            {
                sum += POINTS[j];  // Summing the points up
            }
            else
            {
                sum += 0; // Handling other characters thataren't from A-Z
            }
        }
    }
    return sum;
}
