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
#if !defined(XBOOLEAN_HEADER_GUARD_1357924680)
#define XBOOLEAN_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XObject.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XBoolean : public XObject
{
public:

    typedef XObject     ParentType;

    /**
     * Construct an XBoolean object from a boolean value
     * 
     * @param val   The boolean value to initialize object
     * @param theMemoryManager The MemoryManager instance.
     */
    XBoolean(
            bool            val,
            MemoryManager&  theMemoryManager);

    /**
     * Construct an XBoolean object from another.
     * 
     * @param source The source XBoolean instance.
     * @param theMemoryManager The MemoryManager instance.
     */
    XBoolean(
            const XBoolean&     source,
            MemoryManager&      theMemoryManager);

    virtual
    ~XBoolean();

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

    using ParentType::str;

    virtual double
    stringLength(XPathExecutionContext&     executionContext) const;

    virtual void
    ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject);

    virtual void
    ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject) const;

private:

    bool    m_value;
};



XALAN_CPP_NAMESPACE_END



#endif  // XBOOLEAN_HEADER_GUARD_1357924680
