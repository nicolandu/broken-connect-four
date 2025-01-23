#include "search.h"

#include "search_helpers.h"
#include "display.h"
#include "tt.h"

#include <algorithm>



// Used for checking moves from center outward.
// Files D, C, E, F, B, A, G
constexpr const Bitboard move_order[7] = {FileD_BB, FileC_BB, FileE_BB, FileF_BB, FileB_BB, FileA_BB, FileG_BB};


// See http://blog.gamesolver.org/solving-connect-four/01-introduction/ for info



int negamax(Bitboard our_bb, Bitboard their_bb, int depth_left, int alpha, int beta)
{
    auto tuple = sort_moves(our_bb, their_bb);

    auto [sorted, num_moves] = tuple;

    if (num_moves == 0) return -depth_left; // loss/draw if no non-losing moves
    
    // get TT upper bound
    int tt_val = tt.probe(our_bb, their_bb);

    // our initial upper bound, as we can't win next
    // (wouldn't be a non-losing move at our parent node)
    int max = depth_left - 1;

    // don't forget to check if entry was not found
    if (tt_val != TT_NOT_FOUND)
    {
        // set max score to the tt's upper bound for the current position
        max = tt_val;
    }

    // early beta-pruning with the new lower bound
    if (max < beta)
    {
        beta = max; // no need to keep beta above our max possible score
        if (alpha >= beta) return beta; // prune if [alpha; beta] window is empty
    }
    
    for (int i = 0; i < num_moves; ++i)
    {
        /*
        // isolate tentative move from legal moves in order
        Bitboard tentative_move_only = non_losing_moves & move_order[file_index];

        // if tentative move is nonexistent, continue
        if (tentative_move_only == Empty_BB) continue;

        // set other bits from our current BB
        Bitboard tentative_move = (tentative_move_only | our_bb);
        */

        // "Our" new move becomes the child's adversary,
        // our adversary's bitboard becomes our child's bitboard.
        // Also negate and swap alpha, beta and the result
        int score = -negamax(their_bb, sorted[i].move, depth_left-1, -beta, -alpha);

        if (score >= beta) return score; // beta cut-off
        
        // tighten alpha bound for next iteration
        if (score > alpha) alpha = score;
    }
    
    // store the new position upper bound
    tt.save(our_bb, their_bb, alpha);
    
    return alpha;
}

int root_search(Bitboard our_bb, Bitboard their_bb, bool weak)
{
    int depth_left = NUM_STONES - popcount(our_bb|their_bb);
    
    std::cout << "Depth left: " << depth_left << '\n';
    
    // check if we can win in one, as negamax function doesn't handle this case.
    Bitboard possible = possible_moves(our_bb, their_bb);
    
    if (possible == Empty_BB) return 0; // draw
    while (possible)
    {
        Bitboard move = possible & -possible; // isolate LS set bit
        possible ^= move; // clear the move from possible moves
        
        // check if we win
        if (check_win(our_bb|move)) return depth_left;
    }

    // worst we can have is opposite the number of squares left minus 1 (we play before)
    // if weak search, use null window instead
    int min = weak ? -1 : (-depth_left - 1);

    // best we can have is the number of squares left
    // if weak search, use null window instead
    int max = weak ? 1 : depth_left;
    
    // iteratively narrow the search window, doing a sort-of binary search
    // end when the window is empty
    while (min < max)
    {
        // round difference between bounds AWAY from 0
        int mdp = min + (max-min)/2;

        // restrict abs(midpoint) to be lower than
        // halfway between 0 and either min or max
        if(mdp <= 0 && min/2 < mdp) mdp = min/2;
        else if(mdp >= 0 && max/2 > mdp) mdp = max/2;

        int score = negamax(our_bb, their_bb, depth_left, mdp, mdp+1);   // use a null depth window to know if the actual score is greater or smaller than med
        
        // if the score is worse than midpoint, make it our new max
        // if it is equal/better, make it our new min
        if(score <= mdp) max = score;
        else min = score;
    }
    
    return min; // the final minimum is our position's score
}

int find_best_move(Bitboard our_bb, Bitboard their_bb, Bitboard & best_move, bool weak)
{
    best_move = Empty_BB; // initialize best move to an empty move

    Bitboard possible = possible_moves(our_bb, their_bb);

    if (!possible) return 0; // draw if no moves left

    int value = -INT_MAX; // max player


    int depth_left = NUM_STONES - popcount(our_bb|their_bb);

    for (int col_index = 0; col_index < 7; ++col_index)
    {
        // isolate tentative move from legal moves in order
        Bitboard tentative_move_only = possible & move_order[col_index];

        // if tentative move is nonexistent, continue
        if (tentative_move_only == Empty_BB) continue;

        Bitboard tentative_move = (tentative_move_only | our_bb); // set other bits from our current BB

        // compute the value of the new position for our adversary
        // and invert it to get our value (zero-sum game)
        int tentative_value = -root_search(their_bb, tentative_move, weak);

        if (tentative_value > value) // if better than other moves
        {
            value = tentative_value;
            best_move = tentative_move_only;
        }

        std::cout << "For move\n";
        display_bitboard(tentative_move_only);
        std::cout << "at root level with depth " << depth_left << ", score is " << tentative_value << "\n\n";
    }
    
    return value;
}

/*
int root_search_deepening(Bitboard our_bb, Bitboard their_bb, int max_depth, Bitboard & best_move)
{
    for (int depth = 1; depth <= max_depth; ++depth)
    {
        // Get value at a given depth, thereby setting best_move.
        int value = root_search(our_bb, their_bb, best_move);
        
        std::cout << "Depth " << depth << ": value " << value << '\n';

        // If a win for either player was found at a given depth, no need to go deeper.
        if (value != 0) return value;
    }
    // If nothing was found (draw at max depth), return 0 (draw)
    return 0;
}
*/