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

#if defined XALAN_ICU_MSG_LOADER


// Class header file.
#include "xalanc/Include/XalanVersion.hpp"

#include <xalanc/PlatformSupport/XalanICUMessageLoader.hpp>
#include <xalanc/PlatformSupport/PlatformSupportInit.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>

#include "unicode/uloc.h"
#include "unicode/udata.h" 

#include <xercesc/util/XMLMsgLoader.hpp> 
#include <xercesc/util/XMLString.hpp>

#define XALAN_MESSAGES_NAME XalanMessages_

#define MAKE_STRING(a) #a
#define INVK_MAKE_STRING(a) MAKE_STRING(a)

#ifdef WIN32

#ifdef  NDEBUG_SYMBOLS // release with symbols

#define ENTRY_POINT_SUFFIX S_dat
#define PACKAGE_NAME_SUFFIX S

#elif NDEBUG // release

#define ENTRY_POINT_SUFFIX _dat
#define PACKAGE_NAME_SUFFIX

#elif _DEBUG // debug only 

#define ENTRY_POINT_SUFFIX D_dat
#define PACKAGE_NAME_SUFFIX D

#endif // NDEBUG_SYMBOLS


#define ICUDLL_ENTRYPOINT_NAME INVK_CAT3_RAW_NUMERIC(XALAN_MESSAGES_NAME,\
									INVK_CAT2_RAW_NUMERIC_SEP_UNDERSCORE(XALAN_VERSION_MAJOR,XALAN_VERSION_MINOR),\
									ENTRY_POINT_SUFFIX)

#define PACKAGE_NAME INVK_CAT3_RAW_NUMERIC(XALAN_MESSAGES_NAME,\
									INVK_CAT2_RAW_NUMERIC_SEP_UNDERSCORE(XALAN_VERSION_MAJOR,XALAN_VERSION_MINOR),\
									PACKAGE_NAME_SUFFIX)

extern "C" const char U_IMPORT  ICUDLL_ENTRYPOINT_NAME [];
 
static const char* const	sPackageName=INVK_MAKE_STRING(PACKAGE_NAME)	;	
												   
#else // NON-WIN32 systems

extern "C" const char U_IMPORT  xalanMsg_dat [];
static const char* const	sPackageName="xalanMsg";

#endif // WIN32



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


static const char* domainName = INVK_MAKE_STRING(XALAN_PRODUCT);



XalanICUMessageLoader::XalanICUMessageLoader():
	m_localeBundle(0),
	m_domainBundle(0),
	m_unknownMessage(XALAN_STATIC_UCODE_STRING("The message was not found in the ICU resource bundle."))
{
 	UErrorCode err = U_ZERO_ERROR;

#ifdef WIN32
	udata_setAppData(sPackageName, &ICUDLL_ENTRYPOINT_NAME, &err);
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

	m_localeBundle = ures_open(sPackageName, szLocal , &err);
	if (!U_SUCCESS(err) || m_localeBundle == 0)
	{
		assert( 0 );
	}

	err = U_ZERO_ERROR;
    m_domainBundle = ures_getByKey(m_localeBundle, domainName, 0, &err);
    if (!U_SUCCESS(err) || m_domainBundle == 0)
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
   const UChar * const	name = ures_getStringByIndex(m_domainBundle, (int32_t)msgToLoad+1, &strLen, &err);
   
   if (!U_SUCCESS(err) || (name == 0))
   {
	   XalanCopy(m_unknownMessage.c_str(), m_unknownMessage.c_str() + m_unknownMessage.length() + 1, toFill);

	   return false;
   }

   const unsigned int	retStrLen = (strLen + 1 ) > maxChars ? maxChars : (strLen + 1);

   XalanCopy(name, name + retStrLen, toFill);
  
   return bResult;
}

XALAN_CPP_NAMESPACE_END

#endif // defined (XALAN_ICU_MSG_LOADER)


