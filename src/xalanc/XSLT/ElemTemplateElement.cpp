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
#include "ElemTemplateElement.hpp"



#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/XalanDOM/XalanAttr.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/AttributeListImpl.hpp>
#include <xalanc/PlatformSupport/DOMStringPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XMLSupport/FormatterToText.hpp>



#include <xalanc/XPath/MutableNodeRefList.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPathExecutionContext.hpp>
#include <xalanc/XPath/XPath.hpp>



#include "Constants.hpp"
#include "ElemCallTemplate.hpp"
#include "ElemForEach.hpp"
#include "ElemTemplate.hpp"
#include "ElemTextLiteral.hpp"
#include "NamespacesHandler.hpp"
#include "NodeSorter.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"
#include "SelectionEvent.hpp"
#include "TracerEvent.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString			ElemTemplateElement::s_emptyString;

const XalanQNameByReference			ElemTemplateElement::s_emptyQName(s_emptyString, s_emptyString);



ElemTemplateElement::ElemTemplateElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			int								lineNumber,
			int								columnNumber,
			int								xslToken) :
	PrefixResolver(),
	m_stylesheet(stylesheetTree),
	m_namespacesHandler(
			constructionContext,
			stylesheetTree.getNamespacesHandler(),
			stylesheetTree.getNamespaces(),
			stylesheetTree.getXSLTNamespaceURI()),
	m_lineNumber(lineNumber),
	m_columnNumber(columnNumber),
	m_xslToken(xslToken),
	m_parentNode(0),
	m_nextSibling(0),
	m_previousSibling(0),
	m_firstChild(0),
	m_baseIndentifier(constructionContext.getPooledString(stylesheetTree.getCurrentIncludeBaseIdentifier())),
	m_locatorProxy(*this),
	m_flags(eCanGenerateAttributes)
{
}



ElemTemplateElement::ElemTemplateElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			int								xslToken,
			const XalanDOMString&			baseURI,
			int								lineNumber,
			int								columnNumber) :
	PrefixResolver(),
	m_stylesheet(stylesheetTree),
	m_namespacesHandler(),
	m_lineNumber(lineNumber),
	m_columnNumber(columnNumber),
	m_xslToken(xslToken),
	m_parentNode(0),
	m_nextSibling(0),
	m_previousSibling(0),
	m_firstChild(0),
	m_baseIndentifier(constructionContext.getPooledString(baseURI)),
	m_locatorProxy(*this),
	m_flags(eCanGenerateAttributes)
{
}



ElemTemplateElement::~ElemTemplateElement()
{
}



const LocatorType*
ElemTemplateElement::getLocator() const
{
	return &m_locatorProxy;
}



bool
ElemTemplateElement::isWhitespace() const
{
	return false;
}



bool
ElemTemplateElement::isAttrOK(
			const XalanDOMChar*				attrName,
			const AttributeListType&		atts,
			int								which,
			StylesheetConstructionContext&	constructionContext) const
{
    return m_stylesheet.isAttrOK(attrName, atts, which, constructionContext);
}



bool
ElemTemplateElement::processSpaceAttr(
			const XalanDOMChar*				aname, 
			const AttributeListType&		atts,
			int								which,
			StylesheetConstructionContext&	constructionContext)
{
    if(constructionContext.isXMLSpaceAttribute(
			aname,
			getStylesheet(),
			getLocator()) == false)
	{
		return false;
	}
	else
    {
		const XalanDOMChar*	const	spaceVal = atts.getValue(which);

		if(equals(spaceVal, Constants::ATTRVAL_PRESERVE) == true)
		{
			m_flags |= eSpacePreserve;
		}
		else if (equals(spaceVal, Constants::ATTRVAL_DEFAULT) == false)
		{
			constructionContext.error(
				XalanMessageLoader::getMessage(XalanMessages::AttributeHasIllegalValue_1Param,"xml:space"),
				0,
				this);
		}

		return true;
    }
}



bool
ElemTemplateElement::isValidNCName(const XalanDOMString&	s)
{
	return XalanQName::isValidNCName(s);
}



void
ElemTemplateElement::execute(StylesheetExecutionContext&	executionContext) const
{
	if(0 != executionContext.getTraceListeners())
    {
		executionContext.fireTraceEvent(
			TracerEvent(executionContext, *this));
	}    
}



void
ElemTemplateElement::executeChildren(StylesheetExecutionContext&	executionContext) const
{
	if (hasChildren() == true)
	{
		if (hasDirectTemplate() == true)
		{
			assert(m_directTemplate != 0);

			m_directTemplate->execute(executionContext);
		}
		else
		{
			if (hasParams() == true || hasVariables() == true)
			{
				StylesheetExecutionContext::PushAndPopElementFrame	thePushAndPop(executionContext, this);

				for (ElemTemplateElement* node = m_firstChild; node != 0; node = node->m_nextSibling) 
				{
					node->execute(executionContext);
				}
			}
			else
			{
				for (ElemTemplateElement* node = m_firstChild; node != 0; node = node->m_nextSibling) 
				{
					node->execute(executionContext);
				}
			}
		}
	}
}



void
ElemTemplateElement::executeChildren(
		StylesheetExecutionContext&		executionContext,
		XalanNode*						sourceNode) const
{
	if (hasChildren() == true)
	{
		XalanNode* const	theCurrentNode = executionContext.getCurrentNode();
		
		if (theCurrentNode == sourceNode)
		{
			executeChildren(executionContext);
		}
		else
		{
			const XPathExecutionContext::CurrentNodePushAndPop	theCurrentNodePushAndPop(executionContext, sourceNode);

			executeChildren(executionContext);
		}
	}
}



const XalanQName&
ElemTemplateElement::getNameAttribute() const
{
	return s_emptyQName;
}



void
ElemTemplateElement::addToStylesheet(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						/* theStylesheet */)
{
	// An illegal call to addToStylesheet() was made during compilation of the stylesheet.
	assert ( false );
}



void
ElemTemplateElement::processSortElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						/* theStylesheet */,
			const AttributeListType&		/* atts */,
			const LocatorType*				locator)
{
	constructionContext.error(
		XalanMessageLoader::getMessage(XalanMessages::TemplateIsNotAllowedAtThisPosition_1Param,Constants::ELEMNAME_SORT_WITH_PREFIX_STRING),
		0,
		locator);
}



void
ElemTemplateElement::setDefaultTemplate(bool	value)
{
	m_flags |= eDefaultTemplate;

	for (ElemTemplateElement* node = m_firstChild; node != 0; node = node->m_nextSibling) 
	{
		node->setDefaultTemplate(value);
	}
}



XalanDOMString&
ElemTemplateElement::doChildrenToString(
			StylesheetExecutionContext&		executionContext, 
			XalanDOMString&					result) const
{
	reserve(result, length(result) + 1024);

	// Create a print writer and formatter to generate the children as
	// a string.
	DOMStringPrintWriter	thePrintWriter(result);

	// Borrow a FormatterToText, and don't normalize CR/LF, since we don't want
	// this text to be normalized.  Finally, have the formatter handle any ignorable
	// whitespace events.
	StylesheetExecutionContext::BorrowReturnFormatterToText	theFormatter(
					executionContext,
					thePrintWriter,
					false,
					true);

	// Create an object to set and restore the execution state.
	StylesheetExecutionContext::OutputContextPushPop	theOutputContextPushPop(
						executionContext,
						theFormatter.get());

	theFormatter->startDocument();

	executeChildren(executionContext);

	theFormatter->endDocument();

	return result;
}



const XalanDOMString&
ElemTemplateElement::childrenToString(
			StylesheetExecutionContext&		executionContext,			
			XalanDOMString&					result) const
{
	if (hasSingleTextChild() == true)
	{
		assert(m_textLiteralChild != 0);

		assign(result, m_textLiteralChild->getText(), m_textLiteralChild->getLength());
	}
	else
	{
		doChildrenToString(executionContext, result);
	}

	return result;
}



void
ElemTemplateElement::childrenToResultAttribute(
			StylesheetExecutionContext&		executionContext,
			const XalanDOMString&			theName) const
{
	if (hasSingleTextChild() == true)
	{
		executionContext.addResultAttribute(
				theName,
				m_textLiteralChild->getText());
	}
	else
	{
		StylesheetExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

		childrenToString(executionContext, theResult.get());

		executionContext.addResultAttribute(
				theName,
				theResult.get());
	}
}



void
ElemTemplateElement::childrenToResultComment(StylesheetExecutionContext&	executionContext) const
{
	if (hasSingleTextChild() == true)
	{
		executionContext.comment(m_textLiteralChild->getText());
	}
	else
	{
		StylesheetExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

		childrenToString(executionContext, theResult.get());

		executionContext.comment(c_wstr(theResult.get()));
	}
}



void
ElemTemplateElement::childrenToResultPI(
			StylesheetExecutionContext&		executionContext,
			const XalanDOMString&			theTarget) const
{
	if (hasSingleTextChild() == true)
	{
		executionContext.processingInstruction(
				c_wstr(theTarget),
				m_textLiteralChild->getText());
	}
	else
	{
		StylesheetExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

		childrenToString(executionContext, theResult.get());

		executionContext.processingInstruction(
				c_wstr(theTarget),
				c_wstr(theResult.get()));
	}
}



ElemTemplateElement*
ElemTemplateElement::getFirstChildElem() const
{
    return m_firstChild;
}



void
ElemTemplateElement::setFirstChildElem(ElemTemplateElement*		theElement)
{
    m_firstChild = theElement;
}



ElemTemplateElement*
ElemTemplateElement::getLastChildElem() const
{
	ElemTemplateElement* lastChild = 0;

	for (ElemTemplateElement* node = m_firstChild; node != 0; node = node->m_nextSibling) 
	{
		lastChild = node;
	}

	return lastChild;
}



ElemTemplateElement*
ElemTemplateElement::getNextSiblingElem() const
{
	return m_nextSibling;
}



void
ElemTemplateElement::setNextSiblingElem(ElemTemplateElement*	theSibling)
{
	m_nextSibling = theSibling;
}



ElemTemplateElement*
ElemTemplateElement::getPreviousSiblingElem() const
{
	return m_previousSibling;
}



void
ElemTemplateElement::setPreviousSiblingElem(ElemTemplateElement*	theSibling)
{
	m_previousSibling = theSibling;
}



ElemTemplateElement*
ElemTemplateElement::getParentNodeElem() const
{
	return m_parentNode;
}



void
ElemTemplateElement::setParentNodeElem(ElemTemplateElement*		theParent)
{
	m_parentNode = theParent;
}



ElemTemplateElement*
ElemTemplateElement::appendChildElem(ElemTemplateElement*	newChild)
{
	assert(newChild != 0);
	assert(newChild->getXSLToken() != StylesheetConstructionContext::ELEMNAME_TEXT);
	assert(newChild->getXSLToken() != StylesheetConstructionContext::ELEMNAME_UNDEFINED);

	if (childTypeAllowed(newChild->getXSLToken()) == false)
	{
		throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
	}
	else if(0 == m_firstChild)
	{
		m_firstChild = newChild;

		newChild->setPreviousSiblingElem(0);
	}
	else
	{
		ElemTemplateElement* const	last = getLastChildElem();
		assert(last != 0);

		last->setNextSiblingElem(newChild);

		newChild->setPreviousSiblingElem(last);
	}

	newChild->setParentNodeElem(this);
	newChild->setNextSiblingElem(0);

	return newChild;
}



const XPath*
ElemTemplateElement::getXPath(unsigned int	/* index */) const
{
	return 0;
}



void
ElemTemplateElement::transformChild(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement&		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						child) const
{
	assert(child != 0);

	transformChild(
		executionContext,
		xslInstruction,
		theTemplate,
		child,
		child->getNodeType());
}




void
ElemTemplateElement::transformChild(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement&		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						child,
			XalanNode::NodeType				nodeType) const
{
	assert(child != 0);

	if(0 == theTemplate)
	{
		// Find the XSL template that is the best match for the 
		// element...
		const bool			isApplyImports = xslInstruction.getXSLToken() ==
			StylesheetConstructionContext::ELEMNAME_APPLY_IMPORTS;
        assert(isApplyImports == false || executionContext.getCurrentTemplate() != 0);

		const Stylesheet*	stylesheetTree = isApplyImports == true ?
								&executionContext.getCurrentTemplate()->getStylesheet() :
								&getStylesheet().getStylesheetRoot();

		theTemplate = stylesheetTree->findTemplate(
						executionContext,
						child,
						nodeType,
						*executionContext.getCurrentMode(),
						isApplyImports);
	}

	if(0 == theTemplate)
	{
		switch(nodeType)
		{
		case XalanNode::DOCUMENT_FRAGMENT_NODE:
		case XalanNode::ELEMENT_NODE:
			theTemplate = getStylesheet().getStylesheetRoot().getDefaultRule();
			break;

		case XalanNode::CDATA_SECTION_NODE:
		case XalanNode::TEXT_NODE:
		case XalanNode::ATTRIBUTE_NODE:
			theTemplate = getStylesheet().getStylesheetRoot().getDefaultTextRule();
			break;

		case XalanNode::DOCUMENT_NODE:
			theTemplate = getStylesheet().getStylesheetRoot().getDefaultRootRule();
			break;

		default:
			break;
		}     
	}
				
	if(0 != theTemplate)
	{
		if(theTemplate == getStylesheet().getStylesheetRoot().getDefaultTextRule())
		{
			switch(nodeType)
			{
		    case XalanNode::CDATA_SECTION_NODE:
			case XalanNode::TEXT_NODE:
				executionContext.cloneToResultTree(
                    *child,
                    XalanNode::TEXT_NODE,
                    true,
                    false,
                    getLocator());
				break;

			case XalanNode::ATTRIBUTE_NODE:
				{
					const XalanDOMString&	val = child->getNodeValue();

					const XalanDOMString::size_type		len = length(val);

					if (len > 0)
					{
						executionContext.characters(
							toCharArray(val), 
							0,
							len);
					}
				}
				break;

			default:
				assert(false);
				break;
			}
		}
		else
		{
			if(0 != executionContext.getTraceListeners())
			{
				const TracerEvent   te(executionContext,							   
								       *theTemplate);

				executionContext.fireTraceEvent(te);
			}

			theTemplate->executeChildren(executionContext, child);
		}
	}
}


void
ElemTemplateElement::postConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler)
{
	namespacesPostConstruction(
			constructionContext,
			theParentHandler,
			m_namespacesHandler);

	if (hasChildren() == true)
	{
		for (ElemTemplateElement* node = m_firstChild; node != 0; node = node->m_nextSibling) 
		{
			node->postConstruction(constructionContext, m_namespacesHandler);

			const int	theToken = node->getXSLToken();

			if (hasVariables() == false &&
				(theToken == StylesheetConstructionContext::ELEMNAME_VARIABLE ||
				 theToken == StylesheetConstructionContext::ELEMNAME_PARAM))
			{
				m_flags |= eHasVariables;
			}

			if (hasParams() == false &&
				theToken == StylesheetConstructionContext::ELEMNAME_WITH_PARAM)
			{
				m_flags |= eHasParams;
			}
		}

		assert(m_firstChild != 0);

		const int	theToken = m_firstChild->getXSLToken();

		// There are opportunities for optimization if there's only one
		// xsl:text child node.  See childrenToString()...
		if (theToken == StylesheetConstructionContext::ELEMNAME_TEXT_LITERAL_RESULT &&
			m_firstChild->getNextSiblingElem() == 0)
		{
			m_flags |= eHasSingleTextChild;
		}
		else if (theToken == StylesheetConstructionContext::ELEMNAME_CALL_TEMPLATE &&
				 m_firstChild->getNextSiblingElem() == 0)
		{
			// Just a single xsl:call-template child, so we don't need to
			// execute it if it has no params -- we can just execute the
			// template directly...
			if (m_firstChild->hasParams() == false)
			{
				m_flags |= eHasDirectTemplate;

				ElemCallTemplate* const		theCallTemplateChild =
#if defined(XALAN_OLD_STYLE_CASTS)
					(ElemCallTemplate*)m_firstChild;
#else
					static_cast<ElemCallTemplate*>(m_firstChild);
#endif

				m_directTemplate = theCallTemplateChild->getTemplate();
			}
		}
		else if (canGenerateAttributes() == false &&
				 theToken != StylesheetConstructionContext::ELEMNAME_LITERAL_RESULT)
		{
			m_flags |= eCanGenerateAttributes;
		}
	}
}



void
ElemTemplateElement::namespacesPostConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler,
			NamespacesHandler&				theHandler)
{
	theHandler.postConstruction(
			constructionContext,
			true,
			getElementName(),
			&theParentHandler);
}



const XalanDOMString*
ElemTemplateElement::getNamespaceForPrefix(const XalanDOMString&	prefix) const
{
	return getNamespaceForPrefixInternal(prefix);
}



bool
ElemTemplateElement::processPrefixControl(
			StylesheetConstructionContext&	constructionContext,
			const Stylesheet&				stylesheetTree,
			const XalanDOMString&			localName,
			const XalanDOMChar*				attrValue)
{
	if(equals(localName, Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES))
	{
		m_namespacesHandler.processExtensionElementPrefixes(
				constructionContext,
				attrValue,
				stylesheetTree.getNamespaces());

		return true;
	}
	else if (equals(localName, Constants::ATTRNAME_EXCLUDE_RESULT_PREFIXES))
	{
		m_namespacesHandler.processExcludeResultPrefixes(
				constructionContext,
				attrValue,
				stylesheetTree.getNamespaces());

		return true;
	}
	else
	{
		return false;
	}
}



const XalanDOMString*
ElemTemplateElement::getNamespaceForPrefixInternal(const XalanDOMString&	prefix) const
{
    const XalanDOMString*	nameSpace = 0;

	if (isEmpty(prefix) == false)
	{
		if(getFinishedConstruction() == true)
		{
			if (equals(prefix, DOMServices::s_XMLString) == true)
			{
				nameSpace = &DOMServices::s_XMLNamespaceURI;
			}
			else
			{
				nameSpace = getNamespacesHandler().getNamespace(prefix);

				if(nameSpace == 0)
				{
					if (m_parentNode != 0)
					{
						nameSpace = m_parentNode->getNamespaceForPrefixInternal(prefix);
					}

					// Try one last time with the stylesheet...
					if (nameSpace == 0)
					{
						nameSpace = getStylesheet().getNamespaceForPrefix(prefix);
					}
				}
			}
		}
		else
		{
			nameSpace = getStylesheet().getNamespaceForPrefixFromStack(prefix);
		}
	}

    return nameSpace;
}



const XalanDOMString&
ElemTemplateElement::getURI() const
{
	return m_baseIndentifier;
}



ElemTemplateElement::LocatorProxy::LocatorProxy(const ElemTemplateElement&	theElement) :
	m_element(theElement)
{
}



ElemTemplateElement::LocatorProxy::~LocatorProxy()
{
}



ElemTemplateElement::LocatorProxy::size_type
ElemTemplateElement::LocatorProxy::getLineNumber() const
{
	return m_element.getLineNumber();
}



ElemTemplateElement::LocatorProxy::size_type
ElemTemplateElement::LocatorProxy::getColumnNumber() const
{
	return m_element.getColumnNumber();
}



const XMLCh*
ElemTemplateElement::LocatorProxy::getPublicId() const
{
	return 0;
}



const XMLCh*
ElemTemplateElement::LocatorProxy::getSystemId() const
{
	const XalanDOMString&	theURI =
		m_element.getURI();

	if (length(theURI) == 0)
	{
		return 0;
	}
	else
	{
		return c_wstr(theURI);
	}
}



bool
ElemTemplateElement::childTypeAllowed(int	/* xslToken */) const
{
	return true;
}



XALAN_CPP_NAMESPACE_END
