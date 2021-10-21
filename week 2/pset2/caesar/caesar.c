#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    //store the value provided by the arg
    int key = 0;

    //Check if the number of arg is equal to 1
    if (argc == 2)
    {
        //check if the arg is a decimal number
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (isdigit(argv[1][i]) == false)
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }

        //check if the arg is a non-negative number
        if (atoi(argv[1]) > 0)
        {
            key = atoi(argv[1]);
        }
        else
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }

    }
    else
    {
        printf("Error\n");
        return 1;
    }

    //Prompt user for plaintext
    string text = get_string("plaintext: ");

    printf("ciphertext: ");

    //Rotate the characters based on the value of the key and preserving case
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //check if the character is an alphabet character
        if (isalpha(text[i]))
        {
            //decide whether the character is Uppercase or Lowercase
            if (isupper(text[i]))
            {
                printf("%c", (text[i] - 65 + key) % 26 + 65);
            }
            else
            {
                printf("%c", (text[i] - 97 + key) % 26 + 97);
            }
        }
        else
        {
            printf("%c", text[i]);
        }
    }
    printf("\n");
}




