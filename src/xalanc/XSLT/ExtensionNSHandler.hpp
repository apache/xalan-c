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
#if !defined(XALAN_EXTENSIONNSHANDLER_HEADER_GUARD)
#define XALAN_EXTENSIONNSHANDLER_HEADER_GUARD


// Base include file.	Must be first.
#include "XSLTDefinitions.hpp"



// Base class
#include "ExtensionFunctionHandler.hpp"



#include <xalanc/Include/XalanSet.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class Stylesheet;
class StylesheetExecutionContext;
class XalanElement;
class XalanNode;
class XalanQName;



class XALAN_XSLT_EXPORT ExtensionNSHandler : public ExtensionFunctionHandler
{	 
public:	
	/**
	 * Construct a new extension namespace handler for a given extension NS.
	 * This doesn't do anything - just hang on to the namespace URI.
	 * 
	 * @param namespaceUri extension namespace URI being implemented
	 */
	ExtensionNSHandler(const XalanDOMString&	namespaceUri,
                        MemoryManagerType& theManager);

	static ExtensionNSHandler*
    create(const XalanDOMString&	namespaceUri,
                        MemoryManagerType& theManager);
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
            MemoryManagerType&      theManager,
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
	isElementAvailable (const XalanDOMString&	element) const;

	/**
	 * Process a call to this extension namespace via an element. As a side
	 * effect, the results are sent to the XSLTProcessor's result tree.
	 *
	 * @param localPart      element name's local part
	 * @param element        extension element being processed
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
			StylesheetExecutionContext&		executionContext,
			const XalanDOMString&			localPart,
			const XalanElement*				element,
			Stylesheet&						stylesheetTree, 
			const XalanNode*				sourceTree,
			const XalanNode*				sourceNode,
			const XalanQName&				mode);

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

	typedef XalanSet<XalanDOMString>	ExtensionSetType;

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
	static XalanDOMString&
	getScriptString(const XalanElement&		elem, XalanDOMString& theResult);
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_EXTENSIONNSHANDLER_HEADER_GUARD
