/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
