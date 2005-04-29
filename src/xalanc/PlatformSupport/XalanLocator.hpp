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
#if !defined(XALANLOCATOR_HEADER_GUARD_1357924680)
#define XALANLOCATOR_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xercesc/sax/Locator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XALAN_USING_XERCES(Locator)



/**
 * This class defines a base class for Locator derivations in Xalan.  It was defined
 * because Xerces made changes in their Locator class which caused turbulence.
 */
class XALAN_PLATFORMSUPPORT_EXPORT XalanLocator : public Locator
{
public:

    typedef Locator     ParentType;

    typedef XMLSSize_t  size_type;

    XalanLocator() {}

    virtual
    ~XalanLocator() {}

    virtual const XMLCh*
    getPublicId() const = 0;

    virtual const XMLCh*
    getSystemId() const = 0;

    virtual size_type
    getLineNumber() const = 0;

    virtual size_type
    getColumnNumber() const = 0;

    static size_type
    getLineNumber(const ParentType*     theLocator)
    {
        return theLocator == 0 ? size_type(-1) : theLocator->getLineNumber();
    }

    static size_type
    getColumnNumber(const ParentType*   theLocator)
    {
        return theLocator == 0 ? size_type(-1) : theLocator->getColumnNumber();
    }

    static size_type
    getUnknownValue()
    {
        return size_type(-1);
    }

private:

    // Not defined...
    XalanLocator(const XalanLocator&);

    XalanLocator&
    operator=(const XalanLocator&);
};



XALAN_CPP_NAMESPACE_END



#endif  // PREFIXRESOLVER_HEADER_GUARD_1357924680
