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
#if !defined(XALAN_ELEMLITERALRESULT_HEADER_GUARD)
#define XALAN_ELEMLITERALRESULT_HEADER_GUARD 



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



// Base class header file.
#include <xalanc/XSLT/ElemUse.hpp>



XALAN_CPP_NAMESPACE_BEGIN



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
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber);

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
			const XalanDOMChar*				attrName,
			const AttributeListType&		atts,
			int								which,
			StylesheetConstructionContext&	constructionContext) const;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
	virtual const ElemTemplateElement*
	startElement(StylesheetExecutionContext&		executionContext) const;

	virtual void
	endElement(StylesheetExecutionContext&		executionContext) const;

	virtual void
	evaluateAVTs(StylesheetExecutionContext&	executionContext) const;
#else
	virtual void
	execute(StylesheetExecutionContext&		executionContext) const;
#endif

protected:

	/**
	 * Do common initialization.
	 * 
	 * @param constructionContext The current onstruction context.
	 * @param stylesheetTree The stylesheet containing the element.
	 * @param name The name of the element.
	 * @param atts The list of attributes for the element.
	 */
	void
	init(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMChar*				name,
			const AttributeListType&		atts);

	/**
	 * Construct an object corresponding to a literal result element
	 * 
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param name                name of element
	 * @param atts                list of attributes for element
	 * @param lineNumber				line number in document
	 * @param columnNumber			column number in document
	 * @param xslToken			The xsl token value for this class
	 */
	ElemLiteralResult(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMChar*				name,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber,
			int								xslToken);

	virtual void
	namespacesPostConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler,
			NamespacesHandler&				theHandler);

private:

	// not implemented
	ElemLiteralResult(const ElemLiteralResult &);

	ElemLiteralResult&
	operator=(const ElemLiteralResult&);


	/**
	 * The name of the literal result element.
	 */
	const XalanDOMString&	m_elementName;

	/**
	 * A vector to keep track of the attribute elements.
	 */
	const AVT**				m_avts;

	/**
	 * The size of m_avts, once the stylesheet is compiled...
	 */
	unsigned int			m_avtsCount;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMLITERALRESULT_HEADER_GUARD
