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
#if !defined(XALANAUTOPTR_HEADER_GUARD_1357924680)
#define XALANAUTOPTR_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cstddef>



XALAN_CPP_NAMESPACE_BEGIN



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
#if defined(XALAN_STRICT_ANSI_HEADERS)
	operator[](std::size_t	index) const
#else
	operator[](size_t	index) const
#endif
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



XALAN_CPP_NAMESPACE_END



#endif	// if !defined(XALANAUTOPTR_HEADER_GUARD_1357924680)
