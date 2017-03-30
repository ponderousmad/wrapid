#pragma once
#ifndef TOP_H__INCLUDED
#define TOP_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 *
 * This file defines some top level macros for clarifying code.
 * It also serves as a single point of reference for use with
 * precompiled header files, and so should be the first include
 * in every .cpp file.
 * --------------------------------------------------------------- */

#define PREVENT_COPY( C ) private: C( const C& );
#define PREVENT_ASSIGNMENT( C ) private: C& operator=( const C& );
#define PREVENT_COPY_AND_ASSIGNMENT( C ) private: C( const C& ); C& operator=( const C& );

#define NULLPTR 0

#endif // TOP_H__INCLUDED
