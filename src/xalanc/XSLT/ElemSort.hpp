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
#if !defined(XALAN_ELEMSORT_HEADER_GUARD)
#define XALAN_ELEMSORT_HEADER_GUARD 



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



// Base class header file.
#include "AVT.hpp"
#include "ElemTemplateElement.hpp"



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XPath;
class AVT;



class ElemSort: public ElemTemplateElement
{
public:

	/**
	 * Construct an object corresponding to an "xsl:sort" element
	 * 
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param atts                list of attributes for element
	 * @param lineNumber				line number in document
	 * @param columnNumber			column number in document
	 */
	ElemSort(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber);

    static ElemSort*
	create(
            MemoryManagerType& theManager,
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber);

	~ElemSort();

	/**
	 * Retrieve the language attribute value template(AVT) 
	 * 
	 * @return the language AVT
	 */
	const AVT*
	getLangAVT() const
	{
		return m_langAVT;
	}

	/**
	 * Retrieve the order attribute value template(AVT) 
	 * 
	 * @return the order AVT
	 */
	const AVT*
	getOrderAVT() const
	{
		return m_orderAVT;
	}

	/**
	 * Retrieve the case-order attribute value template(AVT) 
	 * 
	 * @return the case-order AVT
	 */
	const AVT*
	getCaseOrderAVT() const
	{
		return m_caseOrderAVT;
	}

	/**
	 * Retrieve the data type attribute value template(AVT) 
	 * 
	 * @return the data type AVT
	 */
	const AVT*
	getDataTypeAVT() const
	{
		return m_dataTypeAVT;
	}

	/**
	 * Retrieve the select pattern
	 * 
	 * @return XPath corresponding to the select pattern
	 */
	const XPath*
	getSelectPattern() const
	{
		return m_selectPattern;
	}

	virtual const XalanDOMString&
	getElementName() const;

	virtual	const XPath*
	getXPath(unsigned int	index = 0) const;

private:

	const XPath*	m_selectPattern;

    const AVT*		m_langAVT;
    const AVT*		m_dataTypeAVT;
	const AVT*		m_orderAVT;
    const AVT*		m_caseOrderAVT;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMSORT_HEADER_GUARD
