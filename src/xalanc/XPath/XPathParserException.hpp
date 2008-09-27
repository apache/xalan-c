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
#if !defined(XPATHPARSEREXCEPTION_HEADER_GUARD_1357924680)
#define XPATHPARSEREXCEPTION_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XPath/XalanXPathException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * Exception class thrown when a problem parsing an XPath is encountered
 */
class XALAN_XPATH_EXPORT XPathParserException : public XalanXPathException
{
public:

    /**
     * Constructor
     * 
     * @param theMessage message to write when exception thrown
     * @param theManager The MemoryManager instance to use.
     * @param theLocator The locator instance for error reporting, if any.
     */
    XPathParserException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager,
            const Locator*          theLocator);

    /**
     * Constructor. Use this constructor when the error message has
     * already been formatted.
     * 
     * @param theMessage message to write when exception thrown
     * @param theManager The MemoryManager instance to use.
     */
    XPathParserException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager);

    XPathParserException(const XPathParserException&    other);

    virtual
    ~XPathParserException();
    
    virtual const XalanDOMChar*
    getType() const;

private:
};



XALAN_CPP_NAMESPACE_END



#endif  // XPATHPARSEREXCEPTION_HEADER_GUARD_1357924680
