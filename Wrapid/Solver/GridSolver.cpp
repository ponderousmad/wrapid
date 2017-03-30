/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Solver\GridSolver.h"

#ifdef BUILD_TESTS

#include "Test.h"

using namespace Solver;

namespace
{
    class GridSolverTest : public UnitTest::Framework
    {
    public:
        void run()
        {
            RUN( solve2DTest );
            RUN( challengeTest );
            RUN( solve3DTest );
        }

        void solve2DTest()
        {
            Coordinate2D size = coord(4, 6);
            Grid2D grid(size);
            grid.unwrap(0);
            grid.unwrap(1);

            grid.setWall(coord(0, 1), coord(1, 1), true);
            grid.setWall(coord(0, 4), coord(1, 4), true);
            grid.setWall(coord(1, 0), coord(2, 0), true);
            grid.setWall(coord(1, 3), coord(2, 3), true);
            grid.setWall(coord(2, 1), coord(3, 1), true);
            grid.setWall(coord(2, 4), coord(3, 4), true);
            
            grid.setWall(coord(0, 2), coord(0, 3), true);
            grid.setWall(coord(1, 1), coord(1, 2), true);
            grid.setWall(coord(1, 4), coord(1, 5), true);
            grid.setWall(coord(2, 1), coord(2, 2), true);
            grid.setWall(coord(2, 2), coord(2, 3), true);
            grid.setWall(coord(2, 4), coord(2, 5), true);
            grid.setWall(coord(3, 2), coord(3, 3), true);

            Sequence symbols(12, 2);
            GridSolver<2> solver(grid, symbols);
            solver.addPreset(10, coord(0, 0));
            solver.addPreset( 7, coord(2, 1));
            solver.addPreset( 3, coord(1, 4));
            solver.addPreset( 8, coord(3, 5));

            GridValues<2> target(size);
            Symbol solution[4][6] = {
                {10, 11, 12,  1, 12, 11},
                { 9,  8,  1,  2,  3, 10},
                { 6,  7,  2,  5,  4,  9},
                { 5,  4,  3,  6,  7,  8}
            };
            for(int i = 0; i < size[0]; ++i)
            {
                for(int j = 0; j < size[1]; ++j)
                {
                    target.place(solution[i][j], coord(i, j));
                }
            }

            CHECK_ASSERT(solver.nextSolution() == Solver::kFoundSolution);
            CHECK(Solver::isMatch(target, solver.getSolution()));

            CHECK(solver.nextSolution() == Solver::kNoSolution);
        }

        void challengeTest()
        {
            Coordinate2D size = coord(4, 6);
            Grid2D grid(size);
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

            Sequence symbols(12, 2);
            GridSolver<2> solver(grid, symbols);
            solver.addPreset(12, coord(0, 0));
            solver.addPreset( 9, coord(3, 0));
            solver.addPreset( 7, coord(0, 5));
            solver.addPreset( 2, coord(3, 5));

            GridValues<2> target(size);
            Symbol solution1[4][6] = {
                {12,  7,  6,  5,  8,  7},
                {11,  8,  1,  4,  5,  6},
                {10,  9,  2,  3,  4,  3},
                { 9, 10, 11, 12,  1,  2}
            };
            for(int i = 0; i < size[0]; ++i)
            {
                for(int j = 0; j < size[1]; ++j)
                {
                    target.place(solution1[i][j], coord(i, j));
                }
            }

            CHECK_ASSERT(solver.nextSolution() == Solver::kFoundSolution);
            CHECK(isMatch(target, solver.getSolution()));

            Symbol solution2[4][6] = {
                {12,  7,  6,  5,  8,  7},
                {11,  8,  3,  4,  5,  6},
                {10,  9,  4,  3,  2,  1},
                { 9, 10, 11, 12,  1,  2}
            };
            for(int i = 0; i < size[0]; ++i)
            {
                for(int j = 0; j < size[1]; ++j)
                {
                    target.place(solution2[i][j], coord(i, j));
                }
            }

            CHECK_ASSERT(solver.nextSolution() == Solver::kFoundSolution);
            CHECK(Solver::isMatch(target, solver.getSolution()));

            CHECK(solver.nextSolution() == Solver::kNoSolution);
        }

        void solve3DTest()
        {
        }
    };
}

DECLARE_TEST( GridSolverTest );

#endif // BUILD_TESTS
