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
#if !defined(MEMORYMANAGERIMPL_HEADER_GUARD_1357924680)
#define MEMORYMANAGERIMPL_HEADER_GUARD_1357924680


// Base include file.  Must be first.
#include <new>


#include <xalanc/Include/PlatformDefinitions.hpp>



#include "xercesc/util/OutOfMemoryException.hpp"



#include "xalanc/Include/XalanMemoryManagement.hpp"



#if defined(XALAN_WINDOWS)
#include <windows.h>
#include <stdexcept>
#include <stdlib.h>


XALAN_USING_XERCES(MemoryManager)

class XalanMemoryManagerImpl : public XALAN_CPP_NAMESPACE_QUALIFIER XalanMemoryManager
{
public:

    XalanMemoryManagerImpl(DWORD    defSize = 0) :
        m_heapHandle(NULL)
    {
        m_heapHandle =
            HeapCreate(
                HEAP_NO_SERIALIZE,
                0, // dwInitialSize
                defSize);

        if (m_heapHandle == NULL)
        {
            XALAN_USING_STD(runtime_error)

            char buffer[20];
            buffer[0] = 0;

            _ultoa( GetLastError(), buffer, 10);

            throw runtime_error(buffer);
        }
    }

    virtual void*
    allocate(size_type  size)   
    {
        LPVOID ptr = HeapAlloc(
                        m_heapHandle,   //HANDLE hHeap
                        0,              //DWORD dwFlags
                        size            //SIZE_T dwBytes
                        );

        if (ptr == 0)
        {
            XALAN_USING_XERCES(OutOfMemoryException)

            throw OutOfMemoryException();
        }

        return ptr;
    }

    virtual void
    deallocate(void*    pDataPointer)
    {
        if (0 == HeapFree(
                        m_heapHandle,   //HANDLE hHeap,
                        0,              //DWORD dwFlags,
                        pDataPointer ) )//*LPVOID lpMem 
        {
            XALAN_USING_XERCES(OutOfMemoryException)

            throw OutOfMemoryException();
        }
    }

    virtual MemoryManager*
    getExceptionMemoryManager()
    {
        return this;
    }

    virtual 
    ~XalanMemoryManagerImpl()
    {
        if (0 == HeapDestroy(m_heapHandle))
        {
            XALAN_USING_STD(runtime_error)

            char buffer[20];
            buffer[0] = 0;

            _ultoa( GetLastError(), buffer, 10);

            throw runtime_error(buffer);
       }
    }

private:

    // Not implemented
    XalanMemoryManagerImpl&
    operator=(const XalanMemoryManagerImpl&);

    XalanMemoryManagerImpl(const XalanMemoryManagerImpl&);

    //Data
    HANDLE  m_heapHandle;   
};

#else

class XalanMemoryManagerImpl : public XALAN_CPP_NAMESPACE_QUALIFIER XalanMemoryManager
{
public:

    typedef XALAN_CPP_NAMESPACE_QUALIFIER XalanSize_t   XalanSize_t;

    virtual
    ~XalanMemoryManagerImpl()
    {
    }

    virtual void*
    allocate(size_type  size)
    {
        try
        {
            void* memptr = ::operator new(size);

            if (memptr != 0) 
            {
                return memptr;
            }
        }
        catch (...)
        {
        }

        XALAN_USING_XERCES(OutOfMemoryException)

        throw OutOfMemoryException();
    }   

    virtual void
    deallocate(void*    pDataPointer)
    {
        operator delete(pDataPointer);
    }

    virtual MemoryManager*
    getExceptionMemoryManager()
    {
        return this;
    }
};



#endif



#endif  // MEMORYMANAGERIMPL_HEADER_GUARD_1357924680
