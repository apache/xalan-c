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
#if !defined(XALANXPATHEXCEPTION_HEADER_GUARD_1357924680)
#define XALANXPATHEXCEPTION_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/XSLException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;



class XALAN_XPATH_EXPORT XalanXPathException : public XSLException
{
public:

    /**
     * Construct an XPath exeption object.
     * 
     * @param theMessage The message explaining the problem.
     * @param theManager The MemoryManager instance to use.
     */
    XalanXPathException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager);

    /**
     * Construct an XPath exeption object.
     * 
     * @param theMessage The message explaining the problem.
     * @param theManager The MemoryManager instance to use.
     * @param theLocator The locator instance for error reporting, if any.
     */
    XalanXPathException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager,
            const Locator*          theLocator);

    XalanXPathException(const XalanXPathException&  other);

    virtual
    ~XalanXPathException();

    virtual const XalanDOMChar*
    getType() const;

private:

    /**
     * Construct an XPath exeption object.
     * 
     */
    explicit
    XalanXPathException(MemoryManager&  theManager);

private:
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANXPATHEXCEPTION_HEADER_GUARD_1357924680
