#pragma once

#include <iostream>

#include "bitboard.h"

/// @brief Display a plain bitboard using ANSI colors.
/// @param board The bitboard
void display_bitboard(Bitboard board);

/// @brief Display a game situation from 2 bitboards.
/// @param player1 First player's pieces (red)
/// @param player2 Second player's pieces (yellow)
void display_bitboard_2player(Bitboard player1, Bitboard player2);

/// @brief Display a game situation from 2 bitboards, plus highlight some squares in red.
/// @param player First player's pieces (red)
/// @param player2 Second player's pieces (yellow)
/// @param highlight Pieces to highlight (red)
void display_bitboard_2player_hlred(Bitboard player1, Bitboard player2, Bitboard highlight);