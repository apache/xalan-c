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
#if !defined(XOBJECTFACTORY_HEADER_GUARD_1357924680)
#define XOBJECTFACTORY_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <cassert>
#include <set>



// Base class header file...
#include <PlatformSupport/Factory.hpp>



#include <XPath/XObject.hpp>



class DOM_Node;
class DOMString;
class MutableNodeRefList;
class NodeRefListBase;
class ResultTreeFragBase;
class XObject;



class XALAN_XPATH_EXPORT XObjectFactory : public Factory
{
public:

	XObjectFactory();

	virtual
	~XObjectFactory();

	// These interfaces are inherited from Resetable...

	/**
	 * Reset the instance.  This invalidates all existing FactoryObject
	 * instances created with this Factory.
	 */
	virtual void
	reset() = 0;

	// These interfaces are inherited from Factory...

	/*
	 * Return an object to the factory.
	 *
	 */
	virtual bool
	returnObject(const FactoryObject*	theFactoryObject) = 0;


	// These interfaces are new to XObjectFactory...
	virtual XObject*
	createBoolean(
			bool	theValue,
			bool	fOptimize = true) = 0;

	virtual XObject*
	createNodeSet(
			const NodeRefListBase&	value,
			bool					fOptimize = true) = 0;

	virtual XObject*
	createNodeSet(
			const MutableNodeRefList&	value,
			bool						fOptimize = true) = 0;

	virtual XObject*
	createNodeSet(
			const DOM_Node&		value,
			bool				fOptimize = true) = 0;

	virtual XObject*
	createNull(bool	fOptimize = true) = 0;

	virtual XObject*
	createNumber(
			double	theValue,
			bool	fOptimize = true) = 0;

	virtual XObject*
	createString(
			const DOMString&	theValue,
			bool				fOptimize = true) = 0;

	virtual XObject*
	createUnknown(
			const DOMString&	theValue,
			bool				fOptimize = true) = 0;

	virtual XObject*
	createResultTreeFrag(
			const ResultTreeFragBase&	theValue,
			bool						fOptimize = true) = 0;

	virtual XObject*
	createSpan(
			const NodeRefListBase&	value,
			bool					fOptimize = true) = 0;

	virtual XObject*
	createSpan(
			const MutableNodeRefList&	value,
			bool						fOptimize = true) = 0;

	virtual XObject*
	createSpan(
			const DOM_Node&		value,
			bool				fOptimize = true) = 0;

private:

	// Not implemented...
	XObjectFactory(const XObjectFactory&);

	XObjectFactory&
	operator=(const XObjectFactory&);

	bool
	operator==(const XObjectFactory&) const;
};



class XObjectGuard
{
public:

	XObjectGuard(XObjectFactory&	theFactory,
			     XObject*			theXObject) :
		m_factory(&theFactory),
		m_object(theXObject)
	{
	}

	// Note that copy construction transfers ownership, just
	// as std::auto_ptr.
	XObjectGuard(XObjectGuard&	theRHS)
	{
		// Release the current object...
		release();

		// Copy the factory and object pointers...
		m_factory = theRHS.m_factory;
		m_object = theRHS.m_object;

		// The source object no longer points to
		// the object...
		theRHS.m_factory = 0;
		theRHS.m_object = 0;
	}

	~XObjectGuard()
	{
		reset();
	}

	XObject*
	operator->() const
	{
		assert(m_object != 0);

		return m_object;
	}

	XObject*
	get() const
	{
		return m_object;
	}

	void
	reset()
	{
		if (m_object != 0)
		{
			assert(m_factory != 0);

			m_factory->returnObject(m_object);

			m_object = 0;
		}

		m_factory = 0;
	}

	XObject*
	release()
	{
		XObject* const	theTemp = m_object;

		m_object = 0;

		return theTemp;
	}

private:

	XObjectGuard&
	operator=(const XObjectGuard&);

	bool
	operator==(const XObjectGuard&) const;


	// Data members...
	XObjectFactory*		m_factory;
    XObject*			m_object;
};



#endif	// XOBJECTFACTORY_HEADER_GUARD_1357924680
