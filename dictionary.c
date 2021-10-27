// Implements a dictionary's functionality

#include <stdbool.h>

#include <stdlib.h>

#include <stdio.h>

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

// Number of buckets in hash table regarding to the hash function chosen
const unsigned int N = (LENGTH + 1) * 'z';

// Hash table
node *table[N];

// Number of words in the dictionary
int total_words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int word_index = hash(word);
    node *cursor = table[word_index];
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }

        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number as the sum of the ascii values of each letter of the word % N
unsigned int hash(const char *word)
{
    int sum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]);
    }
    return (sum % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary in a file
    FILE *dico = fopen(dictionary, "r");
    if (dico == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // Read one word at a time from the file
    node *n = NULL;
    char word[LENGTH + 1];
    while (fscanf(dico, "%s", word) != EOF)
    {
        n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, word);
        n->next = NULL;

        int index = hash(word);
        if (table[index] == NULL)
        {
            table[index] = n;
        }
        else
        {
            n->next = table[index];
            table[index] = n;
        }

        total_words++;
    }

    fclose(dico);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return total_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *tmp = table[i];
        while (cursor != NULL)
        {

            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}
