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



#include <Include/XalanArrayKeyMap.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



/**
 * FormatterToHTML formats SAX-style events into HTML.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToHTML : public FormatterToXML 
{  

public:

#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMChar,
				XalanDOMString,
				less<XalanDOMChar> >	XalanEntityReferenceMapType;
#else
	typedef std::map<XalanDOMChar,
					 XalanDOMString>	XalanEntityReferenceMapType;	
#endif

	/**
	 * Perform static initialization.  See class XMLSupportInit.
	 */
	static void
	initialize();

	/**
	 * Perform static shut down.  See class XMLSupportInit.
	 */
	static void
	terminate();

	enum eDummy
	{
		eDefaultIndentAmount = 0
	};

	/**
	 * Constructor for customized encoding and doctype.
	 * @param writer        The character output stream to use.
	 * @param encoding      Java character encoding in use by <VAR>writer</VAR>.
	 * @param doctype       String to be printed at the top of the document.
	 * @param indent        Number of spaces to indent at each nesting level.
	 */
	FormatterToHTML(
			Writer&					writer,
			const XalanDOMString&	encoding = XalanDOMString(),
			const XalanDOMString&	mediaType = XalanDOMString(),
			const XalanDOMString&	doctypeSystem = XalanDOMString(),
			const XalanDOMString&	doctypePublic = XalanDOMString(),
			bool					doIndent = true,
			int						indent = eDefaultIndentAmount,
			const XalanDOMString&	version = XalanDOMString(),
			const XalanDOMString&	standalone = XalanDOMString(),
			bool					xmlDecl = false);

	virtual
	~FormatterToHTML();

	// These methods are inherited from DocumentHandler ...

	virtual void
	startDocument();

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

	// These methods are inherited from FormatterToXML...

	virtual bool
	accumDefaultEntity(
			XalanDOMChar				ch,
			XalanDOMString::size_type	i,
			const XalanDOMChar			chars[],
			XalanDOMString::size_type	len,
			bool						escLF);

	// These methods are inherited from FormatterListener ...

	virtual void
	entityReference(const XMLCh* const	name);

	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	length);

	virtual void
	processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data);


	class ElemDesc
	{
	public:

		enum eFlags
		{
			EMPTY = (1 << 1),
			FLOW = (1 << 2),
			BLOCK = (1 << 3),
			BLOCKFORM = (1 << 4),
			BLOCKFORMFIELDSET = (1 << 5),
			CDATA = (1 << 6),
			PCDATA = (1 << 7),
			RAW = (1 << 8),
			INLINE = (1 << 9),
			INLINEA = (1 << 10),
			INLINELABEL = (1 << 11),
			FONTSTYLE = (1 << 12),
			PHRASE = (1 << 13),
			FORMCTRL = (1 << 14),
			SPECIAL = (1 << 15),
			ASPECIAL = (1 << 16),
			HEADMISC = (1 << 17),
			HEAD = (1 << 18),
			LIST = (1 << 19),
			PREFORMATTED = (1 << 20),
			WHITESPACESENSITIVE = (1 << 21),
			HEADELEM = (1 << 22),

			ATTRURL = (1 << 1),
			ATTREMPTY = (1 << 2)
		};

		ElemDesc(unsigned int	flags = 0) :
			m_flags(flags)
		{
		}

		~ElemDesc()
		{
		}

		bool
		operator==(const ElemDesc&	theRHS) const
		{
			return m_flags == theRHS.m_flags && m_attrs == theRHS.m_attrs;
		}

		bool
		is(unsigned int		flags) const
		{
			return m_flags & flags ? true : false;
		}

		void
		setAttr(
				const XalanDOMChar*		name,
				unsigned int			flags)
		{
			m_attrs.insert(AttributeMapType::value_type(name, flags));
		}

		bool
		isAttrFlagSet(
				const XalanDOMChar*		name,
				unsigned int			flags) const
		{
			const AttributeMapType::const_iterator	i =
				m_attrs.find(name);

			if (i == m_attrs.end())
			{
				return false;
			}
			else
			{
				return (*i).second & flags ? true : false;
			}
		}

	private:

		typedef XalanArrayKeyMap<
					XalanDOMChar,
					unsigned int,
					less_no_case_ascii_wide_string>		AttributeMapType;

		const unsigned int	m_flags;

		AttributeMapType	m_attrs;
	};


	typedef XalanArrayKeyMap<
				XalanDOMChar,
				ElemDesc,
				less_no_case_ascii_wide_string>		ElementFlagsMapType;

protected:

	virtual void
	writeAttrString(const XalanDOMChar*		theString);

	virtual void
	accumCommentData(const XalanDOMChar*	data);

	void
	writeCharacters(const XalanDOMString&	theString);

	void
	writeCharacters(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

private:

	static const ElementFlagsMapType*							s_elementFlags;

	static const XalanEntityReferenceMapType*					s_xalanHTMLEntities;

	static const XalanEntityReferenceMapType::const_iterator&	s_xalanHTMLEntitiesIteratorEnd; 

	/**
	 * Dummy description for elements not found.
	 */
	static const ElemDesc*			s_dummyDesc;

	/**
	 * The string "<!DOCTYPE  HTML".
	 */
	static const XalanDOMString&	s_doctypeHeaderStartString;

	/**
	 * The string " PUBLIC \"".
	 */
	static const XalanDOMString&	s_doctypeHeaderPublicString;

	/**
	 * The string " SYSTEM".
	 */
	static const XalanDOMString&	s_doctypeHeaderSystemString;

	/**
	 * The string "SCRIPT".
	 */
	static const XalanDOMString&	s_scriptString;

	/**
	 * The string "STYLE".
	 */
	static const XalanDOMString&	s_styleString;

	/**
	 * The string "fnof".
	 */
	static const XalanDOMString&	s_fnofString;

	/**
	 * The string "<META http-equiv=\"Content-Type\" content=\"text/html; charset=".
	 */
	static const XalanDOMString&	s_metaString;

	/**
	 * Set the attribute characters what will require special mapping.
	 */
	void
	initAttrCharsMap();

	/**
	 * Set the output characters what will require special mapping.
	 */
	void
	initCharsMap();

	void
	copyEntityIntoBuffer(const XalanDOMChar*	s);

	void
	copyEntityIntoBuffer(const XalanDOMString&	s);

	/**
	 * Get an ElemDesc instance for the specified name.
	 *
	 * @param name the name to search.
	 * @return a const reference to the ElemDesc instance.
	 */
	static const ElemDesc&
	getElemDesc(const XalanDOMChar*		name);

	/**
	 * Initialize the map of element flags.
	 *
	 * @return map of element flags.
	 */
	static void
	initializeElementFlagsMap(ElementFlagsMapType&	theMap);

	/**
	 * Initialize the map of enity references.
	 *
	 * @return map of enity references.
	 */
	static void
	initializeXalanEntityReferenceMap(XalanEntityReferenceMapType&	theMap);

	/**
	 * Process an attribute.
	 * @param   name	 The name of the attribute.
	 * @param   value   The value of the attribute.
	 */
	virtual void
	processAttribute(
			const XalanDOMChar*		name,
			const XalanDOMChar*		value,
			const ElemDesc&			elemDesc);

	/**
	 * Write the specified string after substituting non ASCII characters,
	 * with %HH, where HH is the hex of the byte value.
	 *
	 * @param   theString     String to convert to XML format.
	 */
	void
	writeAttrURI(const XalanDOMChar*	theString);

	/**
	 * Accumulate the specified character by converting its numeric value to
	 * a hex string, making sure that any string of length 1 are written with
	 * a '0' before the number.
	 *
	 * @param theChar The character to accumulate
	 */
	void
	accumHexNumber(const XalanDOMChar	theChar);

	bool
	popHasNamespace();

	bool
	pushHasNamespace(const XalanDOMChar*	theElementName);

	// Data members...
	XalanDOMString			m_currentElementName;

	bool					m_inBlockElem;

	BoolStackType			m_isRawStack;

	bool					m_isScriptOrStyleElem;

	BoolStackType			m_inScriptElemStack;

	bool					m_escapeURLs;

	/**
	 * A flag so we know whether or not we've put out the first
	 * element.
	 */
	bool					m_isFirstElement;

	/**
	 * A flag so we know whether or not we're writing utf-8.
	 */
	bool					m_isUTF8;

	/**
	 * A counter so we can tell if we're inside the document element.
	 */
	int						m_elementLevel;

	/**
	 * A stack to determine if the current element has
	 * a namespace.
	 */
	BoolStackType			m_hasNamespaceStack;

	static const XalanDOMString		s_emptyString;
};



#endif	// FORMATTERTOHTML_HEADER_GUARD_1357924680
