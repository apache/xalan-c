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
#if !defined(XOBJECTRESULTTREEFRAGPROXYTEXT_HEADER_GUARD_1357924680)
#define XOBJECTRESULTTREEFRAGPROXYTEXT_HEADER_GUARD_1357924680



#include <xalanc/XPath/XPathDefinitions.hpp>


#include <xalanc/Include/XalanMemoryManagement.hpp>


#include <xalanc/XalanDOM/XalanText.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XPathExecutionContext;
class XObject;



class XALAN_XPATH_EXPORT XObjectResultTreeFragProxyText : public XalanText
{
public:

    /**
     * Perform static initialization.  See class XPathInit.
     */
    static void
    initialize(MemoryManager&   theManager);

    /**
     * Perform static shut down.  See class XPathInit.
     */
    static void
    terminate();

    /**
     * Constructor.
     *
     * @param theXObject The XObject instance for which this is a proxy.
     * @param theManager The MemoryManager for this instance.
     * @param theExecutionContext An optional XPathExecutionContext instance.
     */
    XObjectResultTreeFragProxyText(
                const XObject&          theXObject,
                MemoryManager&          theManager,
                XPathExecutionContext*  theExecutionContext);

    virtual
    ~XObjectResultTreeFragProxyText();

    MemoryManager&
    getMemoryManager()
    {
        return m_memoryManager;
    }

    XPathExecutionContext*
    getExecutionContext()
    {
        return m_executionContext;
    }

    // These interfaces are inherited from XalanText...
    virtual const XalanDOMString&
    getNodeName() const;

    virtual const XalanDOMString&
    getNodeValue() const;

    virtual NodeType
    getNodeType() const;

    virtual XalanNode*
    getParentNode() const;

    virtual const XalanNodeList*
    getChildNodes() const;

    virtual XalanNode*
    getFirstChild() const;

    virtual XalanNode*
    getLastChild() const;

    virtual XalanNode*
    getPreviousSibling() const;

    virtual XalanNode*
    getNextSibling() const;

    virtual const XalanNamedNodeMap*
    getAttributes() const;

    virtual XalanDocument*
    getOwnerDocument() const;

    virtual const XalanDOMString&
    getNamespaceURI() const;

    virtual const XalanDOMString&
    getPrefix() const;

    virtual const XalanDOMString&
    getLocalName() const;

    virtual bool
    isIndexed() const;

    virtual IndexType
    getIndex() const;

    virtual const XalanDOMString&
    getData() const;

    virtual bool
    isWhitespace() const;

private:

    // Not implemented...
    XObjectResultTreeFragProxyText&
    operator=(const XObjectResultTreeFragProxyText& theSource);

    bool
    operator==(const XObjectResultTreeFragProxyText&    theRHS) const;


    // Data members...
    const XObject&          m_value;

    MemoryManager&          m_memoryManager;

    XPathExecutionContext*  m_executionContext;

    static const XalanDOMString&    s_nameString;
};



XALAN_CPP_NAMESPACE_END



#endif  // XOBJECTRESULTTREEFRAGPROXYTEXT_HEADER_GUARD_1357924680
