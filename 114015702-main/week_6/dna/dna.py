import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py FILENAMEFORDATABASE FILENAMEFORSEQUECE ")

    filename_for_database = sys.argv[1]
    filename_for_sequence = sys.argv[2]

    # TODO: Read database file into a variable
    db = []
    subsequences = []

    with open(filename_for_database, 'r') as file:
        csv_reader = csv.DictReader(file)

        subsequences = csv_reader.fieldnames[1:]

        for row in csv_reader:
            db.append(row)


    # TODO: Read DNA sequence file into a variable

    sequence = ''

    with open(filename_for_sequence, 'r') as file:
        csv_reader = csv.reader(file)
        sequence = next(csv_reader)[0]

    longest_matches = {}

    # TODO: Find longest match of each STR in DNA sequence
    for subsequence in subsequences:
        longest_matches[subsequence] = longest_match(sequence, subsequence)

    # TODO: Check database for matching profiles

    for row in db:
        people_match_result = all([longest_matches[subsequence] == int(row[subsequence]) for subsequence in subsequences])

        if people_match_result:
            print(row["name"])
            return True

    print("No match")
    return False


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


main()
