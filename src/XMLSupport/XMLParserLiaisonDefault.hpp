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
#if !defined(XMLPARSERLIAISONDEFAULT_HEADER_GUARD_1357924680)
#define XMLPARSERLIAISONDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XMLSupport/XMLSupportDefinitions.hpp>



// Xalan header files.
#include <PlatformSupport/DOMStringHelper.hpp>



#include <DOMSupport/DOMSupportDefault.hpp>
#include <XMLSupport/XMLParserLiaison.hpp>



class XALAN_XMLSUPPORT_EXPORT XMLParserLiaisonDefault : public XMLParserLiaison
{
public:

	/**
	 * Construct a XMLParserLiaisonDefault instance.
	 *
	 * @param formatterListener implemention of the FormatterListener operation
	 *                          (toXMLString, digest, ...)
	 *
	 * @param theDOMSupport        instance of DOMSupport object
	 * @param theParserDescription string description of parser
	 */
	XMLParserLiaisonDefault(
			DOMSupport&				theDOMSupport,
			const XalanDOMString&	theParserDescription =
					XalanDOMString(XALAN_STATIC_UCODE_STRING("(No parser - generic DOM)")));

	virtual
	~XMLParserLiaisonDefault();

	// These interfaces are inherited from Resettable...

	virtual void
	reset();

	// These interfaces are inherited from XMLParserLiaison...

	virtual bool
	supportsSAX() const;

	virtual XalanDocument*
	parseXMLStream(
			InputSource&			inputSource,
			const XalanDOMString&	identifier = XalanDOMString());

	virtual void
	parseXMLStream(
			InputSource&			inputSource,
			DocumentHandler&		handler,
			const XalanDOMString&	identifier = XalanDOMString());

	virtual XalanDocument*
	createDocument();
  
	virtual XalanDocument*
	getDOMFactory();

	virtual XalanDOMString
	getExpandedElementName(const XalanElement&	elem) const;

	virtual XalanDOMString
	getExpandedAttributeName(const XalanAttr&	attr) const;

	virtual const XalanDOMString&
	getSpecialCharacters() const { return m_SpecialCharacters; }

	virtual void
	setSpecialCharacters(const XalanDOMString&	str) { m_SpecialCharacters = str; }

	virtual int
	getIndent() const { return m_Indent; }

	virtual void
	setIndent(int i) { m_Indent = i; }

	virtual bool
	getShouldExpandEntityRefs() const { return m_fShouldExpandEntityRefs; }

	virtual void
	SetShouldExpandEntityRefs(bool b) { m_fShouldExpandEntityRefs = b; }

	virtual bool
	getUseValidation() const { return m_fUseValidation; }

	virtual void
	setUseValidation(bool b) { m_fUseValidation = b; }

	virtual const XalanDOMString&
	getParserDescription() const { return m_ParserDescription; }

protected:

	// Data members...
	DOMSupport&				m_DOMSupport;

private:

	// Not implemented...
	XMLParserLiaisonDefault(const XMLParserLiaisonDefault&);

	XMLParserLiaisonDefault&
	operator=(const XMLParserLiaisonDefault&);

	// Data members...
	XalanDOMString			m_SpecialCharacters;

	int						m_Indent;

	bool					m_fShouldExpandEntityRefs;
	bool					m_fUseValidation;

	const XalanDOMString	m_ParserDescription;
};



#endif	// XMLPARSERLIAISONDEFAULT_HEADER_GUARD_1357924680
