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
#if !defined(DOMHELPER_HEADER_GUARD_1357924680)
#define DOMHELPER_HEADER_GUARD_1357924680



// STL header file for class unary_function.
#include <functional>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_NodeList.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#include <dom/DOM_Element.hpp>



class DOM_NullPtr;
class NodeImpl;



#if 0
template<class T>
inline int
operator==(void*		theAddress,
		   const T&		theProxy)
{
	return theProxy.operator==(reinterpret_cast<const DOM_NullPtr*>(theAddress));
}
#else

inline int
operator==(void*			theAddress,
		   const DOM_Node&	theProxy)
{
	return theProxy.operator==(reinterpret_cast<const DOM_NullPtr*>(theAddress));
}



inline int
operator==(void*					theAddress,
		   const DOM_NamedNodeMap&	theProxy)
{
	return theProxy.operator==(reinterpret_cast<const DOM_NullPtr*>(theAddress));
}



inline int
operator==(void*				theAddress,
		   const DOM_NodeList&	theProxy)
{
	return theProxy.operator==(reinterpret_cast<const DOM_NullPtr*>(theAddress));
}



#endif



template<class T>
inline int
operator!=(void*		theAddress,
		   const T&		theProxy)
{
	return !(theAddress == theProxy);
}



template<class T, class U>
class DOM_ConstSurrogate
{
public:

	DOM_ConstSurrogate(const T&		theInstance) :
		m_instance(theInstance)
	{
	}

	const U&
	operator&() const
	{
		return reinterpret_cast<const U&>(m_instance);
	}

	const U&
	operator*() const
	{
		return reinterpret_cast<const U&>(m_instance);
	}

	const U*
	operator->() const
	{
		return reinterpret_cast<const U*>(&m_instance);
	}

protected:

	T	m_instance;
};



template<class T, class U>
class DOM_Surrogate : public DOM_ConstSurrogate<T, U>
{
public:

	DOM_Surrogate(const T&	theInstance) :
		DOM_ConstSurrogate(theInstance)
	{
	}

	U&
	operator&()
	{
		return reinterpret_cast<U&>(m_instance);
	}

	U&
	operator*()
	{
		return reinterpret_cast<U&>(m_instance);
	}

	U*
	operator->()
	{
		return reinterpret_cast<U*>(&m_instance);
	}
};



// This class is a big hack, so we can get at the pointer to the
// internal implementation of a DOM_Node, for the purposes of hashing
// and implementing operator<().
class XALAN_DOM_NodeHack : public DOM_Node
{
public:

	XALAN_DOM_NodeHack(const DOM_Node&		theOtherNode) :
		DOM_Node(theOtherNode)
	{
	}

	XALAN_DOM_NodeHack(NodeImpl*	theOtherNode) :
		DOM_Node(theOtherNode)
	{
	}

	~XALAN_DOM_NodeHack()
	{
	}

	NodeImpl*
	getImplementationObject() const
	{
		return fImpl;
	}

	friend bool operator<(const XALAN_DOM_NodeHack&, const XALAN_DOM_NodeHack&);
	friend struct DOM_NodeHashFunction;
};



// $$$ ToDo:  This sort of address comparison, is undefined behavior
// according to the C++ standard, but it should work on 99.99% of
// architectures.  We're only using it until STL hash containers are
// available.
inline bool
operator<(
			const XALAN_DOM_NodeHack&	theLHS,
			const XALAN_DOM_NodeHack&	theRHS)
{
	return theLHS.fImpl < theRHS.fImpl ? true : false;
}



// Functor for hashing DOM_Nodes.
struct DOM_NodeHashFunction : public std::unary_function<DOM_Node, size_t>
{
	result_type
	operator() (const argument_type&	theKey) const
	{
		return reinterpret_cast<result_type>(XALAN_DOM_NodeHack(theKey).fImpl);
	}
};



// Functor for hashing DOM_Elements.
struct DOM_ElementHashFunction : public std::unary_function<DOM_Element, size_t>
{
	result_type
	operator() (const argument_type&	theKey) const
	{
		return static_cast<result_type>(DOM_NodeHashFunction()(theKey));
	}
};



#endif	// DOMHELPER_HEADER_GUARD_1357924680
