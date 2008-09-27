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
#if !defined(XALANNAMEDNODEMAP_HEADER_GUARD_1357924680)
#define XALANNAMEDNODEMAP_HEADER_GUARD_1357924680



#include <xalanc/XalanDOM/XalanDOMDefinitions.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanDOMString;
class XalanNode;



class XALAN_DOM_EXPORT XalanNamedNodeMap
{
public:

    XalanNamedNodeMap();

    virtual
    ~XalanNamedNodeMap();

    /**
     * Returns the <code>index</code>th item in the map. 
     *
     * If <code>index</code> 
     * is greater than or equal to the number of nodes in the map, this returns 
     * <code>null</code>.
     * @param index Index into the map.
     * @return The node at the <code>index</code>th position in the 
     *   <code>NamedNodeMap</code>, or <code>null</code> if that is not a valid 
     *   index.
     */
    virtual XalanNode*
    item(XalanSize_t    index) const = 0;

    /**
     * Retrieves a node specified by name.
     *
     * @param name The <code>nodeName</code> of a node to retrieve.
     * @return A <code>Node</code> (of any type) with the specified <code>nodeName</code>, or 
     *   <code>null</code> if it does not identify any node in 
     *   the map. 
     */
    virtual XalanNode*
    getNamedItem(const XalanDOMString&  name) const = 0;

    /**
     * The number of nodes in the map. 
     *
     * The range of valid child node indices is 
     * 0 to <code>length-1</code> inclusive. 
     */
    virtual XalanSize_t
    getLength() const = 0;

    /**
     * Retrieves a node specified by local name and namespace URI.
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *    the node to retrieve.
     * @param localName The <em>local name</em> of the node to retrieve.
     * @return A <code>Node</code> (of any type) with the specified
     *    local name and namespace URI, or <code>null</code> if they do not
     *    identify any node in the map.
     */
    virtual XalanNode*
    getNamedItemNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName) const = 0;

protected:

    XalanNamedNodeMap(const XalanNamedNodeMap&  theSource);

    XalanNamedNodeMap&
    operator=(const XalanNamedNodeMap&      theSource);

    bool
    operator==(const XalanNamedNodeMap&     theRHS) const;

private:
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XALANNAMEDNODEMAP_HEADER_GUARD_1357924680)
