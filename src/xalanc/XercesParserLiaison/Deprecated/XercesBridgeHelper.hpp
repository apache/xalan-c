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
#if !defined(XERCESBRIDGEHELPER_HEADER_GUARD_1357924680)
#define XERCESBRIDGEHELPER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <cassert>



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_Node.hpp>
#else
#include <xercesc/dom/DOM_Node.hpp>
#endif



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeNavigator.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeTypes.hpp>



XALAN_DECLARE_XERCES_CLASS(DOM_CharacterData)



XALAN_CPP_NAMESPACE_BEGIN


/**
 * This class is deprecated.
 *
 * @deprecated This class is part of the deprecated Xerces DOM bridge.
 */
class XALAN_XERCESPARSERLIAISON_EXPORT XercesBridgeHelper
{
public:

    typedef unsigned int    XercesStringLengthType;

    static const DOMStringType
    XalanDOMStringToXercesDOMString(const XalanDOMString&   theString)
    {
        assert(XercesStringLengthType(theString.length()) == theString.length());

        return DOMStringType(&theString[0], XercesStringLengthType(theString.length()));
    }

    static void
    setNodeValue(
            DOM_NodeType&           theXercesNode,
            const XalanDOMString&   nodeValue);

    static void
    normalize(DOM_NodeType&     theXercesNode);

    static bool
    isSupported(
            const DOM_NodeType&     theXercesNode,
            const XalanDOMString&   feature,
            const XalanDOMString&   version);

    static void
    setPrefix(
            DOM_NodeType&           theXercesNode,
            const XalanDOMString&   prefix);

    static const XalanDOMString
    substringData(
            const DOM_CharacterDataType&    theXercesNode,
            unsigned int                    offset, 
            unsigned int                    count);

    static void
    appendData(
            DOM_CharacterDataType&  theXercesNode,
            const XalanDOMString&   arg);

    static void
    insertData(
            DOM_CharacterDataType&  theXercesNode,
            unsigned int            offset,
            const  XalanDOMString&  arg);

    static void
    deleteData(
            DOM_CharacterDataType&  theXercesNode,
            unsigned int            offset, 
            unsigned int            count);

    static void
    replaceData(
            DOM_CharacterDataType&  theXercesNode,
            unsigned int            offset, 
            unsigned int            count, 
            const XalanDOMString&   arg);
private:

    // Not implemented...
    XercesBridgeHelper();

    XercesBridgeHelper(const XercesBridgeHelper&);
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESBRIDGEHELPER_HEADER_GUARD_1357924680)
