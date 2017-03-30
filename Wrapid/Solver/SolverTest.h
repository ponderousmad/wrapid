#pragma once
#ifndef SOLVER_SOLVERTEST_H__INCLUDED
#define SOLVER_SOLVERTEST_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Test.h"
namespace Solver
{
    class SolverTest : public UnitTest::Framework
    {
    public:
        template <class SolverT>
        void testSolver(
            const char** puzzle,
            const std::string& target,
            int targetCount,
            int maxSolves,
            bool logSolutions = false
        )
        {
            SolverT solver;
            CHECK_ASSERT(solver.parse(puzzle) == Solver::kParseSucceed);

            bool foundSolution = false;
            int count = 0;
            Solver::SolveResult result;
            do
            {
                result = solver.findNextSolution();
                if(result == Solver::kFoundSolution)
                {
                    ++count;
                    std::ostringstream stream;
                    solver.print(stream);
                    std::string solution = stream.str();
                    if(solution == target)
                    {
                        foundSolution = true;
                    }
                    if(logSolutions)
                    {
                        std::cout << solution;
                    }
                }
            } while(result == Solver::kFoundSolution && count < maxSolves);
            CHECK(foundSolution);
            CHECK(count == targetCount);
        }
    };
}

#endif // SOLVER_SOLVERTEST_H__INCLUDED
