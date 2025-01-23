#include "tt.h"

#include <cstring>

TranspositionTable tt;

constexpr TTKey make_key(Bitboard our_bb, Bitboard their_bb)
{
    // We use the sum of the bitboard of all pieces and our bitboard as a key.
    // This is a unique, small and fast representation of the position.
    return ((our_bb | their_bb) + our_bb);
}

constexpr size_t make_index(TTKey full_key)
{
    // take full key mod our (coprime) number of entries
    return full_key % TT_NUM_ENTRIES;
}

void TranspositionTable::clear()
{
    // keys are set to 0 by default
    std::memset(m_keys, 0x00, TT_NUM_ENTRIES*sizeof(m_keys[0]));

    // values are set to INT8_MIN by default
    std::memset(m_vals, TT_NOT_FOUND, TT_NUM_ENTRIES*sizeof(m_vals[0]));
}

void TranspositionTable::save(Bitboard our_bb, Bitboard their_bb, int value_bound)
{
    TTKey full_key = make_key(our_bb, their_bb);

    // get index for entry
    size_t index = make_index(full_key);

    // only store truncated key (Chinese remainder theorem)
    m_keys[index] = static_cast<TTPartialKey>(full_key);

    // store value
    m_vals[index] = value_bound;
}

int8_t TranspositionTable::probe(Bitboard our_bb, Bitboard their_bb) const
{
    TTKey full_key = make_key(our_bb, their_bb);
    
    // get index for entry
    size_t index = make_index(full_key);

    // if the truncated key matches the computed truncated key, return the associated value
    // (Chinese remainder theorem), else there is no match
    return (m_keys[index] == static_cast<TTPartialKey>(full_key)) ? m_vals[index] : TT_NOT_FOUND;
}