/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
#if !defined(XOBJECTFACTORYDEFAULT_HEADER_GUARD_1357924680)
#define XOBJECTFACTORYDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <set>
#include <vector>



// Base class header file...
#include <XPath/XObjectFactory.hpp>



#include <XPath/XNodeSetAllocator.hpp>
#include <XPath/XNumberAllocator.hpp>
#include <XPath/XResultTreeFragAllocator.hpp>
#include <XPath/XStringAllocator.hpp>
#include <XPath/XStringAdapterAllocator.hpp>
#include <XPath/XStringCachedAllocator.hpp>
#include <XPath/XStringReferenceAllocator.hpp>
#include <XPath/XTokenNumberAdapterAllocator.hpp>
#include <XPath/XTokenStringAdapterAllocator.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <XPath/XNull.hpp>
#endif



#include <Include/XalanAutoPtr.hpp>



class XNull;
class XNumber;



/**
 * This class handles the creation of XObjects and manages their lifetime.
 */
class XALAN_XPATH_EXPORT XObjectFactoryDefault : public XObjectFactory
{
public:

	// Default block size for strings.
	enum
	{
		eDefaultXStringBlockSize = 10,
		eDefaultXNumberBlockSize = 10,
		eDefaultXNodeSetBlockSize = 10,
		eDefaultXResultTreeFragBlockSize = 10
	};
	
	/**
	 * Construct a factory for creating XObjects.
	 * 
	 * @param theXStringBlockSize allacation block size
	 * @param theXNumberBlockSize allacation block size
	 * @param theXNodeSetBlockSize allacation block size
	 */
	explicit
	XObjectFactoryDefault(
			unsigned int		theXStringBlockSize = eDefaultXStringBlockSize,
			unsigned int		theXNumberBlockSize = eDefaultXNumberBlockSize,
			unsigned int		theXNodeSetBlockSize = eDefaultXNodeSetBlockSize,
			unsigned int		theXResultTreeFragBlockSize = eDefaultXResultTreeFragBlockSize); 

	virtual
	~XObjectFactoryDefault();

	// These methods are inherited from XObjectFactory ...

	virtual void
	reset();

	virtual const XObjectPtr
	createBoolean(bool	theValue);

	virtual const XObjectPtr
	createNodeSet(BorrowReturnMutableNodeRefList&	theValue);

	virtual const XObjectPtr
	createNull();

	virtual const XObjectPtr
	createNumber(double		theValue);

	virtual const XObjectPtr
	createNumber(const XToken&	theValue);

	virtual const XObjectPtr
	createString(const XalanDOMString&	theValue);

	virtual const XObjectPtr
	createString(const XalanDOMChar*	theValue);

	virtual const XObjectPtr
	createString(
			const XalanDOMChar*		theValue,
			unsigned int			theLength);

	virtual const XObjectPtr
	createString(const XToken&	theValue);

	virtual const XObjectPtr
	createStringReference(const XalanDOMString&		theValue);

	virtual const XObjectPtr
	createStringAdapter(const XObjectPtr&	theValue);

	virtual const XObjectPtr
	createString(GetAndReleaseCachedString&		theValue);

	virtual const XObjectPtr
	createUnknown(
			const XalanDOMString&	theValue);

	virtual const XObjectPtr
	createResultTreeFrag(BorrowReturnResultTreeFrag&	theValue);

	virtual const XObjectPtr
	createSpan(BorrowReturnMutableNodeRefList&	theValue);

#if defined(XALAN_NO_NAMESPACES)
	typedef set<XObject*, less<XObject*> >		CollectionType;
	typedef vector<XNumber*>, less<XNumber*> >	XNumberCacheType;
#else
	typedef std::set<XObject*>					CollectionType;
	typedef std::vector<XNumber*>				XNumberCacheType;
#endif

protected:

	virtual bool
	doReturnObject(
			XObject*	theXObject,
			bool		fInReset = false);

private:

	enum { eXNumberCacheMax = 40 };

	// Not implemented...
	XObjectFactoryDefault(const XObjectFactoryDefault&);

	XObjectFactoryDefault&
	operator=(const XObjectFactoryDefault&);

	bool
	operator==(const XObjectFactoryDefault&) const;


	// Data members...

	// This one's first, since it may be be holding references
	// to objects in other allocators.
	XStringAdapterAllocator			m_xstringAdapterAllocator;

	XStringAllocator				m_xstringAllocator;

	XStringCachedAllocator			m_xstringCachedAllocator;

	XStringReferenceAllocator		m_xstringReferenceAllocator;

	XNumberAllocator				m_xnumberAllocator;

	XNodeSetAllocator				m_xnodesetAllocator;

	XResultTreeFragAllocator		m_xresultTreeFragAllocator;

	XTokenNumberAdapterAllocator	m_xtokenNumberAdapterAllocator;

	XTokenStringAdapterAllocator	m_xtokenStringAdapterAllocator;

	CollectionType					m_xobjects;

	XNumberCacheType				m_xnumberCache;

	const XalanAutoPtr<XNull>		m_XNull;
};



#endif	// XOBJECTFACTORYDEFAULT_HEADER_GUARD_1357924680
