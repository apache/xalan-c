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
#if !defined(XercesPARSERLIAISON_HEADER_GUARD_1357924680)
#define XercesPARSERLIAISON_HEADER_GUARD_1357924680


// Base include file.  Must be first.
#include <XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



// Standard Library header files.
#include <string>
#include <memory>



// Xerces DOM header files
#include <dom/DOM_Attr.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_Node.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOMString.hpp>

#include <sax/ErrorHandler.hpp>


// Base class header file.
#include <XMLSupport/XMLParserLiaisonDefault.hpp>



class DOMSupport;
class InputSource;
class URLInputSource;
class XSLProcessor;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesParserLiaison : public XMLParserLiaisonDefault, public ErrorHandler
{
public:

	XercesParserLiaison(
			DOMSupport&		theSupport,
			bool			fUseValidatingParser = false);

	virtual
	~XercesParserLiaison();

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

	// Implementations for SAX ErrorHandler

    virtual void warning(const SAXParseException& exception);
    virtual void error(const SAXParseException& exception);
    virtual void fatalError(const SAXParseException& exception);

private:

	const bool	m_fUseValidatingParser;
};



#endif	// XercesPARSERLIAISON_HEADER_GUARD_1357924680
