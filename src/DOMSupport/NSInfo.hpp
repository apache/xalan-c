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
#if !defined(NSINFO_HEADER_GUARD_1357924680)
#define NSINFO_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XMLSupport/XMLSupportDefinitions.hpp>



#include <dom/DOMString.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



// Note that this call has no export definition, since everything is inline.
class NSInfo
{
public:

	enum  eHasXMLNSAttrs { ANCESTORXMLNSUNPROCESSED,
						   ANCESTORHASXMLNS,
						   ANCESTORNOXMLNS };


	/**
	 * Constructor for NSInfo class, for storing namespace data
	 * 
	 * @param hasProcessedNS $$$
	 * @param hasXMLNSAttrs  $$$
	 */
	explicit NSInfo(bool hasProcessedNS = false,
		   bool hasXMLNSAttrs = false) :
		m_hasProcessedNS(hasProcessedNS),
		m_hasXMLNSAttrs(hasXMLNSAttrs),
		m_namespace(),
		m_ancestorHasXMLNSAttrs(ANCESTORXMLNSUNPROCESSED)
	{
	};

  // Unused at the moment
	/**
	 * Constructor for NSInfo class, for storing namespace data
	 * 
	 * @param hasProcessedNS $$$
	 * @param hasXMLNSAttrs  $$$
	 * @param eHasXMLNSAttrs $$$
	 */
	NSInfo(bool				hasProcessedNS,
		   bool				hasXMLNSAttrs,
		   eHasXMLNSAttrs	ancestorHasXMLNSAttrs) :
		m_hasProcessedNS(hasProcessedNS),
		m_hasXMLNSAttrs(hasXMLNSAttrs),
		m_ancestorHasXMLNSAttrs(ancestorHasXMLNSAttrs),
		m_namespace()
	{
	};

	/**
	 * Constructor for NSInfo class, for storing namespace data
	 * 
	 * @param theNamespace  namespace
	 * @param hasXMLNSAttrs $$$
	 */
	NSInfo(DOMString	theNamespace,
		   bool			hasXMLNSAttrs) :
		m_hasProcessedNS(true),
		m_hasXMLNSAttrs(hasXMLNSAttrs),
		m_namespace(theNamespace),
		m_ancestorHasXMLNSAttrs(ANCESTORXMLNSUNPROCESSED)
	{
	};

	~NSInfo()
	{
	}

	bool
	operator==(const NSInfo&	theRHS) const
	{
		return equals(m_namespace, theRHS.m_namespace) &&
			   equals(m_hasXMLNSAttrs, theRHS.m_hasXMLNSAttrs) &&
			   equals(m_hasProcessedNS, theRHS.m_hasProcessedNS) &&
			   equals(m_ancestorHasXMLNSAttrs, theRHS.m_ancestorHasXMLNSAttrs);
	}

	DOMString		m_namespace;
	bool			m_hasXMLNSAttrs;
	bool			m_hasProcessedNS;
	eHasXMLNSAttrs	m_ancestorHasXMLNSAttrs;
};



#endif	// NSINFO_HEADER_GUARD_1357924680
