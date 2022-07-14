import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) < 3:
        sys.exit("Usage: python dna.py CSVfile TEXTfile")

    # TODO: Read database file into a variable
    database = []
    filename_csv = sys.argv[1]c
    with open(filename_csv, 'r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            database.append(row)

    # TODO: Read DNA sequence file into a variable
    #stores the sequence from text into a variable
    sequence = ''
    filename_txt = sys.argv[2]
    with open(filename_txt, 'r') as file:
        sequence += file.read()

    # TODO: Find longest match of each STR in DNA sequence

    # creating a list of all STR from the database
    #STR_list = [col for col, values in database[0].items()]
    STR_list = list(database[0].keys())

    # create a dictionary to map each STR to count of its appearance in DNA sequence
    match = {}
    for subsequence in STR_list:
        length_of_subsequence = str(longest_match(sequence, subsequence))
        match[subsequence] = match.setdefault(subsequence, length_of_subsequence)

    # TODO: Check database for matching profiles

    # return name if there's a match
    for profile in database:
        match['name'] = profile['name']
        if profile == match:
            return profile['name']

    return 'No match'


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


if __name__ == '__main__':

    print(main())
