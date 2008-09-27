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
#if !defined(XOBJECTRESULTTREEFRAGPROXYBASE_HEADER_GUARD_1357924680)
#define XOBJECTRESULTTREEFRAGPROXYBASE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>

#include <xalanc/Include/XalanMemoryManagement.hpp>


#include <xalanc/XalanDOM/XalanNodeList.hpp>
#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XObjectResultTreeFragProxyBase : public XalanDocumentFragment
{
public:

    XObjectResultTreeFragProxyBase();

    virtual
    ~XObjectResultTreeFragProxyBase();


    // These interfaces are inherited from XalanDocumentFragment...

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
    getFirstChild() const = 0;

    virtual XalanNode*
    getLastChild() const = 0;

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

protected:

    XObjectResultTreeFragProxyBase(const XObjectResultTreeFragProxyBase&    theRHS);

private:

    // Not implemented...
    XObjectResultTreeFragProxyBase&
    operator=(const XObjectResultTreeFragProxyBase& theRHS);

    bool
    operator==(const XObjectResultTreeFragProxyBase&    theRHS);


    // Data members...
    static const XalanDOMString             s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif  // XOBJECTRESULTTREEFRAGPROXYBASE_HEADER_GUARD_1357924680
