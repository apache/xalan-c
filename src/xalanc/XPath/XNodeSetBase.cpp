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
// Class header file.
#include "XNodeSetBase.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "FormatterStringLengthCounter.hpp"
#include "XObjectTypeCallback.hpp"
#include "XPathExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const double    theBogusNumberValue = 123456789;



XNodeSetBase::XNodeSetBase(MemoryManager&   theMemoryManager) :
    XObject(eTypeNodeSet, theMemoryManager),
    m_proxy(*this),
    m_cachedStringValue(theMemoryManager),
    m_cachedNumberValue(theBogusNumberValue)
{
}



XNodeSetBase::XNodeSetBase(
            const XNodeSetBase&     source,
            MemoryManager&          theMemoryManager) :
    XObject(source, theMemoryManager),
    m_proxy(*this),
    m_cachedStringValue(source.m_cachedStringValue, theMemoryManager),
    m_cachedNumberValue(source.m_cachedNumberValue)
{
}



XNodeSetBase::~XNodeSetBase()
{
}



const XalanDOMString&
XNodeSetBase::getTypeString() const
{
    return s_nodesetString;
}



double
XNodeSetBase::num(XPathExecutionContext&    executionContext) const
{
    if (DoubleSupport::equal(m_cachedNumberValue, theBogusNumberValue) == true)
    {
        m_cachedNumberValue =
            DoubleSupport::toDouble(
                str(executionContext),
                getMemoryManager());
    }

    return m_cachedNumberValue;
}



bool
XNodeSetBase::boolean(XPathExecutionContext&    /* executionContext */) const
{
    return getLength() > 0 ? true : false;
}



const XalanDOMString&
XNodeSetBase::str(XPathExecutionContext&    executionContext) const
{
    if (m_cachedStringValue.empty() == true &&
        getLength() > 0)
    {
        const XalanNode* const  theNode = item(0);
        assert(theNode != 0);

        DOMServices::getNodeData(
            *theNode,
            executionContext,
            m_cachedStringValue);
    }

    return m_cachedStringValue;
}



const XalanDOMString&
XNodeSetBase::str() const
{
    if (m_cachedStringValue.empty() == true &&
        getLength() > 0)
    {
        const XalanNode* const  theNode = item(0);
        assert(theNode != 0);

        DOMServices::getNodeData(
            *theNode,
            m_cachedStringValue);
    }

    return m_cachedStringValue;
}



void
XNodeSetBase::str(
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    if (m_cachedStringValue.empty() == false)
    {
        XObject::string(m_cachedStringValue, formatterListener, function);
    }
    else if (getLength() > 0)
    {
        const XalanNode* const  theNode = item(0);
        assert(theNode != 0);

        DOMServices::getNodeData(
            *theNode,
            executionContext,
            formatterListener,
            function);
    }
}



void
XNodeSetBase::str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    if (m_cachedStringValue.empty() == false)
    {
        XObject::string(m_cachedStringValue, formatterListener, function);
    }
    else if (getLength() > 0)
    {
        const XalanNode* const  theNode = item(0);
        assert(theNode != 0);

        DOMServices::getNodeData(
            *theNode,
            formatterListener,
            function);
    }
}



void
XNodeSetBase::str(
            XPathExecutionContext&  executionContext,
            XalanDOMString&         theBuffer) const
{
    if (m_cachedStringValue.empty() == false)
    {
        theBuffer.append(m_cachedStringValue);
    }
    else if (getLength() > 0)
    {
        const XalanNode* const  theNode = item(0);
        assert(theNode != 0);

        DOMServices::getNodeData(
            *theNode,
            executionContext,
            theBuffer);
    }
}



void
XNodeSetBase::str(XalanDOMString&   theBuffer) const
{
    if (m_cachedStringValue.empty() == false)
    {
        theBuffer.append(m_cachedStringValue);
    }
    else if (getLength() > 0)
    {
        const XalanNode* const  theNode = item(0);
        assert(theNode != 0);

        DOMServices::getNodeData(
            *theNode,
            theBuffer);
    }
}



double
XNodeSetBase::stringLength(XPathExecutionContext&   executionContext) const
{
    if (m_cachedStringValue.empty() == false)
    {
        return static_cast<double>(m_cachedStringValue.length());
    }
    else if (getLength() == 0)
    {
        return 0;
    }
    else
    {
        const XalanNode* const  theNode = item(0);
        assert(theNode != 0);

        FormatterStringLengthCounter    theCounter;

        DOMServices::getNodeData(
            *theNode,
            executionContext,
            theCounter,
            &FormatterListener::characters);

        return static_cast<double>(theCounter.getCount());
    }
}



const XalanDocumentFragment&
XNodeSetBase::rtree() const
{
    return m_proxy;
}



void
XNodeSetBase::ProcessXObjectTypeCallback(XObjectTypeCallback&   theCallbackObject)
{
    theCallbackObject.NodeSet(
        *this,
        nodeset());
}



void
XNodeSetBase::ProcessXObjectTypeCallback(XObjectTypeCallback&   theCallbackObject) const
{
    theCallbackObject.NodeSet(
        *this,
        nodeset());
}



void
XNodeSetBase::clearCachedValues()
{
    m_cachedNumberValue = theBogusNumberValue;

    m_cachedStringValue.clear();
}



XALAN_CPP_NAMESPACE_END
