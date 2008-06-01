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
/**
 * 
 * @author David N. Bertoni (david_n_bertoni@lotus.com)
 * */



// Base class header file.
#include "ProblemListenerDefault.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XSLT/ElemTemplateElement.hpp>



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMChar   xsltHeader[] =
{
    XalanUnicode::charLetter_X,
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_L,
    XalanUnicode::charLetter_T,
    XalanUnicode::charSpace,
    0
};

static const XalanDOMChar   xmlHeader[] =
{
    XalanUnicode::charLetter_X,
    XalanUnicode::charLetter_M,
    XalanUnicode::charLetter_L,
    XalanUnicode::charSpace,
    0
};

static const XalanDOMChar   xpathHeader[] =
{
    XalanUnicode::charLetter_X,
    XalanUnicode::charLetter_P,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_h,
    XalanUnicode::charSpace,
    0
};

static const XalanDOMChar   uriHeader[] =
{
    0
};

static const XalanDOMChar   uriSeparator[] =
{
    XalanUnicode::charComma,
    XalanUnicode::charSpace,
    0
};

static const XalanDOMChar   locationOpen[] =
{
    XalanUnicode::charSpace,
    XalanUnicode::charLeftParenthesis,
    0
};

static const XalanDOMChar   locationClose[] =
{
    XalanUnicode::charRightParenthesis,
    0
};


ProblemListenerDefault::ProblemListenerDefault( MemoryManagerType& theManager,
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
            eProblemSource              where,
            eClassification             classification,
            const XalanNode*            sourceNode,
            const ElemTemplateElement*  styleNode,
            const XalanDOMString&       msg,
            const XalanDOMChar*         uri,
            int                         lineNo,
            int                         charOffset)
{
    if (m_pw != 0)
    {
        problem(*m_pw, where, classification, sourceNode, styleNode, msg, uri, lineNo, charOffset);
    }
}



void
ProblemListenerDefault::problem(
            PrintWriter&                pw,
            eProblemSource              where,
            eClassification             classification,
            const XalanNode*            sourceNode,
            const ElemTemplateElement*  styleNode,
            const XalanDOMString&       msg,
            const XalanDOMChar*         uri,
            int                         lineNo,
            int                         charOffset)
{
    MemoryManagerType& theManager = pw.getMemoryManager();

    if (eXMLPARSER == where)
    {
        pw.print(xmlHeader);
    }
    else if (eXPATH == where)
    {
        pw.print(xpathHeader);
    }
    else
    {
        pw.print(xsltHeader);
    }

    XalanDOMString  theBuffer(theManager);

    if (eERROR == classification)
    {
        XalanMessageLoader::getMessage(
            theBuffer,
            XalanMessages::Error_1Param,
            msg);
    }
    else if (eWARNING == classification)
    {
        XalanMessageLoader::getMessage(
            theBuffer,
            XalanMessages::Warning_1Param,
            msg);
    }
    else
    {
        XalanMessageLoader::getMessage(
            theBuffer,
            XalanMessages::Message_1Param,
            msg);
    }

    pw.println(theBuffer);

    if (0 != styleNode)
    {
        pw.print(
            XalanMessageLoader::getMessage(
                theBuffer, 
                XalanMessages::StyleTreeNode_1Param,
                styleNode->getElementName()));
    }

    if (0 != sourceNode)
    {
        pw.print(
            XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::SourceTreeNode_1Param,
                sourceNode->getNodeName()));
    }

    pw.print(locationOpen);

    if (0 != uri)
    {
        pw.print(uri);
        pw.print(uriSeparator);
    }

    XalanDOMString  lineNoString(theManager);
    LongToDOMString(lineNo, lineNoString);

    XalanDOMString  charOffsetString(theManager);
    LongToDOMString(charOffset, charOffsetString);

    XalanMessageLoader::getMessage(
        theBuffer,
        XalanMessages::LineNumberColumnNumber_2Params,
        lineNoString,
        charOffsetString);

    pw.print(theBuffer);

    pw.print(locationClose);

    pw.println();
}



XALAN_CPP_NAMESPACE_END
