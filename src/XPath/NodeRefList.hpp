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
#if !defined(NODEREFLIST_HEADER_GUARD_1357924680)
#define NODEREFLIST_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <vector>



#include <XPath/NodeRefListBase.hpp>



/**
 * Local implementation of NodeRefList.  This class is for internal use only.
 */
class XALAN_XPATH_EXPORT NodeRefList : public NodeRefListBase
{
public:

	explicit
	NodeRefList();

	/**
	 * Construct a node list from another
	 *
	 * @param theSource source node list
	 */
	NodeRefList(const NodeRefList&	theSource);

	/**
	 * Construct a node list from another
	 *
	 * @param theSource source node list
	 */
	explicit
	NodeRefList(const NodeRefListBase&	theSource);

	virtual
	~NodeRefList();

	NodeRefList&
	operator=(const NodeRefListBase&	theRHS);

	NodeRefList&
	operator=(const NodeRefList&	theRHS);

	// These methods are inherited from NodeRefListBase ...

	virtual XalanNode*
	item(unsigned int	index) const;

	virtual unsigned int
	getLength() const;

	virtual unsigned int
	indexOf(const XalanNode*	theNode) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual NodeRefListBase*
#else
	virtual NodeRefList*
#endif
	clone() const;

#if !defined(NDEBUG)
	bool
	checkForDuplicates() const;
#endif

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanNode*>			NodeListVectorType;
#else
	typedef std::vector<XalanNode*>		NodeListVectorType;
#endif

protected:

	// Default vector allocation size.  It seems high, but
	// it's really worth it...
	enum
	{
		eDefaultVectorSize = 100
	};

	/**
	 * Ensure that an allocation is either the default allocation
	 * amount, or the amount specified in the parameter, whichever
	 * is larger.
	 *
	 * @param theSize The requested size.
	 */
	void
	ensureAllocation(NodeListVectorType::size_type	theSize = 0)
	{
		m_nodeList.reserve(eDefaultVectorSize > theSize ? eDefaultVectorSize : theSize);
	}

	NodeListVectorType	m_nodeList;
};



#endif	// NODEREFLIST_HEADER_GUARD_1357924680
