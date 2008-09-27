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
#if !defined(DOMSUPPORTEXCEPTION_HEADER_GUARD_1357924680)
#define DOMSUPPORTEXCEPTION_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/DOMSupport/DOMSupportDefinitions.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/XSLException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanDOMString;



class XALAN_DOMSUPPORT_EXPORT DOMSupportException : public XSLException
{
public:

    /**
     * Constructor for an exception arising from DOMSupport classes
     * 
     * @param theMessage The error message.
     * @param theManager the MemoryManager instance to use.
     * @param theLocator The Locator instance to use, if any.
     */
    DOMSupportException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager,
            const Locator*          theLocator);

    /**
     * Constructor for an exception arising from DOMSupport classes.
     * Use this constructor when the error message is already formatted.
     * 
     * @param theMessage The error message.
     * @param theManager the MemoryManager instance to use.
     */
    DOMSupportException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager);

    virtual
    ~DOMSupportException();

    virtual const XalanDOMChar*
    getType() const;

private:
};



XALAN_CPP_NAMESPACE_END



#endif  // DOMSUPPORTEXCEPTION_HEADER_GUARD_1357924680
