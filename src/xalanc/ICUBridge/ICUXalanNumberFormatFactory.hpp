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
#if !defined(ICUXALANNUMBERFORMATFACTORY_HEADER_GUARD_1357924680)
#define ICUXALANNUMBERFORMATFACTORY_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/ICUBridge/ICUBridgeDefinitions.hpp>



#include <xalanc/XSLT/StylesheetExecutionContextDefault.hpp>



XALAN_CPP_NAMESPACE_BEGIN



// Class that implements the XSLT function format-number using the ICU.
//
class XALAN_ICUBRIDGE_EXPORT ICUXalanNumberFormatFactory : public StylesheetExecutionContextDefault::XalanNumberFormatFactory
{
public:

	ICUXalanNumberFormatFactory(MemoryManagerType& theManager);

	virtual
	~ICUXalanNumberFormatFactory();

	virtual XalanNumberFormat*
	create();
private:
    MemoryManagerType& m_memoryManager;
};



XALAN_CPP_NAMESPACE_END



#endif	// ICUXALANNUMBERFORMATFACTORY_HEADER_GUARD_1357924680
