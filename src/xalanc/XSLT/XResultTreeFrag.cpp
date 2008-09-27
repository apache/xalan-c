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
#include "XResultTreeFrag.hpp"



#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>
#include <xalanc/XalanDOM/XalanNodeList.hpp>
#include <xalanc/XalanDOM/XalanText.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/FormatterStringLengthCounter.hpp>
#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XObjectTypeCallback.hpp>



#include <xalanc/XSLT/StylesheetExecutionContext.hpp>



XALAN_CPP_NAMESPACE_BEGIN



inline const XalanDOMString*
getSingleTextChildValue(const XalanDocumentFragment&    theRTreeFrag)
{
    const XalanNode* const  theFirstChild = theRTreeFrag.getFirstChild();

    if (theFirstChild != 0 &&
        theFirstChild->getNodeType() == XalanNode::TEXT_NODE &&
        theFirstChild->getNextSibling() == 0)
    {
        return &theFirstChild->getNodeValue();
    }
    else
    {
        return 0;
    }
}



const double    theBogusNumberValue = 123456789;



XResultTreeFrag::XResultTreeFrag(
            XalanDocumentFragment&  value,
            MemoryManager&          theManager) :
    XObject(eTypeResultTreeFrag, theManager),
    m_value(&value),
    m_singleTextChildValue(getSingleTextChildValue(value)),
    m_executionContext(0),
    m_cachedStringValue(theManager),
    m_cachedNumberValue(theBogusNumberValue)

{
}



XResultTreeFrag::XResultTreeFrag(
            const XResultTreeFrag&  source,
            MemoryManager&          theManager) :
    XObject(source, theManager),
    m_value(source.m_value),
    m_singleTextChildValue(source.m_singleTextChildValue),
    m_executionContext(0),
    m_cachedStringValue(source.m_cachedStringValue, theManager),
    m_cachedNumberValue(source.m_cachedNumberValue)
{
    assert(m_value != 0);
}



XResultTreeFrag::~XResultTreeFrag()
{
}



const XalanDOMString&
XResultTreeFrag::getTypeString() const
{
    return s_resultTreeFragmentString;
}



double
XResultTreeFrag::num(XPathExecutionContext&     executionContext) const
{
    if (m_cachedNumberValue == theBogusNumberValue)
    {
        m_cachedNumberValue =
            DoubleSupport::toDouble(
                str(executionContext),
                getMemoryManager());
    }

    return m_cachedNumberValue;
}



double
XResultTreeFrag::num() const
{
    if (m_cachedNumberValue == theBogusNumberValue)
    {
        m_cachedNumberValue =
            DoubleSupport::toDouble(
                str(),
                getMemoryManager());
    }

    return m_cachedNumberValue;
}



bool
XResultTreeFrag::boolean(XPathExecutionContext&     /* executionContext */) const
{
    // Result tree fragments always evaluate to true.
    return true;
}



const XalanDOMString&
XResultTreeFrag::str(XPathExecutionContext&     /* executionContext */) const
{
    return XResultTreeFrag::str();
}



const XalanDOMString&
XResultTreeFrag::str() const
{
    if (m_singleTextChildValue != 0)
    {
        assert(m_value->getFirstChild() != 0 &&
                m_value->getFirstChild()->getNodeType() == XalanNode::TEXT_NODE);

        return *m_singleTextChildValue;
    }
    else if (m_cachedStringValue.empty() == true)
    {
        DOMServices::getNodeData(
            *m_value,
            m_cachedStringValue);
    }

    return m_cachedStringValue;
}



void
XResultTreeFrag::str(
            XPathExecutionContext&  /* executionContext */,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    XResultTreeFrag::str(formatterListener, function);
}



void
XResultTreeFrag::str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    if (m_singleTextChildValue != 0)
    {
        assert(m_value->getFirstChild() != 0 &&
               m_value->getFirstChild()->getNodeType() == XalanNode::TEXT_NODE);

        XObject::string(*m_singleTextChildValue, formatterListener, function);
    }
    else if (m_cachedStringValue.empty() == false)
    {
        XObject::string(m_cachedStringValue, formatterListener, function);
    }
    else
    {
        DOMServices::getNodeData(
            *m_value,
            formatterListener,
            function);
    }
}



void
XResultTreeFrag::str(
            XPathExecutionContext&  /* executionContext */,
            XalanDOMString&         theBuffer) const
{
    XResultTreeFrag::str(theBuffer);
}



void
XResultTreeFrag::str(XalanDOMString&    theBuffer) const
{
    if (m_singleTextChildValue != 0)
    {
        theBuffer.append(*m_singleTextChildValue);
    }
    else if (m_cachedStringValue.empty() == false)
    {
        theBuffer.append(m_cachedStringValue);
    }
    else
    {
        DOMServices::getNodeData(
            *m_value,
            theBuffer);
    }
}



double
XResultTreeFrag::stringLength(XPathExecutionContext&    executionContext) const
{
    if (m_singleTextChildValue != 0)
    {
        return static_cast<double>(m_singleTextChildValue->length());
    }
    else if (m_cachedStringValue.empty() == false)
    {
        return static_cast<double>(m_cachedStringValue.length());
    }
    else
    {
        FormatterStringLengthCounter    theCounter;

        DOMServices::getNodeData(
            *m_value,
            executionContext,
            theCounter,
            &FormatterListener::characters);

        return static_cast<double>(theCounter.getCount());
    }
}



const XalanDocumentFragment&
XResultTreeFrag::rtree() const
{
    assert(m_value != 0);

    return *m_value;
}



void
XResultTreeFrag::ProcessXObjectTypeCallback(XObjectTypeCallback&    theCallbackObject)
{
    theCallbackObject.ResultTreeFragment(
        *this,
        rtree());
}



void
XResultTreeFrag::ProcessXObjectTypeCallback(XObjectTypeCallback&    theCallbackObject) const
{
    theCallbackObject.ResultTreeFragment(
        *this,
        rtree());
}



XalanDocumentFragment*
XResultTreeFrag::release()
{
    m_singleTextChildValue = 0;

    m_cachedStringValue.clear();

    m_cachedNumberValue = theBogusNumberValue;

    XalanDocumentFragment* const    temp = m_value;

    m_value = 0;

    return temp;
}



void
XResultTreeFrag::set(XalanDocumentFragment&     theValue)
{
    release();

    m_value = &theValue;

    m_singleTextChildValue = getSingleTextChildValue(*m_value);
}



void
XResultTreeFrag::dereferenced()
{
    if (m_executionContext == 0 ||
        // We're dead after we call this, if it returns true;
        m_executionContext->returnXResultTreeFrag(this) == false)
    {
        delete m_value;

        delete this;
    }
}



XALAN_CPP_NAMESPACE_END
