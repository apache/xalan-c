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
#if !defined(XALAN_AVT_HEADER_GUARD)
#define XALAN_AVT_HEADER_GUARD 

/**
 * $Id$
 * 
 * $State$
 * 
 */

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <vector>



#include <dom/DOMString.hpp>



#include "AVTPart.hpp"



class PrefixResolver;
class StylesheetConstructionContext;



/**
 * Class to hold an Attribute Value Template.
 */
class AVT: public AVTPart
{
public:

	/**
	 * Construct an Attribute Value Template(AVT) by parsing the string, and
	 * either constructing a vector of AVTParts, or simply hold on to the
	 * string if the AVT is simple.
	 * 
	 * @param name                name of AVT
	 * @param type                type of AVT
	 * @param stringedValue       string value to parse
	 * @param resolver            resolver for namespace resolution
	 * @param constructionContext context for construction of AVT
	 */
	AVT(
			const DOMString&				name,
			const XMLCh*					type,
			const XMLCh*					stringedValue,
			const PrefixResolver&			resolver,
			StylesheetConstructionContext&	constructionContext);

	virtual
	~AVT();

	/**
	 * Retrieve the name of the Attribute Value Template
	 * 
	 * @return name of AVT
	 */
    const DOMString&
	getName() const
	{
		return m_name;
	}

	/**
	 * Retrieve the type of the Attribute Value Template
	 * 
	 * @return type of AVT
	 */
	const XMLCh*
	getType() const
	{
		return m_pcType;
	}

   // These methods are inherited from AVTPart ...

	virtual void
	evaluate(
			DOMString&				buf,
			const DOM_Node&			contextNode,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const;

private:

	// not implemented
	AVT(const AVT &);
	AVT& operator=(const AVT &);

/**
 * If the AVT is not complex, just hold the simple string.
 */
	DOMString				m_simpleString;

/**
 * If the AVT is complex, hold a Vector of AVTParts.
 */
	std::vector<AVTPart*>	m_parts;

/**
 * The name of the attribute.
 */
	DOMString				m_name;

/**
 * The attribute type;
 */
	// $$$ ToDO: Is this OK just to hold a pointer?
	const XMLCh*			m_pcType;
};



#endif	// XALAN_AVT_HEADER_GUARD
