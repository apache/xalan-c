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

#include "xalanc/Include/XalanMemoryManagement.hpp"



#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/OutOfMemoryException.hpp"



#include <cassert>
#include <new>



namespace XALAN_CPP_NAMESPACE {



 
XalanMemoryManager::XalanMemoryManager() :
    MemoryManager()
{
}



XalanMemoryManager::~XalanMemoryManager()
{
}



XalanMemoryManager::XalanMemoryManager(const XalanMemoryManager&    /* theSource */) :
    MemoryManager()
{
}



class XalanDummyMemoryManager : public XalanMemoryManager
{
public:

    XalanDummyMemoryManager()
    {
    }

    virtual
    ~XalanDummyMemoryManager()
    {
    }

    virtual void*
    allocate(size_type /*   size */ )
    {
        using xercesc::OutOfMemoryException;

        throw OutOfMemoryException();
    }   

    virtual void
    deallocate(void*    /* pDataPointer */ )
    {
        using xercesc::OutOfMemoryException;

        throw OutOfMemoryException();
    }

    MemoryManager*
    getExceptionMemoryManager()
    {
        return this;
    }
};



static XalanDummyMemoryManager  s_dummyMemMgr;



MemoryManager&
XalanMemMgrs::getDummyMemMgr()
{
    return s_dummyMemMgr;
}



MemoryManager&
XalanMemMgrs::getDefaultXercesMemMgr()
{
    using xercesc::XMLPlatformUtils;

    assert(XMLPlatformUtils::fgMemoryManager != 0);

    return *XMLPlatformUtils::fgMemoryManager;
}



}
