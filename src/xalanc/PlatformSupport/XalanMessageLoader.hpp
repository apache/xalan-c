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
#if !defined(XALANMESSAGELOADER_HEADER_GUARD_1357924680)
#define XALANMESSAGELOADER_HEADER_GUARD_1357924680


// Base include file.  Must be first.
#include "xalanc/PlatformSupport/PlatformSupportDefinitions.hpp"


#include "xalanc/XalanDOM/XalanDOMString.hpp"

#include "xalanc/Include/XalanMemMgrAutoPtr.hpp"
#include "xalanc/Include/XalanMemMgrHelper.hpp"

#include "LocalMsgIndex.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XALAN_USING_XERCES(MemoryManager)



// class for calling fom outside of the module 
class  XALAN_PLATFORMSUPPORT_EXPORT XalanMessageLoader {


public:
    template <class Type>
    class XalanMessageLoaderCreateFunct
    {
    public:
        Type*
        operator()(MemoryManager&   theManager)
        {
            XalanMemMgrAutoPtr<Type, false> theGuard( theManager , (Type*)theManager.allocate(sizeof(Type)));

            Type* theResult = theGuard.get();

            new (theResult) Type(theManager);

            theGuard.release();

            return theResult;
        }
    };
    
    class XalanMessageLoaderDestructFunct
    {
    public:
        void
        operator()(
                    MemoryManager&          theManager,
                    XalanMessageLoader*     p)
        {        
            assert ( p != 0);

            destroyObjWithMemMgr(p, theManager);
        }
    };

    virtual
    ~XalanMessageLoader();

    XalanMessageLoader()
    {
    }

    static void
    initialize(MemoryManagerType&      theManager);

    static void
    terminate();

    static XalanDOMString&
    getMessage(
                XalanDOMString&         theResultMessage,
                XalanMessages::Codes    msgToLoad,
                const char*             repText1 , 
                const char*             repText2 = 0, 
                const char*             repText3 = 0, 
                const char*             repText4 = 0);

    static XalanDOMString&
    getMessage(
                XalanDOMString&         theResultMessage,
                XalanMessages::Codes    msgToLoad,
                const XalanDOMChar*     repText1, 
                const XalanDOMChar*     repText2 = 0,
                const XalanDOMChar*     repText3 = 0,
                const XalanDOMChar*     repText4 = 0);

    static XalanDOMString&
    getMessage(
                XalanDOMString&         theResultMessage,
                XalanMessages::Codes    msgToLoad);

    static XalanDOMString&
    getMessage(
                XalanDOMString&         theResultMessage,
                XalanMessages::Codes    msgToLoad,
                const XalanDOMString&   repText1);

    static XalanDOMString&
    getMessage(
                XalanDOMString&         theResultMessage,
                XalanMessages::Codes    msgToLoad,
                const XalanDOMString&   repText1,  
                const XalanDOMString&   repText2);

    static XalanDOMString&
    getMessage(
                XalanDOMString&         theResultMessage,
                XalanMessages::Codes    msgToLoad,
                const XalanDOMString&   repText1,  
                const XalanDOMString&   repText2,  
                const XalanDOMString&   repText3);

protected:

    virtual bool
    loadMsg(
                XalanMessages::Codes    msgToLoad, 
                XalanDOMChar*           toFill, 
                unsigned int            maxChars) = 0;

private:

    bool
    load(
                XalanMessages::Codes    msgToLoad,
                MemoryManager&          theMemoryManager,
                XalanDOMChar*           toFill,
                unsigned int            maxChars,
                const XalanDOMChar*     repText1, 
                const XalanDOMChar*     repText2 = 0, 
                const XalanDOMChar*     repText3 = 0,
                const XalanDOMChar*     repText4 = 0);
    
    bool
    load(
                XalanMessages::Codes        msgToLoad,
                MemoryManagerType&          theManager,
                XalanDOMChar*               toFill, 
                unsigned int                maxChars, 
                const char*                 repText1 , 
                const char*                 repText2 = 0, 
                const char*                 repText3 = 0, 
                const char*                 repText4 = 0);

    XalanMessageLoader(const XalanMessageLoader&);

    XalanMessageLoader&
    operator=(const XalanMessageLoader&);

    static XalanMessageLoader*  s_msgLoader;
};



XALAN_CPP_NAMESPACE_END

#endif  // XALANMESSAGELOADER_HEADER_GUARD_1357924680
