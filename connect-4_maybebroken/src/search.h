#pragma once

#include "bitboard.h"

/// @brief Calculates alpha-beta value for max (red) player.
/// @param current_hash Zobrist hash upon function entry
/// @param our_bb Our pieces
/// @param their_bb Adversary's bitboard
/// @param depth Depth to go
/// @param alpha Alpha value for this node
/// @param beta Beta value for this node
/// @return Returns the exact score, an upper or lower bound score depending of the case:
/// - if actual score of position <= alpha then actual score <= return value <= alpha;
/// - if actual score of position >= beta then beta <= return value <= actual score;
/// - if alpha <= actual score <= beta then return value = actual score;
int negamax(Bitboard our_bb, Bitboard their_bb, int depth, int alpha, int beta);

/// @brief Calculates value and best move at a root node.
/// The transposition table must be cleared if the previous node search was not a direct sibling of the current position.
/// @param our_bb Our pieces.
/// @param their_bb Our opponent's pieces.
/// @param best_move This bitboard, passed by reference, receives the best move found.
/// @param weak Whether to perform a null window search to speed up the search (returns a non-optimal move).
/// @return Returns the value for the position (value > 0 means we win, value == 0 means draw, value < 0 means we lose).
/// abs(value) + 1 is the number of stones left.
int find_best_move(Bitboard our_bb, Bitboard their_bb, Bitboard & best_move, bool weak);

/// @brief Calculates value of a given root node.
/// The transposition table must be cleared if the previous node search was not a direct sibling of the current position.
/// @param our_bb Our pieces.
/// @param their_bb Our opponent's pieces.
/// @param weak Whether to perform a null window search to speed up the search (returns a non-optimal move).
/// @return 
int root_search(Bitboard our_bb, Bitboard their_bb, bool weak);