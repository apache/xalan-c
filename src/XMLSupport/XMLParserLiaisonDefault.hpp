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
#include <DOMSupport/DOMSupportDefault.hpp>
#include <XMLSupport/XMLParserLiaison.hpp>
#include <XMLSupport/Formatter.hpp>



class DOM_Text;



class XALAN_XMLSUPPORT_EXPORT XMLParserLiaisonDefault : public XMLParserLiaison, public Formatter
{
public:

	XMLParserLiaisonDefault(
			DOMSupport&			theDOMSupport,
			const DOMString&	theParserDescription = DOMString("(No parser - generic DOM)"));

	virtual
	~XMLParserLiaisonDefault();

	// These interfaces are inherited from Resettable...

	/**
	 * Reset for new run.
	 */
	virtual void
	reset();

	// These interfaces are inherited from XMLParserLiaison...

	/**
	 * Returns true if the liaison supports the SAX DocumentHandler 
	 * interface.
	 */
	virtual bool
	supportsSAX() const;

	/**
	 * Parse the text pointed at by the reader as XML, and return 
	 * a DOM Document interface.  May return null if not 
	 * supported.  It is recommended that you pass in some sort 
	 * of recognizable name, such as the filename or URI, with 
	 * which the reader can be recognized if the parse fails.
	 * @param reader A stream that should hold valid XML.
	 * @param identifier Used for diagnostic purposes only, 
	 * some sort of identification for error reporting, may be 
	 * an empty string.
	 */
	virtual DOM_Document
	parseXMLStream(
			InputSource&		reader,
			const DOMString&	identifier = DOMString());

	/**
	 * Parse the text pointed at by the reader as XML, and return 
	 * a DOM Document interface.  May return null if not 
	 * supported.  It is recommended that you pass in some sort 
	 * of recognizable name, such as the filename or URI, with 
	 * which the reader can be recognized if the parse fails.
	 * @param reader A URL input source that should hold valid XML.
	 * @param identifier Used for diagnostic purposes only, 
	 * some sort of identification for error reporting, may be 
	 * an empty string.
	 */
	virtual DOM_Document
	parseXMLStream(
			URLInputSource&		reader,
			const DOMString&	identifier = DOMString());

	/**
	 * Parse the text pointed at by the reader as XML.
	 *
	 * @param reader A URL input source that should hold valid XML.
	 * @param handler An instance of a DocumentHandler.
	 * @param identifier Used for diagnostic purposes only, 
	 * some sort of identification for error reporting, may be 
	 * an empty string.
	 */
	virtual void
	parseXMLStream(
			InputSource&		urlInputSource,
			DocumentHandler&	handler,
			const DOMString&	identifier = DOMString());

	/**
	 * Parse the text pointed at by the reader as XML.
	 *
	 * @param reader A URL input source that should hold valid XML.
	 * @param handler An instance of a DocumentHandler.
	 * @param identifier Used for diagnostic purposes only, 
	 * some sort of identification for error reporting, may be 
	 * an empty string.
	 */
	virtual void
	parseXMLStream(
			URLInputSource&		urlInputSource,
			DocumentHandler&	handler,
			const DOMString&	identifier = DOMString());

	/**
	 * Create an empty DOM Document.  Mainly used for creating an 
	 * output document.
	 */
	virtual DOM_Document
	createDocument();
  
	virtual DOM_Document
	getDOMFactory();

	/**
	 * Return the expanded element name.
	 */
	virtual DOMString
	getExpandedElementName(const DOM_Element&	elem) const;

	/**
	 * Returns the attribute name with the namespace expanded.
	 */
	virtual DOMString
	getExpandedAttributeName(const DOM_Attr&	attr) const;

	/**
	 * Set special characters for attributes that will be escaped.
	 */
	virtual void
	setSpecialCharacters(const DOMString&	str) { m_SpecialCharacters = str; }

	/**
	 * Get special characters for attributes that will be escaped.
	 */
	virtual DOMString
	getSpecialCharacters() const { return m_SpecialCharacters; }

	/**
	 * Get the amount to indent when indent-result="yes".
	 */
	virtual int
	getIndent() const { return m_Indent; }

	/**
	 * Set the amount to indent when indent-result="yes".
	 */
	virtual void
	setIndent(int i) { m_Indent = i; }

	/**
	 * Get whether or not to expand all entity references in the 
	 * source and style trees.
	 */
	virtual bool
	getShouldExpandEntityRefs() const { return m_fShouldExpandEntityRefs; }

	/**
	 * Set whether or not to expand all entity references in the 
	 * source and style trees.
	 */
	virtual void
	SetShouldExpandEntityRefs(bool b) { m_fShouldExpandEntityRefs = b; }

	/**
	 * Get whether or not validation will be performed.  Validation is off by default.
	 */
	virtual bool
	getUseValidation() const { return m_fUseValidation; }

	/**
	 * If set to true, validation will be performed.  Validation is off by default.
	 */
	virtual void
	setUseValidation(bool b) { m_fUseValidation = b; }

	/**
	 * Return a string suitable for telling the user what parser is being used.
	 */
	virtual const DOMString&
	getParserDescription() const { return m_ParserDescription; }

	// These interfaces are inherited from Formatter...

	/**
	 * Print the result tree.
	 * @param doc The result tree.
	 * @param pw The PrintWriter to print the contents to.
	 * @param resultns the value of the resultns attribute.
	 * @param format Tells if it should be pretty-printed.
	 * @exception Exception thrown if the result tree is corrupted.
	 */
	virtual void
	toMarkup(
			const DOM_Document&		doc,
			PrintWriter&			pw,
			const DOMString&		resultns,
			bool					format);

	/**
	 * Get an instance of the formatter listener that is associated 
	 * with this formatter.
	 */
	virtual FormatterListener*
	getFormatterListener() const;

	/**
	 * Set an instance of the formatter listener that is associated 
	 * with this formatter.
	 */
	virtual void
	setFormatterListener(
			PrintWriter&		pw,
			const DOMString&	resultns,
			bool				format);

	/**
	 * Set an instance of the formatter listener that is associated 
	 * with this formatter.
	 */
	virtual void
	setFormatterListener(FormatterListener*		fl);

protected:

	// Data members...
	DOMSupport&				m_DOMSupport;

private:

	// Not implemented...
	XMLParserLiaisonDefault(const XMLParserLiaisonDefault&);

	XMLParserLiaisonDefault&
	operator=(const XMLParserLiaisonDefault&);

	// Data members...
	DOMString				m_SpecialCharacters;

	int						m_Indent;

	bool					m_fShouldExpandEntityRefs;
	bool					m_fUseValidation;

	const DOMString			m_ParserDescription;

	FormatterListener*		m_FormatterListener;

	bool					m_fOwnListener;
};



#endif	// XMLPARSERLIAISONDEFAULT_HEADER_GUARD_1357924680
