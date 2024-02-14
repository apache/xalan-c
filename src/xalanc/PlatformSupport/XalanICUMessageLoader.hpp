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
#if !defined(XALANICUMESSAGELOADER_HEADER_GUARD_1357924680)
#define XALANICUMESSAGELOADER_HEADER_GUARD_1357924680

#if defined (XALAN_ICU_MSG_LOADER)

// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>

#include <xalanc/XalanDOM/XalanDOMString.hpp>

#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>

#include <xercesc/util/XMLUni.hpp>


#include <unicode/ures.h>


namespace XALAN_CPP_NAMESPACE {


class XALAN_PLATFORMSUPPORT_EXPORT XalanICUMessageLoader : public XalanMessageLoader 

{
public:

    XalanICUMessageLoader(MemoryManager& theManager);

    virtual
    ~XalanICUMessageLoader();

protected:

    // -----------------------------------------------------------------------
    //  Implementation of the virtual message loader API
    // -----------------------------------------------------------------------
    virtual bool
    loadMsg(
        XalanMessages::Codes    msgToLoad,
        XalanDOMChar*           toFill,
        XalanSize_t             maxChars
    );

private:

    XalanICUMessageLoader(const XalanICUMessageLoader&);

    XalanICUMessageLoader&
    operator=(const XalanICUMessageLoader&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fLocaleBundle
    //      pointer to the required locale specific resource bundle,
    //           or to the default locale resrouce bundle in case the required
    //              locale specific resource bundle unavailable.
    //
     // -----------------------------------------------------------------------
    UResourceBundle*        m_localeBundle;
    UResourceBundle*        m_domainBundle;

    const XalanDOMString    m_unknownMessage;
};




}

#endif //  defined (XALAN_ICU_MSG_LOADER)

#endif  // XALANICUMESSAGELOADER_HEADER_GUARD_1357924680
