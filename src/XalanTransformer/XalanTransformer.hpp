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
#if !defined(XALAN_TRANSFORMER_HEADER_GUARD)
#define XALAN_TRANSFORMER_HEADER_GUARD



// Base include file.  Must be first.
#include <XalanTransformer/XalanTransformerDefinitions.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <XPath/XObjectFactoryDefault.hpp>
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
 * the user's responsibility to call initialize and terminate 
 * before creating and after deleting any XalanTransformer instances 
 * respectively. After calling XalanTransformToData, the user should   
 * call XalanFreeData to release the memory allocated by that operation.
*/
class XALAN_TRANSFORMER_EXPORT XalanTransformer
{
public:
	
	XalanTransformer();

	virtual ~XalanTransformer();

	/**
	 * Initialize Xerces and Xalan.
	 * Should be called only once per process before creating any
	 * instances of XalanTransformer. See class XSLTInit.
	 */
	static void
	initialize();

	/**
	 * Terminate Xalan and Xerces.
	 * Should be called only once per process after deleting all
	 * instances of XalanTransformer. See class XSLTInit.
	 */
	static void
	terminate();

	/**
	 * Returns the last error that occurred as a 
	 * result of calling transform.	
	 *
	 * @return	error message const character pointer.
	 */
	const char*
	getLastError() const;

	/**
	 * Transform the source tree to output in the given result tree target.
	 * The processor will apply the the stylesheet source to the input source
	 * and write the transformation output to the target. Called internally by 
	 * all transform methods.
	 *
	 * @param inputSource		input source
	 * @param stylesheetSource	stylesheet source
	 * @param outputTarget		output source tree
	 * @return	0 for success
	 */
	int
	transform(
			const XSLTInputSource&		inputSource, 
			const XSLTInputSource&		stylesheetSource,
			XSLTResultTarget&			outputTarget);

	/**
	 * Transform the XML source tree to the given result file.
	 * The processor will apply the the stylesheet source to the input source
	 * and write the transformation output to the output file.
	 *
	 * @param theXMLFileName	filename of XML input file
	 * @param theXSLFileName	filename of stylesheet file
	 * @param theOutFileName	filename of output file
	 * @return	0 for success
	 */
	int
	transform(
			const char*					theXMLFileName, 
			const char*					theXSLFileName,
			const char*					theOutFileName);

	/**
	 * Transform the XML source tree to the given result file.
	 * The processor will apply the the stylesheet provided as a PI in the 
	 * the XML to the input file and write the transformation output to the 
	 * output file.
	 *
	 * @param theXMLFileName	filename of XML input file	
	 * @param theOutFileName	filename of output file
	 * @return	0 for success
	 */
	int
	transform(
			const char*					theXMLFileName, 
			const char*					theOutFileName);

	/**
	 * Transform the XML source tree to an output stream.
	 * The processor will apply the the stylesheet file to the input file
	 * and write the transformation output to the output stream.
	 *
	 * @param theXMLFileName	filename of XML input source
	 * @param theXSLFileName	filename of stylesheet source
	 * @param theOutStream		a std ostream for the output
	 * @return	0 for success
	 */
	int
	transform(
			const char*					theXMLFileName, 
			const char*					theXSLFileName,
			ostream&					theOutStream);

	/**
	 * Transform the XML source tree to an output stream.
	 * The processor will apply the the stylesheet provided as a PI in the
	 * the XML to the input file and write the transformation output to the 
	 * output stream.
	 *
	 * @param theXMLFileName	filename of XML input source
	 * @param theXSLFileName	filename of stylesheet source
	 * @param theOutStream		a std ostream for the output
	 * @return	0 for success
	 */
	int
	transform(
			const char*					theXMLFileName, 
			ostream&					theOutStream);

	/**
	 * Transform the XML source tree to an output stream.
	 * The processor will apply the the stylesheet stream to the input stream
	 * and write the transformation output to the output stream.
	 *
	 * @param theXMLInStream	a std istream for the input
	 * @param theXSLInStream	a std istream for the input
	 * @param theOutStream		a std ostream for the output
	 * @return	0 for success
	 */
	int
	transform(
			istream&					theXMLInStream, 
			istream&					theXSLInStream,
			ostream&					theOutStream);
	
	/**
	 * Transform the XML source tree to an output stream.
	 * The processor will apply the the stylesheet provided as a PI in the 
	 * XML of the input stream and write the transformation output to the 
	 * output stream.
	 *
	 * @param theXMLInStream	a std istream for the input
	 * @param theOutStream		a std ostream for the output
	 * @return	0 for success
	 */
	int
	transform(
			istream&					theXMLInStream, 
			ostream&					theOutStream);

	/**
	 * Transform the XML source tree to a callback function.
	 * The processor will apply the stylesheet file to the input file
	 * and allocate the transformation result to a callback function  
	 * in pre-allocated blocks. Upon termination, Xalan releases any 
	 * allocated memory. Data passed to the callback is not guaranteed to 
	 * be null terminated.
	 *
	 * - See XalanTransformerOutputStream and XalanOutputHandlerType 
	 * for more details.
	 * 
	 * @param theXMLFileName	filename of XML input source
	 * @param theXSLFileName	filename of stylesheet source	 
	 * @param theOutputHandle	void pointer passed through to callback.
	 * @param theOutputHandler	a user defined (callback) function.
	 * @param theFlushHandler	(optional) a user defined (callback) function.
	 * @return	0 for success 
	 */
	int
	transform(
			const char*					theXMLFileName, 
			const char*					theXSLFileName,
			const void*					theOutputHandle, 
			XalanOutputHandlerType		theOutputHandler,
			XalanFlushHandlerType		theFlushHandler = 0);

protected:

private:

	void 
	reset();

	XalanSourceTreeDOMSupport				m_domSupport;

	XalanSourceTreeParserLiaison			m_parserLiaison;
	
	XSLTProcessorEnvSupportDefault			m_xsltprocessorEnvSupport;
	
	XObjectFactoryDefault					m_xobjectFactory;
	
	XPathFactoryDefault						m_xpathFactory;
	
	XSLTEngineImpl							m_processor;
	
	StylesheetConstructionContextDefault	m_stylesheetConstructionContext;
	
	StylesheetExecutionContextDefault		m_stylesheetExecutionContext;

	CharVectorType							m_errorMessage;

	static XSLTInit*						m_xsltInit;
};



#endif	// XALAN_TRANSFORMER_HEADER_GUARD



