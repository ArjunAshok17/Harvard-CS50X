#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    /* Coleman-Liau Index for readability measurement
     * L - avaerage number of letters per one hundred words
     * S - average number of sentences per one hundred words
     * index = 0.0588 * L - 0.296 * S - 15.8
     */

    // get text
    string text = get_string("Text: ");

    // get variable values
    int num_words = count_words(text);
    float l = count_letters(text) * 100.0 / num_words;
    float s = count_sentences(text) * 100.0 / num_words;

    // calculate index and grade level
    float index = .0588 * l - .296 * s - 15.8;
    int grade = round(index);

    // prints the results depending on the index generated
    printf("\n");
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }

    // return no error
    return 0;
}

int count_letters(string text)
{
    // returns number of letters in a text

    // track num_letters
    int letters = 0;

    // loops through string
    for (int i = 0, l = strlen(text); i < l; i++)
    {
        // if a valid lowercase or uppercase lettter
        if ((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122))
        {
            letters++;
        }
    }

    // return
    return letters;
}

int count_words(string text)
{
    // returns the number of words in a text
    // (a word is anything separated by a space, hyphenated words count as one word)

    // tracks num_words
    int words = 0;

    // loop through string
    for (int i = 0, l = strlen(text); i < l; i++)
    {
        // if there is a space, we know the previous string of chars was a word (including new sentence)
        if (text[i] == ' ')
        {
            words++;
        }
    }

    // account for last word of sentence
    words++;

    // return
    return words;
}

int count_sentences(string text)
{
    // counts the number of sentences in a text
    // (assumes any '.', '!', or '?' denotes a new sentence)

    // track num_sentences
    int sentences = 0;

    // loop through string
    for (int i = 0, l = strlen(text); i < l; i++)
    {
        // check for punctuation
        if (text[i] == '.'
            || text[i] == '!'
            || text[i] == '?')
        {
            sentences++;
        }
    }

    // return
    return sentences;
}