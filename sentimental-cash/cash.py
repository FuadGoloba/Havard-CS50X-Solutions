# Implement a program that calculates the minimum number of coins required to give a user change.

from cs50 import get_float

# Using recursive algorithm to solve


def cash(change):
    # define a global count to track the count of coins
    global count
    # we declare a maximum integer from the change selection
    m = max(l)

    # define a base case; that returns the count of coins if the change is found in the list of avaioable coins
    if change in l:
        count += 1
        return count
    # begin to take out the maximum coin amount from the user's change and increment the count
    if change > m:
        change = round(change - m, 2)
        count += 1

    # if the change is less than the max coin, we find the next maximum and check again
    elif change < m:
        l.remove(m)
    # recursively call the function
    return cash(change)


count = 0
# list of available coins
l = [0.25, 0.1, 0.05, 0.01]
change = get_float('change owed: ')

# Handling non-ve input
while change <= 0:
    change = get_float('change owed: ')
print(cash(change))