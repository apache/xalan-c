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
#if !defined(XPATHFACTORY_HEADER_GUARD_1357924680)
#define XPATHFACTORY_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



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
