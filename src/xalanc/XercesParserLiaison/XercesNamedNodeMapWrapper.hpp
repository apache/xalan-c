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
#if !defined(XERCESNAMEDNODEMAPWRAPPER_HEADER_GUARD_1357924680)
#define XERCESNAMEDNODEMAPWRAPPER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XercesWrapperNavigator;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesNamedNodeMapWrapper : public XalanNamedNodeMap
{
public:

    XercesNamedNodeMapWrapper(
            const DOMNamedNodeMapType*      theXercesNamedNodeMap,
            const XercesWrapperNavigator&   theNavigator);

    virtual
    ~XercesNamedNodeMapWrapper();

    // These interfaces are inherited from XalanNamedNodeMap...

    virtual XalanNode*
    item(XalanSize_t    index) const;

    virtual XalanNode*
    getNamedItem(const XalanDOMString&  name) const;

    virtual XalanSize_t
    getLength() const;

    virtual XalanNode*
    getNamedItemNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName) const;

private:

    // Not implemented...
    XercesNamedNodeMapWrapper(const XercesNamedNodeMapWrapper&  theSource);

    XercesNamedNodeMapWrapper&
    operator=(const XercesNamedNodeMapWrapper&  theRHS);

    bool
    operator==(const XercesNamedNodeMapWrapper& theRHS) const;


    const DOMNamedNodeMapType* const    m_xercesNamedNodeMap;

    const XercesWrapperNavigator&       m_navigator;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESNAMEDNODEMAPWRAPPER_HEADER_GUARD_1357924680)
