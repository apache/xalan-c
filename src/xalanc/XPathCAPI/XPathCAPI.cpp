/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001-2002 The Apache Software Foundation.  All rights 
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

#include "XPathCAPI.h"



#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/sax/SAXException.hpp>



#include <xalanc/Include/XalanAutoPtr.hpp>



#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>
#if defined(XALAN_ALLINONE_BUILD_DLL)
#include <xalanc/XPath/XPathConstructionContextDefault.hpp>
#endif
#include <xalanc/XPath/XPathEvaluator.hpp>
#include <xalanc/XPath/XPathExecutionContextDefault.hpp>
#include <xalanc/XPath/XPathFactoryDefault.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeInit.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



static bool	fInitialized = false;
static bool	fTerminated = false;




XALAN_USING_XALAN(XalanDOMChar)
XALAN_USING_XALAN(XalanDOMString)
XALAN_USING_XALAN(XalanOutputTranscoder)
XALAN_USING_XALAN(XPath)
XALAN_USING_XALAN(XPathEvaluator)
XALAN_USING_XALAN(XalanSourceTreeInit)
XALAN_USING_XALAN(XalanTranscodingServices)



static XalanSourceTreeInit*		theSourceTreeInit = 0;



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanXPathAPIInitialize()
{
	if (fInitialized == true)
	{
		return XALAN_XPATH_API_ERROR_ALREADY_INITIALIZED;
	}
	else if (fTerminated == true)
	{
		return XALAN_XPATH_API_ERROR_CANNOT_REINITIALIZE;
	}
	else
	{
		int	theResult = XALAN_XPATH_API_SUCCESS;

		try
		{
			XALAN_USING_XERCES(XMLPlatformUtils)

			XMLPlatformUtils::Initialize();

			try
			{
				XPathEvaluator::initialize();

				try
				{
					theSourceTreeInit = new XalanSourceTreeInit;
				}
				catch(...)
				{
					XPathEvaluator::terminate();

					throw;
				}
			}
			catch(...)
			{
				XMLPlatformUtils::Terminate();

				throw;
			}

			fInitialized = true;
		}
		catch(...)
		{
			fTerminated = true;

			theResult = XALAN_XPATH_API_ERROR_INITIALIZATION_FAILED;
		}

		return theResult;
	}
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanXPathAPITerminate()
{
	if (fInitialized == false)
	{
		return XALAN_XPATH_API_ERROR_NOT_INITIALIZED;
	}
	else if (fTerminated == true)
	{
		return XALAN_XPATH_API_ERROR_ALREADY_TERMINATED;
	}
	else
	{
		int	theResult = XALAN_XPATH_API_SUCCESS;

		try
		{
			XALAN_USING_XERCES(XMLPlatformUtils)

			delete theSourceTreeInit;

			theSourceTreeInit = 0;

			XPathEvaluator::terminate();

			XMLPlatformUtils::Terminate();

			fTerminated = true;
		}
		catch(...)
		{
			theResult = XALAN_XPATH_API_ERROR_TERMINATION_FAILED;
		}

		return theResult;
	}
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanCreateXPathEvaluator(XalanXPathEvaluatorHandle*	theHandle)
{
	if (fInitialized == false)
	{
		return XALAN_XPATH_API_ERROR_NOT_INITIALIZED;
	}
	else if (fTerminated == true)
	{
		return XALAN_XPATH_API_ERROR_ALREADY_TERMINATED;
	}
	else if (theHandle == 0)
	{
		return XALAN_XPATH_API_ERROR_INVALID_PARAMETER;
	}
	else
	{
		int	theResult = XALAN_XPATH_API_SUCCESS;

		try
		{
			*theHandle = new XPathEvaluator;
		}
		catch(...)
		{
			theResult = XALAN_XPATH_API_ERROR_UNKNOWN;
		}

		return theResult;
	}
}



inline XPathEvaluator*
getEvaluator(XalanXPathEvaluatorHandle	theHandle)
{
	assert(theHandle != 0);

#if defined(XALAN_OLD_STYLE_CASTS)
	return (XPathEvaluator*)theHandle;
#else
	return static_cast<XPathEvaluator*>(theHandle);
#endif
}



inline XPath*
getXPath(XalanXPathHandle	theHandle)
{
	assert(theHandle != 0);

#if defined(XALAN_OLD_STYLE_CASTS)
	return (XPath*)theHandle;
#else
	return static_cast<XPath*>(theHandle);
#endif
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanDestroyXPathEvaluator(XalanXPathEvaluatorHandle	theXalanHandle)
{
	if (fInitialized == false)
	{
		return XALAN_XPATH_API_ERROR_NOT_INITIALIZED;
	}
	else if (fTerminated == true)
	{
		return XALAN_XPATH_API_ERROR_ALREADY_TERMINATED;
	}
	else if (theXalanHandle == 0)
	{
		return XALAN_XPATH_API_ERROR_INVALID_PARAMETER;
	}
	else
	{
		int	theResult = XALAN_XPATH_API_SUCCESS;

		try
		{
			delete getEvaluator(theXalanHandle);
		}
		catch(...)
		{
			theResult = XALAN_XPATH_API_ERROR_UNKNOWN;
		}

		return theResult;
	}
}



inline int
transcodeString(
			XalanOutputTranscoder*	theTranscoder,
			const char*				theString,
			XalanDOMChar*			theChars,
			unsigned char*			theCharsCount,
			size_t					theLength,
			XalanDOMString&			theResultString)
{
	assert(theTranscoder != 0);
	assert(theString != 0);
	assert(theChars != 0);
	assert(theCharsCount != 0);

	size_t	theSourceCharsTranscoded = 0;
	size_t	theTargetBytesUsed = 0;

	const XalanTranscodingServices::eCode	theCode = theTranscoder->transcode(
#if defined(XALAN_OLD_STYLE_CASTS)
					(const XalanOutputTranscoder::XalanXMLByte*)theString,
#else
					reinterpret_cast<const XalanOutputTranscoder::XalanXMLByte*>(theString),
#endif
					theLength,
					theChars,
					theLength,
					theSourceCharsTranscoded,
					theTargetBytesUsed,
					theCharsCount);

	if (theCode != XalanTranscodingServices::OK)
	{
		return XALAN_XPATH_API_ERROR_TRANSCODING;
	}
	else
	{
		theResultString.assign(theChars, theTargetBytesUsed);

		return XALAN_XPATH_API_SUCCESS;
	}
}



inline int
transcodeString(
			const char*			theString,
			const char*			theStringEncoding,
			XalanDOMString&		theResultString)
{
	assert(theString != 0);

	int		theResult = XALAN_XPATH_API_SUCCESS;

	if (theStringEncoding == 0 || XalanDOMString::length(theStringEncoding) == 0)
	{
		theResultString = theString;
	}
	else
	{
		XalanTranscodingServices::eCode		theCode = XalanTranscodingServices::OK;

		XalanOutputTranscoder* const	theTranscoder = 
			XalanTranscodingServices::makeNewTranscoder(
						XalanDOMString(theStringEncoding),
						theCode,
						1024);

		if (theCode == XalanTranscodingServices::UnsupportedEncoding)
		{
			theResult = XALAN_XPATH_API_ERROR_UNSUPPORTED_ENCODING;
		}
		else if (theCode != XalanTranscodingServices::OK)
		{
			theResult = XALAN_XPATH_API_ERROR_UNKNOWN;
		}
		else
		{
			assert(theTranscoder != 0);

			// Since UTF-16 can represent any Unicode value in
			// one 16-bit value, we'll use the length of the
			// string as the maximum length of the resulting
			// transcoded string.  This doesn't account for
			// surrogate pairs, but those are for private use
			// only right now, so we don't really need to
			// worry about them.
			const XalanDOMString::size_type		theLength = XalanDOMString::length(theString);

			// Only use a dynamically-allocated array for very long
			// XPath expressions.
			const XalanDOMString::size_type		maxStackArraySize = 100;

			if (theLength >= maxStackArraySize)
			{
				XALAN_USING_XALAN(XalanArrayAutoPtr)

				XalanArrayAutoPtr<unsigned char>	theCharsCount(new unsigned char[theLength + 1]);
				XalanArrayAutoPtr<XalanDOMChar>		theChars(new XalanDOMChar[theLength + 1]);

				theResult = transcodeString(
								theTranscoder,
								theString,
								theChars.get(),
								theCharsCount.get(),
								theLength,
								theResultString);
			}
			else
			{
				unsigned char	theCharsCount[maxStackArraySize];
				XalanDOMChar	theChars[maxStackArraySize];

				theResult = transcodeString(
								theTranscoder,
								theString,
								theChars,
								theCharsCount,
								theLength,
								theResultString);
			}

			XalanTranscodingServices::destroyTranscoder(theTranscoder);
		}
	}

	return theResult;
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanCreateXPath(
			XalanXPathEvaluatorHandle	theXalanHandle,
			const char*					theXPathExpression,
			const char*					theXPathExpressionEncoding,
			XalanXPathHandle*			theXPathHandle)
{
	if (fInitialized == false)
	{
		return XALAN_XPATH_API_ERROR_NOT_INITIALIZED;
	}
	else if (fTerminated == true)
	{
		return XALAN_XPATH_API_ERROR_ALREADY_TERMINATED;
	}
	else if (theXalanHandle == 0 || theXPathHandle == 0 || theXPathExpression == 0 || XalanDOMString::length(theXPathExpression) == 0)
	{
		return XALAN_XPATH_API_ERROR_INVALID_PARAMETER;
	}
	else
	{
		int		theResult = XALAN_XPATH_API_SUCCESS;

		try
		{
			XPathEvaluator* const	theEvaluator = getEvaluator(theXalanHandle);
			assert(theEvaluator != 0);

			XalanDOMString	theExpressionString;

			theResult = transcodeString(
				theXPathExpression,
				theXPathExpressionEncoding,
				theExpressionString);

			if (theResult == XALAN_XPATH_API_SUCCESS)
			{
				const XalanDOMChar* const	thePointer = theExpressionString.c_str();

				XALAN_USING_XALAN(length)

				if (length(thePointer) == 0)
				{
					theResult = XALAN_XPATH_API_ERROR_TRANSCODING;
				}
				else
				{
					*theXPathHandle = theEvaluator->createXPath(thePointer);
				}
			}
		}
		catch(...)
		{
			theResult = XALAN_XPATH_API_ERROR_INVALID_XPATH;
		}

		return theResult;
	}
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanDestroyXPath(
			XalanXPathEvaluatorHandle	theXalanHandle,
			XalanXPathHandle			theXPathHandle)
{
	if (fInitialized == false)
	{
		return XALAN_XPATH_API_ERROR_NOT_INITIALIZED;
	}
	else if (fTerminated == true)
	{
		return XALAN_XPATH_API_ERROR_ALREADY_TERMINATED;
	}
	else if (theXalanHandle == 0 || theXPathHandle == 0)
	{
		return XALAN_XPATH_API_ERROR_INVALID_PARAMETER;
	}
	else
	{
		int	theResult = XALAN_XPATH_API_SUCCESS;

		try
		{
			XPathEvaluator* const	theEvaluator = getEvaluator(theXalanHandle);
			assert(theEvaluator != 0);

			if (theEvaluator->destroyXPath(getXPath(theXPathHandle)) == false)
			{
				theResult = XALAN_XPATH_API_ERROR_INVALID_XPATH;
			}
		}
		catch(...)
		{
			theResult = XALAN_XPATH_API_ERROR_UNKNOWN;
		}

		return theResult;
	}
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanEvaluateXPathAsBoolean(
			XalanXPathEvaluatorHandle	theXalanHandle,
			XalanXPathHandle			theXPathHandle,
			const char*					theXML,
			int*						theResult)
{
	if (fInitialized == false)
	{
		return XALAN_XPATH_API_ERROR_NOT_INITIALIZED;
	}
	else if (fTerminated == true)
	{
		return XALAN_XPATH_API_ERROR_ALREADY_TERMINATED;
	}
	else if (theXalanHandle == 0 || theXPathHandle == 0 || theXML == 0)
	{
		return XALAN_XPATH_API_ERROR_INVALID_PARAMETER;
	}
	else
	{
		int	theError = XALAN_XPATH_API_SUCCESS;

		XALAN_USING_XERCES(SAXException)

		try
		{
			XPathEvaluator* const	theEvaluator = getEvaluator(theXalanHandle);
			assert(theEvaluator != 0);

			XPath* const	theXPath = getXPath(theXPathHandle);
			assert(theXPath != 0);

			XALAN_USING_XALAN(XalanDocument)
			XALAN_USING_XALAN(XalanSourceTreeDOMSupport)
			XALAN_USING_XALAN(XalanSourceTreeParserLiaison)

			XalanSourceTreeDOMSupport		theDOMSupport;
			XalanSourceTreeParserLiaison	theLiaison(theDOMSupport);

			// Hook the two together...
			theDOMSupport.setParserLiaison(&theLiaison);

			XALAN_USING_XERCES(MemBufInputSource)

			// Create an input source...
			const MemBufInputSource		theInputSource(
#if defined(XALAN_OLD_STYLE_CASTS)
											(const XMLByte*)theXML,
#else
											reinterpret_cast<const XMLByte*>(theXML),
#endif
											XalanDOMString::length(theXML),
											"SourceXML",
											false);

			// Parse the document...
			XalanDocument* const	theDocument =
							theLiaison.parseXMLStream(theInputSource);
			assert(theDocument != 0);

			if (theEvaluator->evaluate(theDOMSupport, theDocument, *theXPath)->boolean() == true)
			{
				*theResult = 1;
			}
			else
			{
				*theResult = 0;
			}
		}
		catch(const SAXException&)
		{
			theError = XALAN_XPATH_API_ERROR_BAD_XML;
		}
		catch(...)
		{
			theError = XALAN_XPATH_API_ERROR_UNKNOWN;
		}

		return theError;
	}
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanEvaluateXPathExpressionAsBoolean(
			XalanXPathEvaluatorHandle	theXalanHandle,
			const char*					theXPathExpression,
			const char*					theXPathExpressionEncoding,
			const char*					theXML,
			int*						theResult)
{
	XalanXPathHandle	theXPathHandle = 0;

	int		theError = XalanCreateXPath(
					theXalanHandle,
					theXPathExpression,
					theXPathExpressionEncoding,
					&theXPathHandle);

	if (theError == XALAN_XPATH_API_SUCCESS)
	{
		assert(theXPathHandle != 0);

		theError = XalanEvaluateXPathAsBoolean(theXalanHandle, theXPathHandle, theXML, theResult);

		const int	theDestroyResult =
			XalanDestroyXPath(theXalanHandle, theXPathHandle);

		if (theDestroyResult != XALAN_XPATH_API_SUCCESS)
		{
			theError = theDestroyResult;
		}
	}

	return theError;
}
