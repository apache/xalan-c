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
#if !defined(FORMATTERLISTENER_HEADER_GUARD_1357924680)
#define FORMATTERLISTENER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xercesc/sax/DocumentHandler.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class PrefixResolver;
class Writer;



typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator          LocatorType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER AttributeList    AttributeListType;

XALAN_USING_XERCES(Locator)
XALAN_USING_XERCES(AttributeList)


/**
 * A SAX-based formatter interface for the XSL processor.  This interface 
 * will be called as result tree elements are constructed.
 */
class XALAN_PLATFORMSUPPORT_EXPORT FormatterListener : public XERCES_CPP_NAMESPACE_QUALIFIER DocumentHandler
{
public:

    typedef XERCES_CPP_NAMESPACE_QUALIFIER DocumentHandler  ParentType;

    // A handy typedef...  Must match DocumentHandler's type for characters(), etc...
    typedef XalanSize_t     size_type;

    enum eFormat
    {
        OUTPUT_METHOD_NONE = 0,
        OUTPUT_METHOD_XML = 1,
        OUTPUT_METHOD_HTML = 2,
        OUTPUT_METHOD_TEXT = 3,
        OUTPUT_METHOD_DOM = 4,
        OUTPUT_METHOD_OTHER = 5
    };

    enum eXMLVersion
    {
        XML_VERSION_1_0 = 0,
        XML_VERSION_1_1 = 1
    };

    FormatterListener(eFormat   theFormat);

    virtual
    ~FormatterListener();

    /**
     * Get the output format for the instance.
     *
     * @return An enum indicating the output format.
     */
    eFormat
    getOutputFormat() const
    {
        return m_outputFormat;
    }

    /**
     * Get the version of XML the FormatterListener is
     * generating.
     *
     * @return An enum indication the XML version.
     */
    eXMLVersion
    getXMLVersion() const
    {
        return m_XMLVersion;
    }

    /**
     * Determine if the version of XML output is 1.1.
     *
     * @return true if the output version is 1.1, false if not.
     */
    bool
    isXML1_1Version() const
    {
        return ( XML_VERSION_1_1 == m_XMLVersion );
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
    setPrefixResolver(const PrefixResolver*     thePrefixResolver)
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
            const XMLCh* const  chars,
            const size_type     length) = 0;

    /**
     * Called when a Comment is to be constructed.
     *
     * @param   data    pointer to comment data
     * @exception SAXException
     */
    virtual void
    comment(const XMLCh* const  data) = 0;

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
            const XMLCh* const  ch,
            const size_type     length) = 0;

    /**
     * Receive notification of a entityReference.
     *
     * @param data pointer to characters from the XML document
     * @exception SAXException
     */
    virtual void
    entityReference(const XMLCh* const  name) = 0;


// These methods are inherited from DocumentHandler ...

    virtual void
    characters(
            const XMLCh* const  chars,
            const size_type     length) = 0;

    virtual void
    endDocument() = 0;

    virtual void
    endElement(const XMLCh* const   name) = 0;

    virtual void
    ignorableWhitespace(
            const XMLCh* const  chars,
            const size_type     length) = 0;

    virtual void
    processingInstruction(
            const XMLCh* const  target,
            const XMLCh* const  data) = 0;

    virtual void
    resetDocument() = 0;

    virtual void
    setDocumentLocator(const Locator* const     locator) = 0;

    virtual void
    startDocument() = 0;

    virtual void
    startElement(
            const   XMLCh* const    name,
            AttributeList&          attrs) = 0;

    virtual Writer*
    getWriter() const;

    virtual const XalanDOMString&
    getDoctypeSystem() const;

    virtual const XalanDOMString&
    getDoctypePublic() const;

    virtual const XalanDOMString&
    getEncoding() const;

    virtual const XalanDOMString&
    getMediaType() const;

    virtual int
    getIndent() const;


    // Used when creating PI to work around limitations of
    // our interfaces...
    static const XalanDOMChar   s_piTarget[];

    static const XalanDOMChar   s_piData[];

#if defined(XALAN_INLINE_INITIALIZATION)
    static const XalanDOMString::size_type  s_piTargetLength = 5;
    static const XalanDOMString::size_type  s_piDataLength = 3;
#else
    static const XalanDOMString::size_type  s_piTargetLength;
    static const XalanDOMString::size_type  s_piDataLength;
#endif


protected:

    // Data membmers...
    const PrefixResolver*   m_prefixResolver;

    static const XalanDOMString     s_emptyString;

    /**
     * Set the output version during serializing.
     *
     * @param theVersion XML version of the output
     */
    void
    setXMLVersion(eXMLVersion   theVersion)
    {
        m_XMLVersion = theVersion;
    }

private:

    // Not implemented...
    FormatterListener(const FormatterListener&);

    FormatterListener&
    operator=(const FormatterListener&);

    bool
    operator==(const FormatterListener&) const;

    // Data membmers...
    const eFormat   m_outputFormat;

    eXMLVersion     m_XMLVersion;
};



XALAN_CPP_NAMESPACE_END



#endif  // FORMATTERLISTENER_HEADER_GUARD_1357924680
