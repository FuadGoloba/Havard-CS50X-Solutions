// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// SEarching the linkedlist in the hashtable using recursion
bool search(node *head, const char *s);

// Freeing memory using recursion
void free_list(node *list);

// TODO: Choose number of buckets in hash table
// I choose 5490 based on the type of hash function implemented
// The hash function implemented takes the ascii sum of words as hash value
// Therefore, since the ascii of the last alphabet z is 127 and the longest word in the dictionary is 45 characters long
// then the possibility of a max hash value (assuming z to the power of 45 (zzzzzz....) is a word) is 127*45 = 5490
const unsigned int N = 5490;

// Hash table
node *table[N];

// Initialise an integer to keep track of the nuber of words in the dictionary
int dictionary_size = 0;


// SEarching the linkedlist in the hashtable using recursion
bool search(node *head, const char *s)
{
    // takes a linked list called head and a string s and searches for that string in the linked list

    // Create a base case; as long as the head isn't null (end of the linkedlist)
    if (head == NULL)
    {
        return false;
    }

    // comparing the string s with the word in the linkedlist to check if they are same and returns true
    if (strcasecmp(head->word, s) == 0)
    {
        return true;
    }
    // else recuresively search making the next node as head
    else
    {
        return search(head->next, s);
    }

}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // First hash word to obtain a hash value
    int hash_value = hash(word);

    // ACcess linked list using the hash value as index in the hash table
    node *list = table[hash_value];
    // Search linked list to check if the word is in the list
    return search(list, word);
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // initialising a sum of ascii lowercase characters in a word
    int total_ascii = 0;

    // Traversing each word to sum its ascii number

    for (int i = 0; i < strlen(word); i++)
    {
        total_ascii += tolower(word[i]);
    }
    // return a hash value in the range of N (should total ascii exceed N = 5490)
    return total_ascii % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        //printf("Could not open file\n");
        return false;
    }
    // Create a character array to store each string to be read from the dictionary file
    char string[LENGTH + 1];
    // Read the file, string by string
    while (fscanf(file, "%s", string) != EOF)
    {
        // Create a new node to store each string read
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        // copy the string into the pointer of the node
        strcpy(n->word, string);
        //n->next = NULL;

        // hash the word to a hash_value, this value is an index to the array where the word shiuld be stored
        int hash_value = hash(n->word);

        //Insert node into hash table
        // First point the 'next' pointer of n (which has the word) to the array that houses the linkedlist where the word should be stored. Remember the array also points to the first element of the linkedlist where the word should be stored
        n->next = table[hash_value];
        // Point the array pointerback to the new word pointer which is now the first element of the linkedlist. This way we have inserted the new word into the hashtable
        table[hash_value] = n;
        // keeping track of the number of words in dictionary by incremeting dictionary size as we load
        dictionary_size++;
    }
    // Close dictionary file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    // return the dictionary size
    return dictionary_size;
}

// recursively free linked list
void free_list(node *list)
{
    // Create a base case to check if we've got to the end of the list
    if (list == NULL)
    {
        return;
    }
    // recuresively free the next pointer
    free_list(list->next);
    // Free the list
    free(list);
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // Traverse the entire hash table to free each of its linked lists
    // start at 96 since the ascii number for an spostrophe is 96, should there be a word that starts with an apostrophe
    for (int i = 96; i < N; i ++)
    {
        // free the list for each index (hash value) of the hash table
        free_list(table[i]);
        // return true if we get to the last index of the hash table
        if (i == N-1)
        {
            return true;
        }
    }
    return false;
}
