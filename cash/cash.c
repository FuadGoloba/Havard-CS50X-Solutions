#include <cs50.h>
#include <stdio.h>

int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    // Ask how many cents the customer is owed
    int cents = get_cents();

    // Calculate the number of quarters to give the customer
    int quarters = calculate_quarters(cents);
    cents = cents - quarters * 25;

    // Calculate the number of dimes to give the customer
    int dimes = calculate_dimes(cents);
    cents = cents - dimes * 10;

    // Calculate the number of nickels to give the customer
    int nickels = calculate_nickels(cents);
    cents = cents - nickels * 5;

    // Calculate the number of pennies to give the customer
    int pennies = calculate_pennies(cents);
    cents = cents - pennies * 1;

    // Sum coins
    int coins = quarters + dimes + nickels + pennies;

    // Print total number of coins to give the customer
    printf("%i\n", coins);
}

int get_cents(void)
{
    // TODO
    int num_cents;
    do
    {
        num_cents = get_int("number of cents: ");
    }
    while (num_cents < 0);
    return num_cents;
}

int calculate_quarters(int cents)
{
    // TODO
    // Calculating the number of quarters a customer should be given (e.g, if ccustomer is owed )
    // We loop through by checking if money owed is greater than a quarter and increment the number of times it;s greater
    int quarters = 0;
    while (cents >= 25)
    {
        cents -= 25;
        quarters++;
    }
        return quarters;
}

int calculate_dimes(int cents)
{
    // TODO
    // Calculating the number of dimes a customer should be given (e.g, if ccustomer is owed )
    // We loop through by checking if money owed is more than a dime and increment the count for every time the condition is met
    int dimes = 0;
    while (cents >= 10)
    {
        cents -= 10;
        dimes++;
    }
        return dimes;
}

int calculate_nickels(int cents)
{
    // TODO
    // Calculating the number of nickels a customer should be given (e.g, if ccustomer is owed )
    // We loop through by checking if money owed is more than a nickel and increment the count for every time the condition is met
    int nickels = 0;
    while (cents >= 5)
    {
        cents -= 5;
        nickels++;
    }
        return nickels;
}

int calculate_pennies(int cents)
{
    // TODO
    // Calculating the number of pennies a customer should be given (e.g, if ccustomer is owed )
    // We loop through by checking if money owed is more than a pennies and increment the count for every time the condition is met
    int pennies = 0;
    while (cents >= 1)
    {
        cents -= 1;
        pennies++;
    }
        return pennies;
}
