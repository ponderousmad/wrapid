/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Top.h"
#include "Solver/GridValues.h"

#ifdef BUILD_TESTS

#include "Test.h"

using Solver::coord;
using Solver::Symbol;
using Solver::GridValues;

namespace
{
    class GridValuesTest : public UnitTest::Framework
    {
    public:
        void run()
        {
            RUN( gridValues2DTest );
            RUN( gridValues3DTest );
        }

        void gridValues2DTest()
        {
            const Symbol kSequenceMax = 9;
            Solver::Coordinate<2> size(3, 3);
            GridValues<2> values(size);

            CHECK(values[coord(0, 0)] == Solver::kUnsetSymbol);

            Symbol s = Solver::kFirstSymbol;
            for(int i = 0; i < size[0]; ++i)
            {
                for(int j = 0; j < size[1]; ++j)
                {
                    values.place(s, coord(i, j));
                    ++s;
                }
            }

            GridValues<2>::SymbolCounts counts = values.getSymbolCounts();

            CHECK(counts.size() == kSequenceMax);
            for(GridValues<2>::SymbolCounts::iterator it = counts.begin(); it != counts.end(); ++it)
            {
                CHECK(it->second == 1);
            }

            values.clear(coord(1, 1));
            CHECK(values[coord(1, 1)] == Solver::kUnsetSymbol);
        }

        void gridValues3DTest()
        {
            const Symbol kSequenceMax = 27;
            Solver::Coordinate<3> size(3, 3, 3);
            GridValues<3> values(size);

            CHECK(values[coord(0, 0, 0)] == Solver::kUnsetSymbol);

            Symbol s = Solver::kFirstSymbol;
            for(int i = 0; i < size[0]; ++i)
            {
                for(int j = 0; j < size[1]; ++j)
                {
                    for(int k = 0; k < size[2]; ++k)
                    {
                        values.place(s, coord(i, j, k));
                        ++s;
                    }
                }
            }

            GridValues<3>::SymbolCounts counts = values.getSymbolCounts();

            CHECK(counts.size() == kSequenceMax);
            for(GridValues<3>::SymbolCounts::iterator it = counts.begin(); it != counts.end(); ++it)
            {
                CHECK(it->second == 1);
            }

            values.clear(coord(0, 1, 2));
            CHECK(values[coord(0, 1, 2)] == Solver::kUnsetSymbol);
        }
    };
}

DECLARE_TEST( GridValuesTest );

#endif // BUILD_TESTS
