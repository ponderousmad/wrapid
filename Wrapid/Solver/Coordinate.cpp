/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Solver/Coordinate.h"

#ifdef BUILD_TESTS

#include "Test.h"

using Solver::Coordinate;

namespace
{
    class CoordinateTest : public UnitTest::Framework
    {
    public:
        void run()
        {
            RUN( coordinate1DTest );
            RUN( coordinate2DTest );
        }
        
        void coordinate1DTest()
        {
            Coordinate<1> c;
            CHECK( c[0] == 0 );

            c[0] = 1;
            CHECK( c[0] == 1 );
            CHECK( c == Coordinate<1>(1) );
            CHECK( c != Coordinate<1>() );

            const Coordinate<1> d(2);

            CHECK( d[0] == 2 );
        }
        
        void coordinate2DTest()
        {
            Coordinate<2> point;
            CHECK( point[0] == 0 );
            CHECK( point[1] == 0 );

            point[0] = 1;
            CHECK( point[0] == 1 );

            point[1] = 2;
            CHECK( point[1] == 2 );

            CHECK( point == Coordinate<2>(1, 2) );
            CHECK( point != Coordinate<2>(2, 1) );

            const Coordinate<2> cPoint(2);

            CHECK( cPoint[0] == 2 );
            CHECK( cPoint[1] == 0 );

            const Coordinate<2> dPoint(3, 4);

            CHECK( dPoint[0] == 3 );
            CHECK( dPoint[1] == 4 );
        }
    };
}

DECLARE_TEST( CoordinateTest );

#endif // BUILD_TESTS
