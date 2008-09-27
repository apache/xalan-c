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
#if !defined(XRESULTTREEFRAG_HEADER_GUARD_1357924680)
#define XRESULTTREEFRAG_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



// Base class header file.
#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XObject.hpp>



#include <xalanc/XSLT/StylesheetExecutionContext.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class ResultTreeFrag;



class XALAN_XSLT_EXPORT XResultTreeFrag : public XObject
{
public:

    typedef XObject     ParentType;


    /**
     * Construct an XResultTreeFrag object from a result tree fragment
     * 
     * @param value The source result tree fragment.
     * @param theManager The MemoryManager for this instance.
     */
    XResultTreeFrag(
            XalanDocumentFragment&  value,
            MemoryManager&          theManager);

    /**
     * Construct an XResultTreeFrag object from another
     * 
     * @param source     source XResultTreeFrag
     * @param theManager The MemoryManager for this instance.
     */
    XResultTreeFrag(
            const XResultTreeFrag&  source,
            MemoryManager&          theManager);

    virtual
    ~XResultTreeFrag();

    MemoryManager&
    getMemoryManager()const
    {
        return m_cachedStringValue.getMemoryManager();
    }

    void
    setExecutionContext(StylesheetExecutionContext*     theExecutionContext)
    {
        m_executionContext = theExecutionContext;
    }

    // These methods are inherited from XObject ...

    virtual const XalanDOMString&
    getTypeString() const;
  
    virtual double
    num(XPathExecutionContext&  executionContext) const;

    virtual double
    num() const;

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

    virtual void
    ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject);

    virtual void
    ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject) const;

    /**
     * Release the ResultTreeFrag held by the instance.
     */
    XalanDocumentFragment*
    release();

    /**
     * Change the value of an XResultTreeFrag
     *
     * @param theValue The new value.
     */
    void
    set(XalanDocumentFragment&  theValue);

protected:

    virtual void 
    dereferenced();

private:
    //not implemented
    XResultTreeFrag();
    XResultTreeFrag(const XResultTreeFrag&);

    // Data members...
    XalanDocumentFragment*          m_value;    

    const XalanDOMString*           m_singleTextChildValue;

    StylesheetExecutionContext*     m_executionContext;

    mutable XalanDOMString          m_cachedStringValue;

    mutable double                  m_cachedNumberValue;
};



XALAN_CPP_NAMESPACE_END



#endif  // XRESULTTREEFRAG_HEADER_GUARD_1357924680
