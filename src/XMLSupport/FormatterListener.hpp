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
#include <XMLSupport/XMLSupportDefinitions.hpp>



#include <sax/DocumentHandler.hpp>



class DOMString;



/**
 * A SAX-based formatter interface for the XSL processor.  This interface 
 * will be called as result tree elements are constructed.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterListener : public DocumentHandler
{
public:

	FormatterListener();

	virtual
	~FormatterListener();

    virtual void
	characters(
			const XMLCh* const	chars,
			const unsigned int	length) = 0;

    virtual void
	charactersRaw(
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
	setDocumentLocator(Locator* const locator) = 0;

	virtual void
	startDocument() = 0;

	virtual void
	startElement(
			const	XMLCh* const	name,
			AttributeList&			attrs) = 0;

	virtual void
	comment(const XMLCh* const	data) = 0;

	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	length) = 0;

	virtual void
	entityReference(const XMLCh* const	name) = 0;
};



#endif	// FORMATTERLISTENER_HEADER_GUARD_1357924680
