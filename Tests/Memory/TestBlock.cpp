/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
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
	operator()(const Type*	/* theType */) const
	{
	}
};



template<class Type>
class DeleteFunctor
{
public:

	void
	operator()(const Type*	theType) const
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
			int		argc,
			char*	argv[])
{
	if (argc != 3)
	{
		Usage();
	}
	else
	{
		const size_t	theCount = atol(argv[1]);
		const size_t	theBlockCount = atol(argv[2]);

		if (theCount <= 0 || theBlockCount <= 0)
		{
			Usage();
		}
		else
		{
			vector<string*>		theStringVector;

			theStringVector.reserve(theCount);

			clock_t	theStartClock = 0;

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
				_CrtMemState	theStartState;

				_CrtMemCheckpoint(&theStartState);
#endif

				StringArenaAllocator	m_allocator(theBlockCount);

				for (unsigned int i = 0; i < theCount; ++i)
				{
					theStringVector.push_back(m_allocator.create("Test string"));
				}

#if defined(_MSC_VER) && defined(_DEBUG)
				_CrtMemState	theEndState;

				_CrtMemCheckpoint(&theEndState);

				_CrtMemState	theDiffs;

				_CrtMemDifference(&theDiffs, &theStartState, &theEndState);

				_CrtMemDumpStatistics(&theDiffs);
#endif

				cout << "Create complete, hit <Enter> to continue..."
					 << endl;

//				cin.get();

				for_each(theStringVector.begin(),
						 theStringVector.end(),
						 NullFunctor<string>());
			}

			clock_t		theEndClock = clock();

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
				_CrtMemState	theStartState;

				_CrtMemCheckpoint(&theStartState);
#endif

				theStartClock = clock();

				ReusableStringArenaAllocator	m_allocator(theBlockCount);

				for (unsigned int i = 0; i < theCount; ++i)
				{
					theStringVector.push_back(m_allocator.create("Test string"));
				}

#if defined(_MSC_VER) && defined(_DEBUG)
				_CrtMemState	theEndState;

				_CrtMemCheckpoint(&theEndState);

				_CrtMemState	theDiffs;

				_CrtMemDifference(&theDiffs, &theStartState, &theEndState);

				_CrtMemDumpStatistics(&theDiffs);
#endif

				cout << "Create complete, hit <Enter> to continue..."
					 << endl;

//				cin.get();

				for_each(theStringVector.begin(),
						 theStringVector.end(),
						 NullFunctor<string>());

				for (unsigned int j = 0; j < theCount; ++j)
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
				_CrtMemState	theStartState;

				_CrtMemCheckpoint(&theStartState);
#endif

				for (unsigned int i = 0; i < theCount; ++i)
				{
					theStringVector.push_back(new string("Test string"));
				}

#if defined(_MSC_VER) && defined(_DEBUG)
				_CrtMemState	theEndState;

				_CrtMemCheckpoint(&theEndState);

				_CrtMemState	theDiffs;

				_CrtMemDifference(&theDiffs, &theStartState, &theEndState);

				_CrtMemDumpStatistics(&theDiffs);
#endif

				cout << "Create complete, hit <Enter> to continue..."
					 << endl;

//				cin.get();

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
