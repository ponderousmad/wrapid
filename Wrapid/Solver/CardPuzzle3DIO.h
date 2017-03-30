#pragma once
#ifndef SOLVER_CARDPUZZLE3DIO_H__INCLUDED
#define SOLVER_CARDPUZZLE3DIO_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Solver/Grid.h"
#include "Solver/GridValues.h"
#include "Solver/PuzzleIOUtils.h"

#include <iostream>

namespace Solver
{
    /*
     * Text based IO for card puzzles in 3D.
     * Puzzles are structured like so:
    const char* puzzle[13] = {
        "+----+----+----+ +----+----+----+ +----+----+----+",
        "|   o|         | |    |        o| |    |         |",
        "+    +    +----+ +    +    +----+ +    +    +----+",
        "|    |    |    | |    |    |    | |    |    |    |",
        "+    +    +    + +    +    +    + +    +    +    +",
        "|  3 |    |    | |    |    |    | |    |    |  K |",
        "+    +    +    + +    +    +    + +    +    +    +",
        "|    |    |    | |   o|    |   o| |    |    |    |",
        "+    +    +    + +    +    +    + +    +    +    +",
        "|         |   o| |         |    | |    |  ? |    |",
        "+----+    +----+ +----+----+    + +    +    +----+",
        "|              | |       A      | |   o|         |",
        "+----+----+----+ +----+----+----+ +----+----+----+"
    };

    The symbols are the numbers 2 through 10 and A, J, Q, K, with
    four of each, just like a deck of cards, with two '?'s for the jokers.
    Here the 'o's represent holes from that level to the next
    level. An 'o' in the rightmost level would represent a tunnel
    back to the first level. The 'o' must occur in the forth
    character of the cell, and thus the symbol must be only in the first three.
    */
    ParseResult parse(Grid3D& grid, GridValues3D& values, const PuzzleIOUtils::Strings& puzzle);
    void print(std::ostream& stream, const Grid3D& grid, const GridValues3D& values);
}

#endif // SOLVER_CARDPUZZLE3DIO_H__INCLUDED
