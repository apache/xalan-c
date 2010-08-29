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
#if !defined(EXECUTIONCONTEXT_HEADER_GUARD_1357924680)
#define EXECUTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/Include/XalanMemoryManagement.hpp>



#include "xalanc/PlatformSupport/ProblemListenerBase.hpp"



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator  LocatorType;



class XalanDOMString;
class XalanNode;
class XalanText;



//
// An abstract class which provides support for execution.
//
class XALAN_PLATFORMSUPPORT_EXPORT ExecutionContext : public ProblemListenerBase
{
public:

    ExecutionContext(MemoryManager&     theMemoryManager);

    virtual
    ~ExecutionContext();


    // These interfaces are inherited from ProblemListenerBase
    virtual void
    problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode) = 0;

    virtual void
    problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode) = 0;

    bool
    hasPreserveOrStripSpaceConditions() const
    {
        return m_hasPreserveOrStripConditions;
    }

    /**
     * Determine if a text node should be stripped from the source tree,
     * as if it weren't there.
     *
     * @param textNode text node from the source tree
     * @return true if the text node should be stripped
     */
    virtual bool
    shouldStripSourceNode(const XalanText&  node) = 0;

    MemoryManager&
    getMemoryManager() const
    {
        return m_memoryManager;
    }

    MemoryManager&
    getExceptionMemoryManager() const
    {
        return XalanMemoryManager::getExceptionMemoryManager(m_memoryManager);
    }

protected:

    MemoryManager&  m_memoryManager;

    bool                    m_hasPreserveOrStripConditions;
};



XALAN_CPP_NAMESPACE_END



#endif  // EXECUTIONCONTEXT_HEADER_GUARD_1357924680
