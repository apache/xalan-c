/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
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
#if !defined(XALANTRANSFORMER_HEADER_GUARD)
#define XALANTRANSFORMER_HEADER_GUARD



// Base include file.  Must be first.
#include <XalanTransformer/XalanTransformerDefinitions.hpp>



#include <vector>



#include <XPath/XalanQNameByValue.hpp>



#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTResultTarget.hpp>



class Function;
class StylesheetExecutionContextDefault;
class XSLTInit;
class XalanDocumentBuilder;
class XalanCompiledStylesheet;
class XalanParsedSource;
class XalanTransformerOutputStream;



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
			const XalanParsedSource&	theParsedXML, 
			const XSLTInputSource&		theStylesheetSource,
			const XSLTResultTarget& 	theResultTarget);

	/**
	 * Transform will apply the compiled stylesheet to the parsed xml source
	 * and write the transformation output to the target. 
	 *
	 * @param theParsedXML			the parsed input source
	 * @param theCompiledStylesheet pointer to a compiled stylesheet
	 * @param theResultTarget		output source 
	 * @return	0 for success
	 */
	int
	transform(
			const XalanParsedSource&		theParsedXML, 
			const XalanCompiledStylesheet*	theCompiledStylesheet,
			const XSLTResultTarget& 		theResultTarget);

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
			const XSLTResultTarget& 	theResultTarget);

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
			const XSLTResultTarget& 	theResultTarget);

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
			void*						theOutputHandle, 
			XalanOutputHandlerType		theOutputHandler,
			XalanFlushHandlerType		theFlushHandler = 0);
	
	/**
	 * Transform will apply the compiled stylesheet to the input source
	 * and write the transformation output to the target. The input 
	 * source and result target can be a file name, a stream or a root
	 * node. 
	 *
	 * @param theInputSource		input source
	 * @param theCompiledStylesheet pointer to a compiled stylesheet
	 * @param theResultTarget		output source 
	 * @return	0 for success
	 */
	int
	transform(
			const XSLTInputSource&			theInputSource, 
			const XalanCompiledStylesheet*	theCompiledStylesheet,
			const XSLTResultTarget& 		theResultTarget);
 
	/**
	 * Transform will apply the compiled stylesheet to the source
	 * and write the transformation result to a callback function  
	 * in pre-allocated blocks. The input source can be a file name, 
	 * a stream or a root node. Upon termination, Xalan releases any 
	 * allocated memory. Data passed to the callback is not guaranteed to 
	 * be null terminated.
	 *
	 * - See XalanTransformerOutputStream and XalanOutputHandlerType 
	 * for more details.
	 * 
	 * @param theParsedSource		Parsed source instance
	 * @param theCompiledStylesheet pointer to a compiled stylesheet
	 * @param theOutputHandle		void pointer passed through to callback.
	 * @param theOutputHandler		a user defined callback function.
	 * @param theFlushHandler		An optional user-defined callback function.
	 * @return	0 for success 
	 */
	int
	transform(
			const XalanParsedSource&		theParsedSource,
			const XalanCompiledStylesheet*	theCompiledStylesheet,
			void*							theOutputHandle,
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
	 * @param theOutputHandler		a user defined callback function.
	 * @param theFlushHandler		an optional user-defined callback function.
	 * @return	0 for success 
	 */
	int
	transform(
			const XSLTInputSource&		theInputSource,
			void*						theOutputHandle,
			XalanOutputHandlerType		theOutputHandler,
			XalanFlushHandlerType		theFlushHandler = 0);

	/**
	 * Creates a compeled stylesheet.  The input source can be 
	 * a file name, a stream or a root node.   The XalanTransformer
	 * instance owns the XalanCompiledStylesheet instance and will
	 * delete it when the XalanTransformer instance goes out of scope,
	 * or you explicitly call destroyStylesheet().	You must not delete
	 * the instance yourself.
	 *
	 * @param theStylesheetSource input source
	 * @param theCompiledStylesheet a reference to a pointer to a XalanCompileStylesheet.
	 * @return 0 for success 
	 */
	int
	compileStylesheet(
			const XSLTInputSource&				theStylesheetSource,
			const XalanCompiledStylesheet*& 	theCompiledStylesheet);

	/**
	 * Destroy a XalanCompiledStylesheet instance created by a previous
	 * call to compileStylesheet().
	 *
	 * @param theStylesheet The instance to destroy.
	 * @return 0 for success 
	 */
	int
	destroyStylesheet(const XalanCompiledStylesheet*	theStylesheet);

	/**
	 * Parse a source XML document.  The input source can be 
	 * a file name, a stream or a root node.  The XalanTransformer
	 * instance owns the XalanParsedSource instance and will
	 * delete it when the XalanTransformer instance goes out of scope,
	 * or you explicitly call destroyParsedSource().  You must not
	 * delete the instance yourself.
	 *
	 * @param theInputSource input source
	 * @param theParsedSource a reference to a pointer to a XalanParsedSource.
	 * @param useXercesDOM input use default or xerces dom source tree
	 * @return 0 for success 
	 */
	int
	parseSource(
			const XSLTInputSource&		theInputSource,
			const XalanParsedSource*&	theParsedSource,
			bool						useXercesDOM = false);

	/**
	 * Destroy a parsed source created by a previous call to parseSource().
	 *
	 * @param theParsedSource The XalanParsedSource instance to destroy.
	 * @return 0 for success 
	 */
	int
	destroyParsedSource(const XalanParsedSource*	theParsedSource);

	/**
	 * Create a document builder.  Using the document builder, you
	 * can construct a document using SAX2 interfaces.	The XalanTransformer
	 * instance owns the document builder and will delete it when the
	 * XalanTransformer instance goes out of scope, or you explicitly call
	 * destroyDocumentBuilder().  You must not delete the instance yourself.
	 *
	 * @return	a pointer to a XalanDocumentBuilder instance or 0 for failure.
	 */
	XalanDocumentBuilder*
	createDocumentBuilder();

	/**
	 * Destroy a document builder created by a previous call to createDocumentBuilder().
	 * Passing a pointer that is not created by a call to createDocumentBuilder() can
	 * result in undefined behavior.
	 *
	 * @param theDocumentBuilder The document builder to destroy.
	 */
	void
	destroyDocumentBuilder(XalanDocumentBuilder*	theDocumentBuilder);

	/**
	 * Install an external function in the local space.
	 *
	 * @param theNamespace The namespace for the functionl
	 * @param functionName The name of the function.
	 * @param function The function to install.
	 */
	void
	installExternalFunction(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName,
			const Function& 		function);

	/**
	 * Install an external function in the global space.
	 *
	 * @param theNamespace The namespace for the functionl
	 * @param functionName The name of the function.
	 * @param function The function to install.
	 */
	static void
	installExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName,
			const Function& 		function);

	/**
	 * Uninstall an external local function.
	 *
	 * @param theNamespace The namespace for the function
	 * @param functionName The name of the function.
	 */
	void
	uninstallExternalFunction(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName);

	/**
	 * Uninstall an external global function.
	 *
	 * @param theNamespace The namespace for the function
	 * @param functionName The name of the function.
	 */
	static void
	uninstallExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName);

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

	/**
	 * Set a top-level stylesheet parameter.  This value can be evaluated via
	 * xsl:param-variable.
	 *
	 * @param key name of the param
	 * @param expression expression that will be evaluated
	 */
	void
	setStylesheetParam(
			const char*		key,
			const char*		expression);

	/**
	 * Set a flag to indicate whether or not the source file(s) for the
	 * transformation will be validated.
	 *
	 * @param fValue the new value for the flag.
	 */
	bool
	getUseValidation() const
	{
		return m_useValidation;
	}

	/**
	 * Set a flag to indicate whether or not the source file(s) for the
	 * transformation will be validated.
	 *
	 * @param fValue the new value for the flag.
	 */
	void
	setUseValidation(bool	fValue)
	{
		m_useValidation = fValue;
	}

	/**
	 * Returns the last error that occurred as a 
	 * result of calling transform. 
	 *
	 * @return	error message const character pointer.
	 */
	const char*
	getLastError() const;

	/**
	 * Get the current number of spaces to indent.
	 * 
	 * @return number of spaces
	 */
	int
	getIndent() const;

	/**
	 * Set the current number of spaces to indent.
	 * 
	 * @param indentAmount The number of spaces to indent.  Use -1 for the default amount.
	 */
	void
	setIndent(int	indentAmount);

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<const XalanCompiledStylesheet*>		CompiledStylesheetPtrVectorType;
	typedef vector<const XalanParsedSource*>			ParsedSourcePtrVectorType;
	typedef pair<XalanDOMString, XalanDOMString>		ParamPairType;
	typedef vector<ParamPairType>						ParamPairVectorType;
	typedef pair<XalanQNameByValue, Function*>			FunctionPairType;
	typedef vector<FunctionPairType>					FunctionParamPairVectorType;
#else
	typedef std::vector<const XalanCompiledStylesheet*> CompiledStylesheetPtrVectorType;
	typedef std::vector<const XalanParsedSource*>		ParsedSourcePtrVectorType;
	typedef std::pair<XalanDOMString, XalanDOMString>	ParamPairType;
	typedef std::vector<ParamPairType>					ParamPairVectorType;
	typedef std::pair<XalanQNameByValue, Function*>		FunctionPairType;
	typedef std::vector<FunctionPairType>				FunctionParamPairVectorType;
#endif

	class EnsureDestroyParsedSource
	{
	public:

		EnsureDestroyParsedSource(
				XalanTransformer&			theTransformer,
				const XalanParsedSource*	theParsedSource) :
			m_transformer(theTransformer),
			m_parsedSource(theParsedSource)
		{
		}

		~EnsureDestroyParsedSource()
		{
			m_transformer.destroyParsedSource(m_parsedSource);
		}

	private:

		XalanTransformer&				m_transformer;

		const XalanParsedSource* const	m_parsedSource;
	};

	struct EnsureDestroyCompiledStylesheet
	{
		EnsureDestroyCompiledStylesheet(
				XalanTransformer&				theTransformer,
				const XalanCompiledStylesheet*	theCompiledStylesheet) :
			m_transformer(theTransformer),
			m_compiledStylesheet(theCompiledStylesheet)
		{
		}

		~EnsureDestroyCompiledStylesheet()
		{
			m_transformer.destroyStylesheet(m_compiledStylesheet);
		}

	private:

		XalanTransformer&						m_transformer;

		const XalanCompiledStylesheet* const	m_compiledStylesheet;
	};

	struct EnsureDestroyDocumentBuilder
	{
		EnsureDestroyDocumentBuilder(
				XalanTransformer&		theTransformer,
				XalanDocumentBuilder*	theDocumentBuilder) :
			m_transformer(theTransformer),
			m_documentBuilder(theDocumentBuilder)
		{
		}

		~EnsureDestroyDocumentBuilder()
		{
			m_transformer.destroyDocumentBuilder(m_documentBuilder);
		}

	private:

		XalanTransformer&			m_transformer;

		XalanDocumentBuilder* const m_documentBuilder;
	};

protected:

private:

	void
	reset();

	class EnsureReset
	{
	public:

		EnsureReset(XalanTransformer&	theTransformer) :
			m_transformer(theTransformer)
		{
		}

		~EnsureReset();

	private:

		XalanTransformer&	m_transformer;
	};

	friend class EnsureReset;

	CompiledStylesheetPtrVectorType 		m_compiledStylesheets;

	ParsedSourcePtrVectorType				m_parsedSources;

	ParamPairVectorType 					m_paramPairs;

	FunctionParamPairVectorType 			m_functionPairs;

	CharVectorType							m_errorMessage;

	bool									m_useValidation;

	// This should always be the latest data member!!!
	StylesheetExecutionContextDefault*		m_stylesheetExecutionContext;

	static const XSLTInit*					s_xsltInit;
};



#endif	// XALANTRANSFORMER_HEADER_GUARD
