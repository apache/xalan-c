/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(XALAN_ELEMTEXTLITERAL_HEADER_GUARD)
#define XALAN_ELEMTEXTLITERAL_HEADER_GUARD 


// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <XalanDOM/XalanDOMString.hpp>



// Base class header file.
#include "ElemTemplateElement.hpp"



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

	const XalanDOMString::size_type
	getLength() const
	{
		return m_length;
	}

	virtual bool
	isWhitespace() const;

	// These methods are inherited from ElemTemplateElement ...
	
	virtual const XalanDOMString&
	getElementName() const;

	virtual void
	execute(StylesheetExecutionContext&		executionContext) const;

private:

	// not implemented
	ElemTextLiteral(const ElemTextLiteral&);

	ElemTextLiteral&
	operator=(const ElemTextLiteral&);

	const bool	m_isWhitespace;

	const XalanDOMChar* const			m_ch;
	const XalanDOMString::size_type		m_length;
};



#endif	// XALAN_ELEMTEXTLITERAL_HEADER_GUARD
