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
 * $ Id: $
 *
 */

#include "ExtensionFunctionHandler.hpp"



#include <cassert>



#include <PlatformSupport/StringTokenizer.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>



/**
 * Class handling an extension namespace for XPath. Provides functions
 * to test a function's existence and call a function
 *
 * @author Sanjiva Weerawarana (sanjiva@watson.ibm.com)
 */

ExtensionFunctionHandler::ExtensionFunctionHandler(const DOMString& namespaceUri) :
	m_namespaceUri(namespaceUri),
	m_scriptLang(),
	m_scriptSrc(),
	m_scriptSrcURL(),
	m_javaObject(0),
	m_functions(),
	m_componentStarted(false)
{
}



ExtensionFunctionHandler::~ExtensionFunctionHandler()
{
}



/////////////////////////////////////////////////////////////////////////

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
ExtensionFunctionHandler::ExtensionFunctionHandler (
									const DOMString& namespaceUri,
									const DOMString& funcNames,
									const DOMString& lang,
									const DOMString& srcURL,
									const DOMString& src) :
	m_namespaceUri(namespaceUri),
	m_scriptLang(lang),
	m_scriptSrc(src),
	m_scriptSrcURL(srcURL),
	m_javaObject(0),
	m_functions(),
	m_componentStarted(false)
{
	setFunctions (funcNames);
}

/////////////////////////////////////////////////////////////////////////
// Main API
/////////////////////////////////////////////////////////////////////////

/**
 * Set function local parts of extension NS.
 *
 * @param functions whitespace separated list of function names defined
 *        by this extension namespace.
 */
void ExtensionFunctionHandler::setFunctions (const DOMString& funcNames) 
{
	if (isEmpty(funcNames)) 
	{
		return;
	}
	StringTokenizer st(funcNames, " \t\n\r", false);
	while (st.hasMoreTokens ()) 
	{
		DOMString tok = st.nextToken();
		m_functions.insert(tok);
	}
}

/////////////////////////////////////////////////////////////////////////

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
void ExtensionFunctionHandler::setScript (const DOMString& lang,
							const DOMString& srcURL,
							const DOMString& scriptSrc) 
{
	m_scriptLang = lang;
	m_scriptSrcURL = srcURL;
	m_scriptSrc = scriptSrc;
}

/////////////////////////////////////////////////////////////////////////

/**
 * Tests whether a certain function name is known within this namespace.
 *
 * @param function name of the function being tested
 *
 * @return true if its known, false if not.
 */
bool ExtensionFunctionHandler::isFunctionAvailable (const DOMString& function) 
{
	return m_functions.find(function) != m_functions.end();
}

/////////////////////////////////////////////////////////////////////////

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
XObject* ExtensionFunctionHandler::callFunction(const DOMString& funcName, const ArgVector& args)
{
	assert(0);	// @@ TODO: Not implemented
	if (!m_componentStarted) 
	{
		startupComponent ();
	}

	// System.out.println("Extensions not implemented!");
	return 0;
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

void ExtensionFunctionHandler::startupComponent()
{
	// special case the javaclass engine - the scriptSrcURL is 
	// the class name to run. If it starts with class: then use the
	// class object with that name instead of init'ing it as the
	// target of the calls later
	if (m_scriptLang.equals ("javaclass")) 
	{
		try 
		{
			DOMString cname = m_scriptSrcURL;
			bool isClass = false;
			if (startsWith (m_scriptSrcURL, "class:")) 
			{
				cname = substring (m_scriptSrcURL, 6);
				isClass = true;
			}
			// @@ JMD: Can't do this in C++
			/*
			Class cl = Class.forName (cname);
			if (isClass) 
			{
				m_javaObject = cl;
			}
			else
			{
				m_javaObject = cl.newInstance ();
			}
			*/
			m_componentStarted = true;
			return;
		}
		//catch (Exception e) 
		catch (...) 
		{
			assert(0);
			// System.out.println("Extension error: "+e.getMessage ());
			// throw new XPathProcessorException (e.getMessage (), e);
		}
	}

	// if scriptSrcURL is specified read it off
	if (!isEmpty(m_scriptSrcURL)) 
	{
		assert(0);
		// throw new XPathProcessorException ("src attr not supported (yet)");
	}

	if (isEmpty(m_scriptSrc)) 
	{
		return;
	}

	// evaluate the src to load whatever content is in that string to 
	// the engines
	assert(0);
	// System.out.println("Error!	Extensions not implemented!");
	/*
	try 
	{
		mgr.exec (m_scriptLang, "XSLTScript", -1, -1, scriptSrc);
	}
	catch (BSFException bsfe) 
	{
		throw new XPathProcessorException (bsfe.getMessage (), bsfe);
	}
	*/
	m_componentStarted = true;
}
