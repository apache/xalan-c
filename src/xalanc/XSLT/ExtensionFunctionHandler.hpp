/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
 * $ Id: $
 *
 */

#if !defined(XALAN_EXTENSIONFUNCTIONHANDLER_HEADER_GUARD)
#define XALAN_EXTENSIONFUNCTIONHANDLER_HEADER_GUARD



// Base include file.	Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <set>
#include <vector>



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
	ExtensionFunctionHandler(const XalanDOMString&	namespaceUri);

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
			const XalanDOMString&	namespaceUri,
			const XalanDOMString&	funcNames,
			const XalanDOMString&	lang,
			const XalanDOMString&	srcURL,
			const XalanDOMString&	scriptSrc);


	virtual
	~ExtensionFunctionHandler();

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
#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<void*>								ArgVectorType;
	typedef set<XalanDOMString, less<XalanDOMString> >	StringSetType;
#else
	typedef std::vector<void*>			ArgVectorType;
	typedef std::set<XalanDOMString>	StringSetType;
#endif

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
