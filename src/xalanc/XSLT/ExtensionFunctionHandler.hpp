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

#if !defined(XALAN_EXTENSIONFUNCTIONHANDLER_HEADER_GUARD)
#define XALAN_EXTENSIONFUNCTIONHANDLER_HEADER_GUARD



// Base include file.	Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanSet.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/XPath/XObject.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XObjectPtr;



/**
 * Class handling an extension namespace for XPath. Provides functions
 * to test a function's existence and call a function
 *
 * @author Sanjiva Weerawarana (sanjiva@watson.ibm.com)
 */
class XALAN_XSLT_EXPORT ExtensionFunctionHandler 
{

public:

	/**
	 * Construct a new extension namespace handler for a given extension NS.
	 * This doesn't do anything - just hang on to the namespace URI.
	 * 
	 * @param namespaceUri the extension namespace URI that I'm implementing
	 */
	ExtensionFunctionHandler(const XalanDOMString&	namespaceUri,
                            MemoryManagerType& theManager);

	/**
	 * Construct a new extension namespace handler given all the information
	 * needed. 
	 * 
	 * @param namespaceUri the extension namespace URI that I'm implementing
	 * @param funcNames    string containing list of functions of extension NS
	 * @param lang         language of code implementing the extension
	 * @param srcURL       value of src attribute (if any) - treated as a URL
	 *                     or a classname depending on the value of lang. If
	 *                     srcURL is not null, then scriptSrc is ignored.
	 * @param scriptSrc    the actual script code (if any)
	 */
	ExtensionFunctionHandler(
            MemoryManagerType& theManager,
			const XalanDOMString&	namespaceUri,
			const XalanDOMString&	funcNames,
			const XalanDOMString&	lang,
			const XalanDOMString&	srcURL,
			const XalanDOMString&	scriptSrc);


	virtual
	~ExtensionFunctionHandler();

    MemoryManagerType&
    getMemoryManager()
    {
        return m_namespaceUri.getMemoryManager();
    }

	/**
	 * Set function local parts of extension NS.
	 *
	 * @param functions whitespace separated list of function names defined
	 *                  by this extension namespace.
	 */
	virtual void
	setFunctions(const XalanDOMString&	funcNames);

	/**
	 * Set the script data for this extension NS. If srcURL is !null then
	 * the script body is read from that URL. If not the scriptSrc is used
	 * as the src. This method does not actually execute anything - that's
	 * done when the component is first hit by the user by an element or 
	 * a function call.
	 *
	 * @param lang      language of the script.
	 * @param srcURL    value of src attribute (if any) - treated as a URL
	 *                  or a classname depending on the value of lang. If
	 *                  srcURL is not null, then scriptSrc is ignored.
	 * @param scriptSrc the actual script code (if any)
	 */
	virtual void
	setScript(
			const XalanDOMString&	lang,
			const XalanDOMString&	srcURL,
			const XalanDOMString&	scriptSrc);

	/**
	 * Tests whether a certain function name is known within this namespace.
	 *
	 * @param function name of the function being tested
	 * @return true if its known, false if not.
	 */
	virtual bool
	isFunctionAvailable(const XalanDOMString&	function) const;

	/// Vector of pointers to function arguments
	typedef XalanVector<void*>							ArgVectorType;

	typedef XalanSet<XalanDOMString>	StringSetType;

	/**
	 * Process a call to a function.
	 *
	 * @param funcName Function name.
	 * @param args     The arguments of the function call.
	 *
	 * @return the return value of the function evaluation.
	 *
	 * @exception XSLProcessorException thrown if something goes wrong 
	 *            while running the extension handler.
	 * @exception MalformedURLException if loading trouble
	 * @exception FileNotFoundException if loading trouble
	 * @exception IOException           if loading trouble
	 * @exception SAXException          if parsing trouble
	 */

	virtual XObjectPtr
	callFunction(
			const XalanDOMString&	funcName,
			const ArgVectorType&	args);

protected:

	XalanDOMString	m_namespaceUri;  // uri of the extension namespace
	XalanDOMString	m_scriptLang;    // scripting language of implementation
	XalanDOMString	m_scriptSrc;     // script source to run (if any)
	XalanDOMString	m_scriptSrcURL;  // URL of source of script (if any)

	void* m_javaObject;		    // object for javaclass engine

	StringSetType	m_functions; // functions of namespace

	//  BSFManager mgr = new BSFManager (); // mgr used to run scripts

	bool			m_componentStarted; // true when the scripts in a

	// component description (if any) have been run

	/**
	 * Start the component up by executing any script that needs to run
	 * at startup time. This needs to happen before any functions can be
	 * called on the component. 
	 * 
	 * @exception XPathProcessorException if something bad happens.
	 */
	virtual void
	startupComponent();

	static const XalanDOMChar	s_tokenDelimiterCharacters[];
};



XALAN_CPP_NAMESPACE_END
 


#endif	// XALAN_EXTENSIONFUNCTIONHANDLER_HEADER_GUARD
