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
#if !defined(FORMATTERLISTENER_HEADER_GUARD_1357924680)
#define FORMATTERLISTENER_HEADER_GUARD_1357924680

/**
 * $Id$
 * 
 * $State$
 * 
 * @author David N. Bertoni (david_n_bertoni@lotus.com)
 */



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <sax/DocumentHandler.hpp>



#include <XalanDOM/XalanDOMString.hpp>



class PrefixResolver;



/**
 * A SAX-based formatter interface for the XSL processor.  This interface 
 * will be called as result tree elements are constructed.
 */
class XALAN_PLATFORMSUPPORT_EXPORT FormatterListener : public DocumentHandler
{
public:

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


	enum eFormat
	{
		OUTPUT_METHOD_NONE = 0,
		OUTPUT_METHOD_XML = 1,
		OUTPUT_METHOD_HTML = 2,
		OUTPUT_METHOD_TEXT = 3,
		OUTPUT_METHOD_DOM = 4,
		OUTPUT_METHOD_OTHER = 5
	};

	FormatterListener(eFormat	theFormat);

	virtual
	~FormatterListener();


	eFormat
	getOutputFormat() const
	{
		return m_outputFormat;
	}

	/**
	 * Get the PrefixResolver for the FormatterListener
	 *
	 * @return A pointer to the PrefixResolver, if any.
	 */
	const PrefixResolver*
	getPrefixResolver() const
	{
		return m_prefixResolver;
	}

	/**
	 * Set the PrefixResolver for the FormatterListener
	 *
	 * @param thePrefixResolver A pointer to the PrefixResolver, if any.
	 */
	void
	setPrefixResolver(const PrefixResolver*		thePrefixResolver)
	{
		m_prefixResolver = thePrefixResolver;
	}

	/**
	 * Receive notification of character data. If available, when the
	 * disable-output-escaping attribute is used, output raw text without
	 * escaping.
	 *
	 * @param chars  pointer to characters from the XML document
	 * @param length number of characters to read from the array
	 * @exception SAXException
	 */
	virtual void
	charactersRaw(
			const XMLCh* const	chars,
			const unsigned int	length) = 0;

	/**
	 * Called when a Comment is to be constructed.
	 *
	 * @param   data	pointer to comment data
	 * @exception SAXException
	 */
	virtual void
	comment(const XMLCh* const	data) = 0;

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
	 * @param ch     pointer to characters from the XML document
	 * @param length number of characters to read from the array
	 * @exception SAXException
	 */
	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	length) = 0;

	/**
	 * Receive notification of a entityReference.
	 *
	 * @param data pointer to characters from the XML document
	 * @exception SAXException
	 */
	virtual void
	entityReference(const XMLCh* const	name) = 0;


// These methods are inherited from DocumentHandler ...

    virtual void
	characters(
			const XMLCh* const	chars,
			const unsigned int	length) = 0;

    virtual void
	endDocument() = 0;

    virtual void
	endElement(const XMLCh* const	name) = 0;

	virtual void
	ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length) = 0;

	virtual void
	processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data) = 0;

	virtual void
	resetDocument() = 0;

	virtual void
	setDocumentLocator(const Locator* const		locator) = 0;

	virtual void
	startDocument() = 0;

	virtual void
	startElement(
			const	XMLCh* const	name,
			AttributeList&			attrs) = 0;


	// Used when creating PI to work around limitations of
	// our interfaces...
	static const XalanDOMString&	s_xsltNextIsRawString;

	static const XalanDOMString&	s_formatterListenerString;

protected:

	const PrefixResolver*	m_prefixResolver;

private:

	const eFormat			m_outputFormat;
};



#endif	// FORMATTERLISTENER_HEADER_GUARD_1357924680
