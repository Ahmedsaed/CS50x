from sys import argv
import csv


def main():
    # check the number of command line arguments
    if len(argv) != 3:
        print("Usage: dna.py file.csv file.txt")
        return
    
    # read the first line of the csv file
    dna_types = {}
    with open(argv[1]) as csvfile:
        head = next(csv.reader(csvfile))
    
    for h in head:
        dna_types[h] = 0
    
    # read and allocate the rest of the csv file into the memory
    data = []
    with open(argv[1]) as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            data.append(row)
    
    # read the DNA sequence to the memory   
    with open(argv[2]) as text:
        seq = text.read()
    
    # calculate the longest run of consecutive repeats for each STR
    for d in dna_types:
        if d == "name":
            continue
        dna_types[d] = STR_count(seq, d)
    
    # print the name of the person with the matched DNA
    print(check(data, dna_types))


# A function to count the longest run of consecutive repeats of a STR
def STR_count(seq, strt):
    count = 0
    strtype = strt
    while True:
        if strtype in seq:
            count += 1
            strtype = strtype + strt
        else:
            break

    return count


# compare each person in the database with the DNA sequence and returns the match if found
def check(data, dna_types):
    for row in data:
        match = 0
        for d in dna_types:
            if d == "name":
                continue
            if int(row[d]) == dna_types[d]:
                match += 1

        if match == len(dna_types) - 1:
            return row['name']

    return "No match"


main()

