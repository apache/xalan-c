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
#if !defined(XSTRINGREFERENCE_HEADER_GUARD_1357924680)
#define XSTRINGREFERENCE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XStringBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XStringReference : public XStringBase
{
public:

	typedef XStringBase		ParentType;

	/**
	 * Construct an XStringReference object from a string.
	 * The XStringReference will hold a reference to the
	 * supplied string, so the string must be in scope for
	 * the lifetime of the instance
	 * 
	 * @param value      source string
	 */
	XStringReference(const XalanDOMString&	val,
                        MemoryManagerType&  theManager);

	XStringReference(const XStringReference&	source,
                        MemoryManagerType&      theManager);

	virtual
	~XStringReference();


	// These methods are inherited from XObject ...

	virtual const XalanDOMString&
	str() const;
  
	virtual void
	str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::str;
#endif

	virtual double
	stringLength() const;

protected:

	virtual eObjectType
	getRealType() const;

private:
    // Not implemented
    XStringReference(const XStringReference&	source);
    XStringReference();

	const XalanDOMString&	m_value;
};



XALAN_CPP_NAMESPACE_END



#endif	// XSTRINGREFERENCE_HEADER_GUARD_1357924680
