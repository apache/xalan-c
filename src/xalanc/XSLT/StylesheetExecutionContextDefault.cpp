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

#include "StylesheetExecutionContextDefault.hpp"



#include <algorithm>
#include <cassert>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanNumberFormat.hpp>
#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanFileOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanFStreamOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathEnvSupport.hpp>
#include <xalanc/XPath/XPathExecutionContext.hpp>
#include <xalanc/XPath/XObject.hpp>



#include <xalanc/XMLSupport/FormatterToXML.hpp>
#include <xalanc/XMLSupport/FormatterToXML_UTF8.hpp>
#include <xalanc/XMLSupport/FormatterToXML_UTF16.hpp>
#include <xalanc/XMLSupport/FormatterToHTML.hpp>
#include <xalanc/XMLSupport/XMLParserLiaison.hpp>



#include <xalanc/XalanSourceTree/FormatterToSourceTree.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDocument.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDocumentFragment.hpp>



#include "Constants.hpp"
#include "ElemTemplateElement.hpp"
#include "ElemWithParam.hpp"
#include "KeyTable.hpp"
#include "StylesheetConstructionContextDefault.hpp"
#include "StylesheetRoot.hpp"
#include "XSLTEngineImpl.hpp"
#include "XSLTProcessorException.hpp"



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "C:/Program Files/Rational/Quantify/pure.h"
#endif



XALAN_CPP_NAMESPACE_BEGIN



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
	m_stylesheetRoot(0),
	m_formatterListeners(),
	m_printWriters(),
	m_outputStreams(),
	m_collationCompareFunctor(0),
	m_formatNumberFunctor(0),
	m_variablesStack(),
	m_matchPatternCache(),
	m_keyTables(),
	m_keyDeclarationSet(),
	m_countersTable(),
	m_sourceTreeResultTreeFactory(),
	m_mode(0),
	m_currentTemplateStack(),
	m_formatterToTextCache(),
	m_formatterToSourceTreeCache(),
	m_nodeSorterCache(),
	m_indentAmount(-1),
	m_xresultTreeFragAllocator(eXResultTreeFragAllocatorBlockSize),
	m_documentFragmentAllocator(eDocumentFragmentAllocatorBlockSize),
	m_documentAllocator(eDocumentAllocatorBlockSize),
	m_usePerInstanceDocumentFactory(false),
	m_cloneTextNodesOnly(false),
	m_escapeURLs(eEscapeURLsDefault),
	m_omitMETATag(eOmitMETATagDefault),
	m_hasStripOrPreserveSpace(false)
{
    m_currentTemplateStack.push_back(0);
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
	m_stylesheetRoot(0),
	m_formatterListeners(),
	m_printWriters(),
	m_outputStreams(),
	m_collationCompareFunctor(0),
	m_formatNumberFunctor(0),
	m_variablesStack(),
	m_matchPatternCache(),
	m_keyTables(),
	m_keyDeclarationSet(),
	m_countersTable(),
	m_sourceTreeResultTreeFactory(),
	m_mode(0),
	m_currentTemplateStack(),
	m_formatterToTextCache(),
	m_formatterToSourceTreeCache(),
	m_nodeSorterCache(),
	m_indentAmount(-1),
	m_xresultTreeFragAllocator(eXResultTreeFragAllocatorBlockSize),
	m_documentFragmentAllocator(eDocumentFragmentAllocatorBlockSize),
	m_documentAllocator(eDocumentAllocatorBlockSize),
	m_usePerInstanceDocumentFactory(false),
	m_cloneTextNodesOnly(false),
	m_escapeURLs(eEscapeURLsDefault),
	m_hasStripOrPreserveSpace(false)
{
    m_currentTemplateStack.push_back(0);
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



void
StylesheetExecutionContextDefault::setStylesheetRoot(const StylesheetRoot*	theStylesheet)
{
	assert(m_xsltProcessor != 0);

	m_stylesheetRoot = theStylesheet;

	m_xsltProcessor->setStylesheetRoot(theStylesheet);

	if (theStylesheet == 0)
	{
		m_xsltProcessor->setExecutionContext(0);
	}
	else
	{
		m_xsltProcessor->setExecutionContext(this);

		m_hasStripOrPreserveSpace = theStylesheet->hasPreserveOrStripSpaceElements();

		m_countersTable.resize(theStylesheet->getElemNumberCount());
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
	return m_currentTemplateStack.back();
}



void
StylesheetExecutionContextDefault::pushCurrentTemplate(const ElemTemplate*	theTemplate)
{		
	m_currentTemplateStack.push_back(theTemplate);
}



void
StylesheetExecutionContextDefault::popCurrentTemplate()
{		
	m_currentTemplateStack.pop_back();
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
StylesheetExecutionContextDefault::addResultAttribute(
			const XalanDOMString&	aname,
			const XalanDOMChar*		value)
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
			const XPath&				xpath,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver)
{
	XalanNode* const	theCurrentNode = getCurrentNode();
			
	if (theCurrentNode == contextNode)
	{
		return xpath.execute(resolver, *this);
	}
	else
	{
		return xpath.execute(contextNode, resolver, *this);
	}
}



const XObjectPtr
StylesheetExecutionContextDefault::createVariable(
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
StylesheetExecutionContextDefault::pushParams(const ElemTemplateElement&	xslCallTemplateElement)
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

		getParams(xslCallTemplateElement, m_paramsVector);

		m_variablesStack.pushParams(m_paramsVector);
	}
	else
	{
		ParamsVectorType	tempParams;

		getParams(xslCallTemplateElement, tempParams);

		m_variablesStack.pushParams(tempParams);
	}
}



const XObjectPtr
StylesheetExecutionContextDefault::getParamVariable(const XalanQName&	theName)
{
	bool	fFound;

	return m_variablesStack.getParamVariable(theName, *this, fFound);
}



void
StylesheetExecutionContextDefault::pushElementFrame(const ElemTemplateElement*	elem)
{
	m_variablesStack.pushElementFrame(elem);
}



void
StylesheetExecutionContextDefault::popElementFrame()
{
	m_variablesStack.popElementFrame();
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
			const XalanNode&	node,
			const LocatorType*	locator)
{
	m_xsltProcessor->cloneToResultTree(node, m_cloneTextNodesOnly, locator);
}



void
StylesheetExecutionContextDefault::cloneToResultTree(
			const XalanNode&		node,
			XalanNode::NodeType		nodeType,
			bool					overrideStrip,
			bool					shouldCloneAttributes,
			const LocatorType*		locator)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->cloneToResultTree(
			node,
			nodeType,
			overrideStrip,
			shouldCloneAttributes,
			m_cloneTextNodesOnly,
			locator);
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
		m_xresultTreeFragAllocator.create(*theDocumentFragment);

	theXResultTreeFrag->setExecutionContext(this);

	return XObjectPtr(theXResultTreeFrag);
}



void
StylesheetExecutionContextDefault::outputToResultTree(
			const XObject&		xobj,
			const LocatorType*	locator)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->outputToResultTree(xobj, m_cloneTextNodesOnly, locator);
}



void
StylesheetExecutionContextDefault::outputResultTreeFragment(
			const XObject&		theTree,
			const LocatorType*	locator)
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->outputResultTreeFragment(theTree, m_cloneTextNodesOnly, locator);
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
	assert(theElement != 0);

	XALAN_USING_STD(find)

	const ElementTemplateElementStackType::const_iterator	i =
				find(m_elementRecursionStack.begin(),
					 m_elementRecursionStack.end(),
					 theElement);

	return i == m_elementRecursionStack.end() ? false : true;
}



void
StylesheetExecutionContextDefault::pushOnElementRecursionStack(const ElemTemplateElement*	theElement)
{
	assert(theElement != 0);

	if (findOnElementRecursionStack(theElement) == true)
	{
		const LocatorType* const	theLocator = theElement->getLocator();

		if (theLocator == 0)
		{
			throw XSLTProcessorException(
					XalanMessageLoader::getMessage(
						XalanMessages::InfiniteRecursion_1Param,
							theElement->getElementName()));
		}
		else
		{
			throw XSLTProcessorException(
					*theLocator,
					XalanMessageLoader::getMessage(
						XalanMessages::InfiniteRecursion_1Param,
							theElement->getElementName()));
		}
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
		XalanDocumentFragment* const	theDocumentFragment =
			theXResultTreeFrag->release();

		m_xresultTreeFragAllocator.destroy(theXResultTreeFrag);

		if (m_usePerInstanceDocumentFactory == true)
		{
#if defined(XALAN_OLD_STYLE_CASTS)
			m_documentAllocator.destroy((XalanSourceTreeDocument*)theDocumentFragment->getOwnerDocument());
#else
			m_documentAllocator.destroy(static_cast<XalanSourceTreeDocument*>(theDocumentFragment->getOwnerDocument()));
#endif
		}

#if defined(XALAN_OLD_STYLE_CASTS)
		m_documentFragmentAllocator.destroy((XalanSourceTreeDocumentFragment*)theDocumentFragment);
#else
		m_documentFragmentAllocator.destroy(static_cast<XalanSourceTreeDocumentFragment*>(theDocumentFragment));
#endif

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



FormatterListener*
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
	if (doIndent == false &&
		(encoding.empty() == true || XalanTranscodingServices::encodingIsUTF8(encoding)))
	{
		FormatterToXML_UTF8* const	theFormatter =
			new FormatterToXML_UTF8(
				writer,
				version,
				mediaType,
				doctypeSystem,
				doctypePublic,
				xmlDecl,
				standalone);

		m_formatterListeners.push_back(theFormatter);

		return theFormatter;
	}
	else if (doIndent == false && XalanTranscodingServices::encodingIsUTF16(encoding))
	{
		FormatterToXML_UTF16* const	theFormatter =
			new FormatterToXML_UTF16(
				writer,
				version,
				mediaType,
				doctypeSystem,
				doctypePublic,
				xmlDecl,
				standalone);

		m_formatterListeners.push_back(theFormatter);

		return theFormatter;
	}
	else
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
}



FormatterListener*
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

	m_formatterListeners.push_back(theFormatter);

    theFormatter->setPrefixResolver(m_xsltProcessor);

	return theFormatter;
}



FormatterListener*
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
			const XalanDOMString&				theLHS,
			const XalanDOMString&				theRHS,
			XalanCollationServices::eCaseOrder	theCaseOrder)
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
			const XalanDOMString&				theLHS,
			const XalanDOMString&				theRHS,
			const XalanDOMString&				theLocale,
			XalanCollationServices::eCaseOrder	theCaseOrder)
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
			const XalanDOMChar*					theLHS,
			const XalanDOMChar*					theRHS,
			XalanCollationServices::eCaseOrder	theCaseOrder)
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
			const XalanDOMChar*					theLHS,
			const XalanDOMChar*					theRHS,
			const XalanDOMChar*					theLocale,
			XalanCollationServices::eCaseOrder	theCaseOrder)
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



StylesheetExecutionContextDefault::DefaultCollationCompareFunctor::DefaultCollationCompareFunctor()
{
}



StylesheetExecutionContextDefault::DefaultCollationCompareFunctor::~DefaultCollationCompareFunctor()
{
}



int
StylesheetExecutionContextDefault::DefaultCollationCompareFunctor::operator()(
			const XalanDOMChar*					theLHS,
			const XalanDOMChar*					theRHS,
			XalanCollationServices::eCaseOrder	/* theCaseOrder */) const
{
	return XALAN_CPP_NAMESPACE :: collationCompare(theLHS, theRHS);
}



int
StylesheetExecutionContextDefault::DefaultCollationCompareFunctor::operator()(
			const XalanDOMChar*					theLHS,
			const XalanDOMChar*					theRHS,
			const XalanDOMChar*					/* theLocale */,
			XalanCollationServices::eCaseOrder	theCaseOrder) const
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



static const XalanQNameByValue	theEmptyQName;



void 
StylesheetExecutionContextDefault::formatNumber(
			double								number,
			const XalanDOMString&				pattern,
			XalanDOMString&						theResult,
			const XalanNode*					context,
			const LocatorType*					locator)
{
	const XalanDecimalFormatSymbols * theDFS = getDecimalFormatSymbols(theEmptyQName);

	if (m_formatNumberFunctor == 0) 
	{
		m_xpathExecutionContextDefault.doFormatNumber(number,pattern, theDFS, theResult,context,locator);
	} 
	else 
	{
		(*m_formatNumberFunctor)(*this, number,	pattern, theDFS, theResult, context, locator);
	}
}
 


void 
StylesheetExecutionContextDefault::formatNumber(
			double					number,
			const XalanDOMString&	pattern,
			const XalanDOMString&	dfsName,
			XalanDOMString&			theResult,
			const XalanNode*		context,
			const LocatorType*		locator)
{
	XalanQNameByValue&	theDFSQName = m_xpathExecutionContextDefault.getScratchQName();

    theDFSQName.set(dfsName, getPrefixResolver(), locator);

	const XalanDecimalFormatSymbols*	theDFS = getDecimalFormatSymbols(theDFSQName);

	if (theDFS == 0)
	{
		warn(
				XalanMessageLoader::getMessage(XalanMessages::Decimal_formatElementNotFound_1Param,"format-number()"),
				context,
				locator);

		theDFS = getDecimalFormatSymbols(theEmptyQName);
		
	}

	if (m_formatNumberFunctor == 0) 
	{
		m_xpathExecutionContextDefault.doFormatNumber(number,pattern,theDFS,theResult,context,locator);
	} 
	else 
	{	
		(*m_formatNumberFunctor)(*this, number, pattern, theDFS, theResult, context, locator);
	}
}



const StylesheetExecutionContextDefault::FormatNumberFunctor*
StylesheetExecutionContextDefault::installFormatNumberFunctor(FormatNumberFunctor*	theFunctor)
{
	assert(theFunctor != 0);

	const FormatNumberFunctor * const	temp = m_formatNumberFunctor;

	m_formatNumberFunctor = theFunctor;

	return temp;
}



StylesheetExecutionContextDefault::FormatNumberFunctor*
StylesheetExecutionContextDefault::uninstallFormatNumberFunctor()
{
	if (m_formatNumberFunctor == 0)
	{
		return 0;
	}
	else
	{
		FormatNumberFunctor * const	temp = m_formatNumberFunctor;

		m_formatNumberFunctor = 0;

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

    m_rootDocument = 0;
    m_stylesheetRoot = 0;
	m_mode = 0;

	m_currentTemplateStack.clear();
    m_currentTemplateStack.push_back(0);

	m_formatterToTextCache.reset();
	m_formatterToSourceTreeCache.reset();
	m_nodeSorterCache.reset();

	m_xresultTreeFragAllocator.reset();
    m_documentFragmentAllocator.reset();
	m_documentAllocator.reset();

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

	m_hasStripOrPreserveSpace = false;
}



XalanNode*
StylesheetExecutionContextDefault::getCurrentNode() const
{
	return m_xpathExecutionContextDefault.getCurrentNode();
}



void
StylesheetExecutionContextDefault::pushCurrentNode(XalanNode*	theCurrentNode)
{
	m_xpathExecutionContextDefault.pushCurrentNode(theCurrentNode);
}



void
StylesheetExecutionContextDefault::popCurrentNode()
{
	m_xpathExecutionContextDefault.popCurrentNode();
}



bool
StylesheetExecutionContextDefault::isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const
{
	return m_xpathExecutionContextDefault.isNodeAfter(node1, node2);
}



void
StylesheetExecutionContextDefault::pushContextNodeList(const NodeRefListBase&	theContextNodeList)
{
	m_xpathExecutionContextDefault.pushContextNodeList(theContextNodeList);
}



void
StylesheetExecutionContextDefault::popContextNodeList()
{
	m_xpathExecutionContextDefault.popContextNodeList();
}



const NodeRefListBase&
StylesheetExecutionContextDefault::getContextNodeList() const
{
	return m_xpathExecutionContextDefault.getContextNodeList();
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
StylesheetExecutionContextDefault::elementAvailable(const XalanQName&	theQName) const
{
	if (theQName.getNamespace() == XSLTEngineImpl::getXSLNameSpaceURL())
	{
		const int	xslToken = StylesheetConstructionContextDefault::getElementNameToken(theQName.getLocalPart());

		return xslToken < 0 ? false : true;
	}
	else
	{
		return m_xpathExecutionContextDefault.elementAvailable(theQName);
	}
}



bool
StylesheetExecutionContextDefault::elementAvailable(
			const XalanDOMString&	theName, 
			const LocatorType*		theLocator) const
{
	XalanQNameByValue&	theQName = m_xpathExecutionContextDefault.getScratchQName();

	theQName.set(theName, getPrefixResolver(), theLocator);

	return elementAvailable(theQName);
}



bool
StylesheetExecutionContextDefault::functionAvailable(const XalanQName&	theQName) const
{
	return m_xpathExecutionContextDefault.functionAvailable(theQName);
}



bool
StylesheetExecutionContextDefault::functionAvailable(
			const XalanDOMString&	theName, 
			const LocatorType*		theLocator) const
{
	return m_xpathExecutionContextDefault.functionAvailable(theName, theLocator);
}



const XObjectPtr
StylesheetExecutionContextDefault::extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec,
			const LocatorType*				locator)
{
	assert(m_xpathExecutionContextDefault.getXPathEnvSupport() != 0);

	return m_xpathExecutionContextDefault.getXPathEnvSupport()->extFunction(*this, theNamespace, functionName, context, argVec, locator);
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
			XalanDocument*			doc,
			const XalanQName&		qname,
			const XalanDOMString&	ref,
			MutableNodeRefList&		nodelist)
{
	assert(m_stylesheetRoot != 0);

	m_stylesheetRoot->getNodeSetByKey(
		doc,
		qname,
		ref,
		*getPrefixResolver(),
		nodelist,
		*this,
		m_keyTables);
}



void
StylesheetExecutionContextDefault::getNodeSetByKey(
			XalanDocument*			doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const LocatorType*		locator,
			MutableNodeRefList&		nodelist)
{
	assert(m_stylesheetRoot != 0);

	const PrefixResolver* const		resolver =
				getPrefixResolver();
	assert(resolver != 0);

	XalanQNameByValue&	theQName = m_xpathExecutionContextDefault.getScratchQName();

	theQName.set(name, resolver, locator);

	m_stylesheetRoot->getNodeSetByKey(
		doc,
		theQName,
		ref,
		*resolver,
		nodelist,
		*this,
		m_keyTables);
}



const XObjectPtr
StylesheetExecutionContextDefault::getVariable(
			const XalanQName&	name,
			const LocatorType*	locator)
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
		StylesheetExecutionContext::GetAndReleaseCachedString	theGuard(*this);

		XalanDOMString&		theString = theGuard.get();

		theString = XalanMessageLoader::getMessage(XalanMessages::VariableIsNotDefined_1Param,name.getLocalPart());

		error(
			theString,
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
StylesheetExecutionContextDefault::shouldStripSourceNode(const XalanText&	node)
{
	if (m_hasStripOrPreserveSpace == false || m_stylesheetRoot == 0)
	{
		return false;
	}
	else
	{
		assert(m_stylesheetRoot->hasPreserveOrStripSpaceElements() == true);
        assert(length(node.getData()) != 0);

		return m_stylesheetRoot->shouldStripSourceNode(node);
	}
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
		return 0; 
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
StylesheetExecutionContextDefault::createPrintWriter(StreamType&	theStream)
{
	XalanOutputStream* const		theOutputStream =
		new XalanStdOutputStream(theStream);

	m_outputStreams.push_back(theOutputStream);

	return createPrintWriter(theOutputStream);
}



PrintWriter*
StylesheetExecutionContextDefault::createPrintWriter(FILE*	theStream)
{
	XalanOutputStream* const		theOutputStream =
		new XalanFStreamOutputStream(theStream);

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



StylesheetExecutionContextDefault::tl_size_type
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
			const XalanDOMString&		msg,
			const ElemTemplateElement&	styleNode,
			const XalanNode*			sourceNode) const
{
	assert(m_xsltProcessor != 0);

	m_xsltProcessor->error(msg, sourceNode != 0 ? sourceNode : getCurrentNode(), &styleNode);
}



void
StylesheetExecutionContextDefault::error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const LocatorType*		locator) const
{
	assert(m_xsltProcessor != 0);

	if (sourceNode == 0)
	{
		sourceNode = getCurrentNode();
	}

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
			const char*					msg,
			const ElemTemplateElement&	styleNode,
			const XalanNode*			sourceNode) const
{
	error(TranscodeFromLocalCodePage(msg), styleNode, sourceNode);
}



void
StylesheetExecutionContextDefault::error(
			const char*			msg,
			const XalanNode*	sourceNode,
			const LocatorType*	locator) const
{
	error(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



void
StylesheetExecutionContextDefault::warn(
			const XalanDOMString&		msg,
			const ElemTemplateElement&	styleNode,
			const XalanNode* 			sourceNode) const
{
	assert(m_xsltProcessor != 0);

	if (sourceNode == 0)
	{
		sourceNode = getCurrentNode();
	}

	m_xsltProcessor->warn(msg, sourceNode, &styleNode);
}



void
StylesheetExecutionContextDefault::warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const LocatorType*		locator) const
{
	assert(m_xsltProcessor != 0);

	if (sourceNode == 0)
	{
		sourceNode = getCurrentNode();
	}

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
			const char*					msg,
			const ElemTemplateElement&	styleNode,
			const XalanNode*			sourceNode) const
{
	warn(TranscodeFromLocalCodePage(msg), styleNode, sourceNode);
}



void
StylesheetExecutionContextDefault::warn(
			const char*			msg,
			const XalanNode*	sourceNode,
			const LocatorType*	locator) const
{
	warn(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



void
StylesheetExecutionContextDefault::message(
			const XalanDOMString&		msg,
			const ElemTemplateElement&	styleNode,
			const XalanNode* 			sourceNode) const
{
	assert(m_xsltProcessor != 0);

	if (sourceNode == 0)
	{
		sourceNode = getCurrentNode();
	}

	m_xsltProcessor->message(msg, sourceNode, &styleNode);
}



void
StylesheetExecutionContextDefault::message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const LocatorType*		locator) const
{
	assert(m_xsltProcessor != 0);

	if (sourceNode == 0)
	{
		sourceNode = getCurrentNode();
	}

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
			const char*					msg,
			const ElemTemplateElement&	styleNode,
			const XalanNode*			sourceNode) const
{
	message(TranscodeFromLocalCodePage(msg), styleNode, sourceNode);
}



void
StylesheetExecutionContextDefault::message(
			const char*			msg,
			const XalanNode*	sourceNode,
			const LocatorType*	locator) const
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
			ParamsVectorType&			params)
{
	assert(getCurrentNode() != 0);
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
			if(StylesheetConstructionContext::ELEMNAME_WITH_PARAM == child->getXSLToken())
			{
				const XPath* const	pxpath = child->getXPath();

				XObjectPtr	theXObject;

				if(0 != pxpath)
				{
					theXObject =
						createVariable(
							*pxpath,
							getCurrentNode(),
							*child);
				}
				else
				{
					theXObject =
						createVariable(
							*child,
							getCurrentNode());
				}

				const ElemWithParam* const	xslParamElement =
#if defined(XALAN_OLD_STYLE_CASTS)
						(ElemWithParam*)child;
#else
						static_cast<const ElemWithParam*>(child);
#endif

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
	XALAN_USING_STD(for_each)

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
	XALAN_USING_STD(for_each)

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



XALAN_CPP_NAMESPACE_END
