/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Solver/HourPuzzle.h"
#include "Solver/PuzzleSover.h"
#include "Solver/GridSolver.h"
#include "Solver/HourPuzzleIO.h"

using Solver::PuzzleSolver;
using Solver::HourPuzzle;
using Solver::coord;
using Solver::ParseResult;

namespace
{
    const Solver::Coordinate2D kSize(4, 6);
    const int kSymbols = Solver::kHourPuzzleSymbols;
    const int kRepeats = Solver::kHourPuzzleRepeats;
}

HourPuzzle::HourPuzzle()
    : mSolver(new PuzzleSolver<2>(kSize, kSymbols, kRepeats))
{
}

HourPuzzle::~HourPuzzle()
{
}

ParseResult HourPuzzle::parse(const char** puzzle)
{
    return mSolver->parse(puzzle);
}

ParseResult HourPuzzle::parse(const std::vector<const std::string>& puzzle)
{
    return mSolver->parse(puzzle);
}

Solver::SolveResult HourPuzzle::findNextSolution()
{
    return mSolver->findNextSolution();
}

void HourPuzzle::print(std::ostream& stream) const
{
    return mSolver->print(stream);
}

#ifdef BUILD_TESTS

#include "Solver/SolverTest.h"

#include <sstream>

namespace
{
    class HourPuzzleTest : public Solver::SolverTest
    {
    public:
        void run()
        {
            RUN( solveTest );
            RUN( lessConstrainedTest );
            RUN( freeRegionTest );
        }

        void solveTest()
        {
            const char* puzzle[9] = {
                "+----+----+----+----+----+----+",
                "| 12 |              |       7 |",
                "+    +    +----+    +----+    +",
                "|    |    |    |              |",
                "+    +    +    +    +----+----+",
                "|         |                   |",
                "+----+    +----+----+----+    +",
                "|  9                        2 |",
                "+----+----+----+----+----+----+"
            };

            std::string target(
                "+----+----+----+----+----+----+\n"
                "| 12 |  7    6    5 |  8    7 |\n"
                "+    +    +----+    +----+    +\n"
                "| 11 |  8 |  1 |  4    5    6 |\n"
                "+    +    +    +    +----+----+\n"
                "| 10    9 |  2    3    4    3 |\n"
                "+----+    +----+----+----+    +\n"
                "|  9   10   11   12    1    2 |\n"
                "+----+----+----+----+----+----+\n"
            );

            testSolver<HourPuzzle>(puzzle, target, 2, 5);
        }

        void lessConstrainedTest()
        {
            const char* puzzle[9] = {
                "+----+----+----+----+----+----+",
                "| 12 |              |         |",
                "+    +    +----+    +----+    +",
                "|    |    |    |              |",
                "+    +    +    +----+----+----+",
                "|         |                   |",
                "+----+    +----+----+----+    +",
                "|                             |",
                "+----+----+----+----+----+----+"
            };

            std::string target(
                "+----+----+----+----+----+----+\n"
                "| 12 |  7    6    5 |  8    7 |\n"
                "+    +    +----+    +----+    +\n"
                "| 11 |  8 |  1 |  4    5    6 |\n"
                "+    +    +    +----+----+----+\n"
                "| 10    9 |  2    3    4    3 |\n"
                "+----+    +----+----+----+    +\n"
                "|  9   10   11   12    1    2 |\n"
                "+----+----+----+----+----+----+\n"
            );

            testSolver<HourPuzzle>(puzzle, target, 48, 50);
        }

        void freeRegionTest()
        {
            const char* puzzle[9] = {
                "+----+----+----+----+----+----+",
                "| 12 |       6      |       7 |",
                "+    +    +----+    +----+    +",
                "|    |    |    |              |",
                "+    +    +    +----+----+----+",
                "|         |    |  3           |",
                "+----+    +----+----+----+    +",
                "|  9                        2 |",
                "+----+----+----+----+----+----+"
            };

            std::string target(
                "+----+----+----+----+----+----+\n"
                "| 12 |  7    6    5 |  8    7 |\n"
                "+    +    +----+    +----+    +\n"
                "| 11 |  8 |  1 |  4    5    6 |\n"
                "+    +    +    +----+----+----+\n"
                "| 10    9 |  2 |  3    4    3 |\n"
                "+----+    +----+----+----+    +\n"
                "|  9   10   11   12    1    2 |\n"
                "+----+----+----+----+----+----+\n"
            );

            testSolver<HourPuzzle>(puzzle, target, 4, 5);
        }
    };
}

DECLARE_TEST( HourPuzzleTest );

#endif // BUILD_TESTS
