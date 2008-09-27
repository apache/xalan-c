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
#if !defined(XERCESNODELISTWRAPPER_HEADER_GUARD_1357924680)
#define XERCESNODELISTWRAPPER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xalanc/XalanDOM/XalanNodeList.hpp>



#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XercesWrapperNavigator;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesNodeListWrapper : public XalanNodeList
{
public:

    XercesNodeListWrapper(
            const DOMNodeListType*          theXercesNodeList,
            const XercesWrapperNavigator&   theNavigator);

    XercesNodeListWrapper(const XercesNodeListWrapper&  theSource);

    virtual
    ~XercesNodeListWrapper();

    bool
    operator==(const XercesNodeListWrapper&     theRHS) const
    {
        return m_xercesNodeList == theRHS.m_xercesNodeList ? true : false;
    }

    // These interfaces are inherited from XalanNodeList...

    virtual XalanNode*
    item(XalanSize_t    index) const;

    virtual XalanSize_t
    getLength() const;

private:

    // Not implemented...
    XercesNodeListWrapper&
    operator=(const XercesNodeListWrapper&  theRHS);


    // Data members...
    const DOMNodeListType*          m_xercesNodeList;

    const XercesWrapperNavigator&   m_navigator;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESNODELISTWRAPPER_HEADER_GUARD_1357924680)
