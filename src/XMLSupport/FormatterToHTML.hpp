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
#if !defined(FORMATTERTOHTML_HEADER_GUARD_1357924680)
#define FORMATTERTOHTML_HEADER_GUARD_1357924680

/**
 * $Id$
 * 
 * $State$
 * 
 * @author David N. Bertoni <david_n_bertoni@lotus.com>
 */



// Base include file.  Must be first.
#include <XMLSupport/XMLSupportDefinitions.hpp>



#include <set>
#include <map>
#include <vector>



// Base class header file.
#include <XMLSupport/FormatterToXML.hpp>

#include <XPath/QName.hpp>

typedef std::vector<QName> QNameVectorType;

/**
 * FormatterToHTML formats SAX-style events into HTML.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToHTML : public FormatterToXML 
{  

public:

	/**
	 * Constructor for customized encoding and doctype.
	 * @param writer        The character output stream to use.
	 * @param encoding      Java character encoding in use by <VAR>writer</VAR>.
	 * @param doctype       String to be printed at the top of the document.
	 * @param indent        Number of spaces to indent at each nesting level.
	 */
	FormatterToHTML(
	  Writer&				writer,
	  const DOMString& version,
	  bool doIndent, 
	  int indent,
	  const DOMString& encoding, 
	  const DOMString& mediaType,
	  const DOMString& doctypeSystem,
	  const DOMString& doctypePublic,
	  bool xmlDecl,
	  const DOMString& standalone, 
	  const QNameVectorType* const cdataSectionElems);

	virtual
	~FormatterToHTML();

	/**
	 * Receive notification of the end of a document.
	 *
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 */
	virtual void
	endDocument();

	/**
	 * Receive notification of the beginning of an element.
	 *
	 * <p>The Parser will invoke this method at the beginning of every
	 * element in the XML document; there will be a corresponding
	 * endElement() event for every startElement() event (even when the
	 * element is empty). All of the element's content will be
	 * reported, in order, before the corresponding endElement()
	 * event.</p>
	 *
	 * <p>If the element name has a namespace prefix, the prefix will
	 * still be attached.  Note that the attribute list provided will
	 * contain only attributes with explicit values (specified or
	 * defaulted): #IMPLIED attributes will be omitted.</p>
	 *
	 * @param name The element type name.
	 * @param atts The attributes attached to the element, if any.
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 * @see #endElement
	 * @see org.xml.sax.AttributeList 
	 */
	virtual void
	startElement(
			const	XMLCh* const	name,
			AttributeList&			attrs);

	/**
	 * Receive notification of the end of an element.
	 *
	 * <p>The SAX parser will invoke this method at the end of every
	 * element in the XML document; there will be a corresponding
	 * startElement() event for every endElement() event (even when the
	 * element is empty).</p>
	 *
	 * <p>If the element name has a namespace prefix, the prefix will
	 * still be attached to the name.</p>
	 *
	 * @param name The element type name
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 */
    virtual void
	endElement(const XMLCh* const	name);

	/**
	 * Receive notification of character data.
	 *
	 * <p>The Parser will call this method to report each chunk of
	 * character data.  SAX parsers may return all contiguous character
	 * data in a single chunk, or they may split it into several
	 * chunks; however, all of the characters in any single event
	 * must come from the same external entity, so that the Locator
	 * provides useful information.</p>
	 *
	 * <p>The application must not attempt to read from the array
	 * outside of the specified range.</p>
	 *
	 * <p>Note that some parsers will report whitespace using the
	 * ignorableWhitespace() method rather than this one (validating
	 * parsers must do so).</p>
	 *
	 * @param ch The characters from the XML document.
	 * @param start The start position in the array.
	 * @param length The number of characters to read from the array.
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 * @see #ignorableWhitespace 
	 * @see org.xml.sax.Locator
	 */
    virtual void
	characters(
			const XMLCh* const	chars,
			const unsigned int	length);


	/**
	 * Receive notivication of a entityReference.
	 */
	virtual void
	entityReference(const XMLCh* const	name);


	/**
	 * Receive notification of cdata.
	 *
	 * <p>The Parser will call this method to report each chunk of
	 * character data.  SAX parsers may return all contiguous character
	 * data in a single chunk, or they may split it into several
	 * chunks; however, all of the characters in any single event
	 * must come from the same external entity, so that the Locator
	 * provides useful information.</p>
	 *
	 * <p>The application must not attempt to read from the array
	 * outside of the specified range.</p>
	 *
	 * <p>Note that some parsers will report whitespace using the
	 * ignorableWhitespace() method rather than this one (validating
	 * parsers must do so).</p>
	 *
	 * @param ch The characters from the XML document.
	 * @param start The start position in the array.
	 * @param length The number of characters to read from the array.
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 * @see #ignorableWhitespace 
	 * @see org.xml.sax.Locator
	 */
	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	length);

protected:

	/**
	 * Process an attribute.
	 * @param   name	 The name of the attribute.
	 * @param   value   The value of the attribute.
	 */
	virtual void
	processAttribute(
			const DOMString&	name,
			const DOMString&	value);

	/**
	 * Returns the specified <var>string</var> after substituting non ASCII characters,
	 * with <CODE>%HH</CODE>, where HH is the hex of the byte value.
	 *
	 * @param   string      String to convert to XML format.
	 * @param   specials    Chracters, should be represeted in chracter referenfces.
	 * @param   encoding    CURRENTLY NOT IMPLEMENTED.
	 * @return              XML-formatted string.
	 * @see #backReference
	 * NOTE: return value destroyed on subsequent calls
	 */
	const DOMString& prepAttrURI(
			const DOMString& string,
			const DOMString& specials,
			const DOMString& encoding);
	// java: throws SAXException

public:

	// Static tables.
	typedef std::set<DOMString>		EmptiesSetType;
	typedef	std::vector<DOMString>	HTMLAttributesVectorType;
	typedef std::vector<DOMString>	HTMLSymbolsVectorType;
	typedef std::set<DOMString>		StringSetType;
	// java: hashtable(map) to String vector
	typedef std::map<DOMString, StringSetType> AttributesMapType;

	static EmptiesSetType
	createEmpties();

	static EmptiesSetType
	createAttrEmpties();

	static HTMLAttributesVectorType
	createAttributes();

	static StringSetType
	createNonBlockElems();

	static StringSetType
	createEscapeElems();

	static AttributesMapType
	createAttributesMap();

	static HTMLSymbolsVectorType
	createSymbols();

private:

	static const EmptiesSetType				s_empties;
	static const EmptiesSetType				s_attrempties;
	static const HTMLAttributesVectorType	s_HTMLlat1;
	static const HTMLSymbolsVectorType		s_HTMLsymbol1;
	static const StringSetType					s_nonblockelems;
	static const AttributesMapType			s_attruris;
	static const StringSetType					s_escapetb;

	DOMString								m_currentElementName;

};



#endif	// FORMATTERTOHTML_HEADER_GUARD_1357924680
