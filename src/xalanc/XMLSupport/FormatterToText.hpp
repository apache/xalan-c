/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#if !defined(FORMATTERTOTEXT_HEADER_GUARD_1357924680)
#define FORMATTERTOTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/FormatterListener.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class Writer;



/**
 * This class takes SAX events (in addition to some extra events 
 * that SAX doesn't handle yet) and produces simple text only.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToText : public FormatterListener
{
public:

	/**
	 * FormatterToText instance constructor.
	 */
	FormatterToText();

	/**
	 * FormatterToText instance constructor.
	 *
	 * @param writer writer for output
	 * @param normalizeLindefeed Normalize \n or \r\n (on certain platforms).
	 * @param handleIgnorableWhitespace If true ignorableWhitespace() will write data to the Writer
	 */
	FormatterToText(
			Writer&		writer,
			bool		normalizeLinefeed = true,
			bool		handleIgnorableWhitespace = true);

	/**
	 * FormatterToText instance constructor.
	 *
	 * @param writer writer for output
	 * @param encoding character encoding for the writer
	 * @param normalizeLindefeed Normalize \n or \r\n on certain platforms.
	 * @param handleIgnorableWhitespace If true ignorableWhitespace() will write data to the Writer
	 */
	FormatterToText(
			Writer&					writer,
			const XalanDOMString&	encoding,
			bool					normalizeLinefeed = true,
			bool					handleIgnorableWhitespace = true);

	virtual
	~FormatterToText();


	Writer*
	getWriter() const
	{
		return m_writer;
	}

	void
	setWriter(Writer*	theWriter)
	{
		m_writer = theWriter;

		update(false);
	}

	void
	clearEncoding();

	const XalanDOMString&
	getEncoding() const
	{
		return m_encoding;
	}

	void
	setEncoding(const XalanDOMString&	theEncoding)
	{
		m_encoding = theEncoding;

		update(false);
	}

	XalanDOMChar
	getMaxCharacter() const
	{
		return m_maxCharacter;
	}

	void
	setMaxCharacter(XalanDOMChar	theMaxChar)
	{
		m_maxCharacter = theMaxChar;
	}

	bool
	getNormalizeLinefeed() const
	{
		return m_normalize;
	}

	void
	setNormalizeLinefeed(bool	fNormalize)
	{
		m_normalize = fNormalize;
	}

	bool
	getHandleIgnorableWhitespace() const
	{
		return m_handleIgnorableWhitespace;
	}

	void
	setHandleIgnorableWhitespace(bool	fHandle)
	{
		m_handleIgnorableWhitespace = fHandle;
	}

	// These methods are inherited from FormatterListener ...

	virtual void
	setDocumentLocator(const LocatorType* const		locator);

	virtual void
	startDocument();

	virtual void
	endDocument();

	virtual void
	startElement(
			const XMLCh* const	name,
			AttributeListType&	attrs);

    virtual void
	endElement(const XMLCh* const	name);

    virtual void
	characters(
			const XMLCh* const	chars,
			const unsigned int	length);

    virtual void
	charactersRaw(
			const XMLCh* const	chars,
			const unsigned int	length);

	virtual void
	entityReference(const XMLCh* const	name);

	virtual void
	ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length);

	virtual void
	processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data);

	virtual void
	resetDocument();

	virtual void
	comment(const XMLCh* const	data);

	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	length);

private:

	// These are not implemented.
	FormatterToText(const FormatterToText&);

	FormatterToText&
	operator=(const FormatterToText&);

	bool
	operator==(const FormatterToText&) const;

	// Utility function to update various member variables
	// when data changes.
	void
	update(bool	fNormalizationOnly);

	// Data members...
	Writer*			m_writer;

	XalanDOMChar	m_maxCharacter;

	XalanDOMString	m_encoding;

	bool			m_haveEncoding;

	bool			m_normalize;

	bool			m_handleIgnorableWhitespace;

	const XalanDOMChar*			m_newlineString;

	XalanDOMString::size_type	m_newlineStringLength;
};



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTOTEXT_HEADER_GUARD_1357924680
