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
#if !defined(FACTORY_HEADER_GUARD_1357924680)
#define FACTORY_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cassert>
#include <functional>



#include <PlatformSupport/Resettable.hpp>



class FactoryObject;



class XALAN_PLATFORMSUPPORT_EXPORT Factory : public Resettable
{
public:

	explicit
	Factory();

	virtual
	~Factory();

	// These interfaces are inherited from Resetable...

	/**
	 * Reset the instance.  This invalidates all existing FactoryObject
	 * instances created with this Factory.
	 */
	virtual void
	reset() = 0;

	// These interfaces are new...

	/**
	 * Return an object to the factory.
	 * 
	 * @param theFactoryObject object to be returned
	 */

	bool
	returnObject(const FactoryObject*	theFactoryObject)
	{
		return doReturnObject(theFactoryObject);
	}

protected:

	/**
	 * Delete a FactoryObject instance.
	 */
	virtual bool
	deleteObject(const FactoryObject*	theFactoryObject) const;

	/**
	 * Return an object to the factory.
	 * 
	 * @param theFactoryObject object to be returned
	 * @param fInReset true when called during reset().
	 */

	virtual bool
	doReturnObject(
			const FactoryObject*	theFactoryObject,
			bool					fInReset = false) = 0;

	/**
	 *
	 * A functor for use with stl algorithms.
	 *
	 */
	#if defined(XALAN_NO_NAMESPACES)
	struct ProtectedDeleteFactoryObjectFunctor : public unary_function<const FactoryObject*, void>
	#else
	struct ProtectedDeleteFactoryObjectFunctor : public std::unary_function<const FactoryObject*, void>
	#endif
	{
	public:

		ProtectedDeleteFactoryObjectFunctor(
			Factory&		theFactoryInstance,
			bool			fInReset) :
			m_factoryInstance(theFactoryInstance),
			m_fInReset(fInReset)
		{
		}

		result_type
		operator()(argument_type	theFactoryObject) const
		{
			m_factoryInstance.doReturnObject(theFactoryObject,
											 m_fInReset);
		}

	private:

		Factory&	m_factoryInstance;

		const bool	m_fInReset;
	};

	friend struct ProtectedDeleteFactoryObjectFunctor;

private:

	// Not implemented...
	Factory(const Factory&);

	Factory&
	operator=(const Factory&);

	bool
	operator==(const Factory&) const;
};



// auto_ptr-like class for FactoryObject instances (and it's derivatives).
template<class Type>
class FactoryObjectAutoPointer
{
public:

	// Construct a FactoryObjectAutoPointer.  Either both parameters should
	// be valid or pointers or they should both be 0.
	explicit
	FactoryObjectAutoPointer(
			Factory*	theFactory = 0,
			Type*		theObject = 0) :
		m_factory(theFactory),
		m_object(theObject)
	{
		assert(theFactory != 0 && theObject != 0 ||
			   theFactory == 0 && theObject == 0);

	}

	// Note that copy construction is not const for the source object.  Once
	// copied, the source object will no longer refer to a valid object!
#if defined(XALAN_MEMBER_TEMPLATES)
	template <class CompatibleType>
	FactoryObjectAutoPointer(FactoryObjectAutoPointer<CompatibleType>&	theRHS)
#else
	FactoryObjectAutoPointer(FactoryObjectAutoPointer<Type>&	theRHS)
#endif
	{
		adopt(theRHS);
	}

	~FactoryObjectAutoPointer()
	{
		returnObject();
	}

	Type*
	operator->() const
	{
		return m_object;
	}

	Type*
	get() const
	{
		return m_object;
	}

	void
	reset(Factory*	theFactory = 0,
		  Type*		theObject = 0)
	{
		assert(theFactory != 0 && theObject != 0 ||
			   theFactory == 0 && theObject == 0);

		returnObject();

		m_factory = theFactory;
		m_object = theObject;
	}

	Type*
	release()
	{
		Type* const		theTemp = m_object;

		m_object = 0;
		m_factory = 0;
		
		return theTemp;
	}

	bool
	returnObject()
	{
		bool fReturn = false;

		if (m_object != 0)
		{
			assert(m_factory != 0);

			fReturn = m_factory->returnObject(m_object);
		}

		m_object = 0;
		m_factory = 0;

		return fReturn;
	}

	// Note that assignment is not const for the source object.  Once
	// copied, the source object will no longer refer to a valid object!
#if defined(XALAN_MEMBER_TEMPLATES)
	template <class CompatibleType>
	FactoryObjectAutoPointer&
	operator=(FactoryObjectAutoPointer<CompatibleType&	theRHS)
#else
	FactoryObjectAutoPointer&
	operator=(FactoryObjectAutoPointer<Type>&	theRHS)
#endif
	{
		if (static_cast<void*>(this) != static_cast<void*>(&theRHS))
		{
			adopt(theRHS);
		}

		return *this;
	}

#if defined(XALAN_MEMBER_TEMPLATES)
	template <class CompatibleType>
	bool
	operator==(const FactoryObjectAutoPointer<CompatibleType&		theRHS) const
#else
	bool
	operator==(const FactoryObjectAutoPointer&) const
#endif
	{
		return m_object == theRHS.m_object ? true : false;
	}

private:

#if defined(XALAN_MEMBER_TEMPLATES)
	template <class CompatibleType>
	void
	adopt(FactoryObjectAutoPointer<CompatibleType&	theRHS)
#else
	void
	adopt(FactoryObjectAutoPointer<Type>&	theRHS)
#endif
	{
		if (m_object != theRHS.m_object)
		{
			// Return the object we're now pointing
			// to.
			returnObject();
		}

		// Adopt the object and factory of the source
		// object.
		m_factory = theRHS.m_factory;
		m_object = theRHS.m_object;

		// The source object will no longer refer
		// to the object.
		theRHS.m_object = 0;
		theRHS.m_factory = 0;
	}

	// Data members...
	Factory*	m_factory;
    Type*		m_object;
};



/**
 *
 * A public functor for use with stl algorithms.
 *
 */
#if defined(XALAN_NO_NAMESPACES)
struct DeleteFactoryObjectFunctor : public unary_function<const FactoryObject*, void>
#else
struct DeleteFactoryObjectFunctor : public std::unary_function<const FactoryObject*, void>
#endif
{
public:

	DeleteFactoryObjectFunctor(Factory&		theFactoryInstance) :
		m_factoryInstance(theFactoryInstance)
	{
	}

	result_type
	operator()(argument_type	theFactoryObject) const
	{
		m_factoryInstance.returnObject(theFactoryObject);
	}

private:

	Factory&	m_factoryInstance;
};



#endif	// FACTORY_HEADER_GUARD_1357924680
