/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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
#if !defined(XERCESDOMIMPLEMENTATIONWRAPPER_HEADER_GUARD_1357924680)
#define XERCESDOMIMPLEMENTATIONWRAPPER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>

#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>

#include <xalanc/XalanDOM/XalanDOMImplementation.hpp>



#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XercesWrapperNavigator;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOMImplementationWrapper : public XalanDOMImplementation
{
public:

    XercesDOMImplementationWrapper(DOMImplementationType*   theXercesDOMImplementation);

    static XercesDOMImplementationWrapper*
    create(MemoryManager& theManager, DOMImplementationType*    theXercesDOMImplementation);

    virtual
    ~XercesDOMImplementationWrapper();

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
            const XalanDOMString&   feature,
            const XalanDOMString&   version);

 //@}

    /** @name Functions introduced in DOM Level 2. */
    //@{

    /**
     * Creates an empty <code>DOMDocumentType</code> node.
     * Entity declarations and notations are not made available.
     * Entity reference expansions and default attribute additions
     * do not occur. It is expected that a future version of the DOM
     * will provide a way for populating a <code>DOMDocumentType</code>.
     * @param qualifiedName The <em>qualified name</em>
     * of the document type to be created.
     * @param publicId The external subset public identifier.
     * @param systemId The external subset system identifier.
     * @return A new <code>DocumentType</code> node with
     * <code>Node.ownerDocument</code> set to <code>null</code>.
     */
    virtual XalanDocumentType*
    createDocumentType(
            const XalanDOMString&   qualifiedName,
            const XalanDOMString&   publicId,
            const XalanDOMString&   systemId);

    /**
     * Creates an XML <code>DOMDocument</code> object of the specified type
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
            const XalanDOMString&       namespaceURI,
            const XalanDOMString&       qualifiedName,
            const XalanDocumentType&    doctype);

    /**
     * Get the Xerces node this instance represent.
     *
     * @return The Xerces node
     */
    DOMImplementationType*
    getXercesNode() const
    {
        return m_xercesNode;
    }

    //@}

private:

    // These are not defined...

    XercesDOMImplementationWrapper(const XercesDOMImplementationWrapper&    theSource);

    XercesDOMImplementationWrapper&
    operator=(const XercesDOMImplementationWrapper&     theSource);

    bool
    operator==(const XercesDOMImplementationWrapper&    theRHS) const;

    // Data members...

    DOMImplementationType* const    m_xercesNode;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined (XERCESDOMIMPLEMENTATIONWRAPPER_HEADER_GUARD_1357924680)
