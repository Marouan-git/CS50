#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
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
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool col_false(bool locked[][MAX], int col);

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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int pairs_index = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pair new_pair;
                new_pair.winner = i;
                new_pair.loser = j;
                pairs[pairs_index] = new_pair;
                pair_count++;
                pairs_index++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pair new_pair;
                new_pair.winner = j;
                new_pair.loser = i;
                pairs[pairs_index] = new_pair;
                pair_count++;
                pairs_index++;
            }
            else
            {
                continue;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int i, j, max_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < pair_count; i++)
    {
        // Find the maximum element in unsorted array
        max_idx = i;
        for (j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[max_idx].winner][pairs[max_idx].loser])
            {
                max_idx = j;
            }
        }

        // Swap the found maximum element with the first element
        pair temp = pairs[max_idx];
        pairs[max_idx] = pairs[i];
        pairs[i] = temp;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Count the number of col in locked that contain only values at false
    int nb_col_false = candidate_count;
    int i = 0;
    // Add a condition that verifies if we have at least one col with only values at false in order to not create a cycle
    for (; i < pair_count && nb_col_false > 1; i++)
    {
        // If the col is false, we must decrement nb_col_false after locking the pair
        if (col_false(locked, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            nb_col_false--;
        }
        else
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    if (i < pair_count)
    {
        for (int j = i; j < pair_count; j++)
        {
            if (!col_false(locked, pairs[j].loser))
            {
                locked[pairs[j].winner][pairs[j].loser] = true;
            }
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int j = 0; j < MAX; j++)
    {
        if (col_false(locked, j))
        {
            printf("%s\n", candidates[j]);
            break;
        }
    }
    return;
}

// Check if the col j of tab contains only values at false, if so returns true, otherwise returns false
bool col_false(bool tab[][MAX], int j)
{
    for (int i = 0; i < MAX; i++)
    {
        if (tab[i][j] == true)
        {
            return false;
        }
    }
    return true;
}

