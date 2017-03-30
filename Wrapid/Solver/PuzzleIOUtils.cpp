/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Solver/PuzzleIOUtils.h"

#include <assert.h>
#include <sstream>

using Solver::PuzzleIOUtils;
using Solver::Symbol;

namespace
{
    const int kCellWidth = 5;
}

PuzzleIOUtils::PuzzleIOUtils()
    : kCWall("|")
    , kNoCWall(" ")
    , kRWall("----")
    , kNoRWall("    ")
    , kZWall(" ")
    , kNoZWall("o")
    , kWWall(" ")
    , kNoWWall("@")
    , kCorner("+")
    , kLevelGap(" ")
{
}

PuzzleIOUtils::~PuzzleIOUtils()
{
}

void PuzzleIOUtils::addFinder(Symbol s, Rep& representation)
{
    mSymbolFinder.insert(SymbolFinder::value_type(representation, s));
}

void PuzzleIOUtils::addOutput(Symbol s, Rep& representation)
{
    mOutputReps.insert(OutputRepresentations::value_type(s, representation));
}

namespace
{
    PuzzleIOUtils::Rep spaces(int count)
    {
        switch(count)
        {
        case 1: return " ";
        case 2: return "  ";
        case 3: return "   ";
        }
        return "";
    }
}

void PuzzleIOUtils::addSymbol(Symbol s, Rep& base, int size)
{
    int length = base.length();
    int idealBefore = size - length - (size == 4 ? 1 : 0);
    for(int before = 0; before + length <= size; ++before)
    {
        int after = size - length - before;
        Rep rep = spaces(before) + base + spaces(after);
        if(before == idealBefore)
        {
            addOutput(s, rep);
        }
        addFinder(s, rep);
    }
}

void PuzzleIOUtils::setupCards(int repSize)
{
    void setupCards(int repSize);
    {
        addSymbol(Solver::kCardPuzzleAce, "A", repSize);
        addSymbol(Solver::kCardPuzzleJack, "J", repSize);
        addSymbol(Solver::kCardPuzzleQueen, "Q", repSize);
        addSymbol(Solver::kCardPuzzleKing, "K", repSize);
        for(int i = Solver::kCardPuzzleAce + 1; i < Solver::kCardPuzzleJack; ++i)
        {
            addSymbol(i, PuzzleIOUtils::asString(i), repSize);
        }
        addSymbol(Solver::kCardPuzzleJoker, "?", repSize);
    }
}

std::pair<bool, Symbol> PuzzleIOUtils::findSymbol(Rep& representation) const
{
    SymbolFinder::const_iterator it = mSymbolFinder.find(representation);
    if(it != mSymbolFinder.end())
    {
        return std::pair<bool, Symbol>(true, it->second);
    }
    else
    {
        return std::pair<bool, Symbol>(false, Solver::kUnsetSymbol);
    }
}

PuzzleIOUtils::Rep& PuzzleIOUtils::findOutput(Symbol s) const
{
    assert(mOutputReps.find(s) != mOutputReps.end());
    return mOutputReps.find(s)->second;
}

std::string PuzzleIOUtils::asString(int value)
{
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

PuzzleIOUtils::Strings PuzzleIOUtils::asStrings(const char** puzzle, int lineCount)
{
    std::vector<const std::string> lines;
    for(int i = 0; i < lineCount; ++i)
    {
        lines.push_back(puzzle[i]);
    }
    return lines;
}