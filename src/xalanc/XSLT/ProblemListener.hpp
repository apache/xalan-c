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
#if !defined(XALAN_PROBLEMLISTENER_HEADER_GUARD)
#define XALAN_PROBLEMLISTENER_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class ElemTemplateElement;
class XalanNode;
class PrintWriter;



/**
 * This is the abstract class that the XSL processor uses when it has a
 * problem of some kind, that requires a message, an error or a warning. Users
 * should ask the XSLTProcessor class to setProblemListener if they wish an
 * object instance to be called when a problem event occurs.
 */
class XALAN_XSLT_EXPORT ProblemListener
{
public:

	/// Sources of problem
	enum eProblemSource { eXMLPARSER		= 1,
						  eXSLPROCESSOR		= 2,
						  eXPATH			= 3 };

	/// Severity of problem
	enum eClassification {	eMESSAGE	= 0,
							eWARNING	= 1,
							eERROR		= 2 };

	ProblemListener();

	virtual
	~ProblemListener();

	/** 
	 * Set the print writer to which the problem is reported.
	 * 
	 * @param pw writer to receive messages
	 */
	virtual void
	setPrintWriter(PrintWriter*		pw) = 0;

	/**
 	 * Function that is called when a problem event occurs.
     * 
     * @param   where          either in XMLPARSER, XSLPROCESSOR, or QUERYENGINE
     * @param   classification either MESSAGE, ERROR or WARNING
	 * @param   sourceNode     source tree node where the problem occurred
	 *                         (may be 0)
	 * @param   styleNode      style tree node where the problem occurred
	 *                         (may be 0)
	 * @param   msg            string message explaining the problem.
	 * @param   uri            the URI of the document where the problem occurred.  May be 0.
	 * @param   lineNo         line number where the problem occurred.
	 * @param   charOffset     character offset where the problem.
	 */
	virtual void
	problem(
			eProblemSource				where,
			eClassification				classification,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode,
			const XalanDOMString&		msg,
			const XalanDOMChar*			uri,
			XalanFileLoc				lineNo,
			XalanFileLoc				charOffset) = 0;

};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_PROBLEMLISTENER_HEADER_GUARD
