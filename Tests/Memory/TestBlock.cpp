/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(_MSC_VER)

#pragma warning(disable: 4127 4251 4511 4512 4514 4702 4710 4711 4786 4097; error: 4150 4172 4238 4239 4715)

#if defined(_DEBUG)

#include <crtdbg.h>

#endif

#endif



#include "ArenaBlock.hpp"
#include "ReusableArenaBlock.hpp"
#include "ArenaAllocator.hpp"
#include "ReusableArenaAllocator.hpp"
#include "StringArenaAllocator.hpp"
#include "ReusableStringArenaAllocator.hpp"


#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>



template<class Type>
class NullFunctor
{
public:

    void
    operator()(const Type*  /* theType */) const
    {
    }
};



template<class Type>
class DeleteFunctor
{
public:

    void
    operator()(const Type*  theType) const
    {
        delete theType;
    }
};



using std::cin;
using std::cout;
using std::endl;
using std::for_each;
using std::string;
using std::vector;



void
Usage()
{
    cout << endl
         << "Usage: ArenaBlock <count> <block size>"
         << endl;
}



int
main(
            int     argc,
            char*   argv[])
{
    if (argc != 3)
    {
        Usage();
    }
    else
    {
        const size_t    theCount = atol(argv[1]);
        const size_t    theBlockCount = atol(argv[2]);

        if (theCount <= 0 || theBlockCount <= 0)
        {
            Usage();
        }
        else
        {
            vector<string*>     theStringVector;

            theStringVector.reserve(theCount);

            clock_t theStartClock = 0;

#if defined(_MSC_VER) && defined(_DEBUG)
            // Send all reports to STDOUT
            _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
            _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
            _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
            _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
            _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
            _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

#endif

            {
                theStartClock = clock();

#if defined(_MSC_VER) && defined(_DEBUG)
                _CrtMemState    theStartState;

                _CrtMemCheckpoint(&theStartState);
#endif

                StringArenaAllocator    m_allocator(theBlockCount);

                for (size_t i = 0; i < theCount; ++i)
                {
                    theStringVector.push_back(m_allocator.create("Test string"));
                }

#if defined(_MSC_VER) && defined(_DEBUG)
                _CrtMemState    theEndState;

                _CrtMemCheckpoint(&theEndState);

                _CrtMemState    theDiffs;

                _CrtMemDifference(&theDiffs, &theStartState, &theEndState);

                _CrtMemDumpStatistics(&theDiffs);
#endif

                cout << "Create complete, hit <Enter> to continue..."
                     << endl;

//              cin.get();

                for_each(theStringVector.begin(),
                         theStringVector.end(),
                         NullFunctor<string>());
            }

            clock_t     theEndClock = clock();

            cout << "Time to create and delete "
                 << theCount
                 << " strings using the arena allocator was "
                 << theEndClock - theStartClock
                 << " clock ticks."
                 << endl
                 << endl;

            theStringVector.clear();

            {
#if defined(_MSC_VER) && defined(_DEBUG)
                _CrtMemState    theStartState;

                _CrtMemCheckpoint(&theStartState);
#endif

                theStartClock = clock();

                ReusableStringArenaAllocator    m_allocator(theBlockCount);

                for (size_t i = 0; i < theCount; ++i)
                {
                    theStringVector.push_back(m_allocator.create("Test string"));
                }

#if defined(_MSC_VER) && defined(_DEBUG)
                _CrtMemState    theEndState;

                _CrtMemCheckpoint(&theEndState);

                _CrtMemState    theDiffs;

                _CrtMemDifference(&theDiffs, &theStartState, &theEndState);

                _CrtMemDumpStatistics(&theDiffs);
#endif

                cout << "Create complete, hit <Enter> to continue..."
                     << endl;

//              cin.get();

                for_each(theStringVector.begin(),
                         theStringVector.end(),
                         NullFunctor<string>());

                for (size_t j = 0; j < theCount; ++j)
                {
                    m_allocator.destroy(theStringVector[j]);
                }
            }

            theEndClock = clock();

            cout << "Time to create and delete "
                 << theCount
                 << " strings using the reusable arena allocator was "
                 << theEndClock - theStartClock
                 << " clock ticks."
                 << endl
                 << endl;

            theStringVector.clear();

            {
                theStartClock = clock();

#if defined(_MSC_VER) && defined(_DEBUG)
                _CrtMemState    theStartState;

                _CrtMemCheckpoint(&theStartState);
#endif

                for (size_t i = 0; i < theCount; ++i)
                {
                    theStringVector.push_back(new string("Test string"));
                }

#if defined(_MSC_VER) && defined(_DEBUG)
                _CrtMemState    theEndState;

                _CrtMemCheckpoint(&theEndState);

                _CrtMemState    theDiffs;

                _CrtMemDifference(&theDiffs, &theStartState, &theEndState);

                _CrtMemDumpStatistics(&theDiffs);
#endif

                cout << "Create complete, hit <Enter> to continue..."
                     << endl;

//              cin.get();

                for_each(theStringVector.begin(),
                         theStringVector.end(),
                         DeleteFunctor<string>());

            }

            theEndClock = clock();

            cout << "Time to create and delete "
                 << theCount
                 << " strings using standard new and delete was "
                 << theEndClock - theStartClock
                 << " clock ticks."
                 << endl
                 << endl;
        }
    }

    return 0;
}
