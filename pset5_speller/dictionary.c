// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // variable handling //
    unsigned int hash_code = hash(word);
    node *traverser = NULL;

    // index into dictionary //
    traverser = table[hash_code];
    while (traverser != NULL)
    {
        // match //
        if (strcasecmp(traverser->word, word) == 0)
        {
            return true;
        }

        // next node //
        traverser = traverser->next;
    }

    // end function //
    return false;
}

// Hashes word to a number; inspired by UCSD CSE department
unsigned int hash(const char *word)
{
    // variable handling //
    const int POLY_LIMIT = 16;
    int twos, hash_code = 0;

    // calculate hash_code //
    for (int i = 0; word[i] != '\0'; i++, twos *= 2)
    {
        // reset twos //
        if (i % POLY_LIMIT == 0)
        {
            twos = 1;
        }

        hash_code += (toupper(word[i]) - 'A') * twos;
    }

    // end function //
    return hash_code % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // variable handling //
    char word_buffer[LENGTH + 2]; // store newline and null char
    FILE *reader = fopen(dictionary, "r");
    unsigned int hash_code;
    int null_index;

    // error handling //
    if (reader == NULL)
    {
        return false;
    }

    // read dictionary //
    while (fgets(word_buffer, LENGTH + 2, reader) != NULL)
    {
        // adjust word buffer //
        for (null_index = 0; word_buffer[null_index] != '\0'; null_index++)
        {
            continue;
        }
        null_index--;
        word_buffer[null_index] = '\0';

        // get hash //
        hash_code = hash(word_buffer);

        // allocate space //
        if (table[hash_code] == NULL)
        {
            table[hash_code] = (node *) calloc(1, sizeof(node));
            table[hash_code]->next = NULL;
        }
        else
        {
            node *temp = table[hash_code];
            table[hash_code] = (node *) calloc(1, sizeof(node));
            table[hash_code]->next = temp;
        }

        // copy word //
        for (int i = 0; i <= null_index; i++)
        {
            table[hash_code]->word[i] = word_buffer[i];
        }
    }

    // end function //
    fclose(reader);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // variable handling //
    int num_words = 0;
    node *traverser = NULL;

    // iterate dictionary //
    for (int i = 0; i < N; i++)
    {
        traverser = table[i];
        while (traverser != NULL)
        {
            num_words++;
            traverser = traverser->next;
        }
    }

    // end function //
    return num_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // variable handling //
    node *slow_trav = NULL, *fast_trav = NULL;

    // free node lists //
    for (int i = 0; i < N; i++)
    {
        // helper vars //
        slow_trav = table[i];
        fast_trav = slow_trav != NULL ? slow_trav->next : NULL;

        // free each node //
        while (fast_trav != NULL)
        {
            free(slow_trav);
            slow_trav = fast_trav;
            fast_trav = fast_trav->next;
        }
        free(slow_trav);
    }

    // end function //
    return true;
}
