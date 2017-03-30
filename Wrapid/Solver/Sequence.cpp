/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Solver/Sequence.h"

#include <algorithm>
#include <assert.h>

using Solver::Symbol;
using Solver::Sequence;

Sequence::Sequence()
    : mNextSymbol(Solver::kFirstSymbol)
{
}

Sequence::~Sequence()
{
}

Sequence::Sequence(int symbolCount, int repeatCount)
    : mNextSymbol(Solver::kFirstSymbol)
{
    for(int i = 0; i < symbolCount; ++i)
    {
        addSymbol(repeatCount);
    }
    makeAdjacent(Solver::kFirstSymbol, symbolCount);

    for(Symbol i = Solver::kFirstSymbol; i < symbolCount; ++i)
    {
        makeAdjacent(i, i + 1);
    }
}

Symbol Sequence::addSymbol(int count)
{
    Symbol next = mNextSymbol;
    ++mNextSymbol;
    mSymbols[next].first = count;
    return next;
}

void Sequence::addJoker()
{
    Symbol joker = addSymbol(Solver::kCardPuzzleJokerCount);
    assert(joker == Solver::kCardPuzzleJoker);

    for(Symbol s = Solver::kCardPuzzleAce; s <= Solver::kCardPuzzleSuitSymbols; ++s)
    {
        makeAdjacent(joker, s);
    }
    // The joker is beside himself
    makeAdjacentDirected(joker, joker);
}

void Sequence::makeAdjacentDirected(Symbol s, Symbol t)
{
    assert(mSymbols.find(s) != mSymbols.end());
    assert(std::find(mSymbols[s].second.begin(), mSymbols[s].second.end(), t) == mSymbols[s].second.end());

    mSymbols[s].second.insert(t);
}

void Sequence::makeAdjacent(Symbol s, Symbol t)
{
    makeAdjacentDirected(s, t);
    makeAdjacentDirected(t, s);
}

Sequence::SymbolSet Sequence::getSymbols() const
{
    SymbolSet symbols;
    SymbolMap::const_iterator end = mSymbols.end();
    for(SymbolMap::const_iterator it = mSymbols.begin(); it != end; ++it)
    {
        symbols.insert(it->first);
    }
    return symbols;
}

const int Sequence::count(Symbol s) const
{
    assert(mSymbols.find(s) != mSymbols.end());

    return mSymbols.find(s)->second.first;
}

const Sequence::SymbolSet& Sequence::getAdjacent(Symbol s) const
{
    assert(mSymbols.find(s) != mSymbols.end());

    return mSymbols.find(s)->second.second;
}
