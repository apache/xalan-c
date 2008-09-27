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
#if !defined(XALANSOURCETREEATTRNS_HEADER_GUARD_1357924680)
#define XALANSOURCETREEATTRNS_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeAttr.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/*
 * <meta name="usage" content="experimental"/>
 *
 * Base class for the Xalan source tree Attr interface.
 *
 * This class is experimental and subject to change!!
 */

class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeAttrNS : public XalanSourceTreeAttr
{
public:

    /**
     * Constructor.
     *
     * @param theName The name of the attribute
     * @param theLocalName The local name of the attribute
     * @param theNamespaceURI The namespace URI of the attribute
     * @param thePrefix The namespace prefix of the attribute
     * @param theValue The value of the attribute
     * @param theOwnerElement The element that owns the instance
     * @param theIndex The document-order index of the node.
     */
    XalanSourceTreeAttrNS(
            const XalanDOMString&       theName,
            const XalanDOMString&       theLocalName,
            const XalanDOMString&       theNamespaceURI,
            const XalanDOMString&       thePrefix,
            const XalanDOMString&       theValue,
            XalanSourceTreeElement*     theOwnerElement = 0,
            IndexType                   theIndex = 0);

    virtual
    ~XalanSourceTreeAttrNS();


    virtual const XalanDOMString&
    getNamespaceURI() const;

    virtual const XalanDOMString&
    getPrefix() const;

    virtual const XalanDOMString&
    getLocalName() const;

private:

    // Not defined...
    XalanSourceTreeAttrNS(const XalanSourceTreeAttrNS&      theSource);

    XalanSourceTreeAttrNS&
    operator=(const XalanSourceTreeAttrNS&  theSource);

    bool
    operator==(const XalanSourceTreeAttrNS&     theRHS) const;

    // Data members...
    const XalanDOMString&   m_localName;

    const XalanDOMString&   m_prefix;

    const XalanDOMString&   m_namespaceURI;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XALANSOURCETREEATTRNS_HEADER_GUARD_1357924680)
