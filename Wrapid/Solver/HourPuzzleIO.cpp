/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Solver/HourPuzzleIO.h"

#include <assert.h>

using Solver::PuzzleIOUtils;
using Solver::Grid2D;
using Solver::GridValues2D;
using Solver::coord;
using Solver::Symbol;
using Solver::ParseResult;

namespace
{
    void setupUtils(PuzzleIOUtils& utils)
    {
        utils.addFinder(Solver::kUnsetSymbol, "    ");
        utils.addOutput(Solver::kUnsetSymbol, "    ");

        for(int i = Solver::kFirstSymbol; i <= Solver::kHourPuzzleSymbols; ++i)
        {
            utils.addSymbol(i, PuzzleIOUtils::asString(i), PuzzleIOUtils::kCellWidth - 1);
        }
    }
}

ParseResult Solver::parse(Grid2D& grid, GridValues2D& values, const PuzzleIOUtils::Strings& puzzle)
{
    assert(grid.getSize() == values.getSize());
    int rows = grid.getSize()[0];
    int columns = grid.getSize()[1];

    PuzzleIOUtils u;
    setupUtils(u);

    const int cellWidth = PuzzleIOUtils::kCellWidth;
    size_t kMinLineLength = (int)(cellWidth * columns + 1);

    if((int)puzzle.size() < 2 * rows + 1)
    {
        return Solver::kParseNotEnoughLines;
    }

    for(int i = 0; i < rows; ++i)
    {
        const std::string& floorLine = puzzle[(i + 1) * 2];
        const std::string& cellLine = puzzle[(i * 2) + 1];
        if(floorLine.length() < kMinLineLength || cellLine.length() < kMinLineLength)
        {
            return Solver::kParseLineTooShort;
        }
        for(int j = 0; j < columns; ++j)
        {
            std::string floor = floorLine.substr(j * cellWidth + 1, cellWidth - 1);
            bool isRowWall = floor == u.kRWall;
            if(!isRowWall && floor != u.kNoRWall)
            {
                return Solver::kParseInvalidFloor;
            }

            if(i == rows - 1)
            {
                std::string loopWall = puzzle[0].substr(j * cellWidth + 1, cellWidth - 1);
                if(isRowWall != (loopWall == u.kRWall))
                {
                    return Solver::kParseFloorWrapMismatch;
                }
            }

            std::string cell = cellLine.substr(j * cellWidth + 1, cellWidth);
            std::string rightWall = cell.substr(cellWidth - 1);
            bool isColumnWall = rightWall == u.kCWall;
            if(!isColumnWall && rightWall != u.kNoCWall)
            {
                return Solver::kParseInvalidWall;
            }

            if(j == columns - 1)
            {
                // Check that the wrap around holes actually match.
                if(isColumnWall != (cellLine.substr(0, 1) == u.kCWall))
                {
                    return Solver::kParseWallWrapMismatch;
                }
            }

            grid.setWall(coord(i, j), coord((i + 1) % rows, j), isRowWall);
            grid.setWall(coord(i, j), coord(i, (j + 1) % columns), isColumnWall);

            std::pair<bool, Symbol> lookup = u.findSymbol(cell.substr(0, cellWidth - 1));
            if(!lookup.first)
            {
                return Solver::kParseInvalidSymbol;
            }
            values.place(lookup.second, coord(i, j));
        }
    }

    return Solver::kParseSucceed;
}

void Solver::print(std::ostream& stream, const Grid2D& grid, const GridValues2D& values)
{
    assert(grid.getSize() == values.getSize());
    int rows = grid.getSize()[0];
    int columns = grid.getSize()[1];

    PuzzleIOUtils u;
    setupUtils(u);

    stream << u.kCorner;
    for(int column = 0; column < columns; ++column)
    {
        stream << (u.isWall(grid, coord(0, column), 0, false) ? u.kRWall : u.kNoRWall);
        stream << u.kCorner;
    }
    stream << std::endl;
    for(int row = 0; row < rows; ++row)
    {
        stream << (u.isWall(grid, coord(row, 0), 1, false) ? u.kCWall : u.kNoCWall);
        for(int column = 0; column < columns; ++column)
        {
            stream << u.findOutput(values[coord(row, column)]);
            stream << (u.isWall(grid, coord(row, column), 1, true) ? u.kCWall : u.kNoCWall);
        }
        stream << std::endl << u.kCorner;
        for(int column = 0; column < columns; ++column)
        {
            stream << (u.isWall(grid, coord(row, column), 0, true) ? u.kRWall : u.kNoRWall);
            stream << u.kCorner;
        }
        stream << std::endl;
    }
}

#ifdef BUILD_TESTS

#include "Test.h"
#include <sstream>

using Solver::Coordinate2D;

namespace
{
    class HourPuzzleIOTest : public UnitTest::Framework
    {
    public:
        void run()
        {
            RUN( parseTest );
            RUN( outputTest );
        }

        void setupGrid(Grid2D& grid)
        {
            grid.unwrap(0);
            grid.unwrap(1);

            grid.setWall(coord(0, 2), coord(1, 2), true);
            grid.setWall(coord(0, 4), coord(1, 4), true);
            grid.setWall(coord(1, 4), coord(2, 4), true);
            grid.setWall(coord(1, 5), coord(2, 5), true);
            grid.setWall(coord(2, 0), coord(3, 0), true);
            grid.setWall(coord(2, 2), coord(3, 2), true);
            grid.setWall(coord(2, 3), coord(3, 3), true);
            grid.setWall(coord(2, 4), coord(3, 4), true);

            grid.setWall(coord(0, 0), coord(0, 1), true);
            grid.setWall(coord(0, 3), coord(0, 4), true);
            grid.setWall(coord(1, 0), coord(1, 1), true);
            grid.setWall(coord(1, 1), coord(1, 2), true);
            grid.setWall(coord(1, 2), coord(1, 3), true);
            grid.setWall(coord(2, 1), coord(2, 2), true);
        }

        void setupPresets(GridValues2D& presets)
        {
            presets.place(12, coord(0, 0));
            presets.place( 9, coord(3, 0));
            presets.place( 7, coord(0, 5));
            presets.place( 2, coord(3, 5));
        }

        void parseTest()
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

            Coordinate2D size = coord(4, 6);
            Grid2D target(size);
            setupGrid(target);

            GridValues2D presets(size);
            setupPresets(presets);

            Grid2D grid(size);
            GridValues2D values(size);

            ParseResult result = Solver::parse(grid, values, Solver::asStrings(puzzle, size));
            CHECK(result == Solver::kParseSucceed);
            CHECK(target.isMatch(grid));
            CHECK(Solver::isMatch(presets, values));
        }

        void outputTest()
        {
            std::string target(
                "+----+----+----+----+----+----+\n"
                "| 12 |              |       7 |\n"
                "+    +    +----+    +----+    +\n"
                "|    |    |    |              |\n"
                "+    +    +    +    +----+----+\n"
                "|         |                   |\n"
                "+----+    +----+----+----+    +\n"
                "|  9                        2 |\n"
                "+----+----+----+----+----+----+\n"
            );

            Coordinate2D size = coord(4, 6);
            Grid2D grid(size);
            setupGrid(grid);

            GridValues2D presets(size);
            setupPresets(presets);

            std::ostringstream stream;
            Solver::print(stream, grid, presets);

            CHECK(target == stream.str());
        }
    };
}

DECLARE_TEST( HourPuzzleIOTest );

#endif // BUILD_TESTS
