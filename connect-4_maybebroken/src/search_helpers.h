#include "bitboard.h"

#include <array>
#include <utility> // std::pair


/// @brief A struct to store a move and its associated number of winning positions.
struct ScoredMove
{
    // the tentative move only
    Bitboard move;

    // number of winning positions
    int score;
};



/// @brief Check whether a given bitboard contains a winning piece configuration.
/// @param player_pieces The player's pieces
/// @return Whether a win was found.
constexpr bool check_win(Bitboard player_pieces)
{
    /* HORIZONTAL */
    // set mask to player pieces having another piece to their left (decrease index by 7)
    Bitboard mask = player_pieces & (player_pieces >> 7);
    // check if 2 consecutive runs of 2 pieces can be found
    if (mask & (mask >> 14)) return true; // found a sequence


    /* DIAGONAL '\' */
    if (mask & (mask >> 12)) return true; // found a sequence


    /* DIAGONAL '/' */
    mask = player_pieces & (player_pieces >> 8); // shift to bottom left
    if (mask & (mask >> 16)) return true; // found a sequence


    /* VERTICAL */
    mask = player_pieces & (player_pieces >> 1); // shift down
    if (mask & (mask >> 2)) return true; // found a sequence
    
    return false; // found nothing
}


/// @brief Generate all legal moves from a position.
/// @param our_bb Our pices.
/// @param their_bb Our opponent's pieces.
/// @return A bitboard of all our legal moves.
constexpr Bitboard possible_moves(Bitboard our_bb, Bitboard their_bb)
{
    // All ranks are stacks of 1's. As such, adding the bottom tile of each
    // file forces a carry ripple and sets the first non-empty cell of the file to a 1, while
    // clearing the other ones. Any ripples into the sentinel are cleared by a bitwise AND
    // with the valid tiles.
    return ((our_bb|their_bb) + Rank1_BB) & All_Tiles_BB;
}

/// @brief Generate all tiles where we would win if we placed a stone.
/// @param our_bb Our pieces.
/// @param their_bb Our opponent's pieces.
/// @return A bitboard of our winning positions.
constexpr Bitboard winning_positions(Bitboard our_bb, Bitboard their_bb)
{   
    /* VERTICAL */
    // do some shifts up
    Bitboard result = (our_bb << 1) & (our_bb << 2) & (our_bb << 3);


    /* HORIZONTAL */
    Bitboard partial = (our_bb << 7) & (our_bb << (2*7)); // shift right 1x + 2x

    // check for missing stone at very right of sequence (shift right 3x)
    result |= (partial & (our_bb << (7*3)));

    // check for missing stone at second tile from right of sequence (shift left 1x)
    result |= (partial & (our_bb >> 7));

    // shift partial 3x left
    partial >>= (3*7);

    // check for missing stone at second tile from left of sequence (shift right 1x)
    result |= (partial & (our_bb << 7));

    // check for missing stone at very left of sequence (shift left 3x)
    result |= (partial & (our_bb >> (7*3)));



    /* DIAGONAL '\' */
    partial = (our_bb << 6) & (our_bb << (2*6)); // shift down-right 1x + 2x

    // check for missing stone at very right of sequence (shift down-right 3x)
    result |= (partial & (our_bb << (6*3)));

    // check for missing stone at second tile from right of sequence (shift up-left 1x)
    result |= (partial & (our_bb >> 6));

    // shift partial 3x up-left
    partial >>= (3*6);

    // check for missing stone at second tile from left of sequence
    // (shift down-right 1x)
    result |= (partial & (our_bb << 6));

    // check for missing stone at very left of sequence (shift up-left 3x)
    result |= (partial & (our_bb >> (6*3)));
    

    
    /* DIAGONAL '/' */
    partial = (our_bb << 8) & (our_bb << (2*8)); // shift up-right 1x + 2x

    // check for missing stone at very right of sequence (shift up-right 3x)
    result |= (partial & (our_bb << (8*3)));

    // check for missing stone at second tile from right of sequence (shift down-left 1x)
    result |= (partial & (our_bb >> 8));

    // shift partial 3x down-left
    partial >>= (3*8);

    // check for missing stone at second tile from left of sequence
    // (shift up-right 1x)
    result |= (partial & (our_bb << 8));

    // check for missing stone at very left of sequence (shift down-left 3x)
    result |= (partial & (our_bb >> (8*3)));

    // MORE STUFF HERE

    // constrain result to empty tiles
    return (result & (All_Tiles_BB ^ (our_bb|their_bb)));
}

/// @brief Generate all legal moves which do not make us lose when our opponent replies.
/// @param our_bb Our pieces.
/// @param their_bb Our opponent's pieces.
/// @return A bitboard of all our non-losing moves.
constexpr Bitboard possible_non_losing_moves(Bitboard our_bb, Bitboard their_bb)
{
    Bitboard possible = possible_moves(our_bb, their_bb);

    // notice the argument inversion here (check if OPPONENT can win)
    Bitboard opponent_win = winning_positions(their_bb, our_bb);

    // we must play these to not lose
    Bitboard forced = possible & opponent_win;

    if (forced)
    {
        if (forced & (forced-1)) // more than one opponent win possible
        {
            return 0;
        }
        else // only one forced move
        {
            possible = forced;
        }
    }

    // don't play under a winning position
    return possible & ~(opponent_win >> 1);
}



/// @brief Sort all possible moves for a given position by the number of winning positions (empty slots) they create.
/// @param our_bb Our pieces.
/// @param their_bb Our opponent's pieces.
/// @return A pair of the number of possible moves (n) and of an array of 7 ScoredMove, of which indices [0, n[ are valid moves. Index n-1 is best.
inline auto sort_moves(Bitboard our_bb, Bitboard their_bb)
{
    Bitboard possible = possible_non_losing_moves(our_bb, their_bb);

    // not pre-initialized, so it's not a constexpr function
    std::array<ScoredMove, 7> move_arr;

    // used to count iterations
    int i = 0;
    for ( ; possible != Empty_BB; ++i)
    {
        // isolate LS set bit
        Bitboard tentative_move_only = possible & -possible;
        
        // clear bit from possible moves
        possible ^= tentative_move_only;
        
        Bitboard tentative_move = (tentative_move_only | our_bb);

        // set current entry to tentative move and score after move
        move_arr[i] = ScoredMove{tentative_move, popcount(winning_positions(tentative_move, their_bb))};

        // sort current move into array (insertion sort)
        for (int j = i; j > 0 && (move_arr[j-1].score > move_arr[j].score); --j)
        {
            std::swap(move_arr[j-1], move_arr[j]);
        }
    }

    return std::make_pair(move_arr, i);
}