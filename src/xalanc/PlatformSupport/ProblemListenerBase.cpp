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
#include "ProblemListenerBase.hpp"



#include "xalanc/XalanDOM/XalanNode.hpp"



#include "PrintWriter.hpp"
#include "XalanLocator.hpp"
#include "XalanMessageLoader.hpp"
#include "XSLException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ProblemListenerBase::ProblemListenerBase()
{
}



ProblemListenerBase::~ProblemListenerBase()
{
}



void
ProblemListenerBase::defaultFormat(
            PrintWriter&            pw,
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode)
{
    MemoryManager&  theManager = pw.getMemoryManager();

    XalanDOMString  theTypeBuffer(theManager);

    XalanMessageLoader::getMessage(
        theTypeBuffer,
        s_messageCodes[source][classification]);

    XalanDOMString  theBuffer(theManager);

    const XalanDOMChar* const uri = XalanLocator::getSystemId(locator);
    const XalanFileLoc lineNo = XalanLocator::getLineNumber(locator);
    const XalanFileLoc charOffset = XalanLocator::getLineNumber(locator);

    XSLException::defaultFormat(
        msg.c_str(),
        msg.length(),
        uri,
        uri == 0 ? 0 : length(uri),
        lineNo,
        charOffset,
        theTypeBuffer.c_str(),
        theTypeBuffer.length(),
        theBuffer);

    pw.println(theBuffer);

    if (0 != sourceNode)
    {
        pw.println(
            XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::SourceTreeNode_1Param,
                sourceNode->getNodeName()));
    }
}



void
ProblemListenerBase::defaultFormat(
            PrintWriter&            pw,
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode)
{
    MemoryManager&  theManager = pw.getMemoryManager();

    XalanDOMString  theBuffer(theManager);

    pw.print(
        XalanMessageLoader::getMessage(
            theBuffer,
            s_messageCodes[source][classification]));

    pw.print(": ");

    pw.println(msg);

    if (0 != sourceNode)
    {
        pw.println(
            XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::SourceTreeNode_1Param,
                sourceNode->getNodeName()));
    }
}



const XalanMessages::Codes  ProblemListenerBase::s_messageCodes[eSourceCount][eClassificationCount] =
{
    {
        XalanMessages::XMLMessage,
        XalanMessages::XMLWarning,
        XalanMessages::XMLError,
    },
    {
        XalanMessages::XSLTMessage,
        XalanMessages::XSLTWarning,
        XalanMessages::XSLTError,
    },
    {
        XalanMessages::XPathMessage,
        XalanMessages::XPathWarning,
        XalanMessages::XPathError,
    }
};




XALAN_CPP_NAMESPACE_END
