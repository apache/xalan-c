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
// Class header file.
#include "AttributesImpl.hpp"



#include <algorithm>
#include <cassert>



#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>
#include <xalanc/Include/STLHelper.hpp>



#include "AttributeVectorEntryExtended.hpp"



XALAN_CPP_NAMESPACE_BEGIN



AttributesImpl::AttributesImpl(MemoryManager&      theManager) :
    AttributesType(),
    m_attributesVector(theManager),
    m_cacheVector(theManager)
{
}



AttributesImpl::~AttributesImpl()
{
    // Clean up everything...
    clear();

    assert(m_attributesVector.empty() == true);

    deleteEntries(m_cacheVector);
}



AttributesImpl::AttributesImpl(
            const AttributesImpl&   theSource,
            MemoryManager&          theManager) :
    AttributesType(),
    m_attributesVector(theManager),
    m_cacheVector(theManager)
{
    // Use the assignment operator to do the dirty work...
    *this = theSource;

    assert(getLength() == theSource.getLength());
}



AttributesImpl::AttributesImpl(const AttributesType&    theSource,
                               MemoryManager&      theManager) :
    AttributesType(),
    m_attributesVector(theManager),
    m_cacheVector(theManager)
{
    // Use the assignment operator to do the dirty work...
    *this = theSource;

    assert(getLength() == theSource.getLength());
}



void
AttributesImpl::deleteEntries(AttributesVectorType&     theVector)
{
    // Delete all of the objects in the vector.
    XALAN_STD_QUALIFIER for_each(
            theVector.begin(),
            theVector.end(),
            DeleteFunctor<AttributeVectorEntryExtended>(theVector.getMemoryManager()));
}



AttributesImpl&
AttributesImpl::operator=(const AttributesImpl&     theRHS)
{
    if (this != &theRHS)
    {
        // Note that we can't chain up to our base class operator=()
        // because it's private.

        // Some temporary structures to hold everything
        // until we're done.
        AttributesVectorType        tempVector(getMemoryManager());

        const XalanSize_t   theLength = theRHS.getLength();

        if (theLength > 0)
        {
            // Reserve the appropriate capacity right now...
            tempVector.reserve(theLength);

            // This will delete everything in tempVector when we're done...
            CollectionDeleteGuard<AttributesVectorType,
                                  DeleteFunctor<AttributeVectorEntryExtended> >     theGuard(tempVector);

            typedef AttributesVectorType::const_iterator        const_iterator;

            const const_iterator    theEnd = theRHS.m_attributesVector.end();

            // Copy the vector entries, and build the index map...
            for(const_iterator i = theRHS.m_attributesVector.begin(); i != theEnd; ++i)
            {
                AttributeVectorEntryExtended* const     theEntry = *i;

                assert(theEntry != 0);

                // Add the item...
                tempVector.push_back(
                    getNewEntry(
                        &*theEntry->m_Name.begin(),
                        &*theEntry->m_Type.begin(),
                        &*theEntry->m_Value.begin(),
                        &*theEntry->m_uri.begin(),
                        &*theEntry->m_localName.begin()));
            }

            // OK, we're safe, so swap the contents of the
            // containers.  This is guaranteed not to throw.
            m_attributesVector.swap(tempVector);
        }

        assert(getLength() == theLength);
    }

    return *this;
}



AttributesImpl&
AttributesImpl::operator=(const AttributesType&     theRHS)
{
    if (this != &theRHS)
    {
        // Note that we can't chain up to our base class operator=()
        // because it's private.

        // Add all of the attributes to this temp list,
        // then swap at the end.  This means we're exception
        // safe and don't need any try blocks.
        AttributesImpl  theTempList(getMemoryManager());

        const XalanSize_t   theLength = theRHS.getLength();

        theTempList.reserve(theLength);

        // Add each attribute.
        for(XalanSize_t i = 0; i < theLength; i++)
        {
            theTempList.addAttribute(
                    theRHS.getURI(i),
                    theRHS.getLocalName(i),
                    theRHS.getQName(i),
                    theRHS.getType(i),
                    theRHS.getValue(i));
        }

        // Now that the temp list is built, swap everything. This is
        // guaranteed not to throw.
        swap(theTempList);
    }

    return *this;
}



XalanSize_t
AttributesImpl::getLength() const
{
    return unsigned(m_attributesVector.size());
}



const XMLCh*
AttributesImpl::getURI(const XalanSize_t    index) const
{
    assert(index < getLength());

    return &*m_attributesVector[index]->m_uri.begin();
}



const XMLCh*
AttributesImpl::getLocalName(const XalanSize_t  index) const
{
    assert(index < getLength());

    return &*m_attributesVector[index]->m_localName.begin();
}



const XMLCh*
AttributesImpl::getQName(const XalanSize_t  index) const
{
    assert(index < getLength());

    return &*m_attributesVector[index]->m_Name.begin();
}



const XMLCh*
AttributesImpl::getType(const XalanSize_t   index) const
{
    assert(index < getLength());

    return &*m_attributesVector[index]->m_Type.begin();
}



const XMLCh*
AttributesImpl::getValue(const XalanSize_t  index) const
{
    assert(index < getLength());

    return &*m_attributesVector[index]->m_Value.begin();
}



struct NameCompareFunctor
{
    NameCompareFunctor(const XMLCh*     theQName) :
        m_qname(theQName)
    {
    }

    bool
    operator()(const AttributeVectorEntryExtended*  theEntry) const
    {
        return equals(&*theEntry->m_Name.begin(), m_qname);
    }

private:

    const XMLCh* const  m_qname;
};



struct URIAndLocalNameCompareFunctor
{
    URIAndLocalNameCompareFunctor(
            const XMLCh*    theURI,
            const XMLCh*    theLocalName) :
        m_uri(theURI),
        m_localName(theLocalName)
    {
    }

    bool
    operator()(const AttributeVectorEntryExtended*  theEntry) const
    {
        return equals(&*theEntry->m_uri.begin(), m_uri) && equals(&*theEntry->m_localName.begin(), m_localName) ;
    }

private:

    const XMLCh* const  m_uri;
    const XMLCh* const  m_localName;
};



const XMLCh*
AttributesImpl::getType(const XMLCh* const  qname) const
{
    const int   theIndex = getIndex(qname);

    if (theIndex == -1)
    {
        return 0;
    }
    else
    {
        return getType(theIndex);
    }
}



const XMLCh*
AttributesImpl::getValue(const XMLCh* const     qname) const
{
    const int   theIndex = getIndex(qname);

    if (theIndex == -1)
    {
        return 0;
    }
    else
    {
        return getValue(theIndex);
    }
}



const XMLCh*
AttributesImpl::getType(
            const XMLCh* const  uri,
            const XMLCh* const  localName) const
{
    const int   theIndex = getIndex(uri, localName);

    if (theIndex == -1)
    {
        return 0;
    }
    else
    {
        return getType(theIndex);
    }
}



const XMLCh*
AttributesImpl::getValue(
            const XMLCh* const  uri,
            const XMLCh* const  localName) const
{
    const int   theIndex = getIndex(uri, localName);

    if (theIndex == -1)
    {
        return 0;
    }
    else
    {
        return getValue(theIndex);
    }
}



bool
AttributesImpl::getIndex(
            const XMLCh* const  uri,
            const XMLCh* const  localPart,
            XalanSize_t&        index) const
{
    const int   tempIndex =
        getIndex(uri, localPart);

    if (tempIndex == -1)
    {
        return false;
    }
    else
    {
        index = tempIndex;

        return true;
    }
}



int
AttributesImpl::getIndex(
            const XMLCh* const  uri,
            const XMLCh* const  localPart) const
{
    assert(uri != 0 && localPart != 0);

    const AttributesVectorType::const_iterator  i =
        XALAN_STD_QUALIFIER find_if(
            m_attributesVector.begin(),
            m_attributesVector.end(),
            URIAndLocalNameCompareFunctor(uri, localPart));

    if (i != m_attributesVector.end())
    {
        // Found it, so return the index, which is the difference between
        // begin() and i.
        return int(i - m_attributesVector.begin());
    }
    else
    {
        return -1;
    }
}



bool
AttributesImpl::getIndex(
            const XMLCh* const  qName,
            XalanSize_t&        index) const
{
    const int   tempIndex =
        getIndex(qName);

    if (tempIndex == -1)
    {
        return false;
    }
    else
    {
        index = tempIndex;

        return true;
    }
}



int
AttributesImpl::getIndex(const XMLCh* const     qname) const
{
    assert(qname != 0);

    const AttributesVectorType::const_iterator  i =
        XALAN_STD_QUALIFIER find_if(
            m_attributesVector.begin(),
            m_attributesVector.end(),
            NameCompareFunctor(qname));

    if (i != m_attributesVector.end())
    {
        // Found it, so return the index, which is the difference between
        // begin() and i.
        return int(i - m_attributesVector.begin());
    }
    else
    {
        return -1;
    }
}



void
AttributesImpl::clear()
{
    m_cacheVector.insert(m_cacheVector.end(), m_attributesVector.begin(), m_attributesVector.end());

    // Clear everything out.
    m_attributesVector.clear();
}



void
AttributesImpl::addAttribute(
            const XMLCh*    uri,
            const XMLCh*    localName,
            const XMLCh*    name,
            const XMLCh*    type,
            const XMLCh*    value)
{
    assert(name != 0);
    assert(type != 0);
    assert(value != 0);

    typedef AttributeVectorEntry::XMLChVectorType   XMLChVectorType;

    if (m_attributesVector.capacity() == 0)
    {
        m_attributesVector.reserve(eDefaultVectorSize);
    }

    typedef XalanMemMgrAutoPtr<AttributeVectorEntryExtended>    AutoPtr;

    AutoPtr theEntry(getMemoryManager(), getNewEntry(name, type, value, uri, localName));

    // Add the new one.
    m_attributesVector.push_back(theEntry.get());

    theEntry.release();
}



AttributeVectorEntryExtended*
AttributesImpl::getNewEntry(
            const XMLCh*    name,
            const XMLCh*    type,
            const XMLCh*    value,
            const XMLCh*    uri,
            const XMLCh*    localName)
{
    assert(name != 0);
    assert(type != 0);
    assert(value != 0);
    assert(uri != 0);
    assert(localName != 0);

    if (m_cacheVector.empty() == true)
    {
        return AttributeVectorEntryExtended::create(name, value, type, uri, localName,getMemoryManager());
    }
    else
    {
        AttributeVectorEntryExtended* const     theEntry =
            m_cacheVector.back();

        theEntry->clear();

        assert(theEntry->m_Name.empty() == true && theEntry->m_Value.empty() == true &&
               theEntry->m_Type.empty() == true && theEntry->m_uri.empty() == true &&
               theEntry->m_localName.empty() == true);

        theEntry->m_Name.insert(theEntry->m_Name.begin(), name, AttributeVectorEntry::endArray(name) + 1);
        theEntry->m_Value.insert(theEntry->m_Value.begin(), value, AttributeVectorEntry::endArray(value) + 1);
        theEntry->m_Type.insert(theEntry->m_Type.begin(), type, AttributeVectorEntry::endArray(type) + 1);
        theEntry->m_uri.insert(theEntry->m_uri.begin(), uri, AttributeVectorEntry::endArray(uri) + 1);
        theEntry->m_localName.insert(theEntry->m_localName.begin(), localName, AttributeVectorEntry::endArray(localName) + 1);

        m_cacheVector.pop_back();

        return theEntry;
    }
}



bool
AttributesImpl::removeAttribute(const XMLCh*        name)
{
    assert(name != 0);

    bool    fResult = false;

    // Update the attribute, if it's already there...
    const AttributesVectorType::iterator        i =
        XALAN_STD_QUALIFIER find_if(
            m_attributesVector.begin(),
            m_attributesVector.end(),
            NameCompareFunctor(name));

    if (i != m_attributesVector.end())
    {
        m_cacheVector.push_back(*i);

        m_attributesVector.erase(i);

        fResult = true;
    }

    return fResult;
}



XALAN_CPP_NAMESPACE_END
