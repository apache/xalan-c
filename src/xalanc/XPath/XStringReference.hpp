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
#if !defined(XSTRINGREFERENCE_HEADER_GUARD_1357924680)
#define XSTRINGREFERENCE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XStringBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XStringReference : public XStringBase
{
public:

    typedef XStringBase     ParentType;

    /**
     * Construct an XStringReference object from a string.
     * The XStringReference will hold a reference to the
     * supplied string, so the string must be in scope for
     * the lifetime of the instance
     * 
     * @param value      source string
     */
    XStringReference(const XalanDOMString&  val,
                        MemoryManager&  theManager);

    XStringReference(const XStringReference&    source,
                        MemoryManager&      theManager);

    virtual
    ~XStringReference();


    // These methods are inherited from XObject ...

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

protected:

    virtual eObjectType
    getRealType() const;

private:

    // Not implemented
    XStringReference(const XStringReference&    source);

    XStringReference();

    const XalanDOMString&   m_value;
};



XALAN_CPP_NAMESPACE_END



#endif  // XSTRINGREFERENCE_HEADER_GUARD_1357924680
