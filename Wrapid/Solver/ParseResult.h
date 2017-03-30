#pragma once
#ifndef SOLVER_PARSERESULT_H__INCLUDED
#define SOLVER_PARSERESULT_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 * --------------------------------------------------------------- */

namespace Solver
{
    enum ParseResult
    {
        kParseSucceed,
        kParseNotEnoughLines,
        kParseLineTooShort,
        kParseInvalidSymbol,
        kParseInvalidFloor,
        kParseInvalidWall,
        kParseInvalidLevelTunnel,
        kParseInvalidVolumeTunnel,
        kParseFloorWrapMismatch,
        kParseWallWrapMismatch
    };
}

#endif // SOLVER_PARSERESULT_H__INCLUDED
