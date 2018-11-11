//Drawing obstacles for Mario game on screen
#include <stdio.h>
#include "cs50.h" // used for 'get_int()'


int main(void)
{
    int num; // this is the main input value for rows

    do {
        num = get_int("Number: ");
    } while (num < 0 || num > 23);

    for (int i = 0; i < num; i++) {
        int hash = i + 1;
        int space = num - i - 1;

        for (int j = 0; j < space; j++)
            printf(" ");

        for (int k = 0; k < hash; k++)
            printf("#");

        printf("  ");

        for (int l = 0; l < hash; l++)
            printf("#");

        printf("\n");
    }
}
