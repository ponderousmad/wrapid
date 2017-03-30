#pragma once
#ifndef SOLVER_GRIDSOLVER_H__INCLUDED
#define SOLVER_GRIDSOLVER_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Solver/GridValues.h."
#include "Solver/Grid.h"
#include "Solver/Sequence.h"
#include "Solver/SolveResult.h"

#include <vector>
#include <algorithm>

namespace Solver
{
    template <int Dimensions>
    class GridSolver;


    /*
        This do -nothing tempate is here as an entry point to debug the
        solver algorithm for a specific dimensionality without having
        to change any other code.
        For example, if you add the following in CardPuzzle3D.cpp to
        find out what's going wrong when the solver is looking
        at location (1, 2, 3):

        namespace Solver
        {
            template <>
            void diagnose<3>(
                const Grid<3>& grid,
                const GridValues<3>& values,
                const Coordinate<3>& location,
                const std::vector<Symbol>& options
            )
            {
                if(location == coord(1, 2, 3))
                {
                    // output some information.
                }
            }
        }

        It must be added before the GridSolver template is instantiated, which
        should be in the constructor in that case.
    */
    template <int Dimensions>
    void diagnose(
        const Grid<Dimensions>&,
        const GridValues<Dimensions>&,
        const Coordinate<Dimensions>&,
        const std::vector<Symbol>&
    )
    {
    }
}

// Finds solutions to grid puzzles.
// The basic aproach is to use backtracking
template <int Dimensions>
class Solver::GridSolver
{
    PREVENT_COPY_AND_ASSIGNMENT(GridSolver);
public:
    typedef Grid<Dimensions> GridD;
    typedef Coordinate<Dimensions> Coord;
    typedef GridValues<Dimensions> Values;
    typedef std::vector<Symbol> SymbolList;
    typedef Sequence::SymbolSet SymbolSet;

    // The information needed at each choice point of the
    // backtracker. Note that the structure of the problem is that
    // we don't need to clear the locations when we pop the stack,
    // just follow the same path through the grid with different values.
    struct Stage
    {
        Stage(Coord loc, bool freeRegionStart = false)
            : location(loc)
            , isFreeRegionStart(freeRegionStart)
        {
        }

        Coord location;
        bool isFreeRegionStart;
        SymbolList options;
    };
    typedef std::vector<Stage> Stages;

    GridSolver(const GridD& grid, const Sequence& sequence)
        : mGrid(grid)
        , mTotalCount(mGrid.getTotalSize())
        , mSequence(sequence)
        , mValues(grid.getSize())
        , mStackTop(-1)
    {
#if BUILD_TESTS
        assert(mGrid.integrityCheck());
#endif
    }

    // Set the initial values on the grid prior to solve.
    void addPreset(Symbol s, Coord location)
    {
        // Don't allow adding presets after the solve has started.
        assert(mStackTop < 0);
        mValues.place(s, location);
    }

    // Set all initial values on the grid prior to solve.
    void addPresets(const Values& values)
    {
        Values::const_iterator end = values.end();
        for(Values::const_iterator it = values.begin(); it != end; ++it)
        {
            addPreset(it->second, it->first);
        }
    }

    // The top level of the backtracker.
    SolveResult nextSolution()
    {
        if(mStack.size() > 0)
        {
            // We are still sitting at the point of the last solve.
            popOption();
        }
        else if(!findNextUnsetNeighbour())
        {
            if(isSolution())
            {
                return kAlreadySolved;
            }
            else
            {
                findNextFreeLocation();
            }
        }
        while(mStackTop >= 0)
        {
            if(placeNextOption())
            {
                if(isSolution())
                {
                    return kFoundSolution;
                }
                else if(!findNextUnsetNeighbour())
                {
                    findNextFreeLocation();
                }
            }
            else
            {
                popOption();
            }
        }
        return kNoSolution;
    }

    const GridValues<Dimensions>& getSolution() const
    {
        return mValues;
    }

private:
    bool isSolution() const
    {
        return mValues.valueCount() == mTotalCount;
    }

    bool findNextUnsetNeighbour()
    {
        ++mStackTop;
        if(mStackTop == (int)mStack.size())
        {
            Coord current = mSearchLocation;
            while(isSet(current) || !hasSetNeighbour(current))
            {
                current = next(current, mGrid.getSize());
                if(current == mGrid.getSize())
                {
                    // loop back to the start.
                    current = Coord();
                }
                if(current == mSearchLocation)
                {
                    // We got back to where we started, so
                    // there's nothing to find.
                    return false;
                }
            }
            mStack.push_back(Stage(current));
            mSearchLocation = nextSearchLocation(current);
        }
        updateOptions();
        return true;
    }

    // Try to grow as directly as possible by starting the next
    // stage close to where we are now.
    Coord nextSearchLocation(const Coord& location)
    {
        GridD::Neighbours n = mGrid.getNeighbours(location, false);
        while(n.advance())
        {
            if(!isSet(n.current()))
            {
                return n.current();
            }
        }
        // Nothing good, just pick something.
        Coord nextLocation = next(location, mGrid.getSize());
        if(nextLocation == mGrid.getSize())
        {
            return Coord();
        }
        else
        {
            return nextLocation;
        }
    }

    bool isSet(const Coord& location) const
    {
        return mValues[location] != Solver::kUnsetSymbol;
    }

    bool hasSetNeighbour(const Coord& location) const
    {
        GridD::Neighbours n = mGrid.getNeighbours(location, false);
        while(n.advance())
        {
            if(isSet(n.current()))
            {
                return true;
            }
        }
        return false;
    }

    bool findNextFreeLocation()
    {
        assert(mStackTop == (int)mStack.size());
        Coord current = mSearchLocation;
        while(isSet(current))
        {
            current = next(current, mGrid.getSize());
            if(current == mGrid.getSize())
            {
                // loop back to the start.
                current = Coord();
            }
            assert(current != mSearchLocation);
        }
        mStack.push_back(Stage(current, true));
        mSearchLocation = nextSearchLocation(current);
        updateOptions();
        return true;
    }

    void updateOptions()
    {
        SymbolList& options = mStack[mStackTop].options;
        options.clear();

        if(mStack[mStackTop].isFreeRegionStart)
        {
            addConstrainedBySequence(options, mSequence.getSymbols());
            return;
        }

        GridD::Neighbours n = mGrid.getNeighbours(mStack[mStackTop].location, false);
        while(n.advance())
        {
            Symbol atN = mValues[n.current()];
            if(atN != Solver::kUnsetSymbol)
            {
                const SymbolSet& adjacent = mSequence.getAdjacent(atN);
                // Is this the first neighbour we encounter?
                if(options.size() == 0)
                {
                    addConstrainedBySequence(options, adjacent);
                    if(options.size() == 0)
                    {
                        // We've run out of options.
                        return;
                    }
                }
                else
                {
                    removeIncompatible(options, adjacent);

                    if(options.size() == 0)
                    {
                        // We can't satisfy the constraints.
                        return;
                    }
                }
            }
        }
        diagnose(mGrid, mValues, mStack[mStackTop].location, options);
    }

    void addConstrainedBySequence(SymbolList& options, const SymbolSet& adjacent)
    {
        Sequence::SymbolSet::const_iterator adjIt = adjacent.begin();
        for(; adjIt != adjacent.end(); ++adjIt)
        {
            Symbol s = *adjIt;
            if(mSequence.count(s) > mValues.symbolCount(s))
            {
                options.push_back(s);
            }
        }
    }

    // Functor used to filter the list of options.
    template <typename T>
    struct IsNotIn
    {
        IsNotIn(const std::set<T>& items)
            : mItems(items)
        {
        }

        bool operator()(T item) const
        {
            return mItems.find(item) == mItems.end();
        }

    private:
        PREVENT_ASSIGNMENT(IsNotIn);
        const std::set<T>& mItems;
    };

    void removeIncompatible(SymbolList& options, const SymbolSet& adjacent)
    {
        options.erase(
            std::remove_if(options.begin(), options.end(), IsNotIn<Symbol>(adjacent)),
            options.end()
        );
    }

    bool placeNextOption()
    {
        Stage& current = mStack[mStackTop];
        if(current.options.size() > 0)
        {
            mValues.place(current.options.back(), current.location);
            current.options.pop_back();
            return true;
        }
        else
        {
            return false;
        }
    }

    void popOption()
    {
        Stage& current = mStack[mStackTop];
        mValues.clear(current.location);

        // The location will still be the next neighbour to look at,
        // so we won't need to find it again, just move back
        // the stack one step.
        --mStackTop;
    }

    const GridD& mGrid;
    int mTotalCount;
    const Sequence& mSequence;
    Values mValues;

    Stages mStack;
    int mStackTop;
    Coord mSearchLocation;
};

#endif // SOLVER_GRIDSOLVER_H__INCLUDED