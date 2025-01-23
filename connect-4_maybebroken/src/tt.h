#pragma once

#include "bitboard.h"

#include "constants.h"


// used to store a position key
using TTKey = Bitboard;

// used in conjunction with Chinese theorem to reduce key storage size
using TTPartialKey = uint32_t;

class TranspositionTable
{
public:
    /// @brief Create a TranspositionTable. Beware of its 512MB size!
    TranspositionTable()
    {
        m_keys = new TTPartialKey[TT_NUM_ENTRIES]; // uninitialized memory
        m_vals = new int8_t[TT_NUM_ENTRIES]; // uninitialized memory
        clear(); // initialize the table
    }
    
    /// @brief NO COPY CONSTRUCTOR ALLOWED 
    TranspositionTable(const TranspositionTable&) = delete;

    ~TranspositionTable()
    {
        delete m_keys;
        delete m_vals;
    }

    /// @brief Zero the transposition table's contents, deleting its entries.
    void clear();
    
    /// @brief Save an entry into the transposition table, overwriting previous entries.
    /// @param our_bb Our pieces.
    /// @param their_bb Our opponent's pieces.
    /// @param value_bound The upper bound of the position's value.
    void save(Bitboard our_bb, Bitboard their_bb, int value_bound);
    
    /// @brief Check whether an entry exists for a given key in the transposition table.
    /// @param our_bb Our pieces.
    /// @param their_bb Their pieces.
    /// @return The upper bound stored for the key if it exists, TT_NOT_FOUND if it wasn't found.
    int8_t probe(Bitboard our_bb, Bitboard their_bb) const;

private:
    TTPartialKey * m_keys; // a pointer to the full key entries
    int8_t * m_vals; // a pointer to the values
    // A full entry consists of an entry in each of these arrays.
};


// The global transposition table.
extern TranspositionTable tt;