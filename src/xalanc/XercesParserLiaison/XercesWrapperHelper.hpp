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
#if !defined(XERCESWRAPPERHELPER_HEADER_GUARD_1357924680)
#define XERCESWRAPPERHELPER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <cassert>



#include <xercesc/util/XercesDefs.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XERCESPARSERLIAISON_EXPORT XercesWrapperHelper
{
public:

	typedef XMLSize_t	XercesStringLengthType;

	static const XMLCh*
	XalanDOMStringToXercesDOMString(const XalanDOMString&	theString)
	{
		assert(XercesStringLengthType(theString.length()) == theString.length());

		return theString.c_str();
	}

	static bool
	isSupported(
			const DOMNodeType*		theXercesNode,
			const XalanDOMString&	feature,
			const XalanDOMString&	version);

	static const XalanDOMString&
	substringData(
			const DOMCharacterDataType*		theXercesNode,
			unsigned int					offset, 
			unsigned int					count,
            XalanDOMString&                 theResult);

private:

	// Not implemented...
	XercesWrapperHelper();

	XercesWrapperHelper(const XercesWrapperHelper&);
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XERCESWRAPPERHELPER_HEADER_GUARD_1357924680)
