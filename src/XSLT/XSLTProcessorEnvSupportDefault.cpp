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
#include "XSLTProcessorEnvSupportDefault.hpp"



#include <algorithm>



#include <util/XMLURL.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/STLHelper.hpp>
#include <PlatformSupport/URISupport.hpp>



#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XPath/XPathExecutionContext.hpp>



#include <XMLSupport/XMLParserLiaison.hpp>



#include "KeyTable.hpp"
#include "StylesheetRoot.hpp"
#include "XSLTProcessor.hpp"
#include "XSLTInputSource.hpp"



XSLTProcessorEnvSupportDefault::XSLTProcessorEnvSupportDefault(XSLTProcessor*	theProcessor) :
	XSLTProcessorEnvSupport(),
	m_defaultSupport(),
	m_processor(theProcessor),
	m_keyTables(),
	m_xlocatorTable()
{
}



XSLTProcessorEnvSupportDefault::~XSLTProcessorEnvSupportDefault()
{
	reset();
}



void
XSLTProcessorEnvSupportDefault::installExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	extensionName,
			const Function&			function)
{
	XPathEnvSupportDefault::installExternalFunctionGlobal(theNamespace, extensionName, function);
}



void
XSLTProcessorEnvSupportDefault::uninstallExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	extensionName)
{
	XPathEnvSupportDefault::uninstallExternalFunctionGlobal(theNamespace, extensionName);
}



void
XSLTProcessorEnvSupportDefault::installExternalFunctionLocal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	extensionName,
			const Function&			function)
{
	m_defaultSupport.installExternalFunctionLocal(theNamespace, extensionName, function);
}



void
XSLTProcessorEnvSupportDefault::uninstallExternalFunctionLocal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	extensionName)
{
	m_defaultSupport.uninstallExternalFunctionLocal(theNamespace, extensionName);
}



void
XSLTProcessorEnvSupportDefault::reset()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Clean up the key table vector
	for_each(m_keyTables.begin(),
			 m_keyTables.end(),
			 makeMapValueDeleteFunctor(m_keyTables));

	m_keyTables.clear();

	m_xlocatorTable.clear();
}



KeyTable*
XSLTProcessorEnvSupportDefault::getKeyTable(const XalanNode*	doc) const
{
	const KeyTablesTableType::const_iterator		i =
					m_keyTables.find(doc);

	if (i == m_keyTables.end())
	{
		return 0;
	}
	else
	{
		return i->second;
	}
}



void
XSLTProcessorEnvSupportDefault::setKeyTable(
			KeyTable*			keytable,
			const XalanNode*	doc)
{
	// Get rid of any existing keytable
	delete m_keyTables[doc];

	m_keyTables[doc] = keytable;
}



const NodeRefListBase*
XSLTProcessorEnvSupportDefault::getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const PrefixResolver&	resolver,
			XPathExecutionContext&	executionContext) const
{
	if (m_processor == 0)
	{
		return m_defaultSupport.getNodeSetByKey(doc,
											    name,
												ref,
												resolver,
												executionContext);
	}
	else
	{
		const NodeRefListBase*	nl = 0;

		const Stylesheet* const		theStylesheet =
			m_processor->getStylesheetRoot();

		if (theStylesheet != 0)
		{
			// $$$ ToDo: Figure out this const stuff!!!
			nl = theStylesheet->getNodeSetByKey(&const_cast<XalanNode&>(doc),
												name,
												ref,
												resolver,
												executionContext,
#if defined(XALAN_NO_MUTABLE)
												(KeysTableType&)m_keyTables);
#else
												m_keyTables);
#endif
		}

		if(0 == nl)
		{
			m_processor->error(XalanDOMString("There is no xsl:key declaration for '") + name + XalanDOMString("'!"));
		}

		return nl;
	}
}



XObject*
XSLTProcessorEnvSupportDefault::getVariable(
			XObjectFactory&		factory,
			const QName&		name) const
{
	if (m_processor == 0)
	{
		return m_defaultSupport.getVariable(factory,
											name);
	}
	else
	{
		return m_processor->getVariable(name);
	}
}



XalanDocument*
XSLTProcessorEnvSupportDefault::parseXML(
		const XalanDOMString&	urlString,
		const XalanDOMString&	base)
{
	if (m_processor == 0)
	{
		return m_defaultSupport.parseXML(urlString, base);
	}
	else
	{
		XMLParserLiaison&	parserLiaison =
			m_processor->getXMLParserLiaison();

		typedef URISupport::URLAutoPtrType	URLAutoPtrType;

		// $$$ ToDo: we should re-work this code to only use
		// XMLRUL when necessary.
		URLAutoPtrType	xslURL =
			URISupport::getURLFromString(urlString, base);

		const XMLCh* const	urlText = xslURL->getURLText();

		XSLTInputSource		inputSource(urlText);

		XalanDocument*		theDocument = 
			parserLiaison.parseXMLStream(inputSource);

		setSourceDocument(urlText, theDocument);

		return theDocument;
	}
}



XalanDocument*
XSLTProcessorEnvSupportDefault::getSourceDocument(const XalanDOMString&		theURI) const
{
	return m_defaultSupport.getSourceDocument(theURI);
}



void
XSLTProcessorEnvSupportDefault::setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument)
{
	m_defaultSupport.setSourceDocument(theURI, theDocument);
}



XalanDOMString
XSLTProcessorEnvSupportDefault::findURIFromDoc(const XalanDocument*		owner) const
{
	return m_defaultSupport.findURIFromDoc(owner);
}



XalanDocument*
XSLTProcessorEnvSupportDefault::getDOMFactory() const
{
	if (m_processor == 0)
	{
		return m_defaultSupport.getDOMFactory();
	}
	else
	{
		return m_processor->getDOMFactory();
	}
}



bool
XSLTProcessorEnvSupportDefault::elementAvailable(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	extensionName) const
{
	return m_defaultSupport.elementAvailable(theNamespace,
											 extensionName);
}



bool
XSLTProcessorEnvSupportDefault::functionAvailable(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	extensionName) const
{
	return m_defaultSupport.functionAvailable(theNamespace,
											  extensionName);
}



XObject*
XSLTProcessorEnvSupportDefault::extFunction(
			XPathExecutionContext&			executionContext,
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			extensionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec) const
{
	return m_defaultSupport.extFunction(executionContext,
										theNamespace,
										extensionName,
										context,
										argVec);
}



XLocator*
XSLTProcessorEnvSupportDefault::getXLocatorFromNode(const XalanNode*	node) const
{
	const XLocatorTableType::const_iterator		i =
					m_xlocatorTable.find(node);

	if (i == m_xlocatorTable.end())
	{
		return 0;
	}
	else
	{
		return i->second;
	}
}



void
XSLTProcessorEnvSupportDefault::associateXLocatorToNode(
			const XalanNode*	node,
			XLocator*			xlocator)
{
	m_xlocatorTable[node] = xlocator;
}



bool
XSLTProcessorEnvSupportDefault::shouldStripSourceNode(const XalanNode&	node) const
{
	if (m_processor == 0)
	{
		return m_defaultSupport.shouldStripSourceNode(node);
	}
	else
	{
		return m_processor->shouldStripSourceNode(node);
	}
}



bool
XSLTProcessorEnvSupportDefault::problem(
			eSource					where,
			eClassification			classification,
			const XalanNode*		styleNode,
			const XalanNode*		sourceNode,
			const XalanDOMString&	msg,
			int						lineNo,
			int						charOffset) const
{
	return m_defaultSupport.problem(where,
									classification,
									styleNode,
									sourceNode,
									msg,
									lineNo,
									charOffset);
}



bool
XSLTProcessorEnvSupportDefault::problem(
			eSource					where,
			eClassification			classification,
			const PrefixResolver*	resolver,
			const XalanNode*		sourceNode,
			const XalanDOMString&	msg,
			int						lineNo,
			int						charOffset) const
{
	return m_defaultSupport.problem(where,
									classification,
									resolver,
									sourceNode,
									msg,
									lineNo,
									charOffset);
}
