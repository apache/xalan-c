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
#if !defined(XALAN_ELEMTEXTLITERAL_HEADER_GUARD)
#define XALAN_ELEMTEXTLITERAL_HEADER_GUARD 


// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <xalanc/XalanDOM/XalanDOMString.hpp>



// Base class header file.
#include "ElemTemplateElement.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class ElemTextLiteral : public ElemTemplateElement
{
public:

	/**
	 * Construct an object corresponding to literal text element
	 * 
	 * @param constructionContext   context for construction of object
	 * @param stylesheetTree        stylesheet containing element
	 * @param lineNumber            line number in document
	 * @param columnNumber          column number in document
	 * @param ch                    pointer to character string for element
	 * @param start                 starting offset of element
	 * @param length                number of characters in element
	 * @param fPreserveSpace         true is space should be preserved
	 * @param fDisableOutputEscaping true if output escaping should be disabled
	 */
	ElemTextLiteral(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			int								lineNumber,
			int								columnNumber,
            const XalanDOMChar*				ch,
			XalanDOMString::size_type		start,
			XalanDOMString::size_type		length,
			bool							fPreserveSpace,
            bool							fDisableOutputEscaping);

	virtual
	~ElemTextLiteral();

	/**
	 * Determine if whitespace should be preserved within the element
	 * 
	 * @return true if whitespace should be preserved
	 */
	bool
	isPreserveSpace() const
	{
		return preserveSpace();
	}

	const XalanDOMChar*
	getText() const
	{
		return m_ch;
	}

	XalanDOMString::size_type
	getLength() const
	{
		return m_length;
	}

	virtual bool
	isWhitespace() const;

	// These methods are inherited from ElemTemplateElement ...
	
	virtual const XalanDOMString&
	getElementName() const;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
	virtual const ElemTemplateElement*
	startElement(StylesheetExecutionContext&	executionContext) const;
#else
	virtual void
	execute(StylesheetExecutionContext&		executionContext) const;
#endif

private:

	// not implemented
	ElemTextLiteral(const ElemTextLiteral&);

	ElemTextLiteral&
	operator=(const ElemTextLiteral&);

	const bool	m_isWhitespace;

	const XalanDOMChar* const			m_ch;
	const XalanDOMString::size_type		m_length;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMTEXTLITERAL_HEADER_GUARD
