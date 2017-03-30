#pragma once
#ifndef SOLVER_PUZZLEIOUTILS_H__INCLUDED
#define SOLVER_PUZZLEIOUTILS_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Solver/Symbol.h"
#include "Solver/Grid.h"
#include "Solver/ParseResult.h"

#include <utility>
#include <map>
#include <vector>
#include <string>

namespace Solver
{
    class PuzzleIOUtils;
}

class Solver::PuzzleIOUtils
{
    PREVENT_COPY_AND_ASSIGNMENT(PuzzleIOUtils);
public:
    typedef const std::string Rep;
    typedef std::map<Rep, Symbol> SymbolFinder;
    typedef std::map<Symbol, Rep> OutputRepresentations;
    typedef std::vector<const std::string> Strings;

    enum
    {
        kCellWidth = 5
    };

    PuzzleIOUtils();
    ~PuzzleIOUtils();

    void addFinder(Symbol s, Rep& representation);
    void addOutput(Symbol s, Rep& representation);
    void addSymbol(Symbol s, Rep& base, int size);

    void setupCards(int repSize);

    std::pair<bool, Symbol> findSymbol(Rep& representation) const;
    Rep& findOutput(Symbol s) const;

    template <int D>
    bool isWall(const Grid<D>& grid, const Coordinate<D>& c, int dimension, bool forward) const
    {
        int direction = forward ? Grid<D>::kForwardIndex : Grid<D>::kReverseIndex;
        return grid.getWalls(c).wall[dimension][direction];
    }

    static std::string asString(int value);
    static Strings asStrings(const char** puzzle, int lineCount);

    // Walls between columns representation.
    Rep kCWall;
    Rep kNoCWall;

    // Walls between rows representation.
    Rep kRWall;
    Rep kNoRWall;

    // Walls/Holes between 3D layers representation.
    Rep kZWall;
    Rep kNoZWall;

    // Walls/Holes between hyper layers representation.
    Rep kWWall;
    Rep kNoWWall;

    Rep kCorner;
    Rep kLevelGap;

private:
    SymbolFinder mSymbolFinder;
    OutputRepresentations mOutputReps;
};

namespace Solver
{
    // Read the number of lines required for the puzzle from the array.
    template <int Dimensions>
    inline PuzzleIOUtils::Strings asStrings(const char** puzzle, Coordinate<Dimensions> size)
    {
        return PuzzleIOUtils::asStrings(puzzle, size[0] * 2 + 1);
    }

    template <>
    inline PuzzleIOUtils::Strings asStrings<4>(const char** puzzle, Coordinate<4> size)
    {
        return PuzzleIOUtils::asStrings(puzzle, (size[0] * 2 + 1) * size[3]);
    }
}

#endif // SOLVER_PUZZLEIOUTILS_H__INCLUDED
