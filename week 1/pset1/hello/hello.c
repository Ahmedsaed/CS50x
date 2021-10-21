#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Asking for the user's name
    string answer = get_string("What's your name? ");

    //displaying the output
    printf("hello, %s\n", answer);
}