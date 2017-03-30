#pragma once
#ifndef SYMBOL_H__INCLUDED
#define SYMBOL_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

namespace Solver
{
    typedef int Symbol;

    enum
    {
        kUnsetSymbol = 0,
        kFirstSymbol = 1,
        kHourPuzzleSymbols = 12,
        kHourPuzzleRepeats = 2,
        kCardPuzzleSuits = 4,
        kCardPuzzleAce = 1,
        kCardPuzzleJack = 11,
        kCardPuzzleQueen = 12,
        kCardPuzzleKing = 13,
        kCardPuzzleSuitSymbols = kCardPuzzleKing,
        kCardPuzzleJoker = 14,
        kCardPuzzleJokerCount = 2
    };
}

#endif // SYMBOL_H__INCLUDED
