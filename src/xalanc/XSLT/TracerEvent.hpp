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
#if !defined(XALAN_TracerEvent_HEADER_GUARD)
#define XALAN_TracerEvent_HEADER_GUARD 



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



// $$$ ToDo: This is necessary while XalanDOMString is a typedef.
#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;
class XalanNodeList;
class XalanQName;
class StylesheetExecutionContext;
class ElemTemplateElement;



/**
 * This is the parent class of events generated for tracing the
 * progress of the XSL processor.
 */
class XALAN_XSLT_EXPORT TracerEvent
{
public:

	/**
	 * Construct a tracer event.
	 * 
	 * @param executionContext    XSLT processor instance
	 * @param sourceNode current  context node
	 * @param styleNode           node in the style tree where the event occurs
	 */
	TracerEvent(
			const StylesheetExecutionContext&	executionContext,
			const ElemTemplateElement&			styleNode);

	virtual
	~TracerEvent();

	/**
	 * Returns a string representation of the node. The string returned for
	 * elements will contain the element name and any attributes enclosed in
	 * angle brackets. The string returned for attributes will be of form,
	 * "name=value."
	 *
	 * @param n any DOM node
	 * @return string representation of the given node
	 */
	static XalanDOMString&
	printNode(const XalanNode&	n, XalanDOMString& theResult);

	/**
	 * Returns a string representation of the node list. The string will contain
	 * the list of nodes inside square braces. Elements will contain the element
	 * name and any attributes enclosed in angle brackets. Attributes will be of
	 * form, "name=value."
	 *
	 * @param l any DOM node list
	 * @return string representation of the given node list
	 */
	static XalanDOMString&
	printNodeList(const XalanNodeList&	l, XalanDOMString& theResult);

	/**
	 * The XSLT processor instance.
	 */
	const StylesheetExecutionContext&		m_executionContext;
 
	/**
	 * The node in the style tree where the event occurs.
	 */
	const ElemTemplateElement&				m_styleNode;
  
private:

	// not implemented
	TracerEvent& operator=(const TracerEvent &);

};



XALAN_CPP_NAMESPACE_END



#endif	//XALAN_TracerEvent_HEADER_GUARD
