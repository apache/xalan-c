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
// $ Id: $

#if !defined(XALAN_XSLTRESULTTARGET_HEADER_GUARD)
#define XALAN_XSLTRESULTTARGET_HEADER_GUARD

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"

#include <dom/DOMString.hpp>
#include <dom/DOM_Node.hpp>
#include <sax/DocumentHandler.hpp>
#include <XMLSupport/FormatterListener.hpp>

class OutputStream;
class Writer;


class XALAN_XSLT_EXPORT XSLTResultTarget
{
public:
	/**
	 * Zero-argument default constructor.
	 */
	XSLTResultTarget();

	/**
	 * Create a new output target with a file name.
	 *
	 * @param fileName Must be a valid system file name.
	 */
	XSLTResultTarget (DOMString& fileName); // File?

	/**
	 * Create a new output target with a byte stream.
	 *
	 * @param byteStream The raw byte stream that will contain the document.
	 */
	XSLTResultTarget (OutputStream* byteStream);

	/**
	 * Create a new output target with a character stream.
	 *
	 * @param characterStream The character stream where the results will be written.
	 */ 
	XSLTResultTarget (Writer* characterStream);

	/**
	 * Create a new output target with a character stream.
	 *
	 * @param characterStream The character stream where the results will be written.
	 */
	XSLTResultTarget (DOM_Node& n);

	/**
	 * Set the file name where the results will be written.
	 *
	 * @param fileName The system identifier as a string.
	 */
	void setFileName (DOMString& fileName) // File?
	{
		m_fileName = fileName;
	}


	/**
	 * Get the file name where the results will be written to.
	 * 
	 * @return The file name.
	 */
	DOMString& getFileName ()
	{
		return m_fileName;
	}


	/**
	 * Set the byte stream for this output target.
	 *
	 * @param byteStream A byte stream that will contain the result document.
	 */
	void setByteStream (OutputStream* byteStream)
	{
		m_byteStream = byteStream;
	}


	/**
	 * Get the byte stream for this output target.
	 *
	 * @return The byte stream, or null if none was supplied.
	 */
	OutputStream* getByteStream ()
	{
		return m_byteStream;
	}


	/** 
	 * Set the character encoding, if known.
	 */
	void setEncoding (DOMString& encoding)
	{
		m_encoding = encoding;
	}


	/**
	 * Get the character encoding that was used.
	 *
	 * @return The encoding, or null if none was supplied.
	 */
	DOMString& getEncoding ()
	{
		return m_encoding;
	}


	/**
	 * Set the character stream for this output target.
	 *
	 * @param characterStream The character stream that will contain 
	 *										 the result document.
	 */
	void setCharacterStream (Writer* characterStream)
	{
		m_characterStream = characterStream;
	}


	/**
	 * Get the character stream for this output target.
	 *
	 * @return The character stream, or null if none was supplied.
	 */
	Writer* getCharacterStream ()
	{
		return m_characterStream;
	}

	/**
	 * Set the node that will contain the result nodes.
	 */
	void setNode (const DOM_Node& node)
	{
		m_node = node;
	}

	/**
	 * Get the node that will contain the result nodes.
	 */
	DOM_Node& getNode ()
	{
		return m_node;
	}
	
	/**
	 * Set a SAX DocumentHandler to process the result tree events.
	 */
	void setDocumentHandler (DocumentHandler* handler)
	{
		m_formatterListener = handler;
	}

	/**
	 * Get the SAX DocumentHandler that will process the result tree events.
	 */
	DocumentHandler* getDocumentHandler ()
	{
		return m_formatterListener;
	}

	/**
	 * Set a FormatterListener to process the result tree events.
	 */
	void setFormatterListener (FormatterListener* handler)
	{
		m_formatterListener = static_cast<DocumentHandler*>(handler);
	}

	/**
	 * Get the FormatterListener that will process the result tree events.
	 */
	FormatterListener* getFormatterListener ()
	{
		return static_cast<FormatterListener*>(m_formatterListener);
	}

	
	//////////////////////////////////////////////////////////////////////
	// Internal state.
	//////////////////////////////////////////////////////////////////////

private:

	DOMString m_fileName;
	OutputStream* m_byteStream;
	DOMString m_encoding;
	Writer* m_characterStream;
	DOM_Node m_node;
	DocumentHandler* m_formatterListener;

};

#endif	// XALAN_XSLTRESULTTARGET_HEADER_GUARD
