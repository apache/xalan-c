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
#if !defined(XSLEXCEPTION_HEADER_GUARD_1357924680)
#define XSLEXCEPTION_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanLocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator  LocatorType;
XALAN_USING_XERCES(Locator)



class XALAN_PLATFORMSUPPORT_EXPORT XSLException
{
public:

    /**
     * Constructor
     *
     * @param theMessage The message to display when exception thrown
     * @param theManager The MemoryManager instance to use.
     * @param theLocator The locator instance for error reporting, if any.
     */
    XSLException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager,
            const Locator*          theLocator);

    /**
     * Constructor.  Use this constructor when the message has 
     * already been formatted.
     * 
     * @param theMessage The message to display when exception thrown
     * @param theManager The MemoryManager instance to use.
     */
    XSLException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager);

    XSLException(const XSLException&    other);

    virtual
    ~XSLException();

    /**
     * Retrieve type of exception
     * 
     * @return type of exception
     */
    virtual const XalanDOMChar*
    getType() const = 0;

    /**
     * Retrieve message for exception
     * 
     * @return exception message
     */
    const XalanDOMString&
    getMessage() const
    {
        return m_message;
    }

    /**
     * Get the URI for the associated document, if any
     * 
     * @return The URI.
     */
    const XalanDOMString&
    getURI() const
    {
        return m_uri;
    }

    /**
     * Retrieve the line number.
     * 
     * @return the line number
     */
    XalanFileLoc
    getLineNumber() const
    {
        return m_lineNumber;
    }

    /**
     * Retrieve the column number.
     * 
     * @return the column number
     */
    XalanFileLoc
    getColumnNumber() const
    {
        return m_columnNumber;
    }

    const MemoryManager&
    getMemoryManager() const
    {
        return m_memoryManager;
    }

    MemoryManager&
    getMemoryManager()
    {
        return m_memoryManager;
    }

    XalanDOMString&
    defaultFormat(XalanDOMString&   theBuffer) const;

    static XalanDOMString&
    defaultFormat(
            const XalanDOMString&   theMessage,
            const XalanDOMString&   theURI,
            XalanFileLoc            theLineNumber,
            XalanFileLoc            theColumnNumber,
            const XalanDOMChar*     theType,
            XalanDOMString&         theBuffer)
    {
        return defaultFormat(
                    theMessage.c_str(),
                    theMessage.size(),
                    theURI.c_str(),
                    theURI.size(),
                    theLineNumber,
                    theColumnNumber,
                    theType,
                    XalanDOMString::length(theType),
                    theBuffer);
    }

    static XalanDOMString&
    defaultFormat(
            const XalanDOMChar*     theMessage,
            const XalanDOMChar*     theURI,
            XalanFileLoc            theLineNumber,
            XalanFileLoc            theColumnNumber,
            const XalanDOMChar*     theType,
            XalanDOMString&         theBuffer)
    {
        assert(theMessage != 0);

        return defaultFormat(
                    theMessage,
                    XalanDOMString::length(theMessage),
                    theURI,
                    theURI == 0 ? 0 : XalanDOMString::length(theURI),
                    theLineNumber,
                    theColumnNumber,
                    theType,
                    theType == 0 ? 0 : XalanDOMString::length(theType),
                    theBuffer);
    }

    static XalanDOMString&
    defaultFormat(
            const XalanDOMChar*                 theMessage,
            const XalanDOMString::size_type     theMessageLength,
            const XalanDOMChar*                 theURI,
            const XalanDOMString::size_type     theURILength,
            XalanFileLoc                        theLineNumber,
            XalanFileLoc                        theColumnNumber,
            const XalanDOMChar*                 theType,
            const XalanDOMString::size_type     theTypeLength,
            XalanDOMString&                     theBuffer);

protected:

    MemoryManager&  m_memoryManager;
    
private:

    const XalanDOMString    m_message;
    const XalanDOMString    m_uri;

    const XalanFileLoc      m_lineNumber;
    const XalanFileLoc      m_columnNumber;

    // When true, the message has already formatted.
    const bool              m_formatted;
};



XALAN_CPP_NAMESPACE_END



#endif  // XSLEXCEPTION_HEADER_GUARD_1357924680
