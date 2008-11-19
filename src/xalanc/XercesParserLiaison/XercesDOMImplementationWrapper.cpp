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
#include "XercesDOMImplementationWrapper.hpp"



#include <xercesc/dom/DOMImplementation.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "XercesDOMException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesDOMImplementationWrapper::XercesDOMImplementationWrapper(DOMImplementationType*   theXercesDOMImplementation) :
    XalanDOMImplementation(),
    m_xercesNode(theXercesDOMImplementation)
{
    assert(theXercesDOMImplementation != 0);
}



XercesDOMImplementationWrapper::~XercesDOMImplementationWrapper()
{
}



XercesDOMImplementationWrapper::XercesDOMImplementationWrapper(const XercesDOMImplementationWrapper&    theSource) :
    XalanDOMImplementation(theSource),
    m_xercesNode(theSource.m_xercesNode)
{
}



XercesDOMImplementationWrapper*
XercesDOMImplementationWrapper::create(
            MemoryManager&          theManager,
            DOMImplementationType*  theXercesDOMImplementation)
{
    typedef XercesDOMImplementationWrapper  ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(theXercesDOMImplementation);

    theGuard.release();

    return theResult;
}



bool
XercesDOMImplementationWrapper::hasFeature(
            const XalanDOMString&   feature,
            const XalanDOMString&   version)
{
    return m_xercesNode->hasFeature(feature.c_str(), version.c_str());
}



XalanDocumentType*
XercesDOMImplementationWrapper::createDocumentType(
            const XalanDOMString&   /* qualifiedName */,
            const XalanDOMString&   /* publicId */,
            const XalanDOMString&   /* systemId */)
{
    throw XercesDOMException(XercesDOMException::NO_MODIFICATION_ALLOWED_ERR);

    return 0;
}



XalanDocument*
XercesDOMImplementationWrapper::createDocument(
            const XalanDOMString&       /* namespaceURI */,
            const XalanDOMString&       /* qualifiedName */,
            const XalanDocumentType&    /* doctype */)
{
    throw XercesDOMException(XercesDOMException::NO_MODIFICATION_ALLOWED_ERR);

    return 0;
}



XALAN_CPP_NAMESPACE_END
