/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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
            const StylesheetExecutionContext&   executionContext,
            const ElemTemplateElement&          styleNode) :
    m_executionContext(executionContext),
    m_styleNode(styleNode)
{
}



TracerEvent::~TracerEvent()
{
}



XalanDOMString&
TracerEvent::printNode(const XalanNode&     n,
                       XalanDOMString& r)
{

    PointerToDOMString(&n, r);

    r.append(" ");

    if (n.getNodeType() == XalanNode::ELEMENT_NODE)
    {
        r += XalanDOMString("<", r.getMemoryManager());
        r += n.getNodeName();

        const XalanNode*    c = n.getFirstChild();

        XalanDOMString theBuffer(r.getMemoryManager());

        while (c != 0)
        {
            if (c->getNodeType() == XalanNode::ATTRIBUTE_NODE)
            {
                r += printNode(*c,theBuffer);
                r += XalanDOMString(" ", r.getMemoryManager());
            }

            c = c->getNextSibling();
        }

        r += XalanDOMString(">", r.getMemoryManager());
    }
    else
    {
        if (n.getNodeType() == XalanNode::ATTRIBUTE_NODE)
        {
            r += n.getNodeName();
            r += XalanDOMString("=", r.getMemoryManager());
            r += n.getNodeValue();
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
XalanDOMString&
TracerEvent::printNodeList(const XalanNodeList& l, XalanDOMString& r)
{

    PointerToDOMString(&l, r);

    
    r.append(XalanDOMString("[", r.getMemoryManager()));

    const XalanSize_t   len = l.getLength();
    XalanSize_t         i = 0;

    XalanDOMString theBuffer(r.getMemoryManager());


    while (i < len)
    {
        XalanNode* const    n = l.item(i);

        if (n != 0)
        {
            r += printNode(*n, theBuffer);

            if (i != len - 1)
            {
                r += XalanDOMString(", ", r.getMemoryManager());
            }
        }

        ++i;
    }

    r +=  XalanDOMString("]", r.getMemoryManager());

    return r;
}



XALAN_CPP_NAMESPACE_END
