/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#if !defined(ATTRIBUTESIMPL_HEADER_GUARD_1357924680)
#define ATTRIBUTESIMPL_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



#include <xercesc/sax2/Attributes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class AttributeVectorEntryExtended;



typedef XERCES_CPP_NAMESPACE_QUALIFIER Attributes	AttributesType;



class XALAN_PLATFORMSUPPORT_EXPORT AttributesImpl : public AttributesType
{
public:

	explicit
	AttributesImpl(MemoryManagerType&      theManager XALAN_DEFAULT_MEMMGR);

	virtual
	~AttributesImpl();

    AttributesImpl(const AttributesImpl&	theSource,
                    MemoryManagerType&      theManager);

    AttributesImpl(const AttributesType&	theSource,
                    MemoryManagerType&      theManager);

    AttributesImpl&
	operator=(const AttributesImpl&		theRHS);

	AttributesImpl&
	operator=(const AttributesType&		theRHS);

	// These are inherited from AttributeList
    virtual unsigned int
	getLength() const;

	virtual const XMLCh*
	getURI(const unsigned int index) const;

	virtual const XMLCh*
	getLocalName(const unsigned int index) const;

    virtual const XMLCh*
	getQName(const unsigned int index) const;

    virtual const XMLCh*
	getType(const unsigned int index) const;

    virtual const XMLCh*
	getValue(const unsigned int index) const;

	virtual int
	getIndex(
			const XMLCh* const	uri,
			const XMLCh* const	localName) const;

	virtual int
	getIndex(const XMLCh* const		qname) const;

    virtual const XMLCh*
	getType(const XMLCh* const qname) const;

	virtual const XMLCh*
	getType(
			const XMLCh* const	uri,
			const XMLCh* const	localName) const;

    virtual const XMLCh*
	getValue(const XMLCh* const qname) const;

	virtual const XMLCh*
	getValue(
			const XMLCh* const	uri,
			const XMLCh* const	localName) const;

    MemoryManagerType&
    getMemoryManager()
    {
        return m_attributesVector.getMemoryManager();
    }

	// The mutators are new to this class.

	/**
	 * Remove all attributes from the list
	 */
	virtual void
	clear();

	/**
	 * Adds an attribute to the attribute list.  Does not check for
	 * duplicates.
	 *
	 * @param  qname   attribute qname
	 * @param type   attribute type, "CDATA," for example
	 * @param value  attribute value
	 */
	void
	addAttribute(
			const XMLCh*	qname,
			const XMLCh*	type,
			const XMLCh*	value)
	{
		const XMLCh		theDummy = 0;

		addAttribute(&theDummy, &theDummy, qname, type, value);
	}

	/**
	 * Adds an attribute to the attribute list.  Does not check for
	 * duplicates.
	 *
	 * @param uri attribute namespace URI
	 * @param localName attribute local name
	 * @param qname attribute qname
	 * @param type attribute type, "CDATA," for example
	 * @param value attribute value
	 */
	void
	addAttribute(
			const XMLCh*	uri,
			const XMLCh*	localName,
			const XMLCh*	qname,
			const XMLCh*	type,
			const XMLCh*	value);

	/**
	 * Removes an attribute from the attribute list
	 *
	 * @param  qname   attribute qname
	 */
	virtual bool
	removeAttribute(const XMLCh*	qname);

	/**
	 * Swap the contents of two instances.  This must _never_
	 * throw an exception.
	 *
	 * @param thOther The instance with which to swap.
	 */
	void
	swap(AttributesImpl&		theOther)
	{
		m_attributesVector.swap(theOther.m_attributesVector);
	}

	/**
	 * Reserve room for the given number of
	 * attributes.
	 *
	 * @param theCount The number to reserve
	 */
	void
	reserve(unsigned int	theCount)
	{
		m_attributesVector.reserve(theCount);
	}

	// This vector will hold the entries.
	typedef XalanVector<AttributeVectorEntryExtended*>		AttributesVectorType;

#if defined(XALAN_NEEDS_EXPLICIT_TEMPLATE_INSTANTIATION)
	struct NameCompareFunctor
	{
		NameCompareFunctor(const XMLCh*		theQName) :
			m_qname(theQName)
		{
		}

		bool
		operator()(const AttributeVectorEntryExtended*	theEntry) const;

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
		operator()(const AttributeVectorEntryExtended*	theEntry) const;

	private:

		const XMLCh* const	m_uri;
		const XMLCh* const	m_localName;
	};
#endif

private:

	// This is not implemented.
    bool
	operator==(const AttributesImpl&) const;

	// Default vector allocation size.
	enum
	{
		eDefaultVectorSize = 5
	};

	AttributeVectorEntryExtended*
	getNewEntry(
			const XMLCh*	qname,
			const XMLCh*	type,
			const XMLCh*	value,
			const XMLCh*	uri = 0,
			const XMLCh*	localName = 0);

	// Helper function to delete entries...
	static void
	deleteEntries(AttributesVectorType&		theVector);

	AttributesVectorType	m_attributesVector;

	AttributesVectorType	m_cacheVector;
};



XALAN_CPP_NAMESPACE_END



#endif	// ATTRIBUTESIMPL_HEADER_GUARD_1357924680
