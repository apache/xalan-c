/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#include "TraceListenerDefault.hpp"



#include <xalanc/PlatformSupport/PrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanLocator.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XPath.hpp>



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
			PrintWriter&	thePrintWriter,
			bool			traceTemplates,
			bool			traceElements,
			bool			traceGeneration,
			bool			traceSelection) :
	m_printWriter(thePrintWriter),
	m_traceTemplates(traceTemplates),
	m_traceElements(traceElements),
	m_traceGeneration(traceGeneration),
	m_traceSelection(traceSelection)
{
}



TraceListenerDefault::~TraceListenerDefault()
{
}



void
TraceListenerDefault::trace(const TracerEvent&	ev)
{
	switch(ev.m_styleNode.getXSLToken())
	{
	case StylesheetConstructionContext::ELEMNAME_TEXT_LITERAL_RESULT:
		if(m_traceElements == true)
		{
			printNodeInfo(ev.m_styleNode);

			m_printWriter.print(XALAN_STATIC_UCODE_STRING(": "));
			m_printWriter.print(ev.m_styleNode.getElementName());
			m_printWriter.print(XALAN_STATIC_UCODE_STRING("    "));

			const ElemTextLiteral&	etl =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const ElemTextLiteral&)ev.m_styleNode;
#else
				static_cast<const ElemTextLiteral&>(ev.m_styleNode);
#endif

			m_printWriter.println(etl.getText());
		}
		break;

	case StylesheetConstructionContext::ELEMNAME_TEMPLATE:
		if(m_traceTemplates == true || m_traceElements == true)
		{
			const ElemTemplate&		et =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const ElemTemplate&)ev.m_styleNode;
#else
				static_cast<const ElemTemplate&>(ev.m_styleNode);
#endif

			m_printWriter.print(XalanMessageLoader::getMessage(XalanMessages::LineNumber));
			m_printWriter.print(ev.m_styleNode.getLineNumber());
			m_printWriter.print(XALAN_STATIC_UCODE_STRING(", "));
			m_printWriter.print(XalanMessageLoader::getMessage(XalanMessages::ColumnNumber));
			m_printWriter.print(ev.m_styleNode.getColumnNumber());
			m_printWriter.print(XALAN_STATIC_UCODE_STRING(": "));
			m_printWriter.print(ev.m_styleNode.getElementName());

			const XPath* const	theMatchPattern =
				et.getMatchPattern();

			if(0 != theMatchPattern)
			{
				m_printWriter.print(XalanMessageLoader::getMessage(XalanMessages::MatchIs_1Param, 
					theMatchPattern->getExpression().getCurrentPattern() ));

			}

			const XalanQName&	theName = et.getNameAttribute();

			if(theName.isEmpty() == false)
			{
				m_printWriter.print(XalanMessageLoader::getMessage(XalanMessages::NameIs_1Param));

				const XalanDOMString&	theNamespace =
					theName.getNamespace();

				if (isEmpty(theNamespace) == false)
				{
					m_printWriter.print(theNamespace);
					m_printWriter.print(XalanUnicode::charColon);
				}

				m_printWriter.print(theName.getLocalPart());
				m_printWriter.print(XALAN_STATIC_UCODE_STRING("\" "));
			}

			m_printWriter.println();
		}
		break;

	default:
		if(m_traceElements == true)
		{
			m_printWriter.print(XalanMessageLoader::getMessage(XalanMessages::LineNumber));
			m_printWriter.print(ev.m_styleNode.getLineNumber());
			m_printWriter.print(XalanMessageLoader::getMessage(XalanMessages::ColumnNumber));
			m_printWriter.print(ev.m_styleNode.getColumnNumber());
			m_printWriter.print(XALAN_STATIC_UCODE_STRING(": "));
			m_printWriter.println(ev.m_styleNode.getElementName());
		}
		break;
	}
}



void
TraceListenerDefault::processNodeList(const NodeRefListBase&		nl)
{
	m_printWriter.println();

	const NodeRefListBase::size_type	n = nl.getLength();

	if(n == 0)
	{
		m_printWriter.println(XalanMessageLoader::getMessage(XalanMessages::EmptyNodeList));
	}
	else
	{
		for(NodeRefListBase::size_type i = 0; i < n; i++)
		{
			assert(nl.item(i) != 0);

			m_printWriter.print(XALAN_STATIC_UCODE_STRING("     "));
			m_printWriter.println(DOMServices::getNodeData(*nl.item(i)));
		}
	}
}



void
TraceListenerDefault::selected(const SelectionEvent&	ev)
{
    if(m_traceSelection == true)
    {
		const ElemTemplateElement&	ete = ev.m_styleNode;

		if(ev.m_styleNode.getLineNumber() == XalanLocator::getUnknownValue())
		{
			// You may not have line numbers if the selection is occuring from a
			// default template.

			ElemTemplateElement* const	parent = ete.getParentNodeElem();

			if(parent == ete.getStylesheet().getStylesheetRoot().getDefaultRootRule())
			{
				m_printWriter.print(XalanMessageLoader::getMessage(XalanMessages::DefaultRootRule_1Param,"root"));
			}
			else if(parent == ete.getStylesheet().getStylesheetRoot().getDefaultTextRule())
			{
				m_printWriter.print(XalanMessageLoader::getMessage(XalanMessages::DefaultRootRule_1Param,"text"));
			}
			else if(parent == ete.getStylesheet().getStylesheetRoot().getDefaultRule())
			{
				m_printWriter.print(XalanMessageLoader::getMessage(XalanMessages::DefaultRootRule_1Param," "));
			}
		}
		else
		{
			m_printWriter.print(XalanMessageLoader::getMessage(XalanMessages::TextAndColumnNumber_2Param,
				LongToDOMString(ev.m_styleNode.getLineNumber()),LongToDOMString( ev.m_styleNode.getColumnNumber())));
		}

		m_printWriter.print(ete.getElementName());
		m_printWriter.print(XALAN_STATIC_UCODE_STRING(", "));
		m_printWriter.print(ev.m_attributeName);
		m_printWriter.print(XALAN_STATIC_UCODE_STRING("=\""));
		m_printWriter.print(ev.m_xpathExpression);
		m_printWriter.print(XALAN_STATIC_UCODE_STRING("\": "));

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
			processNodeList(ev.m_selection->nodeset());
		}
		else
		{
			m_printWriter.println(ev.m_selection->str());
		}
    }
}



void
TraceListenerDefault::generated(const GenerateEvent&	ev)
{
	if(m_traceGeneration == true)
	{
		switch(ev.m_eventType)
		{
		case GenerateEvent::EVENTTYPE_STARTDOCUMENT:
			m_printWriter.println(XALAN_STATIC_UCODE_STRING("STARTDOCUMENT"));
			break;

		case GenerateEvent::EVENTTYPE_ENDDOCUMENT:
			m_printWriter.println();
			m_printWriter.println(XALAN_STATIC_UCODE_STRING("ENDDOCUMENT"));
			break;

		case GenerateEvent::EVENTTYPE_STARTELEMENT:
			m_printWriter.print(XALAN_STATIC_UCODE_STRING("STARTELEMENT: "));
			m_printWriter.println(ev.m_name);
			break;

		case GenerateEvent::EVENTTYPE_ENDELEMENT:
			m_printWriter.print(XALAN_STATIC_UCODE_STRING("ENDELEMENT: "));
			m_printWriter.println(ev.m_name);
			break;

		case GenerateEvent::EVENTTYPE_CHARACTERS:
			m_printWriter.print(XALAN_STATIC_UCODE_STRING("CHARACTERS: "));
			m_printWriter.println(ev.m_characters);
			break;

		case GenerateEvent::EVENTTYPE_CDATA:
			m_printWriter.print(XALAN_STATIC_UCODE_STRING("CDATA: "));
			m_printWriter.println(ev.m_characters);
			break;

		case GenerateEvent::EVENTTYPE_COMMENT:
			m_printWriter.print(XALAN_STATIC_UCODE_STRING("COMMENT: "));
			m_printWriter.println(ev.m_data);
			break;

		case GenerateEvent::EVENTTYPE_PI:
			m_printWriter.print(XALAN_STATIC_UCODE_STRING("PI: "));
			m_printWriter.print(ev.m_name);
			m_printWriter.print(XALAN_STATIC_UCODE_STRING(", "));
			m_printWriter.println(ev.m_data);
			break;

		case GenerateEvent::EVENTTYPE_ENTITYREF:
			m_printWriter.println(XALAN_STATIC_UCODE_STRING("ENTITYREF: "));
			m_printWriter.println(ev.m_name);
			break;

		case GenerateEvent::EVENTTYPE_IGNORABLEWHITESPACE:
			m_printWriter.println(XALAN_STATIC_UCODE_STRING("IGNORABLEWHITESPACE"));
			break;
		}
	}
}



void
TraceListenerDefault::printNodeInfo(const ElemTemplateElement&	node)
{
	const XalanDOMString&	uri = node.getURI();

	m_printWriter.print(XALAN_STATIC_UCODE_STRING("Line #"));
	m_printWriter.print(node.getLineNumber());
	m_printWriter.print(XALAN_STATIC_UCODE_STRING(", "));
	m_printWriter.print(XALAN_STATIC_UCODE_STRING("Column #"));
	m_printWriter.print(node.getColumnNumber());

	if (uri.length() != 0)
	{
		m_printWriter.print(XALAN_STATIC_UCODE_STRING(", ("));
		m_printWriter.print(uri);
		m_printWriter.print(XALAN_STATIC_UCODE_STRING(")"));
	}
}



XALAN_CPP_NAMESPACE_END
