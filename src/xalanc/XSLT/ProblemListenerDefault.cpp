/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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
/**
 * 
 * @author David N. Bertoni (david_n_bertoni@lotus.com)
 * */



// Base class header file.
#include "ProblemListenerDefault.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrintWriter.hpp>



#include <xalanc/XSLT/ElemTemplateElement.hpp>



XALAN_CPP_NAMESPACE_BEGIN



static const char* const	errorHeader = "error: ";
static const char* const	warningHeader = "warning: ";
static const char* const	messageHeader = "message: ";

static const char* const	xslHeader = "XSLT ";
static const char* const	xmlHeader = "XML ";
static const char* const	xpathHeader = "XPath ";

static const char* const	styleTreeNodeHeader = ", style tree node: ";
static const char* const	sourceTreeNodeHeader = ", source tree node: ";
static const char* const	locationOpen = " (";
static const char* const	uriHeader = "";
static const char* const	lineNoHeader = ", line ";
static const char* const	charOffsetHeader = ", column ";
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
		pw.print(errorHeader);
	}
	else if (eWARNING == classification)
	{
		pw.print(warningHeader);
	}
	else
	{
		pw.print(messageHeader);
	}

	pw.print(msg);

	if (0 != styleNode)
	{
		pw.print(styleTreeNodeHeader);
		pw.print(styleNode->getNodeName());
	}

	if (0 != sourceNode)
	{
		pw.print(sourceTreeNodeHeader);
		pw.print(sourceNode->getNodeName());
	}

	pw.print(locationOpen);

	if (0 != uri)
	{
		pw.print(uriHeader);
		pw.print(uri);
	}

	pw.print(lineNoHeader);
	pw.print(lineNo);

	pw.print(charOffsetHeader);
	pw.print(charOffset);

	pw.print(locationClose);

	pw.println();
}



XALAN_CPP_NAMESPACE_END
