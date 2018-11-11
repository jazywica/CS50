// Encrypting messages by shifting each letter alphabetically by some number of places
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "cs50.h" // used for 'get_string()'

void encrypt(char l, int k); // declare functions to be used in advance


int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: ./caesar k"); // a keyword (number) must be provided as a command line argument
        return 1;
    }

    int k = atoi(argv[1]);

    char *text = get_string("plaintext:  ");
    printf("ciphertext:  ");

    for (int i = 0, n = strlen(text); i < n; i++) {
        char letter = text[i];
        if (isalpha(letter))
            encrypt(letter, k);
        else
            printf("%c", text[i]);
    }

    printf("\n");

    return 0;
}

void encrypt(char l, int  k)
{
    char capital = (isupper(l)) ? 'A' : 'a';
    int n = ((l - capital + k) % 26) + capital; // here we wrap around the number of letters we have available in ASCI
    char ciphered = (char)n;
    printf("%c", ciphered);
}
