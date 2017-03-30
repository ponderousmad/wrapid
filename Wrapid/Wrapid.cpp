/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Test.h"
#include "Utils/Stopwatch.h"
#include "Solver/HourPuzzle.h"
#include "Solver/CardPuzzle3D.h"
#include "Solver/CardPuzzle4D.h"

#include <iostream>
#include <vector>
#include <tchar.h>
#include <assert.h>

namespace 
{

#ifdef BUILD_TESTS
    int runUnitTests()
    {
        const char* tests[] = {
          "CoordinateTest",
          "CoordinateNextTest",
          "GridTest",
          "GridValuesTest",
          "GridSolverTest",
          "HourPuzzleIOTest",
          "HourPuzzleTest",
          "CardPuzzle3DIOTest",
          "CardPuzzle3DTest",
          "CardPuzzle4DIOTest",
          "CardPuzzle4DTest",
          0
        };
        return UnitTest::Framework::runTests( tests, "" );
    }
#endif

    const char* parseDiagnostics(Solver::ParseResult result)
    {
        switch(result)
        {
        case Solver::kParseNotEnoughLines:
            return "The provided puzzle did not have enough lines to be valid.";
        case Solver::kParseLineTooShort:
            return "One of the provided puzzle's lines was too short.";
        case Solver::kParseInvalidSymbol:
            return "One of the specified grid values is not valid for this puzzle.";
        case Solver::kParseInvalidFloor:
            return "One of the squares in the grid has a floor or ceiling with an invalid character.";
        case Solver::kParseInvalidWall:
            return "One of the squares in the grid has a wall with an invalid character.";
        case Solver::kParseInvalidLevelTunnel:
            return "One of the squares in the grid has a invalid character where you indicate a tunnel to the next level.";
        case Solver::kParseInvalidVolumeTunnel:
            return "One of the squares in the grid has a invalid character where you indicate a tunnel to the next row of levels.";
        case Solver::kParseFloorWrapMismatch:
            return "In one of the columns, the wrap at the edge does not match at the top and bottom.";
        case Solver::kParseWallWrapMismatch:
            return "In one of the rows, the wrap at the edge does not match at the left and right sides.";
        }
        return "Unknown error.";
    }

    void pauseForInput(const char* message = NULLPTR)
    {
        if(std::cin.good())
        {
            std::cout << std::endl;
            std::cout << (message ? message : "Press enter to continue . . .");
            std::getchar();
            std::cout << std::endl;
        }
    }

    void printGap(int count = 5)
    {
        for(int i = 0; i < count; ++i)
        {
            std::cout << std::endl;
        }
    }

    template <class Puzzle>
    Solver::SolveResult showSolving(Puzzle& puzzle)
    {
        std::cout << "Solving . . . . ";

        Utils::Stopwatch watch;

        Solver::SolveResult result = puzzle.findNextSolution();

        double time = watch.elapsed();

        std::cout << " " << time << " ms\n" << std::endl;
        return result;
    }

    template <class Puzzle>
    int solvePuzzle()
    {
        const int kMaxLineLength = 1000;
        char buffer[kMaxLineLength];
        std::vector<std::string> lines;
        std::string line;
        while(std::cin.good())
        {
            std::cin.getline(buffer, kMaxLineLength - 1);
            line = std::string(buffer);
            if(line.length() == 0)
            {
                break;
            }
            lines.push_back(line);
        }

        Puzzle puzzle;
        Solver::ParseResult result = puzzle.parse(lines);
        if(result == Solver::kParseSucceed)
        {
            Solver::SolveResult nextResult = showSolving(puzzle);

            switch(nextResult)
            {
            case Solver::kFoundSolution:
                std::cout << "The following solution was found: " << std::endl;
                puzzle.print(std::cout);
                pauseForInput();
                return 0;
            case Solver::kAlreadySolved:
                std::cout << "The puzzle you provided was already solved: " << std::endl;
                puzzle.print(std::cout);
                pauseForInput();
                return 0;
            }
            return -nextResult;
        }
        else
        {
            const int kParseErrorRange = 100;
            std::cout << parseDiagnostics(result);
            printGap();
            pauseForInput();
            return -(result + kParseErrorRange);
        }
    }

    template <class Puzzle>
    void storySolve(Puzzle& puzzle, const char** data)
    {
        puzzle.parse(data);
        puzzle.print(std::cout);
        pauseForInput("Press enter to search for a solution . . .");
        showSolving(puzzle);
        puzzle.print(std::cout);
        pauseForInput();
    }

    void printUsage()
    {
        std::cout << "This program can be run with command line options to solve puzzles\n"
                     "presented on standard input. The options are case sensitive.\n\n"
                     "  -Hour\n"
                     "      The standard 4 row, 6 column hour puzzle,\n"
                     "      but with torus topology allowed.\n\n"
                     "  -Card3D\n"
                     "      The deck of cards puzzle with 6 rows, 3 columns and 3 levels.\n\n"
                     "  -Card4D\n"
                     "      The 4D puzzle with 3 rows, 3 columns, 3 levels and 2 volumes.\n\n"
                     "  -UnitTest\n"
                     "      Runs the unit test suite, which may be excluded at compile time." << std::endl;
    }

    int story()
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

        std::cout << "This program solves hour puzzles. In this puzzle you need to place the\n"
                     "numbers from 1 to 12 on the grid twice such that numbers are only allowed\n"
                     "to be adjacent to each other if they are on a clock face.\n\n"
                     "Here's an example:\n" << std::endl;

        Solver::HourPuzzle hours;
        storySolve(hours, puzzle);
        printGap();

        std::cout << "In this case there's more than one solution." << std::endl;
        pauseForInput("Press enter to search for the next solution . . .\n");
        showSolving(hours);
        hours.print(std::cout);
        pauseForInput();
        printGap();

        std::cout << "However, as soon as I thought about the problem of solving this puzzle,\n"
                     "I immediately began to consider extensions.\n\n"
                     "My first thought was to consider non-planar embeddings. For instance,\n"
                     "this grid has the topology of a torus, you can wrap around either edge:\n" << std::endl;

        const char* torus[9] = {
            "+----+----+----+----+    +----+",
            "|    |              |       7 |",
            "+    +    +----+    +----+    +",
            "     |         |         |     ",
            "+    +----+    +----+----+----+",
            "|         | 12                |",
            "+----+    +----+----+----+    +",
            "|                   |         |",
            "+----+----+----+----+    +----+"
        };
        storySolve(hours, torus);
        printGap();

        std::cout << "Of course, the next step was to jump up to higher dimensions.\n\n"
                     "When you do that, you start to increase the potential number\n"
                     "of neighbours any cell has. So you need more than two of each\n"
                     "option to have a reasonable chance of success. However, there are\n"
                     "only 24 hours in a day (on earth anyway).\n" << std::endl;
        pauseForInput();
        printGap();

        std::cout << "Then it struck me that a deck of cards has just the right structure.\n"
                     "The 52 card deck doesn't factor into 3 dimensions. But if you\n"
                     "include the jokers, you now have 54 = 6 * 3 * 3. So I designed the\n"
                     "solver to allow the sequence to be flexible, and added another\n"
                     "dimension.\n" << std::endl;
        pauseForInput();
        printGap();

        std::cout << "So, in the following puzzle, the 'o's indicate a tunnel from one\n"
                     "level of the puzzle to the level to the right. As in the previous\n"
                     "puzzle, toroidal topologies are allowed which loop back around the\n"
                     "'edges'. Here the symbols are A, 2 - 10, J, Q, K and ? for the joker.\n"
                     "The joker is allowed to be adjacent to anything, including itself.\n"<< std::endl;

        const char* card3D[13] = {
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
        Solver::CardPuzzle3D card3DPuzzle;
        storySolve(card3DPuzzle, card3D);
        printGap();

        std::cout << "But wait, 6 is just 2 * 3, so obviously we should try and build,\n"
                     "a 4 dimensional puzzle. This time the '@' symbol is used to show\n"
                     "where you can move between the two 'volumes'. It helps that we have\n"
                     "jokers in the puzzle, because a cell can have seven neighbours,\n"
                     "such as the one in the middle of the lower volume.\n"
                     "The maximum is seven because with only two volumes, a tunnel\n"
                     "in either direction gets you the same place.\n" << std::endl;

        const char* card4D[14] = {
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "|   o|       2 | |    |        o| |  5 |         |",
                "+    +    +----+ +    +    +----+ +    +    +    +",
                "|    |    |   o| |    |    |@   | |    |    |  Q |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|  2 |         | |         |    | |    |@   |    |",
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "|  5 |    |    | |   o|  4 |   o| |         |    |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|        o|   o| |     @  o   7 | |    |    | 10 |",
                "+----+    +----+ +----+    +    + +    +    +----+",
                "|            5 | |         |    | |   o|  K      |",
                "+----+----+----+ +----+----+----+ +----+----+----+"
        };
        Solver::CardPuzzle4D card4DPuzzle;
        storySolve(card4DPuzzle, card4D);
        printGap();

        std::cout << "As fun as it would be to go to higher dimensions, I suspect that\n"
                     "you have better things to do with your time." << std::endl;
        printGap(2);
        pauseForInput();
        printGap();

        printUsage();

        printGap(2);
        pauseForInput();
        return 0;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    if(argc > 1)
    {
        const _TCHAR* option = argv[argc - 1];
        if(_tcscmp(option, _T("-UnitTest")) == 0)
        {
#ifdef BUILD_TESTS
            return runUnitTests();
#else
            std::cout << "Unit tests not built." << std::endl;
            return 0;
#endif
        }
        else if(_tcscmp(option, _T("-Hour")) == 0)
        {
            return solvePuzzle<Solver::HourPuzzle>();
        }
        else if(_tcscmp(option, _T("-Card3D")) == 0)
        {
            return solvePuzzle<Solver::CardPuzzle3D>();
        }
        else if(_tcscmp(option, _T("-Card4D")) == 0)
        {
            return solvePuzzle<Solver::CardPuzzle4D>();
        }
        else
        {
            printUsage();
            printGap(2);
            pauseForInput();
            return 0;
        }
    }
    return story();
}
