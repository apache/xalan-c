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
 */
#include "ElemTemplateElement.hpp"



#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <XalanDOM/XalanAttr.hpp>
#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanDOMException.hpp>



#include <PlatformSupport/AttributeListImpl.hpp>
#include <PlatformSupport/DOMStringPrintWriter.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



#include <DOMSupport/DOMServices.hpp>



#include <XMLSupport/FormatterToText.hpp>



#include <XPath/MutableNodeRefList.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathExecutionContext.hpp>
#include <XPath/XPath.hpp>



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

const XalanEmptyNamedNodeMap	ElemTemplateElement::s_fakeAttributes;

const XalanQNameByReference			ElemTemplateElement::s_emptyQName(s_emptyString, s_emptyString);



ElemTemplateElement::ElemTemplateElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			int								lineNumber,
			int								columnNumber,
			int								xslToken) :
	XalanElement(),
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
			Stylesheet&						stylesheetTree,
			int								lineNumber,
			int								columnNumber,
			int								xslToken) :
	XalanElement(),
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
	m_baseIndentifier(s_emptyString),
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
				"xml:space has an illegal value",
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
		const XPathExecutionContext::CurrentNodeSetAndRestore	theCurrentNodeSetAndRestore(executionContext, sourceNode);

		executeChildren(executionContext);
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
	constructionContext.error(
		"An illegal call to addToStylesheet() was made during compilation of the stylesheet.",
		0,
		this);
}



void
ElemTemplateElement::processSortElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						/* theStylesheet */,
			const AttributeListType&		/* atts */,
			const LocatorType*				locator)
{
	constructionContext.error(
		"xsl:sort is not allowed at this position in the stylesheet",
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

	if (newChild->isWhitespace() == false &&
		childTypeAllowed(newChild->getXSLToken()) == false)
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



ElemTemplateElement*
ElemTemplateElement::insertBeforeElem(
			ElemTemplateElement*	newChild,
			ElemTemplateElement*	refChild)
{
	assert(newChild != 0);

	if (refChild != 0 && refChild->getParentNode() != this)
	{
		throw XalanDOMException(XalanDOMException::NOT_FOUND_ERR);
	}
	else if (newChild->getOwnerDocument() != getOwnerDocument())
	{
		throw XalanDOMException(XalanDOMException::WRONG_DOCUMENT_ERR);
	}

	if (refChild == 0)
	{
		appendChildElem(newChild);
	}
	else
	{
		ElemTemplateElement* const	previousChild =
			refChild->getPreviousSiblingElem();

		if (previousChild != 0)
		{
			previousChild->setNextSiblingElem(newChild);
		}
		else
		{
			assert(m_firstChild == refChild);

			// The old child was the first child,
			// so update m_firstChild...
			m_firstChild = newChild;
		}

		newChild->setPreviousSiblingElem(previousChild);
		newChild->setNextSiblingElem(refChild);

		refChild->setPreviousSiblingElem(newChild);

		if (refChild == m_firstChild)
		{
			m_firstChild = newChild;
		}
	}

	return newChild;
}



ElemTemplateElement*
ElemTemplateElement::replaceChildElem(
			ElemTemplateElement*	newChild,
			ElemTemplateElement*	oldChild)
{
	assert(newChild != 0);
	assert(oldChild != 0);

	if (oldChild->getParentNode() != this)
	{
		throw XalanDOMException(XalanDOMException::NOT_FOUND_ERR);
	}
	else if (newChild->getOwnerDocument() != getOwnerDocument())
	{
		throw XalanDOMException(XalanDOMException::WRONG_DOCUMENT_ERR);
	}

	ElemTemplateElement* const	previousChild =
			oldChild->getPreviousSiblingElem();

	if (previousChild != 0)
	{
		previousChild->setNextSiblingElem(newChild);
	}
	else
	{
		assert(m_firstChild == oldChild);

		// The old child was the first child,
		// so update m_firstChild...
		m_firstChild = newChild;
	}

	newChild->setPreviousSiblingElem(previousChild);

	ElemTemplateElement* const	nextChild =
			oldChild->getNextSiblingElem();

	newChild->setNextSiblingElem(nextChild);

	if (nextChild != 0)
	{
		nextChild->setPreviousSiblingElem(newChild);
	}

	oldChild->setParentNodeElem(0);
	oldChild->setPreviousSiblingElem(0);
	oldChild->setNextSiblingElem(0);

	return oldChild;
}



bool
ElemTemplateElement::transformChild(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement&		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						child) const
{
	assert(child != 0);

	const XalanNode::NodeType	nodeType = child->getNodeType();

	if(0 == theTemplate)
	{
		// Find the XSL template that is the best match for the 
		// element...
		const bool			isApplyImports = xslInstruction.getXSLToken() ==
			StylesheetConstructionContext::ELEMNAME_APPLY_IMPORTS;

		const Stylesheet*	stylesheetTree = isApplyImports ?
								&xslInstruction.getStylesheet() :
								&getStylesheet().getStylesheetRoot();

		theTemplate = stylesheetTree->findTemplate(
						executionContext,
						child,
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
				executionContext.cloneToResultTree(*child, nodeType, false, false, false, this);
				break;

			case XalanNode::ATTRIBUTE_NODE:
				{
					const XalanDOMString&	val = child->getNodeValue();

					executionContext.characters(
						toCharArray(val), 
						0,
						length(val));
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
				TracerEvent te(executionContext,							   
								*theTemplate);

				executionContext.fireTraceEvent(te);
			}

			theTemplate->executeChildren(executionContext, 
										 child);
		}
	}

	return true;
}



const XalanDOMString&
ElemTemplateElement::getNodeName() const
{
	return getElementName();
}



const XalanDOMString&
ElemTemplateElement::getNodeValue() const
{
	return s_emptyString;
}



ElemTemplateElement::NodeType
ElemTemplateElement::getNodeType() const
{
    return XalanNode::ELEMENT_NODE;
}



XalanNode*
ElemTemplateElement::getParentNode() const
{
	return m_parentNode;
}



const XalanNodeList*
ElemTemplateElement::getChildNodes() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
ElemTemplateElement::getFirstChild() const
{
	return m_firstChild;
}



XalanNode*
ElemTemplateElement::getLastChild() const
{
	return getLastChildElem();
}



XalanNode*
ElemTemplateElement::getPreviousSibling() const 
{
	return m_previousSibling;
}



XalanNode*
ElemTemplateElement::getNextSibling() const 
{
	return m_nextSibling;
}



const XalanNamedNodeMap*
ElemTemplateElement::getAttributes() const
{
	return &s_fakeAttributes;
}



XalanDocument*
ElemTemplateElement::getOwnerDocument() const
{
	return &m_stylesheet;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
ElemTemplateElement*
#endif
ElemTemplateElement::cloneNode(bool		/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
ElemTemplateElement::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
ElemTemplateElement::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
ElemTemplateElement::removeChild(XalanNode*		/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}


/** 
 * Throw a template element error.
 * 
 * @param msg Description of the error that occured.
 */

XalanNode*
ElemTemplateElement::appendChild(XalanNode*		/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
ElemTemplateElement::hasChildNodes() const
{
    return 0 != m_firstChild ? true : false;
}


void
ElemTemplateElement::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
ElemTemplateElement::normalize()
{
}



bool
ElemTemplateElement::isSupported(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
ElemTemplateElement::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
ElemTemplateElement::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
ElemTemplateElement::getLocalName() const
{
	return s_emptyString;
}



void
ElemTemplateElement::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
ElemTemplateElement::isIndexed() const
{
	return false;
}



unsigned long
ElemTemplateElement::getIndex() const
{
	return 0;
}



const XalanDOMString&
ElemTemplateElement::getTagName() const
{
	return getElementName();
}



const XalanDOMString&
ElemTemplateElement::getAttribute(const XalanDOMString&		/* name */) const
{
	return s_emptyString;
}



XalanAttr*
ElemTemplateElement::getAttributeNode(const XalanDOMString&		/* name */) const
{
	return 0;
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
			m_firstChild->getNextSibling() == 0)
		{
			m_flags |= eHasSingleTextChild;
		}
		else if (theToken == StylesheetConstructionContext::ELEMNAME_CALL_TEMPLATE &&
				 m_firstChild->getNextSibling() == 0)
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



XalanNodeList*
ElemTemplateElement::getElementsByTagName(const XalanDOMString&		/* name */) const
{
	return 0;
}



void
ElemTemplateElement::setAttribute(
			const XalanDOMString&	/* name */, 
			const XalanDOMString&	/* value */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanAttr*
ElemTemplateElement::setAttributeNode(XalanAttr*	/* newAttr */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanAttr*
ElemTemplateElement::removeAttributeNode(XalanAttr*	/* oldAttr */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



void
ElemTemplateElement::removeAttribute(const XalanDOMString&	/* name */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



const XalanDOMString&
ElemTemplateElement::getAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	return s_emptyString;
}



void
ElemTemplateElement::setAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* qualifiedName */,
			const XalanDOMString&	/* value */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
ElemTemplateElement::removeAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanAttr*
ElemTemplateElement::getAttributeNodeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	return 0;
}



XalanAttr*
ElemTemplateElement::setAttributeNodeNS(XalanAttr*	/* newAttr */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanNodeList*
ElemTemplateElement::getElementsByTagNameNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	return 0;
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
		if(finishedConstruction() == true)
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
