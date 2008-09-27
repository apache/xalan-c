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

#if !defined(ICUBRIDGE_COLLATIONCOMPAREFUNCTOR_GUARD_1357924680)
#define ICUBRIDGE_COLLATIONCOMPAREFUNCTOR_GUARD_1357924680



#include <xalanc/ICUBridge/ICUBridgeDefinitions.hpp>



#include <xalanc/PlatformSupport/XalanCollationServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class ICUBridgeCollationCompareFunctorImpl;



class XALAN_ICUBRIDGE_EXPORT ICUBridgeCollationCompareFunctor : public XalanCollationServices::CollationCompareFunctor
{
public:

    /**
     * Constructor.
     * 
     * @param fCacheCollators If true, the instance will cache collators.  This is not thread-safe, so each thread must have its own instance.
     */
    ICUBridgeCollationCompareFunctor(MemoryManager& theManager, 
                                                bool    fCacheCollators = false);

    static ICUBridgeCollationCompareFunctor*
    create(MemoryManager& theManager, bool  fCacheCollators = false);

    virtual
    ~ICUBridgeCollationCompareFunctor();

    virtual int
    operator()(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault) const;

    virtual int
    operator()(
            const XalanDOMChar*     theLHS,
            const XalanDOMChar*     theRHS,
            const XalanDOMChar*     theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault) const;
private:

    ICUBridgeCollationCompareFunctorImpl* const     m_impl;
};



XALAN_CPP_NAMESPACE_END



#endif  // ICUBRIDGE_COLLATIONCOMPAREFUNCTOR_GUARD_1357924680
