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
#if !defined(XALANCOLLATIONSERVICES_HEADER_GUARD_1357924680)
#define XALANCOLLATIONSERVICES_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT XalanCollationServices
{
public:

	enum eCaseOrder { eDefault, eLowerFirst, eUpperFirst };

	class XALAN_PLATFORMSUPPORT_EXPORT CollationCompareFunctor
	{
	public:

		CollationCompareFunctor()
		{
		}

		virtual
		~CollationCompareFunctor()
		{
		}

		// Const version is suitable for use by
		// multiple threads.
		virtual int
		operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			eCaseOrder				theCaseOrder = eDefault) const = 0;

		// Const version is suitable for use by
		// multiple threads.
		virtual int
		operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			const XalanDOMChar*		theLocale,
			eCaseOrder				theCaseOrder = eDefault) const = 0;
	};

};



XALAN_CPP_NAMESPACE_END



#endif	// XALANCOLLATIONSERVICES_HEADER_GUARD_1357924680
