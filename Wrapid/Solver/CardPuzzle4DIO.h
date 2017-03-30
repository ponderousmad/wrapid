#pragma once
#ifndef SOLVER_CARDPUZZLE4DIO_H__INCLUDED
#define SOLVER_CARDPUZZLE4DIO_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Solver/Grid.h"
#include "Solver/GridValues.h"
#include "Solver/PuzzleIOUtils.h"

#include <iostream>

/*
    * Text based IO for card puzzles in 4D.
    * Puzzles are structured like so:
const char* puzzle[14] = {
    "+----+----+----+ +----+----+----+ +----+----+----+",
    "|   o|         | |    |        o| |    |         |",
    "+    +    +----+ +    +    +----+ +    +    +    +",
    "|    |    |   o| |    |    |@   | |    |    |  Q |",
    "+    +    +    + +    +    +    + +    +    +    +",
    "|  2 |         | |         |    | |    |@   |    |",
    "+----+----+----+ +----+----+----+ +----+----+----+",
    "+----+----+----+ +----+----+----+ +----+----+----+",
    "|    |    |    | |   o|  4 |   o| |    |    |    |",
    "+    +    +    + +    +    +    + +    +    +    +",
    "|     @   |   o| |         |    | |    |    |    |",
    "+----+    +----+ +----+----+    + +    +    +----+",
    "|              | |              | |   o|         |",
    "+----+----+----+ +----+----+----+ +----+----+----+"
};

The symbols are the numbers 2 through 10 and A, J, Q, K, with
four of each, just like a deck of cards, with two '?'s for the jokers.
Here the 'o's represent holes from that level to the next
level. An 'o' in the rightmost level would represent a tunnel
back to the first level.
Similarly, the '@'s represent holes between the top 'volume', and
the bottom one. The '@' and the 'o' must occur in the first
and forth character of the cell, resectively, and thus the symbol
must be only in the middle two.
*/
namespace Solver
{
    ParseResult parse(Grid4D& grid, GridValues4D& values, const PuzzleIOUtils::Strings& puzzle);
    void print(std::ostream& stream, const Grid4D& grid, const GridValues4D& values);
}

#endif // SOLVER_CARDPUZZLE4DIO_H__INCLUDED
