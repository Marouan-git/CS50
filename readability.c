#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string s);
int count_words(string s);
int count_sentences(string s);
void print_grade(string s);

int main(void)
{
    string txt = get_string("Text : ");
    
    /*
    printf("%i letter(s) \n", count_letters(txt));
    printf("%i word(s) \n", count_words(txt));
    printf("%i sentence(s) \n", count_sentences(txt));
    */
    
    //prints the grade
    print_grade(txt);
    
    return 0;
}

// Returns the number of letters in s
int count_letters(string s)
{
    int counter = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (isalpha(s[i]))
        {
            counter++;
        }
    }
    
    return counter;
}

// Returns the number of words in s
int count_words(string s)
{
    int counter = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (s[i] == ' ')
        {
            counter++;
        }
    }
    
    return counter + 1;
}

//Returns the number of sentences in s
int count_sentences(string s)
{
    int counter = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (s[i] == '.' || s[i] == '!' || s[i] == '?')
        {
            counter++;
        }
    }
    
    return counter;
}

//Prints the grade of s
void print_grade(string s)
{
    float L = ((float) count_letters(s) / count_words(s)) * 100;
    float S = ((float) count_sentences(s) / count_words(s)) * 100;
    int grade = round(0.0588 * L - 0.296 * S - 15.8);
    
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade <= 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}
