#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Ask the user for the starting size
    int startSize;
    do
    {
        startSize = get_int("Start Size = ");
    }
    while (startSize < 9);

    //Ask the user for the ending size
    int endSize;
    do
    {
        endSize = get_int("End Size = ");
    }
    while (endSize < startSize);

    //Calculate the number of years
    int years = 0;
    int currentPopulation = startSize;
    do
    {
        currentPopulation = currentPopulation + currentPopulation / 3 - currentPopulation / 4;
        if (startSize < endSize)
        {
            years++;
        }
    }
    while (currentPopulation < endSize);

    //Display the output
    printf("Years: %i\n", years);
}

