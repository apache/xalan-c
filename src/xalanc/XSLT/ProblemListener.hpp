/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
/*
 * $Id$
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
			int							lineNo,
			int							charOffset) = 0;

};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_PROBLEMLISTENER_HEADER_GUARD
