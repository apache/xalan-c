/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <set>
#include <vector>



// Base class header file...
#include <xalanc/XPath/XObjectFactory.hpp>



#include <xalanc/XPath/XNodeSetAllocator.hpp>
#include <xalanc/XPath/XNodeSetNodeProxyAllocator.hpp>
#include <xalanc/XPath/XNumberAllocator.hpp>
#include <xalanc/XPath/XStringAllocator.hpp>
#include <xalanc/XPath/XStringAdapterAllocator.hpp>
#include <xalanc/XPath/XStringCachedAllocator.hpp>
#include <xalanc/XPath/XStringReferenceAllocator.hpp>
#include <xalanc/XPath/XTokenNumberAdapterAllocator.hpp>
#include <xalanc/XPath/XTokenStringAdapterAllocator.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <xalanc/XPath/XNull.hpp>
#include <xalanc/XPath/XBoolean.hpp>
#endif



#include <xalanc/Include/XalanAutoPtr.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XBoolean;
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
		eDefaultXNodeSetNodeProxyBlockSize = 5,
		eXNumberCacheMax = 40,
		eXNodeSetCacheMax = 40,
		eXStringCacheMax = 40,
		eXResultTreeFragCacheMax = 40
	};

	typedef unsigned long	size_type;

	/**
	 * Construct a factory for creating XObjects.
	 * 
	 * @param theXStringBlockSize allocation block size
	 * @param theXNumberBlockSize allocation block size
	 * @param theXNodeSetBlockSize allocation block size
	 */
	explicit
	XObjectFactoryDefault(
			size_type	theXStringBlockSize = eDefaultXStringBlockSize,
			size_type	theXNumberBlockSize = eDefaultXNumberBlockSize,
			size_type	theXNodeSetBlockSize = eDefaultXNodeSetBlockSize,
			size_type	theXNodeSetNodeProxyBlockSize = eDefaultXNodeSetNodeProxyBlockSize); 

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
	createNodeSet(XalanNode* 	theValue);

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

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<XObject*>		XObjectCollectionType;
	typedef vector<XNumber*>		XNumberCacheType;
	typedef vector<XNodeSet*>		XNodeSetCacheType;
	typedef vector<XString*>		XStringCacheType;
#else
	typedef std::vector<XObject*>	XObjectCollectionType;
	typedef std::vector<XNumber*>	XNumberCacheType;
	typedef std::vector<XNodeSet*>	XNodeSetCacheType;
	typedef std::vector<XString*>	XStringCacheType;
#endif

protected:

	virtual bool
	doReturnObject(
			XObject*	theXObject,
			bool		fInReset = false);

private:

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

	XNodeSetNodeProxyAllocator		m_xnodesetNodeProxyAllocator;

	XTokenNumberAdapterAllocator	m_xtokenNumberAdapterAllocator;

	XTokenStringAdapterAllocator	m_xtokenStringAdapterAllocator;

	XObjectCollectionType			m_xobjects;

	XNumberCacheType				m_xnumberCache;

	XNodeSetCacheType				m_xnodesetCache;

	XStringCacheType				m_xstringCache;

	const XalanAutoPtr<XNull>		m_XNull;

	const XalanAutoPtr<XBoolean>	m_xbooleanFalse;

	const XalanAutoPtr<XBoolean>	m_xbooleanTrue;
};



XALAN_CPP_NAMESPACE_END



#endif	// XOBJECTFACTORYDEFAULT_HEADER_GUARD_1357924680
