/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



#include <xalanc/PlatformSupport/AttributeListImpl.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/FormatterListener.hpp>
#include <xalanc/PlatformSupport/NamedNodeMapAttributeList.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FormatterTreeWalker::FormatterTreeWalker(FormatterListener& 	formatterListener) :
	TreeWalker(),
	m_formatterListener(formatterListener)
{
}



FormatterTreeWalker::~FormatterTreeWalker()
{
}



bool
FormatterTreeWalker::startNode(const XalanNode*		node)
{
	assert(node != 0);

	switch(node->getNodeType())
	{
	case XalanNode::COMMENT_NODE:
		{
			m_formatterListener.comment(c_wstr(node->getNodeValue()));
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
			m_formatterListener.processingInstruction(
				c_wstr(node->getNodeName()),
				c_wstr(node->getNodeValue()));
		}
		break;

	case XalanNode::CDATA_SECTION_NODE:
		{
			const XalanDOMString&	data = node->getNodeValue();

			assert(length(data) == FormatterListener::size_type(length(data)));

			m_formatterListener.cdata(c_wstr(data), FormatterListener::size_type(length(data)));
		}
		break;

	case XalanNode::TEXT_NODE:
		{
			const XalanDOMString&	data = node->getNodeValue();

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

	return false;
}



bool
FormatterTreeWalker::startNode(XalanNode*	node)
{
	assert(node != 0);

#if defined(XALAN_OLD_STYLE_CASTS)
	return startNode((const XalanNode*)node);
#else
	return startNode(const_cast<const XalanNode*>(node));
#endif
}



bool
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

	return false;
}



bool
FormatterTreeWalker::endNode(XalanNode*		node)
{
#if defined(XALAN_OLD_STYLE_CASTS)
	return endNode((const XalanNode*)node);
#else
	return endNode(const_cast<const XalanNode*>(node));
#endif
}



XALAN_CPP_NAMESPACE_END
