/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#if !defined(XALAN_OUTPUTCONTEXTSTACK_HEADER_GUARD)
#define XALAN_OUTPUTCONTEXTSTACK_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <deque>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/AttributeListImpl.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class FormatterListener;



class XALAN_XSLT_EXPORT OutputContextStack
{
public:

	struct OutputContext
	{
		OutputContext(FormatterListener*	theListener = 0) :
			m_flistener(theListener),
			m_pendingAttributes(),
			m_pendingElementName(),
			m_hasPendingStartDocument(false),
			m_mustFlushPendingStartDocument(false)
		{
		}

		~OutputContext()
		{
		}

		void
		reset()
		{
			m_flistener = 0;

			m_pendingAttributes.clear();

			m_pendingElementName.clear();

			m_hasPendingStartDocument = false;

			m_mustFlushPendingStartDocument = false;
		}

		FormatterListener*	m_flistener;

		AttributeListImpl	m_pendingAttributes;

		XalanDOMString		m_pendingElementName;

		bool				m_hasPendingStartDocument;

		bool				m_mustFlushPendingStartDocument;
	};

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef deque<OutputContext>			OutputContextStackType;
#else
	typedef std::deque<OutputContext>		OutputContextStackType;
#endif

	typedef OutputContextStackType::size_type	size_type;

	explicit
	OutputContextStack();

	~OutputContextStack();

	void
	pushContext(FormatterListener*	theListener = 0);

	void
	popContext();

	FormatterListener*
	getFormatterListener() const
	{
		return (*m_stackPosition).m_flistener;
	}

	FormatterListener*&
	getFormatterListener()
	{
		return (*m_stackPosition).m_flistener;
	}

	const AttributeListImpl&
	getPendingAttributes() const
	{
		return (*m_stackPosition).m_pendingAttributes;
	}

	AttributeListImpl&
	getPendingAttributes()
	{
		return (*m_stackPosition).m_pendingAttributes;
	}

	const XalanDOMString&
	getPendingElementName() const
	{
		return (*m_stackPosition).m_pendingElementName;
	}

	XalanDOMString&
	getPendingElementName()
	{
		return (*m_stackPosition).m_pendingElementName;
	}

	const bool&
	getHasPendingStartDocument() const
	{
		return (*m_stackPosition).m_hasPendingStartDocument;
	}

	bool&
	getHasPendingStartDocument()
	{
		return (*m_stackPosition).m_hasPendingStartDocument;
	}

	const bool&
	getMustFlushPendingStartDocument() const
	{
		return (*m_stackPosition).m_mustFlushPendingStartDocument;
	}

	bool&
	getMustFlushPendingStartDocument()
	{
		return (*m_stackPosition).m_mustFlushPendingStartDocument;
	}

	size_type
	size() const
	{
		// Since we always keep one dummy entry at the beginning,
		// subtract one from the size
		assert(m_stackSize == size_type(OutputContextStackType::const_iterator(m_stackPosition) - m_stack.begin()));

		return m_stackSize;
	}

	bool
	empty() const
	{
		return size() == 0 ? true : false;
	}

	void
	clear();

	void
	reset();

private:

	// not implemented
	OutputContextStack(const OutputContextStack&);

	bool
	operator==(const OutputContextStack&) const;

	OutputContextStack&
	operator=(const OutputContextStack&);

	/**
	 * A stack to hold the output contexts...
	 */
	OutputContextStackType				m_stack;

	OutputContextStackType::iterator	m_stackPosition;

	size_type							m_stackSize;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_RESULTNAMESPACESSTACK_HEADER_GUARD
