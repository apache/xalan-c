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
#if !defined(XOBJECTFACTORYDEFAULT_HEADER_GUARD_1357924680)
#define XOBJECTFACTORYDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



// Base class header file...
#include <xalanc/XPath/XObjectFactory.hpp>



#include <xalanc/XPath/XBoolean.hpp>
#include <xalanc/XPath/XNodeSetAllocator.hpp>
#include <xalanc/XPath/XNodeSetNodeProxyAllocator.hpp>
#include <xalanc/XPath/XNull.hpp>
#include <xalanc/XPath/XNumberAllocator.hpp>
#include <xalanc/XPath/XStringAllocator.hpp>
#include <xalanc/XPath/XStringAdapterAllocator.hpp>
#include <xalanc/XPath/XStringCachedAllocator.hpp>
#include <xalanc/XPath/XStringReferenceAllocator.hpp>
#include <xalanc/XPath/XTokenNumberAdapterAllocator.hpp>
#include <xalanc/XPath/XTokenStringAdapterAllocator.hpp>




XALAN_CPP_NAMESPACE_BEGIN



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

    typedef XNodeSetAllocator::size_type	size_type;

	/**
	 * Construct a factory for creating XObjects.
	 * 
	 * @param theXStringBlockSize allocation block size
	 * @param theXNumberBlockSize allocation block size
	 * @param theXNodeSetBlockSize allocation block size
	 */
	explicit
	XObjectFactoryDefault(
            MemoryManagerType& theManager XALAN_DEFAULT_MEMMGR,
			size_type	theXStringBlockSize = eDefaultXStringBlockSize,
			size_type	theXNumberBlockSize = eDefaultXNumberBlockSize,
			size_type	theXNodeSetBlockSize = eDefaultXNodeSetBlockSize,
			size_type	theXNodeSetNodeProxyBlockSize = eDefaultXNodeSetNodeProxyBlockSize); 

    static XObjectFactoryDefault*
    create(
            MemoryManagerType& theManager,
			size_type	theXStringBlockSize = eDefaultXStringBlockSize,
			size_type	theXNumberBlockSize = eDefaultXNumberBlockSize,
			size_type	theXNodeSetBlockSize = eDefaultXNodeSetBlockSize,
			size_type	theXNodeSetNodeProxyBlockSize = eDefaultXNodeSetNodeProxyBlockSize);


	virtual
	~XObjectFactoryDefault();

    MemoryManagerType&
    getMemoryManager()
    {
        return m_xobjects.getMemoryManager();
    }
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

	typedef XalanVector<XObject*>		XObjectCollectionType;
	typedef XalanVector<XNumber*>		XNumberCacheType;
	typedef XalanVector<XNodeSet*>		XNodeSetCacheType;
	typedef XalanVector<XString*>		XStringCacheType;

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

	XNull		                    m_xnull;

	XBoolean	                    m_xbooleanFalse;

	XBoolean	                    m_xbooleanTrue;
};



XALAN_CPP_NAMESPACE_END



#endif	// XOBJECTFACTORYDEFAULT_HEADER_GUARD_1357924680
