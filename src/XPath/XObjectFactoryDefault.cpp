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
// Class header file...
#include "XObjectFactoryDefault.hpp"



#include <algorithm>
#include <memory>



#include "XBoolean.hpp"
#include "XNodeSet.hpp"
#include "XNull.hpp"
#include "XNumber.hpp"
#include "XString.hpp"
#include "XStringAdapter.hpp"
#include "XStringCached.hpp"
#include "XStringReference.hpp"
#include "XUnknown.hpp"



XObjectFactoryDefault::XObjectFactoryDefault(
			unsigned int	theXStringBlockSize,
			unsigned int	theXNumberBlockSize,
			unsigned int	theXNodeSetBlockSize) : 
	XObjectFactory(),
	m_xstringAdapterAllocator(theXStringBlockSize),
	m_xstringAllocator(theXStringBlockSize),
	m_xstringCachedAllocator(theXStringBlockSize),
	m_xstringReferenceAllocator(theXStringBlockSize),
	m_xnumberAllocator(theXNumberBlockSize),
	m_xnodesetAllocator(theXNodeSetBlockSize),
	m_xtokenNumberAdapterAllocator(theXNumberBlockSize),
	m_xtokenStringAdapterAllocator(theXStringBlockSize),
	m_xobjects(),
	m_xnumberCache(),
	m_xnodesetCache(),
	m_xstringCache(),
	m_XNull(new XNull),
	m_xbooleanFalse(new XBoolean(false)),
	m_xbooleanTrue(new XBoolean(true))
{
}



XObjectFactoryDefault::~XObjectFactoryDefault()
{
	reset();
}



bool
XObjectFactoryDefault::doReturnObject(
			XObject*	theXObject,
			bool		fInReset)
{
	assert(theXObject != 0);

	bool bStatus = false;	

	const XObject::eObjectType	theType = getRealType(*theXObject);

	switch(theType)
	{
	case XObject::eTypeBoolean:
	case XObject::eTypeNull:
		{		
			bStatus = true;
		}

	case XObject::eTypeStringAdapter:
		{
			XStringAdapter* const		theXStringAdapter =
#if defined(XALAN_OLD_STYLE_CASTS)
				(XStringAdapter*)theXObject;
#else
				static_cast<XStringAdapter*>(theXObject);
#endif

			bStatus = m_xstringAdapterAllocator.destroy(theXStringAdapter);
		}
		break;

	case XObject::eTypeXTokenNumberAdapter:
		{
			XTokenNumberAdapter* const	theAdapter =
#if defined(XALAN_OLD_STYLE_CASTS)
				(XTokenNumberAdapter*)theXObject;
#else
				static_cast<XTokenNumberAdapter*>(theXObject);
#endif

			bStatus = m_xtokenNumberAdapterAllocator.destroy(theAdapter);
		}
		break;

	case XObject::eTypeXTokenStringAdapter:
		{
			XTokenStringAdapter* const	theAdapter =
#if defined(XALAN_OLD_STYLE_CASTS)
				(XTokenStringAdapter*)theXObject;
#else
				static_cast<XTokenStringAdapter*>(theXObject);
#endif

			bStatus = m_xtokenStringAdapterAllocator.destroy(theAdapter);
		}
		break;

	case XObject::eTypeString:
		{
			XString* const	theXString =
#if defined(XALAN_OLD_STYLE_CASTS)
				(XString*)theXObject;
#else
				static_cast<XString*>(theXObject);
#endif

			if (m_xstringCache.size() < eXStringCacheMax)
			{
				m_xstringCache.push_back(theXString);

				bStatus = true;
			}
			else
			{
				bStatus = m_xstringAllocator.destroy(theXString);
			}
		}
		break;

	case XObject::eTypeStringCached:
		{
			XStringCached* const	theXStringCached =
#if defined(XALAN_OLD_STYLE_CASTS)
				(XStringCached*)theXObject;
#else
				static_cast<XStringCached*>(theXObject);
#endif

			bStatus = m_xstringCachedAllocator.destroy(theXStringCached);
		}
		break;

	case XObject::eTypeStringReference:
		{
			XStringReference* const		theXStringReference =
#if defined(XALAN_OLD_STYLE_CASTS)
				(XStringReference*)theXObject;
#else
				static_cast<XStringReference*>(theXObject);
#endif

			bStatus = m_xstringReferenceAllocator.destroy(theXStringReference);
		}
		break;

	case  XObject::eTypeNumber:
		{
			XNumber* const	theXNumber =
#if defined(XALAN_OLD_STYLE_CASTS)
				(XNumber*)theXObject;
#else
				static_cast<XNumber*>(theXObject);
#endif

			if (m_xnumberCache.size() < eXNumberCacheMax)
			{
				m_xnumberCache.push_back(theXNumber);

				bStatus = true;
			}
			else
			{
				bStatus = m_xnumberAllocator.destroy(theXNumber);
			}
		}
		break;

	case XObject::eTypeNodeSet:
		{
			XNodeSet* const		theXNodeSet =
#if defined(XALAN_OLD_STYLE_CASTS)
				(XNodeSet*)theXObject;
#else
				static_cast<XNodeSet*>(theXObject);
#endif

			if (m_xnodesetCache.size() < eXNodeSetCacheMax)
			{
				theXNodeSet->release();

				m_xnodesetCache.push_back(theXNodeSet);

				bStatus = true;
			}
			else
			{
				bStatus = m_xnodesetAllocator.destroy(theXNodeSet);
			}
		}
		break;

	default:
		{
#if !defined(XALAN_NO_NAMESPACES)
			using std::find;
#endif

			const XObjectCollectionType::iterator	i =
					find(m_xobjects.begin(), m_xobjects.end(), theXObject);

			if (i != m_xobjects.end())
			{
				if (fInReset == false)
				{
					m_xobjects.erase(i);
				}

				deleteObject(theXObject);

				bStatus = true;
			}
		}
		break;
	}
	
	return bStatus;
}



const XObjectPtr
XObjectFactoryDefault::createBoolean(
			bool	theValue)
{
	if (theValue == true)
	{
		return XObjectPtr(m_xbooleanTrue.get());
	}
	else
	{
		return XObjectPtr(m_xbooleanFalse.get());
	}
}



const XObjectPtr
XObjectFactoryDefault::createNull()
{	
	return XObjectPtr(m_XNull.get());
}



const XObjectPtr
XObjectFactoryDefault::createUnknown(const XalanDOMString&	theValue)
{
	XUnknown* const	theXUnknown = new XUnknown(theValue);

	m_xobjects.push_back(theXUnknown);

	theXUnknown->setFactory(this);

	return XObjectPtr(theXUnknown);
}



const XObjectPtr
XObjectFactoryDefault::createNumber(double	theValue)
{
	if (m_xnumberCache.empty() == false)
	{
		XNumber* const	theXNumber = m_xnumberCache.back();

		m_xnumberCache.pop_back();

		theXNumber->set(theValue);

		return XObjectPtr(theXNumber);
	}
	else
	{
		m_xnumberCache.reserve(eXNumberCacheMax);

		XObject* const	theXObject = m_xnumberAllocator.createNumber(theValue);

		theXObject->setFactory(this);

		return XObjectPtr(theXObject);
	}
}



const XObjectPtr
XObjectFactoryDefault::createNumber(const XToken&	theValue)
{
	XObject*	theXObject = m_xtokenNumberAdapterAllocator.create(theValue);

	theXObject->setFactory(this);

	return XObjectPtr(theXObject);
}



const XObjectPtr
XObjectFactoryDefault::createNodeSet(BorrowReturnMutableNodeRefList&	theValue)
{
	if (m_xnodesetCache.empty() == false)
	{
		XNodeSet* const		theXObject = m_xnodesetCache.back();

		m_xnodesetCache.pop_back();

		theXObject->set(theValue);

		return XObjectPtr(theXObject);
	}
	else
	{
		m_xnodesetCache.reserve(eXNodeSetCacheMax);

		XNodeSet* const		theXObject = m_xnodesetAllocator.createNodeSet(theValue);

		theXObject->setFactory(this);

		return XObjectPtr(theXObject);
	}
}



const XObjectPtr
XObjectFactoryDefault::createString(const XalanDOMString&	theValue)
{
	if (m_xstringCache.empty() == false)
	{
		XString* const	theXString = m_xstringCache.back();

		m_xstringCache.pop_back();

		theXString->set(theValue);

		return XObjectPtr(theXString);
	}
	else
	{
		m_xstringCache.reserve(eXStringCacheMax);

		XString* const	theXString = m_xstringAllocator.createString(theValue);

		theXString->setFactory(this);

		return XObjectPtr(theXString);
	}
}



const XObjectPtr
XObjectFactoryDefault::createString(const XalanDOMChar*		theValue)
{
	XString* const	theXString = m_xstringAllocator.createString(theValue);

	theXString->setFactory(this);

	return XObjectPtr(theXString);
}



const XObjectPtr
XObjectFactoryDefault::createString(
			const XalanDOMChar*		theValue,
			unsigned int			theLength)
{
	XString* const	theXString = m_xstringAllocator.createString(theValue, theLength);

	theXString->setFactory(this);

	return XObjectPtr(theXString);
}



const XObjectPtr
XObjectFactoryDefault::createString(const XToken&	theValue)
{
	XObject*	theXObject = m_xtokenStringAdapterAllocator.create(theValue);

	theXObject->setFactory(this);

	return XObjectPtr(theXObject);
}



const XObjectPtr
XObjectFactoryDefault::createStringReference(const XalanDOMString&	theValue)
{
	XStringReference* const	theXStringReference = m_xstringReferenceAllocator.createString(theValue);

	theXStringReference->setFactory(this);

	return XObjectPtr(theXStringReference);
}



const XObjectPtr
XObjectFactoryDefault::createStringAdapter(const XObjectPtr&	theValue)
{
	XStringAdapter* const	theXStringAdapter = m_xstringAdapterAllocator.createString(theValue);

	theXStringAdapter->setFactory(this);

	return XObjectPtr(theXStringAdapter);
}



const XObjectPtr
XObjectFactoryDefault::createString(GetAndReleaseCachedString&	theValue)
{
	XStringCached* const	theXStringCached = m_xstringCachedAllocator.createString(theValue);

	theXStringCached->setFactory(this);

	return XObjectPtr(theXStringCached);
}



void
XObjectFactoryDefault::reset()
{
	m_xstringAdapterAllocator.reset();

	m_xstringAllocator.reset();

	m_xstringReferenceAllocator.reset();

	m_xnumberAllocator.reset();

	m_xnodesetAllocator.reset();

#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(m_xobjects.begin(),
			 m_xobjects.end(),
			 DeleteXObjectFunctor(*this, true));

	m_xobjects.clear();

	m_xnumberCache.clear();

	m_xnodesetCache.clear();

	m_xstringCache.clear();
}
