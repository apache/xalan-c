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
#if !defined(XMLPARSERLIAISON_HEADER_GUARD_1357924680)
#define XMLPARSERLIAISON_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XMLSupport/XMLSupportDefinitions.hpp>



// Xerces DOM header files
#include <dom/DOM_Node.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOMString.hpp>



#include <PlatformSupport/Resettable.hpp>



class DocumentHandler;
class FormatterListener;
class InputSource;
class URLInputSource;



class XALAN_XMLSUPPORT_EXPORT XMLParserLiaison : public Resettable
{
public:

	XMLParserLiaison();

	virtual
	~XMLParserLiaison();

	// These interfaces are inherited from Resettable...

	/**
	 * Reset for new run.
	 */
	virtual void
	reset() = 0;

	// These interfaces are new to XMLParserLiaison

	/**
	 * Returns true if the liaison supports the SAX DocumentHandler 
	 * interface.
	 */
	virtual bool
	supportsSAX() const = 0;

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
			const DOMString&	identifier = DOMString()) = 0;

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
			const DOMString&	identifier = DOMString()) = 0;

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
			const DOMString&	identifier = DOMString()) = 0;

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
			const DOMString&	identifier = DOMString()) = 0;

	/**
	 * Create an empty DOM Document.  Mainly used for creating an 
	 * output document.
	 */
	virtual DOM_Document
	createDocument() = 0;
  
	virtual DOM_Document
	getDOMFactory() = 0;

	/**
	 * Return the expanded element name.
	 */
	virtual DOMString
	getExpandedElementName(const DOM_Element&	elem) const = 0;

	/**
	 * Returns the attribute name with the namespace expanded.
	 */
	virtual DOMString
	getExpandedAttributeName(const DOM_Attr&	attr) const = 0;

	/**
	 * Set special characters for attributes that will be escaped.
	 */
	virtual void
	setSpecialCharacters(const DOMString&	str) = 0;

	/**
	 * Get special characters for attributes that will be escaped.
	 */
	virtual DOMString
	getSpecialCharacters() const = 0;

	/**
	 * Get the amount to indent when indent-result="yes".
	 */
	virtual int
	getIndent() const = 0;

	/**
	 * Set the amount to indent when indent-result="yes".
	 */
	virtual void
	setIndent(int	i) = 0;

	/**
	 * Get whether or not to expand all entity references in the 
	 * source and style trees.
	 */
	virtual bool
	getShouldExpandEntityRefs() const = 0;

	/**
	 * Set whether or not to expand all entity references in the 
	 * source and style trees.
	 */
	virtual void
	SetShouldExpandEntityRefs(bool	b) = 0;

	/**
	 * Get whether or not validation will be performed.  Validation is off by default.
	 */
	virtual bool
	getUseValidation() const = 0;

	/**
	 * If set to true, validation will be performed.  Validation is off by default.
	 */
	virtual void
	setUseValidation(bool	b) = 0;

	/**
	 * Return a string suitable for telling the user what parser is being used.
	 */
	virtual const DOMString&
	getParserDescription() const = 0;

private:

	// Not implemented
	XMLParserLiaison(const XMLParserLiaison&);

	XMLParserLiaison&
	operator=(const XMLParserLiaison&);
};



#endif	// XMLPARSERLIAISON_HEADER_GUARD_1357924680
