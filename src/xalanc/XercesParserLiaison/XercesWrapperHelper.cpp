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

#include "XercesWrapperHelper.hpp"



#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMCharacterData.hpp>



#include "XercesDOMWrapperException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



bool
XercesWrapperHelper::isSupported(
			const DOMNodeType*		theXercesNode,
			const XalanDOMString&	feature,
			const XalanDOMString&	version)
{
	assert(theXercesNode != 0);

	return theXercesNode->isSupported(
				XalanDOMStringToXercesDOMString(feature),
				XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString
XercesWrapperHelper::substringData(
			const DOMCharacterDataType*		theXercesNode,
			unsigned int					offset,
			unsigned int					count)
{
	assert(theXercesNode != 0);

	try
	{
		const XMLCh* const	theString = theXercesNode->substringData(offset, count);

		return XalanDOMString(theString, XalanDOMString::length(theString));
	}
	catch(const DOMExceptionType&	theException)
	{
		throw XercesDOMWrapperException(theException);
	}
}



XALAN_CPP_NAMESPACE_END
