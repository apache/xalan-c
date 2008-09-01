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

    XalanLocator() {}

    virtual
    ~XalanLocator() {}

    virtual const XMLCh*
    getPublicId() const = 0;

    virtual const XMLCh*
    getSystemId() const = 0;

    virtual XalanFileLoc
    getLineNumber() const = 0;

    virtual XalanFileLoc
    getColumnNumber() const = 0;

    static const XalanDOMChar*
    getPublicId(
            const Locator*          theLocator,
            const XalanDOMChar*     theAlternateId = 0)
    {
        return theLocator == 0 ? theAlternateId : theLocator->getPublicId();
    }

    static const XalanDOMChar*
    getSystemId(
            const Locator*          theLocator,
            const XalanDOMChar*     theAlternateId = 0)
    {
        return theLocator == 0 ? theAlternateId : theLocator->getSystemId();
    }

    static XalanFileLoc
    getLineNumber(const ParentType*     theLocator)
    {
        return theLocator == 0 ? getUnknownValue() : theLocator->getLineNumber();
    }

    static XalanFileLoc
    getColumnNumber(const ParentType*   theLocator)
    {
        return theLocator == 0 ? getUnknownValue() : theLocator->getColumnNumber();
    }

    static XalanFileLoc
    getUnknownValue()
    {
        // The parser reports the maximum value of the XalanFileLoc
        // type for an unknown value.
        return ~static_cast<XalanFileLoc>(0);
    }

    static XalanFileLoc
    getUnknownDisplayValue()
    {
        // The parser reports the maximum value of the XalanFileLoc
        // type for an unknown value, but that is really non-sensical
        // for display purposes, so we use 0 instead.
        return static_cast<XalanFileLoc>(0);
    }

    static bool
    isUnknownValue(XalanFileLoc     theLocation)
    {
        return theLocation == getUnknownValue();
    }

private:

    // Not defined...
    XalanLocator(const XalanLocator&);

    XalanLocator&
    operator=(const XalanLocator&);
};



XALAN_CPP_NAMESPACE_END



#endif  // PREFIXRESOLVER_HEADER_GUARD_1357924680
