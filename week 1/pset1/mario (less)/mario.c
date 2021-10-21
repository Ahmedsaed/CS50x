#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Asking for the height
    int h;
    do
    {
        h = get_int("Height = ");
    }
    while (h < 1 || h > 8); //Cheking if the input is vaild or not

    //Building the pyramid
    for (int i = 1; i < h + 1; i++)
    {
        for (int d = 0; d < h - i; d++)
        {
            printf(" ");
        }
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}




