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

#include "StylesheetExecutionContextDefault.hpp"



#include <algorithm>
#include <cassert>



#include <PlatformSupport/STLHelper.hpp>
#include <PlatformSupport/TextOutputStream.hpp>



#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XPath/ResultTreeFragBase.hpp>
#include <XPath/XPathExecutionContext.hpp>
#include <XPath/QName.hpp>



#include <XMLSupport/XMLParserLiaison.hpp>



// Yuck, these really shouldn't be here...
#include <XercesPlatformSupport/XercesDOMPrintWriter.hpp>
#include <XercesPlatformSupport/XercesStdTextOutputStream.hpp>
#include <XercesPlatformSupport/TextFileOutputStream.hpp>



#include "StylesheetRoot.hpp"
#include "XSLTEngineImpl.hpp"



StylesheetExecutionContextDefault::StylesheetExecutionContextDefault(
			XSLTEngineImpl&			xsltProcessor,
			XPathEnvSupport&		theXPathEnvSupport,
			XPathSupport&			theXPathSupport,
			XObjectFactory&			theXObjectFactory,
			XalanNode*				theCurrentNode,
			const NodeRefListBase&	theContextNodeList,
			const PrefixResolver*	thePrefixResolver) :
	StylesheetExecutionContext(),
	m_xpathExecutionContextDefault(theXPathEnvSupport,
								   theXPathSupport,
								   theXObjectFactory,
								   theCurrentNode,
								   theContextNodeList,
								   thePrefixResolver),
	m_xsltProcessor(xsltProcessor),
	m_elementRecursionStack(),
	m_prefixResolver(0),
	m_stylesheetRoot(0),
	m_printWriters(),
	m_textOutputStreams()
{
}



StylesheetExecutionContextDefault::~StylesheetExecutionContextDefault()
{
	reset();
}



void
StylesheetExecutionContextDefault::reset()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(m_printWriters.begin(),
			 m_printWriters.end(),
			 DeleteFunctor<PrintWriter>());

	m_printWriters.clear();

	for_each(m_textOutputStreams.begin(),
			 m_textOutputStreams.end(),
			 DeleteFunctor<TextOutputStream>());

	m_textOutputStreams.clear();
}



XObject*
StylesheetExecutionContextDefault::getTopLevelVariable(const XalanDOMString&	theName) const
{
	return m_xsltProcessor.getTopLevelVariable(theName);
}



bool
StylesheetExecutionContextDefault::getQuietConflictWarnings() const
{
	return m_xsltProcessor.getQuietConflictWarnings();
}



XalanDocument*
StylesheetExecutionContextDefault::getRootDocument() const
{
	return m_xsltProcessor.getRootDoc();
}



void
StylesheetExecutionContextDefault::setRootDocument(XalanDocument*	theDocument)
{
	m_xsltProcessor.setRootDoc(theDocument);
}



XalanDocument*
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
			XalanNode*	sourceTree,
			XalanNode*	xmlNode)
{
	m_xsltProcessor.resetCurrentState(sourceTree, xmlNode);
}


bool
StylesheetExecutionContextDefault::doDiagnosticsOutput() const
{
	return m_xsltProcessor.doDiagnosticsOutput();
}



void
StylesheetExecutionContextDefault::diag(const XalanDOMString&	theString)
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
			const XalanDOMString&	theMessage,
			const void*				theKey)
{
	m_xsltProcessor.displayDuration(theMessage, theKey);
}



const AttributeList&
StylesheetExecutionContextDefault::getPendingAttributes() const
{
	return m_xsltProcessor.getPendingAttributes();
}



XalanDOMString
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
			const XalanDOMChar*		theName,
			const XalanDOMChar*		theNewType,
			const XalanDOMChar*		theNewValue)
{
	// Make a copy of the attribute list, and modify it, to be more exception-safe.
	AttributeListImpl	thePendingAttributes(m_xsltProcessor.getPendingAttributes());

	// Remove the old attribute, then add the new one...
	// thePendingAttributes.removeAttribute(theName);
	thePendingAttributes.addAttribute(theName, theNewType, theNewValue);

	// Set the new pending attributes...
	m_xsltProcessor.setPendingAttributes(thePendingAttributes);
}



void
StylesheetExecutionContextDefault::setPendingElementName(const XalanDOMString&	elementName)
{
	m_xsltProcessor.setPendingElementName(elementName);
}



void
StylesheetExecutionContextDefault::addResultAttribute(
			const XalanDOMString&	aname,
			const XalanDOMString&	value)
{
	m_xsltProcessor.addResultAttribute(aname, value);
}



void
StylesheetExecutionContextDefault::copyNamespaceAttributes(
			const XalanNode&	src,
			bool				srcIsStylesheetTree)
{
	m_xsltProcessor.copyNamespaceAttributes(src, srcIsStylesheetTree);
}



XalanDOMString
StylesheetExecutionContextDefault::getResultPrefixForNamespace(
			const XalanDOMString&	theNamespace) const
{
	return m_xsltProcessor.getResultPrefixForNamespace(theNamespace);
}



XalanDOMString
StylesheetExecutionContextDefault::getResultNamespaceForPrefix(const XalanDOMString&	thePrefix) const
{
	return m_xsltProcessor.getResultNamespaceForPrefix(thePrefix);
}



XalanDOMString
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
			const XalanDOMString&	str,
			XalanNode*				contextNode,
			const PrefixResolver&	resolver)
{
	return m_xsltProcessor.evalXPathStr(str,
										contextNode,
										resolver,
										*this);
}



XObject*
StylesheetExecutionContextDefault::executeXPath(
			const XalanDOMString&	str,
			XalanNode*				contextNode,
			const XalanElement&		resolver)
{
	return m_xsltProcessor.evalXPathStr(str,
										contextNode,
										resolver,
										*this);
}



XPath*
StylesheetExecutionContextDefault::createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver)
{
	return m_xsltProcessor.createMatchPattern(str, resolver);
}



const XalanDOMString
StylesheetExecutionContextDefault::evaluateAttrVal(
			XalanNode*				contextNode,
			const XalanElement&		namespaceContext,
			const XalanDOMString&	stringedValue)
{
	return m_xsltProcessor.evaluateAttrVal(contextNode,
										   namespaceContext,
										   stringedValue,
										   *this);
}



void
StylesheetExecutionContextDefault::pushVariable(
			const QName&		name,
			XObject*			var,
			const XalanNode*	element)
{
	m_xsltProcessor.pushVariable(name, var, element);
}



void
StylesheetExecutionContextDefault::pushContextMarker(
			const XalanNode*	caller,
			const XalanNode*	sourceNode)
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
			XalanNode*					sourceTree, 
			XalanNode*					sourceNode,
			const QName&				mode,
			const XalanNode*			targetTemplate)
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



int
StylesheetExecutionContextDefault::getCurrentStackFrameIndex() const
{
	return m_xsltProcessor.getVariableStacks().getCurrentStackFrameIndex();
}



void
StylesheetExecutionContextDefault::setCurrentStackFrameIndex(int	currentStackFrameIndex)
{
	m_xsltProcessor.getVariableStacks().setCurrentStackFrameIndex(currentStackFrameIndex);
}



void
StylesheetExecutionContextDefault::markGlobalStackFrame()
{
	m_xsltProcessor.getVariableStacks().markGlobalStackFrame();
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
			const XalanDOMChar*		ch,
			unsigned int			start,
			unsigned int			length)
{
	m_xsltProcessor.characters(ch, start, length);
}



void
StylesheetExecutionContextDefault::charactersRaw(
			const XalanDOMChar*		ch,
			unsigned int			start,
			unsigned int			length)
{
	m_xsltProcessor.charactersRaw(ch, start, length);
}



void
StylesheetExecutionContextDefault::comment(const XalanDOMChar*	data)
{
	m_xsltProcessor.comment(data);
}



void
StylesheetExecutionContextDefault::processingInstruction(
			const XalanDOMChar*		target,
			const XalanDOMChar*		data)
{
	m_xsltProcessor.processingInstruction(target, data);
}



void
StylesheetExecutionContextDefault::startElement(const XalanDOMChar*		name)
{
	m_xsltProcessor.startElement(name);
}



void
StylesheetExecutionContextDefault::endElement(const XalanDOMChar*	name)
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
			XalanNode&	node, 
			bool		isLiteral,
			bool		overrideStrip,
			bool		shouldCloneAttributes)
{
	m_xsltProcessor.cloneToResultTree(node,
									  isLiteral,
									  overrideStrip,
									  shouldCloneAttributes);
}



XObject*
StylesheetExecutionContextDefault::createXResultTreeFrag(
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceTree,
			XalanNode*					sourceNode)
{
	return createXResultTreeFrag(templateChild,
								 sourceTree,
								 sourceNode,
								 QName());
}



XObject*
StylesheetExecutionContextDefault::createXResultTreeFrag(
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceTree,
			XalanNode*					sourceNode,
			const QName&				mode)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::auto_ptr;
#endif

	auto_ptr<ResultTreeFragBase>
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



const XalanDOMString&
StylesheetExecutionContextDefault::getXSLNameSpaceURL() const
{
	return m_xsltProcessor.getXSLNameSpaceURL();
}



const XalanDOMString&
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
			const XalanElement&		theTemplate,
			const NodeRefListBase&	nl) const
{
	m_xsltProcessor.traceSelect(theTemplate, nl);
}



bool
StylesheetExecutionContextDefault::findOnElementRecursionStack(const ElemTemplateElement*	theElement) const
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::find;
#endif

	const ElementRecursionStackType::const_iterator	i =
				find(m_elementRecursionStack.begin(),
					 m_elementRecursionStack.end(),
					 theElement);

	return i == m_elementRecursionStack.end() ? false : true;
}



void
StylesheetExecutionContextDefault::pushOnElementRecursionStack(const ElemTemplateElement*	theElement)
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



XalanNode*
StylesheetExecutionContextDefault::getCurrentNode() const
{
	return m_xpathExecutionContextDefault.getCurrentNode();
}



void
StylesheetExecutionContextDefault::setCurrentNode(XalanNode*	theCurrentNode)
{
	m_xpathExecutionContextDefault.setCurrentNode(theCurrentNode);
}



XObjectFactory&
StylesheetExecutionContextDefault::getXObjectFactory() const
{
	return m_xpathExecutionContextDefault.getXObjectFactory();
}



XalanDOMString
StylesheetExecutionContextDefault::getNamespaceOfNode(const XalanNode&	n) const
{
	return m_xpathExecutionContextDefault.getNamespaceOfNode(n);
}



XalanDOMString
StylesheetExecutionContextDefault::getLocalNameOfNode(const XalanNode&	n) const
{
	return m_xpathExecutionContextDefault.getLocalNameOfNode(n);
}



XalanNode*
StylesheetExecutionContextDefault::getParentOfNode(const XalanNode&		theNode) const
{
	return m_xpathExecutionContextDefault.getParentOfNode(theNode);
}



bool
StylesheetExecutionContextDefault::isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const
{
	return m_xpathExecutionContextDefault.isNodeAfter(node1, node2);
}



XalanDOMString
StylesheetExecutionContextDefault::getNodeData(const XalanNode&		n) const
{
	return m_xpathExecutionContextDefault.getNodeData(n);
}



XalanElement*
StylesheetExecutionContextDefault::getElementByID(
			const XalanDOMString&		id,
			const XalanDocument&		doc) const
{
	return m_xpathExecutionContextDefault.getElementByID(id, doc);
}



const NodeRefListBase&
StylesheetExecutionContextDefault::getContextNodeList() const
{
	return m_xpathExecutionContextDefault.getContextNodeList();
}



void
StylesheetExecutionContextDefault::setContextNodeList(const NodeRefListBase&	theContextNodeList)
{
	m_xpathExecutionContextDefault.setContextNodeList(theContextNodeList);
}



int
StylesheetExecutionContextDefault::getContextNodeListLength() const
{
	return m_xpathExecutionContextDefault.getContextNodeListLength();
}



int
StylesheetExecutionContextDefault::getContextNodeListPosition(const XalanNode&	contextNode) const
{
	return m_xpathExecutionContextDefault.getContextNodeListPosition(contextNode);
}



bool
StylesheetExecutionContextDefault::elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	extensionName) const
{
	return m_xpathExecutionContextDefault.elementAvailable(theNamespace, extensionName);
}



bool
StylesheetExecutionContextDefault::functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	extensionName) const
{
	return m_xpathExecutionContextDefault.functionAvailable(theNamespace, extensionName);
}



XObject*
StylesheetExecutionContextDefault::extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			extensionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec)
{
	return m_xpathExecutionContextDefault.extFunction(theNamespace, extensionName, context, argVec);
}



XLocator*
StylesheetExecutionContextDefault::getXLocatorFromNode(const XalanNode*		node) const
{
	return m_xpathExecutionContextDefault.getXLocatorFromNode(node);
}



void
StylesheetExecutionContextDefault::associateXLocatorToNode(
			const XalanNode*	node,
			XLocator*			xlocator)
{
	m_xpathExecutionContextDefault.associateXLocatorToNode(node, xlocator);
}



XalanDocument*
StylesheetExecutionContextDefault::parseXML(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) const
{
	return m_xpathExecutionContextDefault.parseXML(urlString, base);
}



MutableNodeRefList
StylesheetExecutionContextDefault::createMutableNodeRefList() const
{
	return m_xpathExecutionContextDefault.createMutableNodeRefList();
}



bool
StylesheetExecutionContextDefault::getProcessNamespaces() const
{
	return m_xpathExecutionContextDefault.getProcessNamespaces();
}



const NodeRefListBase*
StylesheetExecutionContextDefault::getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const XalanElement&		nscontext)
{
	return m_xpathExecutionContextDefault.getNodeSetByKey(doc, name, ref, nscontext);
}



const NodeRefListBase*
StylesheetExecutionContextDefault::getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref)
{
	return m_xpathExecutionContextDefault.getNodeSetByKey(doc, name, ref);
}



const NodeRefListBase*
StylesheetExecutionContextDefault::getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const PrefixResolver&	resolver)
{
	return m_xpathExecutionContextDefault.getNodeSetByKey(doc, name, ref, resolver);
}



XObject*
StylesheetExecutionContextDefault::getVariable(const QName&		name) const
{
	return m_xpathExecutionContextDefault.getVariable(name);
}



const PrefixResolver*
StylesheetExecutionContextDefault::getPrefixResolver() const
{
	return m_xpathExecutionContextDefault.getPrefixResolver();
}



void
StylesheetExecutionContextDefault::setPrefixResolver(const PrefixResolver*		thePrefixResolver)
{
	m_xpathExecutionContextDefault.setPrefixResolver(thePrefixResolver);
}



XalanDOMString
StylesheetExecutionContextDefault::getNamespaceForPrefix(const XalanDOMString&	prefix) const
{
	return m_xpathExecutionContextDefault.getNamespaceForPrefix(prefix);
}



XalanDOMString
StylesheetExecutionContextDefault::findURIFromDoc(const XalanDocument*	owner) const
{
	return m_xpathExecutionContextDefault.findURIFromDoc(owner);
}



XalanDOMString
StylesheetExecutionContextDefault::getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const
{
	return m_xpathExecutionContextDefault.getUnparsedEntityURI(theName, theDocument);
}



bool
StylesheetExecutionContextDefault::shouldStripSourceNode(const XalanNode&	node) const
{
	return m_xpathExecutionContextDefault.shouldStripSourceNode(node);
}



bool
StylesheetExecutionContextDefault::getThrowFoundIndex() const
{
	return m_xpathExecutionContextDefault.getThrowFoundIndex();
}



void
StylesheetExecutionContextDefault::setThrowFoundIndex(bool 	fThrow)
{
	m_xpathExecutionContextDefault.setThrowFoundIndex(fThrow);
}



void
StylesheetExecutionContextDefault::setCurrentPattern(const XalanDOMString&	thePattern)
{
	m_xpathExecutionContextDefault.setCurrentPattern(thePattern);
}



XalanDOMString
StylesheetExecutionContextDefault::getCurrentPattern() const
{
	return m_xpathExecutionContextDefault.getCurrentPattern();
}



XalanDocument*
StylesheetExecutionContextDefault::getSourceDocument(const XalanDOMString&	theURI) const
{
	return m_xpathExecutionContextDefault.getSourceDocument(theURI);
}



void
StylesheetExecutionContextDefault::setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument)
{
	m_xpathExecutionContextDefault.setSourceDocument(theURI, theDocument);
}



const DecimalFormatSymbols*
StylesheetExecutionContextDefault::getDecimalFormatSymbols(const XalanDOMString&	name)
{
	if (m_stylesheetRoot == 0)
	{
		return m_xpathExecutionContextDefault.getDecimalFormatSymbols(name);
	}
	else
	{
		return m_stylesheetRoot->getDecimalFormatSymbols(name);
	}
}



PrintWriter*
StylesheetExecutionContextDefault::createPrintWriter(TextOutputStream*	theTextOutputStream)
{
	assert(theTextOutputStream != 0);

	PrintWriter* const	thePrintWriter =
		new XercesDOMPrintWriter(*theTextOutputStream);

	m_printWriters.insert(thePrintWriter);

	return thePrintWriter;
}



PrintWriter*
StylesheetExecutionContextDefault::createPrintWriter(
			const XalanDOMString&		theFileName,
			const XalanDOMString&		/* theEncoding */)
{
	// $$$ ToDo: We need to either remove these explicit dependencies on the
	// Xerces classes, or make the Xerces classes more generic. (I prefer the
	// latter...)
	TextOutputStream* const		theTextOutputStream =
		new TextFileOutputStream(theFileName);

	m_textOutputStreams.insert(theTextOutputStream);

	return createPrintWriter(theTextOutputStream);
}



PrintWriter*
#if defined(XALAN_NO_NAMESPACES)
StylesheetExecutionContextDefault::createPrintWriter(ostream&	theStream)
#else
StylesheetExecutionContextDefault::createPrintWriter(std::ostream&	theStream)
#endif
{
	// $$$ ToDo: We need to either remove these explicit dependencies on the
	// Xerces classes, or make the Xerces classes more generic. (I prefer the
	// latter...)
	TextOutputStream* const		theTextOutputStream =
		new XercesStdTextOutputStream(theStream);

	m_textOutputStreams.insert(theTextOutputStream);

	return createPrintWriter(theTextOutputStream);
}



void
StylesheetExecutionContextDefault::error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const XalanNode*		styleNode) const
{
	m_xpathExecutionContextDefault.error(msg, sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const XalanNode*		styleNode) const
{
	m_xpathExecutionContextDefault.warn(msg, sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const XalanNode*		styleNode) const
{
	m_xpathExecutionContextDefault.message(msg, sourceNode, styleNode);
}
