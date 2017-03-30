#pragma once
#ifndef SOLVER_COORDINATE_H__INCLUDED
#define SOLVER_COORDINATE_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include <assert.h>
#include <stdlib.h>

namespace Solver
{
    template <int Dimensions>
    class Coordinate;

    typedef Coordinate<2> Coordinate2D;
    typedef Coordinate<3> Coordinate3D;
    typedef Coordinate<4> Coordinate4D;
}

template <int Dimensions>
class Solver::Coordinate
{
public:
    Coordinate()
    {
        Zero(0);
    }

    Coordinate(int row)
    {
        assert(Dimensions > 0);
        mLocation[0] = row;
        Zero(1);
    }

    Coordinate(int row, int column)
    {
        assert(Dimensions > 1);
        mLocation[0] = row;
        mLocation[1] = column;
        Zero(2);
    }

    Coordinate(int row, int column, int z)
    {
        assert(Dimensions > 2);
        mLocation[0] = row;
        mLocation[1] = column;
        mLocation[2] = z;
        Zero(3);
    }

    Coordinate(int row, int column, int z, int w)
    {
        assert(Dimensions > 2);
        mLocation[0] = row;
        mLocation[1] = column;
        mLocation[2] = z;
        mLocation[3] = w;
        Zero(4);
    }

    int& operator[](int coord)
    {
        assert(coord >= 0 && coord < Dimensions);
        return mLocation[coord];
    }

    int operator[](int coord) const
    {
        assert(coord >= 0 && coord < Dimensions);
        return mLocation[coord];
    }

    int size() const
    {
        return Dimensions;
    }

    bool operator==(const Coordinate<Dimensions>& other) const
    {
        for(int d = 0; d < Dimensions; ++d)
        {
            if(mLocation[d] != other[d])
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Coordinate<Dimensions>& other) const
    {
        return !operator==(other);
    }

private:
    void Zero(int startIndex)
    {
        for(int i = startIndex; i < Dimensions; ++i)
        {
            mLocation[i] = 0;
        }
    }

    int mLocation[Dimensions];
};

namespace Solver
{
    inline Coordinate<1> coord(int row)
    {
        return Coordinate<1>(row);
    }

    inline Coordinate2D coord(int row, int column)
    {
        return Coordinate2D(row, column);
    }

    inline Coordinate3D coord(int row, int column, int z)
    {
        return Coordinate3D(row, column, z);
    }

    inline Coordinate<4> coord(int row, int column, int z, int w)
    {
        return Coordinate4D(row, column, z, w);
    }
}

#endif // SOLVER_COORDINATE_H__INCLUDED
