#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    // checks number of args
    if (argc != 2)
    {
        printf("USAGE: ./substitution key %i\n", argc);
        return 1;
    }

    // checks correct length key
    if (strlen(argv[1]) != 26)
    {
        printf("ERROR INVALID KEY: key must contain 26 characters!\n");
        return 1;
    }

    // checks valid key for characters
    int char_count[26];
    char c;

    for (int i = 0; i < 26; i++)
    {
        // assign variables for simplicity
        c = argv[1][i];

        // account for uppercase
        if (isupper(c))
        {
            char_count[c - 65]++;
        }
        // account for lowercase
        else if (islower(c))
        {
            char_count[c - 97]++;
        }
        // account for invalid char
        else
        {
            printf("ERROR INVALID KEY: key must contain only letters\n");
            return 1;
        }
    }

    // check valid key for duplicates
    for (int i = 0; i < 26; i++)
    {
        if (char_count[i] == 2 || char_count[i] == 0)
        {
            printf("ERROR INVALID KEY: key must contain all letters\n");
            return 1;
        }
    }

    // get unencrypted text
    string plain_text = get_string("plaintext: ");

    // encrypt text
    for (int i = 0, l = strlen(plain_text); i < l; i++)
    {
        // assign variable for simplicity
        c = plain_text[i];

        // account for uppercase
        if (isupper(c))
        {
            plain_text[i] = toupper(argv[1][c - 65]);
        }
        // account for lowercase
        else if (islower(c))
        {
            plain_text[i] = tolower(argv[1][c - 97]);
        }
    }

    // print result
    printf("ciphertext: %s\n", plain_text);

    // exit status no error
    return 0;
}