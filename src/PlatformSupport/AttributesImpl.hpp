/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights 
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
#if !defined(ATTRIBUTESIMPL_HEADER_GUARD_1357924680)
#define ATTRIBUTESIMPL_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <vector>



#include <sax2/Attributes.hpp>



class AttributeVectorEntryExtended;



class XALAN_PLATFORMSUPPORT_EXPORT AttributesImpl : public Attributes
{
public:

	explicit
	AttributesImpl();

	virtual
	~AttributesImpl();

    AttributesImpl(const AttributesImpl&	theSource);

    AttributesImpl(const Attributes&	theSource);

    AttributesImpl&
	operator=(const AttributesImpl&	theRHS);

	AttributesImpl&
	operator=(const Attributes&	theRHS);

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
		addAttribute(0, 0, qname, type, value);
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

#if defined(XALAN_NO_NAMESPACES)
	// This vector will hold the entries.
	typedef vector<AttributeVectorEntryExtended*>		AttributesVectorType;
#else
	// This vector will hold the entries.
	typedef std::vector<AttributeVectorEntryExtended*>	AttributesVectorType;
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



#endif	// ATTRIBUTESIMPL_HEADER_GUARD_1357924680
