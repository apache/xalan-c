/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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



static const char* const	xslHeader = "XSLT ";
static const char* const	xmlHeader = "XML ";
static const char* const	xpathHeader = "XPath ";

static const char* const	uriHeader = "";

static const char* const	locationOpen = " (";
static const char* const	locationClose = ")";


ProblemListenerDefault::ProblemListenerDefault(PrintWriter*		pw) :
	ProblemListener(),
	m_pw(pw)
{
}



ProblemListenerDefault::~ProblemListenerDefault()
{
}



void
ProblemListenerDefault::setPrintWriter(PrintWriter*		pw)
{
	m_pw = pw;
}



void
ProblemListenerDefault::problem(
			eProblemSource				where,
			eClassification				classification,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode,
			const XalanDOMString&		msg,
			const XalanDOMChar*			uri,
			int							lineNo,
			int							charOffset)
{
	if (m_pw != 0)
	{
		problem(*m_pw, where, classification, sourceNode, styleNode, msg, uri, lineNo, charOffset);
	}
}



void
ProblemListenerDefault::problem(
			PrintWriter&				pw,
			eProblemSource				where,
			eClassification				classification,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode,
			const XalanDOMString&		msg,
			const XalanDOMChar*			uri,
			int							lineNo,
			int							charOffset)
{
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
		pw.print(xslHeader);
	}

	if (eERROR == classification)
	{
		pw.print(XalanMessageLoader::getMessage(XalanMessages::Error_1Param, msg));
	}
	else if (eWARNING == classification)
	{
		pw.print(XalanMessageLoader::getMessage(XalanMessages::Warning_1Param, msg));
	}
	else
	{
		pw.print(XalanMessageLoader::getMessage(XalanMessages::Message_1Param, msg));
	}

	if (0 != styleNode)
	{
		pw.print(XalanMessageLoader::getMessage(XalanMessages::StyleTreeNode_1Param, styleNode->getElementName()));
	}

	if (0 != sourceNode)
	{
		pw.print(XalanMessageLoader::getMessage(XalanMessages::SourceTreeNode_1Param, sourceNode->getNodeName()));
	}

	pw.print(locationOpen);

	if (0 != uri)
	{
		pw.print(uriHeader);
		pw.print(uri);
	}

	const XalanDOMString lineNoString = LongToDOMString(lineNo);

	const XalanDOMString charOffsetString = LongToDOMString(charOffset);

	pw.print(XalanMessageLoader::getMessage(XalanMessages::LineNumberColumnNumber_2Params, lineNoString , charOffsetString));

	pw.print(locationClose);

	pw.println();
}



XALAN_CPP_NAMESPACE_END
