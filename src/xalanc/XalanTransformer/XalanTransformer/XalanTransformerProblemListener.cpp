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
#include "XalanTransformerProblemListener.hpp"



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <ostream>
#endif



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DOMStringPrintWriter.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XalanTransformerProblemListener::XalanTransformerProblemListener(
            MemoryManager&  theManager,
            StreamType*     theWarningStream,
            PrintWriter*    thePrintWriter) :
    ProblemListener(),
    m_problemListener(theManager, thePrintWriter),
    m_warningStream(theWarningStream),
    m_warningString(theManager)
{
}



XalanTransformerProblemListener::~XalanTransformerProblemListener()
{
}



void
XalanTransformerProblemListener::setPrintWriter(PrintWriter*    pw)
{
    m_problemListener.setPrintWriter(pw);
}



void
XalanTransformerProblemListener::problem(
            eSource                     source,
            eClassification             classification, 
            const XalanNode*            sourceNode,
            const ElemTemplateElement*  styleNode,
            const XalanDOMString&       msg,
            const XalanDOMChar*         uri,
            XalanFileLoc                lineNo,
            XalanFileLoc                charOffset)
{
    if (classification == eERROR)
    {
        m_problemListener.problem(
            source,
            classification,
            sourceNode,
            styleNode,
            msg,
            uri,
            lineNo,
            charOffset);
    }
    else if (m_warningStream != 0)
    {
        m_warningString.erase();

        DOMStringPrintWriter    thePrintWriter(m_warningString);

        ProblemListenerDefault::defaultFormat(
            thePrintWriter,
            source,
            classification,
            sourceNode,
            styleNode,
            msg,
            uri,
            lineNo,
            charOffset);

        *m_warningStream << m_warningString;
    }
}



void
XalanTransformerProblemListener::problem(
            eSource                     source,
            eClassification             classification, 
            const XalanDOMString&       msg,
            const Locator*              locator,
            const XalanNode*            sourceNode)
{
    if (classification == eERROR)
    {
        m_problemListener.problem(
            source,
            classification,
            msg,
            locator,
            sourceNode);
    }
    else if (m_warningStream != 0)
    {
        m_warningString.erase();

        DOMStringPrintWriter    thePrintWriter(m_warningString);

        ProblemListenerDefault::defaultFormat(
            thePrintWriter,
            source,
            classification,
            msg,
            locator,
            sourceNode);

        *m_warningStream << m_warningString;
    }
}



void
XalanTransformerProblemListener::problem(
            eSource                     source,
            eClassification             classification, 
            const XalanDOMString&       msg,
            const XalanNode*            sourceNode)
{
    if (classification == eERROR)
    {
        m_problemListener.problem(
            source,
            classification,
            msg,
            sourceNode);
    }
    else if (m_warningStream != 0)
    {
        m_warningString.erase();

        DOMStringPrintWriter    thePrintWriter(m_warningString);

        ProblemListenerDefault::defaultFormat(
            thePrintWriter,
            source,
            classification,
            msg,
            sourceNode);

        *m_warningStream << m_warningString;
    }
}



XALAN_CPP_NAMESPACE_END
