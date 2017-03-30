#pragma once
#ifndef SOLVER_CARDPUZZLE3D_H__INCLUDED
#define SOLVER_CARDPUZZLE3D_H__INCLUDED

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
    class CardPuzzle3D;

    template <int Dimensions>
    class PuzzleSolver;
}

class Solver::CardPuzzle3D
{
    PREVENT_COPY_AND_ASSIGNMENT(CardPuzzle3D);

public:
    CardPuzzle3D();
    ~CardPuzzle3D();

    // See CardPuzzle3DIO for required format.
    ParseResult parse(const char** puzzle);
    ParseResult parse(const std::vector<std::string>& puzzle);

    SolveResult findNextSolution();

    void print(std::ostream& stream) const;

private:
    std::auto_ptr< PuzzleSolver<3> > mSolver;
};

#endif // SOLVER_CARDPUZZLE3D_H__INCLUDED
