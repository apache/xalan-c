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
#include "XSLTDefinitions.hpp"



// Base class header file.
#include "ElemUse.hpp"



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
			const XalanDOMString&			name,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber,
			int								xslToken = Constants::ELEMNAME_LITERALRESULT);

	virtual
	~ElemLiteralResult();

	// These methods are inherited from ElemUse ...
	
	virtual bool
	isAttrOK(
			const XalanDOMChar*				attrName,
			const AttributeList&			atts,
			int								which,
			StylesheetConstructionContext&	constructionContext) const;

	virtual void
	execute(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const;

private:

	// not implemented
	ElemLiteralResult(const ElemLiteralResult &);

	ElemLiteralResult&
	operator=(const ElemLiteralResult&);

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<const AVT*>				AVTVectorType;
	typedef vector<XalanDOMString>			ExtensionElementPrefixesVectorType;
	typedef map<DOMString, DOMString>	String2StringMapType;
#else
	typedef std::vector<const AVT*>			AVTVectorType;
	typedef std::vector<XalanDOMString>		ExtensionElementPrefixesVectorType;
	typedef std::map<DOMString, DOMString>	String2StringMapType;
#endif

	/**
	 * A stack to keep track of the attribute elements.
	 */
	AVTVectorType							m_avts;

	XalanDOMString							m_QName;


	ExtensionElementPrefixesVectorType		m_extensionElementPrefixes;
	
	/**
	 * This is in support of the exclude-result-prefixes attribute.  It is
	 * really needed only at construction time, and so should probably go
	 * somewhere else.
	 */
	String2StringMapType m_excludeResultPrefixes;

	/**
	 * Process the exclude-result-prefixes or the extension-element-prefixes
	 * attributes, for the purpose of prefix exclusion.
	 */
	void
	ElemLiteralResult::processPrefixControl(
			const DOMString&	localName, 
			const DOMString&	attrValue);
		
};



#endif	// XALAN_ELEMLITERALRESULT_HEADER_GUARD
