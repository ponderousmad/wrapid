#pragma once
#ifndef SOLVER_HOURPUZZLE_H__INCLUDED
#define SOLVER_HOURPUZZLE_H__INCLUDED

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
    class HourPuzzle;

    template <int Dimensions>
    class PuzzleSolver;
}

class Solver::HourPuzzle
{
    PREVENT_COPY_AND_ASSIGNMENT(HourPuzzle);
public:
    HourPuzzle();
    ~HourPuzzle();

    // See HourPuzzleIO for required format.
    ParseResult parse(const char** puzzle);
    ParseResult parse(const std::vector<std::string>& puzzle);

    SolveResult findNextSolution();

    void print(std::ostream& stream) const;

private:
    std::auto_ptr< PuzzleSolver<2> > mSolver;
};

#endif // SOLVER_HOURPUZZLE_H__INCLUDED
