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
#if !defined(XTOKENSTRINGADAPTER_HEADER_GUARD_1357924680)
#define XTOKENSTRINGADAPTER_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



// Base class header file.
#include <xalanc/XPath/XStringBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XToken;



class XALAN_XPATH_EXPORT XTokenStringAdapter : public XStringBase
{
public:

	/**
	 * Create an XTokenStringAdapter from an XToken.
	 *
	 * @param theXToken The XToken instance to adapt
	 */
	XTokenStringAdapter(const XToken&	    theToken,
                        MemoryManagerType& theManager);

	XTokenStringAdapter(const XTokenStringAdapter&	source,
                        MemoryManagerType&          theManager);

	virtual
	~XTokenStringAdapter();

	// These methods are inherited from XObject ...

	virtual double
	num() const;

	virtual const XalanDOMString&
	str() const;

	virtual void
	str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const;

	virtual void
	str(XalanDOMString&	theBuffer) const;

	virtual double
	stringLength() const;

protected:

	virtual eObjectType
	getRealType() const;

private:
    // Not implemented
    XTokenStringAdapter(const XTokenStringAdapter&	source);
    XTokenStringAdapter();

	// XToken instance that we're adapting...
	const XToken&	m_value;
};



XALAN_CPP_NAMESPACE_END



#endif	// XTOKENSTRINGADAPTER_HEADER_GUARD_1357924680
