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
#if !defined(XSTRING_HEADER_GUARD_1357924680)
#define XSTRING_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XStringBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XString : public XStringBase
{
public:

	typedef XStringBase		ParentType;

	/**
	 * Construct an XString object from a string.
	 * 
	 * @param value      source string
	 */
	XString(const XalanDOMString&	val);

	XString(const XalanDOMChar*		val);

	XString(
			const XalanDOMChar*		val,
			unsigned int			len);

	XString(const XString&	source);

	virtual
	~XString();

	void
	set(const XalanDOMString&	theString)
	{
		m_value = theString;
	}

	// These methods are inherited from XObject ...

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XObject*
#else
	virtual XString*
#endif
	clone(void*		theAddress = 0) const;

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

private:

	XalanDOMString	m_value;
};



XALAN_CPP_NAMESPACE_END



#endif	// XSTRING_HEADER_GUARD_1357924680
