/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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

// Class header file...
#include "OutputContextStack.hpp"



#include <PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



OutputContextStack::OutputContextStack() :
	m_stack(1),
	m_stackPosition(m_stack.begin())
{
	// m_stack is initialized to a size of 1, so that
	// we always have a dummy entry at the beginning
	// of the deque.  This makes the implementation
	// much simpler.
}



OutputContextStack::~OutputContextStack()
{
}



void
OutputContextStack::pushContext(FormatterListener*	theListener)
{
	++m_stackPosition;

	if (m_stackPosition == m_stack.end())
	{
		m_stack.resize(m_stack.size() + 1);

		m_stackPosition = m_stack.end() - 1;
	}

	if (theListener != 0)
	{
		(*m_stackPosition).m_flistener  = theListener;
	}
}



void
OutputContextStack::popContext()
{
	assert(m_stackPosition != m_stack.begin());

	OutputContext&	theCurrentContext = *m_stackPosition;

	theCurrentContext.reset();

	--m_stackPosition;
}



void
OutputContextStack::clear()
{
	// Since we always keep one dummy entry at the beginning,
	// swap with an OutputContextStackType instance of size 1.
	OutputContextStackType(1).swap(m_stack);

	m_stackPosition = m_stack.begin();
}



void
OutputContextStack::reset()
{
	while(m_stackPosition != m_stack.begin())
	{
		popContext();
	}
}



XALAN_CPP_NAMESPACE_END
