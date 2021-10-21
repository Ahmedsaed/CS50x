from cs50 import get_float

# declare values
q = 25
d = 10
n = 5
p = 1

# Ask the user for input
while True:
    money = get_float("Change owed: ")
    # validate the input
    if money > 0:
        break

# number of cents
cents = float(money * 100.0)

# Calculate the number of coins
coins = 0
while True:
    if q <= cents:
        cents = cents - q
        coins += 1
    elif d <= cents:
        cents = cents - d
        coins += 1
    elif n <= cents:
        cents = cents - n
        coins += 1
    elif p <= cents:
        cents = cents - p
        coins += 1

    if cents <= 0:
        break

# Display the number of coins
print(coins)