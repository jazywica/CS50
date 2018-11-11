//Validating and determining a credit card type
#include <stdio.h>
#include <cs50.h> // used for 'get_long_long()'

int main(void)
{
    long long cc_number;
    cc_number = get_long_long("Number: ");

    long long mod1 = 100;
    long long mod2 = 10;
    int count_1 = 0;
    int count_2 = 0;
    int input_1 = 0;
    int input_2 = 0;
    int sum = 0;

    do {
        input_1 = cc_number % mod1 / (mod1 / 10);
        int result = 2 * input_1;
        if (result > 9)
            sum += result / 10 + result % 10;
        else
            sum += result;
		
        mod1 *= 100;
        count_1 += 1;
    } while ((mod1 / 10) < cc_number);

    do {
        input_2 = cc_number % mod2 / (mod2 / 10);
        sum += input_2;
        mod2 *= 100;
        count_2 += 1;
    } while ((mod2 / 10) < cc_number);


    if (sum % 10 == 0) {
        int count = count_1 + count_2;
        int start = (count_1 > count_2 || count_1 == count_2) ? (input_1 * 10 + input_2) : (input_2 * 10 + input_1);

        if (count == 15 && (start == 34 || start == 37))
            printf("AMEX\n");
        else if (count == 16 && start > 50 && start < 56)
            printf("MASTERCARD\n");
        else if ((count == 16 || count == 13) && start / 10 == 4)
            printf("VISA\n");
        else
            printf("INVALID\n");
    }
    else
        printf("INVALID\n");
}