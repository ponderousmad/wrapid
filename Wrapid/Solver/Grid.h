#pragma once
#ifndef SOLVER_GRID_H__INCLUDED
#define SOLVER_GRID_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include <assert.h>
#include <stdlib.h>

#include "Solver/CoordinateNext.h"

#ifdef TRACE
#include <iostream>
#endif

namespace Solver
{
    template <int Dimensions>
    class Grid;

    typedef Grid<2> Grid2D;
    typedef Grid<3> Grid3D;
    typedef Grid<4> Grid4D;
}

// Keeps track of structure of the game board, specifically its size
// and the locations of all the walls.
template <int Dimensions>
class Solver::Grid
{
    PREVENT_COPY_AND_ASSIGNMENT(Grid);

public:
    typedef Coordinate<Dimensions> Coord;

    enum DirectionIndex
    {
        kReverseIndex = 0,
        kForwardIndex = 1,
        kDirectionCount = 2
    };

private:
    struct Walls
    {
        Walls()
        {
            for(int i = 0; i < Dimensions; ++i)
            {
                wall[i][kReverseIndex] = false;
                wall[i][kForwardIndex] = false;
            }
        }

        bool wall[Dimensions][kDirectionCount];
    };

public:
    Grid(Coord size)
        : mSize(size)
        , mOrder(size)
        , mStructure(constructStructure(size))
    {
    }

    ~Grid()
    {
        delete[] mStructure;
    }

    const Coord& getSize() const
    {
        return mSize;
    }
    
    int getTotalSize() const
    {
        return getTotalSize(mSize);
    }

    void unwrap(int dimension)
    {
        assert(dimension >= 0 && dimension < Dimensions);
        unwrap<Dimensions>(dimension, 0, false);
    }

    void blockAll(int dimension)
    {
        assert(dimension >= 0 && dimension < Dimensions);
        for(int i = getTotalSize() - 1; i >= 0; --i)
        {
            mStructure[i].wall[dimension][kForwardIndex] = true;
            mStructure[i].wall[dimension][kReverseIndex] = true;
        }
    }

    bool setWall(Coord first, Coord second, bool value)
    {
        int differAt = getAdjacency(first, second);

        if(differAt >= 0)
        {
            int difference = second[differAt] - first[differAt];
            bool forward = difference == 1 || first[differAt] == (mSize[differAt]-1);
            getWalls(first).wall[differAt][forward ? kForwardIndex : kReverseIndex] = value;
            getWalls(second).wall[differAt][forward ? kReverseIndex : kForwardIndex] = value;

            if(mSize[differAt] == 2 && !value)
            {
                // When the wrapping would mean seeing the same neighbour twice, force one wall on.
                getWalls(first).wall[differAt][forward ? kReverseIndex : kForwardIndex] = true;
                getWalls(second).wall[differAt][forward ? kForwardIndex : kReverseIndex] = true;
            }
            return true;
        }
        return false;
    }

    // Visit all the neighbours of a grid location that can be
    // reached without hitting walls. It's a lazy iterator.
    class Neighbours
    {
        PREVENT_ASSIGNMENT(Neighbours)
    public:
        Neighbours(const Grid<Dimensions>& grid, Coord location, bool onlyForward)
            : mGrid(grid)
            , mHome(location)
            , mWalls(grid.getWalls(location))
            , mDimension(-1)
            , mDirection(kReverseIndex)
            , mOnlyForward(onlyForward)
        {
        }

        bool advance()
        {
            bool advanced = false;
            do
            {
                advanced = advanceOne();
            } while(advanced && mWalls.wall[mDimension][mDirection]);
            return advanced;
        }

        Coord current() const
        {
            assert(mDimension < Dimensions);
            Coord c = mHome;
            if(mDirection == kForwardIndex)
            {
                if(c[mDimension] == mGrid.mSize[mDimension] - 1)
                {
                    c[mDimension] = 0;
                }
                else
                {
                    ++c[mDimension];
                }
            }
            else
            {
                if(c[mDimension] == 0)
                {
                    c[mDimension] = mGrid.mSize[mDimension] - 1;
                }
                else
                {
                    --c[mDimension];
                }
            }
            return c;
        }

    private:
        bool advanceOne()
        {
            if(mOnlyForward || mDirection == kReverseIndex)
            {
                ++mDimension;
                if(mDimension >= Dimensions)
                {
                    return false;
                }
                mDirection = kForwardIndex;
                return true;
            }
            else
            {
                mDirection = kReverseIndex;
                return true;
            }
        }

        const Grid<Dimensions>& mGrid;
        Coord mHome;
        const Walls& mWalls;

        int mDimension;
        DirectionIndex mDirection;
        bool mOnlyForward;
    };

    // Get iterator over the reachable neighbours of the specified location.
    // If onlyForward is true, it only returns neighbours with higher
    // coordinate values (or wrapped).
    Neighbours getNeighbours(Coord location, bool onlyForward) const
    {
        assert(inVolume(location, mSize));
        return Neighbours(*this, location, onlyForward);
    }

    const Walls& getWalls(Coord location) const
    {
        assert(inVolume(location, mSize));
        return mStructure[mOrder(location)];
    }

#ifdef BUILD_TESTS
    bool isMatch(const Grid<Dimensions>& other) const
    {
        if(mSize != other.getSize())
        {
            return false;
        }

        Coordinate<Dimensions> c;
        while(c != mSize)
        {
            const Walls& walls = getWalls(c);
            const Walls& otherWalls = other.getWalls(c);
            for(int i = 0; i < Dimensions; ++i)
            {
                for(int dir = kForwardIndex; dir < kDirectionCount; ++dir)
                {
                    if(walls.wall[i][dir] != otherWalls.wall[i][dir])
                    {
                        return false;
                    }
                }
            }
            c = next(c, mSize);
        }
        return true;
    }

    bool integrityCheck() const
    {
        Coordinate<Dimensions> c;
        while(c != mSize)
        {
            const Walls& walls = getWalls(c);
            for(int i = 0; i < Dimensions; ++i)
            {
                for(int dir = kForwardIndex; dir < kDirectionCount; ++dir)
                {
                    Coordinate<Dimensions> n = c;
                    n[i] = (n[i] + (dir == kForwardIndex ? 1 : -1)) % mSize[i];
                    if(walls.wall[i][dir] != getWalls(n).wall[i][dir == kForwardIndex ? kReverseIndex : kForwardIndex])
                    {
                        return false;
                    }
                }
            }
            c = next(c, mSize);
        }
        return true;
    }
#endif

private:
    Walls& getWalls(Coord location)
    {
        assert(inVolume(location, mSize));
        return mStructure[mOrder(location)];
    }

    int getAdjacency(Coord first, Coord second)
    {
        const int kInvalidAxis = -1;
        int differAt = kInvalidAxis;
        for(int d = 0; d < Dimensions; ++d)
        {
            int diff = first[d] - second[d];
            if(diff != 0)
            {
                if(differAt >= 0)
                {
                    // Multiple differences, so are not adjacent on the grid.
                    return kInvalidAxis;
                }

                if(abs(diff) != 1 && abs(diff) != (mSize[d]-1))
                {
                    // Difference too large, so are not adjacent on the grid.
                    return kInvalidAxis;
                }

                differAt = d;
            }
        }
        return differAt;
    }

    // Initially the grid has it possible to move circularly around the grid in a
    // torus (or hypertorus). This has the effect of unwrapping the grid in the
    // specified dimension.
    template <int D>
    void unwrap(int dimension, int offset, bool forward)
    {
        if(dimension == Dimensions - D)
        {
            // For the dimension in question, we only need to visit the first and last 'slice'
            unwrap<D-1>(dimension, offset, false);
            unwrap<D-1>(dimension, offset + (mSize[dimension]-1) * mOrder.stride(dimension), true);
        }
        else
        {
            for(int i = 0; i < mSize[Dimensions - D]; ++i)
            {
                unwrap<D-1>(dimension, offset + i * mOrder.stride(Dimensions - D), forward);
            }
        }
    }

    // Template recursion base case.
    template <>
    void unwrap<0>(int dimension, int offset, bool forward)
    {
        mStructure[offset].wall[dimension][forward ? kForwardIndex : kReverseIndex] = true;
    }

    static int getTotalSize(const Coord& size)
    {
        int totalSize = 1;
        for(int d = 0; d < Dimensions; ++d)
        {
            // We can't talk about adjacency without every axis having size at least 2.
            assert(size[d] > 1);
            totalSize *= size[d];
        }
        return totalSize;
    }

    static Walls* constructStructure(Coord size)
    {
        return new Walls[getTotalSize(size)];
    }

    Coord mSize;
    CoordinateOrder<Dimensions> mOrder;
    Walls* mStructure;
};

#endif // SOLVER_GRID_H__INCLUDED
