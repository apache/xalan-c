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
#if !defined(XALAN_SelectionEvent_HEADER_GUARD)
#define XALAN_SelectionEvent_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XPath/XObject.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;
class ElemTemplateElement;
class StylesheetExecutionContext;
class XPath;



class XALAN_XSLT_EXPORT SelectionEvent
{
public:

	/**
	 * Create an event originating at the given node of the style tree.
	 *
	 * @param executionContext The current execution context
	 * @param sourceNode The source node selected.
	 * @param styleNode The node in the style tree reference for the event
	 * @param attributeName The attribute name where the XPath expression was supplied
	 * @param xpath The XPath instance executed
	 * @param selection The result of evaluating the XPath
	 *
	 */
	SelectionEvent(
			StylesheetExecutionContext&		executionContext,
			const XalanNode*				sourceNode,
			const ElemTemplateElement&		styleNode,
			const XalanDOMString&			attributeName,
			const XPath&					xpath,
			const XObjectPtr				selection);

	/**
	 * Create an event originating at the given node of the style tree.
	 *
	 * @param executionContext The current execution context
	 * @param sourceNode The source node selected.
	 * @param styleNode The node in the style tree reference for the event
	 * @param attributeName The attribute name where the XPath expression was supplied
	 * @param xpathExpression The XPath expression executed
	 * @param selection The result of evaluating the XPath
	 *
	 */
	SelectionEvent(
			StylesheetExecutionContext&		executionContext,
			const XalanNode*				sourceNode,
			const ElemTemplateElement&		styleNode,
			const XalanDOMString&			attributeName,
			const XalanDOMString&			xpathExpression,
			const XObjectPtr				selection);

	/**
	 * Create an event originating at the given node of the style tree.
	 *
	 * @param executionContext The current execution context
	 * @param sourceNode The source node selected.
	 * @param styleNode The node in the style tree reference for the event
	 * @param attributeName The attribute name where the XPath expression was supplied
	 * @param xpath The XPath instance executed
	 * @param selection The result of evaluating the XPath
	 *
	 */
	SelectionEvent(
			StylesheetExecutionContext&		executionContext,
			const XalanNode*				sourceNode,
			const ElemTemplateElement&		styleNode,
			const XalanDOMString&			attributeName,
			const XPath&					xpath,
			bool							selection);

	/**
	 * Create an event originating at the given node of the style tree.
	 *
	 * @param executionContext The current execution context
	 * @param sourceNode The source node selected.
	 * @param styleNode The node in the style tree reference for the event
	 * @param attributeName The attribute name where the XPath expression was supplied
	 * @param xpath The XPath instance executed
	 * @param selection The result of evaluating the XPath
	 *
	 */
	SelectionEvent(
			StylesheetExecutionContext&		executionContext,
			const XalanNode*				sourceNode,
			const ElemTemplateElement&		styleNode,
			const XalanDOMString&			attributeName,
			const XPath&					xpath,
			const NodeRefListBase&			selection);

	virtual
	~SelectionEvent();

	enum eSelectionType { eNone, eBoolean, eNodeSet, eUnknown };

	/**
	 * The executionContext instance.
	 */
	const StylesheetExecutionContext&	m_executionContext;

	/**
	 * The current context node.
	 */
	const XalanNode*					m_sourceNode;
  
	/**
	 * The node in the style tree where the event occurs.
	 */
	const ElemTemplateElement&			m_styleNode;

	/**
	 * The attribute name from which the selection is made.
	 */
	const XalanDOMString&				m_attributeName;

	/**
	 * The XPath expression evaluated.
	 */
	const XalanDOMString&				m_xpathExpression;
  
	/**
	 * The result of the selection.  If it's null, m_sourceNode
	 * was selected, or some specific type was selected.  See
	 * the above enums.
	 */
	const XObjectPtr					m_selection;

	/**
	 * The type of the selection.
	 */
	const eSelectionType				m_type;

	bool								m_boolean;

	const NodeRefListBase* const		m_nodeList;

private:

	// Unimplemented...
	SelectionEvent&
	operator=(const SelectionEvent&	other);
};



XALAN_CPP_NAMESPACE_END



#endif	//XALAN_SelectionEvent_HEADER_GUARD
