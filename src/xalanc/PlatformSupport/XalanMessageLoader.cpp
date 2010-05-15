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


// Class header file.

// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>

#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>

#include <xalanc/Utils/XalanMsgLib/XalanMsgLib.hpp>

#include <xercesc/util/XMLString.hpp>

#if defined (XALAN_INMEM_MSG_LOADER)
#include <xalanc/PlatformSupport/XalanInMemoryMessageLoader.hpp>
#elif defined (XALAN_ICU_MSG_LOADER)
#include <xalanc/PlatformSupport/XalanICUMessageLoader.hpp>
#elif defined (XALAN_NLS_MSG_LOADER)
#include <xalanc/PlatformSupport/XalanNLSMessageLoader.hpp>
#else
#error You must provide a message loader
#endif

XALAN_CPP_NAMESPACE_BEGIN

static const size_t kMaxMessageLength = 1024;

XalanMessageLoader*     XalanMessageLoader::s_msgLoader = 0;

static MemoryManager*      s_initManager;

XalanMessageLoader::~XalanMessageLoader()
{
}



void
XalanMessageLoader::initialize(MemoryManager&      theManager)
{
#if defined (XALAN_INMEM_MSG_LOADER)
        typedef XalanMessageLoader::XalanMessageLoaderCreateFunct <XalanInMemoryMessageLoader> LoaderCreatorType;
#elif defined (XALAN_ICU_MSG_LOADER)
        typedef XalanMessageLoader::XalanMessageLoaderCreateFunct <XalanICUMessageLoader> LoaderCreatorType;
#elif defined (XALAN_NLS_MSG_LOADER)
        typedef XalanMessageLoader::XalanMessageLoaderCreateFunct <XalanNLSMessageLoader> LoaderCreatorType;
#else
#error You must provide a message loader
#endif

    if (s_msgLoader == 0)
    {
        s_initManager = &theManager;

        s_msgLoader = LoaderCreatorType()(theManager);
    }

    assert(s_msgLoader != 0);
}



void
XalanMessageLoader::terminate()
{

    typedef XalanMessageLoader::XalanMessageLoaderDestructFunct LoaderDestructType;

    assert(s_initManager != 0);
    
    LoaderDestructType()(*s_initManager, s_msgLoader);

    s_msgLoader = 0;
}



XalanDOMString&
XalanMessageLoader::getMessage(
            XalanDOMString&         theResultMessage,
            XalanMessages::Codes    msgToLoad,
            const XalanDOMString&   repText1 )
{
    assert(s_msgLoader != 0);

    XalanDOMChar    sBuffer[kMaxMessageLength + 1];

    s_msgLoader->load(
        msgToLoad,
        theResultMessage.getMemoryManager(),
        sBuffer,
        kMaxMessageLength,
        repText1.c_str());

    theResultMessage.assign(sBuffer);

    return theResultMessage;
}



XalanDOMString&
XalanMessageLoader::getMessage(
            XalanDOMString&         theResultMessage,
            XalanMessages::Codes    msgToLoad,
            const XalanDOMString&   repText1, 
            const XalanDOMString&   repText2)
{
    assert(s_msgLoader != 0);

    XalanDOMChar    sBuffer[kMaxMessageLength + 1];

    s_msgLoader->load(
        msgToLoad,
        theResultMessage.getMemoryManager(),
        sBuffer,
        kMaxMessageLength,
        repText1.c_str(),
        repText2.c_str());

    theResultMessage.assign(sBuffer);

    return theResultMessage;
}



XalanDOMString&
XalanMessageLoader::getMessage(
            XalanDOMString&         theResultMessage,
            XalanMessages::Codes    msgToLoad,
            const XalanDOMString&   repText1 ,
            const XalanDOMString&   repText2 ,
            const XalanDOMString&   repText3)
{
    assert(s_msgLoader != 0);

    XalanDOMChar    sBuffer[kMaxMessageLength + 1];

    s_msgLoader->load(
        msgToLoad,
        theResultMessage.getMemoryManager(),
        sBuffer,
        kMaxMessageLength,
        repText1.c_str(),
        repText2.c_str(),
        repText3.c_str());

    theResultMessage.assign(sBuffer);

    return theResultMessage;
}



XalanDOMString&
XalanMessageLoader::getMessage(
            XalanDOMString&         theResultMessage,
            XalanMessages::Codes    msgToLoad)

{
    assert(s_msgLoader != 0);

    XalanDOMChar    sBuffer[kMaxMessageLength + 1];

    s_msgLoader->loadMsg(msgToLoad,
                            sBuffer,
                            kMaxMessageLength) ;

    theResultMessage.assign(sBuffer);

    return theResultMessage;
}



bool
XalanMessageLoader::load(
            XalanMessages::Codes    msgToLoad,
            MemoryManager&          theMemoryManager,
            XalanDOMChar*           toFill,
            XalanSize_t             maxChars,
            const XalanDOMChar*     repText1, 
            const XalanDOMChar*     repText2, 
            const XalanDOMChar*     repText3,
            const XalanDOMChar*     repText4) 
{
    // Call the other version to load up the message
    if (!loadMsg(msgToLoad, toFill, maxChars))
        return false;

    XALAN_USING_XERCES(XMLString)

    // And do the token replacement
    XMLString::replaceTokens(
        toFill,
        maxChars,
        repText1,
        repText2,
        repText3,
        repText4,
        &theMemoryManager);

    return true;
}



XalanDOMString&
XalanMessageLoader::getMessage(
            XalanDOMString&         theResultMessage,
            XalanMessages::Codes    msgToLoad,
            const char*             repText1,
            const char*             repText2,
            const char*             repText3,
            const char*             repText4)
{
    XalanDOMChar    sBuffer[kMaxMessageLength + 1];

    s_msgLoader->load(
        msgToLoad,
        theResultMessage.getMemoryManager(),
        sBuffer,
        kMaxMessageLength,
        repText1,
        repText2,
        repText3,
        repText4);

    theResultMessage.assign(sBuffer);

    return theResultMessage;
}



XalanDOMString&
XalanMessageLoader::getMessage(
            XalanDOMString&         theResultMessage,
            XalanMessages::Codes    msgToLoad,
            const XalanDOMChar*     repText1, 
            const XalanDOMChar*     repText2,
            const XalanDOMChar*     repText3,
            const XalanDOMChar*     repText4)
{
    XalanDOMChar    sBuffer[kMaxMessageLength + 1];

    s_msgLoader->load(
        msgToLoad,
        theResultMessage.getMemoryManager(),
        sBuffer,
        kMaxMessageLength,
        repText1,
        repText2,
        repText3,
        repText4);

    theResultMessage.assign(sBuffer);

    return theResultMessage;
}



bool 
XalanMessageLoader::load(
            XalanMessages::Codes    msgToLoad,
            MemoryManager&      theManager,
            XalanDOMChar*           toFill,
            XalanSize_t             maxChars,
            const char*             repText1, 
            const char*             repText2, 
            const char*             repText3, 
            const char*             repText4)
{
    XalanDOMString theFirstParam(theManager);
    if (repText1 != 0)
    {
        theFirstParam.assign(repText1);
    }

    XalanDOMString theSecParam(theManager);
    if (repText2 != 0)
    {
        theSecParam.assign(repText2);
    }

    XalanDOMString theThirdParam(theManager);
    if( repText3 != 0)
    {
        theThirdParam.assign(repText3);
    }

    XalanDOMString theForthParam(theManager);
    if (repText4 != 0)
    {
        theForthParam.assign(repText4);
    }

    return load(
                msgToLoad,
                theManager,
                toFill,
                maxChars,
                theFirstParam.c_str(),
                theSecParam.c_str(),
                theThirdParam.c_str(),
                theForthParam.c_str());
}



XALAN_CPP_NAMESPACE_END
