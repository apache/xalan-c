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


#if defined (XALAN_INMEM_MSG_LOADER)

// Class header file.
#include "XalanInMemoryMessageLoader.hpp"

#include "PlatformSupportInit.hpp"

#include "DOMStringHelper.hpp"

#include <xalanc/Utils/XalanMsgLib/XalanMsgLib.hpp>




XALAN_CPP_NAMESPACE_BEGIN



bool
XalanInMemoryMessageLoader::loadMsg(
					 XalanMessages::Codes	msgToLoad
					 , XalanDOMChar*		toFill
					 , unsigned int         maxChars )
{
	if ( toFill == 0 )
	{
		return false;
	}
	else
	{
		if (XalanMsgContainer::getNumbOfMsgs() < (unsigned int)msgToLoad)
		{
			const XalanDOMString::size_type		msgLength =
                m_unknownMessage.length() + 1;

			XalanCopy(
                m_unknownMessage.c_str(),
                m_unknownMessage.c_str() + (msgLength < maxChars ? msgLength : maxChars),
                toFill);
		}
		else
		{
			const XalanDOMChar* const	pErrMsg = XalanMsgContainer::getMessage(msgToLoad);
			assert (pErrMsg != 0);

			const XalanDOMString::size_type		msgLength =
                XalanDOMString::length(pErrMsg) + 1;

            XalanCopy(
                pErrMsg,
                pErrMsg + (msgLength < maxChars ? msgLength : maxChars),
                toFill);
		}

		return true;
	}
}



XalanInMemoryMessageLoader::~XalanInMemoryMessageLoader()
{
}



XalanInMemoryMessageLoader::XalanInMemoryMessageLoader() :
	m_unknownMessage(XALAN_STATIC_UCODE_STRING("The message was not found in the message library."))
{
}


XALAN_CPP_NAMESPACE_END

#endif
