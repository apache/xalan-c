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


#include "XalanDocumentFragmentNodeRefListBaseProxy.hpp"



#include <cassert>



#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XalanDocumentFragmentNodeRefListBaseProxy::XalanDocumentFragmentNodeRefListBaseProxy(const XalanDocumentFragment&   value) :
    NodeRefListBase(),
    m_value(value)
{
}



XalanDocumentFragmentNodeRefListBaseProxy::XalanDocumentFragmentNodeRefListBaseProxy(const XalanDocumentFragmentNodeRefListBaseProxy&   source) :
    NodeRefListBase(),
    m_value(source.m_value)
{
}



XalanDocumentFragmentNodeRefListBaseProxy::~XalanDocumentFragmentNodeRefListBaseProxy()
{
}



XalanNode*
#if defined(NDEBUG)
XalanDocumentFragmentNodeRefListBaseProxy::item(size_type) const
#else
XalanDocumentFragmentNodeRefListBaseProxy::item(size_type   index) const
#endif
{
    assert(index == 0);

    return const_cast<XalanDocumentFragment*>(&m_value);
}



XalanDocumentFragmentNodeRefListBaseProxy::size_type
XalanDocumentFragmentNodeRefListBaseProxy::getLength() const
{
    return 1;
}



XalanDocumentFragmentNodeRefListBaseProxy::size_type
XalanDocumentFragmentNodeRefListBaseProxy::indexOf(const XalanNode*     theNode) const
{
    return theNode == &m_value ? 0 : npos;
}



XALAN_CPP_NAMESPACE_END
