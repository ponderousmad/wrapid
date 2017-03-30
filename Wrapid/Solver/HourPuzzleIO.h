#pragma once
#ifndef SOLVER_HOURPUZZLEIO_H__INCLUDED
#define SOLVER_HOURPUZZLEIO_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Solver/Grid.h"
#include "Solver/GridValues.h"
#include "Solver/PuzzleIOUtils.h"

#include <iostream>

/*
    * Text based IO for hour puzzles.
    * Puzzles are structured like so:
const char* puzzle[9] = {
    "+----+----+----+----+----+----+",
    "| 12 |              |       7 |",
    "+    +    +----+    +----+    +",
    "|    |    |    |              |",
    "+    +    +    +    +----+----+",
    "|         |                   |",
    "+----+    +----+----+----+    +",
    "|  9                        2 |",
    "+----+----+----+----+----+----+"
};
*/
namespace Solver
{
    ParseResult parse(Grid2D& grid, GridValues2D& values, const PuzzleIOUtils::Strings& puzzle);
    void print(std::ostream& stream, const Grid2D& grid, const GridValues2D& values);
}



#endif // SOLVER_HOURPUZZLEIO_H__INCLUDED