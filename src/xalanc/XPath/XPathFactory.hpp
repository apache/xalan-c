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
#if !defined(XPATHFACTORY_HEADER_GUARD_1357924680)
#define XPATHFACTORY_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>

#include <xalanc/Include/XalanMemoryManagement.hpp>

#include <cassert>
#include <functional>



XALAN_CPP_NAMESPACE_BEGIN



class XPath;



class XALAN_XPATH_EXPORT XPathFactory
{
public:

	explicit
	XPathFactory();

	virtual
	~XPathFactory();

	/**
	 * Return an XPath to the factory.
	 * 
	 * @param theXPath The XPath to be returned
	 * @return true if the object belongs to the factory, false if not.
	 */
	bool
	returnObject(const XPath*	theXPath)
	{
		return doReturnObject(theXPath);
	}

	/**
	 * Reset the instance.  This invalidates all existing instances created
	 * with this XPathFactory.
	 */
	virtual void
	reset() = 0;

	/**
	 * Create an XPath.  The XPath instance is owned by the factory, and should
	 * not be deleted.  The factory will manage the lifetime.
	 *
	 */
	virtual XPath*
	create() = 0;

	/**
	 *
	 * A functor for use with stl algorithms.
	 *
	 */
#if defined(XALAN_NO_STD_NAMESPACE)
	struct DeleteXPathFunctor : public unary_function<const XPath*, void>
#else
	struct DeleteXPathFunctor : public std::unary_function<const XPath*, void>
#endif
	{
	public:

		DeleteXPathFunctor(
			XPathFactory&		theFactoryInstance,
			bool				fInReset = false) :
			m_factoryInstance(theFactoryInstance),
			m_fInReset(fInReset)
		{
		}

		result_type
		operator()(argument_type	theXPath) const
		{
			if (m_fInReset == true)
			{
				m_factoryInstance.doReturnObject(theXPath,
												 true);
			}
			else
			{
				m_factoryInstance.returnObject(theXPath);
			}
		}

	private:

		XPathFactory&		m_factoryInstance;

		const bool			m_fInReset;
	};

	friend struct DeleteXPathFunctor;

protected:

	virtual bool
	doReturnObject(
			const XPath*	theXPath,
			bool			fInReset = false) = 0;
};



/**
 * Manages the lifetime of an XPath instance.
 */
class XPathGuard
{
public:

	/**
	 * Construct an XPathGuard instance from a factory object and an XPath.
	 * 
	 * @param theFactory object that manages lifetime of XPaths
	 * @param theXPath pointer to XPath managed
	 */
	XPathGuard(
			XPathFactory&	theFactory,
			const XPath*	theXPath) :
		m_factory(&theFactory),
		m_object(theXPath)
	{
	}

	// Note that copy construction transfers ownership, just
	// as std::auto_ptr.
	XPathGuard(XPathGuard&	theRHS)
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

	~XPathGuard()
	{
		reset();
	}

	/**
	 * Retrieve the object pointer (must not be null)
	 * 
	 * @return pointer to XPath
	 */
	const XPath*
	operator->() const
	{
		assert(m_object != 0);

		return m_object;
	}

	/**
	 * Retrieve the object pointer (may be null)
	 * 
	 * @return pointer to XPath
	 */
	const XPath*
	get() const
	{
		return m_object;
	}

	/**
	 * Return the referenced object to the factory and set pointers to null.
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
	 * Transfers ownership of XPath to caller
	 * 
	 * @return pointer to XPath
	 */
	const XPath*
	release()
	{
		const XPath* const	theTemp = m_object;

		m_object = 0;

		return theTemp;
	}

private:

	XPathGuard&
	operator=(const XPathGuard&);

	bool
	operator==(const XPathGuard&) const;


	// Data members...
	XPathFactory*	m_factory;
    const XPath*	m_object;
};



XALAN_CPP_NAMESPACE_END



#endif	// XPATHFACTORY_HEADER_GUARD_1357924680
