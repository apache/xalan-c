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
#if !defined(XALAN_ELEMFORWARDCOMPATIBLE_HEADER_GUARD)
#define XALAN_ELEMFORWARDCOMPATIBLE_HEADER_GUARD


// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



// Base class header file.
#include "ElemTemplateElement.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class ElemForwardCompatible : public ElemTemplateElement
{
public:

	/**
	 * Construct an object corresponding to an unknown XSLT element
	 * for forward-compatible processing.
	 * 
	 * @param constructionContext   context for construction of object
	 * @param stylesheetTree        stylesheet containing element
	 * @param name                  name of element
	 * @param atts					the attribute list
	 * @param lineNumber            line number in document
	 * @param columnNumber          column number in document
	 */
	ElemForwardCompatible(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMChar*				name,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber);

    static ElemForwardCompatible*
    create(
            MemoryManagerType&              theManager,
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMChar*				name,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber);
	virtual
	~ElemForwardCompatible();

	// These methods are inherited from ElemTemplateElement ...

	virtual const XalanDOMString&
	getElementName() const;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
	virtual const ElemTemplateElement*
	startElement(StylesheetExecutionContext& executionContext) const;

	virtual bool 
	executeChildElement(
			StylesheetExecutionContext& executionContext,
			const ElemTemplateElement*	element) const;
#else
	virtual void
	execute(StylesheetExecutionContext&		executionContext) const;
#endif

private:

	// not implemented
	ElemForwardCompatible(const ElemForwardCompatible&);

	ElemForwardCompatible&
	operator=(const ElemForwardCompatible&);


	const XalanDOMString&	m_elementName;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMFORWARDCOMPATIBLE_HEADER_GUARD
