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
#include "AttributeListImpl.hpp"



#include <algorithm>
#include <cassert>



#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>
#include <xalanc/Include/STLHelper.hpp>



#include "AttributeVectorEntry.hpp"



XALAN_CPP_NAMESPACE_BEGIN



AttributeListImpl::AttributeListImpl(MemoryManager&      theManager) :
    AttributeListType(),
    m_AttributeVector(theManager),
    m_cacheVector(theManager)
{
}



AttributeListImpl::~AttributeListImpl()
{
    // Clean up everything...
    clear();

    assert(m_AttributeVector.empty() == true);

    deleteEntries(m_cacheVector);
}



AttributeListImpl::AttributeListImpl(const AttributeListImpl&   theSource,
                                     MemoryManager&      theManager) :
    AttributeListType(),
    m_AttributeVector(theManager),
    m_cacheVector(theManager)
{
    // Use the assignment operator to do the dirty work...
    *this = theSource;

    assert(getLength() == theSource.getLength());
}



AttributeListImpl::AttributeListImpl(const AttributeListType&   theSource,
                                     MemoryManager&      theManager) :
    AttributeListType(),
    m_AttributeVector(theManager),
        m_cacheVector(theManager)

{
    // Use the assignment operator to do the dirty work...
    *this = theSource;

    assert(getLength() == theSource.getLength());
}



void
AttributeListImpl::deleteEntries(AttributeVectorType&   theVector)
{
    // Delete all of the objects in the vector.
    XALAN_STD_QUALIFIER for_each(
        theVector.begin(),
        theVector.end(),
        DeleteFunctor<AttributeVectorEntry>(theVector.getMemoryManager()));
}



AttributeListImpl&
AttributeListImpl::operator=(const AttributeListImpl&   theRHS)
{
    if (this != &theRHS)
    {
        // Note that we can't chain up to our base class operator=()
        // because it's private.

        // Some temporary structures to hold everything
        // until we're done.
        AttributeVectorType     tempVector(getMemoryManager());

        const XalanSize_t   theLength = theRHS.getLength();

        if (theLength > 0)
        {
            // Reserve the appropriate capacity right now...
            tempVector.reserve(theLength);

            // This will delete everything in tempVector when we're done...
            CollectionDeleteGuard<AttributeVectorType,
                                  DeleteFunctor<AttributeVectorEntry> >     theGuard(tempVector);

            typedef AttributeVectorType::const_iterator     const_iterator;

            const const_iterator    theEnd = theRHS.m_AttributeVector.end();

            // Copy the vector entries, and build the index map...
            for(const_iterator i = theRHS.m_AttributeVector.begin(); i != theEnd; ++i)
            {
                AttributeVectorEntry* const     theEntry = *i;

                assert(theEntry != 0);

                // Add the item...
                tempVector.push_back(
                    getNewEntry(
                        &*theEntry->m_Name.begin(),
                        &*theEntry->m_Type.begin(),
                        &*theEntry->m_Value.begin()));
            }

            // OK, we're safe, so swap the contents of the
            // containers.  This is guaranteed not to throw.
            m_AttributeVector.swap(tempVector);
        }

        assert(getLength() == theLength);
    }

    return *this;
}



AttributeListImpl&
AttributeListImpl::operator=(const AttributeListType&   theRHS)
{
    if (this != &theRHS)
    {
        // Note that we can't chain up to our base class operator=()
        // because it's private.

        // Add all of the attributes to this temp list,
        // then swap at the end.  This means we're exception
        // safe and don't need any try blocks.
        AttributeListImpl   theTempList(getMemoryManager());

        const XalanSize_t   theLength = theRHS.getLength();

        theTempList.reserve(theLength);

        // Add each attribute.
        for(XalanSize_t i = 0; i < theLength; i++)
        {
            theTempList.addAttribute(
                    theRHS.getName(i),
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
AttributeListImpl::getLength() const
{
    // Make sure the mismatch between Xerces and vector<> doesn't cause a problem...
    assert(m_AttributeVector.size() == unsigned(m_AttributeVector.size()));

    return unsigned(m_AttributeVector.size());
}



const XMLCh*
AttributeListImpl::getName(const XalanSize_t    index) const
{
    assert(index < getLength());

    return &*m_AttributeVector[index]->m_Name.begin();
}



const XMLCh*
AttributeListImpl::getType(const XalanSize_t    index) const
{
    assert(index < getLength());

    return &*m_AttributeVector[index]->m_Type.begin();
}



const XMLCh*
AttributeListImpl::getValue(const XalanSize_t   index) const
{
    assert(index < getLength());

    return &*m_AttributeVector[index]->m_Value.begin();
}



const XMLCh*
AttributeListImpl:: getValue(const char* const /*name*/) const
{
    assert(0);

    return 0;
}



struct NameCompareFunctor
{
    NameCompareFunctor(const XMLCh*     theName) :
        m_name(theName)
    {
    }

    bool
    operator()(const AttributeVectorEntry*  theEntry) const
    {
        return equals(&*theEntry->m_Name.begin(), m_name);
    }

private:

    const XMLCh* const  m_name;
};



const XMLCh*
AttributeListImpl::getType(const XMLCh* const name) const
{
    assert(name != 0);

    const AttributeVectorType::const_iterator   i =
        XALAN_STD_QUALIFIER find_if(
            m_AttributeVector.begin(),
            m_AttributeVector.end(),
            NameCompareFunctor(name));

    if (i != m_AttributeVector.end())
    {
        // Found it, so return a pointer to the type.
        return &*(*i)->m_Type.begin();
    }
    else
    {
        return 0;
    }
}


const XMLCh*
AttributeListImpl::getValue(const XMLCh* const name) const
{
    assert(name != 0);

    const AttributeVectorType::const_iterator   i =
        XALAN_STD_QUALIFIER find_if(
            m_AttributeVector.begin(),
            m_AttributeVector.end(),
            NameCompareFunctor(name));

    if (i != m_AttributeVector.end())
    {
        // Found it, so return a pointer to the value.
        return &*(*i)->m_Value.begin();
    }
    else
    {
        return 0;
    }
}



void
AttributeListImpl::clear()
{
    m_cacheVector.insert(m_cacheVector.end(), m_AttributeVector.begin(), m_AttributeVector.end());

    // Clear everything out.
    m_AttributeVector.clear();
}



bool
AttributeListImpl::addAttribute(
            const XMLCh*    name,
            const XMLCh*    type,
            const XMLCh*    value)
{
    assert(name != 0);
    assert(type != 0);
    assert(value != 0);

    bool    fResult = false;

    XALAN_USING_STD(find_if)
    XALAN_USING_STD(copy)

    typedef AttributeVectorEntry::XMLChVectorType   XMLChVectorType;

    // Update the attribute, if it's already there...
    const AttributeVectorType::const_iterator   i =
        find_if(
            m_AttributeVector.begin(),
            m_AttributeVector.end(),
            NameCompareFunctor(name));

    if (i != m_AttributeVector.end())
    {
        // This is a special optimization for type, since it's (almost) always "CDATA".
        if (equals(type, &*(*i)->m_Type.begin()) == false)
        {
            // If necessary, create the a new vector and swap them.  Otherwise,
            // just copy the new data in.
            const XMLCh* const  theNewTypeEnd = AttributeVectorEntry::endArray(type) + 1;

            if ((*i)->m_Type.capacity() < XMLChVectorType::size_type(theNewTypeEnd - type))
            {
                XMLChVectorType     theNewType(type, theNewTypeEnd, getMemoryManager());

                theNewType.swap((*i)->m_Type);
            }
            else
            {
                copy(type, theNewTypeEnd, (*i)->m_Type.begin());
            }
        }

        const XMLCh* const  theNewValueEnd = AttributeVectorEntry::endArray(value) + 1;

        const XMLChVectorType::size_type    theNewSize =
                        XMLChVectorType::size_type(theNewValueEnd - value);

        // If necessary, create the a new vector and swap them.  Otherwise,
        // just copy the new data in.
        if ((*i)->m_Value.capacity() < theNewSize)
        {
            XMLChVectorType     theNewValue(value, theNewValueEnd, getMemoryManager());

            theNewValue.swap((*i)->m_Value); 
        }
        else
        {
            (*i)->m_Value.resize(theNewSize);

            copy(value, theNewValueEnd, (*i)->m_Value.begin());
        }
    }
    else
    {
        if (m_AttributeVector.capacity() == 0)
        {
            m_AttributeVector.reserve(eDefaultVectorSize);
        }
        
        typedef XalanMemMgrAutoPtr<AttributeVectorEntry>    AutoPtr;

        AutoPtr theEntry(getMemoryManager(), getNewEntry(name, type, value));

        // Add the new one.
        m_AttributeVector.push_back(theEntry.get());


        theEntry.release();

        fResult = true;
    }

    return fResult;
}



AttributeVectorEntry*
AttributeListImpl::getNewEntry(
            const XMLCh*    name,
            const XMLCh*    type,
            const XMLCh*    value)
{
    if (m_cacheVector.empty() == true)
    {
        return AttributeVectorEntry::create(name, value, type,getMemoryManager());
    }
    else
    {
        AttributeVectorEntry* const     theEntry =
            m_cacheVector.back();

        theEntry->clear();

        assert(theEntry->m_Name.empty() == true && theEntry->m_Value.empty() == true && theEntry->m_Type.empty() == true);

        theEntry->m_Name.insert(theEntry->m_Name.begin(), name, AttributeVectorEntry::endArray(name) + 1);
        theEntry->m_Value.insert(theEntry->m_Value.begin(), value, AttributeVectorEntry::endArray(value) + 1);
        theEntry->m_Type.insert(theEntry->m_Type.begin(), type, AttributeVectorEntry::endArray(type) + 1);

        m_cacheVector.pop_back();

        return theEntry;
    }
}



bool
AttributeListImpl::removeAttribute(const XMLCh*     name)
{
    assert(name != 0);

    bool    fResult = false;

    // Update the attribute, if it's already there...
    const AttributeVectorType::iterator     i =
        XALAN_STD_QUALIFIER find_if(
            m_AttributeVector.begin(),
            m_AttributeVector.end(),
            NameCompareFunctor(name));

    if (i != m_AttributeVector.end())
    {
        m_cacheVector.push_back(*i);

        m_AttributeVector.erase(i);

        fResult = true;
    }

    return fResult;
}



XALAN_CPP_NAMESPACE_END
