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
#if !defined(XALANSOURCETREEDOMSUPPORT_HEADER_GUARD_1357924680)
#define XALANSOURCETREEDOMSUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/DOMSupport/DOMSupport.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanSourceTreeParserLiaison;



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeDOMSupport : public DOMSupport
{
public:

    XalanSourceTreeDOMSupport();

    XalanSourceTreeDOMSupport(const XalanSourceTreeParserLiaison&   theParserLiaison);

    virtual
    ~XalanSourceTreeDOMSupport();

    virtual void
    reset();

    // These interfaces are inherited from DOMSupport...

    virtual const XalanDOMString&
    getUnparsedEntityURI(
            const XalanDOMString&   theName,
            const XalanDocument&    theDocument) const;

    virtual bool
    isNodeAfter(
            const XalanNode&    node1,
            const XalanNode&    node2) const;

    const XalanSourceTreeParserLiaison*
    getParserLiaison() const
    {
        return m_parserLiaison;
    }

    void
    setParserLiaison(const XalanSourceTreeParserLiaison*    theParserLiaison)
    {
        m_parserLiaison = theParserLiaison;
    }

private:

    const XalanSourceTreeParserLiaison*     m_parserLiaison;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANSOURCETREEDOMSUPPORT_HEADER_GUARD_1357924680
