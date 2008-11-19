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
#if !defined(ATTRIBUTEVECTORENTRYEXTENDED_HEADER_GUARD_1357924680)
#define ATTRIBUTEVECTORENTRYEXTENDED_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/PlatformSupport/AttributeVectorEntry.hpp>

#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>

XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT AttributeVectorEntryExtended : public AttributeVectorEntry
{
public:

    AttributeVectorEntryExtended(
            const XMLChVectorType&  theName,
            const XMLChVectorType&  theValue,
            const XMLChVectorType&  theType,
            const XMLChVectorType&  theURI ,
            const XMLChVectorType&  theLocalName,
            MemoryManager&          theManager) :
        AttributeVectorEntry(
            theName,
            theValue,
            theType,
            theManager),
        m_uri(
            theURI,
            theManager),
        m_localName(
            theLocalName,
            theManager)
    {
    }

    AttributeVectorEntryExtended(
            const XMLCh*    theName,
            const XMLCh*    theValue,
            const XMLCh*    theType,
            const XMLCh*    theURI,
            const XMLCh*    theLocalName,
            MemoryManager&  theManager) :
        AttributeVectorEntry(
            theName,
            theValue,
            theType,
            theManager),
        m_uri(
            theURI,
            theURI + length(theURI) + 1,
            theManager),
        m_localName(
            theLocalName,
            theLocalName + length(theLocalName) + 1,
            theManager)
    {
    }

    AttributeVectorEntryExtended(
            const XMLCh*    theName,
            const XMLCh*    theValue,
            const XMLCh*    theType,
            MemoryManager&      theManager) :
        AttributeVectorEntry(
            theName,
            theValue,
            theType,theManager),
        m_uri(theManager),
        m_localName(theManager)
    {
    }

    AttributeVectorEntryExtended(MemoryManager&     theManager) :
        AttributeVectorEntry(theManager),
        m_uri(theManager),
        m_localName(theManager)
    {
    }

    static AttributeVectorEntryExtended*
    create(
            const XMLCh*    theName,
            const XMLCh*    theValue,
            const XMLCh*    theType,
            const XMLCh*    theURI,
            const XMLCh*    theLocalName,
            MemoryManager&  theManager)
    {
        AttributeVectorEntryExtended*   theInstance;

        return XalanConstruct(
                theManager,
                theInstance,
                theName,
                theValue,
                theType,
                theURI,
                theLocalName,
                theManager);
    }

    virtual
    ~AttributeVectorEntryExtended()
    {
    }

    void
    clear()
    {
        AttributeVectorEntry::clear();

        m_uri.clear();
        m_localName.clear();
    }

    XMLChVectorType     m_uri;
    XMLChVectorType     m_localName;
};



XALAN_CPP_NAMESPACE_END



#endif  // ATTRIBUTEVECTORENTRY_HEADER_GUARD_1357924680
