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
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */

#include "StylesheetExecutionContextDefault.hpp"



#include <algorithm>
#include <cassert>



#include <Include/STLHelper.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanOutputStream.hpp>
#include <PlatformSupport/XalanNumberFormat.hpp>
#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <PlatformSupport/XalanStdOutputStream.hpp>
#include <PlatformSupport/XalanFileOutputStream.hpp>



#include <XPath/XalanQNameByReference.hpp>
#include <XPath/ResultTreeFragBase.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPath.hpp>
#include <XPath/XPathExecutionContext.hpp>
#include <XPath/XObject.hpp>



#include <XMLSupport/FormatterToDOM.hpp>
#include <XMLSupport/FormatterToXML.hpp>
#include <XMLSupport/FormatterToHTML.hpp>
#include <XMLSupport/XMLParserLiaison.hpp>



#include <XalanSourceTree/FormatterToSourceTree.hpp>
#include <XalanSourceTree/XalanSourceTreeDocument.hpp>
#include <XalanSourceTree/XalanSourceTreeDocumentFragment.hpp>



#include "Constants.hpp"
#include "ElemTemplateElement.hpp"
#include "ElemWithParam.hpp"
#include "StylesheetRoot.hpp"
#include "XSLTEngineImpl.hpp"
#include "XSLTProcessorException.hpp"



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "C:/Program Files/Rational/Quantify/pure.h"
#endif



StylesheetExecutionContextDefault::XalanNumberFormatFactory			StylesheetExecutionContextDefault::s_defaultXalanNumberFormatFactory;

StylesheetExecutionContextDefault::XalanNumberFormatFactory*		StylesheetExecutionContextDefault::s_xalanNumberFormatFactory =
		&StylesheetExecutionContextDefault::getDefaultXalanNumberFormatFactory();

const StylesheetExecutionContextDefault::DefaultCollationCompareFunctor		StylesheetExecutionContextDefault::s_defaultCollationFunctor;



StylesheetExecutionContextDefault::StylesheetExecutionContextDefault(
			XSLTEngineImpl&			xsltProcessor,
			XPathEnvSupport&		theXPathEnvSupport,
			DOMSupport&				theDOMSupport,
			XObjectFactory&			theXObjectFactory,
			XalanNode*				theCurrentNode,
			const NodeRefListBase*	theContextNodeList,
			const PrefixResolver*	thePrefixResolver) :
	StylesheetExecutionContext(&theXObjectFactory),
	m_xpathExecutionContextDefault(theXPathEnvSupport,
								   theDOMSupport,
								   theXObjectFactory,
								   theCurrentNode,
								   theContextNodeList,
								   thePrefixResolver),
	m_xsltProcessor(&xsltProcessor),
	m_rootDocument(0),
	m_elementRecursionStack(),
	m_prefixResolver(0),
	m_stylesheetRoot(0),
	m_formatterListeners(),
	m_printWriters(),
	m_outputStreams(),
	m_collationCompareFunctor(0),
	m_variablesStack(),
	m_matchPatternCache(),
	m_keyTables(),
	m_keyDeclarationSet(),
	m_countersTable(),
	m_ignoreHTMLElementNamespaces(false),
	m_sourceTreeResultTreeFactory(),
	m_mode(0),
	m_currentTemplate(0),
	m_formatterToTextCache(),
	m_formatterToSourceTreeCache(),
	m_nodeSorterCache(),
	m_indentAmount(-1),
	m_xresultTreeFragAllocator(eXResultTreeFragAllocatorBlockSize),
	m_resultTreeFragAllocator(eResultTreeFragAllocatorBlockSize),
	m_documentFragmentAllocator(eDocumentFragmentAllocatorBlockSize),
	m_documentAllocator(eDocumentAllocatorBlockSize),
	m_usePerInstanceDocumentFactory(true),
	m_cloneTextNodesOnly(false),
	m_escapeURLs(eEscapeURLsDefault),
	m_omitMETATag(eOmitMETATagDefault)
{
}



StylesheetExecutionContextDefault::StylesheetExecutionContextDefault(
			XalanNode*				theCurrentNode,
			const NodeRefListBase*	theContextNodeList,
			const PrefixResolver*	thePrefixResolver) :
	StylesheetExecutionContext(),
	m_xpathExecutionContextDefault(theCurrentNode,
								   theContextNodeList,
								   thePrefixResolver),
	m_xsltProcessor(0),
	m_rootDocument(0),
	m_elementRecursionStack(),
	m_prefixResolver(0),
	m_stylesheetRoot(0),
	m_formatterListeners(),
	m_printWriters(),
	m_outputStreams(),
	m_collationCompareFunctor(0),
	m_variablesStack(),
	m_matchPatternCache(),
	m_keyTables(),
	m_keyDeclarationSet(),
	m_countersTable(),
	m_ignoreHTMLElementNamespaces(false),
	m_sourceTreeResultTreeFactory(),
	m_mode(0),
	m_currentTemplate(0),
	m_formatterToTextCache(),
	m_formatterToSourceTreeCache(),
	m_nodeSorterCache(),
	m_indentAmount(-1),
	m_xresultTreeFragAllocator(eXResultTreeFragAllocatorBlockSize),
	m_resultTreeFragAllocator(eResultTreeFragAllocatorBlockSize),
	m_documentFragmentAllocator(eDocumentFragmentAllocatorBlockSize),
	m_documentAllocator(eDocumentAllocatorBlockSize),
	m_usePerInstanceDocumentFactory(true),
	m_cloneTextNodesOnly(false),
	m_escapeURLs(eEscapeURLsDefault)
{
}



StylesheetExecutionContextDefault::~StylesheetExecutionContextDefault()
{
	reset();
}



bool
StylesheetExecutionContextDefault::getQuietConflictWarnings() const
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->getQuietConflictWarnings();
}



bool
StylesheetExecutionContextDefault::getCopyTextNodesOnly() const
{
	return m_cloneTextNodesOnly;
}



void
StylesheetExecutionContextDefault::setCopyTextNodesOnly(bool	fValue)
{
	m_cloneTextNodesOnly = fValue;
}



XalanNode*
StylesheetExecutionContextDefault::getRootDocument() const
{
	return m_rootDocument;
}



void
StylesheetExecutionContextDefault::setRootDocument(XalanNode*	theDocument)
{
	m_rootDocument = theDocument;
}



XalanDocument*
StylesheetExecutionContextDefault::createDocument() const
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->getXMLParserLiaison().createDocument();
}



void
StylesheetExecutionContextDefault::setStylesheetRoot(const StylesheetRoot*	theStylesheet)
{
	assert(m_xsltProcessor != 0);
	assert(theStylesheet == 0 || theStylesheet->isRoot() == true);

	m_stylesheetRoot = theStylesheet;

	m_xsltProcessor->setStylesheetRoot(theStylesheet);

	if (theStylesheet == 0)
	{
		m_xsltProcessor->setExecutionContext(0);
	}
	else
	{
		m_xsltProcessor->setExecutionContext(this);
	}
}



const XalanQName*
StylesheetExecutionContextDefault::getCurrentMode() const
{
	return m_mode;
}



void
StylesheetExecutionContextDefault::setCurrentMode(const XalanQName*		theMode)
{		
	m_mode = theMode;
}



const ElemTemplate*
StylesheetExecutionContextDefault::getCurrentTemplate() const
{
	return m_currentTemplate;
}



void
StylesheetExecutionContextDefault::setCurrentTemplate(const ElemTemplate*	theTemplate)
{		
	m_currentTemplate = theTemplate;
}



bool
StylesheetExecutionContextDefault::doDiagnosticsOutput() const
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->doDiagnosticsOutput();
}



void
StylesheetExecutionContextDefault::diag(const XalanDOMString&	theString)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->diag(theString);
}



void
StylesheetExecutionContextDefault::pushTime(const void*	theKey)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->pushTime(theKey);
}



void
StylesheetExecutionContextDefault::displayDuration(
			const XalanDOMString&	theMessage,
			const void*				theKey)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->displayDuration(theMessage, theKey);
}



bool
StylesheetExecutionContextDefault::isElementPending() const
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->isElementPending();
}



void
StylesheetExecutionContextDefault::replacePendingAttribute(
			const XalanDOMChar*		theName,
			const XalanDOMChar*		theNewType,
			const XalanDOMChar*		theNewValue)
{
	assert(m_xsltProcessor != 0);

	// Remove the old attribute, then add the new one.  AttributeListImpl::addAttribute()
	// does this for us.
	m_xsltProcessor->replacePendingAttribute(theName, theNewType, theNewValue);
}



void
StylesheetExecutionContextDefault::pushOutputContext(FormatterListener*		flistener)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->pushOutputContext(flistener);
}



void
StylesheetExecutionContextDefault::popOutputContext()
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->popOutputContext();
}



void
StylesheetExecutionContextDefault::addResultAttribute(
			const XalanDOMString&	aname,
			const XalanDOMString&	value)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->addResultAttribute(aname, value);
}



void
StylesheetExecutionContextDefault::copyNamespaceAttributes(const XalanNode&		src)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->copyNamespaceAttributes(src);
}



const XalanDOMString*
StylesheetExecutionContextDefault::getResultPrefixForNamespace(const XalanDOMString&	theNamespace) const
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->getResultPrefixForNamespace(theNamespace);
}



const XalanDOMString*
StylesheetExecutionContextDefault::getResultNamespaceForPrefix(const XalanDOMString&	thePrefix) const
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->getResultNamespaceForPrefix(thePrefix);
}



bool
StylesheetExecutionContextDefault::isPendingResultPrefix(const XalanDOMString&	thePrefix)
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->isPendingResultPrefix(thePrefix);
}



XalanDOMString
StylesheetExecutionContextDefault::getUniqueNamespaceValue() const
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->getUniqueNamespaceValue();
}



void
StylesheetExecutionContextDefault::getUniqueNamespaceValue(XalanDOMString&	theValue) const
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->getUniqueNamespaceValue(theValue);
}



FormatterListener*
StylesheetExecutionContextDefault::getFormatterListener() const
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->getFormatterListener();
}



void
StylesheetExecutionContextDefault::setFormatterListener(FormatterListener*	flistener)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->setFormatterListener(flistener);
}



int
StylesheetExecutionContextDefault::getIndent() const
{
	if (m_indentAmount != -1)
	{
		return m_indentAmount;
	}
	else
	{
		assert(m_xsltProcessor != 0);

		return m_xsltProcessor->getXMLParserLiaison().getIndent();
	}
}



void
StylesheetExecutionContextDefault::setIndent(int	indentAmount)
{
	m_indentAmount = indentAmount;
}



const XObjectPtr
StylesheetExecutionContextDefault::executeXPath(
			const XalanDOMString&	str,
			XalanNode*				contextNode,
			const XalanElement&		resolver)
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->evalXPathStr(str,
										contextNode,
										resolver,
										*this);
}



const XPath*
StylesheetExecutionContextDefault::createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver)
{
	assert(m_xsltProcessor != 0);

	const XPath*	theResult = 0;

	// We won't cache any xpath that has a namespace, since
	// we have no idea how that might be resolved.  We could
	// enhance XPath so that we can tell if str would match
	// the XPath, once the namespace is resolved, but it may
	// not be worth it...
	const XalanDOMString::size_type		index = indexOf(str, XalanUnicode::charColon);
	const XalanDOMString::size_type		len = length(str);

	// If we found a ':' before the end of the string, and
	// it's by itself (:: would indicate an axis), don't
	// try to cache the XPath...
	if (index < len - 1 && (charAt(str, index + 1) != XalanUnicode::charColon))
	{
		theResult = m_xsltProcessor->createMatchPattern(str, resolver);
	}
	else
	{
		const XPathCacheMapType::iterator	i =
			m_matchPatternCache.find(str);

		if (i != m_matchPatternCache.end())
		{
			// Update hit time...
#if defined(XALAN_STRICT_ANSI_HEADERS)
			(*i).second.second = std::clock();
#else
			(*i).second.second = clock();
#endif

			theResult = (*i).second.first;
		}
		else
		{
			theResult = m_xsltProcessor->createMatchPattern(str, resolver);

			addToXPathCache(str, theResult);
		}
	}

	return theResult;
}



void
StylesheetExecutionContextDefault::returnXPath(const XPath*		xpath)
{
	assert(m_xsltProcessor != 0);

	if (isCached(xpath) == false)
	{
		m_xsltProcessor->returnXPath(xpath);
	}
}



void
StylesheetExecutionContextDefault::pushTopLevelVariables(const ParamVectorType&		topLevelParams)
{
	assert(m_stylesheetRoot != 0);

	m_stylesheetRoot->pushTopLevelVariables(*this, topLevelParams);
}



const XObjectPtr
StylesheetExecutionContextDefault::createVariable(
			const ElemTemplateElement*	/* element */,
			const XPath&				xpath,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver)
{
	return xpath.execute(contextNode, resolver, *this);
}



const XObjectPtr
StylesheetExecutionContextDefault::createVariable(
			const ElemTemplateElement*	/* element */,
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceNode)
{
	return createXResultTreeFrag(templateChild, sourceNode);
}



void
StylesheetExecutionContextDefault::pushVariable(
			const XalanQName&			name,
			const ElemTemplateElement*	element,
			const XalanDOMString&		str,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver)
{
	assert(m_xsltProcessor != 0);

	if (length(str) > 0)
	{
		m_variablesStack.pushVariable(
			name,
			m_xsltProcessor->evalXPathStr(
							str,
							contextNode,
							resolver,
							*this),
			element);
	}
}



void
StylesheetExecutionContextDefault::pushVariable(
			const XalanQName&			name,
			const XObjectPtr			val,
			const ElemTemplateElement*	element)
{
	m_variablesStack.pushVariable(name, val, element);
}



void
StylesheetExecutionContextDefault::pushVariable(
			const XalanQName&			name,
			const ElemVariable*			var,
			const ElemTemplateElement*	element)
{
	m_variablesStack.pushVariable(name, var, element);
}



void
StylesheetExecutionContextDefault::pushVariable(
			const XalanQName&			name,
			const ElemTemplateElement*	element,
			const XPath&				xpath,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver)
{
	m_variablesStack.pushVariable(name, xpath.execute(contextNode, resolver, *this), element);
}



void
StylesheetExecutionContextDefault::pushVariable(
			const XalanQName&			name,
			const ElemTemplateElement*	element,
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceNode)
{
	m_variablesStack.pushVariable(name, createXResultTreeFrag(templateChild, sourceNode), element);
}



void
StylesheetExecutionContextDefault::pushContextMarker()
{
	m_variablesStack.pushContextMarker();
}



void
StylesheetExecutionContextDefault::popContextMarker()
{
	m_variablesStack.popContextMarker();
}



void
StylesheetExecutionContextDefault::resolveTopLevelParams()
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->resolveTopLevelParams(*this);

	m_variablesStack.markGlobalStackFrame();
}



void
StylesheetExecutionContextDefault::clearTopLevelParams()
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->clearTopLevelParams();

	m_variablesStack.unmarkGlobalStackFrame();
}



void
StylesheetExecutionContextDefault::pushParams(
			const ElemTemplateElement&	xslCallTemplateElement,
			XalanNode*					sourceNode,
			const ElemTemplateElement*	targetTemplate)
{
	// We have a params vector that we reuse, but occasionally, a
	// param will result in recursive execution, so we'll use a
	// temporary when we detect such a situation.
	if(m_paramsVector.empty() == true)
	{
		// This will ensure that the contents of m_paramsVector are
		// cleared.
		CollectionClearGuard<ParamsVectorType>	theGuard(m_paramsVector);

		// Make sure we have the default capacity for the params
		// vector...
		if (m_paramsVector.capacity() == 0)
		{
			 m_paramsVector.reserve(eDefaultParamsVectorSize);
		}

		getParams(xslCallTemplateElement, sourceNode, m_paramsVector);

		m_variablesStack.pushParams(
					m_paramsVector,
					targetTemplate);
	}
	else
	{
		ParamsVectorType	tempParams;

		getParams(xslCallTemplateElement, sourceNode, tempParams);

		m_variablesStack.pushParams(
					tempParams,
					targetTemplate);
	}
}



const XObjectPtr
StylesheetExecutionContextDefault::getParamVariable(const XalanQName&	theName)
{
	bool				fFound;

	const XObjectPtr	theValue(m_variablesStack.getParamVariable(theName, *this, fFound));

	return theValue;
}



void
StylesheetExecutionContextDefault::pushElementFrame(const ElemTemplateElement*	elem)
{
	m_variablesStack.pushElementFrame(elem);
}



void
StylesheetExecutionContextDefault::popElementFrame(const ElemTemplateElement*	elem)
{
	m_variablesStack.popElementFrame(elem);
}



int
StylesheetExecutionContextDefault::getGlobalStackFrameIndex() const
{
	return m_variablesStack.getGlobalStackFrameIndex();
}



int
StylesheetExecutionContextDefault::getCurrentStackFrameIndex() const
{
	return m_variablesStack.getCurrentStackFrameIndex();
}



void
StylesheetExecutionContextDefault::setCurrentStackFrameIndex(int	currentStackFrameIndex)
{
	m_variablesStack.setCurrentStackFrameIndex(currentStackFrameIndex);
}



void
StylesheetExecutionContextDefault::startDocument()
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->startDocument();
}



void
StylesheetExecutionContextDefault::endDocument()
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->endDocument();

	cleanUpTransients();

	setFormatterListener(0);
}



void
StylesheetExecutionContextDefault::characters(
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->characters(ch, start, length);
}



void
StylesheetExecutionContextDefault::charactersRaw(
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->charactersRaw(ch, start, length);
}



void
StylesheetExecutionContextDefault::comment(const XalanDOMChar*	data)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->comment(data);
}



void
StylesheetExecutionContextDefault::processingInstruction(
			const XalanDOMChar*		target,
			const XalanDOMChar*		data)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->processingInstruction(target, data);
}



void
StylesheetExecutionContextDefault::startElement(const XalanDOMChar*		name)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->startElement(name);
}



void
StylesheetExecutionContextDefault::endElement(const XalanDOMChar*	name)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->endElement(name);
}



void
StylesheetExecutionContextDefault::flushPending()
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->flushPending();
}



void
StylesheetExecutionContextDefault::cloneToResultTree(
			const XalanNode&			node,
			const ElemTemplateElement*	styleNode)
{
	m_xsltProcessor->cloneToResultTree(node, m_cloneTextNodesOnly, styleNode);
}



void
StylesheetExecutionContextDefault::cloneToResultTree(
			const XalanNode&			node,
			XalanNode::NodeType			nodeType,
			bool						isLiteral,
			bool						overrideStrip,
			bool						shouldCloneAttributes,
			const ElemTemplateElement*	styleNode)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->cloneToResultTree(
			node,
			nodeType,
			isLiteral,
			overrideStrip,
			shouldCloneAttributes,
			m_cloneTextNodesOnly,
			styleNode);
}



const XObjectPtr
StylesheetExecutionContextDefault::createXResultTreeFrag(
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceNode)
{
	assert(m_xsltProcessor != 0);

	XalanSourceTreeDocument* const	theDocument = m_usePerInstanceDocumentFactory == true ?
		m_documentAllocator.create(
			eDefaultAttributeAllocatorBlockSize,
			eDefaultAttributeNSAllocatorBlockSize,
			eDefaultCommentAllocatorBlockSize,
			eDefaultElementAllocatorBlockSize,
			eDefaultElementNSAllocatorBlockSize,
			eDefaultPIAllocatorBlockSize,
			eDefaultTextAllocatorBlockSize,
			eDefaultTextIWSAllocatorBlockSize) :
		getSourceTreeFactory();
	assert(theDocument != 0);

	XalanSourceTreeDocumentFragment* const	theDocumentFragment =
		m_documentFragmentAllocator.create(*theDocument);
	assert(theDocumentFragment != 0);

	ResultTreeFragBase* const	theResultTreeFrag =
		m_resultTreeFragAllocator.create(theDocumentFragment);
	assert(theResultTreeFrag != 0);

	GuardCachedObject<FormatterToSourceTreeCacheType>	theGuard(m_formatterToSourceTreeCache);

	FormatterToSourceTree* const	theFormatter = theGuard.get();
	assert(theFormatter != 0);

	theFormatter->setDocument(theDocument);

	theFormatter->setDocumentFragment(theDocumentFragment);

	theFormatter->setPrefixResolver(m_xsltProcessor);

	StylesheetExecutionContext::OutputContextPushPop	theOutputContextPushPop(
				*this,
				theFormatter);

	theFormatter->startDocument();

	templateChild.executeChildren(*this, sourceNode);

	theFormatter->endDocument();

	XResultTreeFrag* const	theXResultTreeFrag =
		m_xresultTreeFragAllocator.create(*theResultTreeFrag);

	theXResultTreeFrag->setExecutionContext(this);

	return XObjectPtr(theXResultTreeFrag);
}



void
StylesheetExecutionContextDefault::outputToResultTree(
			const XObject&				xobj,
			const ElemTemplateElement*	styleNode)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->outputToResultTree(xobj, m_cloneTextNodesOnly, styleNode);
}



void
StylesheetExecutionContextDefault::outputResultTreeFragment(
			const XObject&				theTree,
			const ElemTemplateElement*	styleNode)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->outputResultTreeFragment(theTree, m_cloneTextNodesOnly, styleNode);
}



const XalanDOMString&
StylesheetExecutionContextDefault::getXSLNameSpaceURL() const
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->getXSLNameSpaceURL();
}



const XalanDOMString&
StylesheetExecutionContextDefault::getXalanXSLNameSpaceURL() const
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->getXalanXSLNameSpaceURL();
}



bool
StylesheetExecutionContextDefault::getTraceSelects() const
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->getTraceSelects();
}



void
StylesheetExecutionContextDefault::traceSelect(
			const ElemTemplateElement&	theTemplate,
			const NodeRefListBase&		nl,
			const XPath*				xpath)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->traceSelect(*this, theTemplate, nl, xpath);
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
	if (findOnElementRecursionStack(theElement) == true)
	{
		XalanDOMString	theMessage(XALAN_STATIC_UCODE_STRING("Infinite recursion detected for element: "));

		theMessage += theElement->getNodeName();

		throw XSLTProcessorException(theMessage);
	}

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



bool
StylesheetExecutionContextDefault::returnXResultTreeFrag(XResultTreeFrag*	theXResultTreeFrag)
{
	assert(theXResultTreeFrag != 0);

	if (m_xresultTreeFragAllocator.ownsObject(theXResultTreeFrag) == false)
	{
		return false;
	}
	else
	{
		ResultTreeFragBase* const	theResultTreeFragBase =
			theXResultTreeFrag->release();

		m_xresultTreeFragAllocator.destroy(theXResultTreeFrag);

		ResultTreeFrag* const	theResultTreeFrag =
#if defined(XALAN_OLD_STYLE_CASTS)
			(ResultTreeFrag*)theResultTreeFragBase;
#else
			static_cast<ResultTreeFrag*>(theResultTreeFragBase);
#endif

		XalanSourceTreeDocumentFragment* const	theDocumentFragment =
			theResultTreeFrag->getDocumentFragment();
		assert(theDocumentFragment != 0);

		m_resultTreeFragAllocator.destroy(theResultTreeFrag);

		if (m_usePerInstanceDocumentFactory == true)
		{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
#if defined(XALAN_OLD_STYLE_CASTS)
			m_documentAllocator.destroy((XalanSourceTreeDocument*)theDocumentFragment->getOwnerDocument());
#else
			m_documentAllocator.destroy(static_cast<XalanSourceTreeDocument*>(theDocumentFragment->getOwnerDocument()));
#endif
#else
			m_documentAllocator.destroy(theDocumentFragment->getOwnerDocument());
#endif
		}

		m_documentFragmentAllocator.destroy(theDocumentFragment);

        return true;
	}
}



StylesheetExecutionContextDefault::eEscapeURLs
StylesheetExecutionContextDefault::getEscapeURLs() const
{
	return m_escapeURLs;
}



void
StylesheetExecutionContextDefault::setEscapeURLs(eEscapeURLs	value)
{
	m_escapeURLs = value;
}



StylesheetExecutionContextDefault::eOmitMETATag
StylesheetExecutionContextDefault::getOmitMETATag() const
{
	return m_omitMETATag;
}



void
StylesheetExecutionContextDefault::setOmitMETATag(eOmitMETATag	value)
{
	m_omitMETATag = value;
}



FormatterToXML*
StylesheetExecutionContextDefault::createFormatterToXML(
			Writer&					writer,
			const XalanDOMString&	version,
			bool					doIndent,
			int						indent,
			const XalanDOMString&	encoding,
			const XalanDOMString&	mediaType,
			const XalanDOMString&	doctypeSystem,
			const XalanDOMString&	doctypePublic,
			bool					xmlDecl,
			const XalanDOMString&	standalone)
{
	FormatterToXML* const	theFormatter =
		new FormatterToXML(
			writer,
			version,
			doIndent,
			indent,
			encoding,
			mediaType,
			doctypeSystem,
			doctypePublic,
			xmlDecl,
			standalone);

	m_formatterListeners.push_back(theFormatter);

	return theFormatter;
}



FormatterToHTML*
StylesheetExecutionContextDefault::createFormatterToHTML(
			Writer&					writer,
			const XalanDOMString&	encoding,
			const XalanDOMString&	mediaType,
			const XalanDOMString&	doctypeSystem,
			const XalanDOMString&	doctypePublic,
			bool					doIndent,
			int						indent,
			bool					escapeURLs,
			bool					omitMetaTag)
{
	FormatterToHTML* const	theFormatter =
		new FormatterToHTML(
			writer,
			encoding,
			mediaType,
			doctypeSystem,
			doctypePublic,
			doIndent,
			indent,
			escapeURLs,
			omitMetaTag);

	// Check to see if the user has asked us to ignore
	// namespaces in HTML output.
	if (m_ignoreHTMLElementNamespaces == false)
	{
		// Nope, so give the formatter a prefix resolver...
		theFormatter->setPrefixResolver(m_xsltProcessor);
	}

	m_formatterListeners.push_back(theFormatter);

	return theFormatter;
}



FormatterToDOM*
StylesheetExecutionContextDefault::createFormatterToDOM(
			XalanDocument*			doc,
			XalanDocumentFragment*	docFrag,
			XalanElement*			currentElement)
{
	FormatterToDOM* const	theFormatter =
		new FormatterToDOM(
			doc,
			docFrag,
			currentElement);

	m_formatterListeners.push_back(theFormatter);

	theFormatter->setPrefixResolver(m_xsltProcessor);

	return theFormatter;
}



FormatterToDOM*
StylesheetExecutionContextDefault::createFormatterToDOM(
			XalanDocument*	doc,
			XalanElement*	elem)
{
	FormatterToDOM* const	theFormatter =
		new FormatterToDOM(
			doc,
			elem);

	m_formatterListeners.push_back(theFormatter);

	theFormatter->setPrefixResolver(m_xsltProcessor);

	return theFormatter;
}



FormatterToText*
StylesheetExecutionContextDefault::createFormatterToText(
			Writer&					writer,
			const XalanDOMString&	encoding)
{
	FormatterToText* const	theFormatter =
		new FormatterToText(writer, encoding);

	m_formatterListeners.push_back(theFormatter);

	return theFormatter;
}



FormatterToText*
StylesheetExecutionContextDefault::borrowFormatterToText()
{
	return m_formatterToTextCache.get();
}



bool
StylesheetExecutionContextDefault::returnFormatterToText(FormatterToText*	theFormatter)
{
	return m_formatterToTextCache.release(theFormatter);
}



NodeSorter*
StylesheetExecutionContextDefault::borrowNodeSorter()
{
	return m_nodeSorterCache.get();
}



bool
StylesheetExecutionContextDefault::returnNodeSorter(NodeSorter*		theSorter)
{
	return m_nodeSorterCache.release(theSorter);
}



StylesheetExecutionContextDefault::XalanNumberFormatAutoPtr
StylesheetExecutionContextDefault::createXalanNumberFormat()
{
	return XalanNumberFormatAutoPtr(s_xalanNumberFormatFactory->create());
}



StylesheetExecutionContextDefault::XalanNumberFormatFactory::XalanNumberFormatFactory()
{
}



StylesheetExecutionContextDefault::XalanNumberFormatFactory::~XalanNumberFormatFactory()
{
}



XalanNumberFormat*
StylesheetExecutionContextDefault::XalanNumberFormatFactory::create()
{
	return new XalanNumberFormat();
}



StylesheetExecutionContextDefault::XalanNumberFormatFactory*
StylesheetExecutionContextDefault::installXalanNumberFormatFactory(XalanNumberFormatFactory*	theFactory)
{
	XalanNumberFormatFactory* const		theOldFactory =
		s_xalanNumberFormatFactory;

	if (theFactory == 0)
	{
		s_xalanNumberFormatFactory = &s_defaultXalanNumberFormatFactory;
	}
	else
	{
		s_xalanNumberFormatFactory = theFactory;
	}

	return theOldFactory;
}



int
StylesheetExecutionContextDefault::collationCompare(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS,
			eCaseOrder				theCaseOrder)
{
	if (m_collationCompareFunctor == 0)
	{
		return s_defaultCollationFunctor(c_wstr(theLHS), c_wstr(theRHS), theCaseOrder);
	}
	else
	{
		return (*m_collationCompareFunctor)(c_wstr(theLHS), c_wstr(theRHS), theCaseOrder);
	}
}



int
StylesheetExecutionContextDefault::collationCompare(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS,
			const XalanDOMString&	theLocale,
			eCaseOrder				theCaseOrder)
{
	if (m_collationCompareFunctor == 0)
	{
		return s_defaultCollationFunctor(c_wstr(theLHS), c_wstr(theRHS), c_wstr(theLocale), theCaseOrder);
	}
	else
	{
		return (*m_collationCompareFunctor)(c_wstr(theLHS), c_wstr(theRHS), c_wstr(theLocale), theCaseOrder);
	}
}



int
StylesheetExecutionContextDefault::collationCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			eCaseOrder				theCaseOrder)
{
	assert(theLHS != 0 && theRHS != 0);

	if (m_collationCompareFunctor == 0)
	{
		return s_defaultCollationFunctor(theLHS, theRHS, theCaseOrder);
	}
	else
	{
		return (*m_collationCompareFunctor)(theLHS, theRHS, theCaseOrder);
	}
}



int
StylesheetExecutionContextDefault::collationCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			const XalanDOMChar*		theLocale,
			eCaseOrder				theCaseOrder)
{
	assert(theLHS != 0 && theRHS != 0);

	if (m_collationCompareFunctor == 0)
	{
		return s_defaultCollationFunctor(theLHS, theRHS, theLocale, theCaseOrder);
	}
	else
	{
		return (*m_collationCompareFunctor)(theLHS, theRHS, theLocale, theCaseOrder);
	}
}



StylesheetExecutionContextDefault::CollationCompareFunctor::CollationCompareFunctor()
{
}



StylesheetExecutionContextDefault::CollationCompareFunctor::~CollationCompareFunctor()
{
}



StylesheetExecutionContextDefault::DefaultCollationCompareFunctor::DefaultCollationCompareFunctor()
{
}



StylesheetExecutionContextDefault::DefaultCollationCompareFunctor::~DefaultCollationCompareFunctor()
{
}



int
StylesheetExecutionContextDefault::DefaultCollationCompareFunctor::operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			eCaseOrder				/* theCaseOrder */) const
{
	return ::collationCompare(theLHS, theRHS);
}



int
StylesheetExecutionContextDefault::DefaultCollationCompareFunctor::operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			const XalanDOMChar*		/* theLocale */,
			eCaseOrder				theCaseOrder) const
{
	return (*this)(theLHS, theRHS, theCaseOrder);
}



const StylesheetExecutionContextDefault::CollationCompareFunctor*
StylesheetExecutionContextDefault::installCollationCompareFunctor(CollationCompareFunctor*	theFunctor)
{
	assert(theFunctor != 0);

	const CollationCompareFunctor* const	temp = m_collationCompareFunctor;

	m_collationCompareFunctor = theFunctor;

	return temp;
}



StylesheetExecutionContextDefault::CollationCompareFunctor*
StylesheetExecutionContextDefault::uninstallCollationCompareFunctor()
{
	if (m_collationCompareFunctor == 0)
	{
		return 0;
	}
	else
	{
		CollationCompareFunctor* const	temp = m_collationCompareFunctor;

		m_collationCompareFunctor = 0;

		return temp;
	}
}



bool
StylesheetExecutionContextDefault::getInConstruction(const KeyDeclaration&	keyDeclaration) const
{
	return m_keyDeclarationSet.count(&keyDeclaration) != 0 ? true : false;
}



void
StylesheetExecutionContextDefault::beginConstruction(const KeyDeclaration&	keyDeclaration)
{	
	m_keyDeclarationSet.insert(&keyDeclaration);
}

	

void
StylesheetExecutionContextDefault::endConstruction(const KeyDeclaration&	keyDeclaration)
{
	m_keyDeclarationSet.erase(&keyDeclaration);
}



void
StylesheetExecutionContextDefault::reset()
{
	assert(m_elementRecursionStack.empty() == true);

	m_variablesStack.reset();

	if (m_xsltProcessor != 0)
	{
		m_xsltProcessor->reset();
	}

	m_mode = 0;
	m_currentTemplate = 0;

	m_formatterToTextCache.reset();
	m_formatterToSourceTreeCache.reset();
	m_nodeSorterCache.reset();
	m_documentAllocator.reset();
	m_documentFragmentAllocator.reset();
	m_resultTreeFragAllocator.reset();
	m_xresultTreeFragAllocator.reset();

	// Just in case endDocument() was not called,
	// clean things up...
	cleanUpTransients();

	// Destroy the source tree factory, which
	// will destroy all result tree fragment nodes
	// that were generated...
	m_sourceTreeResultTreeFactory.reset();

	// Reset the default execution context...
	m_xpathExecutionContextDefault.reset();

	m_cloneTextNodesOnly = false;
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



XObjectPtr
StylesheetExecutionContextDefault::createNodeSet(XalanNode&	theNode)
{
	return m_xpathExecutionContextDefault.createNodeSet(theNode);
}



bool
StylesheetExecutionContextDefault::isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const
{
	return m_xpathExecutionContextDefault.isNodeAfter(node1, node2);
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



StylesheetExecutionContextDefault::size_type
StylesheetExecutionContextDefault::getContextNodeListLength() const
{
	return m_xpathExecutionContextDefault.getContextNodeListLength();
}



StylesheetExecutionContextDefault::size_type
StylesheetExecutionContextDefault::getContextNodeListPosition(const XalanNode&	contextNode) const
{
	return m_xpathExecutionContextDefault.getContextNodeListPosition(contextNode);
}



bool
StylesheetExecutionContextDefault::elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const
{
	if (equals(theNamespace, XSLTEngineImpl::getXSLNameSpaceURL()) == true)
	{
		const int	xslToken = XSLTEngineImpl::getElementToken(elementName);

		return xslToken == -2 ? false : true;
	}
	else
	{
		return m_xpathExecutionContextDefault.elementAvailable(theNamespace, elementName);
	}
}



bool
StylesheetExecutionContextDefault::functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const
{
	return m_xpathExecutionContextDefault.functionAvailable(theNamespace, functionName);
}



const XObjectPtr
StylesheetExecutionContextDefault::extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec,
			const Locator*					locator)
{
	return m_xpathExecutionContextDefault.extFunction(theNamespace, functionName, context, argVec, locator);
}



XalanDocument*
StylesheetExecutionContextDefault::parseXML(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) const
{
	return m_xpathExecutionContextDefault.parseXML(urlString, base);
}



MutableNodeRefList*
StylesheetExecutionContextDefault::borrowMutableNodeRefList()
{
	return m_xpathExecutionContextDefault.borrowMutableNodeRefList();
}



bool
StylesheetExecutionContextDefault::returnMutableNodeRefList(MutableNodeRefList*		theList)
{
	return m_xpathExecutionContextDefault.returnMutableNodeRefList(theList);
}



MutableNodeRefList*
StylesheetExecutionContextDefault::createMutableNodeRefList() const
{
	return m_xpathExecutionContextDefault.createMutableNodeRefList();
}



XalanDOMString&
StylesheetExecutionContextDefault::getCachedString()
{
	return m_xpathExecutionContextDefault.getCachedString();
}



bool
StylesheetExecutionContextDefault::releaseCachedString(XalanDOMString&	theString)
{
	return m_xpathExecutionContextDefault.releaseCachedString(theString);
}



void
StylesheetExecutionContextDefault::getNodeSetByKey(
			XalanNode*				doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const PrefixResolver&	resolver,
			MutableNodeRefList&		nodelist)
{
	assert(m_stylesheetRoot != 0);

	m_stylesheetRoot->getNodeSetByKey(doc, name, ref, resolver, nodelist, *this, m_keyTables);
}



const XObjectPtr
StylesheetExecutionContextDefault::getVariable(
			const XalanQName&	name,
			const Locator*		locator)
{
	bool				fFound;

	const XObjectPtr	theValue(m_variablesStack.getVariable(name, *this, fFound));

	if(fFound == true)
	{
		assert(theValue.null() == false);

		return theValue;
	}
	else
	{
		error(
			TranscodeFromLocalCodePage("Variable reference given for variable out of context or without definition!  Name = '") +
			name.getLocalPart() + "'",
			getCurrentNode(),
			locator);

		return getXObjectFactory().createUnknown(name.getLocalPart());
	}
}



const PrefixResolver*
StylesheetExecutionContextDefault::getPrefixResolver() const
{
	return m_xpathExecutionContextDefault.getPrefixResolver();
}



void
StylesheetExecutionContextDefault::setPrefixResolver(const PrefixResolver*	thePrefixResolver)
{
	m_xpathExecutionContextDefault.setPrefixResolver(thePrefixResolver);
}



const XalanDOMString*
StylesheetExecutionContextDefault::getNamespaceForPrefix(const XalanDOMString&	prefix) const
{
	return m_xpathExecutionContextDefault.getNamespaceForPrefix(prefix);
}



XalanDOMString
StylesheetExecutionContextDefault::findURIFromDoc(const XalanDocument*	owner) const
{
	return m_xpathExecutionContextDefault.findURIFromDoc(owner);
}



const XalanDOMString&
StylesheetExecutionContextDefault::getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const
{
	return m_xpathExecutionContextDefault.getUnparsedEntityURI(theName, theDocument);
}



bool
StylesheetExecutionContextDefault::shouldStripSourceNode(const XalanNode&	node)
{
	if (m_xsltProcessor == 0)
	{
		return m_xpathExecutionContextDefault.shouldStripSourceNode(node);
	}
	else
	{
		return m_xsltProcessor->shouldStripSourceNode(*this, node);
	}
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



const XalanDecimalFormatSymbols*
StylesheetExecutionContextDefault::getDecimalFormatSymbols(const XalanQName&	qname)
{
	if (m_stylesheetRoot == 0)
	{
		return m_xpathExecutionContextDefault.getDecimalFormatSymbols(qname);
	}
	else
	{
		return m_stylesheetRoot->getDecimalFormatSymbols(qname);
	}
}



PrintWriter*
StylesheetExecutionContextDefault::createPrintWriter(XalanOutputStream*	theTextOutputStream)
{
	assert(theTextOutputStream != 0);

	PrintWriter* const	thePrintWriter =
		new XalanOutputStreamPrintWriter(*theTextOutputStream);

	m_printWriters.push_back(thePrintWriter);

	return thePrintWriter;
}



PrintWriter*
StylesheetExecutionContextDefault::createPrintWriter(
			const XalanDOMString&		theFileName,
			const XalanDOMString&		/* theEncoding */)
{
	XalanOutputStream* const	theOutputStream =
		new XalanFileOutputStream(theFileName);

	m_outputStreams.push_back(theOutputStream);

	return createPrintWriter(theOutputStream);
}



PrintWriter*
#if defined(XALAN_NO_NAMESPACES)
StylesheetExecutionContextDefault::createPrintWriter(ostream&	theStream)
#else
StylesheetExecutionContextDefault::createPrintWriter(std::ostream&	theStream)
#endif
{
	XalanOutputStream* const		theOutputStream =
		new XalanStdOutputStream(theStream);

	m_outputStreams.push_back(theOutputStream);

	return createPrintWriter(theOutputStream);
}



CountersTable&
StylesheetExecutionContextDefault::getCountersTable()
{
	return m_countersTable;
}



void
StylesheetExecutionContextDefault::characters(const XalanNode&	node)
{
	m_xsltProcessor->characters(node);
}


void
StylesheetExecutionContextDefault::characters(const XObjectPtr&		xobject)
{
	m_xsltProcessor->characters(xobject);
}



void
StylesheetExecutionContextDefault::charactersRaw(const XalanNode&	node)
{
	m_xsltProcessor->charactersRaw(node);
}



void
StylesheetExecutionContextDefault::charactersRaw(const XObjectPtr&	xobject)
{
	m_xsltProcessor->charactersRaw(xobject);
}



StylesheetExecutionContextDefault::size_type
StylesheetExecutionContextDefault::getTraceListeners() const
{
	assert(m_xsltProcessor != 0);

	return m_xsltProcessor->getTraceListeners();
}



void
StylesheetExecutionContextDefault::fireGenerateEvent(const GenerateEvent&	ge)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->fireGenerateEvent(ge);
}



void
StylesheetExecutionContextDefault::fireTraceEvent(const TracerEvent&	te)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->fireTraceEvent(te);
}



void
StylesheetExecutionContextDefault::fireSelectEvent(const SelectionEvent&	se)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->fireSelectEvent(se);
}



void
StylesheetExecutionContextDefault::error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const XalanNode*		styleNode) const
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->error(msg, sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator*			locator) const
{
	assert(m_xsltProcessor != 0);

	if (locator != 0)
	{
		m_xsltProcessor->error(msg, *locator, sourceNode);
	}
	else
	{
		m_xsltProcessor->error(msg, sourceNode);
	}
}



void
StylesheetExecutionContextDefault::error(
			const char*			msg,
			const XalanNode*	sourceNode,
			const XalanNode*	styleNode) const
{
	error(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::error(
			const char*			msg,
			const XalanNode*	sourceNode,
			const Locator*		locator) const
{
	error(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



void
StylesheetExecutionContextDefault::warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const XalanNode*		styleNode) const
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->warn(msg, sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator*			locator) const
{
	assert(m_xsltProcessor != 0);

	if (locator != 0)
	{
		m_xsltProcessor->warn(msg, *locator, sourceNode);
	}
	else
	{
		m_xsltProcessor->warn(msg, sourceNode);
	}
}



void
StylesheetExecutionContextDefault::warn(
			const char*			msg,
			const XalanNode*	sourceNode,
			const XalanNode*	styleNode) const
{
	warn(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::warn(
			const char*			msg,
			const XalanNode*	sourceNode,
			const Locator*		locator) const
{
	warn(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



void
StylesheetExecutionContextDefault::message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const XalanNode*		styleNode) const
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->message(msg, sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator*			locator) const
{
	assert(m_xsltProcessor != 0);

	if (locator != 0)
	{
		m_xsltProcessor->message(msg, *locator, sourceNode);
	}
	else
	{
		m_xsltProcessor->message(msg, sourceNode);
	}
}



void
StylesheetExecutionContextDefault::message(
			const char*			msg,
			const XalanNode*	sourceNode,
			const XalanNode*	styleNode) const
{
	message(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::message(
			const char*			msg,
			const XalanNode*	sourceNode,
			const Locator*		locator) const
{
	message(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



class PopAndPushContextMarker
{
public:

	PopAndPushContextMarker(StylesheetExecutionContext&	theExecutionContext) :
		m_executionContext(theExecutionContext)
	{
		m_executionContext.popContextMarker();
	}

	~PopAndPushContextMarker()
	{
		m_executionContext.pushContextMarker();
	}

private:

	StylesheetExecutionContext&		m_executionContext;
};



void
StylesheetExecutionContextDefault::getParams(
			const ElemTemplateElement&	xslCallTemplateElement,
			XalanNode*					sourceNode,
			ParamsVectorType&			params)
{
	assert(params.empty() == true);

	const ElemTemplateElement*	child =
			xslCallTemplateElement.getFirstChildElem();

	if (0 != child)
	{
		// This object will take care of popping, then
		// pushing the context marker at the top of the
		// stack, even if an exception is thrown.
		PopAndPushContextMarker		thePopPush(*this);

		while(0 != child)
		{
			if(Constants::ELEMNAME_WITHPARAM == child->getXSLToken())
			{
				const ElemWithParam* const	xslParamElement =
#if defined(XALAN_OLD_STYLE_CASTS)
						(ElemWithParam*)child;
#else
						static_cast<const ElemWithParam*>(child);
#endif

				const XPath* const	pxpath = xslParamElement->getSelectPattern();

				XObjectPtr	theXObject;

				if(0 != pxpath)
				{
					theXObject =
						createVariable(
							&xslCallTemplateElement,
							*pxpath,
							sourceNode,
							*xslParamElement);
				}
				else
				{
					theXObject =
						createVariable(
							&xslCallTemplateElement,
							*xslParamElement,
							sourceNode);
				}

				params.push_back(ParamsVectorType::value_type(&xslParamElement->getQName(), theXObject));
			}

			child = child->getNextSiblingElem();
		}
	}
}



XalanSourceTreeDocument*
StylesheetExecutionContextDefault::getSourceTreeFactory() const
{
	assert(m_xsltProcessor != 0);

	if(m_sourceTreeResultTreeFactory.get() == 0)
	{
#if defined(XALAN_NO_MUTABLE)
		((StylesheetExecutionContextDefault*)this)->m_sourceTreeResultTreeFactory.reset(
#else
		m_sourceTreeResultTreeFactory.reset(
#endif
			new XalanSourceTreeDocument);
	}

	return m_sourceTreeResultTreeFactory.get();
}



bool
StylesheetExecutionContextDefault::isCached(const XPath*	theXPath)
{
	XPathCacheMapType::const_iterator	i =
		m_matchPatternCache.begin();

	const XPathCacheMapType::const_iterator		theEnd =
		m_matchPatternCache.end();

	while(i != theEnd)
	{
		if ((*i).second.first == theXPath)
		{
			return true;
		}
		else
		{
			++i;
		}
	}

	return false;
}



void
StylesheetExecutionContextDefault::XPathCacheReturnFunctor::operator()(const XPathCacheMapType::value_type&		theCacheEntry)
{
	m_xsltProcessor.returnXPath(theCacheEntry.second.first);
}




void
StylesheetExecutionContextDefault::clearXPathCache()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	assert(m_matchPatternCache.empty() == true || m_xsltProcessor != 0);

	if (m_xsltProcessor != 0)
	{
		for_each(m_matchPatternCache.begin(),
				 m_matchPatternCache.end(),
				 XPathCacheReturnFunctor(*m_xsltProcessor));
	}

	m_matchPatternCache.clear();
}



void
StylesheetExecutionContextDefault::addToXPathCache(
			const XalanDOMString&	pattern,
			const XPath*			theXPath)
{
	assert(m_xsltProcessor != 0);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	ClockType	addClock = std::clock();
#else
	ClockType	addClock = clock();
#endif

	if (m_matchPatternCache.size() == eXPathCacheMax)
	{
		// OK, we need to clear something out of the cache...

		// Initialize the lowest clock time found so far
		// with the current clock...
		ClockType	lowest = addClock;

		// Get some iterators ready to search the cache...
		XPathCacheMapType::iterator		i =
			m_matchPatternCache.begin();

		const XPathCacheMapType::iterator	theEnd =
			m_matchPatternCache.end();

		XPathCacheMapType::iterator		earliest(theEnd);

		while(i != theEnd)
		{
			const ClockType		current = (*i).second.second;

			if (current < lowest)
			{
				// OK, found a lower clock time, so
				// update the everything...
				lowest = current;

				earliest = i;
			}
			else
			{
				++i;
			}
		}
		assert(earliest != theEnd);

		// Return the XPath and erase it from the cache.
		m_xsltProcessor->returnXPath((*earliest).second.first);

		m_matchPatternCache.erase(earliest);
	}

	// Add the XPath with the current clock
	m_matchPatternCache.insert(XPathCacheMapType::value_type(pattern, XPathCacheEntry(theXPath, addClock)));
}



void
StylesheetExecutionContextDefault::cleanUpTransients()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(m_formatterListeners.begin(),
			 m_formatterListeners.end(),
			 DeleteFunctor<FormatterListener>());

	m_formatterListeners.clear();

	for_each(m_printWriters.begin(),
			 m_printWriters.end(),
			 DeleteFunctor<PrintWriter>());

	m_printWriters.clear();

	for_each(m_outputStreams.begin(),
			 m_outputStreams.end(),
			 DeleteFunctor<XalanOutputStream>());

	m_outputStreams.clear();

	// Clean up the key table vector
	for_each(m_keyTables.begin(),
			 m_keyTables.end(),
			 makeMapValueDeleteFunctor(m_keyTables));

	m_keyTables.clear();

	m_countersTable.reset();

	// Clear any cached XPaths...
	clearXPathCache();

	assert(m_matchPatternCache.empty() == true);
}
