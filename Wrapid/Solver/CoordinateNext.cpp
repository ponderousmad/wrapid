/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Solver/CoordinateNext.h"

#ifdef BUILD_TESTS

#include "Test.h"

using Solver::Coordinate;
using Solver::coord;

namespace
{
    class CoordinateNextTest : public UnitTest::Framework
    {
    public:
        void run()
        {
            RUN( inVolumeTest);
            RUN( coordinateNextTest );
        }

        void inVolumeTest()
        {
            CHECK(inVolume(coord(0, 0), coord(2, 4)));
            CHECK(inVolume(coord(1, 3), coord(2, 4)));
            CHECK(inVolume(coord(0, 0, 0), coord(2, 4, 1)));
            CHECK(inVolume(coord(1, 1, 0), coord(2, 4, 1)));
            CHECK(inVolume(coord(1, 3, 1), coord(2, 4, 2)));

            CHECK(!inVolume(coord(2, 4), coord(2, 4)));
            CHECK(!inVolume(coord(-1, 0), coord(2, 4)));
            CHECK(!inVolume(coord(1, 5, 0), coord(2, 4, 1)));
            CHECK(!inVolume(coord(2, 0, 0), coord(2, 4, 1)));
            CHECK(!inVolume(coord(1, 3, -1), coord(2, 4, 1)));
        }

        void coordinateNextTest()
        {
            Coordinate<3> current;
            Coordinate<3> size(2, 4, 1);
            Solver::CoordinateOrder<3> order(size);
            int count = 0;
            do
            {
                assert(count == (int)order(current));
                ++count;
                current = Solver::next(current, size);
            } while(current != size);

            assert(count == size[0] * size[1] * size[2]);
        }
    };
}

DECLARE_TEST( CoordinateNextTest );

#endif // BUILD_TESTS
