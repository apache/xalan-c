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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
#if !defined(XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680)
#define XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <vector>



#include <XPath/XPathEnvSupport.hpp>



/**
 * Dummy class in order to make the XPath object happy 
 * for diagnostic purposes.
 */
class XALAN_XPATH_EXPORT XPathEnvSupportDefault : public XPathEnvSupport
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMString, XalanDocument*, less<XalanDOMString> >		SourceDocsTableType;
	typedef map<XalanDOMString, const Function*, less<XalanDOMString> >		FunctionTableType;
	typedef map<XalanDOMString, FunctionTableType, less<XalanDOMString> >	NamespaceFunctionTablesType;
#else
	typedef std::map<XalanDOMString, XalanDocument*>	SourceDocsTableType;
	typedef std::map<XalanDOMString, const Function*>	FunctionTableType;
	typedef std::map<XalanDOMString, FunctionTableType>	NamespaceFunctionTablesType;
#endif

	/**
	 * Perform initialization of statics -- must be called before any
	 * processing occurs.  See class XPathInit.
	 */
	static void
	initialize();

	/**
	 * Perform termination of statics.  See class XPathInit.
	 */
	static void
	terminate();


	XPathEnvSupportDefault();

	virtual
	~XPathEnvSupportDefault();


	// Interfaces to install and uninstall external functions globally.
	// These calls are not thread-safe, and should happen during
	// processing.

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
			const Function&			function);

	/**
	 * Uninstall an external function from the global space.
	 *
	 * @param theNamespace The namespace for the functionl
	 * @param functionName The name of the function.
	 */
	static void
	uninstallExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName);

	// Interfaces to install and uninstall external functions in this instance.

	/**
	 * Install an external function in the local space.
	 *
	 * @param theNamespace The namespace for the functionl
	 * @param functionName The name of the function.
	 * @param function The function to install.
	 */
	virtual void
	installExternalFunctionLocal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName,
			const Function&			function);

	/**
	 * Uninstall an external function from the local space.
	 *
	 * @param theNamespace The namespace for the functionl
	 * @param functionName The name of the function.
	 */
	virtual void
	uninstallExternalFunctionLocal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName);


	// These interfaces are inherited from XPathEnvSupport...

	virtual XalanDocument*
	parseXML(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base);

	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&	theURI) const;

	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument);

	virtual XalanDOMString
	findURIFromDoc(const XalanDocument*		owner) const;

	virtual bool
	elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const;

	virtual bool
	functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const;

	virtual XObjectPtr
	extFunction(
			XPathExecutionContext&			executionContext,
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName, 
			XalanNode*						context,
			const XObjectArgVectorType&		argVec,
			const Locator*					locator) const;

	virtual bool
	problem(
			eSource					where,
			eClassification			classification,
			const XalanNode*		styleNode,
			const XalanNode*		sourceNode,
			const XalanDOMString&	msg,
			const XalanDOMChar*		uri,
			int						lineNo,
			int						charOffset) const;

	virtual bool
	problem(
			eSource					where,
			eClassification			classification,
			const PrefixResolver*	resolver,
			const XalanNode*		sourceNode,
			const XalanDOMString&	msg,
			const XalanDOMChar*		uri,
			int						lineNo,
			int						charOffset) const;

	// These interfaces are inherited from Resettable...

	virtual void
	reset();

	// Delete functor for table cleanup...
	struct NamespaceFunctionTableDeleteFunctor
	{
		typedef FunctionTableType				FunctionTableInnerType;
		typedef NamespaceFunctionTablesType		NamespaceFunctionTablesInnerType;

		/**
		 * Delete the value object in a map value pair.  The value of the pair must
		 * be of pointer type.
		 *
		 * @param thePair key-value pair
		 */
		void
		operator()(const NamespaceFunctionTablesInnerType::value_type&	thePair) const;
	};

protected:

	/**
	 * Find an external function.
	 *
	 * @param theNamespace The namespace for the function.
	 * @param functionName The name of the function.
	 * @return a pointer to the function if found, or 0 if not found.
	 */
	const Function*
	findFunction(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName) const;

private:

	// These are not implemented...
	XPathEnvSupportDefault(const XPathEnvSupportDefault&);

	XPathEnvSupportDefault&
	operator=(const XPathEnvSupportDefault&);

	bool
	operator==(const XPathEnvSupportDefault&) const;

	/**
	 * Update the supplied function table.  If the parameter
	 * function is 0, and a function with the supplied
	 * namespace and name exists in the table, it will be
	 * removed.  If function is not 0, and a function with
	 * the supplied namespace and name exists in the table,
	 * it will be replaced with the new function.  Otherwise,
	 * the function will be added.
	 *
	 * @param theNamespace The namespace for the functionl
	 * @param functionName The name of the function.
	 * @param function The function to install.
	 */
	static void
	updateFunctionTable(
			NamespaceFunctionTablesType&	theTable,
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			const Function*					function);

	/**
	 * Find an external function in the supplied table.
	 *
	 * @param theTable The table to search.
	 * @param theNamespace The namespace for the function.
	 * @param functionName The name of the function.
	 * @return a pointer to the function if found, or 0 if not found.
	 */
	const Function*
	findFunction(
			const NamespaceFunctionTablesType&	theTable,
			const XalanDOMString&				theNamespace,
			const XalanDOMString&				functionName) const;

	// Data members...

	SourceDocsTableType						m_sourceDocs;

	NamespaceFunctionTablesType				m_externalFunctions;

	static 	NamespaceFunctionTablesType		s_externalFunctions;
};



#endif	// XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680
