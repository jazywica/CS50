// Program that cracks passwords (up to 5 charactes long) encrypted with Linux standard DES-based (not MD5-based) 'crypt' function
#define _XOPEN_SOURCE // these two must be in a specific order. check the manual: man crypt
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int process(void);

char *hashed; // when allocating a unmutable string we use *char, when we initialize a table with characters we use 'char NAME[n]'
char salt[3]; // here we allocate the space for a string which is the total length plus one (the last character: \0)
char answer[6]; // initializer for the cracked password


int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: ./crack hash\n");  // a hashed password must be provided as a command line argument
        return 1;
    }

    hashed = argv[1]; // we can't assign argv outside the main function, this is why it is inside 'main'
    strncpy(salt, hashed, 2); // copies the specified length from a string to a new one
    salt[2] = '\0'; // place the null terminator

    // set of loops that will look through all permutations  in the following pattern: A, AA, AAA, AAAA, AAAAA, AAAAB -> AAAAz, then the loop above: AAAB, AAABA -> AAABz, AAAC, AAACA -> AAACz etc.
    for (int i = 65; i < 123; i++) {
        if (i < 91 || i > 96) {
            answer[0] = (char)i;
            answer[1] = '\0';
            if (process())
                return 0;
        }
        for (int j = 65; j < 123; j++) {
            if (j < 91 || j > 96) {
                answer[0] = (char)i;
                answer[1] = (char)j;
                answer[2] = '\0';
                if (process())
                    return 0;
            }
            for (int k = 65; k < 123; k++) {
                if (k < 91 || k > 96) {
                    answer[0] = (char)i;
                    answer[1] = (char)j;
                    answer[2] = (char)k;
                    answer[3] = '\0';
                    if (process())
                        return 0;
                }
                for (int l = 65; l < 123; l++) {
                    if (l < 91 || l > 96) {
                        answer[0] = (char)i;
                        answer[1] = (char)j;
                        answer[2] = (char)k;
                        answer[3] = (char)l;
                        answer[4] = '\0';
                        if (process())
                            return 0;
                    }
                    for (int m = 65; m < 123; m++) {
                        if (m < 91 || m > 96) {
                            answer[0] = (char)i;
                            answer[1] = (char)j;
                            answer[2] = (char)k;
                            answer[3] = (char)l;
                            answer[4] = (char)m;
                            answer[5] = '\0';
                            if (process())
                                return 0;
                        }
                    }
                }
            }
        }
    }
}

int process(void)
{
    if (strcmp(hashed, crypt(answer, salt)) == 0) {
        printf("%s\n", answer);
        return 1;
    }
    return 0;
}