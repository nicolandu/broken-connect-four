#pragma once

#include <cstdint>

constexpr auto NUM_STONES = 42; // 7x6 board
constexpr auto IDS_START_DEPTH = 10; // start depth for iterative deepening

constexpr uint_fast64_t RNG_SEED = 1; // change if unsatisfactory

constexpr size_t TT_STORAGE_BITS = 26; // 64M entries (~80MB)
// Chinese remainder theorem states that 2^TT_STORAGE_BITS and TT_NUM_entries need to be coprime.
// See < http://blog.gamesolver.org/solving-connect-four/11-optimized-transposition-table/ > for more details.
constexpr size_t TT_NUM_ENTRIES = (1ULL << TT_STORAGE_BITS) + 1;

constexpr int8_t TT_NOT_FOUND = INT8_MIN; // used when value is not found