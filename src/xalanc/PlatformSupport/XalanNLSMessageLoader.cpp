/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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


#if defined XALAN_NLS_MSG_LOADER


// Class header file.
#include "XalanNLSMessageLoader.hpp"

#include <xercesc/util/XMLMsgLoader.hpp>

#include <xalanc/PlatformSupport/DOMStringHelper.hpp>

#include <cstring>

#include <cerrno>

XALAN_CPP_NAMESPACE_BEGIN


XALAN_USING_XERCES(XMLMsgLoader)

static const char* const	MSG_FILE_NAME = "XalanMsg_";

#define MAX_LOCALTION_NAME 1024

static const char* const	s_errorMessage = "Message can't be located in the NLS catalog.";

XalanNLSMessageLoader::~XalanNLSMessageLoader()
{
	if ((int)m_catalogHandle != -1)
	{
		catclose(m_catalogHandle);
	}
}

XalanNLSMessageLoader::XalanNLSMessageLoader() :
	m_catalogHandle(nl_catd(-1))
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

    if ((int)m_catalogHandle == -1)
    {
        // Probably have to call panic here
        // the user will get an error with retrieving messages
 		assert(0);
    }
}

bool XalanNLSMessageLoader::loadMsg(XalanMessages::Codes    msgToLoad
								, XalanDOMChar*		toFill
								, unsigned int            maxChars)
{
    
    bool bRetValue = false;

    if( toFill == 0 || maxChars == 0 )
    {
    	return bRetValue;
    }

    if ((int)m_catalogHandle == -1)
    {
    	// for transcoding to Unicode
    	const XalanDOMString	errorMsg("Message can't be retrieved: the message catalog is not open.");
    	
    	if(errorMsg.length() < maxChars)
    	{
    		XalanCopy(errorMsg.c_str(), errorMsg.c_str()+ errorMsg.length() +1 , toFill);
    	}
    }
    else
    {
    	const char* const	catMessage = catgets( m_catalogHandle, 1, (int)msgToLoad+2, s_errorMessage);
	
		// catgets returns a pointer to msgString if it fails to locate the message
		// from the message catalog
    	if (catMessage != 0)
    	{
        	const XalanDOMString	errorMsg(catMessage);
        	
        	if(errorMsg.length() < maxChars)
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


