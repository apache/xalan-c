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
#if !defined(NAMEDNODEMAPATTRIBUTELIST_HEADER_GUARD_1357924680)
#define NAMEDNODEMAPATTRIBUTELIST_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xercesc/sax/AttributeList.hpp>

#include <xalanc/Include/XalanMemoryManagement.hpp>


XALAN_CPP_NAMESPACE_BEGIN



class XalanNamedNodeMap;



class XALAN_PLATFORMSUPPORT_EXPORT NamedNodeMapAttributeList : public XERCES_CPP_NAMESPACE_QUALIFIER AttributeList
{
public:

    typedef XERCES_CPP_NAMESPACE_QUALIFIER AttributeList    ParentType;

    explicit
    NamedNodeMapAttributeList(
                const XalanNamedNodeMap&    theMap,
                MemoryManager&          theManager);

    virtual
    ~NamedNodeMapAttributeList();

    // These are inherited from AttributeList
    virtual XalanSize_t
    getLength() const;

    virtual const XalanDOMChar*
    getName(const XalanSize_t   index) const;

    virtual const XalanDOMChar*
    getType(const XalanSize_t   index) const;

    virtual const XalanDOMChar*
    getValue(const XalanSize_t  index) const;

    virtual const XalanDOMChar*
    getType(const XalanDOMChar* const name) const;

    virtual const XalanDOMChar*
    getValue(const XalanDOMChar*     const name) const;

    virtual const XalanDOMChar* 
    getValue(const char* const name) const;

private:

    // Not implemented...
    NamedNodeMapAttributeList&
    operator=(const NamedNodeMapAttributeList&);

    bool
    operator==(const NamedNodeMapAttributeList&);

    // Data members...
    const XalanNamedNodeMap&    m_nodeMap;

    const XalanSize_t           m_length;

    MemoryManager&          m_memoryManager;

    static const XalanDOMChar   s_typeString[];
};



XALAN_CPP_NAMESPACE_END



#endif  // NAMEDNODEMAPATTRIBUTELIST_HEADER_GUARD_1357924680
