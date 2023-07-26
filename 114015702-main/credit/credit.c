#include <cs50.h>
#include <stdio.h>

bool validateDigits(long number);

int main(void)
{

    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 1);

    long n = number;
    int lenght = 0;
    int firstTwoNumbers = 0;

    for (int i = 0; number != 0; i++)
    {
        if (number / 100 == 0 && firstTwoNumbers == 0)
        {
            firstTwoNumbers = number % 100;
        }

        number /= 10;
        lenght++;
    }

    bool validDigits = validateDigits(n);

    if (lenght == 16 && validDigits &&
        (firstTwoNumbers == 51 || firstTwoNumbers == 52 || firstTwoNumbers == 53 || firstTwoNumbers == 54 || firstTwoNumbers == 55))
    {
        printf("MASTERCARD\n");
    }
    else if (firstTwoNumbers / 10 == 4 && validDigits && (lenght == 16 || lenght == 13))
    {
        printf("VISA\n");
    }
    else if (lenght == 15 && validDigits && (firstTwoNumbers == 34 || firstTwoNumbers == 37))
    {
        printf("AMEX\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

bool validateDigits(long number)
{

    int multiSum = 0;
    int otherSum = 0;
    for (int i = 0; number != 0; i++)
    {
        int digit = number % 10;

        if (i % 2 != 0)
        {
            digit *= 2;
            if (digit / 10 != 0)
            {
                digit = digit / 10 + digit % 10;
            }
            multiSum += digit;
        }
        else
        {
            otherSum += digit;
        }

        number /= 10;
    }

    bool validDigits = (multiSum + otherSum) % 10 == 0;

    return validDigits;
}