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
#if !defined(XALANAUTOPTR_HEADER_GUARD_1357924680)
#define XALANAUTOPTR_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cstddef>



// We're using our own auto_ptr-like class due to wide
// variations amongst the varous platforms we have to
// support
template<class Type>
class XalanAutoPtr
{
public:

	XalanAutoPtr(Type*	thePointer = 0) :
		m_pointer(thePointer)
	{
	}

	XalanAutoPtr(const XalanAutoPtr<Type>&	theSource) :
		m_pointer(((XalanAutoPtr<Type>&)theSource).release())
	{
	}

	XalanAutoPtr<Type>&
	operator=(XalanAutoPtr<Type>&	theRHS)
	{
		if (this != &theRHS)
		{
			// This test ought not to be necessary, but
			// MSVC 6.0 calls delete, which checks for 0.
			// The problem with that is the locking is
			// extremely expensive.
			if (m_pointer != 0)
			{
				delete m_pointer;
			}

			m_pointer = theRHS.release();
		}

		return *this;
	}

	~XalanAutoPtr()
	{
		// See note in operator=() about this...
		if (m_pointer != 0)
		{
			delete m_pointer;
		}
	}

	Type&
	operator*() const
	{
		return *m_pointer;
	}

	Type*
	operator->() const
	{
		return m_pointer;
	}

	Type*
	get() const
	{
		return m_pointer;
	}

	Type*
	release()
	{
		Type* const	temp = m_pointer;

		m_pointer = 0;

		return temp;
	}

	void
	reset(Type*		thePointer = 0)
	{
		// See note in operator=() about this...
		if (m_pointer != 0)
		{
			delete m_pointer;
		}

		m_pointer = thePointer;
	}

private:

	Type*	m_pointer;
};



// A class similar to XalanAutoPtr, but for arrays.
template<class Type>
class XalanArrayAutoPtr
{
public:

	XalanArrayAutoPtr(Type*		thePointer = 0) :
		m_pointer(thePointer)
	{
	}

	XalanArrayAutoPtr(const XalanArrayAutoPtr<Type>&	theSource) :
		m_pointer(((XalanArrayAutoPtr<Type>&)theSource).release())
	{
	}

	XalanArrayAutoPtr<Type>&
	operator=(XalanArrayAutoPtr<Type>&	theRHS)
	{
		if (this != &theRHS)
		{
			// This test ought not to be necessary, but
			// MSVC 6.0 calls delete, which checks for 0.
			// The problem with that is the locking is
			// extremely expensive.
			if (m_pointer != 0)
			{
				delete [] m_pointer;
			}

			m_pointer = theRHS.release();
		}

		return *this;
	}

	~XalanArrayAutoPtr()
	{
		// See note in operator=() about this...
		if (m_pointer != 0)
		{
			delete [] m_pointer;
		}
	}

	Type&
	operator*() const
	{
		return *m_pointer;
	}

	Type&
	operator[](size_t	index) const
	{
		return m_pointer[index];
	}

	Type*
	get() const
	{
		return m_pointer;
	}

	Type*
	release()
	{
		Type* const	temp = m_pointer;

		m_pointer = 0;

		return temp;
	}

	void
	reset(Type*		thePointer = 0)
	{
		// See note in operator=() about this...
		if (m_pointer != 0)
		{
			delete [] m_pointer;
		}

		m_pointer = thePointer;
	}

private:

	Type*	m_pointer;
};



#endif	// if !defined(XALANAUTOPTR_HEADER_GUARD_1357924680)
