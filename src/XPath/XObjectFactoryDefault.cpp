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



#include <PlatformSupport/STLHelper.hpp>



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



namespace
{

XBooleanStatic		theTrueBoolean(true);
XBooleanStatic		theFalseBoolean(false);
};



namespace
{

template<class ValueType, class CollectionType>
void
InsertObjectIntoCollection(
			CollectionType&		theCollection,
			const ValueType&	theObject)
{
#if defined(XALAN_HASH_CONTAINERS_AVAILABLE)
	theCollection.insert(theObject);
#else
	theCollection.push_back(theObject);
#endif
}



template<class ValueType, class CollectionType>
bool
RemoveObjectFromCollection(
			CollectionType&		theCollection,
			const ValueType&	theObject)
{
#if defined(XALAN_HASH_CONTAINERS_AVAILABLE)
	return theCollection.erase(theObject) > 0 ? true : false;
#else
	CollectionType::iterator	i =
		std::find(theCollection.begin(),
				  theCollection.end(),
				  theObject);
	
	if (i == theCollection.end())
	{
		return false;
	}
	else
	{
		theCollection.erase(i);

		return true;
	}
#endif
}

};



XObjectFactoryDefault::XObjectFactoryDefault(
			XPathEnvSupport&	theEnvSupport,
			XPathSupport&		theSupport) :
	m_envSupport(theEnvSupport),
	m_support(theSupport),
	m_xobjects(),
	m_XNull(this,
			new XNull(m_envSupport, m_support))
{
}



XObjectFactoryDefault::~XObjectFactoryDefault()
{
	reset();
}



bool
XObjectFactoryDefault::returnObject(const FactoryObject*	theFactoryObject)
{
	if (theFactoryObject == &theTrueBoolean ||
		theFactoryObject == &theFalseBoolean ||
		theFactoryObject == m_XNull.get())
	{
		return true;
	}
	else
	{
		if (RemoveObjectFromCollection(m_xobjects,
									   theFactoryObject) == true)
		{
			return deleteObject(theFactoryObject);
		}
		else
		{
			return false;
		}
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
		XBoolean* const		theBoolean = new XBoolean(m_envSupport, theValue);

		InsertObjectIntoCollection(m_xobjects,
								   theBoolean);

		return theBoolean;
	}
}



XObject*
XObjectFactoryDefault::createNodeSet(
			const NodeRefListBase&	value,
			bool					/* fOptimize */)
{
	XNodeSet* const		theXNodeSet = new XNodeSet(m_envSupport, m_support, value);

	InsertObjectIntoCollection(m_xobjects,
							   theXNodeSet);

	return theXNodeSet;
}



XObject*
XObjectFactoryDefault::createNodeSet(
			const MutableNodeRefList&	value,
			bool					/* fOptimize */)
{
	XNodeSet* const		theXNodeSet = new XNodeSet(m_envSupport, m_support, value);

	InsertObjectIntoCollection(m_xobjects,
							   theXNodeSet);

	return theXNodeSet;
}



XObject*
XObjectFactoryDefault::createNodeSet(
			const DOM_Node&		value,
			bool				/* fOptimize */)
{
	XNodeSet* const		theXNodeSet = new XNodeSet(m_envSupport, m_support, value);

	InsertObjectIntoCollection(m_xobjects,
							   theXNodeSet);

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

		InsertObjectIntoCollection(m_xobjects,
								   theXNull);

		return theXNull;
	}
}



XObject*
XObjectFactoryDefault::createNumber(
			double	theValue,
			bool	/* fOptimize */)
{
	XNumber*	theXNumber = new XNumber(m_envSupport, theValue);

	InsertObjectIntoCollection(m_xobjects,
							   theXNumber);

	return theXNumber;
}



XObject*
XObjectFactoryDefault::createString(
			const DOMString&	theValue,
			bool				/* fOptimize */)
{
	XString* const	theXString = new XString(m_envSupport, m_support, theValue);

	InsertObjectIntoCollection(m_xobjects,
							   theXString);

	return theXString;
}



XObject*
XObjectFactoryDefault::createUnknown(
			const DOMString&	theValue,
			bool				/* fOptimize */)
{
	XUnknown* const	theXUnknown = new XUnknown(m_envSupport, theValue);

	InsertObjectIntoCollection(m_xobjects,
							   theXUnknown);

	return theXUnknown;
}



XObject*
XObjectFactoryDefault::createResultTreeFrag(
			const ResultTreeFragBase&	theValue,
			bool						/* fOptimize */)
{
	XResultTreeFrag* const	theResultTreeFrag = new XResultTreeFrag(m_envSupport, m_support, theValue);

	InsertObjectIntoCollection(m_xobjects,
							   theResultTreeFrag);

	return theResultTreeFrag;
}



XObject*
XObjectFactoryDefault::createSpan(
			const NodeRefListBase&	theValue,
			bool					/* fOptimize */)
{
	XSpan* const	theXSpan = new XSpan(m_envSupport, m_support, theValue);

	InsertObjectIntoCollection(m_xobjects,
							   theXSpan);

	return theXSpan;
}



XObject*
XObjectFactoryDefault::createSpan(
			const MutableNodeRefList&	theValue,
			bool						/* fOptimize */)
{
	XSpan* const	theXSpan = new XSpan(m_envSupport, m_support, theValue);

	InsertObjectIntoCollection(m_xobjects,
							   theXSpan);

	return theXSpan;
}



XObject*
XObjectFactoryDefault::createSpan(
			const DOM_Node&		theValue,
			bool				/* fOptimize */)
{
	XSpan* const	theXSpan = new XSpan(m_envSupport, m_support, theValue);

	InsertObjectIntoCollection(m_xobjects,
							   theXSpan);

	return theXSpan;
}



void
XObjectFactoryDefault::reset()
{
	std::for_each(m_xobjects.begin(),
				  m_xobjects.end(),
				  DeleteFactoryObjectFunctor(*this));

	m_xobjects.clear();
}
