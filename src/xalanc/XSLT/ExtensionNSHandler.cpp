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

#include "ExtensionNSHandler.hpp"



#include <cassert>


#include <xalanc/XalanDOM/XalanElement.hpp>



#include <xalanc/PlatformSupport/StringTokenizer.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class XSLTProcessor;



struct XSLProcessorContext
{
	const XSLTProcessor&	m_processor;
	const Stylesheet&		m_stylesheetTree;
	const XalanNode*		m_sourceTree;
	const XalanNode*		m_sourceNode;
	const XalanQName&		m_mode;

	XSLProcessorContext(
			const XSLTProcessor&	processor,
			const Stylesheet&		stylesheetTree, 
			const XalanNode*		sourceTree,
			const XalanNode*		sourceNode,
			const XalanQName&		mode) :
		m_processor(processor),
		m_stylesheetTree(stylesheetTree),
		m_sourceTree(sourceTree),
		m_sourceNode(sourceNode),
		m_mode(mode)
	{
	}
};



/**
 * Construct a new extension namespace handler for a given extension NS.
 * This doesn't do anything - just hang on to the namespace URI.
 * 
 * @param namespaceUri the extension namespace URI that I'm implementing
 */
ExtensionNSHandler::ExtensionNSHandler(const XalanDOMString&	namespaceUri) :
	ExtensionFunctionHandler(namespaceUri),
	m_elements(),
	m_componentDescLoaded(false)
{
}



ExtensionNSHandler::ExtensionNSHandler (
			const XalanDOMString& namespaceUri,
			const XalanDOMString& elemNames,
			const XalanDOMString& funcNames,
			const XalanDOMString& lang,
			const XalanDOMString& srcURL,
			const XalanDOMString& src) :
	ExtensionFunctionHandler(namespaceUri, funcNames, lang, srcURL, src),
	m_elements(),
	m_componentDescLoaded(true)
		 
{
	setElements(elemNames);
}



void
ExtensionNSHandler::setFunctions(const XalanDOMString&	funcNames)
{
    ExtensionFunctionHandler::setFunctions(funcNames);

    m_componentDescLoaded = true;
}



void
ExtensionNSHandler::setScript(
			const XalanDOMString&	lang,
			const XalanDOMString&	srcURL,
			const XalanDOMString&	scriptSrc)
{
    ExtensionFunctionHandler::setScript(lang, srcURL, scriptSrc);

    m_componentDescLoaded = true;
}



void
ExtensionNSHandler::setElements(const XalanDOMString&	elemNames)
{
    if (length(elemNames) != 0)
	{
		StringTokenizer		st(elemNames, s_tokenDelimiterCharacters, false);

		while (st.hasMoreTokens() == true)
		{
			m_elements.insert(st.nextToken()); // just stick it in there basically
		}
	
		m_componentDescLoaded = true;
	}
}



bool
ExtensionNSHandler::isElementAvailable(const XalanDOMString&	element) const
{
    return (m_elements.find(element) != m_elements.end());
}



void
ExtensionNSHandler::processElement(
			StylesheetExecutionContext&		/* executionContext */,
			const XalanDOMString&			/* localPart */,
			const XalanElement*				/* element */,
			Stylesheet&						/* stylesheetTree */, 
			const XalanNode*				/* sourceTree */,
			const XalanNode*				/* sourceNode */,
			const XalanQName&				/* mode */)
{
}



void
ExtensionNSHandler::startupComponent()
{
	ExtensionFunctionHandler::startupComponent();
}



void
ExtensionNSHandler::loadComponentDescription()
{
	assert(0);		// Don't try this in C++
}



XalanDOMString
ExtensionNSHandler::getScriptString(const XalanElement&		/* elem */)
{
	return XalanDOMString();
}



XALAN_CPP_NAMESPACE_END
