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


/**
 * FormatterToHTML formats SAX-style events into HTML.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToHTML : public FormatterToXML 
{  

public:

#if defined(XALAN_NO_NAMESPACES)
#	define XALAN_STD
#else
#	define XALAN_STD std::
#endif
	typedef XALAN_STD vector<QName> QNameVectorType;
	typedef XALAN_STD set<DOMString>		EmptiesSetType;
	typedef XALAN_STD vector<DOMString>	HTMLAttributesVectorType;
	typedef XALAN_STD vector<DOMString>	HTMLSymbolsVectorType;
	typedef XALAN_STD set<DOMString>		StringSetType;
	typedef XALAN_STD map<DOMString, StringSetType> AttributesMapType;
#undef XALAN_STD


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

	
	// These methods are inherited from DocumentHandler ...

	virtual void
	endDocument();

	virtual void
	startElement(
			const	XMLCh* const	name,
			AttributeList&			attrs);

    virtual void
	endElement(const XMLCh* const	name);

    virtual void
	characters(
			const XMLCh* const	chars,
			const unsigned int	length);


	// These methods are inherited from FormatterListener ...

	virtual void
	entityReference(const XMLCh* const	name);


	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	length);


	// These methods are new ...

	/**
	 * Initialize the list of HTML elements that do not contain text, for
	 * example, "BR"
	 *
	 * @return set of strings for elements
	 */
	static EmptiesSetType
	createEmpties();

	/**
	 * Initialize the list of attributes for HTML elements that do not contain
	 * text, for example, "checked."
	 *
	 * @return set of strings for elements
	 */
	static EmptiesSetType
	createAttrEmpties();

	/**
	 * Initialize the list of entity reference names, for example, "nbsp."
	 *
	 * @return vector of strings
	 */
	static HTMLAttributesVectorType
	createAttributes();

	/**
	 * Initialize the list of HTML non-block elements, for example, "BR".
	 * These are present alone and do not have the closing "/>" needed for
	 * valid XML.
	 *
	 * @return vector of strings
	 */
	static StringSetType
	createNonBlockElems();

	/**
	 * Initialize the list of symbols that must be escaped, for example, "<"
	 *
	 * @return set of strings
	 */
	static StringSetType
	createEscapeElems();

	/**
	 * Initialize the map of valid attributes for HTML tags.  For example, the
	 * tag "A" can have attributes of "HREF" and "NAME"
	 *
	 * @return map of attributes to vector of tags
	 */
	static AttributesMapType
	createAttributesMap();

	/**
	 * Initialize the list of names for symbols, for example, "chi" for the
	 * Greek letter Chi.
	 *
	 * @return vector of strings for symbol names
	 */
	static HTMLSymbolsVectorType
	createSymbols();

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
