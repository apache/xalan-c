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
#if !defined(XALAN_ELEMTEMPLATE_HEADER_GUARD)
#define XALAN_ELEMTEMPLATE_HEADER_GUARD 



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"

// Base class header file.
#include "ElemTemplateElement.hpp"



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XPath;



class ElemTemplate : public ElemTemplateElement
{
public:

    typedef ElemTemplateElement ParentType;

	/**
	 * Construct an object corresponding to an "xsl:template" element
	 * 
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param atts                list of attributes for element
	 * @param lineNumber				line number in document
	 * @param columnNumber			column number in document
	 */
	ElemTemplate(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber);

	virtual
	~ElemTemplate();

	/**
	 * Retrieve the mode of element
	 * 
	 * @return QName for mode
	 */
	const XalanQName&
	getMode() const
	{
		assert(m_mode != 0);

		return *m_mode;
	}

	/**
	 * Retrieve the match pattern
	 * 
	 * @return XPath corresponding to the match pattern
	 */
	const XPath*
	getMatchPattern() const
	{
		return m_matchPattern;
	}

	/**
	 * Retrieve the priority of element
	 * 
	 * @return priority value
	 */
	double
	getPriority() const
	{
		return m_priority;
	}

	virtual const XalanQName&
	getNameAttribute() const;

	virtual void
	addToStylesheet(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						theStylesheet);

	virtual const XalanDOMString&
	getElementName() const;

	// These methods are inherited from ElemTemplateElement ...
#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
	virtual const ElemTemplateElement*
	startElement(StylesheetExecutionContext&	executionContext) const;

	virtual void
	endElement(StylesheetExecutionContext&		executionContext) const;

	virtual const ElemTemplateElement*
	getInvoker(StylesheetExecutionContext&	executionContext) const;
#else
	virtual void
	execute(StylesheetExecutionContext&		executionContext) const;

	virtual void
	executeChildren(StylesheetExecutionContext& 	executionContext) const;

	virtual void
	executeChildren(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						sourceNode) const;
    void
    executeAsNamed(StylesheetExecutionContext&  executionContext) const
    {
	    ParentType::executeChildren(executionContext);
    }
#endif

    virtual	const XPath*
	getXPath(unsigned int	index = 0) const;

protected:

	virtual bool
	childTypeAllowed(int	xslToken) const;

private:

	// not implemented
	ElemTemplate(const ElemTemplate&);

	ElemTemplate&
	operator=(const ElemTemplate&);

	// Data members...
	const XPath*		m_matchPattern;

	const XalanQName*	m_name;
	const XalanQName*	m_mode;

	double				m_priority;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMTEMPLATE_HEADER_GUARD
