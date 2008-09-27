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
#if !defined(XTOKEN_HEADER_GUARD_1357924680)
#define XTOKEN_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class...
#include <xalanc/XPath/XObject.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XToken : public XObject
{
public:

    /**
     * Create an XToken.
     *
     * @param theMemoryManager The MemoryManager instance.
     */
    XToken(MemoryManager&   theMemoryManager);

    /**
     * Create an XToken for string in the token queue.
     *
     * @param theString The string data for the token.  The instance will keep a point to this string, so it must be persistent.
     * @param theNumber The numeric data for the token.  This must be consistent with the lexical value in theString.
     * @param theMemoryManager The MemoryManager instance.
     */
    XToken(
            const XalanDOMString&   theString,
            double                  theNumber,
            MemoryManager&          theMemoryManager);

    /**
     * Create an XToken for number in the token queue.
     *
     * @param theNumber The numeric data for the token.  This must be consistent with the lexical value in theString.
     * @param theString The string data for the token.  The instance will keep a point to this string, so it must be persistent.
     * @param theMemoryManager The MemoryManager instance.
     */
    XToken(
            double                  theNumber,
            const XalanDOMString&   theString,
            MemoryManager&          theMemoryManager);

    /**
     * Create an XToken from another one.
     *
     * In most cases, the following copy constructor,
     * which accepts an explicit MemoryManager instance,
     * is recommended.
     *
     * @param theSource The source XToken instance.
     */
    XToken(const XToken&    theSource);

    /**
     * Create an XToken from another one.
     *
     * @param theSource The source XToken instance.
     * @param theMemoryManager The MemoryManager instance.
     */
    XToken(
            const XToken&   theSource,
            MemoryManager&  theMemoryManager);

    virtual
    ~XToken();

    virtual const XalanDOMString&
    getTypeString() const;

    virtual double
    num(XPathExecutionContext&  executionContext) const;

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

    virtual void
    ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject);

    virtual void
    ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject) const;

    XToken&
    operator=(const XToken&     theRHS)
    {
        m_stringValue = theRHS.m_stringValue;

        m_numberValue = theRHS.m_numberValue;

        return *this;
    }

    bool
    boolean() const
    {
        assert(m_stringValue != 0);

        return m_isString == true ? XObject::boolean(*m_stringValue) : XObject::boolean(m_numberValue);
    }

    double
    num() const
    {
        assert(m_stringValue != 0);

        return m_numberValue;
    }

    double
    stringLength() const
    {
        assert(m_stringValue != 0);

        return static_cast<double>(m_stringValue->length());
    }

    /**
     * Set the instance as a string in the token queue.
     *
     * @param theString The string data for the token.  XToken will keep a point to this string, so it must be persistent.
     * @param theNumber The numeric data for the token.  This must be consistent with the lexical value in theString.
     */
    void
    set(
            const XalanDOMString&   theString,
            double                  theNumber);

    /**
     * Set the instance as a number in the token queue.
     *
     * @param theNumber The numeric data for the token.  This must be consistent with the lexical value in theString.
     * @param theString The string data for the token.  XToken will keep a point to this string, so it must be persistent.
     */
    void
    set(
            double                  theNumber,
            const XalanDOMString&   theString);

protected:

    virtual void 
    referenced();

    virtual void 
    dereferenced();

private:

    // Not defined...
    bool
    operator==(const XToken&) const;

    // Data members...
    const XalanDOMString*   m_stringValue;

    double                  m_numberValue;

    bool                    m_isString;
};



XALAN_USES_MEMORY_MANAGER(XToken)



XALAN_CPP_NAMESPACE_END



#endif  // XTOKEN_HEADER_GUARD_1357924680
