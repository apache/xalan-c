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
#if !defined(XALANXMLSERIALIZERBASE_HEADER_GUARD_1357924680)
#define XALANXMLSERIALIZERBASE_HEADER_GUARD_1357924680




// Base include file.  Must be first.
#include "xalanc/XMLSupport/XMLSupportDefinitions.hpp"



#include "xalanc/Include/XalanVector.hpp"



#include "xalanc/XalanDOM/XalanDOMString.hpp"



// Base class header file.
#include "xalanc/PlatformSupport/FormatterListener.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class XalanOutputStream;



XALAN_USING_XERCES(MemoryManager)



/**
 * XalanXMLSerializerBase serves as a base class for XML serializers based on
 * FormatterListener events.
 */
class XALAN_XMLSUPPORT_EXPORT XalanXMLSerializerBase : public FormatterListener 
{
public:

    /**
     * Perform static initialization.  See class XMLSupportInit.
     */
    static void
    initialize(MemoryManager&   theManager);
 
    /**
     * Perform static shut down.  See class XMLSupportInit.
     */
    static void
    terminate();


    /**
     * Constructor
     *
     * @param theManager        The MemoryManager instance to use for any memory
     *                          allocations
     * @param doctypeSystem     system identifier to be used in the document
     *                          type declaration
     * @param doctypePublic     public identifier to be used in the document
     *                          type declaration
     * @param xmlDecl           true if the XSLT processor should output an XML
     *                          declaration
     * @param theStandalone     The string the XSLT processor should output for
     *                          the standalone document declaration
     */
    XalanXMLSerializerBase(
                MemoryManager&              theManager,
                eXMLVersion                 theXMLVersion,
                const XalanDOMString&       theEncoding,
                const XalanDOMString&       theDoctypeSystem,
                const XalanDOMString&       theDoctypePublic,
                bool                        xmlDecl,
                const XalanDOMString&       theStandalone);

    virtual
    ~XalanXMLSerializerBase();

    MemoryManagerType&
    getMemoryManager()
    {
        return m_elemStack.getMemoryManager();
    }


    // These methods are inherited from FormatterListener ...

    virtual void
    setDocumentLocator(const Locator* const     locator);

    virtual void
    startDocument();

    virtual void
    startElement(
            const XMLCh* const  name,
            AttributeList&      attrs) = 0;

    virtual void
    endElement(const XMLCh* const   name) = 0;

    virtual void
    characters(
            const XMLCh* const  chars,
            const unsigned int  length);

    virtual void
    charactersRaw(
            const XMLCh* const  chars,
            const unsigned int  length) = 0;

    virtual void
    entityReference(const XMLCh* const  name) = 0;

    virtual void
    ignorableWhitespace(
            const XMLCh* const  chars,
            const unsigned int  length);

    virtual void
    processingInstruction(
            const XMLCh* const  target,
            const XMLCh* const  data);

    virtual void
    resetDocument();

    virtual void
    comment(const XMLCh* const  data) = 0;

    virtual void
    cdata(
            const XMLCh* const  ch,
            const unsigned int  length);

    virtual const XalanDOMString&
    getDoctypeSystem() const;

    virtual const XalanDOMString&
    getDoctypePublic() const;

    virtual const XalanDOMString&
    getEncoding() const = 0;

    const XalanDOMString&
    getVersion() const
    {
        return m_version;
    }

    const XalanDOMString&
    getStandalone() const
    {
        return m_standalone;
    }

    bool
    getShouldWriteXMLHeader() const
    {
        return m_shouldWriteXMLHeader;
    }

    void
    setShouldWriteXMLHeader(bool    b)
    {
        m_shouldWriteXMLHeader = b;
    }

    typedef XalanVector<bool>       BoolStackType;


    static const XalanDOMString&    s_1_0String;

    static const XalanDOMString&    s_1_1String;


    class XALAN_XMLSUPPORT_EXPORT UTF8
    {
    public:

        // Static data members...
        /**
         * The string "UTF-8".
         */
        static const XalanDOMString&    s_encodingString;

        /**
         * The string "<!DOCTYPE ".
         */
        static const char                       s_doctypeHeaderStartString[];

        static const XalanDOMString::size_type  s_doctypeHeaderStartStringLength;

        /**
         * The string " PUBLIC \"".
         */
        static const char                       s_doctypeHeaderPublicString[];

        static const XalanDOMString::size_type  s_doctypeHeaderPublicStringLength;

        /**
         * The string " SYSTEM \"".
         */
        static const char                       s_doctypeHeaderSystemString[];

        static const XalanDOMString::size_type  s_doctypeHeaderSystemStringLength;

        /**
         * The string "<?xml version=\"".
         */
        static const char                       s_xmlHeaderStartString[];

        static const XalanDOMString::size_type  s_xmlHeaderStartStringLength;

        /**
         * The string "\" encoding=\"".
         */
        static const char                       s_xmlHeaderEncodingString[];

        static const XalanDOMString::size_type  s_xmlHeaderEncodingStringLength;

        /**
         * The string "\" standalone=\"".
         */
        static const char                       s_xmlHeaderStandaloneString[];

        static const XalanDOMString::size_type  s_xmlHeaderStandaloneStringLength;

        /**
         * The string "\"?>".
         */
        static const char                       s_xmlHeaderEndString[];

        static const XalanDOMString::size_type  s_xmlHeaderEndStringLength;

        /**
         * The string "1.0".
         */
        static const char                       s_defaultVersionString[];

        static const XalanDOMString::size_type  s_defaultVersionStringLength;

        /**
         * The string "-//W3C//DTD XHTML".
         */
        static const XalanDOMChar               s_xhtmlDocTypeString[];

        static const XalanDOMString::size_type  s_xhtmlDocTypeStringLength;

        /**
         * The string "<![CDATA[".
         */
        static const char                       s_cdataOpenString[];

        static const XalanDOMString::size_type  s_cdataOpenStringLength;

        /**
         * The string "]]>".
         */
        static const char                       s_cdataCloseString[];

        static const XalanDOMString::size_type  s_cdataCloseStringLength;

        /**
         * The string "&lt;".
         */
        static const char                       s_lessThanEntityString[];

        static const XalanDOMString::size_type  s_lessThanEntityStringLength;

        /**
         * The string "&gt;".
         */
        static const char                       s_greaterThanEntityString[];

        static const XalanDOMString::size_type  s_greaterThanEntityStringLength;

        /**
         * The string "&amp;".
         */
        static const char                       s_ampersandEntityString[];

        static const XalanDOMString::size_type  s_ampersandEntityStringLength;

        /**
         * The string "&quot;".
         */
        static const char                       s_quoteEntityString[];

        static const XalanDOMString::size_type  s_quoteEntityStringLength;
    };

    class XALAN_XMLSUPPORT_EXPORT UTF16
    {
    public:

        /**
         * The string "UTF-16".
         */
        static const XalanDOMString&    s_encodingString;

        /**
         * The string "<!DOCTYPE ".
         */
        static const XalanDOMChar               s_doctypeHeaderStartString[];

        static const XalanDOMString::size_type  s_doctypeHeaderStartStringLength;

        /**
         * The string " PUBLIC \"".
         */
        static const XalanDOMChar               s_doctypeHeaderPublicString[];

        static const XalanDOMString::size_type  s_doctypeHeaderPublicStringLength;

        /**
         * The string " SYSTEM \"".
         */
        static const XalanDOMChar               s_doctypeHeaderSystemString[];

        static const XalanDOMString::size_type  s_doctypeHeaderSystemStringLength;

        /**
         * The string "<?xml version=\"".
         */
        static const XalanDOMChar               s_xmlHeaderStartString[];

        static const XalanDOMString::size_type  s_xmlHeaderStartStringLength;

        /**
         * The string "\" encoding=\"".
         */
        static const XalanDOMChar               s_xmlHeaderEncodingString[];

        static const XalanDOMString::size_type  s_xmlHeaderEncodingStringLength;

        /**
         * The string "\" standalone=\"".
         */
        static const XalanDOMChar               s_xmlHeaderStandaloneString[];

        static const XalanDOMString::size_type  s_xmlHeaderStandaloneStringLength;

        /**
         * The string "\"?>".
         */
        static const XalanDOMChar               s_xmlHeaderEndString[];

        static const XalanDOMString::size_type  s_xmlHeaderEndStringLength;

        /**
         * The string "1.0".
         */
        static const XalanDOMChar               s_defaultVersionString[];

        static const XalanDOMString::size_type  s_defaultVersionStringLength;

        /**
         * The string "-//W3C//DTD XHTML".
         */
        static const XalanDOMChar               s_xhtmlDocTypeString[];

        static const XalanDOMString::size_type  s_xhtmlDocTypeStringLength;

        /**
         * The string "<![CDATA[".
         */
        static const XalanDOMChar               s_cdataOpenString[];

        static const XalanDOMString::size_type  s_cdataOpenStringLength;

        /**
         * The string "]]>".
         */
        static const XalanDOMChar               s_cdataCloseString[];

        static const XalanDOMString::size_type  s_cdataCloseStringLength;

        /**
         * The string "&lt;".
         */
        static const XalanDOMChar               s_lessThanEntityString[];

        static const XalanDOMString::size_type  s_lessThanEntityStringLength;

        /**
         * The string "&gt;".
         */
        static const XalanDOMChar               s_greaterThanEntityString[];

        static const XalanDOMString::size_type  s_greaterThanEntityStringLength;

        /**
         * The string "&amp;".
         */
        static const XalanDOMChar               s_ampersandEntityString[];

        static const XalanDOMString::size_type  s_ampersandEntityStringLength;

        /**
         * The string "&quot;".
         */
        static const XalanDOMChar               s_quoteEntityString[];

        static const XalanDOMString::size_type  s_quoteEntityStringLength;
    };

    enum
    {
        eBufferSize = 512       // The size of the buffer
    };

    class XALAN_XMLSUPPORT_EXPORT CharFunctor1_0 
    {
    public:

        bool
        attribute(XalanDOMChar  theChar) const
        {
            return theChar > s_lastSpecial ?
                        false :
                        s_specialChars[theChar] > eNone;
        }

        bool
        content(XalanDOMChar  theChar) const
        {
            return theChar > s_lastSpecial ?
                        false :
                        s_specialChars[theChar] > eAttr;
        }

        bool
        range(XalanDOMChar  theChar) const
        {
            assert(theChar > 0);

            return theChar > s_lastSpecial;
        }

        bool
        isForbidden(XalanDOMChar  theChar) const
        {
            return theChar > s_lastSpecial ?
                        false :
                        s_specialChars[theChar] == eForb;
        }

        bool
        isCharRefForbidden(XalanDOMChar  theChar) const
        {
            return theChar > s_lastSpecial ?
                        false :
                        s_specialChars[theChar] == eForb;
        }
 

    private:
         static const size_t    s_lastSpecial;

         static const char      s_specialChars[];
    };



    class XALAN_XMLSUPPORT_EXPORT CharFunctor1_1
    {
    public:

        bool
        attribute(XalanDOMChar  theChar) const
        {
            return theChar > s_lastSpecial ?
                        false :
                        s_specialChars[theChar] > eNone;
        }

        bool
        content(XalanDOMChar  theChar) const
        {
            return theChar > s_lastSpecial ?
                        false :
                        s_specialChars[theChar] > eAttr;
        }

        bool
        range(XalanDOMChar  theChar) const
        {
            assert(theChar > 0);

            return theChar > s_lastSpecial;
        }

        bool
        isForbidden(XalanDOMChar  theChar) const
        {
            return theChar > s_lastSpecial ?
                        false :
                        s_specialChars[theChar] == eForb;
        }

        bool
        isCharRefForbidden(XalanDOMChar  theChar) const
        {
            return theChar > s_lastSpecial ?
                        false :
                        s_specialChars[theChar] == eCRFb;
        }

    private:
        static const size_t     s_lastSpecial;

        static const char       s_specialChars[];

    };

  


    enum
    {
        eNone = 0u,
        eAttr = 1u,  // A flag to indicate a value in s_specialChars applies to attributes
        eBoth = 2u,  // A flag to indicate a value in s_specialChars applies to both content and attributes
        eForb = 4u,  // A flag to indicate a forbidden value in s_specialChars 
                     // XML1.1 put a requirement to output chars #x1...#x1F and #x7F...#x9F as charRefs only
                     // In the comments , PI and CDATA usage of charRefs is forbidden, so we will report an error in 
        eCRFb = 5u   // that case. For the elemets and attributes is should work the same as eBoth
    };
 


protected:

    virtual void
    writeXMLHeader() = 0;

    virtual void
    flushBuffer() = 0;

    virtual void
    writeDoctypeDecl(const XalanDOMChar*    name) = 0;

    virtual void
    writeProcessingInstruction(
            const XMLCh*    target,
            const XMLCh*    data) = 0;

    virtual void
    writeCharacters(
            const XMLCh*    chars,
            unsigned int    length) = 0;

    virtual void
    writeCDATA(
            const XMLCh*    chars,
            unsigned int    length) = 0;

    virtual void
    outputNewline() = 0;

    /**
     * Mark the parent element as having a child.  If this
     * is the first child, return true, otherwise, return
     * false.  This allows the child element to determine
     * if the parent tag has already been closed.
     *
     * @return true if the parent element has not been previously marked for children.
     */
    bool
    markParentForChildren()
    {
        if(!m_elemStack.empty())
        {
            // See if the parent element has already been flagged as having children.
            if(false == m_elemStack.back())
            {
                m_elemStack.back() = true;

                return true;
            }
        }

        return false;
    }

    /**
     * Determine if it a DOCTYPE declaration needs to
     * be written.
     */
    bool
    getNeedToOutputDoctypeDecl() const
    {
        return m_needToOutputDoctypeDecl;
    }

    /**
     * Open an element for possibile children
     */
    void
    openElementForChildren()
    {
        m_elemStack.push_back(false);
    }

    bool
    outsideDocumentElement() const
    {
        return m_elemStack.empty();
    }

    /**
     * Determine if an element ever had any children added.
     *
     * @return true if the children were added, false if not.
     */
    bool
    childNodesWereAdded()
    {
        bool    fResult = false;

        if (m_elemStack.empty() == false)
        {
            fResult = m_elemStack.back();

            m_elemStack.pop_back();
        }

        return fResult;
    }

    void
    generateDoctypeDecl(const XalanDOMChar*     name)
    {
        if(true == m_needToOutputDoctypeDecl)          
        {
            assert(m_doctypeSystem.empty() == false);

            writeDoctypeDecl(name);

            m_needToOutputDoctypeDecl = false;
        }
    }

    /**
     * Tell if the next text should be raw.
     */
    bool        m_nextIsRaw;

    /**
     * Add space before '/>' for XHTML.
     */
    bool        m_spaceBeforeClose;

    /**
     * The System ID for the doc type.
     */
    const XalanDOMString    m_doctypeSystem;

    /**
     * The public ID for the doc type.
     */
    const XalanDOMString    m_doctypePublic;

    /**
     * Tells the XML version, for writing out to the XML decl.
     */
    const XalanDOMString&   m_version;

    /**
     * Text for standalone part of header.
     */
    const XalanDOMString    m_standalone;

    const XalanDOMString  m_encoding;

    static bool
    isUTF16HighSurrogate(XalanDOMChar   theChar)
    {
        return 0xD800u <= theChar && theChar <= 0xDBFFu ? true : false;
    }

    static bool
    isUTF16LowSurrogate(XalanDOMChar    theChar)
    {
        return 0xDC00u <= theChar && theChar <= 0xDFFFu ? true : false;
    }

    static unsigned int
    decodeUTF16SurrogatePair(
            XalanDOMChar    theHighSurrogate,
            XalanDOMChar    theLowSurrogate,
            MemoryManager&  theManager);

    /**
     * Throw an exception when an invalid
     * surrogate is encountered.
     * @param ch The first character in the surrogate
     */
    static void
    throwInvalidUTF16SurrogateException(
            XalanDOMChar    ch,
            MemoryManager&  theManager);

    /**
     * Throw an exception when an invalid
     * surrogate is encountered.
     * @param ch The first character in the surrogate
     * @param next The next character in the surrogate
     */
    static void
    throwInvalidUTF16SurrogateException(
            XalanDOMChar    ch,
            XalanDOMChar    next,
            MemoryManager&  theManager);

    /**
     * Throw an exception when an invalid
     * character is encountered.
     * @param ch The first character in the surrogate
     * @param next The next character in the surrogate
     */
    static void
    throwInvalidCharacterException(
            unsigned int    ch,
            MemoryManager&  theManager);


    /**
     * Throw an exception when an invalid
     * character for the specific XML version is encountered.
     * @param ch The first character in the surrogate
     * @param next The next character in the surrogate
     */
    static void
    throwInvalidXMLCharacterException(
            unsigned int                ch,
            const XalanDOMString&       theXMLversion,
            MemoryManager&              theManager);
private:

    // These are not implemented.
    XalanXMLSerializerBase(const XalanXMLSerializerBase&);

    XalanXMLSerializerBase&
    operator=(const XalanXMLSerializerBase&);

    bool
    operator==(const XalanXMLSerializerBase&) const;

    // Data members...
    /**
     * Flag to tell that we need to add the doctype decl, 
     * which we can't do until the first element is 
     * encountered.
     */
    bool        m_needToOutputDoctypeDecl;

    /**
     * If true, XML header should be written to output.
     */
    bool        m_shouldWriteXMLHeader;

    /**
     * A stack of Boolean objects that tell if the given element 
     * has children.
     */
    BoolStackType   m_elemStack;

    /**
     * The string "-//W3C//DTD XHTML".
     */
    static const XalanDOMChar               s_xhtmlDocTypeString[];

    static const XalanDOMString::size_type  s_xhtmlDocTypeStringLength;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANXMLSERIALIZERBASE_HEADER_GUARD_1357924680
