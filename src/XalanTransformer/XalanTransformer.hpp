/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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
#if !defined(XALANTRANSFORMER_HEADER_GUARD)
#define XALANTRANSFORMER_HEADER_GUARD



// Base include file.  Must be first.
#include <XalanTransformer/XalanTransformerDefinitions.hpp>



#include <vector>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathFactoryBlock.hpp>
#include <XPath/XPathFactoryDefault.hpp>



#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/StylesheetRoot.hpp>
#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <XSLT/XSLTResultTarget.hpp>



#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



#include <XalanTransformer/XalanCompiledStylesheet.hpp>
#include <XalanTransformer/XalanDefaultParsedSource.hpp>
#include <XalanTransformer/XalanParsedSource.hpp>
#include <XalanTransformer/XercesDOMParsedSource.hpp>
#include <XalanTransformer/XalanTransformerOutputStream.hpp>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif



#if !defined(XALAN_NO_NAMESPACES)
	using std::ostream;
	using std::istream;
#endif



/**
 * This is a simple C++ interface for some common usage patterns. It's 
 * the user's responsibility to call initialize and terminate for Xerces 
 * and Xalan before creating and after deleting any XalanTransformer  
 * instances.
*/
class XALAN_TRANSFORMER_EXPORT XalanTransformer
{
public:
	
	XalanTransformer();

	virtual
	~XalanTransformer();

	/**
	 * Initialize Xalan.
	 * Should be called only once per process before creating any
	 * instances of XalanTransformer. See class XSLTInit.
	 */
	static void
	initialize();

	/**
	 * Terminate Xalan.
	 * Should be called only once per process after deleting all
	 * instances of XalanTransformer. See class XSLTInit.
	 */
	static void
	terminate();

	/**
	 * Transform will apply the stylesheet source to the parsed xml source
	 * and write the transformation output to the target. 
	 *
	 * @param theParsedXML			the parsed input source
	 * @param theStylesheetSource	stylesheet source
	 * @param theResultTarget		output source
	 * @return	0 for success
	 */
	int
	transform(
			XalanParsedSource&			theParsedXML, 
			const XSLTInputSource&		theStylesheetSource,
			const XSLTResultTarget&		theResultTarget);

	/**
	 * Transform will apply the compiled stylesheet to the parsed xml source
	 * and write the transformation output to the target. 
	 *
	 * @param theParsedXML			the parsed input source
	 * @param theCompiledStylesheet	pointer to a compiled stylesheet
	 * @param theResultTarget		output source 
	 * @return	0 for success
	 */
	int
	transform(
			XalanParsedSource&				theParsedXML, 
			const XalanCompiledStylesheet*	theCompiledStylesheet,
			const XSLTResultTarget&			theResultTarget);

	/**
	 * Transform will apply the stylesheet source to the input source
	 * and write the transformation output to the target. The input 
	 * source and result target can be a file name, a stream or a root
	 * node.
	 *
	 * @param theInputSource		input source
	 * @param theStylesheetSource	stylesheet source
	 * @param theResultTarget		output source
	 * @return	0 for success
	 */
	int
	transform(
			const XSLTInputSource&		theInputSource, 
			const XSLTInputSource&		theStylesheetSource,
			const XSLTResultTarget&		theResultTarget);

	/**
	 * Transform will apply the compiled stylesheet to the input source
	 * and write the transformation output to the target. The input 
	 * source and result target can be a file name, a stream or a root
	 * node. 
	 *
	 * @param theInputSource		input source
	 * @param theCompiledStylesheet	pointer to a compiled stylesheet
	 * @param theResultTarget		output source 
	 * @return	0 for success
	 */
	int
	transform(
			const XSLTInputSource&			theInputSource, 
			const XalanCompiledStylesheet*	theCompiledStylesheet,
			const XSLTResultTarget&			theResultTarget);

	/**
	 * Transform will apply the stylesheet provided as a PI in the 
	 * XML of the input source and write the transformation output to 
	 * the target. The input source and result target can be a file 
	 * name, a stream or a root node.
	 *
	 * @param theInputSource		input source
	 * @param theResultTarget		output source tree
	 * @return	0 for success
	 */
	int
	transform(
			const XSLTInputSource&		theInputSource, 		
			const XSLTResultTarget&		theResultTarget);

	/**
	 * Transform will apply the stylesheet source to the input source
	 * and write the transformation result to a callback function  
	 * in pre-allocated blocks. The input source can be a file name, 
	 * a stream or a root node. Upon termination, Xalan releases any 
	 * allocated memory. Data passed to the callback is not guaranteed to 
	 * be null terminated.
	 *
	 * - See XalanTransformerOutputStream and XalanOutputHandlerType 
	 * for more details.
	 * 
	 * @param theInputSource		input source
	 * @param theStylesheetSource	stylesheet source	
	 * @param theOutputHandle		void pointer passed through to callback.
	 * @param theOutputHandler		a user defined (callback) function.
	 * @param theFlushHandler		(optional) a user defined (callback) function.
	 * @return	0 for success 
	 */
	int
	transform(
			const XSLTInputSource&		theInputSource, 
			const XSLTInputSource&		theStylesheetSource,
			const void*					theOutputHandle, 
			XalanOutputHandlerType		theOutputHandler,
			XalanFlushHandlerType		theFlushHandler = 0);
	
	/**
	 * Transform will apply the compiled stylesheet to the input source
	 * and write the transformation result to a callback function  
	 * in pre-allocated blocks. The input source can be a file name, 
	 * a stream or a root node. Upon termination, Xalan releases any 
	 * allocated memory. Data passed to the callback is not guaranteed to 
	 * be null terminated.
	 *
	 * - See XalanTransformerOutputStream and XalanOutputHandlerType 
	 * for more details.
	 * 
	 * @param theInputSource		input source
	 * @param theCompiledStylesheet	pointer to a compiled stylesheet
	 * @param theOutputHandle		void pointer passed through to callback.
	 * @param theOutputHandler		a user defined (callback) function.
	 * @param theFlushHandler		(optional) a user defined (callback) function.
	 * @return	0 for success 
	 */
	int
	transform(
			const XSLTInputSource&			theInputSource, 
			const XalanCompiledStylesheet*	theCompiledStylesheet,
			const void*						theOutputHandle, 
			XalanOutputHandlerType			theOutputHandler,
			XalanFlushHandlerType			theFlushHandler = 0);

	/**
	 * Transform will apply the stylesheet provided as a PI in the 
	 * XML of the input source and write  the transformation result to a  
	 * callback function in pre-allocated blocks. The input source can be 
	 * a file name, a stream or a root node. Upon termination, Xalan 
	 * releases any allocated memory. Data passed to the callback is not 
	 * guaranteed to be null terminated.
	 *
	 * - See XalanTransformerOutputStream and XalanOutputHandlerType 
	 * for more details.
	 * 
	 * @param theInputSource		input source
	 * @param theOutputHandle		void pointer passed through to callback.
	 * @param theOutputHandler		a user defined (callback) function.
	 * @param theFlushHandler		(optional) a user defined (callback) function.
	 * @return	0 for success 
	 */
	int
	transform(
			const XSLTInputSource&		theInputSource, 		
			const void*					theOutputHandle, 
			XalanOutputHandlerType		theOutputHandler,
			XalanFlushHandlerType		theFlushHandler =0);

	/**
	 * Creates a complied stylesheet.  The input source can be 
	 * a file name, a stream or a root node.
	 *
	 * @param theStylesheetSource	input source
	 * @return	a pointer to a XalanCompiledStylesheet or 0 for failure.
	 */
	XalanCompiledStylesheet*
	compileStylesheet(const XSLTInputSource&		theStylesheetSource);

	/**
	 * Set a top-level stylesheet parameter.  This value can be evaluated via
	 * xsl:param-variable.
	 *
	 * @param key name of the param
	 * @param expression expression that will be evaluated
	 */
	void
	setStylesheetParam(
			const XalanDOMString&	key,
			const XalanDOMString&	expression);
	void
	setStylesheetParam(
			const char*				key,
			const char*				expression);

	/**
	 * Parse source document.  The input source can be 
	 * a file name, a stream or a root node.
	 *
	 * @param theInputSource	input source
	 * @param useXercesDOM		input use default or xerces dom source tree
	 * @return	a pointer to a XalanParsedSource or 0 for failure.
	 */
	XalanParsedSource*
	parseSource(
			const XSLTInputSource&	theInputSource, 
			bool  useXercesDOM = 0);

	/**
	 * Returns the last error that occurred as a 
	 * result of calling transform.	
	 *
	 * @return	error message const character pointer.
	 */
	const char*
	getLastError() const;


#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanCompiledStylesheet*>			CompiledStylesheetPtrVectorType;
	typedef vector<XalanParsedSource*>					ParsedSourcePtrVectorType;
	typedef pair<XalanDOMString, XalanDOMString>    	ParamPairType;
	typedef vector<ParamPairType>		                ParamPairVectorType;
#else
	typedef std::vector<const XalanCompiledStylesheet*>	CompiledStylesheetPtrVectorType;
	typedef std::vector<const XalanParsedSource*>		ParsedSourcePtrVectorType;
	typedef std::pair<XalanDOMString, XalanDOMString>	ParamPairType;
	typedef std::vector<ParamPairType>	                ParamPairVectorType;
#endif

protected:

private:

	void 
	reset();

	StylesheetExecutionContextDefault		m_stylesheetExecutionContext;

	CompiledStylesheetPtrVectorType			m_compiledStylesheets;

	ParsedSourcePtrVectorType				m_parsedSources;

    ParamPairVectorType                     m_paramPairs;

	CharVectorType							m_errorMessage;

	static XSLTInit*						m_xsltInit;
};

#endif	// XALANTRANSFORMER_HEADER_GUARD



