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

#include "NodeSortKey.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString     s_emptyString(XalanMemMgrs::getDummyMemMgr());



NodeSortKey::NodeSortKey(
            ExecutionContext&                   executionContext,
            const XPath*                        selectPat,
            bool                                treatAsNumbers,
            bool                                descending,
            XalanCollationServices::eCaseOrder  caseOrder,
            const XalanDOMString&               langValue,
            const PrefixResolver&               resolver) :
    m_executionContext(&executionContext),
    m_selectPat(selectPat),
    m_treatAsNumbers(treatAsNumbers),
    m_descending(descending),
    m_caseOrder(caseOrder),
    m_prefixResolver(&resolver),
    m_languageString(&langValue)
{
}



NodeSortKey::NodeSortKey() :
    m_executionContext(0),
    m_selectPat(0),
    m_treatAsNumbers(false),
    m_descending(false),
    m_caseOrder(XalanCollationServices::eDefault),
    m_prefixResolver(0),
    m_languageString(&s_emptyString)
{
}



NodeSortKey::NodeSortKey(const NodeSortKey&     theSource) :
    m_executionContext(theSource.m_executionContext),
    m_selectPat(theSource.m_selectPat),
    m_treatAsNumbers(theSource.m_treatAsNumbers),
    m_descending(theSource.m_descending),
    m_caseOrder(theSource.m_caseOrder),
    m_prefixResolver(theSource.m_prefixResolver),
    m_languageString(theSource.m_languageString)
{
}



NodeSortKey::~NodeSortKey()
{
}



NodeSortKey&
NodeSortKey::operator=(const NodeSortKey&   theRHS)
{
    if (this != &theRHS)
    {
        m_executionContext = theRHS.m_executionContext;
        m_selectPat = theRHS.m_selectPat;
        m_treatAsNumbers = theRHS.m_treatAsNumbers;
        m_descending = theRHS.m_descending;
        m_caseOrder = theRHS.m_caseOrder;
        m_prefixResolver = theRHS.m_prefixResolver;
        m_languageString = theRHS.m_languageString;

        assert(m_languageString != 0);
    }

    return *this;
}



XALAN_CPP_NAMESPACE_END
