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

#if defined XALAN_ICU_MSG_LOADER


// Class header file.
#include "XalanICUMessageLoader.hpp"

#include "PlatformSupportInit.hpp"

#include "DOMStringHelper.hpp"

#include "unicode/uloc.h"

#include "unicode/udata.h" 

#include <xercesc/util/XMLMsgLoader.hpp> //

#include <xercesc/util/XMLString.hpp>

#ifdef WIN32
extern "C" const char U_IMPORT  xalanMsg_1_6_dat []; 
static const char* const	sPackageName="xalanMsg_1_6";
#else
extern "C" const char U_IMPORT  xalanMsg_dat [];
static const char* const	sPackageName="xalanMsg";
#endif



XALAN_CPP_NAMESPACE_BEGIN

XALAN_USING_XERCES(XMLMsgLoader)

XALAN_USING_XERCES(XMLString)



// ---------------------------------------------------------------------------
//  Local static methods
// ---------------------------------------------------------------------------

/*
 *  Resource Data Reference.  
 * 
 *  The data is packaged as a dll (or .so or whatever, depending on the platform) that exports a data symbol.
 *  The application (thic *.cpp) references that symbol here, and will pass the data address to ICU, which 
 *  will then  be able to fetch resources from the data.
 */


static const char* domainName = "XalanMsg";



XalanICUMessageLoader::XalanICUMessageLoader():
	m_localeBundle(0),
	m_domainBundle(0),
	m_unknownMessage(XALAN_STATIC_UCODE_STRING("The message was not found in the ICU resource bundle."))
{
 	UErrorCode err = U_ZERO_ERROR;

#ifdef WIN32
	udata_setAppData(sPackageName, &xalanMsg_1_6_dat, &err);
#else
	udata_setAppData(sPackageName, &xalanMsg_dat, &err);
#endif

	if (! U_SUCCESS(err))
	{
		assert( 0 );
	}
	const char* szLocal = XMLMsgLoader::getLocale();
	if ( szLocal == 0 )
	{
		// Opps , we failed to get local from Xerces, let's try our
		szLocal="en_US";
	}

	fLocaleBundle = ures_open(sPackageName, szLocal , &err);
	if (!U_SUCCESS(err) || fLocaleBundle == 0)
	{
		assert( 0 );
	}

	err = U_ZERO_ERROR;
    fDomainBundle = ures_getByKey(fLocaleBundle, domainName, 0, &err);
    if (!U_SUCCESS(err) || fDomainBundle == 0)
    {
        assert( 0 );
    }
}



XalanICUMessageLoader::~XalanICUMessageLoader()
{
	ures_close(m_domainBundle);	
	ures_close(m_localeBundle);
}



bool XalanICUMessageLoader::loadMsg(XalanMessages::Codes    msgToLoad
									, XalanDOMChar*		toFill
									, unsigned int      maxChars)
{


   bool bResult = true;
   UErrorCode   err = U_ZERO_ERROR;
   int32_t      strLen = 0;
   
   // Assuming array format
   const UChar * const	name = ures_getStringByIndex(fDomainBundle, (int32_t)msgToLoad+1, &strLen, &err);
   
   if (!U_SUCCESS(err) || (name == 0))
   {
	   XalanCopy(m_unknownMessage.c_str(), m_unknownMessage.c_str() + m_unknownMessage.length() + 1, toFill);

	   return false;
   }


   int retStrLen = strLen > (int32_t)maxChars ? maxChars : strLen;
   
   if (sizeof(UChar)==sizeof(XMLCh))
   {

	   XMLString::moveChars(toFill, (const XMLCh*)name, retStrLen);
	   toFill[retStrLen] = (XMLCh) 0;
   }
   else
   {
	      
	   XMLCh* retStr = toFill;
	   const UChar *srcPtr = name;
	   
	   while (retStrLen--)
           *retStr++ = *srcPtr++;
	   
	   *retStr = 0;
   }
  
   return bResult;
}

XALAN_CPP_NAMESPACE_END

#endif // defined (XALAN_ICU_MSG_LOADER)


