/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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
#include "XalanTransformerDefinitions.hpp"



#include <util/PlatformUtils.hpp>



#include <cassert>
#if defined(XALAN_OLD_STREAM_HEADERS)
#include <strstream.h>
#else
#include <strstream>
#endif



#include "XalanCAPI.h"
#include "XalanTransformer.hpp"



XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanInitialize()
{
	// Call the static initializer for Xerces.
	XMLPlatformUtils::Initialize();

	// Initialize Xalan.
	XalanTransformer::initialize();
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanTerminate()
{
	// Terminate Xalan.
	XalanTransformer::terminate();

	// Call the static terminator for Xerces.
	XMLPlatformUtils::Terminate();
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(XalanHandle)
CreateXalanTransformer()
{	
	// Create a XalanTransformer object.
	return new XalanTransformer();
}



inline XalanTransformer*
getTransformer(XalanHandle	theHandle)
{
	assert(theHandle != 0);

#if defined(XALAN_OLD_STYLE_CASTS)
	return (XalanTransformer*)theHandle;
#else
	return static_cast<XalanTransformer*>(theHandle);
#endif
}



inline const XalanCompiledStylesheet*
getStylesheet(XalanCSSHandle	theHandle)
{
	assert(theHandle != 0);

#if defined(XALAN_OLD_STYLE_CASTS)
	return (const XalanCompiledStylesheet*)theHandle;
#else
	return reinterpret_cast<const XalanCompiledStylesheet*>(theHandle);
#endif
}



inline const XalanParsedSource*
getParsedSource(XalanPSHandle	theHandle)
{
	assert(theHandle != 0);

#if defined(XALAN_OLD_STYLE_CASTS)
	return (const XalanParsedSource*)theHandle;
#else
	return reinterpret_cast<const XalanParsedSource*>(theHandle);
#endif
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
DeleteXalanTransformer(XalanHandle theXalanHandle)
{
	// Delete a XalanTransformer object.
	delete getTransformer(theXalanHandle);
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanTransformToFile(
			const char*		theXMLFileName, 
			const char*		theXSLFileName,
			const char*		theOutFileName,
			XalanHandle		theXalanHandle)
{
	if(theXSLFileName == 0)
	{
		return getTransformer(theXalanHandle)->transform(
			theXMLFileName,
			theOutFileName);
	}
	else
	{
		return getTransformer(theXalanHandle)->transform(
			theXMLFileName,
			theXSLFileName,
			theOutFileName);
	}
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanTransformToFileCSS(
			const char*		theXMLFileName, 
			XalanCSSHandle	theCSSHandle,
			const char*		theOutFileName,
			XalanHandle		theXalanHandle)
{
	// Do the transformation...
	return getTransformer(theXalanHandle)->transform(
				theXMLFileName,
				getStylesheet(theCSSHandle),
				theOutFileName);
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanTransformToData(
			const char*		theXMLFileName, 
			const char*		theXSLFileName,
			char**			theOutput,
			XalanHandle		theXalanHandle)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::ostrstream;
#endif

	int	status = 0;		

	ostrstream	theOutputStream;	

	if(theXSLFileName == 0)
	{
		status = getTransformer(theXalanHandle)->transform(
			theXMLFileName,
			theOutputStream);
	}
	else
	{
		status = getTransformer(theXalanHandle)->transform(
			theXMLFileName,
			theXSLFileName,
			theOutputStream);
	}

	if (status == 0)
	{
		// Null-terminate the data.
		theOutputStream << '\0';

		*theOutput = theOutputStream.str();
	}

	return status;
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanTransformToDataCSS(
			const char*		theXMLFileName, 
			XalanCSSHandle	theCSSHandle,
			char**			theOutput,
			XalanHandle		theXalanHandle)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::ostrstream;
#endif

	ostrstream	theOutputStream;	

	// Do the transformation...
	const int	status =
		getTransformer(theXalanHandle)->transform(
			theXMLFileName,
			getStylesheet(theCSSHandle),
			theOutputStream);

	if (status == 0)
	{
		// Null-terminate the data.
		theOutputStream << '\0';

		*theOutput = theOutputStream.str();
	}

	return status;
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanFreeData(char*	theStream)
{
	// Delete the data.
	delete[] theStream;
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(int) 
XalanTransformToHandler(
			const char*				theXMLFileName, 
			const char*				theXSLFileName,
			XalanHandle				theXalanHandle,
			void*					theOutputHandle, 
			XalanOutputHandlerType	theOutputHandler,
			XalanFlushHandlerType	theFlushHandler)
{
	// Do the transformation...
	return getTransformer(theXalanHandle)->transform(
			theXMLFileName,
			theXSLFileName,
			theOutputHandle,
			theOutputHandler,
			theFlushHandler);
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(int) 
XalanTransformToHandlerCSS(
			const char*				theXMLFileName, 
			XalanCSSHandle			theCSSHandle,
			XalanHandle				theXalanHandle,
			void*					theOutputHandle, 
			XalanOutputHandlerType	theOutputHandler,
			XalanFlushHandlerType	theFlushHandler)
{
	// Do the transformation...
	return getTransformer(theXalanHandle)->transform(
			theXMLFileName,
			getStylesheet(theCSSHandle),
			theOutputHandle,
			theOutputHandler,
			theFlushHandler);
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(XalanCSSHandle)
XalanCompileStylesheet(
			const char*		theXSLFileName,
			XalanHandle		theXalanHandle)
{
	const XalanCompiledStylesheet*	theCompiledStylesheet = 0;

	getTransformer(theXalanHandle)->compileStylesheet(
		theXSLFileName,
		theCompiledStylesheet);

	return theCompiledStylesheet;
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(XalanPSHandle)
XalanParseSource(
			const char*		theXMLFileName,
			XalanHandle		theXalanHandle)
{
	const XalanParsedSource*	theParsedSource = 0;

	getTransformer(theXalanHandle)->parseSource(
		theXMLFileName,
		theParsedSource);

	return theParsedSource;
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(XalanPSHandle)
XalanParseSourceUseXerceDOM(
			const char*		theXMLFileName,
			XalanHandle		theXalanHandle)
{
	const XalanParsedSource*	theParsedSource = 0;

	getTransformer(theXalanHandle)->parseSource(
		theXMLFileName,
		theParsedSource,
		true);

	return theParsedSource;
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanSetStylesheetParam(
			const char*		key,
			const char*		expression,
			XalanHandle		theXalanHandle)
{
	getTransformer(theXalanHandle)->setStylesheetParam(
		XalanDOMString(key),
		XalanDOMString(expression));
}



XALAN_TRANSFORMER_EXPORT_FUNCTION(XalanCCharPtr)
XalanGetLastError(XalanHandle theXalanHandle)
{
	// Get the last error.
	return getTransformer(theXalanHandle)->getLastError();
}
