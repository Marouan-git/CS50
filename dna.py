import csv
import sys


""" Identifies a person based on their DNA """

def main():
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    persons = []
    # Open and read CSV's content into memory
    with open(sys.argv[1], "r") as csvFile:
        reader = csv.DictReader(csvFile)
        for person in reader:
            persons.append(person)

    # Open and read the DNA sequence into memory
    with open(sys.argv[2], "r") as txtFile:
        sequence = txtFile.read()

    # List of the colums names
    keys = list(persons[0].keys())

    # Finding matches
    length_persons = len(persons)
    length_keys = len(keys)
    for i in range(length_persons):
        for j in range(1, length_keys, 1):
            if max_STR_repeats(sequence, keys[j]) != int(persons[i][keys[j]]):
                break
        if j == length_keys - 1:
            print(persons[i]["name"])
            break
        elif j != length_keys and i == length_persons - 1:
            print("No match")
            break

# Determines the length of the longest sequence where STR is repeated in DNA


def max_STR_repeats(DNA, STR):
    lengthSTR = len(STR)
    lengthDNA = len(DNA)
    count_str = 0
    max_str = 0
    i = 0
    j = 0
    while i < lengthDNA:
        j = i + lengthSTR
        if DNA[i:j] == STR:
            count_str += 1
            if count_str > max_str:
                max_str = count_str
            i += lengthSTR
        else:
            count_str = 0
            i += 1

    return max_str


if __name__ == "__main__":
    main()
