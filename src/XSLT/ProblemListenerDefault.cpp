/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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



#include <XalanDOM/XalanNode.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/PrintWriter.hpp>



static const char* const	errorHeader = "Error: ";
static const char* const	warningHeader = "Warning: ";

static const char* const	xslHeader = "XSL ";
static const char* const	xmlHeader = "XML ";
static const char* const	queryHeader = "PATTERN ";

static const char* const	styleTreeNodeHeader = ", style tree node: ";
static const char* const	sourceTreeNodeHeader = ", source tree node: ";
static const char* const	lineNoHeader = ", line ";
static const char* const	charOffsetHeader = ", offset ";



ProblemListenerDefault::ProblemListenerDefault(PrintWriter*		pw) :
	ProblemListener(),
	m_pw(pw)
{
}



ProblemListenerDefault::~ProblemListenerDefault()
{
	m_pw = 0;
}



void
ProblemListenerDefault::setPrintWriter(PrintWriter*		pw)
{
	m_pw = pw;
}



bool
ProblemListenerDefault::problem(
			eProblemSource			where,
			eClassification			classification, 
			const XalanNode*		styleNode,
			const XalanNode*		sourceNode,
			const XalanDOMString&	msg,
			const XalanDOMChar*		/* id */,
			int						lineNo,
			int						charOffset)
{
	if (m_pw != 0)
	{
		if (eXMLPARSER == where)
		{
			m_pw->print(xmlHeader);
		}
		else if (eQUERYENGINE == where)
		{
			m_pw->print(queryHeader);
		}
		else
		{
			m_pw->print(xslHeader);
		}

		if (eERROR == classification)
		{
			m_pw->print(errorHeader);
		}
		else
		{
			m_pw->print(warningHeader);
		}

		m_pw->print(msg);

		if (0 != styleNode)
		{
			m_pw->print(styleTreeNodeHeader);
			m_pw->print(styleNode->getNodeName());
		}

		if (0 != sourceNode)
		{
			m_pw->print(sourceTreeNodeHeader);
			m_pw->print(sourceNode->getNodeName());
		}

		if (0 != lineNo)
		{
			m_pw->print(lineNoHeader);
			m_pw->print(lineNo);
		}

		if (0 != charOffset)
		{
			m_pw->print(charOffsetHeader);
			m_pw->print(charOffset);
		}

		m_pw->println();
	}

	return classification == eERROR ? true : false;
}
