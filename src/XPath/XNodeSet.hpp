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
#if !defined(XNODESET_HEADER_GUARD_1357924680)
#define XNODESET_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <memory>



// Base class header file.
#include <XPath/XObject.hpp>



#include <dom/DOM_DocumentFragment.hpp>
#include <dom/DOMString.hpp>



#include <XPath/ResultTreeFragBase.hpp>
#include <XPath/MutableNodeRefList.hpp>



class NodeRefListBase;



class XPathSupport;



/**
 * Class to hold XPath return types.
 */
class XALAN_XPATH_EXPORT XNodeSet : public XObject
{
public:

	/**
	 * Create an XNodeSet.
	 */
	XNodeSet(
			XPathEnvSupport&		envSupport,
			XPathSupport&			support,
			const NodeRefListBase&	value = MutableNodeRefList());

	XNodeSet(
			XPathEnvSupport&			envSupport,
			XPathSupport&				support,
			const MutableNodeRefList&	value = MutableNodeRefList());

	XNodeSet(
			XPathEnvSupport&	envSupport,
			XPathSupport&		support,
			const DOM_Node&		value);

	XNodeSet(
			const XNodeSet&		source,
			bool				deepClone = true);

	virtual
	~XNodeSet();

	/**
	 * Tell what kind of class this is.
	 */
	virtual	eObjectType
	getType() const;

	/**
	 * Given a request type, return the equivalent string. 
	 * For diagnostic purposes.
	 */
	virtual DOMString
	getTypeString() const;

	/**
	 * Cast result object to a number.
	 */
	virtual double
	num() const;

	/**
	 * Cast result object to a boolean.
	 */
	virtual bool
	boolean() const;

	/**
	 * Cast result object to a string.
	 */
	virtual DOMString
	str() const;

	/**
	 * Cast result object to a result tree fragment.
	 */
	virtual const ResultTreeFragBase&
	rtree() const;

	virtual ResultTreeFragBase&
	rtree();

	/**
	 * Cast result object to a nodelist.
	 */
	virtual const NodeRefListBase&
	nodeset() const;

	/**
	 * Cast result object to a nodelist that is mutable.
	 */
	virtual const MutableNodeRefList&
	mutableNodeset() const;

	virtual MutableNodeRefList&
	mutableNodeset();

	/**
	 * Process a callback request for preferred type information
	 *
	 */
	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject);

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject) const;

	/**
	 * Tell if two objects are functionally equal.
	 */
	virtual bool
	equals(const XObject&	theRHS) const;

private:

	// Not implemented...
	XNodeSet&
	operator=(const XNodeSet&);

	// Data members...
	XPathSupport&								m_support;

	MutableNodeRefList							m_value;

	mutable std::auto_ptr<ResultTreeFragBase>	m_resultTreeFrag;
};



#endif	// XNODESET_HEADER_GUARD_1357924680
