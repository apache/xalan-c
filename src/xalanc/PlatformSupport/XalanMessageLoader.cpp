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
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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


// Class header file.

#include "DOMStringHelper.hpp"

#include "xalanc/Utils/XalanMsgLib/XalanMsgLib.hpp"

#if defined (XALAN_INMEM_MSG_LOADER)

#include "XalanInMemoryMessageLoader.hpp"

#elif defined (XALAN_ICU_MSG_LOADER)

#include "XalanICUMessageLoader.hpp"

#elif defined (XALAN_NLS_MSG_LOADER)
#include "XalanNLSMessageLoader.hpp"

#else
#error You must provide a message loader
#endif

#include <xercesc/util/XMLString.hpp>




XALAN_CPP_NAMESPACE_BEGIN


#define MAX_MESSAGE_LEN 1024


XalanMessageLoader*		XalanMessageLoader::s_msgLoader = 0;


XalanMessageLoader::~XalanMessageLoader()
{
}


void
XalanMessageLoader::createLoader()
{
	if (s_msgLoader == 0)
	{
#if defined (XALAN_INMEM_MSG_LOADER)
		s_msgLoader = new XalanInMemoryMessageLoader;
#elif defined (XALAN_ICU_MSG_LOADER)
		s_msgLoader = new XalanICUMessageLoader;
#elif defined (XALAN_NLS_MSG_LOADER)
		s_msgLoader = new XalanNLSMessageLoader;
#else
#error You must provide a message loader
#endif
	}

	assert(s_msgLoader != 0);
}



void
XalanMessageLoader::destroyLoader()
{
	delete s_msgLoader;

	s_msgLoader = 0;
}



const XalanDOMString
XalanMessageLoader::getMessage(
			XalanMessages::Codes	msgToLoad,
			const XalanDOMString& 	repText1 )
{
	assert(s_msgLoader != 0);

	XalanDOMChar	sBuffer[MAX_MESSAGE_LEN];

	s_msgLoader->load
		(
		msgToLoad
		, sBuffer
		, MAX_MESSAGE_LEN
		, repText1.c_str());


	return XalanDOMString(sBuffer);
}


const XalanDOMString
XalanMessageLoader::getMessage(XalanMessages::Codes		msgToLoad
		, const XalanDOMString& 				repText1 
		, const XalanDOMString& 				repText2)
{
	assert(s_msgLoader != 0);

	XalanDOMChar	sBuffer[MAX_MESSAGE_LEN];

	s_msgLoader->load
		(
		msgToLoad
		, sBuffer
		, MAX_MESSAGE_LEN
		, repText1.c_str()
		, repText2.c_str());


	return XalanDOMString(sBuffer);
}

const XalanDOMString
XalanMessageLoader::getMessage(
		XalanMessages::Codes		msgToLoad
		, const XalanDOMString& 	repText1 
		, const XalanDOMString& 	repText2 
		, const XalanDOMString& 	repText3)
{
	assert(s_msgLoader != 0);

	XalanDOMChar	sBuffer[MAX_MESSAGE_LEN];

	s_msgLoader->load
		(
		msgToLoad
		, sBuffer
		, MAX_MESSAGE_LEN
		, repText1.c_str()
		, repText2.c_str())
		, repText3.c_str();


	return XalanDOMString(sBuffer);
}

const XalanDOMString
XalanMessageLoader::getMessage(XalanMessages::Codes		msgToLoad)

{
	assert(s_msgLoader != 0);

	XalanDOMChar	sBuffer[MAX_MESSAGE_LEN];

	s_msgLoader->loadMsg
		(
		msgToLoad
		, sBuffer
		, MAX_MESSAGE_LEN
		) ;

	return XalanDOMString(sBuffer);
}

bool
XalanMessageLoader::load(
				XalanMessages::Codes	msgToLoad
				, XalanDOMChar*			toFill
				, unsigned int			maxChars
				, const XalanDOMChar* 	repText1 
				, const XalanDOMChar*	repText2 
				, const XalanDOMChar*	repText3 
				, const XalanDOMChar*	repText4) 
{
    // Call the other version to load up the message
    if (!loadMsg(msgToLoad, toFill, maxChars))
        return false;

	XALAN_USING_XERCES(XMLString)

    // And do the token replacement
    XMLString::replaceTokens(toFill, maxChars, repText1, repText2, repText3, repText4);

    return true;
}

const XalanDOMString
XalanMessageLoader::getMessage(XalanMessages::Codes		msgToLoad
					, const char* 			repText1 
					, const char* 			repText2
					, const char* 			repText3
					, const char* 			repText4)
{
	
	XalanDOMChar	sBuffer[MAX_MESSAGE_LEN];
	
	createLoader();

	s_msgLoader->load(
					msgToLoad
					, sBuffer
					, MAX_MESSAGE_LEN
					, repText1, repText2, repText3 , repText4) ;

	return XalanDOMString(sBuffer);
}



const XalanDOMString
XalanMessageLoader::getMessage(XalanMessages::Codes		msgToLoad
					, const XalanDOMChar* 			repText1 
					, const XalanDOMChar* 			repText2
					, const XalanDOMChar* 			repText3
					, const XalanDOMChar* 			repText4)
{
	
	XalanDOMChar	sBuffer[MAX_MESSAGE_LEN];
	
	createLoader();

	s_msgLoader->load(
					msgToLoad
					, sBuffer
					, MAX_MESSAGE_LEN
					, repText1, repText2, repText3 , repText4) ;

	return XalanDOMString(sBuffer);
}



bool 
XalanMessageLoader::load(
				XalanMessages::Codes	msgToLoad
				, XalanDOMChar*			toFill
				, unsigned int			maxChars
				, const char* 			repText1 
				, const char* 			repText2 
				, const char* 			repText3 
				, const char* 			repText4)
{
	XalanDOMString theFirstParam;
	if (repText1 != 0)
	{
		theFirstParam.assign(repText1);
	}

	XalanDOMString theSecParam;
	if (repText2 != 0)
	{
		theSecParam.assign(repText2);
	}

	XalanDOMString theThirdParam;
	if( repText3 != 0)
	{
		theThirdParam.assign(repText3);
	}

	XalanDOMString theForthParam;
	if (repText4 != 0)
	{
		theForthParam.assign(repText4);
	}

    return load(msgToLoad, toFill, maxChars, theFirstParam.c_str(), theSecParam.c_str(), 
							theThirdParam.c_str(), theForthParam.c_str());
}



XALAN_CPP_NAMESPACE_END
