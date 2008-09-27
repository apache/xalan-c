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
#if !defined(XALANSOURCETREEELEMENTNANS_HEADER_GUARD_1357924680)
#define XALANSOURCETREEELEMENTNANS_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeElementNA.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeElementNANS : public XalanSourceTreeElementNA
{
public:

    /**
     * Constructor.
     *
     * @param theTagName The tag name of the element
     * @param theLocalName The local name of the attribute
     * @param theNamespaceURI The namespace URI of the attribute
     * @param thePrefix The namespace prefix of the attribute
     * @param theOwnerDocument The document that owns the instance
     * @param theParentNode The parent node, if any.
     * @param thePreviousSibling The previous sibling, if any.
     * @param theNextSibling The next sibling, if any.
     * @param theIndex The document-order index of the node.
     */
    XalanSourceTreeElementNANS(
            MemoryManager&              theManager,
            const XalanDOMString&       theTagName,
            const XalanDOMString&       theLocalName,
            const XalanDOMString&       theNamespaceURI,
            const XalanDOMString&       thePrefix,
            XalanSourceTreeDocument*    theOwnerDocument,
            XalanNode*                  theParentNode = 0,
            XalanNode*                  thePreviousSibling = 0,
            XalanNode*                  theNextSibling = 0,
            IndexType                   theIndex = 0);

    virtual
    ~XalanSourceTreeElementNANS();


    virtual const XalanDOMString&
    getNamespaceURI() const;

    virtual const XalanDOMString&
    getPrefix() const;

    virtual const XalanDOMString&
    getLocalName() const;

private:

    // Not implemented...
    XalanSourceTreeElementNANS(const XalanSourceTreeElementNANS&);

    XalanSourceTreeElementNANS&
    operator=(const XalanSourceTreeElementNANS& theSource);

    bool
    operator==(const XalanSourceTreeElementNANS&    theRHS) const;


    // Data members...
    const XalanDOMString&   m_localName;

    const XalanDOMString&   m_prefix;

    const XalanDOMString&   m_namespaceURI;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XALANSOURCETREEELEMENTNANS_HEADER_GUARD_1357924680)
