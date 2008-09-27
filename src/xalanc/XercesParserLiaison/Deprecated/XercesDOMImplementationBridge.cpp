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


#include "XercesDOMImplementationBridge.hpp"



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_DOMImplementation.hpp>
#else
#include <xercesc/dom/DOM_DOMImplementation.hpp>
#endif



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesDOMImplementationBridge::XercesDOMImplementationBridge(DOM_DOMImplementationType& theXercesDOMImplementation) :
    XalanDOMImplementation(),
    m_xercesNode(theXercesDOMImplementation)
{
}



XercesDOMImplementationBridge::~XercesDOMImplementationBridge()
{
}



XercesDOMImplementationBridge::XercesDOMImplementationBridge(const XercesDOMImplementationBridge&   theSource) :
    XalanDOMImplementation(theSource),
    m_xercesNode(theSource.m_xercesNode)
{
}



bool
XercesDOMImplementationBridge::hasFeature(
            const XalanDOMString&   feature,
            const XalanDOMString&   version)
{
    return m_xercesNode.hasFeature(feature.c_str(), version.c_str());
}



XalanDocumentType*
XercesDOMImplementationBridge::createDocumentType(
            const XalanDOMString&   /* qualifiedName */,
            const XalanDOMString&   /* publicId */,
            const XalanDOMString&   /* systemId */)
{
    throw XercesDOMException(XercesDOMException::NOT_SUPPORTED_ERR);

    return 0;
}



XalanDocument*
XercesDOMImplementationBridge::createDocument(
            const XalanDOMString&       /* namespaceURI */,
            const XalanDOMString&       /* qualifiedName */,
            const XalanDocumentType&    /* doctype */)
{
    throw XercesDOMException(XercesDOMException::NOT_SUPPORTED_ERR);

    return 0;
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


