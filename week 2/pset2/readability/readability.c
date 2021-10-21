#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int calGrade(string text);
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    //prompt the user for the input
    int grade = calGrade(get_string("Text: "));

    //Display the output
    if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }

}

//calculate the grade value based on Coleman-Liau index
int calGrade(string text)
{
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    //calculate the index value using Coleman-Liau formula
    int index = round(0.0588 * ((letters * 100) / (float) words) - 0.296 * ((sentences * 100) / (float) words) - 15.8);

    return index;
}

//counts the number of letters in the given text
int count_letters(string text)
{
    int l = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //check if the character is an alphabetical character
        if (isalpha(text[i]))
        {
            l++;
        }
    }
    return l;
}

//counts the number of words in the given text
int count_words(string text)
{
    int w = 1;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //check if the character is a space character
        if (isspace(text[i]))
        {
            w++;
        }
    }
    return w;
}

//counts the number of sentences in the given text
int count_sentences(string text)
{
    int s = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //check if the character is a ! or . or ?
        if (text[i] == '!' || text[i] == '.' || text[i] == '?')
        {
            s++;
        }
    }
    return s;
}

