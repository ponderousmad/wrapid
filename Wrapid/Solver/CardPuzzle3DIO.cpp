/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Solver/CardPuzzle3DIO.h"

#include <assert.h>

using Solver::PuzzleIOUtils;
using Solver::Grid3D;
using Solver::GridValues3D;
using Solver::coord;
using Solver::Symbol;
using Solver::ParseResult;

namespace
{
    void setupUtils(PuzzleIOUtils& utils)
    {
        utils.addFinder(Solver::kUnsetSymbol, "   ");
        utils.addOutput(Solver::kUnsetSymbol, "   ");
        utils.setupCards(3);
    }
}


ParseResult Solver::parse(Grid3D& grid, GridValues3D& values, const PuzzleIOUtils::Strings& puzzle)
{
    assert(grid.getSize() == values.getSize());
    int rows = grid.getSize()[0];
    int columns = grid.getSize()[1];
    int levels = grid.getSize()[2];

    PuzzleIOUtils u;
    setupUtils(u);

    const int cellWidth = PuzzleIOUtils::kCellWidth;
    int levelWidth = (cellWidth * columns + 1);
    size_t kMinLineLength = (int)(levels * (levelWidth + 1) - 1);

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
        int levelOffset = 0;
        for(int z = 0; z < levels; ++z)
        {
            for(int j = 0; j < columns; ++j)
            {
                std::string floor = floorLine.substr(levelOffset + j * cellWidth+1, cellWidth-1);
                bool isRowWall = floor == u.kRWall;
                if(!isRowWall && floor != u.kNoRWall)
                {
                    return Solver::kParseInvalidFloor;
                }

                if(i == rows - 1)
                {
                    std::string loopWall = puzzle[0].substr(
                        levelOffset + j * cellWidth + 1, cellWidth - 1
                    );
                    if(isRowWall != (loopWall == u.kRWall))
                    {
                        return Solver::kParseFloorWrapMismatch;
                    }
                }

                std::string cell = cellLine.substr(levelOffset + j * cellWidth + 1, cellWidth);
                std::string rightWall = cell.substr(cellWidth - 1);
                bool isColumnWall = rightWall == u.kCWall;
                if(!isColumnWall && rightWall != u.kNoCWall)
                {
                    return Solver::kParseInvalidWall;
                }
                if(j == columns - 1)
                {
                    // Check that the wrap around holes actually match.
                    if(isColumnWall != (cellLine.substr(levelOffset, 1) == u.kCWall))
                    {
                        return Solver::kParseWallWrapMismatch;
                    }
                }

                std::string levelWall = cell.substr(cellWidth - 2, 1);
                bool isLevelWall = levelWall == u.kZWall;
                if(!isLevelWall && levelWall != u.kNoZWall)
                {
                    return Solver::kParseInvalidLevelTunnel;
                }

                grid.setWall(coord(i, j, z), coord((i + 1) % rows, j, z), isRowWall);
                grid.setWall(coord(i, j, z), coord(i, (j + 1) % columns, z), isColumnWall);
                grid.setWall(coord(i, j, z), coord(i, j, (z + 1) % levels), isLevelWall);

                std::pair<bool, Symbol> lookup = u.findSymbol(cell.substr(0, cellWidth-2));
                if(!lookup.first)
                {
                    return Solver::kParseInvalidSymbol;
                }
                values.place(lookup.second, coord(i, j, z));
            }
            levelOffset += levelWidth + 1;
        }
    }

    return Solver::kParseSucceed;
}

void Solver::print(std::ostream& stream, const Grid3D& grid, const GridValues3D& values)
{
    assert(grid.getSize() == values.getSize());
    int rows = grid.getSize()[0];
    int columns = grid.getSize()[1];
    int levels = grid.getSize()[2];

    PuzzleIOUtils u;
    setupUtils(u);

    for(int level = 0; level < levels; ++level)
    {
        if(level != 0)
        {
            stream << u.kLevelGap;
        }
        stream << u.kCorner;
        for(int column = 0; column < columns; ++column)
        {
            stream << (u.isWall(grid, coord(0, column, level), 0, false) ? u.kRWall : u.kNoRWall);
            stream << u.kCorner;
        }
    }
    stream << std::endl;
    for(int row = 0; row < rows; ++row)
    {
        for(int level = 0; level < levels; ++level)
        {
            if(level != 0)
            {
                stream << u.kLevelGap;
            }
            stream << (u.isWall(grid, coord(row, 0, level), 1, false) ? u.kCWall : u.kNoCWall);
            for(int column = 0; column < columns; ++column)
            {
                Solver::Coordinate3D location = coord(row, column, level);
                stream << u.findOutput(values[location]);
                stream << (u.isWall(grid, location, 2, true) ? u.kZWall : u.kNoZWall);
                stream << (u.isWall(grid, location, 1, true) ? u.kCWall : u.kNoCWall);
            }
        }
        stream << std::endl;
        for(int level = 0; level < levels; ++level)
        {
            if(level != 0)
            {
                stream << u.kLevelGap;
            }
            stream << u.kCorner;
            for(int column = 0; column < columns; ++column)
            {
                stream << (u.isWall(grid, coord(row, column, level), 0, true) ? u.kRWall : u.kNoRWall);
                stream << u.kCorner;
            }
        }
        stream << std::endl;
    }
}

#ifdef BUILD_TESTS

#include "Test.h"
#include <sstream>

using Solver::Coordinate3D;

namespace
{
    class CardPuzzle3DIOTest : public UnitTest::Framework
    {
    public:
        void run()
        {
            RUN( parseTest );
            RUN( outputTest );
        }

        void setupGrid(Grid3D& grid)
        {
            grid.unwrap(0);
            grid.unwrap(1);
            grid.blockAll(2);

            grid.setWall(coord(0, 0, 0), coord(0, 1, 0), true);
            grid.setWall(coord(1, 0, 0), coord(1, 1, 0), true);
            grid.setWall(coord(2, 0, 0), coord(2, 1, 0), true);
            grid.setWall(coord(3, 0, 0), coord(3, 1, 0), true);
            grid.setWall(coord(1, 1, 0), coord(1, 2, 0), true);
            grid.setWall(coord(2, 1, 0), coord(2, 2, 0), true);
            grid.setWall(coord(3, 1, 0), coord(3, 2, 0), true);
            grid.setWall(coord(4, 1, 0), coord(4, 2, 0), true);

            grid.setWall(coord(0, 2, 0), coord(1, 2, 0), true);
            grid.setWall(coord(4, 0, 0), coord(5, 0, 0), true);
            grid.setWall(coord(4, 2, 0), coord(5, 2, 0), true);

            grid.setWall(coord(0, 0, 1), coord(0, 1, 1), true);
            grid.setWall(coord(1, 0, 1), coord(1, 1, 1), true);
            grid.setWall(coord(2, 0, 1), coord(2, 1, 1), true);
            grid.setWall(coord(3, 0, 1), coord(3, 1, 1), true);
            grid.setWall(coord(1, 1, 1), coord(1, 2, 1), true);
            grid.setWall(coord(2, 1, 1), coord(2, 2, 1), true);
            grid.setWall(coord(3, 1, 1), coord(3, 2, 1), true);
            grid.setWall(coord(4, 1, 1), coord(4, 2, 1), true);

            grid.setWall(coord(0, 2, 1), coord(1, 2, 1), true);
            grid.setWall(coord(4, 0, 1), coord(5, 0, 1), true);
            grid.setWall(coord(4, 1, 1), coord(5, 1, 1), true);

            grid.setWall(coord(0, 0, 2), coord(0, 1, 2), true);
            grid.setWall(coord(1, 0, 2), coord(1, 1, 2), true);
            grid.setWall(coord(2, 0, 2), coord(2, 1, 2), true);
            grid.setWall(coord(3, 0, 2), coord(3, 1, 2), true);
            grid.setWall(coord(4, 0, 2), coord(4, 1, 2), true);
            grid.setWall(coord(5, 0, 2), coord(5, 1, 2), true);
            grid.setWall(coord(1, 1, 2), coord(1, 2, 2), true);
            grid.setWall(coord(2, 1, 2), coord(2, 2, 2), true);
            grid.setWall(coord(3, 1, 2), coord(3, 2, 2), true);
            grid.setWall(coord(4, 1, 2), coord(4, 2, 2), true);

            grid.setWall(coord(0, 2, 2), coord(1, 2, 2), true);
            grid.setWall(coord(4, 2, 2), coord(5, 2, 2), true);

            grid.setWall(coord(0, 0, 0), coord(0, 0, 1), false);
            grid.setWall(coord(4, 2, 0), coord(4, 2, 1), false);
            grid.setWall(coord(0, 2, 1), coord(0, 2, 2), false);
            grid.setWall(coord(3, 0, 1), coord(3, 0, 2), false);
            grid.setWall(coord(3, 2, 1), coord(3, 2, 2), false);
            grid.setWall(coord(5, 0, 2), coord(5, 0, 0), false);
        }

        void setupPresets(GridValues3D& presets)
        {
            presets.place( 3, coord(2, 0, 0));
            presets.place( Solver::kCardPuzzleAce, coord(5, 1, 1));
            presets.place( Solver::kCardPuzzleKing, coord(2, 2, 2));
        }

        void parseTest()
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
                "|         |   o| |         |    | |    |    |    |",
                "+----+    +----+ +----+----+    + +    +    +----+",
                "|              | |       A      | |   o|         |",
                "+----+----+----+ +----+----+----+ +----+----+----+"
            };

            Coordinate3D size = coord(6, 3, 3);
            Grid3D target(size);
            setupGrid(target);

            GridValues3D presets(size);
            setupPresets(presets);

            Grid3D grid(size);
            GridValues3D values(size);

            ParseResult result = Solver::parse(grid, values, Solver::asStrings(puzzle, size));
            CHECK(result == Solver::kParseSucceed);
            CHECK(target.isMatch(grid));
            CHECK(Solver::isMatch(presets, values));
        }

        void outputTest()
        {
            std::string target(
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
                "|   o|         | |    |        o| |    |         |\n"
                "+    +    +----+ +    +    +----+ +    +    +----+\n"
                "|    |    |    | |    |    |    | |    |    |    |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|  3 |    |    | |    |    |    | |    |    |  K |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|    |    |    | |   o|    |   o| |    |    |    |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|         |   o| |         |    | |    |    |    |\n"
                "+----+    +----+ +----+----+    + +    +    +----+\n"
                "|              | |       A      | |   o|         |\n"
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
            );

            Coordinate3D size = coord(6, 3, 3);
            Grid3D grid(size);
            setupGrid(grid);

            GridValues3D presets(size);
            setupPresets(presets);

            std::ostringstream stream;
            Solver::print(stream, grid, presets);

            std::string result = stream.str();
            CHECK(target == result);
        }
    };
}

DECLARE_TEST( CardPuzzle3DIOTest );

#endif // BUILD_TESTS
