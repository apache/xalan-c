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

#include "StylesheetExecutionContextDefault.hpp"



#include <algorithm>
#include <cassert>



#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XPath/ResultTreeFragBase.hpp>
#include <XPath/XPathExecutionContext.hpp>
#include <XPath/QName.hpp>

#include <XMLSupport/XMLParserLiaison.hpp>

#include "StylesheetRoot.hpp"
#include "XSLTEngineImpl.hpp"



StylesheetExecutionContextDefault::StylesheetExecutionContextDefault(
			XPathExecutionContext&	xpathExecutionContext,
			XSLTEngineImpl&			xsltProcessor) :
	StylesheetExecutionContext(),
	m_xpathExecutionContext(xpathExecutionContext),
	m_xsltProcessor(xsltProcessor),
	m_elementRecursionStack()
{
}



StylesheetExecutionContextDefault::~StylesheetExecutionContextDefault()
{
}



void
StylesheetExecutionContextDefault::error(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode,
			const DOM_Node&		styleNode) const
{
	m_xpathExecutionContext.error(msg, sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::warn(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode,
			const DOM_Node&		styleNode) const
{
	m_xpathExecutionContext.warn(msg, sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::message(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode,
			const DOM_Node&		styleNode) const
{
	m_xpathExecutionContext.message(msg, sourceNode, styleNode);
}



const DOM_Node
StylesheetExecutionContextDefault::getParentOfNode(const DOM_Node&	theNode) const
{
	return m_xpathExecutionContext.getParentOfNode(theNode);
}



XPathExecutionContext&
StylesheetExecutionContextDefault::getXPathExecutionContext()
{
	return m_xpathExecutionContext;
}



const NodeRefListBase&
StylesheetExecutionContextDefault::getContextNodeList() const
{
	return m_xpathExecutionContext.getContextNodeList();
}


void
StylesheetExecutionContextDefault::setContextNodeList(const NodeRefListBase&	theContextNodeList)
{
	m_xpathExecutionContext.setContextNodeList(theContextNodeList);
}



XObject*
StylesheetExecutionContextDefault::getTopLevelVariable(const DOMString&	theName) const
{
	return m_xsltProcessor.getTopLevelVariable(theName);
}



bool
StylesheetExecutionContextDefault::getQuietConflictWarnings() const
{
	return m_xsltProcessor.getQuietConflictWarnings();
}



const DOM_Document
StylesheetExecutionContextDefault::getRootDocument() const
{
	return m_xsltProcessor.getRootDoc();
}



void
StylesheetExecutionContextDefault::setRootDocument(const DOM_Document&	theDocument)
{
	m_xsltProcessor.setRootDoc(theDocument);
}



const DOM_Document
StylesheetExecutionContextDefault::createDocument() const
{
	return m_xsltProcessor.getXMLParserLiaison().createDocument();
}



void
StylesheetExecutionContextDefault::setStylesheetRoot(StylesheetRoot*	theStylesheet)
{
	assert(theStylesheet->isRoot() == true);

	m_stylesheetRoot = theStylesheet;
}



void
StylesheetExecutionContextDefault::resetCurrentState(
			const DOM_Node&		sourceTree,
			const DOM_Node&		xmlNode)
{
	m_xsltProcessor.resetCurrentState(sourceTree, xmlNode);
}


bool
StylesheetExecutionContextDefault::doDiagnosticsOutput() const
{
	return m_xsltProcessor.doDiagnosticsOutput();
}



void
StylesheetExecutionContextDefault::diag(const DOMString&	theString)
{
	m_xsltProcessor.diag(theString);
}


void
StylesheetExecutionContextDefault::pushTime(const void*	theKey)
{
	m_xsltProcessor.pushTime(theKey);
}


void
StylesheetExecutionContextDefault::displayDuration(
			const DOMString&	theMessage,
			const void*			theKey)
{
	m_xsltProcessor.displayDuration(theMessage, theKey);
}


const AttributeList&
StylesheetExecutionContextDefault::getPendingAttributes() const
{
	return m_xsltProcessor.getPendingAttributes();
}



DOMString
StylesheetExecutionContextDefault::getPendingElementName() const
{
	return m_xsltProcessor.getPendingElementName();
}



void
StylesheetExecutionContextDefault::setPendingAttributes(const AttributeList&	pendingAttributes)
{
	m_xsltProcessor.setPendingAttributes(pendingAttributes);
}



void
StylesheetExecutionContextDefault::replacePendingAttribute(
			const XMLCh*	theName,
			const XMLCh*	theNewType,
			const XMLCh*	theNewValue)
{
	// Make a copy of the attribute list, and modify it, to be more exception-safe.
	AttributeListImpl	thePendingAttributes(m_xsltProcessor.getPendingAttributes());

	// Remove the old attribute, then add the new one...
	thePendingAttributes.removeAttribute(theName);
	thePendingAttributes.addAttribute(theName, theNewType, theNewValue);

	// Set the new pending attributes...
	m_xsltProcessor.setPendingAttributes(thePendingAttributes);
}



void
StylesheetExecutionContextDefault::setPendingElementName(const DOMString&	elementName)
{
	m_xsltProcessor.setPendingElementName(elementName);
}



void
StylesheetExecutionContextDefault::addResultAttribute(
			const DOMString&	aname,
			const DOMString&	value)
{
	m_xsltProcessor.addResultAttribute(aname, value);
}



void
StylesheetExecutionContextDefault::copyNamespaceAttributes(
			const DOM_Node&		src,
			bool				srcIsStylesheetTree)
{
	m_xsltProcessor.copyNamespaceAttributes(src, srcIsStylesheetTree);
}



DOMString
StylesheetExecutionContextDefault::getResultPrefixForNamespace(
			const DOMString&	theNamespace) const
{
	return m_xsltProcessor.getResultPrefixForNamespace(theNamespace);
}



DOMString
StylesheetExecutionContextDefault::getResultNamespaceForPrefix(
			const DOMString&	thePrefix) const
{
	return m_xsltProcessor.getResultNamespaceForPrefix(thePrefix);
}



DOMString
StylesheetExecutionContextDefault::getUniqueNameSpaceValue() const
{
	return m_xsltProcessor.getUniqueNSValue();
}



FormatterListener*
StylesheetExecutionContextDefault::getFormatterListener() const
{
	return m_xsltProcessor.getFormatterListener();
}



void
StylesheetExecutionContextDefault::setFormatterListener(FormatterListener*	flistener)
{
	m_xsltProcessor.setFormatterListener(flistener);
}



int
StylesheetExecutionContextDefault::getIndent() const
{
		return m_xsltProcessor.getXMLParserLiaison().getIndent();
}



XObject*
StylesheetExecutionContextDefault::executeXPath(
			const DOMString&		str,
			const DOM_Node&			contextNode,
			const PrefixResolver&	resolver)
{
	return m_xsltProcessor.evalXPathStr(str,
										contextNode,
										resolver,
										m_xpathExecutionContext);
}



XObject*
StylesheetExecutionContextDefault::executeXPath(
			const DOMString&	str,
			const DOM_Node&		contextNode,
			const DOM_Element&	resolver)
{
	return m_xsltProcessor.evalXPathStr(str,
										contextNode,
										resolver,
										m_xpathExecutionContext);
}



XPath*
StylesheetExecutionContextDefault::createMatchPattern(
			const DOMString&		str,
			const PrefixResolver&	resolver)
{
	return m_xsltProcessor.createMatchPattern(str, resolver);
}



const DOMString
StylesheetExecutionContextDefault::evaluateAttrVal(
			const DOM_Node&		contextNode,
			const DOM_Element&	namespaceContext,
			const DOMString&	stringedValue)
{
	return m_xsltProcessor.evaluateAttrVal(contextNode,
										   namespaceContext,
										   stringedValue,
										   m_xpathExecutionContext);
}



void
StylesheetExecutionContextDefault::pushVariable(
			const QName&		name,
			XObject*			var,
			const DOM_Node&		element)
{
	m_xsltProcessor.pushVariable(name, var, element);
}



void
StylesheetExecutionContextDefault::pushContextMarker(
			const DOM_Node&		caller,
			const DOM_Node&		sourceNode)
{
	m_xsltProcessor.pushContextMarker(caller, sourceNode);
}



void
StylesheetExecutionContextDefault::popCurrentContext()
{
	m_xsltProcessor.popCurrentContext();
}



void
StylesheetExecutionContextDefault::resolveTopLevelParams()
{
	m_xsltProcessor.resolveTopLevelParams(*this);
}



void
StylesheetExecutionContextDefault::clearTopLevelParams()
{
	m_xsltProcessor.clearTopLevelParams();
}



void
StylesheetExecutionContextDefault::pushParams(
			const ElemTemplateElement&	xslCallTemplateElement,
			const DOM_Node&				sourceTree, 
			const DOM_Node&				sourceNode,
			const QName&				mode,
			const DOM_Node&				targetTemplate)
{
	m_xsltProcessor.pushParams(*this,
							   xslCallTemplateElement,
							   sourceTree,
							   sourceNode,
							   mode,
							   targetTemplate);
}



XObject*
StylesheetExecutionContextDefault::getParamVariable(const QName&	theName) const
{
	return m_xsltProcessor.getParamVariable(theName);
}



void
StylesheetExecutionContextDefault::startDocument()
{
	m_xsltProcessor.startDocument();
}



void
StylesheetExecutionContextDefault::endDocument()
{
	m_xsltProcessor.endDocument();
}



void
StylesheetExecutionContextDefault::characters(
			const XMLCh*	ch,
			unsigned int	start,
			unsigned int	length)
{
	m_xsltProcessor.characters(ch, start, length);
}



void
StylesheetExecutionContextDefault::charactersRaw(
			const XMLCh*	ch,
			unsigned int	start,
			unsigned int	length)
{
	m_xsltProcessor.charactersRaw(ch, start, length);
}



void
StylesheetExecutionContextDefault::comment(
			const XMLCh*	data)
{
	m_xsltProcessor.comment(data);
}



void
StylesheetExecutionContextDefault::processingInstruction(
			const XMLCh*	target,
			const XMLCh*	data)
{
	m_xsltProcessor.processingInstruction(target, data);
}



void
StylesheetExecutionContextDefault::startElement(
			const XMLCh*	name)
{
	m_xsltProcessor.startElement(name);
}



void
StylesheetExecutionContextDefault::endElement(
			const XMLCh*	name)
{
	m_xsltProcessor.endElement(name);
}



void
StylesheetExecutionContextDefault::flushPending()
{
	m_xsltProcessor.flushPending();
}



void
StylesheetExecutionContextDefault::cloneToResultTree(
			const DOM_Node&			node, 
			bool					isLiteral,
			bool					overrideStrip,
			bool					shouldCloneAttributes)
{
	m_xsltProcessor.cloneToResultTree(node,
									  isLiteral,
									  overrideStrip,
									  shouldCloneAttributes);
}



XObject*
StylesheetExecutionContextDefault::createXResultTreeFrag(
			const ElemTemplateElement&		templateChild,
			const DOM_Node&					sourceTree,
			const DOM_Node&					sourceNode)
{
	return createXResultTreeFrag(templateChild,
								 sourceTree,
								 sourceNode,
								 QName());
}



XObject*
StylesheetExecutionContextDefault::createXResultTreeFrag(
			const ElemTemplateElement&		templateChild,
			const DOM_Node&					sourceTree,
			const DOM_Node&					sourceNode,
			const QName&					mode)
{
	std::auto_ptr<ResultTreeFragBase>
		theFragment(m_xsltProcessor.createResultTreeFrag(*this,
														 templateChild,
														 sourceTree,
														 sourceNode,
														 mode));

	return m_xsltProcessor.createXResultTreeFrag(*theFragment.get());
}



void
StylesheetExecutionContextDefault::outputResultTreeFragment(const XObject&	theTree)
{
	m_xsltProcessor.outputResultTreeFragment(theTree);
}



const DOMString&
StylesheetExecutionContextDefault::getXSLNameSpaceURL() const
{
	return m_xsltProcessor.getXSLNameSpaceURL();
}


const DOMString&
StylesheetExecutionContextDefault::getXalanXSLNameSpaceURL() const
{
	return m_xsltProcessor.getXalanXSLNameSpaceURL();
}


bool
StylesheetExecutionContextDefault::isTraceSelect() const
{
	return m_xsltProcessor.isTraceSelect();
}


void
StylesheetExecutionContextDefault::traceSelect(
			const DOM_Element&		theTemplate,
			const NodeRefListBase&	nl) const
{
	m_xsltProcessor.traceSelect(theTemplate, nl);
}



bool
StylesheetExecutionContextDefault::findOnElementRecursionStack(
			const ElemTemplateElement*	theElement) const
{
	const ElementRecursionStackType::const_iterator	i =
			std::find(m_elementRecursionStack.begin(),
					  m_elementRecursionStack.end(),
					  theElement);

	return i == m_elementRecursionStack.end() ? false : true;
}



void
StylesheetExecutionContextDefault::pushOnElementRecursionStack(
			const ElemTemplateElement*	theElement)
{
	assert(findOnElementRecursionStack(theElement) == false);

	m_elementRecursionStack.push_back(theElement);
}



const ElemTemplateElement*
StylesheetExecutionContextDefault::popElementRecursionStack()
{
	assert(m_elementRecursionStack.empty() == false);

	const ElemTemplateElement* const	theTemp =
		m_elementRecursionStack.back();

	m_elementRecursionStack.pop_back();

	return theTemp;
}
