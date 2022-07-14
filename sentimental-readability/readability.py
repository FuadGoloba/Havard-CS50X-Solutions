# TODO
import re
from cs50 import get_string

# Getting user input
text = get_string('text: ')

# Getting letter count using regex
# Find all letters and getting their count
letterRegex = re.compile(r'[aA-zZ]')
letter_count = len(letterRegex.findall(text))

# Getting word count using Regex
# FInd all words in text and get the count of words
wordRegex = re.compile(r'''([aA-zZ'-]+)''')
word_count = len(wordRegex.findall(text))

# Find all sentences in the text and get their count
# Sentences are indicated as end of (.!?)
#sentenceRegex = re.compile(r'''[aA-zZ']+[!?.]''')
sentenceRegex = re.compile(r'''\w?\s*[^.!?]*[.!?]''')
sentence_count = len(sentenceRegex.findall(text))
sentence_count

# calculating average letters and sentences per 100 words
L = (letter_count/word_count) * 100
S = (sentence_count/word_count) * 100

# Calculating Coleman-Liau index for readablility
CL_index = round(((0.0588 * L) - (0.296 * S) - 15.8))

# Determining the grade of each index
if CL_index < 1:
    print('Before Grade 1')
elif CL_index >= 16:
    print('Grade 16+')
else:
    print(f'Grade {CL_index}')

