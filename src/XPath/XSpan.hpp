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
#if !defined(XSPAN_HEADER_GUARD_1357924680)
#define XSPAN_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



// Base class header file.
#include <XPath/XNodeSet.hpp>



class XALAN_XPATH_EXPORT XSpan : public XNodeSet
{
public:

	/**
	 * Construct an XSpan object from a node list.
	 * 
	 * @param envSupport XPath environment support class instance
	 * @param support    XPath support class instance
	 * @param value      source node list.  The instance will adopt the value instance.
	 */
	XSpan(
			XPathEnvSupport&	envSupport,
			XPathSupport&		support,
			NodeRefListBase*	value = 0);

	/**
	 * Construct an XSpan object from a DOM node.
	 * 
	 * @param envSupport XPath environment support class instance
	 * @param support    XPath support class instance
	 * @param value      source node
	 */
	XSpan(
			XPathEnvSupport&	envSupport,
			XPathSupport&		support,
			XalanNode&			value);

	XSpan(const XSpan&	source);

	virtual
	~XSpan();

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XObject*
#else
	virtual XSpan*
#endif
	clone(void*		theAddress = 0) const;

	/**
	 * Get the start span offset from the first node, or -1 if offset isn't
	 * set.
	 *
	 * @return starting offset
	 */
	virtual int
	getStart() const;

	/**
	 * Set the start span offset from the first node, or pass -1 if offset
	 * isn't set.
	 *
	 * @param start starting offset
	 */
	virtual void
	setStart(int	start);

	/**
	 * Get the end span offset from start of the last node, or -1 if offset
	 * isn't set.
	 *
	 * @return ending offset
	 */
	virtual int
	getEnd() const;

	/**
	 * Set the end span offset from start of the last node, or -1 if offset
	 * isn't set.
	 *
	 * @param end ending offset
	 */
	virtual void
	setEnd(int	end);
  
	/**
	 * Set the range of the span, based on offsets from the first node and
	 * offsets from the last node.
	 *
	 * @param firstNodeOffset offset of first node
	 * @param lastNodeOffset  offset of last node   
	 */
	virtual void
	setSpan(
			int		firstNodeOffset,
			int		lastNodeOffset);

private:

	int		m_start;
	int		m_end;
};



#endif	// XSPAN_HEADER_GUARD_1357924680
