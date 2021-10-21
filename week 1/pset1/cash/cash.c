#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    //declare values
    int q = 25;
    int d = 10;
    int n = 5;
    int p = 1;

    //Ask the user for input
    float money;
    do
    {
        money = get_float("Change owed: ");
    }
    while (money < 0); //validate the input

    //convert the input from float to int
    int cents = round(money * 100);

    //Calculate the number of coins
    int coins = 0;
    do
    {
        if (q <= cents)
        {
            cents = cents - q;
            coins++;
        }
        else if (d <= cents)
        {
            cents = cents - d;
            coins++;
        }
        else if (n <= cents)
        {
            cents = cents - n;
            coins++;
        }
        else if (p <= cents)
        {
            cents = cents - p;
            coins++;
        }
    }
    while (cents > 0);

    //Display the number of coins
    printf("%i\n", coins);
}