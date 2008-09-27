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

// Class header file...
#include "OutputContextStack.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>


XALAN_CPP_NAMESPACE_BEGIN



OutputContextStack::OutputContextStack(MemoryManager& theManager) :
    m_stack(theManager, 1 ),
    m_stackPosition(m_stack.begin()),
    m_stackSize(0)
{
    // m_stack is initialized to a size of 1, so that
    // we always have a dummy entry at the beginning
    // of the deque.  This makes the implementation
    // much simpler.
}



OutputContextStack::~OutputContextStack()
{
}



void
OutputContextStack::pushContext(FormatterListener*  theListener)
{
    ++m_stackPosition;
    ++m_stackSize;

    if (m_stackPosition == m_stack.end())
    {
        m_stack.resize(m_stack.size() + 1);

        m_stackPosition = m_stack.end() - 1;
    }

    if (theListener != 0)
    {
        (*m_stackPosition).m_flistener  = theListener;
    }
}



void
OutputContextStack::popContext()
{
    assert(m_stackPosition != m_stack.begin());

    OutputContext&  theCurrentContext = *m_stackPosition;

    theCurrentContext.reset();


    --m_stackPosition;
    --m_stackSize;
}



void
OutputContextStack::clear()
{
    // Since we always keep one dummy entry at the beginning,
    // swap with an OutputContextStackType instance of size 1.
    OutputContextStackType  temp( XalanMemMgrs::getDummyMemMgr(), 1);
    temp.swap(m_stack);

    m_stackPosition = m_stack.begin();

    m_stackSize = 0;
}



void
OutputContextStack::reset()
{
    while(m_stackPosition != m_stack.begin())
    {
        popContext();
    }

    assert(empty() == true);
}



XALAN_CPP_NAMESPACE_END
