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

// Class header file
#include "StylesheetRoot.hpp"



#include <algorithm>
#include <memory>



#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>



#include <xalanc/PlatformSupport/AttributeListImpl.hpp>
#include <xalanc/PlatformSupport/PrintWriter.hpp>
#include <xalanc/PlatformSupport/StringTokenizer.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/XalanQNameByReference.hpp>
#include <xalanc/XPath/XPathFactory.hpp>
#include <xalanc/XPath/XPathProcessor.hpp>



#include <xalanc/XMLSupport/FormatterToHTML.hpp>
#include <xalanc/XMLSupport/FormatterToText.hpp>
#include <xalanc/XMLSupport/FormatterToXML.hpp>



#include "Constants.hpp"
#include "ElemApplyTemplates.hpp"
#include "ElemAttributeSet.hpp"
#include "ElemTemplate.hpp"
#include "ElemValueOf.hpp"
#include "KeyTable.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "TraceListener.hpp"
#include "XSLTResultTarget.hpp"



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "C:/Program Files/Rational/Quantify/pure.h"
#endif



XALAN_CPP_NAMESPACE_BEGIN



StylesheetRoot::StylesheetRoot(
        const XalanDOMString&			baseIdentifier,
		StylesheetConstructionContext&	constructionContext) :
	Stylesheet(*this,
			   baseIdentifier,
			   constructionContext),	
	m_version(),
	m_indentResult(false),
	m_encoding(),
	m_mediatype(),
	m_doctypeSystem(),
	m_doctypePublic(),
	m_omitxmlDecl(false),
	m_standalone(),
	m_resultNameSpaceURL(),
	m_outputMethod(FormatterListener::OUTPUT_METHOD_NONE),
	m_cdataSectionElems(),
	m_hasCDATASectionElems(false),
	m_importStack(),
	m_defaultTextRule(0),
	m_defaultRule(0),
	m_defaultRootRule(0),
	m_needToBuildKeysTable(false),
	m_outputEscapeURLs(true),
	m_indentAmount(-1),
	m_omitMETATag(false),
	m_elemNumberNextID(0),
	m_attributeSetsMap()
{
	// Our base class has already resolved the URI and pushed it on
	// the back of the include stack, so get it from there...
	assert(getIncludeStack().empty() == false);

	m_importStack.push_back(getIncludeStack().back());
}				



StylesheetRoot::~StylesheetRoot()
{
}



void
StylesheetRoot::postConstruction(StylesheetConstructionContext&		constructionContext)
{
	// Chain-up first...
	Stylesheet::postConstruction(constructionContext);

	initDefaultRule(constructionContext);

	{
		AttributeSetMapType::iterator			theCurrentMap = m_attributeSetsMap.begin();
		const AttributeSetMapType::iterator		theEndMap = m_attributeSetsMap.end();

		while(theCurrentMap != theEndMap)
		{
			AttributeSetVectorType::iterator		theCurrentVector = (*theCurrentMap).second.begin();
			const AttributeSetVectorType::iterator	theEndVector = (*theCurrentMap).second.end();

			while(theCurrentVector != theEndVector)
			{
				(*theCurrentVector)->postConstruction(constructionContext, getNamespacesHandler());

				++theCurrentVector;
			}

			++theCurrentMap;
		}	
	}

	// We may need to build keys, since we may have inherited them from
	// our imports.
	if (m_needToBuildKeysTable == false && m_keyDeclarations.empty() == false)
	{
		m_needToBuildKeysTable = true;
	}

	// cdata-section-elements only applies to the XML output method...
	if (m_outputMethod != FormatterListener::OUTPUT_METHOD_XML &&
		m_outputMethod != FormatterListener::OUTPUT_METHOD_NONE)
	{
		m_cdataSectionElems.clear();
	}
	else if (m_cdataSectionElems.empty() == false)
	{
		XALAN_USING_STD(sort)

		sort(
			m_cdataSectionElems.begin(),
			m_cdataSectionElems.end(),
			pointer_less<XalanQName>());

		m_hasCDATASectionElems = true;
	}
}



void
StylesheetRoot::process(
			XalanNode*						sourceTree, 
			XSLTResultTarget&				outputTarget,
			StylesheetExecutionContext&		executionContext) const
{
	assert(m_defaultRule != 0);
	assert(m_defaultTextRule != 0);
	assert(m_defaultRootRule != 0);

	// Find the root pattern in the XSL.
	const ElemTemplateElement*		rootRule =
			findTemplate(executionContext, sourceTree);

	if(0 == rootRule)
	{
		rootRule = m_defaultRootRule;
	}
	assert(rootRule != 0);

	executionContext.setStylesheetRoot(this);

	setupFormatterListener(outputTarget, executionContext);

	executionContext.setRootDocument(sourceTree);

	if(executionContext.doDiagnosticsOutput())
	{
		executionContext.diag(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING(" =============================")));
		executionContext.diag(XalanMessageLoader::getMessage(XalanMessages::Transforming));
		executionContext.pushTime(&sourceTree);
	}

	typedef StylesheetExecutionContext::PushAndPopContextMarker	PushAndPopContextMarker;
	typedef StylesheetExecutionContext::PushAndPopElementFrame	PushAndPopElementFrame;
	typedef StylesheetExecutionContext::ResolveAndClearTopLevelParams	ResolveAndClearTopLevelParams;

	PushAndPopContextMarker		thePushAndPopContextMarker(executionContext);

	PushAndPopElementFrame		thePushAndPopElementFrame(
				executionContext,
				0);

	ResolveAndClearTopLevelParams	theResolveAndClearTopLevelParams(executionContext);

#if defined(XALAN_VQ_SPECIAL_TRACE)
	QuantifyStartRecordingData();
#endif

	executionContext.startDocument();

	executionContext.setCurrentMode(&s_emptyQName);

	const XPathExecutionContext::CurrentNodePushAndPop	theCurrentNodePushAndPop(executionContext, sourceTree);

	// Output the action of the found root rule.  All processing
	// occurs from here.
	rootRule->execute(executionContext);

	// At this point, anything transient during the tranformation
	// may have been deleted, so we may not refer to anything the
	// execution context may have created for us.
	executionContext.endDocument();

#if defined(XALAN_VQ_SPECIAL_TRACE)
	QuantifyStopRecordingData();
#endif

	if(executionContext.doDiagnosticsOutput())
	{
		executionContext.diag(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("")));
		executionContext.displayDuration(XalanMessageLoader::getMessage(XalanMessages::Transform), &sourceTree);
		executionContext.diag(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("")));
	}
}



const XalanDOMString&
StylesheetRoot::getEncoding(const XSLTResultTarget&		outputTarget) const
{
	const XalanDOMString&	theEncoding = outputTarget.getEncoding();

	if (theEncoding.length() != 0)
	{
		return theEncoding;
	}
	else
	{
		return m_encoding;
	}
}



FormatterListener*
StylesheetRoot::setupFormatterListener(
			XSLTResultTarget&				outputTarget,
			StylesheetExecutionContext&		executionContext) const
{
	FormatterListener*	flistener = outputTarget.getFormatterListener();

	if (flistener != 0)
	{
		// Do encoding stuff here...
	}
	else if(0 != outputTarget.getCharacterStream() ||
			0 != outputTarget.getByteStream() ||
			0 != outputTarget.getStream() ||
			0 != length(outputTarget.getFileName()))
	{
		/*
		 * Output target has a character or byte stream or file
		 */
		Writer*		pw = 0;

		if(0 != outputTarget.getCharacterStream())
		{
			pw = outputTarget.getCharacterStream();
		}
		else
		{
			if(0 != outputTarget.getByteStream())
			{
				pw = executionContext.createPrintWriter(*outputTarget.getByteStream());
			}
			else if (0 != outputTarget.getStream())
			{
				pw = executionContext.createPrintWriter(outputTarget.getStream());
			}
			else if(!isEmpty(outputTarget.getFileName()))
			{
				pw = executionContext.createPrintWriter(
							outputTarget.getFileName(),
							XalanDOMString());
			}
			else
			{
				executionContext.error(
					XalanMessageLoader::getMessage(XalanMessages::NoValidResultTarget),executionContext.getCurrentNode(), 0);
			}
		}

		int		indentAmount = executionContext.getIndent();

		// If the indent amount is less than 0, that means use
		// the value specified in the stylesheet.
		if (indentAmount < 0)
		{
			indentAmount = m_indentAmount;
		}

		const bool	doIndent = (indentAmount > -1) ? true : m_indentResult;

		const XalanDOMString&	theEncoding = getEncoding(outputTarget);

		switch(m_outputMethod)
		{
		case FormatterListener::OUTPUT_METHOD_HTML:
			{
				if (doIndent == true && indentAmount < 0)
				{
					indentAmount = FormatterToHTML::eDefaultIndentAmount;
				}

				// Start with the default that was set in the stylesheet...
				bool	outputEscapeURLs = m_outputEscapeURLs;

				{
					const StylesheetExecutionContext::eEscapeURLs	eEscapeURLs =
						executionContext.getEscapeURLs();

					// If it's anything other than StylesheetExecutionContext::eEscapeURLsDefault,
					// use the property from the execution context...
					if (eEscapeURLs == StylesheetExecutionContext::eEscapeURLsNo)
					{
						outputEscapeURLs = false;
					}
					else if (eEscapeURLs == StylesheetExecutionContext::eEscapeURLsYes)
					{
						outputEscapeURLs = true;
					}
				}

				// Start with the default that was set in the stylesheet...
				bool	omitMETATag = m_omitMETATag;

				{
					const StylesheetExecutionContext::eOmitMETATag	eOmitMETATag =
						executionContext.getOmitMETATag();

					// If it's anything other than StylesheetExecutionContext::eOmitMETATagDefault,
					// use the property from the execution context...
					if (eOmitMETATag == StylesheetExecutionContext::eOmitMETATagNo)
					{
						omitMETATag = false;
					}
					else if (eOmitMETATag == StylesheetExecutionContext::eOmitMETATagYes)
					{
						omitMETATag = true;
					}
				}

				flistener = executionContext.createFormatterToHTML(
								*pw,
								theEncoding,
								m_mediatype,
								m_doctypeSystem,
								m_doctypePublic,
								doIndent,
								indentAmount,
								outputEscapeURLs,
								omitMETATag);
			}
			break;

		case FormatterListener::OUTPUT_METHOD_TEXT:
			flistener = executionContext.createFormatterToText(*pw, theEncoding);
			break;

		case FormatterListener::OUTPUT_METHOD_NONE:
		case FormatterListener::OUTPUT_METHOD_XML:
		default:
			// Make sure we don't have a negative indent amount if we're
			// indenting
			if (doIndent == true && indentAmount < 0)
			{
				indentAmount = FormatterToXML::eDefaultIndentAmount;
			}

			flistener = executionContext.createFormatterToXML(
						*pw, m_version, doIndent, indentAmount, theEncoding, m_mediatype,
						m_doctypeSystem, m_doctypePublic, !m_omitxmlDecl, m_standalone);
			break;
		}

		executionContext.setFormatterListener(flistener);
	}
	else
	{
		executionContext.error(
					XalanMessageLoader::getMessage(XalanMessages::NoValidResultTarget),
					executionContext.getCurrentNode(),
					0);
	}

	executionContext.setFormatterListener(flistener);

	return flistener;
}



void 
StylesheetRoot::processOutputSpec(
			const XalanDOMChar*				name, 
			const AttributeListType&		atts,
			StylesheetConstructionContext&	constructionContext)
{
	const unsigned int		nAttrs = atts.getLength();

	bool					didSpecifyIndent = false;

	const LocatorType* const	theLocator = constructionContext.getLocatorFromStack();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_OUTPUT_METHOD))
		{
			const XalanDOMChar*	const	method = atts.getValue(i);

			if(equals(method, Constants::ATTRVAL_OUTPUT_METHOD_HTML))
			{
				m_outputMethod = FormatterListener::OUTPUT_METHOD_HTML;
			}
			else if(equals(method, Constants::ATTRVAL_OUTPUT_METHOD_XML))
			{
				m_outputMethod = FormatterListener::OUTPUT_METHOD_XML;
			}
			else if(equals(method, Constants::ATTRVAL_OUTPUT_METHOD_TEXT))
			{
				m_outputMethod = FormatterListener::OUTPUT_METHOD_TEXT;
			}
			else
			{
				constructionContext.warn(XalanMessageLoader::getMessage(XalanMessages::HasAnUnknownMethod_2Param,XalanDOMString(aname),XalanDOMString(method)),0, theLocator);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_OUTPUT_VERSION))
		{
			m_version = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_INDENT))
		{
			m_indentResult = getYesOrNo(aname, atts.getValue(i), constructionContext);

			didSpecifyIndent = true;
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_ENCODING))
		{
			m_encoding = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_MEDIATYPE))
		{
			m_mediatype = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_DOCTYPE_SYSTEM))
		{
			m_doctypeSystem = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_DOCTYPE_PUBLIC))
		{
			m_doctypePublic = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_OMITXMLDECL))
		{
			m_omitxmlDecl = getYesOrNo(aname, atts.getValue(i), constructionContext);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_STANDALONE))
		{
			m_standalone = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS))
		{
			if (m_outputMethod == FormatterListener::OUTPUT_METHOD_NONE ||
				m_outputMethod == FormatterListener::OUTPUT_METHOD_XML)
			{
				StringTokenizer	theTokenizer(atts.getValue(i));

				StringTokenizer::size_type	theTokenCount =
					theTokenizer.countTokens();

				m_cdataSectionElems.reserve(m_cdataSectionElems.size() + theTokenCount);

				XalanDOMString	theToken;

				while(theTokenCount > 0)
				{
					theTokenizer.nextToken(theToken);

					--theTokenCount;

					m_cdataSectionElems.push_back(
						constructionContext.createXalanQName(theToken, getNamespaces(), theLocator, true));
				}

				assert(theTokenizer.hasMoreTokens() == false);
			}
		}
		else
		{
			const XalanQNameByValue		theAttributeName(aname, getNamespaces(), theLocator);

			if (theAttributeName.getNamespace() == constructionContext.getXalanXSLNameSpaceURL())
			{
				if (theAttributeName.getLocalPart() == Constants::ATTRNAME_ESCAPE_URLS)
				{
					m_outputEscapeURLs = getYesOrNo(aname, atts.getValue(i), constructionContext);
				}
				else if (theAttributeName.getLocalPart() == Constants::ATTRNAME_INDENTAMOUNT)
				{
					m_indentAmount = WideStringToInt(atts.getValue(i));

					if (m_indentAmount < 0)
					{
						m_indentAmount = 0;
					}
				}
				else if (theAttributeName.getLocalPart() == Constants::ATTRNAME_OMIT_META_TAG)
				{
					m_omitMETATag = getYesOrNo(aname, atts.getValue(i), constructionContext);
				}
				else
				{
					constructionContext.warn(
						XalanMessageLoader::getMessage(XalanMessages::UnsupportedXalanSpecificAttribute_1Param,theAttributeName.getLocalPart()),
						0,
						theLocator);
				}
			}
			else if (isAttrOK(aname, atts, i, constructionContext) == false)
			{
				constructionContext.error(
						XalanMessageLoader::getMessage(XalanMessages::HasIllegalAttribute_2Param, XalanDOMString(name) ,XalanDOMString(aname)),
						0,
						theLocator);
			}
		}
	}

	if(FormatterListener::OUTPUT_METHOD_HTML == m_outputMethod &&
	   false == didSpecifyIndent)
	{
		m_indentResult = true;
	}
}



void 
StylesheetRoot::initDefaultRule(StylesheetConstructionContext&	constructionContext)
{
	if (m_defaultRule == 0)
	{
		assert(m_defaultTextRule == 0);
		assert(m_defaultRootRule == 0);

		AttributeListImpl		attrs;

		attrs.addAttribute(c_wstr(Constants::ATTRNAME_MATCH),
	 					   c_wstr(Constants::ATTRTYPE_CDATA),
						   XPath::PSEUDONAME_ANY);

		m_defaultRule =
			constructionContext.createElement(
				StylesheetConstructionContext::ELEMNAME_TEMPLATE,
				*this,
				attrs);
		assert(m_defaultRule != 0);

		attrs.clear();

		ElemTemplateElement*	childrenElement =
			constructionContext.createElement(
				StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES,
				*this,
				attrs);
		assert(childrenElement != 0);

		m_defaultRule->appendChildElem(childrenElement);

		m_defaultRule->setDefaultTemplate(true);

		// -----------------------------

		attrs.clear();
		attrs.addAttribute(c_wstr(Constants::ATTRNAME_MATCH),
	 					   c_wstr(Constants::ATTRTYPE_CDATA),
						   c_wstr(Constants::ATTRVAL_DEFAULT_TEXT_RULE));

		m_defaultTextRule =
			constructionContext.createElement(
				StylesheetConstructionContext::ELEMNAME_TEMPLATE,
				*this,
				attrs);
		assert(m_defaultTextRule != 0);

		attrs.clear();
		attrs.addAttribute(c_wstr(Constants::ATTRNAME_SELECT),
	 					   c_wstr(Constants::ATTRTYPE_CDATA),
						   c_wstr(Constants::ATTRVAL_THIS));

		childrenElement =
			constructionContext.createElement(
				StylesheetConstructionContext::ELEMNAME_VALUE_OF,
				*this,
				attrs);
		assert(childrenElement != 0);

		m_defaultTextRule->appendChildElem(childrenElement);

		m_defaultTextRule->setDefaultTemplate(true);

		//--------------------------------
    
		attrs.clear();
		attrs.addAttribute(c_wstr(Constants::ATTRNAME_MATCH),
	 					   c_wstr(Constants::ATTRTYPE_CDATA),
						   XPath::PSEUDONAME_ROOT);

		m_defaultRootRule =
			constructionContext.createElement(
				StylesheetConstructionContext::ELEMNAME_TEMPLATE,
				*this,
				attrs);
		assert(m_defaultRootRule != 0);

		attrs.clear();

		childrenElement =
			constructionContext.createElement(
				StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES,
				*this,
				attrs);
		assert(childrenElement != 0);

		m_defaultRootRule->appendChildElem(childrenElement);

		m_defaultRootRule->setDefaultTemplate(true);
	}

	assert(m_defaultRule != 0);
	assert(m_defaultTextRule != 0);
	assert(m_defaultRootRule != 0);
}



bool
StylesheetRoot::isCDATASectionElementName(const XalanQName&		theQName) const
{
	XALAN_USING_STD(find_if)

	return find_if(
			m_cdataSectionElems.begin(),
			m_cdataSectionElems.end(),
			pointer_equals_predicate<XalanQName>(&theQName)) != m_cdataSectionElems.end() ? true : false;
}



inline XalanNode*
getKeyNode(
			XalanNode*						startNode,
			StylesheetExecutionContext&		executionContext)
{
	// This is a big hack for dealing with result tree fragments coerced to node-sets using the
	// node-set extension function.  For such cases, the "document" is really the
	// XalanDocumentFragment instance that owns the nodes, not the owner document.
	if (startNode->getFirstChild() == 0)
	{
		XalanNode*	currentNode = executionContext.getCurrentNode();
		assert(currentNode != 0);

		if (currentNode->getOwnerDocument() == startNode)
		{
			// OK, the current node belongs to the document, but the document
			// is just a factory for fragments...
			for(;;)
			{
				XalanNode*	parentNode = DOMServices::getParentOfNode(*currentNode);

				if (parentNode == 0)
				{
					break;
				}
				else if (parentNode->getNodeType() == XalanNode::DOCUMENT_FRAGMENT_NODE)
				{
					startNode = parentNode;

					break;
				}
				else
				{
					currentNode = parentNode;
				}
			}
		}
	}

	return startNode;
}



void
StylesheetRoot::getNodeSetByKey(
			XalanDocument*					doc,
			const XalanQName&				qname,
			const XalanDOMString&			ref,
			const PrefixResolver&			resolver,
			MutableNodeRefList&				nodelist,
			StylesheetExecutionContext&		executionContext,
			KeyTablesTableType& 			theKeysTable) const
{
	assert(nodelist.empty() == true || nodelist.getDocumentOrder() == true);

	XalanNode* const	theKeyNode = getKeyNode(doc, executionContext);
	assert(theKeyNode != 0);

	if(m_needToBuildKeysTable == true)
	{
		assert(m_keyDeclarations.empty() == false);

		const KeyTablesTableType::const_iterator	i =
			theKeysTable.find(theKeyNode);

		if (i != theKeysTable.end())
		{
			const MutableNodeRefList&	nl = (*i).second->getNodeSetByKey(qname, ref);

			if (nodelist.empty() == true)
			{
				nodelist = nl;
			}
			else
			{
				nodelist.addNodesInDocOrder(nl, executionContext);
			}
		}
		else
		{
			XalanAutoPtr<KeyTable>	kt(
				new KeyTable(
							 theKeyNode,
							 resolver,
							 m_keyDeclarations,
							 executionContext));

			theKeysTable[theKeyNode] = kt.get();

			const KeyTable* const	theNewTable = kt.release();

			const MutableNodeRefList&	nl = theNewTable->getNodeSetByKey(qname, ref);

			if (nodelist.empty() == true)
			{
				nodelist = nl;
			}
			else
			{
				nodelist.addNodesInDocOrder(nl, executionContext);
			}
		}
	}
}



bool
StylesheetRoot::shouldStripSourceNode(const XalanText&	textNode) const
{
	const XalanNode* const	parent = textNode.getParentNode();
	assert(parent != 0);

	if (textNode.isIgnorableWhitespace() == true &&
		parent->getNodeType() == XalanNode::ELEMENT_NODE)
	{
		const XalanElement* const	theElement =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanElement*)parent;
#else
				static_cast<const XalanElement*>(parent);
#endif

		typedef WhitespaceElementsVectorType::const_iterator	const_iterator;

		for (const_iterator i = m_whitespaceElements.begin();
				i != m_whitespaceElements.end();
					++i)
		{
			if ((*i)(*theElement) != XPath::eMatchScoreNone)
			{
				return (*i).getType() == XalanSpaceNodeTester::eStrip;
			}
		}
	}

	return false;
}



void
StylesheetRoot::addAttributeSet(ElemAttributeSet&	theAttributeSet)
{
	m_attributeSetsMap[&theAttributeSet.getQName()].push_back(&theAttributeSet);
}



void
StylesheetRoot::executeAttributeSet(
			StylesheetExecutionContext&		theExecutionContext,
			const XalanQName&				theQName,
			const LocatorType*				theLocator) const
{
	const AttributeSetMapType::const_iterator	i =
		m_attributeSetsMap.find(&theQName);

	if (i == m_attributeSetsMap.end())
	{
		theExecutionContext.error(
			XalanMessageLoader::getMessage(XalanMessages::UnknownNodeType_1Param,Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING),
			theExecutionContext.getCurrentNode(),
			theLocator);
	}
	else
	{
		const AttributeSetVectorType&					theAttributeSets = (*i).second;
		const AttributeSetVectorType::const_iterator	theEnd = theAttributeSets.end();

		for(AttributeSetVectorType::const_iterator i = theAttributeSets.begin(); i != theEnd; ++i)
		{
			assert(*i != 0);

			(*i)->execute(theExecutionContext);
		}
	}
}



XALAN_CPP_NAMESPACE_END
