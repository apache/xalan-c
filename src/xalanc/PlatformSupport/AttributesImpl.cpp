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
#include "AttributesImpl.hpp"



#include <algorithm>
#include <cassert>



#include <xalanc/Include/XalanAutoPtr.hpp>
#include <xalanc/Include/STLHelper.hpp>



#include "AttributeVectorEntryExtended.hpp"



XALAN_CPP_NAMESPACE_BEGIN



AttributesImpl::AttributesImpl() :
	AttributesType(),
	m_attributesVector(),
	m_cacheVector()
{
}



AttributesImpl::~AttributesImpl()
{
	// Clean up everything...
	clear();

	assert(m_attributesVector.empty() == true);

	deleteEntries(m_cacheVector);
}



AttributesImpl::AttributesImpl(const AttributesImpl&	theSource) :
	AttributesType(),
	m_attributesVector()
{
	// Use the assignment operator to do the dirty work...
	*this = theSource;

	assert(getLength() == theSource.getLength());
}



AttributesImpl::AttributesImpl(const AttributesType&	theSource) :
	AttributesType(),
	m_attributesVector()
{
	// Use the assignment operator to do the dirty work...
	*this = theSource;

	assert(getLength() == theSource.getLength());
}



void
AttributesImpl::deleteEntries(AttributesVectorType&		theVector)
{
	// Delete all of the objects in the vector.
	XALAN_STD_QUALIFIER for_each(
			theVector.begin(),
			theVector.end(),
			DeleteFunctor<AttributeVectorEntryExtended>());
}



AttributesImpl&
AttributesImpl::operator=(const AttributesImpl&		theRHS)
{
	if (this != &theRHS)
	{
		// Note that we can't chain up to our base class operator=()
		// because it's private.

		// Some temporary structures to hold everything
		// until we're done.
		AttributesVectorType		tempVector;

		const unsigned int	theLength = theRHS.getLength();

		if (theLength > 0)
		{
			// Reserve the appropriate capacity right now...
			tempVector.reserve(theLength);

			// This will delete everything in tempVector when we're done...
			CollectionDeleteGuard<AttributesVectorType,
								  DeleteFunctor<AttributeVectorEntryExtended> >		theGuard(tempVector);

			typedef AttributesVectorType::const_iterator		const_iterator;

			const const_iterator	theEnd = theRHS.m_attributesVector.end();

			// Copy the vector entries, and build the index map...
			for(const_iterator i = theRHS.m_attributesVector.begin(); i != theEnd; ++i)
			{
				AttributeVectorEntryExtended* const		theEntry = *i;

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
AttributesImpl::operator=(const AttributesType&		theRHS)
{
	if (this != &theRHS)
	{
		// Note that we can't chain up to our base class operator=()
		// because it's private.

		// Add all of the attributes to this temp list,
		// then swap at the end.  This means we're exception
		// safe and don't need any try blocks.
		AttributesImpl	theTempList;

		const unsigned int	theLength = theRHS.getLength();

		theTempList.reserve(theLength);

		// Add each attribute.
		for(unsigned int i = 0; i < theLength; i++)
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



unsigned int
AttributesImpl::getLength() const
{
	return unsigned(m_attributesVector.size());
}



const XMLCh*
AttributesImpl::getURI(const unsigned int index) const
{
	assert(index < getLength());

	return &*m_attributesVector[index]->m_uri.begin();
}



const XMLCh*
AttributesImpl::getLocalName(const unsigned int index) const
{
	assert(index < getLength());

	return &*m_attributesVector[index]->m_localName.begin();
}



const XMLCh*
AttributesImpl::getQName(const unsigned int		index) const
{
	assert(index < getLength());

	return &*m_attributesVector[index]->m_Name.begin();
}



const XMLCh*
AttributesImpl::getType(const unsigned int	index) const
{
	assert(index < getLength());

	return &*m_attributesVector[index]->m_Type.begin();
}



const XMLCh*
AttributesImpl::getValue(const unsigned int		index) const
{
	assert(index < getLength());

	return &*m_attributesVector[index]->m_Value.begin();
}



#if defined(XALAN_NEEDS_EXPLICIT_TEMPLATE_INSTANTIATION)
bool
AttributesImpl::NameCompareFunctor::operator()(const AttributeVectorEntryExtended*	theEntry) const
{
	return equals(&*theEntry->m_Name.begin(), m_qname);
}

bool
AttributesImpl::URIAndLocalNameCompareFunctor::operator()(const AttributeVectorEntryExtended*	theEntry) const
{
	return equals(&*theEntry->m_uri.begin(), m_uri) && equals(&*theEntry->m_localName.begin(), m_localName) ;
}
#else
struct NameCompareFunctor
{
	NameCompareFunctor(const XMLCh*		theQName) :
		m_qname(theQName)
	{
	}

	bool
	operator()(const AttributeVectorEntryExtended*	theEntry) const
	{
		return equals(&*theEntry->m_Name.begin(), m_qname);
	}

private:

	const XMLCh* const	m_qname;
};



struct URIAndLocalNameCompareFunctor
{
	URIAndLocalNameCompareFunctor(
			const XMLCh*	theURI,
			const XMLCh*	theLocalName) :
		m_uri(theURI),
		m_localName(theLocalName)
	{
	}

	bool
	operator()(const AttributeVectorEntryExtended*	theEntry) const
	{
		return equals(&*theEntry->m_uri.begin(), m_uri) && equals(&*theEntry->m_localName.begin(), m_localName) ;
	}

private:

	const XMLCh* const	m_uri;
	const XMLCh* const	m_localName;
};
#endif



const XMLCh*
AttributesImpl::getType(const XMLCh* const	qname) const
{
	const int	theIndex = getIndex(qname);

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
AttributesImpl::getValue(const XMLCh* const		qname) const
{
	const int	theIndex = getIndex(qname);

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
			const XMLCh* const	uri,
			const XMLCh* const	localName) const
{
	const int	theIndex = getIndex(uri, localName);

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
			const XMLCh* const	uri,
			const XMLCh* const	localName) const
{
	const int	theIndex = getIndex(uri, localName);

	if (theIndex == -1)
	{
		return 0;
	}
	else
	{
		return getValue(theIndex);
	}
}



int
AttributesImpl::getIndex(
			const XMLCh* const	uri,
			const XMLCh* const	localName) const
{
	assert(uri != 0 && localName != 0);

	const AttributesVectorType::const_iterator	i =
		XALAN_STD_QUALIFIER find_if(
			m_attributesVector.begin(),
			m_attributesVector.end(),
			URIAndLocalNameCompareFunctor(uri, localName));

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



int
AttributesImpl::getIndex(const XMLCh* const		qname) const
{
	assert(qname != 0);

	const AttributesVectorType::const_iterator	i =
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
			const XMLCh*	uri,
			const XMLCh*	localName,
			const XMLCh*	name,
			const XMLCh*	type,
			const XMLCh*	value)
{
	assert(name != 0);
	assert(type != 0);
	assert(value != 0);

	typedef AttributeVectorEntry::XMLChVectorType	XMLChVectorType;

	if (m_attributesVector.capacity() == 0)
	{
		m_attributesVector.reserve(eDefaultVectorSize);
	}

	XalanAutoPtr<AttributeVectorEntryExtended>	theEntry(getNewEntry(name, type, value, uri, localName));

	// Add the new one.
	m_attributesVector.push_back(theEntry.get());

	// The entry is now safely in the vector, so release the
	// XalanAutoPtr...
	theEntry.release();
}



AttributeVectorEntryExtended*
AttributesImpl::getNewEntry(
			const XMLCh*	name,
			const XMLCh*	type,
			const XMLCh*	value,
			const XMLCh*	uri,
			const XMLCh*	localName)
{
	assert(name != 0);
	assert(type != 0);
	assert(value != 0);
	assert(uri != 0);
	assert(localName != 0);

	if (m_cacheVector.empty() == true)
	{
		return new AttributeVectorEntryExtended(name, value, type, uri, localName);
	}
	else
	{
		AttributeVectorEntryExtended* const		theEntry =
			m_cacheVector.back();

		theEntry->clear();

		assert(theEntry->m_Name.size() == 0 && theEntry->m_Value.size() == 0 &&
			   theEntry->m_Type.size() == 0 && theEntry->m_uri.size() == 0 &&
			   theEntry->m_localName.size() == 0 );

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
AttributesImpl::removeAttribute(const XMLCh*		name)
{
	assert(name != 0);

	bool	fResult = false;

	// Update the attribute, if it's already there...
	const AttributesVectorType::iterator		i =
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
