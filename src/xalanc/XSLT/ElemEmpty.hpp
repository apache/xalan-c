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
#if !defined(XALAN_ELEMEMPTY_HEADER_GUARD)
#define XALAN_ELEMEMPTY_HEADER_GUARD 

/**
 * Simple empty elem to push on the stack when nothing 
 * else got pushed, so that pop() works correctly.
 */

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



// Base class header file.
#include "ElemTemplateElement.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class ElemEmpty : public ElemTemplateElement
{
public:

	/**
	 * Construct an object corresponding to an empty element
	 * 
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param lineNumber				line number in document
	 * @param columnNumber			column number in document
	 * @param elementName The element name for which this instance is representing
	 */
	ElemEmpty(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			int								lineNumber,
			int								columnNumber,
			const XalanDOMString*			elementName = 0);

	/**
	 * Construct an object corresponding to an empty element.  This
	 * constructor is used for surrogate empty elements that do not
	 * need namespace contexts and Locator information, etc.
	 *
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param elementName The element name for which this instance is representing
	 */
	ElemEmpty(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMString*			elementName = 0);

	virtual
	~ElemEmpty();

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

protected:

	virtual bool
	childTypeAllowed(int	xslToken) const;

private:

	const XalanDOMString* const		m_elementName;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMEMPTY_HEADER_GUARD
