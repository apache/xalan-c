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
#if !defined(XALAN_ELEMAPPLYTEMPLATES_HEADER_GUARD)
#define XALAN_ELEMAPPLYTEMPLATES_HEADER_GUARD 

// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



// Base class header file.
#include <xalanc/XSLT/ElemForEach.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanQName;



class ElemApplyTemplates : public ElemForEach
{
public:

	typedef ElemForEach		ParentType;

	/**
	 * Construct an object corresponding to an "xsl:apply-template" element
	 * 
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param atts                list of attributes for element
	 * @param lineNumber				line number in document
	 * @param columnNumber			column number in document
	 */
	ElemApplyTemplates(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber, 
			int								columnNumber);

    virtual
    ~ElemApplyTemplates();

	// These methods are inherited from ElemTemplateElement ...
	
	virtual const XalanDOMString&
	getElementName() const;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
	virtual const ElemTemplateElement*
	startElement(StylesheetExecutionContext&	executionContext) const;

	virtual void
	endElement(StylesheetExecutionContext&		executionContext) const;

	virtual const ElemTemplateElement*
	getNextChildElemToExecute(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement*		currentElem) const;

	virtual const ElemTemplateElement*
	getFirstChildElemToExecute(
			StylesheetExecutionContext&		executionContext) const;
#else
	virtual void
	execute(StylesheetExecutionContext&		executionContext) const;
#endif



protected:

	virtual bool
	childTypeAllowed(int	xslToken) const;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
	const ElemTemplateElement*
	findNextTemplateToExecute(StylesheetExecutionContext& executionContext) const;
#else
	virtual void
	transformChild(
			StylesheetExecutionContext& 	executionContext,
			const ElemTemplateElement&		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						child) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::transformChild;
#endif

	virtual void
	selectAndSortChildren(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement*		theTemplate,
			NodeSorter*						sorter,
			int								selectStackFrameIndex) const;
#endif

private:

	const XalanQName*	m_mode;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMAPPLYTEMPLATES_HEADER_GUARD
