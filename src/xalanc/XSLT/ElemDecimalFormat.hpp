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
#if !defined(XALAN_ELEMDECIMALFORMAT_HEADER_GUARD)
#define XALAN_ELEMDECIMALFORMAT_HEADER_GUARD



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



// Base class header file.
#include "ElemTemplateElement.hpp"



#include <xalanc/PlatformSupport/XalanDecimalFormatSymbols.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XPath;



class ElemDecimalFormat : public ElemTemplateElement
{
public:

	/**
	 * Construct an object corresponding to a decimal-format element
	 * 
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param atts                list of attributes for element
	 * @param lineNumber				line number in document
	 * @param columnNumber			column number in document
	 */
	ElemDecimalFormat(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber);


	~ElemDecimalFormat();

	// These methods are inherited from ElemTemplateElement ...

	virtual const XalanDOMString&
	getElementName() const;

	const XalanDecimalFormatSymbols&
	getDecimalFormatSymbols() const
	{
		return m_decimalFormatSymbols;
	}

	const XalanQName&
	getQName() const
	{
		assert(m_qname != 0);

		return *m_qname;
	}

	virtual	const XPath*
	getXPath(unsigned int	index = 0) const;

private:

	const XPath*				m_countMatchPattern;
	const XPath*				m_fromMatchPattern;
	const XPath*				m_valueExpr;

	const XalanQName*			m_qname;

	XalanDecimalFormatSymbols	m_decimalFormatSymbols;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMEMPTY_HEADER_GUARD
