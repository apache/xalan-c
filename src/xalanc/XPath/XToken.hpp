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
#if !defined(XTOKEN_HEADER_GUARD_1357924680)
#define XTOKEN_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class...
#include <xalanc/XPath/XObject.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XToken : public XObject
{
public:

	XToken();

	/**
	 * Create an XToken for string in the token queue.
	 *
	 * @param theString The string data for the token.  The instance will keep a point to this string, so it must be persistent.
	 */
	explicit
	XToken(const XalanDOMString&	theString,
            MemoryManagerType&      theManager);

	/**
	 * Create an XToken for number in the token queue.
	 *
	 * @param theNumber The numeric data for the token.  This must be consistent with the lexical value in theString.
	 * @param theString The string data for the token.  The instance will keep a point to this string, so it must be persistent.
	 */
	XToken(
			double					theNumber,
			const XalanDOMString&	theString);

	XToken(const XToken&	theSource);

	virtual
	~XToken();

	virtual const XalanDOMString&
	getTypeString() const;

	virtual double
	num() const;

	virtual bool
	boolean() const;

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

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject);

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject) const;

	XToken&
	operator=(const XToken&		theRHS)
	{
		m_stringValue = theRHS.m_stringValue;

		m_numberValue = theRHS.m_numberValue;

		return *this;
	}

	/**
	 * Set the instance as a string in the token queue.
	 *
	 * @param theString The string data for the token.  XToken will keep a point to this string, so it must be persistent.
	 */
	void
	set(const XalanDOMString&	theString,
        MemoryManagerType& theManager);

	/**
	 * Set the instance as a number in the token queue.
	 *
	 * @param theNumber The numeric data for the token.  This must be consistent with the lexical value in theString.
	 * @param theString The string data for the token.  XToken will keep a point to this string, so it must be persistent.
	 */
	void
	set(
			double					theNumber,
			const XalanDOMString&	theString,
            MemoryManagerType&      theManager);

protected:

	virtual void 
	referenced();

	virtual void 
	dereferenced();

private:

	// Not defined...
	bool
	operator==(const XToken&) const;

	// Data members...
	const XalanDOMString*	m_stringValue;

	double					m_numberValue;

	bool					m_isString;
};



XALAN_CPP_NAMESPACE_END



#endif	// XTOKEN_HEADER_GUARD_1357924680
