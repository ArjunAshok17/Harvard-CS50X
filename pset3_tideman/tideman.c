#include <cs50.h>
#include <stdio.h>
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

bool check_cycle(int orig_winner, int cur_winner);
void print_pairs(void);
void print_locks(void);
// void merge_sort_pairs(int s, int e);
// void merge_arrays(int s1, int s2, int e2);

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
    // TODO
    // check each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // update the preferences in ranks if the name matches
        if (strcmp(name, candidates[i]) == 0)
        {
            // store the candidates's index in the associated rank
            // NOT rank - 1 because index 0 is tied to 1st rank but index 0 is passed in
            // only printed as index + 1 for the user
            ranks[rank] = i;

            // succesful operation
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // for every ranking given
    for (int i = 0, cand; i < candidate_count; i++)
    {
        // current candidate (index)
        cand = ranks[i];

        // for each remaining candidate in ranks
        for (int j = i + 1; j < candidate_count; j++)
        {
            // increase ranking versus the candidates ranked below current
            preferences[cand][ranks[j]]++;
        }
    }

    // end function
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // for every row in the preferences array
    for (int i = 0, ind = 0; i < candidate_count; i++)
    {
        // for every element in the diagonal and to the right
        //   length of each subarray decreases by the row number
        for (int j = i; j < candidate_count; j++)
        {
            // if the current candidate's ratio against the opposing candidate
            // is greater
            if (preferences[i][j] > preferences[j][i])
            {
                // add the pair to the array
                pairs[ind].winner = i;
                pairs[ind++].loser = j;

                // updates number of pairs
                pair_count++;
            }
            // check opposite case, non-inclusive of == case
            else if (preferences[i][j] < preferences[j][i])
            {
                // add the pair to the array
                pairs[ind].winner = j;
                pairs[ind++].loser = i;

                // updates number of pairs
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // // call merge sort function
    // merge_sort_pairs(0, pair_count - 1);

    // selection sort (max selection, not min, for non-ascending order)
    // to swap pairs
    pair swap_pair;

    // for every element in pairs
    for (int i = 0, max_mv, max_index, cur_mv; i < pair_count - 1; i++)
    {
        // initialize to current margin of victory
        max_mv = preferences[pairs[i].winner][pairs[i].loser];
        max_index = i;

        // find max
        // start @ i because previous elements need to be ignored to find the next max
        for (int j = i + 1; j < pair_count; j++)
        {
            // initialize to voter difference of the candidate we are currently checking
            cur_mv = preferences[pairs[j].winner][pairs[j].loser];

            // if current voter difference is greater than greatest recorded difference thus far
            if (cur_mv > max_mv)
            {
                // swap & update index of max
                max_mv = cur_mv;
                max_index = j;
            }
        }

        // swap so current pair is the maximum margin of victory of the remaining subarray
        swap_pair = pairs[i];
        pairs[i] = pairs[max_index];
        pairs[max_index] = swap_pair;
    }

    // end function
    return;
}

// // merge sort function
// void merge_sort_pairs(int s, int e)
// {
//     // merge the two halves of the array
//     // (assume they're sorted since recursive calling will
//     //  ensure they are sorted before merging)

//     // check base case (array is at least a single element)
//     if (s < e)
//     {
//         // helper variable, midpoint
//         int m = (e - s) / 2;

//         // merge sort left half
//         merge_sort_pairs(s, m);
//         // merge sort right half
//         merge_sort_pairs(m + 1, e);

//         // merge both halves
//         merge_arrays(s, m, e);
//     }
// }

// // merge helper functions
// void merge_arrays(int s1, int e1, int e2)
// {
//     // s1 - start of first subarray, e1 - end of first subarray
//     // s2 - start of second subarray, e2 - end of second subarray
//     // s2 = e1 always
//     int s2 = e1 + 1;
//     int s = s1;

//     // subarray lengths
//     int l1 = e1 - s1, l2 = e2 - s2;

//     // s1 & s2 will serve as the indices for comparing elements
//     pair merged[e2 - s1];

//     // for the total number of elements we are comparing,
//     // merge into new pairs array
//     for (int i = 0, l = e2 - s1, n1, n2; i < l; i++)
//     {
//         // set the winning votes
//         // prevent index out of bounds for subarrays
//         if (s1 == l1)
//         {
//             n1 = -1;
//         }
//         else
//         {
//             // set to the corresponding pair's difference in voter preferences
//             n1 = preferences[pairs[s1].winner][pairs[s1].loser] - preferences[pairs[s1].loser][pairs[s1].winner];
//         }
//         // prevent index out of bounds for subarrays
//         if (s2 == l2)
//         {
//             n2 = -1;
//         }
//         else
//         {
//             // set to the corresponding pair's recorded voter preferences
//             n2 = preferences[pairs[s2].winner][pairs[s2].loser] - preferences[pairs[s2].loser][pairs[s2].winner];
//         }

//         // start merging
//         // if winner count is higher or equal for the first subarr element
//         if (n1 >= n2)
//         {
//             // set merged array's next element to the first subarr element
//             merged[i] = pairs[s1++];
//         }
//         // if winner count is not higher or equal for first subarr element
//         else
//         {
//             // set merged array's next element to the first subarr element
//             merged[i] = pairs[s2++];
//         }
//     }

//     // recombine the merged array with the original pairs array
//     for (int i = 0; i < e2; i++)
//     {
//         // every pair is rechosen from the sorted array
//         pairs[i + s] = merged[i];
//     }

//     // end function
//     return;
// }

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // in order of decreasing victory strength (as indicated by sorted pairs array),
    // create an edge from the respective pair's winner to the loser
    print_pairs();

    for (int i = 0, winner, loser; i < pair_count; i++)
    {
        // declare variables
        winner = pairs[i].winner;
        loser = pairs[i].loser;

        // if a cycle wouldn't be created
        bool cycle = check_cycle(winner, loser);

        if (!cycle)
        {
            // create edge
            locked[winner][loser] = true;
        }

        print_locks();
    }

    // end function
    return;
}

bool check_cycle(int orig_winner, int cur_winner)
{
    // checks if a cycle has been created until you either:
    //      1. End up at the same node -> cycle was created, return true
    //      2. Counter exceeds the number of candidates
    //      2. End up at a candidate with no other connections -> no cycle, return false

    // 1. base case
    if (orig_winner == cur_winner)
    {
        // it IS a cycle since it looped
        return true;
    }

    // 2. general recursive case
    for (int j = 0; j < candidate_count; j++)
    {
        // if the potential loser has won a standoff
        // you run check_cycle here because otherwise you'll always end up returning true
        if (locked[cur_winner][j] && check_cycle(orig_winner, j))
        {
            // check if that potential loser eventually cycles back to our original pair
            // j becomes the next current winner
            return true;
        }
    }

    // it ISN'T a cycle since it didn't loop and ended at a candidate with no winning (last candidate)
    // this works since the algorithm ensures there will always be one loser, so assuming the check_cycle
    // worked in the previous pairs will enable us to make the assumption here
    return false;
}

void print_pairs(void)
{
    printf("\n{");
    for (int i = 0; i < pair_count; i++)
    {
        printf(" [%i, %i] ", pairs[i].winner, pairs[i].loser);
    }
    printf("}\n");

    return;
}

void print_locks(void)
{
    printf("\n{");
    for (int i = 0; i < candidate_count; i++)
    {
        printf(" {");
        for (int j = 0; j < candidate_count; j++)
        {
            printf(" %s ", locked[i][j] ? "true" : "false");
        }
        printf("}  ");
    }
    printf(" }\n");

    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // go through graph
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0, k; j < candidate_count; j++)
        {
            // if the candidate is preferred over someone else
            if (locked[i][j])
            {
                // check every connection to the current candidate (ith column)
                for (k = 0; k < candidate_count; k++)
                {
                    // exclude the current candidate
                    // if another candidate is prefered to the current
                    // not source, therefore break and go to next candidate;
                    if (k != i && locked[k][i])
                    {
                        // break out of both loops, next candidate
                        j = candidate_count;
                        k = candidate_count + 1; // + 1 for checking success
                    }
                }

                // if the for-loop has run its entire course without breaking for loops
                if (k == candidate_count)
                {
                    // print current candidate
                    printf("%s\n", candidates[i]);

                    // exit function
                    return;
                }
            }
        }
    }

    // error message
    printf("FAILED to find winner\n");

    // end function
    return;
}