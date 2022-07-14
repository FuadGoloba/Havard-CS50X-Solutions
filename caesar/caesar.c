#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


// implement a program that enables you to encrypt messages using Caesar’s cipher.
// Caesar (yes, that Caesar) used to “encrypt” (i.e., conceal in a reversible way) confidential messages by shifting each letter therein by some number of places.
//For instance, he might write A as B, B as C, C as D, …, and, wrapping around alphabetically, Z as A. And so, to say HELLO to someone, Caesar might write IFMMP instead.
// At the time the user executes the program, they should decide, by providing a command-line argument, what the key should be in the secret message they’ll provide at runtime.

// Function to check and ensure that the user inputs a digit as key in the commandline
bool only_digits(string s);

// Function to rotate/shift a letter by a number
char rotate(char c, int n);

int main(int argc, string argv[])
{
    // User is only allowed to enter 2 arguments at the command prompt; the program name and a key(has to be a numeric digit)
    if (argc!=2 || only_digits(argv[1]) == false)
    {
        printf("Usage: ./caesar Key\n");
        return 1;
    }
    //else if (only_digits(argv[1]) == false)
    //{
    //    printf("Usage: ./caesar Keyyyyy\n");
    //    return 1;
    //}
    // Since the key passed in the command line is a string, convert to interger for arithmetic calculation
    int key = atoi(argv[1]);

    // Prompt user for plain text
    string text = get_string("plaintext: ");

    printf("ciphertext: ");

    // Iterating over every char in the user's plain text and rotating each letter and printing
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        printf("%c", rotate(text[i], key));
    }
    printf("\n");

    return 0;
}

bool only_digits(string s)
{
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit(s[i]))
        {
            return false ;
        }
    }
    return true;
}

char rotate(char c, int n)
{
    if (isupper(c))
    {
        // To rotate a letter, we use rotated = (letter + key) % 26
        // We use modulo 26 to wrap around the 26 alphabets from 0 - 25 when using the ASCII table
        // (i.e If we want to rotate A by 27 such that the rotated letter stays within the 26 alphabets and not other characters. So we get B instead of '[')

        // In order to use the formula, first we find the index of the rotated latter as 0 - 25
        // (So we have A as 0, B as 1, C as 2 etc). -- (int) c - (int) "A" This gives us the rotated index
        // Then we get the rotated letter by converting the index back to its original letter; adding the ASCII value of A to the rotated index
        int index = ((int) c - 65 + n) % 26;
        char rotated = index + 65;
        return rotated;
    }
    else if (islower(c))
    {
        int index = ((int) c - 97 + n) % 26;
        char rotated = index +  97;
        return rotated;
    }
    else
    {
        return c;
    }
}
