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
#if !defined(XALAN_ELEMLITERALRESULT_HEADER_GUARD)
#define XALAN_ELEMLITERALRESULT_HEADER_GUARD 



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



// Base class header file.
#include <XSLT/ElemUse.hpp>



class AVT;



class ElemLiteralResult: public ElemUse
{
public:

	/**
	 * Construct an object corresponding to a literal result element
	 * 
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param name                name of element
	 * @param atts                list of attributes for element
	 * @param lineNumber				line number in document
	 * @param columnNumber			column number in document
	 */
	ElemLiteralResult(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMChar*				name,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber,
			int								xslToken = Constants::ELEMNAME_LITERALRESULT);

	virtual
	~ElemLiteralResult();

	// These methods are inherited from ElemUse ...

	virtual const XalanDOMString&
	getElementName() const;

	virtual void
	postConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler);

	virtual bool
	isAttrOK(
			int						tok,
			const XalanDOMChar*		attrName,
			const AttributeList&	atts,
			int						which) const;

	virtual bool
	isAttrOK(
			const XalanDOMChar*				attrName,
			const AttributeList&			atts,
			int								which,
			StylesheetConstructionContext&	constructionContext) const;

	virtual void
	execute(StylesheetExecutionContext&		executionContext) const;

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<const AVT*>				AVTVectorType;
#else
	typedef std::vector<const AVT*>			AVTVectorType;
#endif

private:

	// not implemented
	ElemLiteralResult(const ElemLiteralResult &);

	ElemLiteralResult&
	operator=(const ElemLiteralResult&);

	/**
	 * Process the exclude-result-prefixes or the extension-element-prefixes
	 * attributes, for the purpose of prefix exclusion.
	 *
	 * @param constructionContext  context when object consructed
	 * @param stylesheetTree The current Stylesheet object.
	 * @param localName The local name of the attribute.
	 * @param attrValue The value of the attribute.
	 */
	bool
	processPrefixControl(
			StylesheetConstructionContext&	constructionContext,
			const Stylesheet&				stylesheetTree,
			const XalanDOMString&			localName,
			const XalanDOMChar*				attrValue);

	/**
	 * Determine if the namespace node should be excluded.
	 *
	 * @param theURI The namespace URI.
	 */
	bool
	shouldExcludeResultNamespaceNode(const XalanDOMString&	theURI) const;

	/**
	 * Add a result attribute, if necessary.
	 *
	 * @param executionContext  The current execution context
	 * @param thePrefix The prefix of the attribute
	 * @param theName The name of the attribute.
	 * @param theValue The value of the attribute.
	 */
	void
	doAddResultAttribute(
			StylesheetExecutionContext&		executionContext,
			const XalanDOMString&			thePrefix,
			const XalanDOMString&			theName,
			const XalanDOMString&			theValue) const;

	/**
	 * The name of the literal result element.
	 */
	const XalanDOMString	m_elementName;

	/**
	 * A vector to keep track of the attribute elements.
	 */
	AVTVectorType			m_avts;
};



#endif	// XALAN_ELEMLITERALRESULT_HEADER_GUARD
