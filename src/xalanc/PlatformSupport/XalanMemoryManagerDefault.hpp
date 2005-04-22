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
#if !defined(XALANMEMORYMANAGERDEFAULT_HEADER_GUARD_1357924680)
#define XALANMEMORYMANAGERDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>




#include <xercesc/framework/MemoryManager.hpp>




XALAN_CPP_NAMESPACE_BEGIN



XALAN_USING_XERCES(MemoryManager)



class XALAN_PLATFORMSUPPORT_EXPORT XalanMemoryManagerDefault : public MemoryManager
{
public:

    XalanMemoryManagerDefault();

    virtual
    ~XalanMemoryManagerDefault();

    virtual void*
    allocate(size_t     size);

    virtual void
    deallocate(void*    pointer);

private:

    // These are not implemented.
    XalanMemoryManagerDefault(const XalanMemoryManagerDefault&);

    XalanMemoryManagerDefault&
    operator=(const XalanMemoryManagerDefault&);
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANMEMORYMANAGERDEFAULT_HEADER_GUARD_1357924680
