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



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/STLHelper.hpp>
#include <PlatformSupport/TextOutputStream.hpp>
#include <PlatformSupport/XalanNumberFormat.hpp>



#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XPath/QName.hpp>
#include <XPath/ResultTreeFragBase.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPath.hpp>
#include <XPath/XPathExecutionContext.hpp>
#include <XPath/QName.hpp>



#include <XMLSupport/FormatterToDOM.hpp>
#include <XMLSupport/FormatterToXML.hpp>
#include <XMLSupport/FormatterToHTML.hpp>
#include <XMLSupport/FormatterToText.hpp>
#include <XMLSupport/XMLParserLiaison.hpp>



// Yuck, these really shouldn't be here...
#include <XercesPlatformSupport/XercesDOMPrintWriter.hpp>
#include <XercesPlatformSupport/XercesStdTextOutputStream.hpp>
#include <XercesPlatformSupport/TextFileOutputStream.hpp>



#include "Constants.hpp"
#include "ElemTemplateElement.hpp"
#include "ElemWithParam.hpp"
#include "StylesheetRoot.hpp"
#include "XSLTEngineImpl.hpp"
#include "XSLTProcessorException.hpp"



StylesheetExecutionContextDefault::XalanNumberFormatFactory			StylesheetExecutionContextDefault::s_defaultXalanNumberFormatFactory;

StylesheetExecutionContextDefault::XalanNumberFormatFactory*		StylesheetExecutionContextDefault::s_xalanNumberFormatFactory =
		&StylesheetExecutionContextDefault::getDefaultXalanNumberFormatFactory();

const StylesheetExecutionContextDefault::DefaultCollationCompareFunctor		StylesheetExecutionContextDefault::s_defaultFunctor;



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
	m_rootDocument(0),
	m_elementRecursionStack(),
	m_prefixResolver(0),
	m_stylesheetRoot(0),
	m_formatterListeners(),
	m_printWriters(),
	m_textOutputStreams(),
	m_collationCompareFunctor(&s_defaultFunctor),
	m_liveVariablesStack(),
	m_variablesStack(),
	m_matchPatternCache()
{
	m_liveVariablesStack.reserve(eDefaultVariablesStackSize);
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

	for_each(m_formatterListeners.begin(),
			 m_formatterListeners.end(),
			 DeleteFunctor<FormatterListener>());

	m_formatterListeners.clear();

	for_each(m_printWriters.begin(),
			 m_printWriters.end(),
			 DeleteFunctor<PrintWriter>());

	m_printWriters.clear();

	for_each(m_textOutputStreams.begin(),
			 m_textOutputStreams.end(),
			 DeleteFunctor<TextOutputStream>());

	m_textOutputStreams.clear();

	clearLiveVariablesStack();

	m_variablesStack.reset();

	assert(m_matchPatternCache.size() == 0);
}



bool
StylesheetExecutionContextDefault::getQuietConflictWarnings() const
{
	return m_xsltProcessor.getQuietConflictWarnings();
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
	return m_xsltProcessor.getXMLParserLiaison().createDocument();
}



void
StylesheetExecutionContextDefault::setStylesheetRoot(const StylesheetRoot*	theStylesheet)
{
	assert(theStylesheet == 0 || theStylesheet->isRoot() == true);

	m_stylesheetRoot = theStylesheet;

	m_xsltProcessor.setStylesheetRoot(theStylesheet);

	if (theStylesheet == 0)
	{
		m_xsltProcessor.setExecutionContext(0);
	}
	else
	{
		m_xsltProcessor.setExecutionContext(this);
	}
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



bool
StylesheetExecutionContextDefault::getHasPendingStartDocument() const
{
	return m_xsltProcessor.getHasPendingStartDocument();
}



void
StylesheetExecutionContextDefault::setHasPendingStartDocument(bool	b)
{
	m_xsltProcessor.setHasPendingStartDocument(b);
}



bool
StylesheetExecutionContextDefault::getMustFlushPendingStartDocument() const
{
	return m_xsltProcessor.getMustFlushPendingStartDocument();
}



void
StylesheetExecutionContextDefault::setMustFlushPendingStartDocument(bool	b)
{
	m_xsltProcessor.setMustFlushPendingStartDocument(b);
}



int
StylesheetExecutionContextDefault::getIndent() const
{
	return m_xsltProcessor.getXMLParserLiaison().getIndent();
}



const XObject*
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



const XPath*
StylesheetExecutionContextDefault::createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver)
{
	const XPath*	theResult = 0;

	// We won't cache any xpath that has a namespace, since
	// we have no idea how that might be resolved.  We could
	// enhance XPath so that we can tell if str would match
	// the XPath, once the namespace is resolved, but it may
	// not be worth it...
	const unsigned int	index = indexOf(str, ':');
	const unsigned int	len = length(str);

	// If we found a ':' before the end of the string, and
	// it's by itself (:: would indicate an axis), don't
	// try to cache the XPath...
	if (index < len - 1 && (charAt(str, index + 1) != ':'))
	{
		theResult = m_xsltProcessor.createMatchPattern(str, resolver);
	}
	else
	{
		const XPathCacheMapType::iterator	i =
			m_matchPatternCache.find(str);

		if (i != m_matchPatternCache.end())
		{
			// Update hit time...
			(*i).second.second = clock();

			theResult = (*i).second.first;
		}
		else
		{
			theResult = m_xsltProcessor.createMatchPattern(str, resolver);

			addToXPathCache(str, theResult);
		}
	}

	return theResult;
}



void
StylesheetExecutionContextDefault::returnXPath(const XPath*		xpath)
{
	if (isCached(xpath) == false)
	{
		m_xsltProcessor.returnXPath(xpath);
	}
}



const XalanDOMString
StylesheetExecutionContextDefault::evaluateAttrVal(
			XalanNode*				contextNode,
			const PrefixResolver&	namespaceContext,
			const XalanDOMString&	stringedValue)
{
	return m_xsltProcessor.evaluateAttrVal(contextNode,
										   namespaceContext,
										   stringedValue,
										   *this);
}



void
StylesheetExecutionContextDefault::pushTopLevelVariables(const ParamVectorType&		topLevelParams)
{
	assert(m_stylesheetRoot != 0);

	m_stylesheetRoot->pushTopLevelVariables(*this, topLevelParams);
}



const XObject*
StylesheetExecutionContextDefault::createVariable(
			const ElemTemplateElement*	/* element */,
			const XPath&				xpath,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver)
{
	const XObject* const	theVariable =
		xpath.execute(contextNode, resolver, *this);

	assert(m_liveVariablesStack.empty() == false);

	// We'll want to return this variable after the current element frame
	// has finished executing, so save this off for later...
	m_liveVariablesStack.back().push_back(theVariable);

	return theVariable;
}



const XObject*
StylesheetExecutionContextDefault::createVariable(
			const ElemTemplateElement*	/* element */,
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceTree,
			XalanNode*					sourceNode,
			const QName&				mode)
{
	const XObject* const	theVariable =
		createXResultTreeFrag(templateChild, sourceTree, sourceNode, mode);

	// We'll want to return this variable after the current element frame
	// has finished executing, so save this off for later...
	m_liveVariablesStack.back().push_back(theVariable);

	return theVariable;
}



void
StylesheetExecutionContextDefault::pushVariable(
			const QName&				name,
			const ElemTemplateElement*	element,
			const XalanDOMString&		str,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver)
{
	const XObject*	var = 0;

	if (length(str) > 0)
	{
		var = 	m_xsltProcessor.evalXPathStr(
						str,
						contextNode,
						resolver,
						*this);
		assert(var != 0);
	}

	// Call to our own member function, to keep track of the XObject...
	pushVariable(name, var, element);
}



void
StylesheetExecutionContextDefault::pushVariable(
			const QName&				name,
			const XObject*				var,
			const ElemTemplateElement*	element)
{
	m_variablesStack.pushVariable(name, var, element);

	if (m_liveVariablesStack.empty() == false)
	{
		// We'll want to return this variable after the current element frame
		// has finished executing, so save this off for later...
		m_liveVariablesStack.back().push_back(var);
	}
}



void
StylesheetExecutionContextDefault::pushVariable(
			const QName&				name,
			const ElemTemplateElement*	element,
			const XPath&				xpath,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver)
{
	XObjectGuard	theXObject(
				m_xpathExecutionContextDefault.getXObjectFactory(),
				xpath.execute(contextNode, resolver, *this));

	pushVariable(name, theXObject.get(), element);

	theXObject.release();
}



void
StylesheetExecutionContextDefault::pushVariable(
			const QName&				name,
			const ElemTemplateElement*	element,
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceTree,
			XalanNode*					sourceNode)
{
	XObjectGuard	theXObject(
				m_xpathExecutionContextDefault.getXObjectFactory(),
				createXResultTreeFrag(templateChild, sourceTree, sourceNode));

	pushVariable(name, theXObject.get(), element);

	theXObject.release();
}



void
StylesheetExecutionContextDefault::pushContextMarker()
{
	m_variablesStack.pushContextMarker();

	m_liveVariablesStack.push_back(LiveVariablesStackType::value_type());

	m_liveVariablesStack.back().reserve(eDefaultVariablesCollectionSize);
}



void
StylesheetExecutionContextDefault::popContextMarker()
{
	m_variablesStack.popContextMarker();

	popLiveVariablesStack();
}



void
StylesheetExecutionContextDefault::resolveTopLevelParams()
{
	pushContextMarker();

	m_xsltProcessor.resolveTopLevelParams(*this);

	m_variablesStack.markGlobalStackFrame();
}



void
StylesheetExecutionContextDefault::clearTopLevelParams()
{
	m_xsltProcessor.clearTopLevelParams();
}



class PopPushContextMarker
{
public:

	PopPushContextMarker(StylesheetExecutionContext&	theExecutionContext) :
		m_executionContext(theExecutionContext)
	{
		m_executionContext.popContextMarker();
	}

	~PopPushContextMarker()
	{
		m_executionContext.pushContextMarker();
	}

private:

	StylesheetExecutionContext&		m_executionContext;
};



void
StylesheetExecutionContextDefault::pushParams(
			const ElemTemplateElement&	xslCallTemplateElement,
			XalanNode*					sourceTree, 
			XalanNode*					sourceNode,
			const QName&				mode,
			const ElemTemplateElement*	targetTemplate)
{
	typedef VariablesStack::ParamsVectorType	ParamsVectorType;
	
	ParamsVectorType	theParams;

	const ElemTemplateElement*	child =
			xslCallTemplateElement.getFirstChildElem();

	if (0 != child)
	{
		// This object will take care of popping, then
		// pushing the context marker at the top of the
		// stack, even if an exception is thrown...
		PopPushContextMarker	thePopPush(*this);

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

				const XObject*	theXObject = 0;

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
							sourceTree,
							sourceNode,
							mode);
				}

				theParams.push_back(ParamsVectorType::value_type(&xslParamElement->getQName(), theXObject));
			}

			child = child->getNextSiblingElem();
		}
	}

	m_variablesStack.pushParams(theParams,
							   targetTemplate);
}



const XObject*
StylesheetExecutionContextDefault::getParamVariable(const QName&	theName) const
{
	return m_variablesStack.getParamVariable(theName);
}



void
StylesheetExecutionContextDefault::pushElementFrame(const ElemTemplateElement*	elem)
{
	m_variablesStack.pushElementFrame(elem);

	m_liveVariablesStack.push_back(LiveVariablesStackType::value_type());
}



void
StylesheetExecutionContextDefault::popElementFrame(const ElemTemplateElement*	elem)
{
	m_variablesStack.popElementFrame(elem);

	popLiveVariablesStack();
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
	m_xsltProcessor.startDocument();
}



void
StylesheetExecutionContextDefault::endDocument()
{
	m_xsltProcessor.endDocument();

	// This matches the pushContextMarker in
	// resolveTopLevelParams().
	popContextMarker();

	// Clear any cached XPaths...
	clearXPathCache();
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



const XObject*
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



const XObject*
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

	return getXObjectFactory().createResultTreeFrag(theFragment.release());
}



bool
StylesheetExecutionContextDefault::destroyXObject(const XObject*	theXObject) const
{
	return getXObjectFactory().returnObject(theXObject);
}



void
StylesheetExecutionContextDefault::outputToResultTree(const XObject&	xobj)
{
	m_xsltProcessor.outputToResultTree(*this, xobj);
}



void
StylesheetExecutionContextDefault::outputResultTreeFragment(const XObject&	theTree)
{
	m_xsltProcessor.outputResultTreeFragment(*this, theTree);
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
StylesheetExecutionContextDefault::getTraceSelects() const
{
	return m_xsltProcessor.getTraceSelects();
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

	m_formatterListeners.insert(theFormatter);

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
			const XalanDOMString&	version,
			const XalanDOMString&	standalone,
			bool					xmlDecl)
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
			version,
			standalone,
			xmlDecl);

	m_formatterListeners.insert(theFormatter);

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

	m_formatterListeners.insert(theFormatter);

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

	m_formatterListeners.insert(theFormatter);

	return theFormatter;
}



FormatterToText*
StylesheetExecutionContextDefault::createFormatterToText(Writer&	writer)
{
	FormatterToText* const	theFormatter =
		new FormatterToText(writer);

	m_formatterListeners.insert(theFormatter);

	return theFormatter;
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
			const XalanDOMString&	theRHS) const
{
	assert(m_collationCompareFunctor != 0);

	return (*m_collationCompareFunctor)(c_wstr(theLHS), c_wstr(theRHS));
}



int
StylesheetExecutionContextDefault::collationCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS) const
{
	assert(m_collationCompareFunctor != 0);

	return (*m_collationCompareFunctor)(theLHS, theRHS);
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
			const XalanDOMChar*		theRHS) const
{
	return ::collationCompare(theLHS, theRHS);
}



const StylesheetExecutionContextDefault::CollationCompareFunctor*
StylesheetExecutionContextDefault::installCollationCompareFunctor(const CollationCompareFunctor*	theFunctor)
{
	assert(theFunctor != 0);

	const CollationCompareFunctor* const	temp = m_collationCompareFunctor;

	m_collationCompareFunctor = theFunctor;

	return temp;
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



bool
StylesheetExecutionContextDefault::isIgnorableWhitespace(const XalanText&	node) const
{
	return m_xpathExecutionContextDefault.isIgnorableWhitespace(node);
}



XalanDOMString
StylesheetExecutionContextDefault::getNamespaceOfNode(const XalanNode&	n) const
{
	return m_xpathExecutionContextDefault.getNamespaceOfNode(n);
}



XalanDOMString
StylesheetExecutionContextDefault::getNameOfNode(const XalanNode&	n) const
{
	return m_xpathExecutionContextDefault.getNameOfNode(n);
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



unsigned int
StylesheetExecutionContextDefault::getContextNodeListLength() const
{
	return m_xpathExecutionContextDefault.getContextNodeListLength();
}



unsigned int
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



StylesheetExecutionContextDefault::XObjectArgVectorType&
StylesheetExecutionContextDefault::pushArgVector()
{
	return m_xpathExecutionContextDefault.pushArgVector();
}



void
StylesheetExecutionContextDefault::popArgVector()
{
	m_xpathExecutionContextDefault.popArgVector();
}



const XObject*
StylesheetExecutionContextDefault::extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec)
{
	return m_xpathExecutionContextDefault.extFunction(theNamespace, functionName, context, argVec);
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



const XObject*
StylesheetExecutionContextDefault::getVariable(const QName&		name) const
{
	const XObject* const	theVariable =
		m_variablesStack.getVariable(name);

	return m_xpathExecutionContextDefault.getXObjectFactory().clone(*theVariable);
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



XalanDocument*
StylesheetExecutionContextDefault::getDOMFactory() const
{
	return m_xpathExecutionContextDefault.getDOMFactory();
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
StylesheetExecutionContextDefault::shouldStripSourceNode(const XalanNode&	node)
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



const XalanDecimalFormatSymbols*
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



unsigned long
StylesheetExecutionContextDefault::getTraceListeners() const
{
	return m_xsltProcessor.getTraceListeners();
}



void
StylesheetExecutionContextDefault::fireGenerateEvent(const GenerateEvent&	ge)
{
	m_xsltProcessor.fireGenerateEvent(ge);
}



void
StylesheetExecutionContextDefault::fireTraceEvent(const TracerEvent&	te)
{
	m_xsltProcessor.fireTraceEvent(te);
}



void
StylesheetExecutionContextDefault::fireSelectEvent(const SelectionEvent&	se)
{
	m_xsltProcessor.fireSelectEvent(se);
}



void
StylesheetExecutionContextDefault::error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const XalanNode*		styleNode) const
{
	m_xsltProcessor.error(msg, sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const XalanNode*		styleNode) const
{
	m_xsltProcessor.warn(msg, sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const XalanNode*		styleNode) const
{
	m_xsltProcessor.message(msg, sourceNode, styleNode);
}



void
StylesheetExecutionContextDefault::popLiveVariablesStack()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	assert(m_liveVariablesStack.empty() == false);

	// Clean up any XObjects we created...
	for_each(
			m_liveVariablesStack.back().begin(),
			m_liveVariablesStack.back().end(),
			XObjectFactory::DeleteXObjectFunctor(getXObjectFactory()));

	// Pop the stack...
	m_liveVariablesStack.pop_back();
}



void
StylesheetExecutionContextDefault::clearLiveVariablesStack()
{
	// Clean up the entire stack.
	while(m_liveVariablesStack.empty() == false)
	{
		popLiveVariablesStack();
	}
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



// I should be able to make this out of a
// bunch of compose<> and select2nd<> adapters...
template<class Type>
class XPathCacheReturnFunctor
{
public:

	XPathCacheReturnFunctor(XSLTEngineImpl&		xsltProcessor) :
		m_xsltProcessor(xsltProcessor)
	{
	}

	void
	operator()(const Type&	theCacheEntry)
	{
		m_xsltProcessor.returnXPath(theCacheEntry.second.first);
	}

private:

	XSLTEngineImpl&		m_xsltProcessor;
};



void
StylesheetExecutionContextDefault::clearXPathCache()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(m_matchPatternCache.begin(),
			 m_matchPatternCache.end(),
			 XPathCacheReturnFunctor<XPathCacheMapType::value_type>(m_xsltProcessor));

	m_matchPatternCache.clear();
}



// I should be able to make this out of a
// bunch of compose<> and select2nd<> adapters...
template<class Type>
class XPathCacheEarliestPredicate
{
public:

	XPathCacheEarliestPredicate(XSLTEngineImpl&		xsltProcessor) :
		m_xsltProcessor(xsltProcessor)
	{
	}

	void
	operator()(const Type&	theCacheEntry)
	{
		m_xsltProcessor.returnXPath(theCacheEntry.second.first);
	}

private:

	XSLTEngineImpl&		m_xsltProcessor;
};






void
StylesheetExecutionContextDefault::addToXPathCache(
			const XalanDOMString&	pattern,
			const XPath*			theXPath)
{
	clock_t		addClock = clock();

	if (m_matchPatternCache.size() == eXPathCacheMax)
	{
		// OK, we need to clear something out of the cache...

		// Initialize the lowest clock time found so far
		// with the current clock...
		clock_t		lowest = addClock;

		// Get some iterators ready to search the cache...
		XPathCacheMapType::iterator		i =
			m_matchPatternCache.begin();

		const XPathCacheMapType::iterator	theEnd =
			m_matchPatternCache.end();

		XPathCacheMapType::iterator		earliest(theEnd);

		while(i != theEnd)
		{
			const clock_t	current = (*i).second.second;

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
		m_xsltProcessor.returnXPath((*earliest).second.first);

		m_matchPatternCache.erase(earliest);
	}

	// Add the XPath with the current clock
	m_matchPatternCache.insert(XPathCacheMapType::value_type(pattern, XPathCacheEntry(theXPath, addClock)));
}
