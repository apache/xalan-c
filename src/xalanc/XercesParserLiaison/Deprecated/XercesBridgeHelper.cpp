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


#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)



#include "XercesBridgeHelper.hpp"



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_CharacterData.hpp>
#else
#include <xercesc/dom/DOM_CharacterData.hpp>
#endif



#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



void
XercesBridgeHelper::setNodeValue(
            DOM_NodeType&           theXercesNode,
            const XalanDOMString&   nodeValue)
{
    try
    {
        theXercesNode.setNodeValue(XalanDOMStringToXercesDOMString(nodeValue));
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }
}



void
XercesBridgeHelper::normalize(DOM_NodeType&     theXercesNode)
{
    try
    {
        theXercesNode.normalize();
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }
}



bool
XercesBridgeHelper::isSupported(
            const DOM_NodeType&     theXercesNode,
            const XalanDOMString&   feature,
            const XalanDOMString&   version)
{
    return theXercesNode.isSupported(
                XalanDOMStringToXercesDOMString(feature),
                XalanDOMStringToXercesDOMString(version));
}



void
XercesBridgeHelper::setPrefix(
            DOM_NodeType&           theXercesNode,
            const XalanDOMString&   prefix)
{
    try
    {
        theXercesNode.setPrefix(XalanDOMStringToXercesDOMString(prefix));
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }
}



const XalanDOMString
XercesBridgeHelper::substringData(
            const DOM_CharacterDataType&    theXercesNode,
            unsigned int                    offset,
            unsigned int                    count)
{
    try
    {
        const DOMStringType     theString(theXercesNode.substringData(offset, count));

        return XalanDOMString(theString.rawBuffer(), theString.length());
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }
}



void
XercesBridgeHelper::appendData(
            DOM_CharacterDataType&  theXercesNode,
            const XalanDOMString&   arg)
{
    try
    {
        theXercesNode.appendData(XalanDOMStringToXercesDOMString(arg));
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }
}



void
XercesBridgeHelper::insertData(
            DOM_CharacterDataType&  theXercesNode,
            unsigned int            offset,
            const  XalanDOMString&  arg)
{
    try
    {
        theXercesNode.insertData(offset, XalanDOMStringToXercesDOMString(arg));
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }
}



void
XercesBridgeHelper::deleteData(
            DOM_CharacterDataType&  theXercesNode,
            unsigned int            offset, 
            unsigned int            count)
{
    try
    {
        theXercesNode.deleteData(offset, count);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }
}



void
XercesBridgeHelper::replaceData(
            DOM_CharacterDataType&  theXercesNode,
            unsigned int            offset, 
            unsigned int            count, 
            const XalanDOMString&   arg)
{
    try
    {
        theXercesNode.replaceData(offset, count, XalanDOMStringToXercesDOMString(arg));
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


