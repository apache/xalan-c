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
#if !defined(XOBJECTFACTORYDEFAULT_HEADER_GUARD_1357924680)
#define XOBJECTFACTORYDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#if defined(XALAN_HASH_CONTAINERS_AVAILABLE)
#include <hash_set>
#elif !defined(XALAN_XTREE_BUG)
#include <slist>
#else
#include <vector>
#endif


// Base class header file...
#include <XPath/XObjectFactory.hpp>



class XPathEnvSupport;
class XPathSupport;



class XALAN_XPATH_EXPORT XObjectFactoryDefault : public XObjectFactory
{
public:

	XObjectFactoryDefault(
			XPathEnvSupport&	theEnvSupport,
			XPathSupport&		theSupport);

	virtual
	~XObjectFactoryDefault();


	// Inherited from Factory...

	/**
	 * Reset the factory, deleting all objects that it owns.
	 */
	virtual void
	reset();

	/*
	 * Return an object to the factory.
	 *
	 */
	virtual bool
	returnObject(const FactoryObject*	theFactoryObject);


	// Inherited from XObjectFactory...
	virtual XObject*
	createBoolean(
			bool	theValue,
			bool	fOptimize = true);

	virtual XObject*
	createNodeSet(
			const NodeRefListBase&	value,
			bool					fOptimize = true);

	virtual XObject*
	createNodeSet(
			const MutableNodeRefList&	value,
			bool						fOptimize = true);

	virtual XObject*
	createNodeSet(
			const DOM_Node&		value,
			bool				fOptimize = true);

	virtual XObject*
	createNull(bool	fOptimize = true);

	virtual XObject*
	createNumber(
			double	theValue,
			bool	fOptimize = true);

	virtual XObject*
	createString(
			const DOMString&	theValue,
			bool				fOptimize = true);

	virtual XObject*
	createUnknown(
			const DOMString&	theValue,
			bool				fOptimize = true);

	virtual XObject*
	createResultTreeFrag(
			const ResultTreeFragBase&	theValue,
			bool						fOptimize = true);

	virtual XObject*
	createSpan(
			const NodeRefListBase&	value,
			bool					fOptimize = true);

	virtual XObject*
	createSpan(
			const MutableNodeRefList&	value,
			bool						fOptimize = true);

	virtual XObject*
	createSpan(
			const DOM_Node&		value,
			bool				fOptimize = true);


#if defined(XALAN_HASH_CONTAINERS_AVAILABLE)
	typedef std::hash_set<const FactoryObject*>		CollectionType;
#elif !defined(XALAN_XTREE_BUG)
	typedef std::slist<const FactoryObject*>		CollectionType;
#else
	typedef std::vector<const FactoryObject*>		CollectionType;
#endif

	CollectionType::size_type
	instanceCount() const
	{
		return m_xobjects.size();
	}

private:

	// Not implemented...
	XObjectFactoryDefault(const XObjectFactoryDefault&);

	XObjectFactoryDefault&
	operator=(const XObjectFactoryDefault&);

	bool
	operator==(const XObjectFactoryDefault&) const;


	// Data members...
	XPathEnvSupport&							m_envSupport;
	XPathSupport&								m_support;

	CollectionType								m_xobjects;

	const FactoryObjectAutoPointer<XObject>		m_XNull;
};



#endif	// XOBJECTFACTORYDEFAULT_HEADER_GUARD_1357924680
