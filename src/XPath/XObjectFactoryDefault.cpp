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
#include "XResultTreeFrag.hpp"
#include "XSpan.hpp"
#include "XString.hpp"
#include "XStringAdapter.hpp"
#include "XStringCached.hpp"
#include "XStringReference.hpp"
#include "XUnknown.hpp"



static XBoolean		theTrueBoolean(true);
static XBoolean		theFalseBoolean(false);



XObjectFactoryDefault::XObjectFactoryDefault(
			unsigned int	theXStringBlockSize,
			unsigned int	theXNumberBlockSize,
			unsigned int	theXNodeSetBlockSize,
			unsigned int	theXResultTreeFragBlockSize) :  
	XObjectFactory(),
	m_xstringAdapterAllocator(theXStringBlockSize),
	m_xstringAllocator(theXStringBlockSize),
	m_xstringCachedAllocator(theXStringBlockSize),
	m_xstringReferenceAllocator(theXStringBlockSize),
	m_xnumberAllocator(theXNumberBlockSize),
	m_xnodesetAllocator(theXNodeSetBlockSize),
	m_xresultTreeFragAllocator(theXResultTreeFragBlockSize),
	m_xobjects(),
	m_XNull(new XNull)
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

	case XObject::eTypeString:
		{
			XString* const	theXString =
#if defined(XALAN_OLD_STYLE_CASTS)
				(XString*)theXObject;
#else
				static_cast<XString*>(theXObject);
#endif

			bStatus = m_xstringAllocator.destroy(theXString);
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

			bStatus = m_xnumberAllocator.destroy(theXNumber);
		}
		break;

	case XObject::eTypeNodeSet:
		{
			XNodeSet* const	theXNodeSet =
#if defined(XALAN_OLD_STYLE_CASTS)
				(XNodeSet*)theXObject;
#else
				static_cast<XNodeSet*>(theXObject);
#endif

			bStatus = m_xnodesetAllocator.destroy(theXNodeSet);
		}
		break;

	case XObject::eTypeResultTreeFrag:	
		{
			XResultTreeFrag* const	theXResultTreeFrag =
#if defined(XALAN_OLD_STYLE_CASTS)
				(XResultTreeFrag*)theXObject;
#else
				static_cast<XResultTreeFrag*>(theXObject);
#endif

			bStatus = m_xresultTreeFragAllocator.destroy(theXResultTreeFrag);
		}
		break;

	default:
		{
			const CollectionType::iterator	i =
			m_xobjects.find(theXObject);

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
	return XObjectPtr(theValue == true ? &theTrueBoolean : &theFalseBoolean);
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

	m_xobjects.insert(theXUnknown);

	theXUnknown->setFactory(this);

	return XObjectPtr(theXUnknown);
}



const XObjectPtr
XObjectFactoryDefault::createSpan(BorrowReturnMutableNodeRefList&	theValue)
{
	XSpan* const	theXSpan = new XSpan(theValue);

	m_xobjects.insert(theXSpan);

	theXSpan->setFactory(this);

	return XObjectPtr(theXSpan);
}



const XObjectPtr
XObjectFactoryDefault::createNumber(double	theValue)
{
	XNumber*	theXNumber = m_xnumberAllocator.createNumber(theValue);

	theXNumber->setFactory(this);

	return XObjectPtr(theXNumber);
}



const XObjectPtr
XObjectFactoryDefault::createNodeSet(BorrowReturnMutableNodeRefList&	theValue)
{
	XNodeSet* const		theXNodeSet = m_xnodesetAllocator.createNodeSet(theValue);

	theXNodeSet->setFactory(this);

	return XObjectPtr(theXNodeSet);
}



const XObjectPtr
XObjectFactoryDefault::createString(const XalanDOMString&	theValue)
{
	XString* const	theXString = m_xstringAllocator.createString(theValue);

	theXString->setFactory(this);

	return XObjectPtr(theXString);
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



const XObjectPtr
XObjectFactoryDefault::createResultTreeFrag(ResultTreeFragBase*		theValue)
{
	XResultTreeFrag* const	theResultTreeFrag =  m_xresultTreeFragAllocator.create(theValue);

	theResultTreeFrag->setFactory(this);

	return XObjectPtr(theResultTreeFrag);
}



void
XObjectFactoryDefault::reset()
{
	m_xstringAdapterAllocator.reset();

	m_xstringAllocator.reset();

	m_xstringReferenceAllocator.reset();

	m_xnumberAllocator.reset();

	m_xnodesetAllocator.reset();

	m_xresultTreeFragAllocator.reset();

#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(m_xobjects.begin(),
			 m_xobjects.end(),
			 DeleteXObjectFunctor(*this, true));

	m_xobjects.clear();
}
