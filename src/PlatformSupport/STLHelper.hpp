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



template <class T>
struct DeleteFunctor : public std::unary_function<const T*, void>
{
	result_type
	operator()(argument_type	thePointer) const
	{
		delete thePointer;
	}
};



template <class PairType>
struct select1st : public std::unary_function<PairType, PairType::first_type>
{
	typedef PairType	value_type;

	result_type
	operator()(const argument_type&		thePair) const
	{
		return thePair.first;
	}
};



template <class PairType>
struct select2nd : public std::unary_function<PairType, PairType::second_type>
{
	typedef PairType	value_type;

	result_type
	operator()(const argument_type&		thePair)
	{
		return thePair.second;
	}
};



template <class OutputIteratorType, class PairMemberSelectType>
struct PairIsolatorOutputIterator
{
	typedef std::output_iterator_tag			iterator_category;
	typedef PairMemberSelectType::value_type	value_type;
	typedef void								difference_type;
	typedef void								pointer;
	typedef void								reference;

	PairIsolatorOutputIterator(
			OutputIteratorType		theOutputIterator,
			PairMemberSelectType	thePairMemberSelector) :
		m_OutputIterator(theOutputIterator),
		m_PairMemberSelector(thePairMemberSelector)
	{
	}

	PairIsolatorOutputIterator&
	operator=(const value_type&		theValue)
	{ 
		m_OutputIterator = m_PairMemberSelector(theValue);
    
		return *this;
	}

	PairIsolatorOutputIterator&
	operator*()
	{
		return *this;
	}

	PairIsolatorOutputIterator&
	operator++()
	{
		++m_OutputIterator;

		return *this;
	} 

	PairIsolatorOutputIterator& operator++(int)
	{
		m_OutputIterator++;

		return *this;
	} 

private:

	OutputIteratorType		m_OutputIterator;
	PairMemberSelectType	m_PairMemberSelector;
};



template <class T>
struct MapValueDeleteFunctor : public std::unary_function<const T::value_type&, void>
{
	result_type
	operator()(argument_type	thePair)
	{
		delete thePair.second;
	}
};



template<class T, class Functor>
struct nested_for_each_functor : public std::unary_function<const T::value_type&, Functor>
{
	nested_for_each_functor(Functor		theFunctor) :
		m_functor(theFunctor)
	{
	}

	result_type
	operator()(argument_type	theContainer)
	{
		return for_each(theContainer.begin(),
						theContainer.end(),
						m_functor);
	}

private:

	Functor		m_functor;
};



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


// This functor is designed to compare 0-terminated arrays.  It substitues for
// the default less<type*> so that pointers to arrays can be compared, rather than
// copies of arrays.  For example, you might want to use C-style strings as keys
// in a map, rather than string objects.  The default algorithm less<const char*>
// would just compare the pointers, and not the vector of characters to which it
// points.  Using this algorithm instead of the default will allow the map to
// work as expected.
template<class T>
struct less_null_terminated_arrays : public std::binary_function<const T*, const T*, bool>
{
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
