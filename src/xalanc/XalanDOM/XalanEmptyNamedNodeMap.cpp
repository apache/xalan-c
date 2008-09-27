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
#include "XalanEmptyNamedNodeMap.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XalanEmptyNamedNodeMap::XalanEmptyNamedNodeMap() :
    XalanNamedNodeMap()
{
}



XalanEmptyNamedNodeMap::~XalanEmptyNamedNodeMap()
{
}



XalanEmptyNamedNodeMap::XalanEmptyNamedNodeMap(const XalanEmptyNamedNodeMap&    theSource) :
    XalanNamedNodeMap(theSource)
{
}



XalanEmptyNamedNodeMap&
XalanEmptyNamedNodeMap::operator=(const XalanEmptyNamedNodeMap&     theSource)
{
    XalanNamedNodeMap::operator=(theSource);

    return *this;
}



bool
XalanEmptyNamedNodeMap::operator==(const XalanEmptyNamedNodeMap&    /* theRHS */) const
{
    return true;
}



XalanNode*
XalanEmptyNamedNodeMap::item(XalanSize_t    /* index */) const
{
    return 0;
}



XalanNode*
XalanEmptyNamedNodeMap::getNamedItem(const XalanDOMString&  /* name */) const
{
    return 0;
}


XalanSize_t
XalanEmptyNamedNodeMap::getLength() const
{
    return 0;
}



XalanNode*
XalanEmptyNamedNodeMap::getNamedItemNS(
            const XalanDOMString&   /* namespaceURI */,
            const XalanDOMString&   /* localName */) const
{
    return 0;
}



XALAN_CPP_NAMESPACE_END
