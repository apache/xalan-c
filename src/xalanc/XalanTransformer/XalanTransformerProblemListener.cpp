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
			StreamType*		theWarningStream,
			PrintWriter*	thePrintWriter) :
	ProblemListener(),
	m_problemListener(thePrintWriter),
	m_warningStream(theWarningStream)
{
}



XalanTransformerProblemListener::~XalanTransformerProblemListener()
{
}



void
XalanTransformerProblemListener::setPrintWriter(PrintWriter*	thePrintWriter)
{
	m_problemListener.setPrintWriter(thePrintWriter);
}



void
XalanTransformerProblemListener::problem(
			eProblemSource				where,
			eClassification				classification, 
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode,
			const XalanDOMString&		msg,
			const XalanDOMChar*			uri,
			int							lineNo,
			int							charOffset)
{
	if (classification == eERROR)
	{
		m_problemListener.problem(
			where,
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
		XalanDOMString	theWarning;

		DOMStringPrintWriter	thePrintWriter(theWarning);

		ProblemListenerDefault::problem(
			thePrintWriter,
			where,
			classification,
			sourceNode,
			styleNode,
			msg,
			uri,
			lineNo,
			charOffset);

		*m_warningStream << theWarning;
	}
}



XALAN_CPP_NAMESPACE_END
