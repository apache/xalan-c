/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanComment.hpp>
#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanDocumentFragment.hpp>
#include <XalanDOM/XalanElement.hpp>
#include <XalanDOM/XalanNamedNodeMap.hpp>
#include <XalanDOM/XalanProcessingInstruction.hpp>
#include <XalanDOM/XalanText.hpp>



#include <PlatformSupport/AttributeListImpl.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/FormatterListener.hpp>
#include <PlatformSupport/NamedNodeMapAttributeList.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FormatterTreeWalker::FormatterTreeWalker(FormatterListener& 	formatterListener) :
	TreeWalker(),
	m_formatterListener(formatterListener)
{
}



FormatterTreeWalker::~FormatterTreeWalker()
{
}



void
FormatterTreeWalker::startNode(const XalanNode*		node)
{
	assert(node != 0);

	switch(node->getNodeType())
	{
	case XalanNode::COMMENT_NODE:
		{
			const XalanComment*	theCommentNode =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanComment*)node;
#else
				static_cast<const XalanComment*>(node);
#endif

			m_formatterListener.comment(c_wstr(theCommentNode->getData()));
		}
		break;

	case XalanNode::DOCUMENT_FRAGMENT_NODE:
		// ??
		break;

	case XalanNode::DOCUMENT_NODE:
		m_formatterListener.startDocument();
		break;

	case XalanNode::ELEMENT_NODE:
		{
			const XalanElement*	theElementNode =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanElement*)node;
#else
				static_cast<const XalanElement*>(node);
#endif

			const XalanNamedNodeMap*	atts = theElementNode->getAttributes();
			assert(atts != 0);

			NamedNodeMapAttributeList	theAttributeList(*atts);

			m_formatterListener.startElement(c_wstr(theElementNode->getNodeName()),
											 theAttributeList);
		}
		break;

	case XalanNode::PROCESSING_INSTRUCTION_NODE:
		{
			const XalanProcessingInstruction*	thePI =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanProcessingInstruction*)node;
#else
				static_cast<const XalanProcessingInstruction*>(node);
#endif

			m_formatterListener.processingInstruction(c_wstr(thePI->getNodeName()),
													  c_wstr(thePI->getData()));
		}
		break;

	case XalanNode::CDATA_SECTION_NODE:
		{
			const XalanText*	theTextNode =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanText*)node;
#else
				static_cast<const XalanText*>(node);
#endif

			const XalanDOMString&	data = theTextNode->getData();

			assert(length(data) == FormatterListener::size_type(length(data)));

			m_formatterListener.cdata(c_wstr(data), FormatterListener::size_type(length(data)));
		}
		break;

	case XalanNode::TEXT_NODE:
		{
			const XalanText*	theTextNode =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanText*)node;
#else
				static_cast<const XalanText*>(node);
#endif

			const XalanDOMString&	data = theTextNode->getData();

			assert(length(data) == FormatterListener::size_type(length(data)));

			m_formatterListener.characters(c_wstr(data), FormatterListener::size_type(length(data)));
		}
		break;

	case XalanNode::ENTITY_REFERENCE_NODE:
		m_formatterListener.entityReference(c_wstr(node->getNodeName()));
		break;

	default:
		// Do nothing...
		break;
	}
}



void
FormatterTreeWalker::startNode(XalanNode*	node)
{
	assert(node != 0);

#if defined(XALAN_OLD_STYLE_CASTS)
	startNode((const XalanNode*)node);
#else
	startNode(const_cast<const XalanNode*>(node));
#endif
}



void
FormatterTreeWalker::endNode(const XalanNode*	node)
{
	assert(node != 0);

	switch(node->getNodeType())
	{
	case XalanNode::DOCUMENT_NODE:
		m_formatterListener.endDocument();
		break;

	case XalanNode::ELEMENT_NODE:
		m_formatterListener.endElement(c_wstr(node->getNodeName()));
		break;

	default:
		// Do nothing
		break;
	}
}



void
FormatterTreeWalker::endNode(XalanNode*		node)
{
#if defined(XALAN_OLD_STYLE_CASTS)
	endNode((const XalanNode*)node);
#else
	endNode(const_cast<const XalanNode*>(node));
#endif
}



XALAN_CPP_NAMESPACE_END
