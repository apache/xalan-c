/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
// Class header file.
#include "AttributeListImpl.hpp"



#include <algorithm>
#include <cassert>



#include <xalanc/Include/XalanAutoPtr.hpp>
#include <xalanc/Include/STLHelper.hpp>



#include "AttributeVectorEntry.hpp"



XALAN_CPP_NAMESPACE_BEGIN



AttributeListImpl::AttributeListImpl() :
	AttributeListType(),
	m_AttributeVector(),
	m_cacheVector()
{
}



AttributeListImpl::~AttributeListImpl()
{
	// Clean up everything...
	clear();

	assert(m_AttributeVector.empty() == true);

	deleteEntries(m_cacheVector);
}



AttributeListImpl::AttributeListImpl(const AttributeListImpl&	theSource) :
	AttributeListType(),
	m_AttributeVector()
{
	// Use the assignment operator to do the dirty work...
	*this = theSource;

	assert(getLength() == theSource.getLength());
}



AttributeListImpl::AttributeListImpl(const AttributeListType&	theSource) :
	AttributeListType(),
	m_AttributeVector()
{
	// Use the assignment operator to do the dirty work...
	*this = theSource;

	assert(getLength() == theSource.getLength());
}



void
AttributeListImpl::deleteEntries(AttributeVectorType&	theVector)
{
	// Delete all of the objects in the vector.
	XALAN_STD_QUALIFIER for_each(
		theVector.begin(),
		theVector.end(),
		DeleteFunctor<AttributeVectorEntry>());
}



AttributeListImpl&
AttributeListImpl::operator=(const AttributeListImpl&	theRHS)
{
	if (this != &theRHS)
	{
		// Note that we can't chain up to our base class operator=()
		// because it's private.

		// Some temporary structures to hold everything
		// until we're done.
		AttributeVectorType		tempVector;

		const unsigned int	theLength = theRHS.getLength();

		if (theLength > 0)
		{
			// Reserve the appropriate capacity right now...
			tempVector.reserve(theLength);

			// This will delete everything in tempVector when we're done...
			CollectionDeleteGuard<AttributeVectorType,
								  DeleteFunctor<AttributeVectorEntry> >		theGuard(tempVector);

			typedef AttributeVectorType::const_iterator		const_iterator;

			const const_iterator	theEnd = theRHS.m_AttributeVector.end();

			// Copy the vector entries, and build the index map...
			for(const_iterator i = theRHS.m_AttributeVector.begin(); i != theEnd; ++i)
			{
				AttributeVectorEntry* const		theEntry = *i;

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
AttributeListImpl::operator=(const AttributeListType&	theRHS)
{
	if (this != &theRHS)
	{
		// Note that we can't chain up to our base class operator=()
		// because it's private.

		// Add all of the attributes to this temp list,
		// then swap at the end.  This means we're exception
		// safe and don't need any try blocks.
		AttributeListImpl	theTempList;

		const unsigned int	theLength = theRHS.getLength();

		theTempList.reserve(theLength);

		// Add each attribute.
		for(unsigned int i = 0; i < theLength; i++)
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



unsigned int
AttributeListImpl::getLength() const
{
	// Make sure the mismatch between Xerces and vector<> doesn't cause a problem...
	assert(m_AttributeVector.size() == unsigned(m_AttributeVector.size()));

	return unsigned(m_AttributeVector.size());
}



const XMLCh*
AttributeListImpl::getName(const unsigned int index) const
{
	assert(index < getLength());

	return &*m_AttributeVector[index]->m_Name.begin();
}



const XMLCh*
AttributeListImpl::getType(const unsigned int index) const
{
	assert(index < getLength());

	return &*m_AttributeVector[index]->m_Type.begin();
}



const XMLCh*
AttributeListImpl::getValue(const unsigned int index) const
{
	assert(index < getLength());

	return &*m_AttributeVector[index]->m_Value.begin();
}



#if defined(XALAN_NEEDS_EXPLICIT_TEMPLATE_INSTANTIATION)
bool
AttributeListImpl::NameCompareFunctor::operator()(const AttributeVectorEntry*	theEntry) const
{
	return equals(&*theEntry->m_Name.begin(), m_name);
}
#else
struct NameCompareFunctor
{
	NameCompareFunctor(const XMLCh*		theName) :
		m_name(theName)
	{
	}

	bool
	operator()(const AttributeVectorEntry*	theEntry) const
	{
		return equals(&*theEntry->m_Name.begin(), m_name);
	}

private:

	const XMLCh* const	m_name;
};
#endif



const XMLCh*
AttributeListImpl::getType(const XMLCh* const name) const
{
	assert(name != 0);

	const AttributeVectorType::const_iterator	i =
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
AttributeListImpl::getValue(const char* const name) const
{
	return getValue(&*MakeXalanDOMCharVector(name).begin());
}



const XMLCh*
AttributeListImpl::getValue(const XMLCh* const name) const
{
	assert(name != 0);

	const AttributeVectorType::const_iterator	i =
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
			const XMLCh*	name,
			const XMLCh*	type,
			const XMLCh*	value)
{
	assert(name != 0);
	assert(type != 0);
	assert(value != 0);

	bool	fResult = false;

	XALAN_USING_STD(find_if)
	XALAN_USING_STD(copy)

	typedef AttributeVectorEntry::XMLChVectorType	XMLChVectorType;

	// Update the attribute, if it's already there...
	const AttributeVectorType::const_iterator	i =
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
			const XMLCh* const	theNewTypeEnd = AttributeVectorEntry::endArray(type) + 1;

			if ((*i)->m_Type.capacity() < XMLChVectorType::size_type(theNewTypeEnd - type))
			{
				XMLChVectorType		theNewType(type, theNewTypeEnd);

				theNewType.swap((*i)->m_Type);
			}
			else
			{
				copy(type, theNewTypeEnd, (*i)->m_Type.begin());
			}
		}

		const XMLCh* const	theNewValueEnd = AttributeVectorEntry::endArray(value) + 1;

		const XMLChVectorType::size_type	theNewSize =
						XMLChVectorType::size_type(theNewValueEnd - value);

		// If necessary, create the a new vector and swap them.  Otherwise,
		// just copy the new data in.
		if ((*i)->m_Value.capacity() < theNewSize)
		{
			XMLChVectorType		theNewValue(value, theNewValueEnd);

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

		XalanAutoPtr<AttributeVectorEntry>	theEntry(getNewEntry(name, type, value));

		// Add the new one.
		m_AttributeVector.push_back(theEntry.get());

		// The entry is now safely in the vector, so release the
		// XalanAutoPtr...
		theEntry.release();

		fResult = true;
	}

	return fResult;
}



AttributeVectorEntry*
AttributeListImpl::getNewEntry(
			const XMLCh*	name,
			const XMLCh*	type,
			const XMLCh*	value)
{
	if (m_cacheVector.empty() == true)
	{
		return new AttributeVectorEntry(name, value, type);
	}
	else
	{
		AttributeVectorEntry* const		theEntry =
			m_cacheVector.back();

		theEntry->clear();

		assert(theEntry->m_Name.size() == 0 && theEntry->m_Value.size() == 0 && theEntry->m_Type.size() == 0);

		theEntry->m_Name.insert(theEntry->m_Name.begin(), name, AttributeVectorEntry::endArray(name) + 1);
		theEntry->m_Value.insert(theEntry->m_Value.begin(), value, AttributeVectorEntry::endArray(value) + 1);
		theEntry->m_Type.insert(theEntry->m_Type.begin(), type, AttributeVectorEntry::endArray(type) + 1);

		m_cacheVector.pop_back();

		return theEntry;
	}
}



bool
AttributeListImpl::removeAttribute(const XMLCh*		name)
{
	assert(name != 0);

	bool	fResult = false;

	// Update the attribute, if it's already there...
	const AttributeVectorType::iterator		i =
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
