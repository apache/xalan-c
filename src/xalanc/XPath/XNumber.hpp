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
#if !defined(XNUMBER_HEADER_GUARD_1357924680)
#define XNUMBER_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



// Base class header file.
#include <xalanc/XPath/XNumberBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XNumber : public XNumberBase
{
public:

	/**
	 * Create an XNumber from a number.
	 *
	 * @param val numeric value to use
	 */
	XNumber(double	val,
            MemoryManagerType& theManager);

	XNumber(const XNumber&	source, MemoryManagerType& theManager);

	virtual
	~XNumber();

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

	// These methods are new to XNumber...

	/**
	 * Change the value of an XNumber
	 *
	 * @param theValue The new value.
	 */
	void
	set(double	theValue);

private:
    // not implemented
    XNumber();
    XNumber(const XNumber&);

	// Value of the number being represented.
	double					m_value;

	mutable XalanDOMString	m_cachedStringValue;
};



XALAN_CPP_NAMESPACE_END



#endif	// XNUMBER_HEADER_GUARD_1357924680
