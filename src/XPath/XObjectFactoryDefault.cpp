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
#include "XBooleanStatic.hpp"
#include "XNodeSet.hpp"
#include "XNull.hpp"
#include "XNumber.hpp"
#include "XPathEnvSupport.hpp"
#include "XPathSupport.hpp"
#include "XResultTreeFrag.hpp"
#include "XSpan.hpp"
#include "XString.hpp"
#include "XUnknown.hpp"



static XBooleanStatic		theTrueBoolean(true);
static XBooleanStatic		theFalseBoolean(false);



XObjectFactoryDefault::XObjectFactoryDefault(
			XPathEnvSupport&	theEnvSupport,
			XPathSupport&		theSupport) :
	XObjectFactory(),
	m_envSupport(theEnvSupport),
	m_support(theSupport),
	m_xobjects(),
	m_XNull(new XNull(theEnvSupport, theSupport))
#if !defined(NDEBUG)
	, m_totalBooleanInstanceCount(0),
	m_totalNodeSetInstanceCount(0),
	m_totalNullInstanceCount(1),
	m_totalNumberInstanceCount(0),
	m_totalStringInstanceCount(0),
	m_totalUnknownInstanceCount(0),
	m_totalResultTreeFragInstanceCount(0),
	m_totalSpanInstanceCount(0)
#endif
{
}



XObjectFactoryDefault::~XObjectFactoryDefault()
{
	reset();
}



bool
XObjectFactoryDefault::doReturnObject(
			const XObject*	theXObject,
			bool			fInReset)
{
	assert(theXObject != 0);

	if (theXObject == &theTrueBoolean ||
		theXObject == &theFalseBoolean ||
		theXObject == m_XNull.get())
	{
		return true;
	}
	else
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

			return true;
		}
		else
		{
			return false;
		}
	}
}



XObject*
XObjectFactoryDefault::clone(const XObject&		theXObject)
{
	if (&theXObject == &theTrueBoolean)
	{
		return &theTrueBoolean;
	}
	else if (&theXObject == &theFalseBoolean)
	{
		return &theFalseBoolean;
	}
	else if (&theXObject == m_XNull.get())
	{
		return m_XNull.get();
	}
	else
	{
		XObject* const	theClone = theXObject.clone();

		m_xobjects.insert(theClone);

		return theClone;
	}
}



XObject*
XObjectFactoryDefault::createBoolean(
			bool	theValue,
			bool	fOptimize)
{
	if (fOptimize == true)
	{
		return theValue == true ? &theTrueBoolean : &theFalseBoolean;
	}
	else
	{
		XBoolean* const		theBoolean = new XBoolean(m_envSupport, m_support, theValue);

		m_xobjects.insert(theBoolean);

#if !defined(NDEBUG)
		++m_totalBooleanInstanceCount;
#endif
		return theBoolean;
	}
}



XObject*
XObjectFactoryDefault::createNodeSet(
			NodeRefListBase*	value,
			bool				/* fOptimize */)
{
	XNodeSet* const		theXNodeSet = new XNodeSet(m_envSupport, m_support, value);

	m_xobjects.insert(theXNodeSet);

#if !defined(NDEBUG)
	++m_totalNodeSetInstanceCount;
#endif

	return theXNodeSet;
}



XObject*
XObjectFactoryDefault::createNodeSet(
			XalanNode&	value,
			bool		/* fOptimize */)
{
	XNodeSet* const		theXNodeSet = new XNodeSet(m_envSupport, m_support, value);

	m_xobjects.insert(theXNodeSet);

#if !defined(NDEBUG)
	++m_totalNodeSetInstanceCount;
#endif

	return theXNodeSet;
}



XObject*
XObjectFactoryDefault::createNull(bool		fOptimize)
{
	if (fOptimize == true)
	{
		return m_XNull.get();
	}
	else
	{
		XNull* const	theXNull = new XNull(m_envSupport, m_support);

		m_xobjects.insert(theXNull);

#if !defined(NDEBUG)
	++m_totalNullInstanceCount;
#endif

		return theXNull;
	}
}



XObject*
XObjectFactoryDefault::createNumber(
			double	theValue,
			bool	/* fOptimize */)
{
	XNumber*	theXNumber = new XNumber(m_envSupport, m_support, theValue);

	m_xobjects.insert(theXNumber);

#if !defined(NDEBUG)
	++m_totalNumberInstanceCount;
#endif

	return theXNumber;
}



XObject*
XObjectFactoryDefault::createString(
			const XalanDOMString&	theValue,
			bool					/* fOptimize */)
{
	XString* const	theXString = new XString(m_envSupport, m_support, theValue);

	m_xobjects.insert(theXString);

#if !defined(NDEBUG)
	++m_totalStringInstanceCount;
#endif

	return theXString;
}



XObject*
XObjectFactoryDefault::createUnknown(
			const XalanDOMString&	theValue,
			bool					/* fOptimize */)
{
	XUnknown* const	theXUnknown = new XUnknown(m_envSupport, m_support, theValue);

	m_xobjects.insert(theXUnknown);

#if !defined(NDEBUG)
	++m_totalUnknownInstanceCount;
#endif

	return theXUnknown;
}



XObject*
XObjectFactoryDefault::createResultTreeFrag(
			ResultTreeFragBase*		theValue,
			bool					/* fOptimize */)
{
	XResultTreeFrag* const	theResultTreeFrag = new XResultTreeFrag(m_envSupport, m_support, theValue);

	m_xobjects.insert(theResultTreeFrag);

#if !defined(NDEBUG)
	++m_totalResultTreeFragInstanceCount;
#endif

	return theResultTreeFrag;
}



XObject*
XObjectFactoryDefault::createSpan(
			NodeRefListBase*	theValue,
			bool				/* fOptimize */)
{
	XSpan* const	theXSpan = new XSpan(m_envSupport, m_support, theValue);

	m_xobjects.insert(theXSpan);

#if !defined(NDEBUG)
	++m_totalSpanInstanceCount;
#endif

	return theXSpan;
}



XObject*
XObjectFactoryDefault::createSpan(
			XalanNode&		theValue,
			bool			/* fOptimize */)
{
	XSpan* const	theXSpan = new XSpan(m_envSupport, m_support, theValue);

	m_xobjects.insert(theXSpan);

#if !defined(NDEBUG)
	++m_totalSpanInstanceCount;
#endif

	return theXSpan;
}



void
XObjectFactoryDefault::reset()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(m_xobjects.begin(),
			 m_xobjects.end(),
			 ProtectedDeleteXObjectFunctor(*this, true));

	m_xobjects.clear();
}
