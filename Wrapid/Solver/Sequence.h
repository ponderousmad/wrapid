#pragma once
#ifndef SOLVER_SEQUENCE_H__INCLUDED
#define SOLVER_SEQUENCE_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Solver/Symbol.h"

#include <vector>
#include <set>
#include <map>

namespace Solver
{
    class Sequence;
}

// Represents the set of symbols that can be placed on the board,
// as well as how many of each symbol is allowed.
class Solver::Sequence
{
    PREVENT_COPY_AND_ASSIGNMENT(Sequence);
public:
    typedef std::set<Symbol> SymbolSet;

    // Construct an empty sequence with no symbols
    // and nothing adjacent.
    Sequence();

    ~Sequence();

    // Construct a simple circular sequence.
    Sequence(int symbolCount, int repeatCount);

    // Produce a new symbol for the sequence which
    // is allowed the specified number of times.
    // Initially adjacent to nothing.
    Symbol addSymbol(int count);

    // Utility method for adding the joker to the card sequence.
    // Jokers are adjacent to every symbol (including itself).
    void addJoker();

    // Allow the two symbols to sit touching each
    // other on the game board.
    void makeAdjacent(Symbol s, Symbol t);

    // Get a collection of all of the symbols.
    SymbolSet getSymbols() const;

    const int count(Symbol s) const;
    const SymbolSet& getAdjacent(Symbol s) const;

private:
    void makeAdjacentDirected(Symbol s, Symbol t);

    typedef std::map<Symbol, std::pair<int, SymbolSet> > SymbolMap;

    SymbolMap mSymbols;
    Symbol mNextSymbol;
};

#endif // SOLVER_SEQUENCE_H__INCLUDED
