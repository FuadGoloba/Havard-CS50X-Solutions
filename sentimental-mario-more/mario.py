from cs50 import get_int


while True:
    # prompt user to get the height of half pyramid
    height = get_int('Height: ')
    # check if input is in range 1 and 8 inclusive
    if height in range(1, 9):
        # loop through the height
        for i in range(1, height + 1):
            print(' '*(height-i) + ('#' * (i)) + '  ' + ('#' * (i)))
        # break as soon as the user enters the right input
        break
    else:
        print('Enter a positive integer between 1 and 8')
