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
 */
#if !defined(XALAN_EXTENSIONNSHANDLER_HEADER_GUARD)
#define XALAN_EXTENSIONNSHANDLER_HEADER_GUARD


// Base include file.	Must be first.
#include "XSLTDefinitions.hpp"



// Base class
#include "ExtensionFunctionHandler.hpp"



#include <set>



#include <XalanDOM/XalanDOMString.hpp>



class QName;
class Stylesheet;
class XalanElement;
class XalanNode;
class XSLTProcessor;



class XALAN_XSLT_EXPORT ExtensionNSHandler : public ExtensionFunctionHandler
{	 
public:	
	/**
	 * Construct a new extension namespace handler for a given extension NS.
	 * This doesn't do anything - just hang on to the namespace URI.
	 * 
	 * @param namespaceUri extension namespace URI being implemented
	 */
	ExtensionNSHandler(const XalanDOMString&	namespaceUri);

	/**
	 * Construct a new extension namespace handler given all the information
	 * needed. 
	 * 
	 * @param namespaceUri extension namespace URI being implemented
	 * @param elemNames    string containing list of elements of extension NS
	 * @param funcNames    string containing list of functions of extension NS
	 * @param lang         language of code implementing the extension
	 * @param srcURL       value of src attribute (if any) - treated as a URL
	 *                     or a classname depending on the value of lang. If
	 *                     srcURL is not null, then scriptSrc is ignored.
	 * @param scriptSrc    the actual script code (if any)
	 */
	ExtensionNSHandler(
			const XalanDOMString&	namespaceUri,
			const XalanDOMString&	elemNames,
			const XalanDOMString&	funcNames,
			const XalanDOMString&	lang,
			const XalanDOMString&	srcURL,
			const XalanDOMString&	src);

	/**
	 * Set function local parts of extension NS. Super does the work; I
	 * just record that a component desc has been loaded.
	 *
	 * @param functions whitespace separated list of function names defined
	 *                  by this extension namespace
	*/
	virtual void
	setFunctions(const XalanDOMString&	funcNames); 

	/**
	 * Set the script data for this extension NS. Deferred to super for
	 * actual work - only record that a component desc has been loaded.
	 *
	 * @param lang      language of the script
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
	 * Set element local parts of extension NS.
	 *
	 * @param elemNames whitespace separated list of element names defined
	 *                  by this extension namespace
	 */
	void
	setElements(const XalanDOMString&	elemNames); 
	

	/**
	 * Tests whether a certain element name is known within this namespace.
	 *
	 * @param element name of the element being tested
	 * @return true if known, false if not
	 */
	bool
	isElementAvailable (const XalanDOMString&	element); 


	/**
	 * Process a call to this extension namespace via an element. As a side
	 * effect, the results are sent to the XSLTProcessor's result tree.
	 *
	 * @param localPart      element name's local part
	 * @param element        extension element being processed
	 * @param processor      handle to XSLTProcessor
	 * @param stylesheetTree compiled stylesheet tree
	 * @param mode           current mode
	 * @param sourceTree     root of the source tree (but don't assume it's a
	 *                       document)
	 * @param sourceNode     current context node
	 * @exception XSLProcessorException thrown if something goes wrong 
	 *                                  while running the extension handler
	 * @exception MalformedURLException if loading trouble
	 * @exception FileNotFoundException if loading trouble
	 * @exception IOException           if loading trouble
	 * @exception SAXException          if parsing trouble
	 */
	void
	processElement (
			const XalanDOMString&	localPart,
			const XalanElement*		element,
			XSLTProcessor&			processor, 
			Stylesheet&				stylesheetTree, 
			const XalanNode*		sourceTree,
			const XalanNode*		sourceNode,
			const QName&			mode);

protected:

	/**
	 * Start the component up by executing any script that needs to run
	 * at startup time. This needs to happen before any functions can be
	 * called on the component.
	 * 
	 */
	virtual void
	startupComponent();


private:

#if defined(XALAN_NO_NAMESPACES)
	typedef set<XalanDOMString>			ExtensionSetType;
#else
	typedef std::set<XalanDOMString>	ExtensionSetType;
#endif

	// Extension elements of this namespace
	ExtensionSetType	m_elements;

	// True when info from the component description has been loaded. This gets
	// set as soon as any of the info has been specified. If this is false,
	// when processElement or processFunction is called it will use the
	// namespace URI as a URL and try to load that location as the component
	// description
	bool m_componentDescLoaded;

	/**
	 * Load the component spec for this extension namespace taking the URI
	 * of this namespace as the URL to read from.
	 */
	void
	loadComponentDescription();

	/**
	 * extract the text nodes and CDATA content children of the given
	 * elem and return as a string. Any other types of node children
	 * are ignored
	 *
	 * @param elem element whose text and cdata children are to be 
	 *				concatenated together.
	 *
	 * @return string resulting from concatanating the text/cdata child
	 *				 nodes' values.
	 */
	static XalanDOMString
	getScriptString(const XalanElement&		elem);
};



#endif	// XALAN_EXTENSIONNSHANDLER_HEADER_GUARD
