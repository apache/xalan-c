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
// Class header file.
#include "FormatterTreeWalker.hpp"



// Xerces header files...
#include <dom/DOM_Node.hpp>
#include <dom/DOM_Comment.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOM_DocumentFragment.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#include <dom/DOM_ProcessingInstruction.hpp>
#include <dom/DOM_Text.hpp>



// XSL4C header files.
#include <Include/DOMHelper.hpp>



#include <PlatformSupport/AttributeListImpl.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/NamedNodeMapAttributeList.hpp>



#include "FormatterListener.hpp"



FormatterTreeWalker::FormatterTreeWalker(FormatterListener& 	formatterListener) :
	TreeWalker(),
	m_formatterListener(formatterListener)
{
}



FormatterTreeWalker::~FormatterTreeWalker()
{
}



void
FormatterTreeWalker::startNode(const DOM_Node&	node)
{
	switch(node.getNodeType())
	{
	case DOM_Node::COMMENT_NODE:
		{
			const DOM_Comment&	theCommentNode =
				static_cast<const DOM_Comment&>(node);

			m_formatterListener.comment(c_wstr(theCommentNode.getData()));
		}
		break;

	case DOM_Node::DOCUMENT_FRAGMENT_NODE:
		// ??
		break;

	case DOM_Node::DOCUMENT_NODE:
		m_formatterListener.startDocument();
		break;

	case DOM_Node::ELEMENT_NODE:
		{
			const DOM_Element&	theElementNode =
				static_cast<const DOM_Element&>(node);

			DOM_NamedNodeMap	atts = theElementNode.getAttributes();

			NamedNodeMapAttributeList	theAttributeList(atts);

			m_formatterListener.startElement(c_wstr(theElementNode.getNodeName()),
											 theAttributeList);
		}
		break;

	case DOM_Node::PROCESSING_INSTRUCTION_NODE:
		{
			const DOM_ProcessingInstruction&	thePI =
				static_cast<const DOM_ProcessingInstruction&>(node);

			m_formatterListener.processingInstruction(c_wstr(thePI.getNodeName()),
													  c_wstr(thePI.getData()));
		}
		break;

	case DOM_Node::CDATA_SECTION_NODE:
		{
			const DOM_Text&	theTextNode =
				static_cast<const DOM_Text&>(node);

			const DOMString		data = theTextNode.getData();

			m_formatterListener.cdata(c_wstr(data),
									  length(data));
		}
		break;

	case DOM_Node::TEXT_NODE:
		{
			const DOM_Text&	theTextNode =
				static_cast<const DOM_Text&>(node);

			const DOMString		data = theTextNode.getData();

			m_formatterListener.characters(c_wstr(data), length(data));
		}
		break;

	case DOM_Node::ENTITY_REFERENCE_NODE:
		m_formatterListener.entityReference(c_wstr(node.getNodeName()));
		break;

	default:
		// Do nothing...
		break;
	}
}



void
FormatterTreeWalker::endNode(const DOM_Node&		node)
{
	switch(node.getNodeType())
	{
	case DOM_Node::DOCUMENT_NODE:
		m_formatterListener.endDocument();
		break;

	case DOM_Node::ELEMENT_NODE:
		m_formatterListener.endElement(c_wstr(node.getNodeName()));
		break;

	default:
		// Do nothing
		break;
	}
}
