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

#include "TopLevelArg.hpp"

#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>

XALAN_CPP_NAMESPACE_BEGIN



TopLevelArg::TopLevelArg(
            MemoryManager&          theManager,
            const XalanQName&       name,
            const XalanDOMString&   expr) :
    m_qname(name, theManager),
    m_expression(expr, theManager),
    m_xobject(0)
{
}



TopLevelArg*
TopLevelArg::create(
            MemoryManager&          theManager,
            const XalanQName&       name,
            const XalanDOMString&   expr)
{
    typedef TopLevelArg ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theManager,
                                name,
                                expr);

    theGuard.release();

    return theResult;
}



TopLevelArg::TopLevelArg(
            MemoryManager&      theManager,
            const XalanQName&   name,
            const XObjectPtr    variable) :
    m_qname(name, theManager),
    m_expression(theManager),
    m_xobject(variable)
{
}



TopLevelArg*
TopLevelArg::create(
            MemoryManager&      theManager,
            const XalanQName&   name,
            const XObjectPtr    variable)
{
    typedef TopLevelArg ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theManager,
                                name,
                                variable);

    theGuard.release();

    return theResult;
}



TopLevelArg::TopLevelArg(
            const TopLevelArg&  theSource,
            MemoryManager&      theManager) :
    m_qname(theSource.m_qname, theManager),
    m_expression(theSource.m_expression, theManager),
    m_xobject(theSource.m_xobject)
{
}



TopLevelArg::~TopLevelArg()
{
}



XALAN_CPP_NAMESPACE_END
