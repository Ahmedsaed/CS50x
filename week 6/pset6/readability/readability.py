from cs50 import get_string


def main():

    # prompt the user for the input
    grade = calGrade(get_string("Text: "))

    # Display the output
    if grade > 16:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print("Grade " + str(grade))


def calGrade(text):
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # calculate the index value using Coleman-Liau formula
    index = round(0.0588 * ((letters * 100) / words) - 0.296 * ((sentences * 100) / words) - 15.8)

    return index


def count_letters(text):
    l = 0
    for i in range(0, len(text)):
        # check if the character is an alphabetical character
        if text[i].isalpha():
            l += 1

    return l
    

def count_words(text):
    w = 1
    for i in range(0, len(text)):
        # check if the character is a space character
        if text[i].isspace():
            w += 1

    return w
    

def count_sentences(text):
    s = 0
    for i in range(0, len(text)):
        # check if the character is a ! or . or ?
        if text[i] == '!' or text[i] == '.' or text[i] == '?':
            s += 1

    return s
    

main()

