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
// $ Id: $
// Class header file.
#include "ExtensionNSHandler.hpp"
#include "ExtensionFunctionHandler.hpp"

class XSLTEngineImpl;
class Stylesheet;
class DOM_Node;
class DOM_Node;
class QName;

struct XSLProcessorContext
{
  const XSLTEngineImpl& m_processor;
  const Stylesheet& m_stylesheetTree;
  const DOM_Node& m_sourceTree;
  const DOM_Node& m_sourceNode;
  const QName& m_mode;

  XSLProcessorContext(
		  const XSLTEngineImpl& processor,
		  const Stylesheet& stylesheetTree, 
		  const DOM_Node& sourceTree,
		  const DOM_Node& sourceNode,
		  const QName& mode) :
    m_processor(processor),
    m_stylesheetTree(stylesheetTree),
    m_mode(mode),
    m_sourceTree(sourceTree),
    m_sourceNode(sourceNode)
   {}
};

/**
 * Construct a new extension namespace handler for a given extension NS.
 * This doesn't do anything - just hang on to the namespace URI.
 * 
 * @param xslp				 handle to the XSL processor that I'm working for
 * @param namespaceUri the extension namespace URI that I'm implementing
 */
ExtensionNSHandler::ExtensionNSHandler(XSLTEngineImpl& xslp,
							const DOMString& namespaceUri) :
	ExtensionFunctionHandler(namespaceUri),
	m_XSLProcessor(xslp),
	m_elements(),
	m_componentDescLoaded(false)
{
}

/**
 * Construct a new extension namespace handler given all the information
 * needed. 
 * 
 * @param xslp				 handle to the XSL processor that I'm working for
 * @param namespaceUri the extension namespace URI that I'm implementing
 * @param elemNames		string containing list of elements of extension NS
 * @param funcNames		string containing list of functions of extension NS
 * @param lang				 language of code implementing the extension
 * @param srcURL			 value of src attribute (if any) - treated as a URL
 *										 or a classname depending on the value of lang. If
 *										 srcURL is not null, then scriptSrc is ignored.
 * @param scriptSrc		the actual script code (if any)
 */
ExtensionNSHandler::ExtensionNSHandler (
	XSLTEngineImpl& xslp,
	const DOMString& namespaceUri,
	const DOMString& elemNames,
	const DOMString& funcNames,
	const DOMString& lang,
	const DOMString& srcURL,
	const DOMString& src) :
		ExtensionFunctionHandler (namespaceUri, funcNames, lang, srcURL, src),
	m_XSLProcessor(xslp),
	m_elements(),
	m_componentDescLoaded(true)
		 
{
	setElements (elemNames);
}

/////////////////////////////////////////////////////////////////////////
// Main API
/////////////////////////////////////////////////////////////////////////

/*
* Set function local parts of extension NS. Super does the work; I
* just record that a component desc has been loaded.
*
* @param functions whitespace separated list of function names defined
*				by this extension namespace.
*/
void ExtensionNSHandler::setFunctions (const DOMString& funcNames)
{
    ExtensionFunctionHandler::setFunctions (funcNames);
    m_componentDescLoaded = true;
}

/////////////////////////////////////////////////////////////////////////

/**
 * Set the script data for this extension NS. Deferred to super for
 * actual work - I only record that a component desc has been loaded.
 *
 * @param lang			language of the script.
 * @param srcURL		value of src attribute (if any) - treated as a URL
 *									or a classname depending on the value of lang. If
 *									srcURL is not null, then scriptSrc is ignored.
 * @param scriptSrc the actual script code (if any)
 */
void ExtensionNSHandler::setScript (const DOMString& lang, const DOMString& srcURL, const DOMString& scriptSrc)
{
    ExtensionFunctionHandler::setScript (lang, srcURL, scriptSrc);
    m_componentDescLoaded = true;
}

/////////////////////////////////////////////////////////////////////////

/**
 * Set element local parts of extension NS.
 *
 * @param elemNames whitespace separated list of element names defined
 *				by this extension namespace.
 */
void ExtensionNSHandler::setElements (const DOMString& elemNames)
{
    if (elemNames.length() == 0) 
      return;
    StringTokenizer st(elemNames, " \t\n\r", false);
    while (st.hasMoreTokens()) 
    {
      DOMString tok = st.nextToken();
      m_elements.insert(tok); // just stick it in there basically
    }
    m_componentDescLoaded = true;
}

/////////////////////////////////////////////////////////////////////////

/**
 * Tests whether a certain element name is known within this namespace.
 * @param element name of the element being tested
 * @return true if its known, false if not.
 */
bool ExtensionNSHandler::isElementAvailable (const DOMString& element)
{
    return (m_elements.find(element) != m_elements.end());
}

/////////////////////////////////////////////////////////////////////////

/**
 * Process a call to this extension namespace via an element. As a side
 * effect, the results are sent to the XSLTProcessor's result tree.
 *
 * @param localPart			Element name's local part.
 * @param element				The extension element being processed.
 * @param processor			Handle to XSLTProcessor.
 * @param stylesheetTree The compiled stylesheet tree.
 * @param mode					 The current mode.
 * @param sourceTree		 The root of the source tree (but don't assume
 *											 it's a Document).
 * @param sourceNode		 The current context node.
 *
 * @exception XSLProcessorException thrown if something goes wrong 
 *						while running the extension handler.
 * @exception MalformedURLException if loading trouble
 * @exception FileNotFoundException if loading trouble
 * @exception IOException					 if loading trouble
 * @exception SAXException					if parsing trouble
 */
void ExtensionNSHandler::processElement (
	const DOMString& localPart,
	const DOM_Element& element,
	XSLTEngineImpl& processor, 
	Stylesheet& stylesheetTree, 
	const DOM_Node& sourceTree,
	const DOM_Node& sourceNode,
	const QName& mode)
	{
		XObject* result = 0;
		if (!m_componentStarted) 
		{
			try 
			{
				startupComponent();
				ExtensionFunctionHandler::ArgVector argv;
				XSLProcessorContext xpc(processor,
					stylesheetTree, sourceTree, sourceNode, mode);
			/*
				java:
				Vector argv = new Vector (2);
				argv.addElement (xpc);
				argv.addElement (element);
			*/
				// $$$ ToDo: There's no way this will ever work...
//				argv.push_back(static_cast<void *>(&xpc));
//				argv.push_back(static_cast<void *>(&element));
				result = ExtensionFunctionHandler::callFunction (localPart, argv);
			}
			// catch (XPathProcessorException e) 
			catch (...) 
			{
			/*
				e.printStackTrace ();
				throw new XSLProcessorException (e.getMessage (), e);
			*/
			//@@ TODO: Error reporting, or throw
			} 
		}
		if (result != 0) 
		{
			processor.outputToResultTree (stylesheetTree, result);
		}
	}

/////////////////////////////////////////////////////////////////////////
// Private/Protected Functions
/////////////////////////////////////////////////////////////////////////

/**
 * Start the component up by executing any script that needs to run
 * at startup time. This needs to happen before any functions can be
 * called on the component.
 * 
 * @exception XPathProcessorException if something bad happens.
 */
void ExtensionNSHandler::startupComponent()
{
	if (!m_componentDescLoaded) 
	{
		try 
		{
			loadComponentDescription ();
		}
		catch (...) 
		// catch (Exception e) 
		{
			// throw new XPathProcessorException (e.getMessage (), e);
			//@@ TODO: Error reporting, or throw
		}
	}
	ExtensionFunctionHandler::startupComponent ();
}

/////////////////////////////////////////////////////////////////////////

/**
 * Load the component spec for this extension namespace taking the URI
 * of this namespace as the URL to read from.
 * 
 * @exception XSLProcessorException if something bad happens.
 * @exception MalformedURLException if loading trouble
 * @exception FileNotFoundException if loading trouble
 * @exception IOException					 if loading trouble
 * @exception SAXException					if parsing trouble
 */
void ExtensionNSHandler::loadComponentDescription ()
{
	// First try treaing the URI of the extension as a fully qualified
	// class name; if it works then go with treating this an extension
	// implemented in "javaclass" for with that class being the srcURL.
	// forget about setting elements and functions in that case - so if
	// u do extension-{element,function}-available then u get false, 
	// but that's ok.
	assert(0);		// Don't try this in C++
	// @@ How to do this in C++ ??
/*
	try {
		String cname = namespaceUri.startsWith ("class:") ?
										 namespaceUri.substring (6) : namespaceUri;
		Class.forName (cname); // does it load?
		setScript ("javaclass", namespaceUri, null);
		m_componentDescLoaded = true;
		return;
	} catch (Exception e) {
		// oops, it failed .. ok, so this path ain't gonna pan out. shucks.
	}

	// parse the document at the URI of the extension, if any
	URL url = m_XSLProcessor.getURLFromString(namespaceUri,
																	m_XSLProcessor.m_stylesheetRoot.m_baseIdent);
	
	m_XSLProcessor.m_parserLiaison.parse(new InputSource(url.toString()));
	Document compSpec = m_XSLProcessor.m_parserLiaison.getDocument();
	Element componentElement = compSpec.getDocumentElement ();

	// determine the functions and elements of this component
	setElements (componentElement.getAttribute ("elements"));
	setFunctions (componentElement.getAttribute ("functions"));

	// is there an lxslt:script element child? [NOTE THAT THIS IS NOT
	// PROPER NAMESPACE-WISE .. I'll FIX IT LATER. .. Sanjiva 8/20/99.]
	NodeList nl = componentElement.getElementsByTagName ("lxslt:script");
	switch (nl.getLength ()) 
	{
	case 0:
		break;
	case 1:
		Element scriptElem = (Element) nl.item (0);
		String lang = scriptElem.getAttribute ("lang");
		Attr srcURLAttr = scriptElem.getAttributeNode ("src");
		String srcURL = (srcURLAttr == null) ? null : srcURLAttr.getValue ();
		String src = getScriptString (scriptElem);
		setScript (lang, srcURL, src);
		break;
	default:
		throw new XSLProcessorException ("too many <script>s in component");
	}
	m_componentDescLoaded = true;
*/
}
/////////////////////////////////////////////////////////////////////////

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
DOMString ExtensionNSHandler::getScriptString (const DOM_Element& elem)
{
	DOMString strBuf;
	for (DOM_Node n = elem.getFirstChild (); n != 0; n = n.getNextSibling ()) 
	{
		switch (n.getNodeType()) 
		{
		case DOM_Node::TEXT_NODE:
		case DOM_Node::CDATA_SECTION_NODE:
			strBuf += n.getNodeValue();
			break;
		default:
			break;
		}
	}
	return strBuf;
}


/*
 *	$ Log: $
 */

