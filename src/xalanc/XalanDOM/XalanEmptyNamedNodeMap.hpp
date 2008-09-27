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
#if !defined(XALANEMPTYNAMEDNODEMAP_HEADER_GUARD_1357924680)
#define XALANEMPTYNAMEDNODEMAP_HEADER_GUARD_1357924680



#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_DOM_EXPORT XalanEmptyNamedNodeMap : public XalanNamedNodeMap
{
public:

    XalanEmptyNamedNodeMap();

    virtual
    ~XalanEmptyNamedNodeMap();

    XalanEmptyNamedNodeMap(const XalanEmptyNamedNodeMap&    theSource);

    XalanEmptyNamedNodeMap&
    operator=(const XalanEmptyNamedNodeMap&     theSource);

    bool
    operator==(const XalanEmptyNamedNodeMap&    theRHS) const;


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
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XALANEMPTYNAMEDNODEMAP_HEADER_GUARD_1357924680)
