/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Solver/CardPuzzle4DIO.h"

#include <assert.h>

using Solver::PuzzleIOUtils;
using Solver::Grid4D;
using Solver::GridValues4D;
using Solver::coord;
using Solver::Symbol;
using Solver::ParseResult;

namespace
{
    void setupUtils(PuzzleIOUtils& utils)
    {
        utils.addFinder(Solver::kUnsetSymbol, "  ");
        utils.addOutput(Solver::kUnsetSymbol, "  ");
        utils.setupCards(2);
    }
}

ParseResult Solver::parse(Grid4D& grid, GridValues4D& values, const PuzzleIOUtils::Strings& puzzle)
{
    assert(grid.getSize() == values.getSize());
    int rows = grid.getSize()[0];
    int columns = grid.getSize()[1];
    int levels = grid.getSize()[2];
    int volumes = grid.getSize()[3];

    PuzzleIOUtils u;
    setupUtils(u);

    if((int)puzzle.size() < volumes * (2 * rows + 1))
    {
        return Solver::kParseNotEnoughLines;
    }

    if(volumes == 2)
    {
        // Since two volumes means that forward and backward openings loop to the same place,
        // we treat this as a special case, starting with all the volume walls up, and only
        // writing the value if it's an open.
        grid.blockAll(3);
    }

    const int cellWidth = PuzzleIOUtils::kCellWidth;
    int levelWidth = (cellWidth * columns + 1);
    size_t kMinLineLength = (int)(levels * (levelWidth + 1) - 1);

    int volumeOffset = 0;
    for(int w = 0; w < volumes; ++w)
    {
        for(int i = 0; i < rows; ++i)
        {
            const std::string& floorLine = puzzle[volumeOffset + (i + 1) * 2];
            const std::string& cellLine = puzzle[volumeOffset + (i * 2) + 1];
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
                        std::string loopWall = puzzle[volumeOffset].substr(
                            levelOffset + j * cellWidth + 1, cellWidth - 1
                        );
                        if(isRowWall != (loopWall == u.kRWall))
                        {
                            return Solver::kParseWallWrapMismatch;
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

                    std::string volumeWall = cell.substr(0, 1);
                    bool isVolumeWall = volumeWall == u.kWWall;
                    if(!isVolumeWall && volumeWall != u.kNoWWall)
                    {
                        return Solver::kParseInvalidVolumeTunnel;
                    }

                    grid.setWall(coord(i, j, z, w), coord((i + 1) % rows, j, z, w), isRowWall);
                    grid.setWall(coord(i, j, z, w), coord(i, (j + 1) % columns, z, w), isColumnWall);
                    grid.setWall(coord(i, j, z, w), coord(i, j, (z + 1) % levels, w), isLevelWall);
                    if(!isVolumeWall || volumes != 2)
                    {
                        // Always make a hole, but only build a wall if there are more than volumes
                        grid.setWall(coord(i, j, z, w), coord(i, j, z, (w + 1 ) % volumes), isVolumeWall);
                    }

                    std::pair<bool, Symbol> lookup = u.findSymbol(cell.substr(1, cellWidth-3));
                    if(!lookup.first)
                    {
                        return Solver::kParseInvalidSymbol;
                    }
                    values.place(lookup.second, coord(i, j, z, w));
                }
                levelOffset += levelWidth + 1;
            }
        }
        volumeOffset += 2 * rows + 1;
    }

    return Solver::kParseSucceed;
}

void Solver::print(std::ostream& stream, const Grid4D& grid, const GridValues4D& values)
{
    assert(grid.getSize() == values.getSize());
    int rows = grid.getSize()[0];
    int columns = grid.getSize()[1];
    int levels = grid.getSize()[2];
    int volumes = grid.getSize()[3];

    PuzzleIOUtils u;
    setupUtils(u);

    for(int volume = 0; volume < volumes; ++volume)
    {
        for(int level = 0; level < levels; ++level)
        {
            if(level != 0)
            {
                stream << u.kLevelGap;
            }
            stream << u.kCorner;
            for(int column = 0; column < columns; ++column)
            {
                stream << (u.isWall(grid, coord(0, column, level, volume), 0, false) ? u.kRWall : u.kNoRWall);
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
                stream << (u.isWall(grid, coord(row, 0, level, volume), 1, false) ? u.kCWall : u.kNoCWall);
                for(int column = 0; column < columns; ++column)
                {
                    Solver::Coordinate4D location = coord(row, column, level, volume);
                    stream << (u.isWall(grid, location, 3, true) ? u.kWWall : u.kNoWWall);
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
                    stream << (u.isWall(grid, coord(row, column, level, volume), 0, true) ? u.kRWall : u.kNoRWall);
                    stream << u.kCorner;
                }
            }
            stream << std::endl;
        }
    }
}

#ifdef BUILD_TESTS

#include "Test.h"
#include <sstream>

using Solver::Coordinate4D;

namespace
{
    class CardPuzzle4DIOTest : public UnitTest::Framework
    {
    public:
        void run()
        {
            RUN( parseTest );
            RUN( outputTest );
        }

        void setupGrid(Grid4D& grid)
        {
            grid.unwrap(0);
            grid.unwrap(1);
            grid.blockAll(2);
            grid.blockAll(3);

            grid.setWall(coord(0, 0, 0, 0), coord(0, 1, 0, 0), true);
            grid.setWall(coord(1, 0, 0, 0), coord(1, 1, 0, 0), true);
            grid.setWall(coord(2, 0, 0, 0), coord(2, 1, 0, 0), true);
            grid.setWall(coord(1, 1, 0, 0), coord(1, 2, 0, 0), true);

            grid.setWall(coord(0, 2, 0, 0), coord(1, 2, 0, 0), true);

            grid.setWall(coord(0, 0, 1, 0), coord(0, 1, 1, 0), true);
            grid.setWall(coord(1, 0, 1, 0), coord(1, 1, 1, 0), true);
            grid.setWall(coord(1, 1, 1, 0), coord(1, 2, 1, 0), true);
            grid.setWall(coord(2, 1, 1, 0), coord(2, 2, 1, 0), true);

            grid.setWall(coord(0, 2, 1, 0), coord(1, 2, 1, 0), true);

            grid.setWall(coord(0, 0, 2, 0), coord(0, 1, 2, 0), true);
            grid.setWall(coord(1, 0, 2, 0), coord(1, 1, 2, 0), true);
            grid.setWall(coord(2, 0, 2, 0), coord(2, 1, 2, 0), true);
            grid.setWall(coord(1, 1, 2, 0), coord(1, 2, 2, 0), true);
            grid.setWall(coord(2, 1, 2, 0), coord(2, 2, 2, 0), true);



            grid.setWall(coord(0, 0, 0, 1), coord(0, 1, 0, 1), true);
            grid.setWall(coord(0, 1, 0, 1), coord(0, 2, 0, 1), true);
            grid.setWall(coord(1, 1, 0, 1), coord(1, 2, 0, 1), true);

            grid.setWall(coord(1, 0, 0, 1), coord(2, 0, 0, 1), true);
            grid.setWall(coord(1, 2, 0, 1), coord(2, 2, 0, 1), true);

            grid.setWall(coord(0, 0, 1, 1), coord(0, 1, 1, 1), true);
            grid.setWall(coord(0, 1, 1, 1), coord(0, 2, 1, 1), true);
            grid.setWall(coord(1, 1, 1, 1), coord(1, 2, 1, 1), true);

            grid.setWall(coord(1, 0, 1, 1), coord(2, 0, 1, 1), true);
            grid.setWall(coord(1, 1, 1, 1), coord(2, 1, 1, 1), true);

            grid.setWall(coord(0, 0, 2, 1), coord(0, 1, 2, 1), true);
            grid.setWall(coord(1, 0, 2, 1), coord(1, 1, 2, 1), true);
            grid.setWall(coord(2, 0, 2, 1), coord(2, 1, 2, 1), true);
            grid.setWall(coord(0, 1, 2, 1), coord(0, 2, 2, 1), true);
            grid.setWall(coord(1, 1, 2, 1), coord(1, 2, 2, 1), true);

            grid.setWall(coord(1, 2, 2, 1), coord(2, 2, 2, 1), true);

            grid.setWall(coord(0, 0, 0, 0), coord(0, 0, 1, 0), false);
            grid.setWall(coord(1, 2, 0, 0), coord(1, 2, 1, 0), false);
            grid.setWall(coord(0, 2, 1, 0), coord(0, 2, 2, 0), false);

            grid.setWall(coord(1, 2, 0, 1), coord(1, 2, 1, 1), false);
            grid.setWall(coord(0, 0, 1, 1), coord(0, 0, 2, 1), false);
            grid.setWall(coord(0, 2, 1, 1), coord(0, 2, 2, 1), false);
            grid.setWall(coord(2, 0, 2, 1), coord(2, 0, 0, 1), false);

            grid.setWall(coord(1, 2, 1, 0), coord(1, 2, 1, 1), false);
            grid.setWall(coord(2, 1, 2, 0), coord(2, 1, 2, 1), false);
            grid.setWall(coord(1, 1, 0, 1), coord(1, 1, 0, 0), false);
        }

        void setupPresets(GridValues4D& presets)
        {
            presets.place( 2, coord(2, 0, 0, 0));
            presets.place( Solver::kCardPuzzleQueen, coord(1, 2, 2, 0));
            presets.place( 4, coord(0, 1, 1, 1));
        }

        void parseTest()
        {
            const char* puzzle[14] = {
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "|   o|         | |    |        o| |    |         |",
                "+    +    +----+ +    +    +----+ +    +    +    +",
                "|    |    |   o| |    |    |@   | |    |    |  Q |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|  2 |         | |         |    | |    |@   |    |",
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "+----+----+----+ +----+----+----+ +----+----+----+",
                "|    |    |    | |   o|  4 |   o| |    |    |    |",
                "+    +    +    + +    +    +    + +    +    +    +",
                "|     @   |   o| |         |    | |    |    |    |",
                "+----+    +----+ +----+----+    + +    +    +----+",
                "|              | |              | |   o|         |",
                "+----+----+----+ +----+----+----+ +----+----+----+"
            };

            Coordinate4D size = coord(3, 3, 3, 2);
            Grid4D target(size);
            setupGrid(target);

            GridValues4D presets(size);
            setupPresets(presets);

            Grid4D grid(size);
            GridValues4D values(size);

            ParseResult result = Solver::parse(grid, values, Solver::asStrings(puzzle, size));
            CHECK(result == Solver::kParseSucceed);
            CHECK(Solver::isMatch(presets, values));
        }

        void outputTest()
        {
            std::string target(
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
                "|   o|         | |    |        o| |    |         |\n"
                "+    +    +----+ +    +    +----+ +    +    +    +\n"
                "|    |    |   o| |    |    |@   | |    |    |  Q |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|  2 |         | |         |    | |    |@   |    |\n"
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
                "|    |    |    | |   o|  4 |   o| |    |    |    |\n"
                "+    +    +    + +    +    +    + +    +    +    +\n"
                "|     @   |   o| |         |    | |    |    |    |\n"
                "+----+    +----+ +----+----+    + +    +    +----+\n"
                "|              | |              | |   o|         |\n"
                "+----+----+----+ +----+----+----+ +----+----+----+\n"
            );

            Coordinate4D size = coord(3, 3, 3, 2);
            Grid4D grid(size);
            setupGrid(grid);

            GridValues4D presets(size);
            setupPresets(presets);

            std::ostringstream stream;
            Solver::print(stream, grid, presets);

            CHECK(target == stream.str());
        }
    };
}

DECLARE_TEST( CardPuzzle4DIOTest );

#endif // BUILD_TESTS
