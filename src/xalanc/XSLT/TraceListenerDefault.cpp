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

#include "TraceListenerDefault.hpp"



#include <xalanc/PlatformSupport/PrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanLocator.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathExecutionContext.hpp>



#include "Constants.hpp"
#include "ElemTextLiteral.hpp"
#include "ElemTemplate.hpp"
#include "GenerateEvent.hpp"
#include "SelectionEvent.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetRoot.hpp"
#include "TracerEvent.hpp"



XALAN_CPP_NAMESPACE_BEGIN



TraceListenerDefault::TraceListenerDefault(
            PrintWriter&            thePrintWriter,
            MemoryManager&          theManager,
            bool                    traceTemplates,
            bool                    traceElements,
            bool                    traceGeneration,
            bool                    traceSelection) :
    TraceListener(),
    m_executionContext(0),
    m_printWriter(thePrintWriter),
    m_traceTemplates(traceTemplates),
    m_traceElements(traceElements),
    m_traceGeneration(traceGeneration),
    m_traceSelection(traceSelection),
    m_memoryManager(theManager)
{
}



TraceListenerDefault::TraceListenerDefault(
            XPathExecutionContext&  theExecutionContext,
            PrintWriter&            thePrintWriter,
            MemoryManager&          theManager,
            bool                    traceTemplates,
            bool                    traceElements,
            bool                    traceGeneration,
            bool                    traceSelection) :
    TraceListener(),
    m_executionContext(&theExecutionContext),
    m_printWriter(thePrintWriter),
    m_traceTemplates(traceTemplates),
    m_traceElements(traceElements),
    m_traceGeneration(traceGeneration),
    m_traceSelection(traceSelection),
    m_memoryManager(theManager)
{
}



TraceListenerDefault::~TraceListenerDefault()
{
}



void
TraceListenerDefault::trace(const TracerEvent&  ev)
{
    switch (ev.m_styleNode.getXSLToken())
    {
    case StylesheetConstructionContext::ELEMNAME_TEXT_LITERAL_RESULT:
        if (m_traceElements == true)
        {
            printNodeInfo(ev.m_styleNode);

            m_printWriter.print(": ");
            m_printWriter.print(ev.m_styleNode.getElementName());
            m_printWriter.print("    ");

            const ElemTextLiteral&  etl =
                static_cast<const ElemTextLiteral&>(ev.m_styleNode);

            m_printWriter.println(etl.getText());
        }
        break;

    case StylesheetConstructionContext::ELEMNAME_TEMPLATE:
        if (m_traceTemplates == true || m_traceElements == true)
        {
            const ElemTemplate&     et =
                static_cast<const ElemTemplate&>(ev.m_styleNode);

            XalanDOMString lineNumbString(m_memoryManager);

            NumberToDOMString(ev.m_styleNode.getLineNumber(), lineNumbString);

            XalanDOMString colNumbString(m_memoryManager);

            NumberToDOMString(ev.m_styleNode.getColumnNumber(), colNumbString);

            XalanDOMString  msg(m_memoryManager);

            m_printWriter.print(
                XalanMessageLoader::getMessage(
                    msg,
                    XalanMessages::LineNumberColumnNumber_2Params,
                    lineNumbString,
                    colNumbString));

            m_printWriter.print(": ");
            m_printWriter.print(ev.m_styleNode.getElementName());

            const XPath* const  theMatchPattern =
                et.getMatchPattern();

            if (0 != theMatchPattern)
            {
                m_printWriter.print(
                    XalanMessageLoader::getMessage(
                        msg,
                        XalanMessages::MatchIs_1Param,
                        theMatchPattern->getExpression().getCurrentPattern()));

            }

            const XalanQName&   theName = et.getNameAttribute();

            if (theName.isEmpty() == false)
            {
                m_printWriter.print(
                    XalanMessageLoader::getMessage(
                        msg,
                        XalanMessages::NameIs_1Param));

                const XalanDOMString&   theNamespace =
                    theName.getNamespace();

                if (theNamespace.empty() == false)
                {
                    m_printWriter.print(theNamespace);
                    m_printWriter.print(":");
                }

                m_printWriter.print(theName.getLocalPart());
                m_printWriter.print("\" ");
            }

            m_printWriter.println();
        }
        break;

    default:
        if (m_traceElements == true)
        {
            XalanDOMString lineNumbString(m_memoryManager);
            NumberToDOMString(ev.m_styleNode.getLineNumber(), lineNumbString);
            
            XalanDOMString colNumbString(m_memoryManager);
            NumberToDOMString(ev.m_styleNode.getColumnNumber(), colNumbString);

            XalanDOMString  msg(m_memoryManager);

            m_printWriter.print(
                XalanMessageLoader::getMessage(
                    msg,
                    XalanMessages::LineNumberColumnNumber_2Params,
                    lineNumbString,
                    colNumbString));

            m_printWriter.print(": ");
            m_printWriter.println(ev.m_styleNode.getElementName());
        }
        break;
    }
}



void
TraceListenerDefault::processNodeList(const NodeRefListBase&        nl)
{
    m_printWriter.println();

    const NodeRefListBase::size_type    n = nl.getLength();

    XalanDOMString  msg(m_memoryManager);

    if(n == 0)
    {
        m_printWriter.println(
            XalanMessageLoader::getMessage(
                msg,
                XalanMessages::EmptyNodeList));
    }
    else
    {
        for (NodeRefListBase::size_type i = 0; i < n; i++)
        {
            assert(nl.item(i) != 0);

            m_printWriter.print("     ");

            if (m_executionContext != 0)
            {
                DOMServices::getNodeData(*nl.item(i), *m_executionContext, msg);
            }
            else
            {
                DOMServices::getNodeData(*nl.item(i), msg);
            }

            m_printWriter.println(msg);
        }
    }
}



void
TraceListenerDefault::selected(const SelectionEvent&    ev)
{
    if (m_traceSelection == true)
    {
        const ElemTemplateElement&  ete = ev.m_styleNode;

        if (ev.m_styleNode.getLineNumber() == XalanLocator::getUnknownValue())
        {
            // You may not have line numbers if the selection is occuring from a
            // default template.

            ElemTemplateElement* const  parent = ete.getParentNodeElem();

            XalanDOMString  msg(m_memoryManager);

            if (parent == ete.getStylesheet().getStylesheetRoot().getDefaultRootRule())
            {
                m_printWriter.print(
                    XalanMessageLoader::getMessage(
                        msg,
                        XalanMessages::DefaultRootRule));
            }
            else if (parent == ete.getStylesheet().getStylesheetRoot().getDefaultTextRule())
            {
                m_printWriter.print(
                    XalanMessageLoader::getMessage(
                        msg,
                        XalanMessages::DefaultRootRule_1Param,
                        Constants::ATTRNAME_DATATYPE));
            }
            else if (parent == ete.getStylesheet().getStylesheetRoot().getDefaultRule())
            {
                m_printWriter.print(
                    XalanMessageLoader::getMessage(
                        msg,
                        XalanMessages::DefaultRootRule_1Param,
                        " "));
            }
        }
        else
        {
            XalanDOMString  msg(m_memoryManager);
            XalanDOMString  lineNumb(m_memoryManager);
            XalanDOMString  columnNumber(m_memoryManager);

            NumberToDOMString(ev.m_styleNode.getLineNumber(), lineNumb);

            NumberToDOMString( ev.m_styleNode.getColumnNumber(), columnNumber);

            m_printWriter.print(
                XalanMessageLoader::getMessage(
                    msg, 
                    XalanMessages::TextAndColumnNumber_2Param,
                    lineNumb,
                    columnNumber));
        }

        m_printWriter.print(ete.getElementName());
        m_printWriter.print(", ");
        m_printWriter.print(ev.m_attributeName);
        m_printWriter.print("=\"");
        m_printWriter.print(ev.m_xpathExpression);
        m_printWriter.print("\": ");

        if (ev.m_selection.null() == true)
        {
            if (ev.m_type == SelectionEvent::eBoolean)
            {
                m_printWriter.println(ev.m_boolean == true ? "true" : "false");
            }
            else if (ev.m_type == SelectionEvent::eNodeSet)
            {
                assert(ev.m_nodeList != 0);

                processNodeList(*ev.m_nodeList);
            }
        }
        else if(ev.m_selection->getType() == XObject::eTypeNodeSet)
        {
            const NodeRefListBase&  theNodeSet =
                m_executionContext != 0 ?
                    ev.m_selection->nodeset() :
                    ev.m_selection->nodeset();

            processNodeList(theNodeSet);
        }
        else
        {
            const XalanDOMString&   theString =
                m_executionContext != 0 ?
                    ev.m_selection->str(*m_executionContext) :
                    ev.m_selection->str();

            m_printWriter.println(theString);
        }
    }
}



void
TraceListenerDefault::generated(const GenerateEvent&    ev)
{
    if(m_traceGeneration == true)
    {
        switch(ev.m_eventType)
        {
        case GenerateEvent::EVENTTYPE_STARTDOCUMENT:
            m_printWriter.println("STARTDOCUMENT");
            break;

        case GenerateEvent::EVENTTYPE_ENDDOCUMENT:
            m_printWriter.println();
            m_printWriter.println("ENDDOCUMENT");
            break;

        case GenerateEvent::EVENTTYPE_STARTELEMENT:
            m_printWriter.print("STARTELEMENT: ");
            m_printWriter.println(ev.m_name);
            break;

        case GenerateEvent::EVENTTYPE_ENDELEMENT:
            m_printWriter.print("ENDELEMENT: ");
            m_printWriter.println(ev.m_name);
            break;

        case GenerateEvent::EVENTTYPE_CHARACTERS:
            m_printWriter.print("CHARACTERS: ");
            m_printWriter.println(ev.m_characters);
            break;

        case GenerateEvent::EVENTTYPE_CDATA:
            m_printWriter.print("CDATA: ");
            m_printWriter.println(ev.m_characters);
            break;

        case GenerateEvent::EVENTTYPE_COMMENT:
            m_printWriter.print("COMMENT: ");
            m_printWriter.println(ev.m_data);
            break;

        case GenerateEvent::EVENTTYPE_PI:
            m_printWriter.print("PI: ");
            m_printWriter.print(ev.m_name);
            m_printWriter.print(", ");
            m_printWriter.println(ev.m_data);
            break;

        case GenerateEvent::EVENTTYPE_ENTITYREF:
            m_printWriter.println("ENTITYREF: ");
            m_printWriter.println(ev.m_name);
            break;

        case GenerateEvent::EVENTTYPE_IGNORABLEWHITESPACE:
            m_printWriter.println("IGNORABLEWHITESPACE");
            break;
        }
    }
}



void
TraceListenerDefault::printNodeInfo(const ElemTemplateElement&  node)
{
    const XalanDOMString&   uri = node.getURI();

    XalanDOMString msg(m_memoryManager), lineNumb(m_memoryManager), columnNumb(m_memoryManager);

    NumberToDOMString(node.getLineNumber(), lineNumb);

    NumberToDOMString(node.getColumnNumber(), columnNumb);

    m_printWriter.print(
        XalanMessageLoader::getMessage(
            msg,
            XalanMessages::TextAndColumnNumber_2Param,
            lineNumb,
            columnNumb));

    if (uri.length() != 0)
    {
        m_printWriter.print(" (");
        m_printWriter.print(uri);
        m_printWriter.print(")");
    }
}



XALAN_CPP_NAMESPACE_END
