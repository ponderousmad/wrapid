#pragma once
#ifndef SOLVER_CARDPUZZLE4D_H__INCLUDED
#define SOLVER_CARDPUZZLE4D_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Solver/Symbol.h"
#include "Solver/SolveResult.h"
#include "Solver/ParseResult.h"

#include <vector>
#include <iostream>
#include <memory>

namespace Solver
{
    class CardPuzzle4D;

    template <int Dimensions>
    class PuzzleSolver;
}

class Solver::CardPuzzle4D
{
    PREVENT_COPY_AND_ASSIGNMENT(CardPuzzle4D);
    struct Data;

public:
    CardPuzzle4D();
    ~CardPuzzle4D();

    // See CardPuzzle4DIO for required format.
    ParseResult parse(const char** puzzle);
    ParseResult parse(const std::vector<const std::string>& puzzle);

    SolveResult findNextSolution();

    void print(std::ostream& stream) const;

private:
    std::auto_ptr< PuzzleSolver<4> > mSolver;
};

#endif // SOLVER_CARDPUZZLE4D_H__INCLUDED
