/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(XALAN_TracerEvent_HEADER_GUARD)
#define XALAN_TracerEvent_HEADER_GUARD 



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



// $$$ ToDo: This is necessary while XalanDOMString is a typedef.
#include <XalanDOM/XalanDOMString.hpp>



class XalanNode;
class XalanNodeList;
class QName;
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
	 * @param sourceTree input    source tree
	 * @param sourceNode current  context node
	 * @param mode                current mode
	 * @param styleNode           node in the style tree where the event occurs
	 */
	TracerEvent(
			const StylesheetExecutionContext&	executionContext,
			const XalanNode*					sourceTree,
			const XalanNode*					sourceNode,
			const QName&						mode,
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
	static XalanDOMString
	printNode(const XalanNode&	n);

	/**
	 * Returns a string representation of the node list. The string will contain
	 * the list of nodes inside square braces. Elements will contain the element
	 * name and any attributes enclosed in angle brackets. Attributes will be of
	 * form, "name=value."
	 *
	 * @param l any DOM node list
	 * @return string representation of the given node list
	 */
	static XalanDOMString
	printNodeList(const XalanNodeList&	l);

	/**
	 * The XSLT processor instance.
	 */
	const StylesheetExecutionContext&		m_executionContext;

	/**
	 * The input source tree.
	 */
	const XalanNode*						m_sourceTree;
  
	/**
	 * The current context node.
	 */
	const XalanNode*						m_sourceNode;
  
	/**
	 * The current mode.
	 */
	const QName&							m_mode;

	/**
	 * The node in the style tree where the event occurs.
	 */
	const ElemTemplateElement&				m_styleNode;
  
private:

	// not implemented
	TracerEvent& operator=(const TracerEvent &);

};


#endif	//XALAN_TracerEvent_HEADER_GUARD
