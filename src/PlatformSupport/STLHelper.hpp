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
#if !defined(STLHELPERS_HEADER_GUARD_1357924680)
#define STLHELPERS_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include "PlatformSupportDefinitions.hpp"



#include <functional>


/**
 * Functor to delete objects, used in STL iteration algorithms.
 */
template <class T>
#if defined(XALAN_NO_NAMESPACES)
struct DeleteFunctor : public unary_function<const T*, void>
#else
struct DeleteFunctor : public std::unary_function<const T*, void>
#endif
{
	/**
	 * Delete the object pointed to by argument.
	 *
	 * @param thePointer pointer to object to be deleted
	 */
	result_type
	operator()(argument_type	thePointer) const
	{
		delete thePointer;
	}
};



template<class T>
class array_auto_ptr
{
public:

	array_auto_ptr(T*	thePointer) :
		m_pointer(thePointer)
	{
	}

	~array_auto_ptr()
	{
		delete [] m_pointer;
	}

	T*
	get() const
	{
		return m_pointer;
	}

	T*
	release()
	{
		T* const	temp = m_pointer;

		m_pointer = 0;

		return temp;
	}

private:

	T*	m_pointer;
};



#if ! defined(__GNUC__)

/**
 * Functor to retrieve the key of a key-value pair in a map, used in STL
 * iteration algorithms.
 */
template <class PairType>
#if defined(XALAN_NO_NAMESPACES)
struct select1st : public unary_function<PairType, PairType::first_type>
#else
struct select1st : public std::unary_function<PairType, PairType::first_type>
#endif
{
	typedef PairType	value_type;

	/**
	 * Retrieve the key of a key-value pair.
	 *
	 * @param thePair key-value pair
	 * @return key
	 */
	result_type
	operator()(const argument_type&		thePair) const
	{
		return thePair.first;
	}
};



/**
 * Functor to retrieve the value of a key-value pair in a map, used in STL
 * iteration algorithms.
 */
template <class PairType>
#if defined(XALAN_NO_NAMESPACES)
struct select2nd : public unary_function<PairType, PairType::second_type>
#else
struct select2nd : public std::unary_function<PairType, PairType::second_type>
#endif
{
	typedef PairType	value_type;

	/**
	 * Retrieve the value of a key-value pair.
	 *
	 * @param thePair key-value pair
	 * @return value
	 */
	result_type
	operator()(const argument_type&		thePair)
	{
		return thePair.second;
	}
};

#endif

/*
$$$ What's it all mean ???
*/
template <class OutputIteratorType, class PairMemberSelectType>
struct PairIsolatorOutputIterator
{

/*
#if defined(XALAN_NO_NAMESPACES)
	typedef output_iterator_tag					iterator_category;
#else
	typedef std::output_iterator_tag			iterator_category;
#endif
*/

	typedef typename PairMemberSelectType::value_type        value_type;

/*
	typedef void								difference_type;
	typedef void								pointer;
	typedef void								reference;
*/

	PairIsolatorOutputIterator(
			OutputIteratorType		theOutputIterator,
			PairMemberSelectType	thePairMemberSelector) :
		m_OutputIterator(theOutputIterator),
		m_PairMemberSelector(thePairMemberSelector)
	{
	}

	/**
	 * Changes the output iterator member based on a value.
	 *
	 * @param theValue value to use
	 * @return this object
	 */
	PairIsolatorOutputIterator&
	operator=(const value_type&		theValue)
	{ 
		m_OutputIterator = m_PairMemberSelector(theValue);
    
		return *this;
	}

	/**
	 * Retrieve the object.
	 *
	 * @return this object
	 */
	PairIsolatorOutputIterator&
	operator*()
	{
		return *this;
	}

	/**
	 * Increments the output iterator member.
	 *
	 * @return this object
	 */
	PairIsolatorOutputIterator&
	operator++()
	{
		++m_OutputIterator;

		return *this;
	} 

	/**
	 * Increments the output iterator member.
	 *
	 * @return this object
	 */
	PairIsolatorOutputIterator& operator++(int)
	{
		m_OutputIterator++;

		return *this;
	} 

private:

	OutputIteratorType		m_OutputIterator;
	PairMemberSelectType	m_PairMemberSelector;
};



/**
 * Functor to delete value objects in maps, used in STL iteration algorithms.
 */
template <class T>
#if defined(XALAN_NO_NAMESPACES)
struct MapValueDeleteFunctor : public unary_function<const T::value_type&, void>
#else
struct MapValueDeleteFunctor : public std::unary_function<const typename T::value_type&, void>
#endif
{
	/**
	 * Delete the value object in a map value pair.  The value of the pair must
	 * be of pointer type.
	 *
	 * @param thePair key-value pair
	 */
	result_type
	operator()(argument_type	thePair)
	{
		delete thePair.second;
	}
};



template<class T>
MapValueDeleteFunctor<T>
makeMapValueDeleteFunctor(const T&	/* theMap */)
{
	return MapValueDeleteFunctor<T>();
}



template <class T>
#if defined(XALAN_NO_NAMESPACES)
struct MapKeyDeleteFunctor : public unary_function<const T::value_type&, void>
#else
struct MapKeyDeleteFunctor : public std::unary_function<const typename T::value_type&, void>
#endif
{
	/**
	 * Delete the value object in a map value pair.  The value of the pair must
	 * be of pointer type.
	 *
	 * @param thePair key-value pair
	 */
	result_type
	operator()(argument_type	thePair)
	{
		delete thePair.first;
	}
};



template<class T>
MapKeyDeleteFunctor<T>
makeMapKeyDeleteFunctor(const T&	/* theMap */)
{
	return MapKeyDeleteFunctor<T>();
}



/**
 * Template allows nested execution of for_each algorithm by defining a unary
 * functor that takes a container object as an argument, allowing iteration
 * through the container.
 */
template<class T, class Functor>
#if defined(XALAN_NO_NAMESPACES)
struct nested_for_each_functor : public unary_function<const T::value_type&, Functor>
#else
struct nested_for_each_functor : public std::unary_function<const typename T::value_type&, Functor>
#endif
{
	/**
	 * Construct a nested_for_each_functor instance.
	 *
	 * @param theFunctor functor to use as return type
	 */
	nested_for_each_functor(Functor		theFunctor) :
		m_functor(theFunctor)
	{
	}

	/**
	 * Execute the () operator, with the side effect of calling for_each on
	 * each element in the container argument with the functor member.
	 *
	 * @param theContainer container object
	 */
 	typename T::result_type
	operator()(argument_type	theContainer)
	{
		return for_each(theContainer.begin(),
						theContainer.end(),
						m_functor);
	}

private:

	Functor		m_functor;
};



// $$$ ???
/*
template <class InputIterator, class Function>
Function
nested_for_each(
			InputIterator	first,
			InputIterator	last,
			Function		function)
{

	return function;
}
*/


/**
 * This functor is designed to compare 0-terminated arrays.  It substitutes
 * for the default less<type*> so that pointers to arrays can be compared,
 * rather than copies of arrays.  For example, you might want to use C-style
 * strings as keys in a map, rather than string objects.  The default
 * algorithm less<const char*> would just compare the pointers, and not the
 * vector of characters to which it points.  Using this algorithm instead of
 * the default will allow the map to work as expected.
 */
template<class T>
#if defined(XALAN_NO_NAMESPACES)
struct less_null_terminated_arrays : public binary_function<const T*, const T*, bool>
#else
struct less_null_terminated_arrays : public std::binary_function<const T*, const T*, bool>
#endif
{
	/**
	 * Compare the values of two objects.
	 *
	 *
	 * @param theLHS first object to compare
	 * @param theRHS second object to compare
	 * @return true if objects are the same
	 */
	result_type
	operator()(first_argument_type		theLHS,
			   second_argument_type		theRHS) const
	{
		while(*theLHS && *theRHS)
		{
			if (*theLHS != *theRHS)
			{
				break;
			}
			else
			{
				theLHS++;
				theRHS++;
			}
		}

		return *theLHS < *theRHS ? true : false;
	}
};




#endif	// STLHELPERS_HEADER_GUARD_1357924680
