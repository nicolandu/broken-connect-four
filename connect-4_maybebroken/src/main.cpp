#include "bitboard.h"
#include "search.h"
#include "display.h"

#include <iostream>


int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    Bitboard red_bb = (SQ_A1|SQ_A2|SQ_B3|SQ_B4|SQ_C1|SQ_C3|SQ_D1|SQ_E2|SQ_F1);
    Bitboard yellow_bb = (SQ_A3|SQ_A4|SQ_B1|SQ_B2|SQ_C2|SQ_C4|SQ_D2|SQ_D3|SQ_E1);

    display_bitboard_2player(Empty_BB, Empty_BB);

    // get score for position
    int score = root_search(Empty_BB, Empty_BB, false);
    std::cout << "score : " << score << '\n';

    return EXIT_SUCCESS;
}