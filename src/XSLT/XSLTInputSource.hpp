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
 *
 * $ Id: $
 *
 */

#if !defined(XALAN_XSLTINPUTSOURCE_HEADER_GUARD)
#define XALAN_XSLTINPUTSOURCE_HEADER_GUARD

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"

#include <framework/URLInputSource.hpp>
#include <dom/DOM_Node.hpp>
#include <sax/InputSource.hpp>


class BinInputStream;
class InputStream;
class Reader;

class XALAN_XSLT_EXPORT XSLTInputSource : public InputSource
{

public:

	XSLTInputSource() : InputSource("")
	{ }

	/**
	 * Create a new input source with a system identifier.
	 *
	 * <p>Applications may use setPublicId to include a public identifier as
	 * well, or setEncoding to specify the character encoding, if known.</p>
	 *
	 * <p>If the system identifier is a URL, it must be full resolved.</p>
	 *
	 * @param systemId  system identifier (URI)
	 */
	XSLTInputSource(const XMLCh* const systemId);

	/**
	 * Create a new input source with a system identifier.
	 *
	 * <p>Applications may use setPublicId to include a public identifier as
	 * well, or setEncoding to specify the character encoding, if known.</p>
	 *
	 * <p>If the system identifier is a URL, it must be full resolved.</p>
	 *
	 * @param systemId  system identifier (URI)
	 */
	XSLTInputSource(const char* const systemId);

	/**
	 * Create a new input source with a system identifier and a public
	 * identifier.
	 *
	 * <p>If the system identifier is a URL, it must be full resolved.</p>
	 *
	 * @param systemId system identifier (URI)
	 * @param publicId public identifier
	 */
	XSLTInputSource(const char* const systemId, const char* const publicId);

	/**
	 * Create a new input source with a system identifier and a public
	 * identifier.
	 *
	 * <p>If the system identifier is a URL, it must be full resolved.</p>
	 *
	 * @param systemId system identifier (URI)
	 * @param publicId public identifier
	 */
	XSLTInputSource(const XMLCh* const systemId, const XMLCh* const publicId);

	/**
	 * Create a new input source with a byte stream.
	 *
	 * <p>Application writers may use setSystemId to provide a base for
	 * resolving relative URIs, setPublicId to include a public identifier,
	 * and/or setEncoding to specify the object's character encoding.</p>
	 *
	 * @param byteStream pointer to raw byte stream containing the document
	 */
	XSLTInputSource (InputStream* byteStream);

	/**
	 * Create a new input source with a character stream.
	 *
	 * <p>Application writers may use setSystemId() to provide a base 
	 * for resolving relative URIs, and setPublicId to include a 
	 * public identifier.</p>
	 *
	 * @param characterStream pointer to character stream containing the document
	 */
	XSLTInputSource (Reader* characterStream);
	
	/**
	 * Create a new input source with a DOM node.
	 *
	 * <p>Application writers may use setSystemId() to provide a base for
	 * resolving relative URIs, and setPublicId to include a public
	 * identifier.</p>
	 *
	 * @param node DOM node that is root of the document
	 */
	XSLTInputSource (const DOM_Node& node);
	
	/**
	 * Makes the byte stream for this input source.
	 *
	 * <p>The SAX parser will ignore this if there is also a character
	 * stream specified, but it will use a byte stream in preference
	 * to opening a URI connection itself.</p>
	 *
	 *
	 * @return pointer to byte stream created
	 */
	virtual BinInputStream* makeStream() const;
	
	/**
	 * Set the DOM node for this input source.
	 *
	 * @param node DOM node that is root of the document
	 */
	void setNode (const DOM_Node& node);

	/**
	 * Retrieve the DOM node for this input source.
	 *
	 * @return DOM node that is root of the document
	 */
	const DOM_Node& getNode ();

private:
	DOM_Node m_node;
};


#endif	// XALAN_XSLTINPUTSOURCE_HEADER_GUARD

//	$ Log: $
