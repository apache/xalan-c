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
#if !defined(XERCESDOMIMPLEMENTATIONBRIDGE_HEADER_GUARD_1357924680)
#define XERCESDOMIMPLEMENTATIONBRIDGE_HEADER_GUARD_1357924680



#include <XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <XalanDOM/XalanDOMImplementation.hpp>



class DOM_DOMImplementation;
class XercesBridgeNavigator;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOMImplementationBridge : public XalanDOMImplementation
{
public:

	XercesDOMImplementationBridge(DOM_DOMImplementation&	theXercesDOMImplementation);

	virtual
	~XercesDOMImplementationBridge();

	/** @name Getter functions */
	//@{

	/**
	 * Test if the DOM implementation implements a specific feature.
	 *
	 * @param feature The string of the feature to test (case-insensitive). The legal 
	 *        values are defined throughout this specification. The string must be 
	 *        an <EM>XML name</EM> (see also Compliance).
	 * @param version This is the version number of the package name to test.  
	 *   In Level 1, this is the string "1.0". If the version is not specified, 
	 *   supporting any version of the  feature will cause the method to return 
	 *   <code>true</code>. 
	 * @return <code>true</code> if the feature is implemented in the specified 
	 *   version, <code>false</code> otherwise.
	 */
	virtual bool
	hasFeature(
			const XalanDOMString&	feature,
			const XalanDOMString&	version);

 //@}

    /** @name Functions introduced in DOM Level 2. */
    //@{

	/**
     * Creates an empty <code>DOM_DocumentType</code> node.
     * Entity declarations and notations are not made available.
     * Entity reference expansions and default attribute additions
     * do not occur. It is expected that a future version of the DOM
     * will provide a way for populating a <code>DOM_DocumentType</code>.
     * @param qualifiedName The <em>qualified name</em>
     * of the document type to be created.
     * @param publicId The external subset public identifier.
     * @param systemId The external subset system identifier.
     * @return A new <code>DocumentType</code> node with
     * <code>Node.ownerDocument</code> set to <code>null</code>.
     */
	virtual XalanDocumentType*
	createDocumentType(
			const XalanDOMString&	qualifiedName,
			const XalanDOMString&	publicId,
			const XalanDOMString&	systemId);

    /**
     * Creates an XML <code>DOM_Document</code> object of the specified type
     * with its document element.
     * @param namespaceURI The <em>namespace URI</em> of
     * the document element to create, or <code>null</code>.
     * @param qualifiedName The <em>qualified name</em>
     * of the document element to be created.
     * @param doctype The type of document to be created or <code>null</code>.
     * <p>When <code>doctype</code> is not <code>null</code>, its
     * <code>Node.ownerDocument</code> attribute is set to the document
     * being created.
     * @return A new <code>Document</code> object.
     * @exception DOMException
     *   WRONG_DOCUMENT_ERR: Raised if <code>doctype</code> has already
     *   been used with a different document.
     */
    virtual XalanDocument*
	createDocument(
			const XalanDOMString&		namespaceURI,
			const XalanDOMString&		qualifiedName,
			const XalanDocumentType&	doctype);

	/**
	 * Get the Xerces node this instance represent.
	 *
	 * @return The Xerces node
	 */
	DOM_DOMImplementation&
	getXercesNode() const
	{
		return m_xercesNode;
	}

    //@}

private:

	// These are not defined...

	XercesDOMImplementationBridge(const XercesDOMImplementationBridge&	theSource);

	XercesDOMImplementationBridge&
	operator=(const XercesDOMImplementationBridge&		theSource);

	bool
	operator==(const XercesDOMImplementationBridge&	theRHS) const;

	// Data members...

	DOM_DOMImplementation&			m_xercesNode;
};



#endif	// !defined (XERCESDOMIMPLEMENTATIONBRIDGE_HEADER_GUARD_1357924680)
