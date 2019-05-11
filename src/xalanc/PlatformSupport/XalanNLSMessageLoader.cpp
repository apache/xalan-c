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


#if defined XALAN_NLS_MSG_LOADER


// Class header file.
#include "XalanNLSMessageLoader.hpp"

#include <xercesc/util/XMLMsgLoader.hpp>

#include <xalanc/PlatformSupport/DOMStringHelper.hpp>

#include <locale.h>

#include <cstring>
#include <cstdlib>

#include <cerrno>

namespace XALAN_CPP_NAMESPACE {


using xercesc::XMLMsgLoader;

static const char* const    MSG_FILE_NAME = "Xalan.cat";

#define MAX_LOCALTION_NAME 1024

static const char* const    s_errorMessage = "Message can't be located in the NLS catalog.";

XalanNLSMessageLoader::~XalanNLSMessageLoader()
{
    if (m_catalogHandle != reinterpret_cast<nl_catd>(-1))
    {
        catclose(m_catalogHandle);
    }
}

XalanNLSMessageLoader::XalanNLSMessageLoader(MemoryManager& theManager) :
    m_catalogHandle(reinterpret_cast<nl_catd>(-1)),
    m_memoryManager(theManager)
{

    const char* Locale = XMLMsgLoader::getLocale();

    if(Locale == 0)
    {
        Locale = "en_US";
    }

    const char* msg = setlocale(LC_MESSAGES, 0);
    char *saved_msg = 0;
    if (msg)
    {
        saved_msg = new char[strlen(msg) + 1];
        strcpy(saved_msg, msg);
    }
    setlocale(LC_MESSAGES, Locale);

   /**
    * To open user-specified locale specific cat file
    * and default cat file if necessary
    */
    m_catalogHandle = catopen(MSG_FILE_NAME, 0);

    if(saved_msg)
      {
        setlocale(LC_MESSAGES, saved_msg);
        delete[] saved_msg;
      }

    if (m_catalogHandle == reinterpret_cast<nl_catd>(-1))
    {
        // Probably have to call panic here
        // the user will get an error with retrieving messages
        assert(0);
    }
}

bool
XalanNLSMessageLoader::loadMsg(
            XalanMessages::Codes    msgToLoad,
            XalanDOMChar*           toFill,
            XalanSize_t             maxChars)
{
    
    bool bRetValue = false;

    if (toFill == 0 || maxChars == 0)
    {
        return bRetValue;
    }

    if (m_catalogHandle == reinterpret_cast<nl_catd>(-1))
    {
        // for transcoding to Unicode
        const XalanDOMString    errorMsg("Message can't be retrieved: the message catalog is not open.", m_memoryManager );
        
        if(errorMsg.length() < maxChars)
        {
            XalanCopy(errorMsg.c_str(), errorMsg.c_str()+ errorMsg.length() + 1 , toFill);
        }
    }
    else
    {
        const char* const   catMessage =
            catgets(
                m_catalogHandle,
                1,
                static_cast<int>(msgToLoad) + 2,
                s_errorMessage);
    
        // catgets returns a pointer to msgString if it fails to locate the message
        // from the message catalog
        if (catMessage != 0)
        {
            const XalanDOMString    errorMsg(catMessage, m_memoryManager);
            
            if (errorMsg.length() < maxChars)
            {
                XalanCopy(errorMsg.c_str(), errorMsg.c_str() + errorMsg.length() +1  , toFill);
                
                bRetValue = true;
            }
        }
    }

    return bRetValue;
}



}



#endif // XALAN_NLS_MSG_LOADER
