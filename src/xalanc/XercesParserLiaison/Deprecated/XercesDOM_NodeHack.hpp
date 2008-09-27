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

#if !defined(XERCESDOM_NODEHACK_HEADER_GUARD_1357924680)
#define XERCESDOM_NODEHACK_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_Node.hpp>
#include <xercesc/dom/deprecated/DOM_Attr.hpp>
#include <xercesc/dom/deprecated/DOM_Element.hpp>
#include <xercesc/dom/deprecated/DOM_Text.hpp>
#else
#include <xercesc/dom/DOM_Node.hpp>
#include <xercesc/dom/DOM_Attr.hpp>
#include <xercesc/dom/DOM_Element.hpp>
#include <xercesc/dom/DOM_Text.hpp>
#endif



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN


/**
 * This class is deprecated.
 *
 * An evil class to hack the Xerces smart-pointer class.  I'm
 * only doing this because I have to...
 * 
 * @deprecated This class is part of the deprecated Xerces DOM bridge.
 */
class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOM_NodeHack : public XERCES_CPP_NAMESPACE_QUALIFIER DOM_Node
{
public:

    typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Node     ParentType;

    XercesDOM_NodeHack(NodeImplType*    theImpl = 0);

    ~XercesDOM_NodeHack();

    NodeImplType*
    getImpl() const
    {
        return fImpl;
    }

    static NodeImplType*
    getImpl(const ParentType&       theNode)
    {
        return static_cast<const XercesDOM_NodeHack&>(theNode).getImpl();
    }
};



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOM_AttrHack : public XERCES_CPP_NAMESPACE_QUALIFIER DOM_Attr
{
public:

    typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Attr     ParentType;

    XercesDOM_AttrHack(AttrImplType*    theImpl = 0);

    XercesDOM_AttrHack(const ParentType&    theSource);

    ~XercesDOM_AttrHack();
};



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOM_ElementHack : public XERCES_CPP_NAMESPACE_QUALIFIER DOM_Element
{
public:

    typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Element  ParentType;

    XercesDOM_ElementHack(ElementImplType*  theImpl = 0);

    XercesDOM_ElementHack(const ParentType&     theSource);

    ~XercesDOM_ElementHack();


    const DOMStringType
    getNodeNameImpl() const;

    const DOMStringType
    getNodeValueImpl() const;

    const DOMStringType
    getNamespaceURIImpl() const;

    const DOMStringType
    getPrefixImpl() const;

    const DOMStringType
    getLocalNameImpl() const;

    const DOMStringType
    getTagNameImpl() const;

    const DOMStringType
    getAttributeImpl(const DOMStringType&   name) const;

    const DOMStringType
    getAttributeNSImpl(
            const DOMStringType&    namespaceURI,
            const DOMStringType&    localName) const;

    ElementImplType*
    getImpl() const
    {
        return reinterpret_cast<ElementImplType*>(fImpl);
    }

    static ElementImplType*
    getImpl(const ParentType&   theNode)
    {
        return static_cast<const XercesDOM_ElementHack&>(theNode).getImpl();
    }
};



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOM_TextHack : public XERCES_CPP_NAMESPACE_QUALIFIER DOM_Text
{
public:

    typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Text     ParentType;

    XercesDOM_TextHack(TextImplType*    theImpl = 0);

    XercesDOM_TextHack(const ParentType&    theSource);

    ~XercesDOM_TextHack();


    const DOMStringType
    getNodeNameImpl() const;

    const DOMStringType
    getNodeValueImpl() const;

    const DOMStringType
    getNamespaceURIImpl() const;

    const DOMStringType
    getPrefixImpl() const;

    const DOMStringType
    getLocalNameImpl() const;

    const DOMStringType
    getDataImpl() const;

    TextImplType*
    getImpl() const
    {
        return reinterpret_cast<TextImplType*>(fImpl);
    }

    static TextImplType*
    getImpl(const ParentType&   theNode)
    {
        return static_cast<const XercesDOM_TextHack&>(theNode).getImpl();
    }
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESDOM_NODEHACK_HEADER_GUARD_1357924680)
