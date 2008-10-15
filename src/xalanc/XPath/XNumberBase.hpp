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
#if !defined(XNUMBERBASE_HEADER_GUARD_1357924680)
#define XNUMBERBASE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



// Base class header file.
#include <xalanc/XPath/XObject.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XNumberBase : public XObject
{
public:

    typedef XObject ParentType;

    /**
     * Construct an XNumberBase object from another.
     * 
     * @param source The source XNumberBase instance.
     * @param theMemoryManager The MemoryManager instance.
     */
    XNumberBase(
            const XNumberBase&  source,
            MemoryManager&      theMemoryManager);

    virtual
    ~XNumberBase();

    // These methods are inherited from XObject ...

    virtual const XalanDOMString&
    getTypeString() const;

    virtual double
    num(XPathExecutionContext&  executionContext) const = 0;

    using ParentType::num;

    virtual bool
    boolean(XPathExecutionContext&  executionContext) const;

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

    virtual void
    ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject);

    virtual void
    ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject) const;

protected:

    /**
     * Constructor for derived classes
     * 
     * @param theMemoryManager The MemoryManager instance.
     */
    XNumberBase(MemoryManager&  theMemoryManager);

private:
};



XALAN_CPP_NAMESPACE_END



#endif  // XNUMBERBASE_HEADER_GUARD_1357924680
