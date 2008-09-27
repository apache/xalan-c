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
#if !defined(XERCESNODELISTBRIDGE_HEADER_GUARD_1357924680)
#define XERCESNODELISTBRIDGE_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_NodeList.hpp>
#else
#include <xercesc/dom/DOM_NodeList.hpp>
#endif



#include <xalanc/XalanDOM/XalanNodeList.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XercesBridgeNavigator;


/**
 * This class is deprecated.
 *
 * @deprecated This class is part of the deprecated Xerces DOM bridge.
 */
class XALAN_XERCESPARSERLIAISON_EXPORT XercesNodeListBridge : public XalanNodeList
{
public:

    XercesNodeListBridge(
            const DOM_NodeListType&         theXercesNodeList,
            const XercesBridgeNavigator&    theNavigator);

    XercesNodeListBridge(const XercesNodeListBridge&    theSource);

    virtual
    ~XercesNodeListBridge();

    bool
    operator==(const XercesNodeListBridge&  theRHS) const
    {
        return m_xercesNodeList == theRHS.m_xercesNodeList ? true : false;
    }

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
    item(unsigned int   index) const;

    /**
     * Returns the number of nodes in the list. 
     *
     * The range of valid child node indices is 0 to <code>length-1</code> inclusive. 
     */
    virtual unsigned int
    getLength() const;

private:

    // Not implemented...
    XercesNodeListBridge&
    operator=(const XercesNodeListBridge&   theRHS);


    // Data members...
    DOM_NodeListType                m_xercesNodeList;

    const XercesBridgeNavigator&    m_navigator;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESNODELISTBRIDGE_HEADER_GUARD_1357924680)
