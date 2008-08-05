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



// Base class header file.
#include "ProblemListenerDefault.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XSLException.hpp>



#include <xalanc/XSLT/ElemTemplateElement.hpp>



XALAN_CPP_NAMESPACE_BEGIN



static const XalanMessages::Codes s_messageCodes[ProblemListener::eSourceCount][ProblemListener::eClassificationCount] =
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



ProblemListenerDefault::ProblemListenerDefault(
            MemoryManagerType&  theManager,
            PrintWriter*        pw) :
    ProblemListener(),
    m_memoryManager(theManager),
    m_pw(pw)
{
}



ProblemListenerDefault::~ProblemListenerDefault()
{
}



void
ProblemListenerDefault::setPrintWriter(PrintWriter*     pw)
{
    m_pw = pw;
}



void
ProblemListenerDefault::problem(
            eProblemSource              source,
            eClassification             classification,
            const XalanNode*            sourceNode,
            const ElemTemplateElement*  styleNode,
            const XalanDOMString&       msg,
            const XalanDOMChar*         uri,
			XalanFileLoc				lineNo,
			XalanFileLoc				charOffset)
{
    if (m_pw != 0)
    {
        problem(
            *m_pw,
            source,
            classification,
            sourceNode,
            styleNode,
            msg,
            uri,
            lineNo,
            charOffset);
    }
}



void
ProblemListenerDefault::problem(
            PrintWriter&                pw,
            eProblemSource              source,
            eClassification             classification,
            const XalanNode*            sourceNode,
            const ElemTemplateElement*  styleNode,
            const XalanDOMString&       msg,
            const XalanDOMChar*         uri,
			XalanFileLoc				lineNo,
			XalanFileLoc				charOffset)
{
    MemoryManager&  theManager = pw.getMemoryManager();

    XalanDOMString  theTypeBuffer(theManager);

    XalanMessageLoader::getMessage(
        theTypeBuffer,
        s_messageCodes[source][classification]);

    XalanDOMString  theBuffer(theManager);

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

    if (0 != styleNode)
    {
        pw.println(
            XalanMessageLoader::getMessage(
                theBuffer, 
                XalanMessages::StyleTreeNode_1Param,
                styleNode->getElementName()));
    }

    if (0 != sourceNode)
    {
        pw.println(
            XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::SourceTreeNode_1Param,
                sourceNode->getNodeName()));
    }
}



XALAN_CPP_NAMESPACE_END
