/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Solver/Grid.h"

#ifdef BUILD_TESTS

#include "Test.h"

using Solver::Grid2D;
using Solver::Grid3D;
using Solver::coord;

namespace
{
    class GridTest : public UnitTest::Framework
    {
    public:
        void run()
        {
            RUN( grid2DTest );
            RUN( grid3DTest );
        }

        void grid2DTest()
        {
            Grid2D grid(coord(3, 3));

            int count = 0;
            Grid2D::Neighbours n1 = grid.getNeighbours(coord(0, 0), false);
            while(n1.advance())
            {
                CHECK(
                    (n1.current()[0] == 0 && n1.current()[1] == 1) ||
                    (n1.current()[0] == 1 && n1.current()[1] == 0) ||
                    (n1.current()[0] == 0 && n1.current()[1] == 2) ||
                    (n1.current()[0] == 2 && n1.current()[1] == 0)
                );
                ++count;
            }
            CHECK(count == 4);

            count = 0;
            Grid2D::Neighbours fn = grid.getNeighbours(coord(0, 0), true);
            while(fn.advance())
            {
                CHECK(
                    (fn.current()[0] == 0 && fn.current()[1] == 1) ||
                    (fn.current()[0] == 1 && fn.current()[1] == 0)
                );
                ++count;
            }
            CHECK(count == 2);

            CHECK(grid.setWall(coord(0, 0), coord(0, 1), true));

            count = 0;
            Grid2D::Neighbours n2 = grid.getNeighbours(coord(0, 0), false);
            while(n2.advance())
            {
                CHECK(
                    (n2.current()[0] == 1 && n2.current()[1] == 0) ||
                    (n2.current()[0] == 0 && n2.current()[1] == 2) ||
                    (n2.current()[0] == 2 && n2.current()[1] == 0)
                );
                ++count;
            }
            CHECK(count == 3);

            grid.unwrap(0);
            count = 0;
            Grid2D::Neighbours n3 = grid.getNeighbours(coord(0, 0), false);
            while(n3.advance())
            {
                CHECK(
                    (n3.current()[0] == 1 && n3.current()[1] == 0) ||
                    (n3.current()[0] == 0 && n3.current()[1] == 2)
                );
                ++count;
            }
            CHECK(count == 2);

            grid.unwrap(1);
            count = 0;
            Grid2D::Neighbours n4 = grid.getNeighbours(coord(0, 0), false);
            while(n4.advance())
            {
                CHECK(n4.current()[0] == 1 && n4.current()[1] == 0);
                ++count;
            }
            CHECK(count == 1);
        }

        void grid3DTest()
        {
            Grid3D grid(coord(3, 3, 4));

            int count = 0;
            Grid3D::Neighbours n1 = grid.getNeighbours(coord(1, 1, 0), false);
            while(n1.advance())
            {
                CHECK(
                    (n1.current()[0] == 2 && n1.current()[1] == 1 && n1.current()[2] == 0) ||
                    (n1.current()[0] == 0 && n1.current()[1] == 1 && n1.current()[2] == 0) ||
                    (n1.current()[0] == 1 && n1.current()[1] == 2 && n1.current()[2] == 0) ||
                    (n1.current()[0] == 1 && n1.current()[1] == 0 && n1.current()[2] == 0) ||
                    (n1.current()[0] == 1 && n1.current()[1] == 1 && n1.current()[2] == 1) ||
                    (n1.current()[0] == 1 && n1.current()[1] == 1 && n1.current()[2] == 3)
                );
                ++count;
            }
            CHECK(count == 6);

            CHECK(grid.setWall(coord(1, 1, 0), coord(0, 1, 0), true));

            count = 0;
            Grid3D::Neighbours n2 = grid.getNeighbours(coord(1, 1, 0), false);
            while(n2.advance())
            {
                CHECK(
                    (n2.current()[0] == 2 && n2.current()[1] == 1 && n2.current()[2] == 0) ||
                    (n2.current()[0] == 1 && n2.current()[1] == 2 && n2.current()[2] == 0) ||
                    (n2.current()[0] == 1 && n2.current()[1] == 0 && n2.current()[2] == 0) ||
                    (n2.current()[0] == 1 && n2.current()[1] == 1 && n2.current()[2] == 1) ||
                    (n2.current()[0] == 1 && n2.current()[1] == 1 && n2.current()[2] == 3)
                );
                ++count;
            }
            CHECK(count == 5);

            grid.unwrap(0);
            count = 0;
            Grid3D::Neighbours n3 = grid.getNeighbours(coord(1, 1, 0), false);
            while(n3.advance())
            {
                CHECK(
                    (n3.current()[0] == 2 && n3.current()[1] == 1 && n3.current()[2] == 0) ||
                    (n3.current()[0] == 1 && n3.current()[1] == 2 && n3.current()[2] == 0) ||
                    (n3.current()[0] == 1 && n3.current()[1] == 0 && n3.current()[2] == 0) ||
                    (n3.current()[0] == 1 && n3.current()[1] == 1 && n3.current()[2] == 1) ||
                    (n3.current()[0] == 1 && n3.current()[1] == 1 && n3.current()[2] == 3)
                );
                ++count;
            }
            CHECK(count == 5);

            count = 0;
            Grid3D::Neighbours fn = grid.getNeighbours(coord(1, 1, 0), true);
            while(fn.advance())
            {
                CHECK(
                    (fn.current()[0] == 2 && fn.current()[1] == 1 && fn.current()[2] == 0) ||
                    (fn.current()[0] == 1 && fn.current()[1] == 2 && fn.current()[2] == 0) ||
                    (fn.current()[0] == 1 && fn.current()[1] == 1 && fn.current()[2] == 1)
                );
                ++count;
            }
            CHECK(count == 3);

            grid.unwrap(2);
            count = 0;
            Grid3D::Neighbours n4 = grid.getNeighbours(coord(1, 1, 0), false);
            while(n4.advance())
            {
                CHECK(
                    (n4.current()[0] == 2 && n4.current()[1] == 1 && n4.current()[2] == 0) ||
                    (n4.current()[0] == 1 && n4.current()[1] == 2 && n4.current()[2] == 0) ||
                    (n4.current()[0] == 1 && n4.current()[1] == 0 && n4.current()[2] == 0) ||
                    (n4.current()[0] == 1 && n4.current()[1] == 1 && n4.current()[2] == 1)
                );
                ++count;
            }
            CHECK(count == 4);
        }
    };
}

DECLARE_TEST( GridTest );

#endif // BUILD_TESTS
