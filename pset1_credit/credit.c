#include <cs50.h>
#include <stdio.h>

int addDigits(int n);

int main(void)
{
    // constant for credit card reference
    const long TEN = 1000000000000; // 10 ^ 12

    // get valid credit card number
    long cardNum;
    do
    {
        cardNum = get_long("Number: ");
    }
    while (cardNum < 0);

    long tens = 10, digit;
    int checkSum = 0;
    for (int i = 1; cardNum / (tens / 10) > 0; i++)
    {
        // isolate digit
        digit = (cardNum % tens) / (tens / 10);

        // add to checkSum accounting for doubling
        if (i % 2 == 1)
        {
            checkSum += digit;
        }
        else
        {
            checkSum += addDigits(2 * digit);
        }

        // update tens
        tens *= 10;
    }

    // first two digits of the credit card number
    int company = cardNum / (tens / 1000);

    if (checkSum % 10 != 0 || cardNum / TEN == 0)
    {
        printf("INVALID\n");
    }
    else if (company == 34 || company == 37)
    {
        printf("AMEX\n");
    }
    else if (company >= 51 && company <= 55)
    {
        printf("MASTERCARD\n");
    }
    else if (company / 10 == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

int addDigits(int n)
{
    // adds the digits of n
    int sum = 0;
    for (int i = 10; n * 10 / i != 0; i *= 10)
    {
        sum += (n % i) / (i  / 10);
    }

    return sum;
}