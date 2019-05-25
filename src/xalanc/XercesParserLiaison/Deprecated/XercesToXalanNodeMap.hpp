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
#if !defined(XERCESTOXALANNODEMAP_HEADER_GUARD_1357924680)
#define XERCESTOXALANNODEMAP_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <map>



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_Node.hpp>
#else
#include <xercesc/dom/DOM_Node.hpp>
#endif



#include <xalanc/XercesParserLiaison/Deprecated/XercesDOM_NodeHack.hpp>



namespace XERCES_CPP_NAMESPACE
{
    class NodeImpl;
}



namespace XALAN_CPP_NAMESPACE {



class XalanNode;


/**
 * This class is deprecated.
 *
 * @deprecated This class works with the deprecated Xerces DOM bridge.
 */
class XALAN_XERCESPARSERLIAISON_EXPORT XercesToXalanNodeMap
{
public:

    typedef xercesc::DOM_Node     DOM_NodeType;
    typedef xercesc::NodeImpl     NodeImplType;

    typedef std::map<XalanNode*, NodeImplType*>     XalanNodeMapType;

    typedef std::map<NodeImplType*, XalanNode*>     XercesNodeMapType;

    XercesToXalanNodeMap();

    ~XercesToXalanNodeMap();

    void
    addAssociation(
            const DOM_NodeType&     theXercesNode,
            XalanNode*              theXalanNode);

    void
    clear();

    XalanNode*
    getNode(const DOM_NodeType&     theXercesNode) const
    {
        return getNode(XercesDOM_NodeHack::getImpl(theXercesNode));
    }

    XalanNode*
    getNode(NodeImplType*   theXercesNodeImpl) const
    {
        const XercesNodeMapType::const_iterator     i =
                m_xercesMap.find(theXercesNodeImpl);

        if (i == m_xercesMap.end())
        {
            return 0;
        }
        else
        {
            return (*i).second;
        }
    }

    DOM_NodeType
    getNode(const XalanNode*    theXalanNode) const
    {
        return XercesDOM_NodeHack(getNodeImpl(theXalanNode));
    }

    NodeImplType*
    getNodeImpl(const XalanNode*    theXalanNode) const;

    class NameMapEqualsFunctor
    {
    public:

        NameMapEqualsFunctor(const XalanNode*   theXalanNode) :
            m_value(theXalanNode)
        {
        }

        bool
        operator()(const XercesNodeMapType::value_type&     thePair) const
        {
            return m_value == thePair.second;
        }

    private:

        const XalanNode*    m_value;
    };

    NodeImplType*
    getNodeImpl(const DOM_NodeType&     theXercesNode) const
    {
        return XercesDOM_NodeHack::getImpl(theXercesNode);
    }

private:

    XalanNodeMapType    m_xalanMap;

    XercesNodeMapType   m_xercesMap;
};



}



#endif  // !defined(XERCESTOXALANNODEMAP_HEADER_GUARD_1357924680)
