from cs50 import get_int


def main():
    # prompt the user for the card Number
    number = get_int("Number: ")

    # check the type of the card
    if (number < 350000000000000 and number >= 340000000000000) or (number < 380000000000000 and number >= 370000000000000):
        validate(number, "AMEX")
    elif number < 5600000000000000 and number >= 5100000000000000:
        validate(number, "MASTERCARD")
    elif (number < 5000000000000000 and number >= 4000000000000000) or (number < 5000000000000 and number >= 4000000000000):
        validate(number, "VISA")
    else:
        print("INVALID")


def validate(cardNumber, cardType):
    sumf = 0
    nDigits = len(str(cardNumber))

    # Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together
    for digit in range(0, nDigits + 1, 2):
        value = int(((cardNumber % (10**(digit + 2))) / (10**(digit + 1)))) * 2
        valueDigits = len(str(value))
        print(value)

        if valueDigits > 1:
            sumf += int(value % 10)
            sumf += int((value % 100) / 10)
        else:
            sumf += value

    # Add the sum to the sum of the digits that weren’t multiplied by 2.
    for digit in range(0, nDigits + 1, 2):
        value = int((cardNumber % (10**(digit + 1)) / (10**digit)))
        sumf += value

    # If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid
    if int(sumf) % 10 == 0:
        print(cardType)
    else:
        print("INVALID")
        print(sumf)


main()
