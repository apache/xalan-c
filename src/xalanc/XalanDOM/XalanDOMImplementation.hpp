/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined(XALANDOMIMPLEMENTATION_HEADER_GUARD_1357924680)
#define XALANDOMIMPLEMENTATION_HEADER_GUARD_1357924680



#include <xalanc/XalanDOM/XalanDOMDefinitions.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanDocument;
class XalanDocumentType;
class XalanDOMString;



/*
 * <meta name="usage" content="experimental"/>
 *
 * Base class for the DOM DOMImplementation interface.
 *
 * This class is experimental and subject to change!!
 */

class XALAN_DOM_EXPORT XalanDOMImplementation
{
public:

	XalanDOMImplementation();

	virtual
	~XalanDOMImplementation();

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
			const XalanDOMString&	version) = 0;

 //@}

    /** @name Functions introduced in DOM Level 2. */
    //@{

	/**
     * Creates an empty <code>DocumentType</code> node.
     * Entity declarations and notations are not made available.
     * Entity reference expansions and default attribute additions
     * do not occur. It is expected that a future version of the DOM
     * will provide a way for populating a <code>DocumentType</code>.
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
			const XalanDOMString&	systemId) = 0;

    /**
     * Creates an XML <code>Document</code> object of the specified type
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
			const XalanDocumentType&	doctype) = 0;

    //@}

protected:

	XalanDOMImplementation(const XalanDOMImplementation&	theSource);

	XalanDOMImplementation&
	operator=(const XalanDOMImplementation&		theSource);

	bool
	operator==(const XalanDOMImplementation&	theRHS) const;

private:
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined (XALANDOMIMPLEMENTATION_HEADER_GUARD_1357924680)
