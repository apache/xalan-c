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

// Class header file.
#include "XalanDOMStringAllocator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanDOMStringAllocator::XalanDOMStringAllocator(size_type	theBlockCount) :
	m_allocator(theBlockCount)
{
}



XalanDOMStringAllocator::~XalanDOMStringAllocator()
{
}



XalanDOMStringAllocator::data_type*
XalanDOMStringAllocator::create(
			const char*				theString,
			data_type_size_type		theCount)
{
	data_type* const	theBlock = m_allocator.allocateBlock();
	assert(theBlock != 0);

	data_type* const	theResult = new(theBlock) data_type(theString, theCount);

	m_allocator.commitAllocation(theBlock);

	return theResult;
}



XalanDOMStringAllocator::data_type*
XalanDOMStringAllocator::create(
			const data_type&		theSource,
			data_type_size_type		theStartPosition,
			data_type_size_type		theCount)
{
	data_type* const	theBlock = m_allocator.allocateBlock();
	assert(theBlock != 0);

	data_type* const	theResult = new(theBlock) data_type(theSource, theStartPosition, theCount);

	m_allocator.commitAllocation(theBlock);

	return theResult;
}



XalanDOMStringAllocator::data_type*
XalanDOMStringAllocator::create(
			const XalanDOMChar*		theString,
			data_type_size_type		theCount)
{
	data_type* const	theBlock = m_allocator.allocateBlock();
	assert(theBlock != 0);

	data_type* const	theResult = new(theBlock) data_type(theString, theCount);

	m_allocator.commitAllocation(theBlock);

	return theResult;
}



XalanDOMStringAllocator::data_type*
XalanDOMStringAllocator::create(
			data_type_size_type		theCount,
			XalanDOMChar			theChar)
{
	data_type* const	theBlock = m_allocator.allocateBlock();
	assert(theBlock != 0);

	data_type* const	theResult = new(theBlock) data_type(theCount, theChar);

	m_allocator.commitAllocation(theBlock);

	return theResult;
}



XALAN_CPP_NAMESPACE_END
