/* ---------------------------------------------------------------
 * Copyright (c) Adrian Smith.
 *
 * NOTE: This file was intially developed as part of the Logress
 * project.  See http://code.google.com/p/logress
 * I've only included it here as part of the unit testing capability.
 * --------------------------------------------------------------- */

#include "Top.h"

#ifdef BUILD_TESTS

#include "Test.h"
#include "Utils/Stopwatch.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>

using std::endl;
using UnitTest::Framework;

Framework::Framework()
    : mName("")
    , mFilesDir("")
    , mRunCount(0)
    , mFailCount(0)
    , mTestFailed(false)
    , mShowSubHeader(false)
{
}

Framework::~Framework()
{
}

void Framework::setName(const char* name)
{
    assert(name);
    mName = name;
}

void Framework::setFilesDir(const char* filesDir)
{
    assert(filesDir);
    mFilesDir = filesDir;
    if(!mFilesDir.empty())
    {
        if(mFilesDir[ mFilesDir.size() - 1 ] != '\\')
        {
            mFilesDir += '\\';
        }
    }
}

std::string Framework::file(const std::string& filePath)
{
    return mFilesDir + filePath;
}

void Framework::preSub(const char* subName)
{
    if(mShowSubHeader)
    {
        std::cout << "~ " << name() << "::" << subName << " ~" << endl;
    }
    mTestFailed = false;
}

void Framework::postSub(const char* subName)
{
    ++mRunCount;
    if(mTestFailed)
    {
        ++mFailCount;
        std::cout << " FAIL : " << name() << "::" << subName << endl;
    }
}

void Framework::checkCondition(bool passed, const char* condition, const char* fileName, int lineNumber)
{
    if(!passed)
    {
        std::cout << fileName << "(" << lineNumber << "): { " << condition << " } failed." << endl;
        mTestFailed = true;
    }
}

namespace {
    typedef std::map<std::string, Framework::Factory*> Factories;
    Factories& getFactories()
    {
        static Factories sFactories;
        return sFactories;
    }

    Framework::Factory* findFactory(const std::string& name)
    {
        Factories::const_iterator found = getFactories().find(name);
        if(found == getFactories().end())
        {
            return 0;
        }

        return found->second;
    }

    void deleteFactories()
    {
        Factories& factories = getFactories();
        for(Factories::iterator it = factories.begin(); it != factories.end(); ++it)
        {
            delete it->second;
        }
        factories.clear();
    }
}

void Framework::registerTest(Factory* factory, const char* name)
{
    assert(getFactories().find(name) == getFactories().end());
    getFactories()[ name ] = factory;
}

int Framework::runTests(const char* tests[], const char* testFilesDir)
{
    int passed = 0;
    int failed = 0;

    for(int i = 0; tests[ i ]; ++i) {
        const char* testName = tests[ i ];
        Factory* pFactory = findFactory(testName);
        if(!pFactory)
        {
            std::cout  << "Unknown test: " << testName << std::endl;
            continue;
        }
        Factory& factory = *pFactory;

        float elapsed = 0.0f;
        std::auto_ptr<Framework> test(factory());
        test->setName(testName);
        test->setFilesDir(testFilesDir);
        {
            Utils::Stopwatch stopwatch;
            test->run();
            elapsed = static_cast<float>(stopwatch.elapsed());
        }
        if(test->passed())
        {
            ++passed;
            std::cout << "passed: " << std::setw(35) << std::left << testName << " (" << elapsed << "ms)\n";
        }
        else
        {
            ++failed;
        }
    }

    if(failed == 0)
    {
        std::cout << endl << passed << " TESTS PASSED" << endl;
    }
    else
    {
        std::cout << "\nTests passed: " << passed << "\nTests failed: " << failed << endl;
    }
    return failed;
}

bool Framework::compareFiles(const char* firstName, const char* secondName)
{
    std::ifstream first(firstName, std::ios::binary);
    std::ifstream second(secondName, std::ios::binary);
    while(first.good() && second.good())
    {
        if(first.get() != second.get())
        {
            return false;
        }
    }
    return first.eof() && second.eof();
}

Framework::Factory::Factory()
{
}

Framework::Factory::~Factory()
{
}

#endif
