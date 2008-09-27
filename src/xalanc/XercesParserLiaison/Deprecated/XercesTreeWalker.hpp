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
#if !defined(XERCESTREEWALKER_HEADER_GUARD_1357924680)
#define XERCESTREEWALKER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN

/**
 * This class is deprecated.
 *
 * @deprecated This class works with the deprecated Xerces DOM bridge.
 */
class XALAN_XERCESPARSERLIAISON_EXPORT XercesTreeWalker
{
public:

    /**
     * Constructor.
     */
    XercesTreeWalker();

    virtual
    ~XercesTreeWalker();

    /**
     * Perform a pre-order traversal non-recursive style.
     */
    virtual void
    traverse(const DOM_NodeType&    pos);

    /**
     * Perform a pre-order traversal non-recursive style.
     */
    virtual void
    traverse(
            const DOM_NodeType&     pos,
            const DOM_NodeType&     parent);

protected:

    virtual void
    startNode(const DOM_NodeType&   node) = 0;

    virtual void
    endNode(const DOM_NodeType&     node) = 0;
};



XALAN_CPP_NAMESPACE_END



#endif  // XERCESTREEWALKER_HEADER_GUARD_1357924680
