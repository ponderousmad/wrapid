#pragma once
#ifndef TEST_H__INCLUDED
#define TEST_H__INCLUDED

/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 *
 * NOTE: This file was intially developed as part of the Logress
 * project.  See http://code.google.com/p/logress
 * I've only included it here as part of the unit testing capability.
 *
 * The tests are defined like so:

#ifdef BUILD_TESTS

#include "Test.h"

namespace
{
    class ExampleTest : public UnitTest::Framework
    {
    public:
        void run()
        {
            RUN( aTest );
            RUN( anotherTest );
        }

        void aTest()
        {
            ...
            CHECK( <some condition> );
            ...
            CHECK_ASSERT( <condition that must pass to continue running the test> );
            ...
            CHECK( <condition> );
        }

        void anotherTest()
        {
            ...
            CHECK( <another condition> );
            ...
        }
    };
}

DECLARE_TEST( ExampleTest );

#endif // BUILD_TESTS

 * Additionally, the test name needs to be added to the
 * arguments to the runTests call in Wrapid.cpp
 * --------------------------------------------------------------- */

#ifdef BUILD_TESTS

#include <string>

#define RUN( test ) preSub( #test ); test(); postSub( #test )

#define CHECK( condition ) checkCondition( !!(condition), #condition, __FILE__, __LINE__ )
#define CHECK_ASSERT( condition ) CHECK( condition ); if( failing() ) return

#define DECLARE_TEST( test ) namespace { UnitTest::Initializer< test > initialze##test( #test ); }

namespace UnitTest {

    class Framework
    {
        PREVENT_COPY_AND_ASSIGNMENT(Framework);
    public:
        Framework();
        virtual ~Framework();

        virtual void run() = 0;

        void setName(const char* name);
        const char* name() const { return mName.c_str(); }

        void setFilesDir(const char* filesDir);
        std::string file(const std::string& filePath);

        void preSub(const char* subName);
        void postSub(const char* subName);

        void checkCondition(bool result, const char* condition, const char* fileName, int lineNumber);

        bool passed() { return mFailCount == 0; }
        bool failing() { return mTestFailed; }
        int subTestCount() { return mRunCount; }

        class Factory
        {
            PREVENT_COPY_AND_ASSIGNMENT(Factory);
        public:
            Factory();
            virtual ~Factory();
            virtual Framework* operator()() const = 0;
        };

        template <class T>
        class DefaultFactory : public Factory
        {
        public:
            Framework* operator()() const {
                return new T();
            }
        };

        static void registerTest(Factory*, const char* name);

        static int runTests(const char* tests[], const char* testFilesDir);

        static bool compareFiles(const char* first, const char* second);

    private:
        std::string mName;
        std::string mFilesDir;
        int mRunCount;
        int mFailCount;
        bool mTestFailed;
        bool mShowSubHeader;
    };

    template <class T >
    class Initializer
    {
        PREVENT_COPY_AND_ASSIGNMENT(Initializer);
    public:
        Initializer(const char* name)
        {
            Framework::registerTest(
                new Framework::DefaultFactory<T>(), name
            );
        }
    };
}

#endif // BUILD_TESTS

#endif // TEST_H__INCLUDED
