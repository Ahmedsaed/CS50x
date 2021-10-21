#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//A custom function to validate the card number by doing the calculations according to Luhn’s Algorithm
void validate(long cardNumber, string cardType)
{
    int sumf = 0;
    int nDigits = floor(log10(cardNumber)) + 1;

    // Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together
    for (int digit = 0; digit < nDigits; digit += 2)
    {
        int value = (int)((cardNumber % (long) pow(10, digit + 2)) / (pow(10, (digit + 1)))) * 2;
        int valueDigits = floor(log10(value)) + 1;

        if (valueDigits > 1)
        {
            sumf += value % (long) pow(10, 1);
            sumf += (value % (long) pow(10, 2)) / (pow(10, 1));
        }
        else
        {
            sumf += value;
        }
    }

    // Add the sum to the sum of the digits that weren’t multiplied by 2.
    for (int digit = 0; digit < nDigits; digit += 2)
    {
        int value = ((cardNumber % (long) pow(10, digit + 1)) / (pow(10, digit)));
        sumf += value;
    }

    //If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid
    if (sumf % 10 == 0)
    {
        printf("%s\n", cardType);
    }
    else
    {
        printf("INVALID\n");
    }
}

int main(void)
{
    //prompt the user for the card Number
    long number = get_long("Number: ");

    //check the type of the card
    if (number < 350000000000000 & number >= 340000000000000 || number < 380000000000000 & number >= 370000000000000)
    {
        validate(number, "AMEX");
    }
    else if (number < 5600000000000000 & number >= 5100000000000000) 
    {  
        validate(number, "MASTERCARD");
    }
    else if (number < 5000000000000000 & number >= 4000000000000000 || number < 5000000000000 & number >= 4000000000000)
    {
        validate(number, "VISA");
    }
    else
    {
        printf("INVALID\n");
    }
}
