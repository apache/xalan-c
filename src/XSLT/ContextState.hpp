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
 *
 * $ Id: $
 *
 */

#if !defined(XALAN_CONTEXTSTATE_HEADER_GUARD)
#define XALAN_CONTEXTSTATE_HEADER_GUARD



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <dom/DOM_Node.hpp>



// Base class header file.
#include "StackEntry.hpp"



/**
 * This class marks where a macro call context ends.  Only 
 * calls above this marker at the top of the stack can 
 * be reached with getVariable(String name).
 */
class ContextState : public StackEntry
{
public:

	/**
	 * Construct a context state from a source node and caller node
	 *
	 * @param caller     caller node
	 * @param sourceNode source node
	 */
	ContextState(
			const DOM_Node&		caller,
			const DOM_Node&		sourceNode);

	ContextState(const ContextState&	theSource);

	virtual
	~ContextState();

	/**
	 * Retrieves the caller node
	 *
	 * @return caller node
	 */
	const DOM_Node&
	getCaller() const
	{
		return m_caller;
	}

	/**
	 * Retrieves the source node
	 *
	 * @return source node
	 */
	const DOM_Node&
	getSource() const
	{
		return m_source;
	}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual StackEntry*
#else
	virtual ContextState*
#endif
	clone() const;

	virtual bool
	equals(const StackEntry& rhs) const;

protected:

	ContextState(
			const DOM_Node&		caller,
			const DOM_Node&		sourceNode,
			eStackEntryType		theType);

	bool
	operator==(const ContextState&	theRHS)
	{
		return equals(theRHS);
	}

private:

	// Data members...
	DOM_Node	m_caller;
	DOM_Node	m_source;
};



#endif	// XALAN_CONTEXTSTATE_HEADER_GUARD

/*
 *      $ Log: $
 */
