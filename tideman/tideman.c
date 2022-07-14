#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser,
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void swap(pair *x, pair *y); // Helper function to swap adjacent values in an array
void sort_pairs(void);
bool is_cycle(int end, int start); // Helper function to recursively check for a cycle in a locked pair graph
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Loop through each candidtae and update a voter's rank of candidate preference if the candidates name matches the name they inputted
    for (int i = 0; i < candidate_count; i++)
    {
        // If the inout name and candidate name matches
        if (strcmp(candidates[i],name) == 0)
        {
            // update the ranks array to indicate that the voter has the candidate as their ith preference
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Function is called for each voter and takes as argument ranks array (which represents a voter's ith preference)

    // Loop through each candidate to record the number of voters who prefer one candidate to the other
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // preferences count is increased by 1 at the position where the candidate in ranks[i] is compared to candidate in ranks[j]
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Adds all pairs of candidates where on candidate is prefereed to the other and include the add in pairs array
    // Disregard situations (tie) where one candidate is not preferred to another
    // And update the pair_count


    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Compare the number of votes of a candidate pair and determine the winner/loser between the pair
            // And then update the pair count so it moves to the next pair
            // Note: pair_count counts the number of pairs comprisons

            if (preferences[i][j] > preferences[j][i])
            {
                // determine the winner/loser between i,j
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
            // update pair count

        }
    }
    return;
}
// Helper function used in sort pairs to swap adjacennt elements in an array
void swap(pair *x, pair *y)
{
    // takes 2 arguments; 2 values in an array
    // Create a temporary variable to store the previous/first element
    pair prev = *x;
    // second element becomes the first element
    *x = *y;
    // previous element becomes the second element
    *y = prev;

}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Sorting the pairs array in descending order of strength of victory
    // Strength f vistory is the no of voters who prefer the prefereeed candidate.

    // Implementing a Bubble sort for this
    // Traverse through all elements in pair_count (length of array)
    for (int i = 0; i < pair_count - 1; i++)
    {
        // Traversing through (pair_count - i - 1) times to compare adjacent elements as last elements (to the right most side) would have been sorted every time of the outer loop
        for (int j = 0; j < pair_count-i-1; j++)
        {
            // initialise a boolean to keep track if a swap occurs or not in the inner loop
            // in order to stop the algorithm if ther's no swap (i.e it is sorted)
            bool swapped = false;
            // Check if the left element is less than its adjacent and swap if true (since we're sortung un decreasing order)
            if ((preferences[pairs[j].winner][pairs[j].loser]) < preferences[pairs[j+1].winner][pairs[j+1].loser])
            {
                // // swap elements not in order
                swap(&pairs[j], &pairs[j+1]);
            }
        }
    }
    return;
}
// A recursive function to check if a locked pair would lead to a cycle
// Returns true if a cycle is formed
bool is_cycle(int end, int start)
{
    // Note: a cycle will be formed if the end (loser) of a locked pair is the begining (winner) of another locked pair thereby formimg a clyclic graph
    // (e.g if d->a is first locked pair and a->b is second locked pair and b->d is 3rd locked pair)
    // (Here, we would have a chain like d->a->b->d which is a cyclic chain)

    // Create a base case for the recursive function; a cycle is formed if the end/loser of a current locked pair is the winner/start of another locked pair in a chain
    if (end == start)
    {
        return true;
    }
    // Traverse all of the candidates to check for locked pairs
    for (int i = 0; i < candidate_count; i++)
    {
        // for a candidate i; check that it is loser in a locked pair
        if (locked[end][i])
        {
            // if true, recursively check that candidate i is a winner in another locked pair
            // if true(i.e i == start), then cyclc is formed
            if (is_cycle(i, start))
            {
                return true;
            }
        }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TRaverse the pairs arrays and check if there exists a cycle between a pair
    for (int i = 0; i < pair_count; i++)
    {
        // if no cycle exists between the pair, lock the pair. (Remember every pair is initially initialised to false to show no lock)
        if (!is_cycle(pairs[i].loser,pairs[i].winner))
        {

            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // Winner of the election is the candidate who is the source of the graph with no arrows poiting to them
    // i.e candidate with no true values in the locked pair

    // Traverse each candidate and initialise a count for count of its false
    for (int i = 0; i < candidate_count; i++)
    {
        int count_false = 0;
        // Iterate each candidate's locked pair
        for (int j = 0; j < candidate_count; j++)
        {
            // increment the count of false belonging to the candidate
            if (locked[j][i] == false)
            {
                count_false++;
                // check if there are no true (i.e if there are 3 candidates, and a particular candidate has the same amount of false as the number of candidates)
                // Print that candidate as the winner
                if (candidate_count == count_false)
                {
                    printf("%s\n", candidates[i]);
                }
            }
        }
    }
    return;
}