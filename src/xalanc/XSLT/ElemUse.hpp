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
#if !defined(XALAN_ELEMUSE_HEADER_GUARD)
#define XALAN_ELEMUSE_HEADER_GUARD 



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



// Base class header file.
#include "ElemTemplateElement.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class ElemUse : public ElemTemplateElement
{
public:

#if defined(XALAN_STRICT_ANSI_HEADERS)
	typedef std::size_t		size_type;
#else
	typedef size_t			size_type;
#endif

	/**
	 * Construct an object corresponding to an "use-attribute-sets" attribute.
	 * This is a base class for "xsl:element," "xsl:copy" and
	 * "xsl:attribute-set" elements, which may specify attribute sets to use. 
	 * 
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param atts                list of attributes for element
	 * @param lineNumber				line number in document
	 * @param columnNumber			column number in document
	 */
	ElemUse(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			int								lineNumber,
			int								columnNumber,
			int								xslToken);

	virtual
	~ElemUse();

	/** 
	 * See if this is a use-attribute-sets attribute, and, if so, process it.
	 * 
	 * @param attrName qualified name of attribute
	 * @param atts     attribute list where the element comes from (not used at 
	 *                 this time)
	 * @param which    index into the attribute list (not used at this time)
	 * @return true if this is a use-attribute-sets attribute
	 */
	bool
	processUseAttributeSets(
			StylesheetConstructionContext&	constructionContext,
			const XalanDOMChar*				attrName,
			const AttributeListType&		atts,
			int								which);

	// These methods are inherited from ElemTemplateElement ...
	
	virtual const XalanDOMString&
	getElementName() const;

	virtual void
	postConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler);

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

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
	/**
	 * Get the next attribute set to execute.
	 * 
	 * @param executionContext	context to execute this element
	 * @returns a pointer to the attribute set element,  0 if no more attribute sets
	 */
	const ElemTemplateElement*
	getNextAttributeSet(
			StylesheetExecutionContext&		executionContext) const;
	
	/**
	 * Evalute the AVTs for this element
	 * 
	 * @param executionContext	context to execute this element
	 */
	virtual void
	evaluateAVTs(
			StylesheetExecutionContext&			executionContext) const;
#else
	/** 
	 * Execute and conditionally apply any attribute sets.  To be used
	 * by deriving classes who want ElemUse to do any default execution
	 * but skip applying attribute sets.  Typically, this would be done
	 * when attempting to recover from an error.
	 *
	 * @param executionContext The current execution context.
	 * @param applyAttributeSets If true, attribute sets will be applied.
	 */
	virtual void
	doExecute(
			StylesheetExecutionContext&		executionContext,
			bool							applyAttributeSets) const;
#endif

private:

	const XalanQName**	m_attributeSetsNames;

	size_type			m_attributeSetsNamesCount;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMUSE_HEADER_GUARD
