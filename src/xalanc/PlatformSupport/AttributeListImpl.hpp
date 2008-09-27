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
#if !defined(ATTRIBUTELISTIMPL_HEADER_GUARD_1357924680)
#define ATTRIBUTELISTIMPL_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



#include <xercesc/sax/AttributeList.hpp>




XALAN_CPP_NAMESPACE_BEGIN



class AttributeVectorEntry;



typedef XERCES_CPP_NAMESPACE_QUALIFIER AttributeList    AttributeListType;



class XALAN_PLATFORMSUPPORT_EXPORT AttributeListImpl : public AttributeListType
{
public:

    explicit
    AttributeListImpl(MemoryManager&      theManager);

    virtual
    ~AttributeListImpl();

    AttributeListImpl(const AttributeListImpl&  theSource,
                        MemoryManager&      theManager);

    AttributeListImpl(const AttributeListType&  theSource,
                        MemoryManager&      theManager);

    AttributeListImpl&
    operator=(const AttributeListImpl&  theRHS);

    AttributeListImpl&
    operator=(const AttributeListType&  theRHS);

    MemoryManager&
    getMemoryManager()
    {
        return m_AttributeVector.getMemoryManager();
    }

    // These are inherited from AttributeList
    virtual XalanSize_t
    getLength() const;

    virtual const XMLCh*
    getName(const XalanSize_t index) const;

    virtual const XMLCh*
    getType(const XalanSize_t index) const;

    virtual const XMLCh*
    getValue(const XalanSize_t index) const;

    virtual const XMLCh*
    getType(const XMLCh* const name) const;

    virtual const XMLCh*
    getValue(const XMLCh* const name) const;

    virtual const XMLCh*
    getValue(const char* const name) const;
    // The mutators are new to this class.

    /**
     * Remove all attributes from the list
     */
    virtual void
    clear();

    /**
     * Adds an attribute to the attribute list
     *
     * @param  name   attribute name
     * @param type   attribute type, "CDATA," for example
     * @param value  attribute value
     */
    virtual bool
    addAttribute(
            const XMLCh*    name,
            const XMLCh*    type,
            const XMLCh*    value);

    /**
     * Removes an attribute from the attribute list
     *
     * @param  name   attribute name
     */
    virtual bool
    removeAttribute(const XMLCh*    name);

    /**
     * Swap the contents of two instances.  This must _never_
     * throw an exception.
     *
     * @param thOther The instance with which to swap.
     */
    void
    swap(AttributeListImpl&     theOther)
    {
        m_AttributeVector.swap(theOther.m_AttributeVector);
    }

    /**
     * Reserve room for the given number of
     * attributes.
     *
     * @param theCount The number to reserve
     */
    void
    reserve(XalanSize_t     theCount)
    {
        m_AttributeVector.reserve(theCount);
    }

    // This vector will hold the entries.
    typedef XalanVector<AttributeVectorEntry*>          AttributeVectorType;

private:

    // This is not implemented.
    bool
    operator==(const AttributeListImpl&) const;

    // Default vector allocation size.
    enum
    {
        eDefaultVectorSize = 5
    };

    AttributeVectorEntry*
    getNewEntry(
            const XMLCh*    name,
            const XMLCh*    type,
            const XMLCh*    value);

    // Helper function to delete entries...
    static void
    deleteEntries(AttributeVectorType&  theVector);

    AttributeVectorType     m_AttributeVector;

    AttributeVectorType     m_cacheVector;
};



XALAN_CPP_NAMESPACE_END



#endif  // ATTRIBUTELISTIMPL_HEADER_GUARD_1357924680
