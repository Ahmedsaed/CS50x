#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    string alphabet = "abcdefghijklmnopqrstuvwxyz";

    //Check if the number of arguments doesn't exceed 1 and that the argument length is 26
    if (argc == 2 && strlen(argv[1]) == 26)
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            //check if the argument's characters are alphabet characters
            if (isalpha(argv[1][i]) == false)
            {
                printf("Error1\n");
                return 1;
            }

            //check if the argument containing each letter exactly once
            for (int j = 0; j < n; j++)
            {
                if (i != j && argv[1][i] == argv[1][j])
                {
                    printf("Error2\n");
                    return 1;
                }
            }
        }
    }
    else
    {
        printf("Error3\n");
        return 1;
    }

    //Prompt user for plaintext
    string text = get_string("plaintext: ");

    //Display the output
    printf("ciphertext: ");
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            if (isupper(text[i]))
            {
                for (int j = 0; j < 26; j++)
                {
                    //Converting Uppercase characters
                    if (tolower(text[i]) == alphabet[j])
                    {
                        printf("%c", toupper(argv[1][j]));
                    }
                }
            }
            else
            {
                for (int j = 0; j < 26; j++)
                {
                    //Converting Lowercase characters
                    if (text[i] == alphabet[j])
                    {
                        printf("%c", tolower(argv[1][j]));
                    }
                }
            }
        }
        else
        {
            //passing non-alphabet charaters
            printf("%c", text[i]);
        }
    }

    printf("\n");
}