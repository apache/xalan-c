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
#if !defined(XERCESLIAISONXALANDOMSTRINGPOOL_HEADER_GUARD_1357924680)
#define XERCESLIAISONXALANDOMSTRINGPOOL_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xalanc/PlatformSupport/XalanDOMStringPool.hpp>



#include <xercesc/util/Mutexes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



// A version of XalanDOMStringPool which is synchronized using a Xerces Mutex.
class XALAN_XERCESPARSERLIAISON_EXPORT XercesLiaisonXalanDOMStringPool : public XalanDOMStringPool
{
public:

    typedef XalanDOMStringPool  ParentType;
    typedef XERCES_CPP_NAMESPACE_QUALIFIER XMLMutex         XMLMutexType;
    typedef XERCES_CPP_NAMESPACE_QUALIFIER XMLMutexLock     XMLMutexLockType;

    explicit
    XercesLiaisonXalanDOMStringPool(MemoryManager& theManager);

    static XercesLiaisonXalanDOMStringPool*
    create(MemoryManager& theManager);

    virtual
    ~XercesLiaisonXalanDOMStringPool();

    // These are inherited from XalanDOMStringPool...

    virtual void
    clear();

    virtual size_t
    size() const;

    virtual const XalanDOMString&
    get(const XalanDOMString&   theString);

    virtual const XalanDOMString&
    get(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength = XalanDOMString::npos);

private:

    // Not implemented, for now...
    XercesLiaisonXalanDOMStringPool(const XercesLiaisonXalanDOMStringPool&);

    XercesLiaisonXalanDOMStringPool&
    operator=(const XercesLiaisonXalanDOMStringPool&);

    bool
    operator==(const XercesLiaisonXalanDOMStringPool&) const;

    // Data members...
    mutable XMLMutexType    m_mutex;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESLIAISONXALANDOMSTRINGPOOL_HEADER_GUARD_1357924680)
