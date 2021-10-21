from cs50 import get_int

# prompt the user for input
while True:
    height = get_int("height = ")
    if height < 9 and height > 0:
        break

# Build the pyramid
for i in range(1, height + 1):
    for d in range(0, height - i):
        print(" ", end="")
    for j in range(i):
        print("#", end="")
    print("  ", end="")
    for j in range(i):
        print("#", end="")
    print("")
