#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


// program that implements a substitution cipher, that takes in a key(non repeated 26 letters of the alphabets) as an argument in the command prompt
// and encrypting a message by replacing every letter with letters in the key:  in this case, a mapping of each of the letters of the alphabet to the letter it should correspond to when we encrypt it.

//A key, for example, might be the string NQXPOMAFTRHLZGECYJIUWSKDVB. This 26-character key means that A (the first letter of the alphabet) should be converted into N (the first character of the key),
// B (the second letter of the alphabet) should be converted into Q (the second character of the key), and so forth.

// $ ./substitution YTNSHKVEFXRBAUQZCLWDMIPGJO
// plaintext:  HELLO
// ciphertext: EHBBQ

// Function to ensure that user inputs only non-repeated letters of the 26 alphabets
bool only_unique_letters(string s);
// Function to convert or replace a letter with another letter in the key
char convert(char c, string s);


int main(int argc, string argv[])
{
    string key = argv[1];

    // Handling user's input to only take in an argument with only non repeated leters of the alphabet
    if (argc!=2 || strlen(key) != 26 || only_unique_letters(key) == false)
    {
        printf("Usage: ./substitution key {Key must contain only non-repeated 26 alphabets[Aa-zZ]}\n");
        return 1;
    }

    // Prompt user for plain text
    string text = get_string("plaintext: ");

    printf("ciphertext: ");

    // Iterating over each letter entered by the user and replacing with it's mapped or corresponding letter in the key
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        printf("%c", convert(text[i], key));
    }
    printf("\n");
    return 0;
}

bool only_unique_letters(string s)
{
    // Iterating over each letter in the key provided by the user to ensure that it contains only alphabets and are non-repeated
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        // returns false if it contains other characters
        if (!isalpha(s[i]))
        {
            return false;
        }
        // Iterating over the key to check for repeated characters
        for (int j = i + 1, m = strlen(s); j < m; j++)
        {
            if (s[i] == s[j])
            {
                return false;
            }
        }
    }
    return true;
}

char convert(char c, string s)
{

    // Replace or substitute a letter given a string of the 26 alphabet letters using the index position
    // (i.e The index of each letter in the message would be the index of the letter to substitute in the key)
    // (e.g. If Key = FEDCBA with index from 0-5, and message = BEF. First we find the index of the ordered letters in the alphabets from A-Z)
    // (i.e A = 0, B = 1, C = 2, D = 3, e = 4 F = 5) and subsititue index by index. Therefore BEF will be replaced with EBA using the key
    if (isupper(c))
    {
        int index = ((int) c - 65) % 26;
        return toupper(s[index]);
    }
    else if (islower(c))
    {
        int index = ((int) c - 97) % 26;
        return tolower(s[index]);
    }
    else
    {
        return c;
    }
}