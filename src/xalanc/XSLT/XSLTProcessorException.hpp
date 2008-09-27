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
#if !defined(XSLTPROCESSOREXCEPTION_HEADER_GUARD_1357924680)
#define XSLTPROCESSOREXCEPTION_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



// Base class header file...
#include <xalanc/PlatformSupport/XSLException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XSLT_EXPORT XSLTProcessorException : public XSLException
{
public:

    /**
     * Construct an XSLT Processor exception object
     * 
     * @param theManager The MemoryManager instance to use.
     * @param theMessage The message to print when exception thrown
     * @param theLocator The Locator instance for error reporting, if any.
     */
    XSLTProcessorException(
            MemoryManager&          theManager,
            const XalanDOMString&   theMessage,
            const Locator*          theLocator);

    /**
     * Construct an XSLT Processor exception object.  Use this
     * constructor when the error message is already formatted.
     * 
     * @param theManager The MemoryManager instance to use.
     * @param theMessage The message to print when exception thrown
     */
    XSLTProcessorException(
            MemoryManager&          theManager,
            const XalanDOMString&   theMessage);

    virtual
    ~XSLTProcessorException();

    virtual const XalanDOMChar*
    getType() const
    {
        return m_type;
    }

private:

    static const XalanDOMChar   m_type[];
};



XALAN_CPP_NAMESPACE_END



#endif  // XSLTPROCESSOREXCEPTION_HEADER_GUARD_1357924680
