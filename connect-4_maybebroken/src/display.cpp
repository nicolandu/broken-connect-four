#include "display.h"


void display_bitboard(Bitboard board)
{
    for (int rank = 5; rank >= 0; --rank)
    {
        for (int file = 0; file < 7; ++file)
        {
            std::cout << ((board >> ((file * 7) + rank) ) & 1 ? "\x1B[31mX" : "\x1B[0m.") ;
        }
        std::cout << '\n';
    }
    std::cout << "\x1B[0m\n";
}

void display_bitboard_2player(Bitboard player1, Bitboard player2)
{
    for (int rank = 5; rank >= 0; --rank)
    {
        for (int file = 0; file < 7; ++file)
        {
            if ( (player1 >> ((file * 7) + rank) ) & 1)
            {
                std::cout << "\x1B[31mX";
            }
            else if ( (player2 >> ((file * 7) + rank) ) & 1)
            {
                std::cout << "\x1B[33mX";
            }
            else
            {
                std::cout << "\x1B[0m.";
            }
        }
        std::cout << '\n';
    }
    std::cout << "\x1B[0m\n";
}


void display_bitboard_2player_hlred(Bitboard player1, Bitboard player2, Bitboard highlight)
{
    for (int rank = 5; rank >= 0; --rank)
    {
        for (int file = 0; file < 7; ++file)
        {
            if ( (highlight >> ((file * 7) + rank) ) & 1)
            {
                std::cout << "\x1B[30;41mX\x1B[0m";
            }
            else if ( (player1 >> ((file * 7) + rank) ) & 1)
            {
                std::cout << "\x1B[31mX";
            }
            else if ( (player2 >> ((file * 7) + rank) ) & 1)
            {
                std::cout << "\x1B[33mX";
            }
            else
            {
                std::cout << "\x1B[0m.";
            }
        }
        std::cout << '\n';
    }
    std::cout << "\x1B[0m\n";
}