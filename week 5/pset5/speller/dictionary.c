// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 60;

// Hash table
node *table[N];

//declaring global variables
int nWords = 0;

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;

    int c = *word;
    c = tolower(c);

    while (*word != 0)
    {
        hash = ((hash << 5) + hash) + c;
        c = *word++;
        c = tolower(c);
    }

    return hash % N;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //calculate the hash of word
    int index = hash(word);

    node *cursor = table[index];

    for (node *temp = cursor; temp != NULL; temp = temp->next)
    {
        if (strcasecmp(temp->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Opens dictionary
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        return false;
    }

    char buffer[LENGTH + 1];
    while (fscanf(file, "%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        //copy word into word
        strcpy(n->word, buffer);
        n->next = NULL;

        //calculate the hash for the word
        int index = hash(n->word);

        //the head of a linked list
        node *head = table[index];

        if (head == NULL)
        {
            table[index] = n;
            nWords++;
        }
        else
        {
            n->next = table[index];
            table[index] = n;
            nWords++;
        }
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return nWords;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // for each of hash table 'buckets' the memory must be free'd.
    //make a cursor which points to the head node. this cursor will work through the linked lists making each = NULL, freeing the memory.
    node *tmp = NULL;
    node *cursor = NULL;
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
