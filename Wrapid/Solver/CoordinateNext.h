#pragma once
#ifndef SOLVER_COORDINATENEXT_H__INCLUDED
#define SOLVER_COORDINATENEXT_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

#include "Coordinate.h"

namespace Solver
{
    // Checks if the specified coordinate has values in range [zero, size[d])
    template <int Dimensions>
    bool inVolume(const Coordinate<Dimensions>& c, const Coordinate<Dimensions>& size)
    {
        for(int d = 0; d < Dimensions; ++d)
        {
            if(c[d] < 0 || size[d] <= c[d])
            {
                return false;
            }
        }
        return true;
    }

    // Allow iterating through a coordinate volume. If repeatedly called starting
    // with the all zero coordinate value, will visit every coordinate within the
    // volume of the specified size.
    // Returns 'size' once the entire space has been exhausted (size is outside the volume)
    template <int Dimensions>
    Coordinate<Dimensions> next(Coordinate<Dimensions> c, const Coordinate<Dimensions>& size)
    {
        bool increment = true;
        for(int d = 0; d < Dimensions; ++d)
        {
            assert(c[d] >= 0);
            if(increment)
            {
                ++c[d];
                increment = false;
            }

            if(c[d] >= size[d])
            {
                c[d] = 0;
                increment = true;
            }
        }
        if(increment)
        {
            return size;
        }
        return c;
    }

    // Functor for use with coordinate hash maps or linear arrays.  Defines
    // a complete ordering on all coordinates bounded by size. Note that it does
    // not protect against overflow, so avoid very large volumes.
    //
    // The ordering is the same as that imposed by the 'next' function above.
    template <int Dimensions>
    struct CoordinateOrder
    {
        typedef Coordinate<Dimensions> Coord;

        // Declarations to support hash maps.
        enum
        {
            bucket_size = 4,
            min_buckets = 2 << Dimensions
        };

        CoordinateOrder(Coord size)
            : mStride(calculateStride(size))
        {
        }

        // Calculate the hash.
        size_t operator()(const Coord& c) const
        {
            // If the hashing was frequent enough, this might
            // be a good place to use SIMD compiler intrinsics.
            size_t hash = 0;
            for(int d = 0; d < Dimensions; ++d)
            {
                hash += c[d] * mStride[d];
            }
            return hash;
        }

        // Less than operation (based on the hash).
        bool operator()(const Coord& c, const Coord& d) const
        {
            return operator()(c) < operator()(d);
        }

        int stride(int dimension) const
        {
            return mStride[dimension];
        }

    private:
        static Coord calculateStride(Coord size)
        {
            Coord strides;
            int stride = 1;
            for(int d = 0; d < Dimensions; ++d)
            {
                strides[d] = stride;
                stride *= size[d];
            }
            return strides;
        }

        Coord mStride;
    };
}

#endif // SOLVER_COORDINATENEXT_H__INCLUDED
