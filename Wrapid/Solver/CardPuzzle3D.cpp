/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"

#include "Solver/CardPuzzle3D.h"
#include "Solver/PuzzleSover.h"
#include "Solver/GridSolver.h"
#include "Solver/CardPuzzle3DIO.h"

using Solver::PuzzleSolver;
using Solver::CardPuzzle3D;
using Solver::coord;
using Solver::ParseResult;

namespace
{
    const Solver::Coordinate3D kSize(6, 3, 3);
    const int kSymbols = Solver::kCardPuzzleSuitSymbols;
    const int kRepeats = Solver::kCardPuzzleSuits;
}

CardPuzzle3D::CardPuzzle3D()
    : mSolver(new PuzzleSolver<3>(kSize, kSymbols, kRepeats, true))
{
}

CardPuzzle3D::~CardPuzzle3D()
{
}

ParseResult CardPuzzle3D::parse(const char** puzzle)
{
    return mSolver->parse(puzzle);
}

ParseResult CardPuzzle3D::parse(const std::vector<const std::string>& puzzle)
{
    return mSolver->parse(puzzle);
}

Solver::SolveResult CardPuzzle3D::findNextSolution()
{
    return mSolver->findNextSolution();
}

void CardPuzzle3D::print(std::ostream& stream) const
{
    return mSolver->print(stream);
}

#ifdef BUILD_TESTS

#include "Solver/SolverTest.h"

#include <sstream>

namespace
{
    class CardPuzzle3DTest : public Solver::SolverTest
    {
    public:
        void run()
        {
            RUN( solveTest );
        }

        void solveTest()
        {
            const char* puzzle[13] = {
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "|  Ko|       3 | |  Q |       9o| |  2 |         |",
                "+    +    +----+ +    +    +----+ +    +    +----+",
                "|    |    |    | |    |    |    | |    |    |    |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|    |    |    | |    |  6 |    | |    |    |  K |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|    |    |    | |   o|    |  Jo| |    |    |    |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|       6 |  Jo| |         |    | |    |    |    |",
                "+----+    +----+ +----+----+    + +    +    +----+",
                "|              | |  2           | |  5o|  8      |",
                "+----+----+----+ +----+----+----+ +----+----+----+"
            };

            std::string target(
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
                "|  Ko|  4    3 | |  Q |  8    9o| |  2 |  9   10 |\n"
                "+    +    +----+ +    +    +----+ +    +    +----+\n"
                "|  ? |  3 | 10 | |  K |  7 |  9 | |  A | 10 |  A |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|  ? |  4 |  J | |  A |  6 | 10 | |  2 |  9 |  K |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|  8 |  5 |  Q | |  2o|  5 |  Jo| |  3 |  8 |  Q |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|  7    6 |  Jo| |  3    4 |  Q | |  4 |  7 |  J |\n"
                "+----+    +----+ +----+----+    + +    +    +----+\n"
                "|  6    5    6 | |  2    A    K | |  5o|  8    7 |\n"
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
            );

            testSolver<CardPuzzle3D>(puzzle, target, 5, 5);
        }

        // This test takes a really long time, especially in a debug cut,
        // so for now it currently isn't included in the standard test suite.
        void hardSolveTest()
        {
            const char* puzzle[13] = {
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "|   o|         | |    |        o| |    |         |",
                "+    +    +----+ +    +    +----+ +    +    +----+",
                "|    |    |    | |    |    |    | |    |    |    |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|  3 |    |    | |    |    |    | |    |    |  K |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|    |    |    | |   o|    |   o| |    |    |    |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|         |   o| |         |    | |    |  ? |    |",
                "+----+    +----+ +----+----+    + +    +    +----+",
                "|              | |       A      | |   o|         |",
                "+----+----+----+ +----+----+----+ +----+----+----+"
            };

            std::string target(
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
                "|  Ko|  4    3 | |  Q |  8    9o| |  2 |  9   10 |\n"
                "+    +    +----+ +    +    +----+ +    +    +----+\n"
                "|  ? |  5 |  8 | |  K |  7 |  9 | |  A | 10 |  A |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|  3 |  6 |  9 | |  A |  6 | 10 | |  2 |  J |  K |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|  4 |  7 | 10 | |  2o|  5 |  Jo| |  3 |  Q |  Q |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|  5    6 |  Jo| |  3    4 |  Q | |  4 |  ? |  J |\n"
                "+----+    +----+ +----+----+    + +    +    +----+\n"
                "|  6    7    8 | |  2    A    K | |  5o|  8    7 |\n"
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
            );

            testSolver<CardPuzzle3D>(puzzle, target, 5, 5);
        }
    };
}

DECLARE_TEST(CardPuzzle3DTest );

#endif // BUILD_TESTS
