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

#if !defined(XALANNODELIST_HEADER_GUARD_1357924680)
#define XALANNODELIST_HEADER_GUARD_1357924680



#include "XalanDOMDefinitions.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;



class XALAN_DOM_EXPORT XalanNodeList
{
public:

    XalanNodeList();

    virtual
    ~XalanNodeList();

    /** @name Get functions. */
    //@{
    /**
     * Returns the <code>index</code>th item in the collection. 
     *
     * If <code>index</code> is greater than or equal to the number of nodes in 
     * the list, this returns <code>null</code>.
     *
     * @param index Index into the collection.
     * @return The node at the <code>index</code>th position in the 
     *   <code>NodeList</code>, or <code>null</code> if that is not a valid 
     *   index.
     */
    virtual XalanNode*
    item(XalanSize_t    index) const = 0;

    /**
     * Returns the number of nodes in the list. 
     *
     * The range of valid child node indices is 0 to <code>length-1</code> inclusive. 
     */
    virtual XalanSize_t
    getLength() const = 0;

protected:

    XalanNodeList(const XalanNodeList&  theSource);

    XalanNodeList&
    operator=(const XalanNodeList&  theSource);

    bool
    operator==(const XalanNodeList&     theRHS) const;

private:
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANNODELIST_HEADER_GUARD_1357924680
