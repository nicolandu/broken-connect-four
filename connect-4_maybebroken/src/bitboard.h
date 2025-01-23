/*
   Files named a-g, ranks named 1-6 (+ 1 sentinel rank)

   a1 is bit 0, a6 is bit 5, b1 is bit 7, and so on.

   A good part of this code is adapted from Stockfish.
*/

#pragma once


#include <cstdint>


enum Square : int
{
    SQ_A1, SQ_A2, SQ_A3, SQ_A4, SQ_A5, SQ_A6, SQ_A_SENTINEL,
    SQ_B1, SQ_B2, SQ_B3, SQ_B4, SQ_B5, SQ_B6, SQ_B_SENTINEL,
    SQ_C1, SQ_C2, SQ_C3, SQ_C4, SQ_C5, SQ_C6, SQ_C_SENTINEL,
    SQ_D1, SQ_D2, SQ_D3, SQ_D4, SQ_D5, SQ_D6, SQ_D_SENTINEL,
    SQ_E1, SQ_E2, SQ_E3, SQ_E4, SQ_E5, SQ_E6, SQ_E_SENTINEL,
    SQ_F1, SQ_F2, SQ_F3, SQ_F4, SQ_F5, SQ_F6, SQ_F_SENTINEL,
    SQ_G1, SQ_G2, SQ_G3, SQ_G4, SQ_G5, SQ_G6, SQ_G_SENTINEL,
    NUM_BB_SQUARE_BITS // (1 after last)
};


enum File : int
{
    File_A, File_B, File_C, File_D, File_E, File_F, File_G
};

constexpr File operator++(File& f)
{
    return f = static_cast<File>(f+1);
}

constexpr File operator++(File& f, int)
{
    File orig = f;
    ++f;
    return orig;
}

constexpr File operator--(File& f)
{
    return f = static_cast<File>(f-1);
}

constexpr File operator--(File& f, int)
{
    File orig = f;
    --f;
    return orig;
}



using Bitboard = uint64_t;




/// @brief Construct a bitboard from a square.
/// @param s The input square
/// @return The bitboard
constexpr Bitboard square_bb(Square s)
{
    return 1ULL << s;
}

/// @brief Construct a square bitboard at a given file and rank.
/// @param file The square's file
/// @param rank The square's rank
/// @return The bitboard
constexpr Bitboard square_bb(int file, int rank)
{
    return 1ULL << ((7*file) + rank);
}

/// @brief Count the population (of set bits) within a bitboard.
/// @param bb The bitboard
/// @return The count
constexpr int popcount(Bitboard bb)
{
    return __builtin_popcountll(bb);
}

/* get square of least significant set bit of BB
 * invariant : bb != 0
 */

/// @brief Construct a square from a bitboard.
/// @param bb The input bitboard
/// @return The square for the least significant set bit. Undefined behaviour for the empty bitboard.
constexpr Square bb_square(Bitboard bb)
{
    return static_cast<Square>(__builtin_ctzll(bb)); // count zeros, long long (64-bit)
}


// Overloads of bitwise operators between a Bitboard and a Square for testing
// whether a given bit is set in a bitboard, and for setting and clearing bits.

constexpr Bitboard  operator&( Bitboard  b, Square s) { return b &  square_bb(s); }
constexpr Bitboard  operator|( Bitboard  b, Square s) { return b |  square_bb(s); }
constexpr Bitboard  operator^( Bitboard  b, Square s) { return b ^  square_bb(s); }
constexpr Bitboard& operator|=(Bitboard& b, Square s) { return b |= square_bb(s); }
constexpr Bitboard& operator^=(Bitboard& b, Square s) { return b ^= square_bb(s); }

constexpr Bitboard  operator&(Square s, Bitboard b) { return b & s; }
constexpr Bitboard  operator|(Square s, Bitboard b) { return b | s; }
constexpr Bitboard  operator^(Square s, Bitboard b) { return b ^ s; }

constexpr Bitboard  operator|(Square s1, Square s2) { return square_bb(s1) | s2; }




constexpr Bitboard Empty_BB = 0ULL;


// these do not include the sentinel rank

constexpr Bitboard FileA_BB = (SQ_A1|SQ_A2|SQ_A3|SQ_A4|SQ_A5|SQ_A6);
constexpr Bitboard FileB_BB = FileA_BB << (7 * 1);
constexpr Bitboard FileC_BB = FileA_BB << (7 * 2);
constexpr Bitboard FileD_BB = FileA_BB << (7 * 3);
constexpr Bitboard FileE_BB = FileA_BB << (7 * 4);
constexpr Bitboard FileF_BB = FileA_BB << (7 * 5);
constexpr Bitboard FileG_BB = FileA_BB << (7 * 6);


// accounting for sentinel

constexpr Bitboard Rank1_BB = (SQ_A1|SQ_B1|SQ_C1|SQ_D1|SQ_E1|SQ_F1|SQ_G1);
constexpr Bitboard Rank2_BB = Rank1_BB << 1;
constexpr Bitboard Rank3_BB = Rank1_BB << 2;
constexpr Bitboard Rank4_BB = Rank1_BB << 3;
constexpr Bitboard Rank5_BB = Rank1_BB << 4;
constexpr Bitboard Rank6_BB = Rank1_BB << 5;
constexpr Bitboard Rank_Sentinel_BB = Rank1_BB << 6; // sentinel rank

// All tiles, without sentinel rank
constexpr Bitboard All_Tiles_BB =
                    (Rank1_BB|Rank2_BB|Rank3_BB|Rank4_BB|Rank5_BB|Rank6_BB);