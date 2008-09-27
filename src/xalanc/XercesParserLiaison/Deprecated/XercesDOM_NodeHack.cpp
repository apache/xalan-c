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


#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)



#include "XercesDOM_NodeHack.hpp"



#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
#include <dom/ElementImpl.hpp>
#include <dom/TextImpl.hpp>
#endif



XALAN_CPP_NAMESPACE_BEGIN



XercesDOM_NodeHack::XercesDOM_NodeHack(NodeImplType*    theImpl) :
    DOM_Node(theImpl)
{
}



XercesDOM_NodeHack::~XercesDOM_NodeHack()
{
}



XercesDOM_AttrHack::XercesDOM_AttrHack(AttrImplType*    theImpl) :
    ParentType(theImpl)
{
}



XercesDOM_AttrHack::XercesDOM_AttrHack(const ParentType&    theSource) :
    ParentType(theSource)
{
}



XercesDOM_AttrHack::~XercesDOM_AttrHack()
{
}



XercesDOM_ElementHack::XercesDOM_ElementHack(ElementImplType*   theImpl) :
    ParentType(theImpl)
{
}



XercesDOM_ElementHack::XercesDOM_ElementHack(const ParentType&  theSource) :
    ParentType(theSource)
{
}



XercesDOM_ElementHack::~XercesDOM_ElementHack()
{
}



const DOMStringType
XercesDOM_ElementHack::getNodeNameImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getNodeName();
#else
    return getNodeName();
#endif
}



const DOMStringType
XercesDOM_ElementHack::getNodeValueImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getNodeValue();
#else
    return getNodeValue();
#endif
}



const DOMStringType
XercesDOM_ElementHack:: getNamespaceURIImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getNamespaceURI();
#else
    return getNamespaceURI();
#endif
}



const DOMStringType
XercesDOM_ElementHack::getPrefixImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getPrefix();
#else
    return getPrefix();
#endif
}



const DOMStringType
XercesDOM_ElementHack::getLocalNameImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getLocalName();
#else
    return getLocalName();
#endif
}



const DOMStringType
XercesDOM_ElementHack::getTagNameImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getTagName();
#else
    return getTagName();
#endif
}



const DOMStringType
XercesDOM_ElementHack::getAttributeImpl(const DOMStringType&    name) const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getAttribute(name);
#else
    return getAttribute(name);
#endif
}



const DOMStringType
XercesDOM_ElementHack::getAttributeNSImpl(
            const DOMStringType&    namespaceURI,
            const DOMStringType&    localName) const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getAttributeNS(namespaceURI, localName);
#else
    return getAttributeNS(namespaceURI, localName);
#endif
}



XercesDOM_TextHack::XercesDOM_TextHack(TextImplType*    theImpl) :
    ParentType(theImpl)
{
}



XercesDOM_TextHack::XercesDOM_TextHack(const ParentType&    theSource) :
    ParentType(theSource)
{
}



XercesDOM_TextHack::~XercesDOM_TextHack()
{
}



const DOMStringType
XercesDOM_TextHack::getNodeNameImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getNodeName();
#else
    return getNodeName();
#endif
}



const DOMStringType
XercesDOM_TextHack::getNodeValueImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getNodeValue();
#else
    return getNodeValue();
#endif
}



const DOMStringType
XercesDOM_TextHack::getNamespaceURIImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getNamespaceURI();
#else
    return getNamespaceURI();
#endif
}



const DOMStringType
XercesDOM_TextHack::getPrefixImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getPrefix();
#else
    return getPrefix();
#endif
}



const DOMStringType
XercesDOM_TextHack::getLocalNameImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getLocalName();
#else
    return getLocalName();
#endif
}



const DOMStringType
XercesDOM_TextHack::getDataImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
    return getImpl()->getData();
#else
    return getData();
#endif
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


