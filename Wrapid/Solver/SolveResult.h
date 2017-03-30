#pragma once
#ifndef SOLVER_SOLVERESULT_H__INCLUDED
#define SOLVER_SOLVERESULT_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

namespace Solver
{
    enum SolveResult
    {
        // The initial setup is already a solution.
        kAlreadySolved,

        // A solution was found.
        kFoundSolution,

        // The constraints cannot be satisfied.
        kNoSolution
    };
}

#endif // SOLVER_SOLVERESULT_H__INCLUDED
