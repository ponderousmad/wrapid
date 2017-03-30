#pragma once
#ifndef SOLVER_PUZZLESOLVER_H__INCLUDED
#define SOLVER_PUZZLESOLVER_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Solver\GridSolver.h"
#include "Solver\PuzzleIOUtils.h"

namespace Solver
{
    template <int Dimensions>
    class PuzzleSolver
    {
        PREVENT_COPY_AND_ASSIGNMENT(PuzzleSolver);
    public:
        PuzzleSolver(Coordinate<Dimensions> size, int symbols, int repeats, bool includeJoker = false)
            : mGrid(size)
            , mSequence(symbols, repeats)
            , mIsSolution(false)
        {
            if(includeJoker)
            {
                mSequence.addJoker();
            }
        }

        ~PuzzleSolver()
        {
        }

        ParseResult parse(const char** puzzle)
        {
            return parse(Solver::asStrings(puzzle, mGrid.getSize()));
        }

        ParseResult parse(const std::vector<const std::string>& puzzle)
        {
            GridValues<Dimensions> values(mGrid.getSize());
            ParseResult result = Solver::parse(mGrid, values, puzzle);
            if(result == Solver::kParseSucceed)
            {
                mSolver = std::auto_ptr< GridSolver<Dimensions> >(
                    new GridSolver<Dimensions>(mGrid, mSequence)
                );

                mSolver->addPresets(values);
            }
            return result;
        }

        Solver::SolveResult findNextSolution()
        {
            assert(mSolver.get() != NULLPTR);
            return mSolver->nextSolution();
        }

        void print(std::ostream& stream) const
        {
            assert(mSolver.get() != NULLPTR);
            Solver::print(
                stream,
                mGrid,
                mSolver->getSolution()
            );
        }

    private:
        Grid<Dimensions> mGrid;
        Sequence mSequence;
        bool mIsSolution;
        std::auto_ptr< GridSolver<Dimensions> > mSolver;
    };
}

#endif // SOLVER_PUZZLESOLVER_H__INCLUDED
