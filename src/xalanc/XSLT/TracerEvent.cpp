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
