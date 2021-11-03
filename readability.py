from cs50 import get_string

""" Outputs the grade level for a text given as input by the user """


def main():
    # Prompt the user for the text
    txt = get_string("Text: ")

    # Prints the grade level for the text
    print_grade(txt)

# Returns the number of letters in s


def count_letters(s):
    counter = 0
    for char in s:
        if char.isalpha():
            counter += 1
    return (counter)

# Returns the number of words in s


def count_words(s):
    return (len(s.strip().split(" ")))

# Returns the number of sentences in s


def count_sentences(s):
    return (s.count(".") + s.count("!") + s.count("?"))

# Prints the grade of txt


def print_grade(txt):
    L = (count_letters(txt) / count_words(txt)) * 100
    S = (count_sentences(txt) / count_words(txt)) * 100
    grade = round(0.0588 * L - 0.296 * S - 15.8)

    if grade >= 16:
        print("Grade 16+")
    elif grade <= 1:
        print("Before Grade 1")
    else:
        print(f"Grade {grade}")


# Runs main function
if __name__ == "__main__":
    main()
