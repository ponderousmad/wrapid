#ifndef STOPWATCH_H__INCLUDED
#define STOPWATCH_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 *
 * NOTE: This file was intially developed as part of the Logress
 * project.  See http://code.google.com/p/logress
 * --------------------------------------------------------------- */

#include <memory>

/*
 * Stopwatch provides high accuracy timing information.
 */

namespace Utils
{
    class Stopwatch;
}

class Utils::Stopwatch
{
    PREVENT_COPY_AND_ASSIGNMENT(Stopwatch);
public:
    // Stopwatch starts counting on construction.
    Stopwatch();
    ~Stopwatch();

    // Operate like a lap timer - return the
    // current elapsed time in ms and reset the clock
    double reset();

    // Read the time since the last reset in ms
    double elapsed() const;

private:
    void accumulate(double elapsed);
    class Impl;

    std::auto_ptr<Impl> mImpl;
};

#endif // STOPWATCH_H__INCLUDED
