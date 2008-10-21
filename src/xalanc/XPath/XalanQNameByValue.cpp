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
// Class header file...
#include "XalanQNameByValue.hpp"



#include <xalanc/PlatformSupport/PrefixResolver.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>

#include <xalanc/XPath/XalanQName.hpp>

#include <xalanc/DOMSupport/DOMServices.hpp>
#include <xalanc/DOMSupport/DOMSupport.hpp>



#include "ElementPrefixResolverProxy.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanQNameByValue::XalanQNameByValue(MemoryManager&     theManager) :
    XalanQName(),
    m_namespace(theManager),
    m_localpart(theManager)
{
}



XalanQNameByValue::XalanQNameByValue(
            const XalanQNameByValue&    theSource,
            MemoryManager&              theManager) :
    XalanQName(theSource),
    m_namespace(theSource.m_namespace, theManager),
    m_localpart(theSource.m_localpart, theManager)
{
}



XalanQNameByValue::XalanQNameByValue(
            const XalanQName&   theSource,
            MemoryManager&      theManager) :
    XalanQName(),
    m_namespace(theSource.getNamespace(), theManager),
    m_localpart(theSource.getLocalPart(), theManager)
{
}



XalanQNameByValue::XalanQNameByValue(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   theLocalPart,
            MemoryManager&          theManager) :
    XalanQName(),
    m_namespace(theNamespace, theManager),
    m_localpart(theLocalPart, theManager)
{
}



XalanQNameByValue*
XalanQNameByValue::create(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   theLocalPart,
            MemoryManager&          theManager)
{
    typedef XalanQNameByValue ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult = 
        new (theGuard.get()) ThisType(
                                theNamespace,
                                theLocalPart,
                                theManager);


    theGuard.release();

    return theResult;
}



XalanQNameByValue::XalanQNameByValue(
            const XalanDOMString&       qname,
            const NamespacesStackType&  namespaces,
            MemoryManager&              theManager,
            const Locator*              locator,
            bool                        fUseDefault) :
    XalanQName(),
    m_namespace(theManager),
    m_localpart(theManager)
{
    initialize(
        qname.c_str(),
        qname.length(),
        namespaces,
        locator,
        fUseDefault);
}



XalanQNameByValue::XalanQNameByValue(
            const XalanDOMChar*         qname,
            const NamespacesStackType&  namespaces,
            MemoryManager&              theManager,
            const Locator*              locator,
            bool                        fUseDefault) :
    XalanQName(),
    m_namespace(theManager),
    m_localpart(theManager)
{
    assert(qname != 0);

    initialize(
        qname,
        length(qname),
        namespaces,
        locator,
        fUseDefault);
}



XalanQNameByValue::XalanQNameByValue(
            const XalanDOMString&   qname,
            MemoryManager&          theManager,
            const PrefixResolver*   theResolver,
            const Locator*          locator) :
    XalanQName(),
    m_namespace(theManager),
    m_localpart(theManager)
{
    resolvePrefix(
        qname.c_str(),
        qname.length(),
        theResolver,
        locator);
}



XalanQNameByValue::~XalanQNameByValue()
{
}



const XalanDOMString&
XalanQNameByValue::getLocalPart() const
{
    return m_localpart;
}



const XalanDOMString&
XalanQNameByValue::getNamespace() const
{
    return m_namespace;
}



void
XalanQNameByValue::set(
            const XalanDOMString&       qname,
            const NamespacesStackType&  namespaces,
            const Locator*              locator,
            bool                        fUseDefault)
{
    initialize(
        qname.c_str(),
        qname.length(),
        namespaces,
        locator,
        fUseDefault);
}



void
XalanQNameByValue::set(
            const XalanDOMChar*         qname,
            const NamespacesStackType&  namespaces,
            const Locator*              locator,
            bool                        fUseDefault)
{
    assert(qname != 0);

    initialize(
        qname,
        length(qname),
        namespaces,
        locator,
        fUseDefault);
}



void
XalanQNameByValue::set(
            const XalanDOMString&   qname,
            const PrefixResolver*   theResolver,
            const Locator*          locator)
{
    resolvePrefix(
        qname.c_str(),
        qname.length(),
        theResolver,
        locator);
}



void
XalanQNameByValue::set(
            const XalanDOMChar*     qname,
            const PrefixResolver*   theResolver,
            const Locator*          locator)
{
    assert(qname != 0);

    resolvePrefix(
        qname,
        length(qname),
        theResolver,
        locator);
}



static void
throwException(
            MemoryManager&          theMemoryManager,            
            XalanMessages::Codes    theCode,
            const XalanDOMString&   theParameter,
            const Locator*          theLocator)
{
    XalanDOMString  theMessage(theMemoryManager);

    XalanMessageLoader::getMessage(
        theMessage,
        theCode,
        theParameter);

    throw XalanQName::InvalidQNameException(
            theMessage,
            theMemoryManager,
            theLocator);
}



static void
throwException(
            MemoryManager&              theMemoryManager,            
            const XalanDOMChar*         theQName,
            XalanDOMString::size_type   theLength,
            const Locator*              theLocator)
{
    XalanDOMString  theBuffer(theMemoryManager);

    throw XalanQName::InvalidQNameException(
            theQName,
            theLength,
            theBuffer,
            theLocator);
}



void
XalanQNameByValue::initialize(
            const XalanDOMChar*         qname,
            XalanDOMString::size_type   len,
            const NamespacesStackType&  namespaces,
            const Locator*              locator,
            bool                        fUseDefault)
{
    const XalanDOMString::size_type     indexOfNSSep = indexOf(qname, XalanUnicode::charColon);

    if (indexOfNSSep == 0)
    {
        throwException(
            getMemoryManager(),
            XalanMessages::PrefixOfLengthZeroDetected,
            m_localpart,    // This is a dummy parameter...
            locator);
    }
    else if(indexOfNSSep < len)
    {
        // Reserve some space for the local part right now, and
        // use it as a temporary for the prefix.
        m_localpart.reserve(len);

        m_localpart.assign(qname, indexOfNSSep);

        if(m_localpart == DOMServices::s_XMLString)
        {
            m_namespace = DOMServices::s_XMLNamespaceURI;
        }
        else if(m_localpart == DOMServices::s_XMLNamespace)
        {
            // The xmlns prefix is not normally bound to a namespace URI, but we're really trying
            // to form a QName, so we'll do what the DOM does...
            m_namespace = DOMServices::s_XMLNamespacePrefixURI;
        }
        else
        {
            const XalanDOMString* const     theNamespace = 
                        getNamespaceForPrefix(namespaces, m_localpart);

            if(theNamespace == 0 || theNamespace->empty() == true)
            {
                throwException(
                    getMemoryManager(),
                    XalanMessages::PrefixIsNotDeclared_1Param,
                    m_localpart,
                    locator);
            }
            else
            {
                m_namespace = *theNamespace;
            }
        }

        m_localpart.assign(qname + indexOfNSSep + 1, len - (indexOfNSSep + 1));
    }
    else
    {
        if (fUseDefault == false)
        {
            m_namespace.clear();
        }
        else
        {
            const XalanDOMString* const     theNamespace = 
                        getNamespaceForPrefix(namespaces, s_emptyString);

            if(theNamespace != 0 && theNamespace->empty() == false)
            {
                m_namespace = *theNamespace;
            }
        }

        m_localpart = qname;
    }

    validate(qname, len, locator);
}



void
XalanQNameByValue::resolvePrefix(
            const XalanDOMChar*         qname,
            XalanDOMString::size_type   theLength,
            const PrefixResolver*       theResolver,
            const Locator*              locator)
{
    const XalanDOMString::size_type     indexOfNSSep = indexOf(qname, XalanUnicode::charColon);

    if (indexOfNSSep == 0)
    {
        throwException(
            getMemoryManager(),
            XalanMessages::PrefixOfLengthZeroDetected,
            m_localpart,    // This is a dummy parameter...
            locator);
    }
    else if(indexOfNSSep >= theLength)
    {
        m_localpart.assign(qname, theLength);

        m_namespace.clear();
    }
    else
    {
        // Reserve some space for the local part right now, and
        // use it as a temporary for the prefix.
        m_localpart.reserve(theLength);

        m_localpart.assign(qname, indexOfNSSep);

        if(m_localpart == DOMServices::s_XMLString)
        {
            m_namespace = DOMServices::s_XMLNamespaceURI;
        }
        else if(m_localpart == DOMServices::s_XMLNamespace)
        {
            // The xmlns prefix is not normally bound to a namespace URI, but we're really trying
            // to form a QName, so we'll do what the DOM does...
            m_namespace = DOMServices::s_XMLNamespacePrefixURI;
        }
        else if (theResolver == 0)
        {
            throwException(
                getMemoryManager(),
                XalanMessages::PrefixIsNotDeclared_1Param,
                m_localpart,
                locator);
        }
        else
        {
            const XalanDOMString* const     theNamespace =
                theResolver->getNamespaceForPrefix(m_localpart);

            if (theNamespace != 0)
            {
                m_namespace = *theNamespace;
            }
        }  

        if(m_namespace.empty() == true)
        {
            throwException(
                getMemoryManager(),
                XalanMessages::PrefixIsNotDeclared_1Param,
                m_localpart,
                locator);
        }

        m_localpart.assign(
            qname + indexOfNSSep + 1,
            theLength - (indexOfNSSep + 1));
    }

    validate(qname, theLength, locator);
}



void
XalanQNameByValue::validate(
            const XalanDOMChar*         theQName,
            XalanDOMString::size_type   theLength,
            const Locator*              theLocator)
{
    if (isValid() == false)
    {
        throwException(
            getMemoryManager(),
            theQName,
            theLength,
            theLocator);
    }
}


    
XALAN_CPP_NAMESPACE_END
