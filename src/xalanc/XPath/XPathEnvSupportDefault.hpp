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
#if !defined(XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680)
#define XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>
#include <xalanc/XPath/XPathEnvSupport.hpp>


#include<xalanc/PlatformSupport/DOMStringHelper.hpp>


#include<xalanc/Include/XalanMap.hpp>


XALAN_CPP_NAMESPACE_BEGIN

typedef XalanMap<XalanDOMString, const Function*>   FunctionTableTypeDefinition;
XALAN_USES_MEMORY_MANAGER(FunctionTableTypeDefinition)


/**
 * Dummy class in order to make the XPath object happy 
 * for diagnostic purposes.
 * @author <a href="mailto:david_n_bertoni@us.ibm.com">David N. Bertoni</a>
 */
class XALAN_XPATH_EXPORT XPathEnvSupportDefault : public XPathEnvSupport
{
public:

	typedef XalanMap<XalanDOMString, XalanDocument*>	SourceDocsTableType;
	typedef FunctionTableTypeDefinition                         FunctionTableType;
	typedef XalanMap<XalanDOMString, FunctionTableType>	NamespaceFunctionTablesType;

	/**
	 * Perform initialization of statics -- must be called before any
	 * processing occurs.  See class XPathInit.
	 */
	static void
	initialize(MemoryManagerType&  theManager);

	/**
	 * Perform termination of statics.  See class XPathInit.
	 */
	static void
	terminate();


	XPathEnvSupportDefault(MemoryManagerType&  theManager XALAN_DEFAULT_MEMMGR);

	virtual
	~XPathEnvSupportDefault();

    MemoryManagerType& 
    getMemoryManager()
    {
        return m_sourceDocs.getMemoryManager();
    }

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
            MemoryManagerType&      theManager,
			const XalanDOMString&	urlString,
			const XalanDOMString&	base);

	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&	theURI) const;

	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument);

	virtual const XalanDOMString&
    findURIFromDoc(const XalanDocument*     owner) const;

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
			const LocatorType*				locator) const;

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

        NamespaceFunctionTableDeleteFunctor(MemoryManagerType& theManager);
		/**
		 * Delete the value object in a map value pair.  The value of the pair must
		 * be of pointer type.
		 *
		 * @param thePair key-value pair
		 */
		void
		operator()(const NamespaceFunctionTablesInnerType::value_type&	thePair) const;
    private:
        MemoryManagerType& m_memMgr;
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

	static NamespaceFunctionTablesType		s_externalFunctions;

    static const XalanDOMString             s_emptyString;
};

XALAN_CPP_NAMESPACE_END



#endif	// XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680
