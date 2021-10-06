#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> 

string substitute(string key, string plaintext);
bool key_validation(string key);


int main(int argc, string argv[])
{
    //Checks if the user entered the right number of arguments in command line
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    string key = argv[1];
    
    bool validation = key_validation(key);
    if (validation == false)
    {
        return 1;
    }
    
    string plaintext = get_string("plaintext: ");
    //printf("%s", plaintext);
    
    string ciphertext = substitute(key, plaintext);
    printf("ciphertext: %s\n", ciphertext);
    
    return 0;
    
    
}


//Returns true if the key is valid, otherwise false with an appropriate message
bool key_validation(string key)
{
    bool res = true;
    
    //Checks if the length of the key is 26
    if (strlen(key) != 26)
    {
        res = false;
        printf("Key must contain 26 characters.\n");
        
    }
    //If the length of the key is 26, we have to check if it contains only alpha characters and that each character is unique
    else
    {
        for (int i = 0, n = strlen(key); i < n; i++)
        {
            //Checks if every character is alpha
            if (!isalpha(key[i]))
            {
                res = false;
                printf("Key must contain only alpha characters.\n");
                break;
            }
            
            //Checks if we did not reach the last character of the key
            if (i != (n - 1))
            {
                //Checks if every alpha character is used only once in the key
                for (int j = i + 1; j < n; j++)
                {
                    if (tolower(key[i]) == tolower(key[j]))
                    {
                        res = false;
                        printf("Key must contain each alpha character only once.\n");
                        break;
                    }
                }  
            }
            
            //No need to continue the loop if res equals false
            if (res == false)
            {
                break;
            }
           
        }
    }
    
    return res;
   
}

//Substitutes every character of the plaintext according to the given key and returns the ciphertext
string substitute(string key, string plaintext)
{
    string ciphertext = malloc(sizeof(plaintext));
    strcpy(ciphertext, plaintext);
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (isalpha(plaintext[i]))
        {
            if (isupper(plaintext[i]))
            {
                ciphertext[i] = toupper(key[plaintext[i] - 'A']);
            }
            else
            {
                ciphertext[i] = tolower(key[plaintext[i] - 'a']);  
            }
        }
        
    }
    
    return ciphertext;
}
