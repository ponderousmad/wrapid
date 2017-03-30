/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Solver/CardPuzzle4D.h"
#include "Solver/PuzzleSover.h"
#include "Solver/GridSolver.h"
#include "Solver/CardPuzzle4DIO.h"

using Solver::PuzzleSolver;
using Solver::CardPuzzle4D;
using Solver::coord;
using Solver::ParseResult;

namespace
{
    const Solver::Coordinate4D kSize(3, 3, 3, 2);
    const int kSymbols = Solver::kCardPuzzleSuitSymbols;
    const int kRepeats = Solver::kCardPuzzleSuits;
}

CardPuzzle4D::CardPuzzle4D()
    : mSolver(new PuzzleSolver<4>(kSize, kSymbols, kRepeats, true))
{
}

CardPuzzle4D::~CardPuzzle4D()
{
}

ParseResult CardPuzzle4D::parse(const char** puzzle)
{
    return mSolver->parse(puzzle);
}

ParseResult CardPuzzle4D::parse(const std::vector<const std::string>& puzzle)
{
    return mSolver->parse(puzzle);
}

Solver::SolveResult CardPuzzle4D::findNextSolution()
{
    return mSolver->findNextSolution();
}

void CardPuzzle4D::print(std::ostream& stream) const
{
    return mSolver->print(stream);
}

#ifdef BUILD_TESTS

#include "Solver/SolverTest.h"

#include <sstream>

namespace
{
    const int kMaxSolves = 5;

    class CardPuzzle4DTest : public Solver::SolverTest
    {
    public:
        void run()
        {
            RUN( solveTest );
        }

        void solveTest()
        {
            const char* puzzle[14] = {
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "|   o|       K | |    |      10o| |    | 10      |",
                "+    +    +----+ +    +    +----+ +    +    +    +",
                "|    |    |  5o| |    |    |@ 6 | |    |    |    |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|  2 |         | |         |    | |  7 |@   |    |",
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "|    |    |  9 | |  3o|  4 |  8o| |         |    |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|     @ A |   o| |         |    | |  5 |    | 10 |",
                "+----+    +----+ +----+----+    + +    +    +----+",
                "|              | | 10           | |   o|       6 |",
                "+----+----+----+ +----+----+----+ +----+----+----+"
            };

            std::string target(
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
                "|  ?o|  A    K | |  Q |  J   10o| |  5 | 10    J |\n"
                "+    +    +----+ +    +    +----+ +    +    +    +\n"
                "|  ? |  2 |  5o| |  K |  Q |@ 6 | |  6 |  9 |  Q |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|  2 |  3    4 | |  Q    J |  7 | |  7 |@ 8 |  J |\n"
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
                "|  A |  K |  9 | |  3o|  4 |  8o| |  4    5 |  9 |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|  K  @ A |  8o| |  2    3 |  7 | |  5 |  6 | 10 |\n"
                "+----+    +----+ +----+----+    + +    +    +----+\n"
                "|  3    2    A | | 10    9    8 | |  4o|  7    6 |\n"
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
            );

            testSolver<CardPuzzle4D>(puzzle, target, 5, 5);
        }

        // This test takes a really long time, especially in a debug cut,
        // so for now it currently isn't included in the standard test suite.
        void hardSolveTest()
        {
            const char* puzzle[14] = {
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "|   o|         | |    |        o| |  5 |         |",
                "+    +    +----+ +    +    +----+ +    +    +    +",
                "|    |    |   o| |    |    |@   | |    |    |  Q |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|  2 |         | |         |    | |    |@   |    |",
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "|    |    |    | |   o|  4 |   o| |         |    |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|     @   |   o| |         |  7 | |    |    |    |",
                "+----+    +----+ +----+----+    + +    +    +----+",
                "|              | |              | |   o|         |",
                "+----+----+----+ +----+----+----+ +----+----+----+"
            };

            std::string target(
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
                "|  ?o|  A    K | |  Q |  J   10o| |  5 | 10    J |\n"
                "+    +    +----+ +    +    +----+ +    +    +    +\n"
                "|  ? |  2 |  5o| |  K |  Q |@ 6 | |  6 |  9 |  Q |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|  2 |  3    4 | |  Q    J |  7 | |  7 |@ 8 |  J |\n"
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
                "|  A |  K |  9 | |  3o|  4 |  8o| |  4    5 |  9 |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|  K  @ A |  8o| |  2    3 |  7 | |  5 |  6 | 10 |\n"
                "+----+    +----+ +----+----+    + +    +    +----+\n"
                "|  3    2    A | | 10    9    8 | |  4o|  7    6 |\n"
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
            );

            testSolver<CardPuzzle4D>(puzzle, target, 5, 5);
        }
    };
}

DECLARE_TEST(CardPuzzle4DTest );

#endif // BUILD_TESTS
