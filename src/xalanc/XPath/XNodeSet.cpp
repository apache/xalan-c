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
// Class header file.
#include "XNodeSet.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "MutableNodeRefList.hpp"
#include "XObjectTypeCallback.hpp"
#include "XPathExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN




XNodeSet::XNodeSet(
            BorrowReturnMutableNodeRefList&     value,
            MemoryManager&                      theMemoryManager) :
    XNodeSetBase(theMemoryManager),
    m_value(value)
{
}



XNodeSet::XNodeSet(const XNodeSet&  source,
                   MemoryManager&   theMemoryManager) :
    XNodeSetBase(source, theMemoryManager),
    m_value(source.m_value.clone())
{
}



XNodeSet::~XNodeSet()
{
}




const NodeRefListBase&
XNodeSet::nodeset() const
{
    return *m_value.get();
}



void
XNodeSet::release()
{
    m_value.release();

    clearCachedValues();
}



void
XNodeSet::set(BorrowReturnMutableNodeRefList&   value)
{
    release();

    m_value = value;
}



XalanNode*
XNodeSet::item(size_type    index) const
{
    return m_value->item(index);
}



XNodeSet::size_type
XNodeSet::getLength() const
{
    return m_value->getLength();
}



XALAN_CPP_NAMESPACE_END
