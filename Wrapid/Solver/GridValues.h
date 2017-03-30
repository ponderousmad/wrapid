#pragma once
#ifndef SOLVER_GRIDVALUES_H__INCLUDED
#define SOLVER_GRIDVALUES_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Solver/CoordinateNext.h"
#include "Solver/Symbol.h"

#include <unordered_map>
#include <assert.h>

#ifdef TRACE
#include <iostream>
#include <iomanip>
#endif

namespace Solver
{
    template <int Dimensions>
    class GridValues;

    typedef GridValues<2> GridValues2D;
    typedef GridValues<3> GridValues3D;
    typedef GridValues<4> GridValues4D;
}

// Keeps track of which symbol is in each grid location.
template <int Dimensions>
class Solver::GridValues
{
public:
    typedef Coordinate<Dimensions> Coord;
    typedef std::unordered_map<Coord, Symbol, CoordinateOrder<Dimensions> > Values;
    typedef std::unordered_map<Symbol, int> SymbolCounts;
    typedef typename Values::const_iterator const_iterator;

    GridValues(Coord size)
        : mSize(size)
        , mValues(10, CoordinateOrder<Dimensions>(size))
    {
    }

    const Coord& getSize() const
    {
        return mSize;
    }

    void place(Symbol s, Coord c)
    {
        assert(inVolume(c, mSize));

        if(s == Solver::kUnsetSymbol)
        {
            clear(c);
            return;
        }

        Values::iterator it = mValues.find(c);
        if(it != mValues.end())
        {
            --mCounts[it->second];
            it->second = s;
        }
        else
        {
            mValues[c] = s;
        }
        ++mCounts[s];
    }

    bool clear(Coord c)
    {
        Values::iterator it = mValues.find(c);
        if(it != mValues.end())
        {
            --mCounts[it->second];
            mValues.erase(it);
            return true;
        }
        return false;
    }

    Symbol operator[](Coord c) const
    {
        Values::const_iterator it = mValues.find(c);
        if(it != mValues.end())
        {
            return it->second;
        }
        return Solver::kUnsetSymbol;
    }

    const_iterator begin() const
    {
        return mValues.begin();
    }

    const_iterator end() const
    {
        return mValues.end();
    }

    int valueCount() const
    {
        return mValues.size();
    }

    const SymbolCounts& getSymbolCounts() const
    {
        return mCounts;
    }

    int symbolCount(Symbol s) const
    {
        SymbolCounts::const_iterator found = mCounts.find(s);
        return found == mCounts.end() ? 0 : found->second;
    }

private:
    Coord mSize;
    Values mValues;
    SymbolCounts mCounts;
};

namespace Solver
{
    template <int Dimensions>
    bool isMatch(const GridValues<Dimensions>& target, const GridValues<Dimensions>& other)
    {
        if(target.getSize() != other.getSize())
        {
            return false;
        }
        Coordinate<Dimensions> c;
        while(c != target.getSize())
        {
            if(target[c] != other[c])
            {
                return false;
            }
            c = next(c, target.getSize());
        }
        return true;
    }
}

#endif // SOLVER_GRIDVALUES_H__INCLUDED
