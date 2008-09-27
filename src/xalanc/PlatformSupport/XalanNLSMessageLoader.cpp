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

#include <cstring>

#include <cerrno>

XALAN_CPP_NAMESPACE_BEGIN


XALAN_USING_XERCES(XMLMsgLoader)

static const char* const    MSG_FILE_NAME = "XalanMsg_";

#define MAX_LOCALTION_NAME 1024

static const char* const    s_errorMessage = "Message can't be located in the NLS catalog.";

XalanNLSMessageLoader::~XalanNLSMessageLoader()
{
    if ((int)m_catalogHandle != -1)
    {
        catclose(m_catalogHandle);
    }
}

XalanNLSMessageLoader::XalanNLSMessageLoader(MemoryManager& theManager) :
    m_catalogHandle(nl_catd(-1)),
    m_memoryManager(theManager)
{

    char fileName[50];
    fileName[0] = 0;
    
    sprintf(fileName,"%s", MSG_FILE_NAME);

    const char* Locale = XMLMsgLoader::getLocale();

    if(Locale == 0)
    {
        Locale = "en_US";
    }

    strcat(fileName, Locale);

    strcat(fileName, ".cat");

   /**
    * To open user-specified locale specific cat file
    * and default cat file if necessary
    */
    m_catalogHandle = catopen(fileName, 0);

    if (reinterpret_cast<int>(m_catalogHandle) == -1)
    {
        // Probably have to call panic here
        // the user will get an error with retrieving messages
        assert(0);
    }
}

bool
XalanNLSMessageLoader::loadMsg(
            XalanMessages::Codes    msgToLoad
            XalanDOMChar*           toFill
            XalanSize_t             maxChars)
{
    
    bool bRetValue = false;

    if (toFill == 0 || maxChars == 0)
    {
        return bRetValue;
    }

    if (static_cast<int>(m_catalogHandle) == -1)
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



XALAN_CPP_NAMESPACE_END



#endif // XALAN_NLS_MSG_LOADER
