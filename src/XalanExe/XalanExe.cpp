/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights 
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

#include <Include/PlatformDefinitions.hpp>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif



#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



#include <xercesc/util/PlatformUtils.hpp>



#include <Include/XalanAutoPtr.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "C:/Program Files/Rational/Quantify/pure.h"
#endif



#if !defined (XALAN_NO_NAMESPACES)
using std::cerr;
using std::cout;
using std::endl;
#endif



void
Usage()
{
	cerr << endl
		 << "Xalan version 1.3"
		 << endl
		 << "Usage: Xalan [options] source stylesheet"
		 << endl
	     << "Options:"
		 << endl
		 << "  -a                    Use xml-stylesheet PI, not the 'stylesheet' argument"
		 << endl
		 << "  -o filename           Write output to the specified file."
		 << endl
		 << "  -p name expression    Sets a stylesheet parameter."
		 << endl
		 << "  -v                    Validates source documents."
		 << endl
		 << "  -?                    Display this message."
		 << endl;
}



class Params
{
public:

	Params(unsigned long	maxParams) :
		m_validate(false),
		m_useStylesheetPI(false),
		m_inFileName(0),
		m_xslFileName(0),
		m_outFileName(0),
		m_params(),
		m_maxParams(maxParams),
		m_currentParam(0)
	{
	}

	bool
	addParam(
			const char*		name,
			const char*		expression)
	{
		if (m_currentParam == m_maxParams)
		{
			return false;
		}
		else
		{
			// Allocate memory if necessary...
			if (m_params.get() == 0)
			{
				m_params.reset(new ParamPair[m_maxParams]);
			}
			assert(m_params.get() != 0);

			m_params[m_currentParam].m_name = name;
			m_params[m_currentParam].m_expression = expression;

			++m_currentParam;

			return true;
		}
	};

	void
	setParams(XalanTransformer&		theTransformer)
	{
		theTransformer.setUseValidation(m_validate);

		for(unsigned long i = 0; i < m_currentParam; ++i)
		{
			theTransformer.setStylesheetParam(
				m_params[i].m_name,
				m_params[i].m_expression);
		}
	}

	bool			m_validate;
	bool			m_useStylesheetPI;

	const char*		m_inFileName;
	const char*		m_xslFileName;
	const char*		m_outFileName;

private:

	struct ParamPair
	{
		ParamPair() :
			m_name(0),
			m_expression(0)
		{
		}

		const char*		m_name;
		const char*		m_expression;
	};

	XalanArrayAutoPtr<ParamPair>	m_params;

	const unsigned long				m_maxParams;
	unsigned long					m_currentParam;
};



bool
getArgs(
			int					argc,
			const char*			argv[],
			Params&				params)
{
#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::strlen;
#endif

	bool fSuccess = true;

	for (int i = 1; i < argc && fSuccess == true; ++i)
	{
		if (argv[i][0] == '-')
		{
			if (strlen(argv[i]) != 2)
			{
				fSuccess = false;
			}
			else if (params.m_inFileName != 0 || params.m_xslFileName != 0)
			{
				fSuccess = false;
			}
			else if (argv[i][1] == 'a') 
			{
				params.m_useStylesheetPI = true;
			}
			else if (argv[i][1] == 'o') 
			{
				++i;

				if(i < argc && argv[i][0] != '-' &&
				   strlen(argv[i]) != 0)
				{
					params.m_outFileName = argv[i];
				}
				else
				{
					fSuccess = false;
				}
			}
			else if (argv[i][1] == 'p') 
			{
				++i;

				if(i >= argc || argv[i][0] == '-')
				{
					fSuccess = false;
				}
				else
				{
					const char* const	name = argv[i];

					++i;

					// Don't check for '-' here, since that might
					// be a valid character in a parameter value.
					if(i >= argc)
					{
						fSuccess = false;
					}
					else
					{
						const char* const	value = argv[i];

						if (params.addParam(name, value) == false)
						{
							cerr << "Maximum numbers of stylesheets params has been exceeded!" << endl;

							fSuccess = false;
						}
					}
				}
			}
			else if (argv[i][1] == 'v')
			{
				params.m_validate = true;
			}
		}
		else if (params.m_inFileName == 0 &&
				 strlen(argv[i]) != 0)
		{
			params.m_inFileName = argv[i];

			if (strlen(params.m_inFileName) == 0)
			{
				fSuccess = false;
			}
		}
		else if (params.m_xslFileName == 0 &&
				 strlen(argv[i]) != 0 &&
				 params.m_useStylesheetPI == false)
		{
			params.m_xslFileName = argv[i];

			if (strlen(params.m_xslFileName) == 0)
			{
				fSuccess = false;
			}
		}
		else
		{
			fSuccess = false;
		}
	}

	if (fSuccess == true && params.m_inFileName == 0)
	{
		return false;
	}
	else if (params.m_xslFileName == 0 && params.m_useStylesheetPI == false)
	{
		return false;
	}
	else
	{
		return fSuccess;
	}
}



int
xsltMain(
			int				argc,
			const char*		argv[])

{
	int	theResult = -1;

	// Set the maximum number of params as half of argc - 1.
	// It's actually argc - 2, but that could get us into negative
	// numbers, so don't bother.  Also, always add 1, in case
	// (argc - 1) / 2 is 0.
	Params	theParams((argc - 1) / 2 + 1);

	if (getArgs(argc, argv, theParams) == false)
	{
		Usage();
	}
	else
	{
		// Call the static initializer for Xerces...
		XMLPlatformUtils::Initialize();

		// Initialize Xalan...
		XalanTransformer::initialize();

		{
			// Create a XalanTransformer instance...
			XalanTransformer	theTransformer;

			// Set any options...
			theParams.setParams(theTransformer);

			if (theParams.m_outFileName != 0)
			{
				theResult = theTransformer.transform(
						theParams.m_inFileName,
						theParams.m_xslFileName,
						theParams.m_outFileName);
			}
			else
			{
				theResult = theTransformer.transform(
						theParams.m_inFileName,
						theParams.m_xslFileName,
						cout);
			}

			if (theResult != 0)
			{
				cerr << theTransformer.getLastError() << endl;
			}
		}

		// Terminate Xalan...
		XalanTransformer::terminate();

		// Terminate Xerces...
		XMLPlatformUtils::Terminate();
	}

	return theResult;
}



int
main(
			int				argc,
			const char*		argv[])
{
#if !defined(XALAN_USE_ICU) && !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

#if defined(XALAN_VQ_SPECIAL_TRACE)
	QuantifyStopRecordingData();
#endif

    if (argc < 2)
	{
		Usage();

		return -1;
	}
	else
	{
		return xsltMain(argc, argv);
	}
}
