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
#if !defined(XNODESETBASE_HEADER_GUARD_1357924680)
#define XNODESETBASE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XObject.hpp>



#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XNodeSetResultTreeFragProxy.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * Class to hold XPath return types.
 */
class XALAN_XPATH_EXPORT XNodeSetBase : public XObject
{
public:

    typedef NodeRefListBase::size_type  size_type;

    typedef XObject     ParentType;

    virtual
    ~XNodeSetBase();

    // These methods are inherited from XObject ...


    virtual const XalanDOMString&
    getTypeString() const;

    virtual double
    num(XPathExecutionContext&  executionContext) const;

    using ParentType::num;

    virtual bool
    boolean(XPathExecutionContext&  executionContext) const;

    virtual const XalanDOMString&
    str(XPathExecutionContext&  executionContext) const;

    virtual const XalanDOMString&
    str() const;

    virtual void
    str(
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const;

    virtual void
    str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const;

    virtual void
    str(
            XPathExecutionContext&  executionContext,
            XalanDOMString&         theBuffer) const;

    virtual void
    str(XalanDOMString&     theBuffer) const;

    virtual double
    stringLength(XPathExecutionContext&     executionContext) const;

    virtual const XalanDocumentFragment&
    rtree() const;

    virtual const NodeRefListBase&
    nodeset() const = 0;

    virtual void
    ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject);

    virtual void
    ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject) const;

    virtual XalanNode*
    item(size_type  index) const = 0;

    virtual size_type
    getLength() const = 0;

protected:

    /**
     * Create an XNodeSetBase
     */
    XNodeSetBase(MemoryManager&     theMemoryManager);

    /**
     * Create an XNodeSetBase from another.
     *
     * @param source    object to copy
     */
    XNodeSetBase(
        const XNodeSetBase&     source,
        MemoryManager&          theMemoryManager);

    void
    clearCachedValues();

private:

    // Not implemented...
    XNodeSetBase();
    XNodeSetBase(const XNodeSetBase&);
    XNodeSetBase&
    operator=(const XNodeSetBase&);

    // Data members...
    XNodeSetResultTreeFragProxy     m_proxy;

    mutable XalanDOMString          m_cachedStringValue;

    mutable double                  m_cachedNumberValue;
};



XALAN_CPP_NAMESPACE_END



#endif  // XNODESETBASE_HEADER_GUARD_1357924680
