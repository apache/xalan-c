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
#if !defined(XALAN_ELEMWITHPARAM_HEADER_GUARD)
#define XALAN_ELEMWITHPARAM_HEADER_GUARD 



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



// Base class header file.
#include "ElemTemplateElement.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class XPath;



class ElemWithParam : public ElemTemplateElement
{
public:

	/**
	 * Construct an object corresponding to an "xsl:with-param" element
	 * 
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param atts                list of attributes for element
	 * @param lineNumber				line number in document
	 * @param columnNumber			column number in document
	 */
	ElemWithParam(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber);

	virtual
	~ElemWithParam();

	/**
	 * Retrieves qualified name of object
	 *
	 * @return qualified name
	 */
	const XalanQName&
	getQName() const
	{
		assert(m_qname != 0);

		return *m_qname;
	}

	virtual const XalanDOMString&
	getElementName() const;

	virtual	const XPath*
	getXPath(unsigned int	index = 0) const;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
	virtual const ElemTemplateElement* 
	startElement(StylesheetExecutionContext&	executionContext) const;

	virtual void
	endElement(StylesheetExecutionContext&		executionContext) const;
#endif


private:

	// not implemented
	ElemWithParam(const ElemWithParam&);

	ElemWithParam&
	operator=(const ElemWithParam&);

	// Data members...
	const XPath*		m_selectPattern;

	const XalanQName*	m_qname;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMWITHPARAM_HEADER_GUARD
