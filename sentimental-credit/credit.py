# TODO

import re
from cs50 import get_string

# Using Regex to validate that user's input is a sequence of length of correct digits
creditRegex = re.compile(r'(\d{4}(-)?){3}\d{3}')

# getting user input
creditNum = get_string('Number: ')

# Handling wrong user input
try:
    m = creditRegex.search(creditNum)
    num = m.group()

except:
    num = str(0)

if num[0] == '3':
    print('AMEX')
elif num[0] == '5':
    print('MASTERCARD') 
elif num[0] == '4':
    print('VISA')
else:
    print('INVALID')
