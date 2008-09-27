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
#if !defined(XALAN_DIAGNOSTICMEMORYMANAGER_HEADER_GUARD_1357924680)
#define XALAN_DIAGNOSTICMEMORYMANAGER_HEADER_GUARD_1357924680



#include "xalanc/Harness/XalanHarnessDefinitions.hpp"



#include <cstddef>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iosfwd>
#include <ios>
#endif



#include "xalanc/Include/XalanMap.hpp"
#include "xalanc/Include/XalanMemoryManagement.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_HARNESS_EXPORT XalanDiagnosticMemoryManager : public XalanMemoryManager
{
public:

#if defined(XALAN_NO_STD_NAMESPACE)
    typedef ostream             StreamType;
#else
    typedef std::ostream        StreamType;
#endif

    class LockException
    {
    public:

        LockException()
        {
        }

        ~LockException()
        {
        }
    };

    XalanDiagnosticMemoryManager(
                MemoryManager&  theMemoryManager,
                bool            fAssertErrors = false,
                StreamType*     theStream = 0);

    virtual
    ~XalanDiagnosticMemoryManager();

    virtual void*
    allocate(size_type  size);

    virtual void
    deallocate(void*    pointer);

    virtual MemoryManager*
    getExceptionMemoryManager();

    bool
    getAssertErrors() const
    {
        return m_assertErrors;
    }

    void
    setAssertErrors(bool    fFlag)
    {
        m_assertErrors = fFlag;
    }

    // Get the high-water mark (the highest amount
    // that was allocated at any particular point).
    size_type
    getHighWaterMark() const
    {
        return m_highWaterMark;
    }

    // Get the number of bytes currently allocated.
    size_type
    getAllocated() const
    {
        return m_currentAllocated;
    }

    // Get the current number of outstanding allocations.
    size_type
    getAllocations() const
    {
        return m_allocations.size();
    }

    struct Data
    {
        Data() :
            m_size(0),
            m_sequence(0)
        {
        }

        Data(
                size_type       theSize,
                size_type       theSequence) :
            m_size(theSize),
            m_sequence(theSequence)
        {
        }

        size_type       m_size;

        size_type       m_sequence;
    };

    typedef XalanMap<void*, Data>   MapType;

    typedef MapType::const_iterator     const_iterator;

    const_iterator
    getAllocationsIterator() const
    {
        return m_allocations.begin();
    }

    void
    lock()
    {
        m_locked = true;
    }

    void
    unlock()
    {
        m_locked = false;
    }

    enum
    {
        defaultBytesToDump = 20u
    };

    void
    dumpStatistics(
                StreamType*  theStream = 0,
                size_type   theBytesToDump = defaultBytesToDump);

private:

    XalanDiagnosticMemoryManager(const XalanDiagnosticMemoryManager&);

    XalanDiagnosticMemoryManager&
    operator=(const XalanDiagnosticMemoryManager&);

    MemoryManager&  m_memoryManager;

    bool            m_assertErrors;

    bool            m_locked;

    size_type       m_sequence;

    size_type       m_highWaterMark;

    size_type       m_currentAllocated;

    MapType         m_allocations;

    StreamType*     m_stream;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALAN_DIAGNOSTICMEMORYMANAGER_HEADER_GUARD_1357924680
