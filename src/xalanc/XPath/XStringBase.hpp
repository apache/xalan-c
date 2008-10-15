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
#if !defined(XSTRINGBASE_HEADER_GUARD_1357924680)
#define XSTRINGBASE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XObjectResultTreeFragProxy.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XPathEnvSupport;



class XALAN_XPATH_EXPORT XStringBase : public XObject
{
public:

    typedef XObject     ParentType;

    /**
     * Construct an XStringBase object.
     * 
     * @param theMemoryManager The MemoryManager instance.
     */
    XStringBase(MemoryManager&  theMemoryManager);

    /**
     * Construct an XStringBase object.
     * 
     * @param theMemoryManager The MemoryManager instance.
     * @param theExecutionContext The current XPathExecutionContext.
     */
    XStringBase(
            MemoryManager&          theMemoryManager,
            XPathExecutionContext&  theExecutionContext);

    /**
     * Construct an XStringBase object from another.
     * 
     * @param source The source XStringBase instance.
     * @param theMemoryManager The MemoryManager instance.
     */
    XStringBase(
            const XStringBase&  source,
            MemoryManager&      theMemoryManager);

    virtual
    ~XStringBase();


    // These methods are inherited from XObject ...

    virtual const XalanDOMString&
    getTypeString() const;
  
    virtual double
    num(XPathExecutionContext&  executionContext) const;

    using ParentType::num;

    virtual bool
    boolean(XPathExecutionContext&  executionContext) const;

    using ParentType::str;

    virtual const XalanDOMString&
    str(XPathExecutionContext&  executionContext) const = 0;

    virtual const XalanDOMString&
    str() const = 0;

    virtual void
    str(
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const = 0;

    virtual void
    str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const = 0;

    virtual void
    str(
            XPathExecutionContext&  executionContext,
            XalanDOMString&         theBuffer) const = 0;

    virtual void
    str(XalanDOMString&     theBuffer) const = 0;

    virtual double
    stringLength(XPathExecutionContext&     executionContext) const = 0;

    virtual const XalanDocumentFragment&
    rtree() const;

    virtual void
    ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject);

    virtual void
    ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject) const;

private:

    friend class XObjectResultTreeFragProxyText;

    // Not implemented
    XStringBase(const XStringBase&  source);

    XStringBase();

    mutable double                      m_cachedNumberValue;

    mutable XObjectResultTreeFragProxy  m_resultTreeFrag;
};



XALAN_CPP_NAMESPACE_END



#endif  // XSTRINGBASE_HEADER_GUARD_1357924680
