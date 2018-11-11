// Encrypting messages using a sequence of keys (a keyword). Keyword is an alphbetical string, whereby A represents 0, B represents 1, C represents 2, …​, and Z represents 25)
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h> // used for 'get_string()'

void encrypt(char l, char k); // declare functions to be used beforehand


int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: ./vigenere k"); // a keyword (one word) must be provided as a command line argument
        return 1;
    }

    char *k = argv[1]; // when passing a complete string we use char* which is just a pointer to the first byte in the memory of type char that then leads to the rest of the string until \0 is reached
    int n_k = strlen(k);
    for (int x = 0; x < n_k; x++) {
        if (!isalpha(k[x])) {
            printf("Usage: ./vigenere k"); // a keyword (one word) must be provided as a command line argument
            return 1;
        }
    }

    char *text = get_string("plaintext:  "); // when passing a complete string we use char* expression
    printf("ciphertext:  ");

    for (int i = 0, j = 0, n = strlen(text); i < n; i++) {
        char letter = text[i];
        if (isalpha(letter)) {
            encrypt(letter, k[j]);
            j++;
            j = j % n_k; // here we wrap around the number of letters we have available in the key
        }
        else
            printf("%c", text[i]);
    }

    printf("\n");

    return 0;
}

void encrypt(char l, char k)
{
    char capital_l = (isupper(l)) ? 'A' : 'a';
    char capital_k = (isupper(k)) ? 'A' : 'a';
    int n = (l - capital_l + k - capital_k) % 26 + capital_l; // here we wrap around the number of letters we have available in ASCI
    char ciphered = (char)n;
    printf("%c", ciphered);
}