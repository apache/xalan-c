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



#include <algorithm>
#include <cassert>
#include <set>



#include <XPath/XObject.hpp>
#include <XPath/XPathExecutionContext.hpp>



class XalanNode;
class MutableNodeRefList;
class NodeRefListBase;
class ResultTreeFragBase;
class XObject;



/**
 * This class handles the creation of XObjects and manages their lifetime.
 */
class XALAN_XPATH_EXPORT XObjectFactory
{
public:

	typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

	XObjectFactory();

	virtual
	~XObjectFactory();


	/**
	 * Return an XObject to the factory.
	 * 
	 * @param theXObject The XObject to be returned
	 * @return true if the object belongs to the factory, false if not.
	 */
	bool
	returnObject(const XObject*		theXObject)
	{
		return doReturnObject(theXObject);
	}

	/**
	 * Reset the instance.  This invalidates all existing instances created
	 * with this XObjectFactory.
	 */
	virtual void
	reset() = 0;

	/**
	 * Clone an XObject instance, and hold in the factory.
	 *
	 * @param theXObject the instance to clone
	 * @return a clone of the instance.
	 */
	virtual XObject*
	clone(const XObject&	theXObject) = 0;

	/**
	 * Create a boolean XObject from a boolean value.
	 * 
	 * @param theValue  value used to create object	 
	 * @return pointer to new object
	 */
	virtual XObject*
	createBoolean(
			bool	theValue) = 0;

	/**
	 * Create a node set XObject from a node list.
	 * 
	 * @param theValue  value used to create object.  theValue will be owned by the new XObject.
	 * @return pointer to new object
	 */
	virtual XObject*
	createNodeSet(
			BorrowReturnMutableNodeRefList&		theValue) = 0;

	/**
	 * Create a null XObject.
	 * 
	 * @param theValue  value used to create object	
	 * @return pointer to new object
	 */
	virtual XObject*
	createNull() = 0;

	/**
	 * Create a numeric XObject from a number.
	 * 
	 * @param theValue  value used to create object	
	 * @return pointer to new object
	 */
	virtual XObject*
	createNumber(
			double	theValue) = 0;

	/**
	 * Create a string XObject from a string.
	 * 
	 * @param theValue  value used to create object	 
	 * @return pointer to new object
	 */
	virtual XObject*
	createString(
			const XalanDOMString&	theValue) = 0;

	/**
	 * Create a string XObject from a null-terminated array of characters.
	 * 
	 * @param theValue  a pointer to the array
	 * @return pointer to new object
	 */
	virtual XObject*
	createString(
			const XalanDOMChar*		theValue) = 0;

	/**
	 * Create a string XObject from an array of characters.
	 * 
	 * @param theValue  a pointer to the array
	 * @paran theLength the length of the array
	 * @return pointer to new object
	 */
	virtual XObject*
	createString(
			const XalanDOMChar*		theValue,
			unsigned int			theLength) = 0;

	/**
	 * Create an "unknown" XObject from a string.
	 * 
	 * @param theValue  value used to create object	 
	 * @return pointer to new object
	 */
	virtual XObject*
	createUnknown(
			const XalanDOMString&	theValue) = 0;

	/**
	 * Create a result tree fragment XObject from a result tree fragment.
	 * 
	 * @param theValue  value used to create object.  theValue will be owned by the new XObject.	
	 * @return pointer to new object
	 */
	virtual XObject*
	createResultTreeFrag(
			ResultTreeFragBase*		theValue) = 0;

	/**
	 * Create a span XObject from a node list.
	 * 
	 * @param theValue  value used to create object.  The new object will own the pointer.
	 * @return pointer to new object
	 */
	virtual XObject*
	createSpan(
			BorrowReturnMutableNodeRefList&		theValue) = 0;

	/**
	 *
	 * A public functor for use with stl algorithms.
	 *
	 */
#if defined(XALAN_NO_NAMESPACES)
	struct DeleteXObjectFunctor : public unary_function<const XObject*, void>
#else
	struct DeleteXObjectFunctor : public std::unary_function<const XObject*, void>
#endif
	{
	public:

		DeleteXObjectFunctor(
			XObjectFactory&		theFactoryInstance,
			bool				fInReset = false) :
			m_factoryInstance(theFactoryInstance),
			m_fInReset(fInReset)
		{
		}

		result_type
		operator()(argument_type	theXObject) const
		{
			if (m_fInReset == true)
			{
				m_factoryInstance.doReturnObject(
					theXObject,
					true);
			}
			else
			{
				m_factoryInstance.returnObject(theXObject);
			}
		}

	private:

		XObjectFactory&		m_factoryInstance;

		const bool			m_fInReset;
	};

	friend struct DeleteXObjectFunctor;

protected:

	/**
	 * Delete a FactoryObject instance.
	 *
	 * @param theXObject the XObject instance to delete.
	 */
	void
	deleteObject(const XObject*		theXObject) const
	{
#if defined(XALAN_CANNOT_DELETE_CONST)
		delete (XObject*)theXObject;
#else
		delete theXObject;
#endif
	}

	/**
	 * Return an XObject to the factory.
	 * 
	 * @param theXObject XObject to be returned
	 * @param fInReset true when called during reset().
	 */

	virtual bool
	doReturnObject(
			const XObject*	theXObject,
			bool			fInReset = false) = 0;

private:

	// Not implemented...
	XObjectFactory(const XObjectFactory&);

	XObjectFactory&
	operator=(const XObjectFactory&);

	bool
	operator==(const XObjectFactory&) const;
};



/**
 * Manages the lifetime of an XObject instance.
 */
class XObjectGuard
{
public:

	/**
	 * Construct an XObjectGuard instance from a factory object and an XObject.
	 * 
	 * @param theFactory object that manages lifetime of XObjects
	 * @param theXObject pointer to XObject managed
	 */
	XObjectGuard(
			XObjectFactory&		theFactory,
			const XObject*		theXObject) :
		m_factory(&theFactory),
		m_object(theXObject)
	{
	}

	explicit
	XObjectGuard() :
		m_factory(0),
		m_object(0)
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

	// Note that assignment transfers ownership, just
	// as std::auto_ptr.
	XObjectGuard&
	operator=(XObjectGuard&		theRHS)
	{
		if (&theRHS != this)
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

		return *this;
	}

	/**
	 * Retrieve the object pointer (must not be null)
	 * 
	 * @return pointer to XObject
	 */
	const XObject*
	operator->() const
	{
		assert(m_object != 0);

		return m_object;
	}

	/**
	 * Retrieve the object pointer (may be null)
	 * 
	 * @return pointer to XObject
	 */
	const XObject*
	get() const
	{
		return m_object;
	}

	/**
	 * Return the referenced object to the factory and set the pointers to null.
	 */
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

	/**
	 * Return the referenced object to the factory, if there is one,
	 * and set the pointers to the new object and factory.
	 */
	void
	reset(
			XObjectFactory&		theFactory,
			const XObject*		theXObject)
	{
		if (m_object != 0)
		{
			assert(m_factory != 0);

			m_factory->returnObject(m_object);
		}

		m_object = theXObject;
		m_factory = &theFactory;
	}

	/**
	 * Transfers ownership of XObject to caller
	 * 
	 * @return pointer to XObject
	 */
	const XObject*
	release()
	{
		const XObject* const	theTemp = m_object;

		m_object = 0;

		return theTemp;
	}

private:

	bool
	operator==(const XObjectGuard&) const;


	// Data members...
	XObjectFactory*		m_factory;
    const XObject*		m_object;
};



#endif	// XOBJECTFACTORY_HEADER_GUARD_1357924680
