/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
#include "TracerEvent.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNodeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



TracerEvent::TracerEvent(
			const StylesheetExecutionContext&	executionContext,
			const ElemTemplateElement&			styleNode) :
	m_executionContext(executionContext),
	m_styleNode(styleNode)
{
}



TracerEvent::~TracerEvent()
{
}



XalanDOMString
TracerEvent::printNode(const XalanNode&		n)
{
	XalanDOMString	r;

	PointerToDOMString(&n, r);

	append(r, XALAN_STATIC_UCODE_STRING(" "));

	if (n.getNodeType() == XalanNode::ELEMENT_NODE)
	{
		r += XALAN_STATIC_UCODE_STRING("<") + n.getNodeName();

		const XalanNode*	c = n.getFirstChild();

		while (c != 0)
		{
			if (c->getNodeType() == XalanNode::ATTRIBUTE_NODE)
			{
				r += printNode(*c) + XALAN_STATIC_UCODE_STRING(" ");
			}

			c = c->getNextSibling();
		}

		r += XALAN_STATIC_UCODE_STRING(">");
	}
	else
	{
		if (n.getNodeType() == XalanNode::ATTRIBUTE_NODE)
		{
			r += n.getNodeName() + XALAN_STATIC_UCODE_STRING("=") + n.getNodeValue();
		}
		else
		{
			r += n.getNodeName();
		}
	}
	
	return r;
}

  /**
   Returns a string representation of the node list.
   The string will contain the list of nodes inside square braces.
   Elements will contain the element name
   and any attributes enclosed in angle brackets.
   Attributes will be of form, "name=value."

   @param l any DOM node list. Must not be null.

   @return a string representation of the given node list.
   */
XalanDOMString
TracerEvent::printNodeList(const XalanNodeList&	l)
{
	XalanDOMString	r;

	PointerToDOMString(&l, r);

	append(r, XALAN_STATIC_UCODE_STRING("["));

	unsigned int	len = l.getLength();
	unsigned int	i = 0;

	while (i < len)
	{
		XalanNode* const	n = l.item(i);

		if (n != 0)
		{
			r += printNode(*n);

			if (i != len - 1)
			{
				r += XALAN_STATIC_UCODE_STRING(", ");
			}
		}

		++i;
	}

	return r + XALAN_STATIC_UCODE_STRING("]");
}



XALAN_CPP_NAMESPACE_END
