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
#include "ElemTemplateElement.hpp"



#include <cassert>



#include <sax/AttributeList.hpp>
#include <sax/SAXException.hpp>



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
#include "ElemForEach.hpp"
#include "ElemSort.hpp"
#include "ElemTemplate.hpp"
#include "NamespacesHandler.hpp"
#include "Stylesheet.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"
#include "SelectionEvent.hpp"
#include "TracerEvent.hpp"



const XalanDOMString						ElemTemplateElement::s_emptyString;
const NodeSorter::NodeSortKeyVectorType		ElemTemplateElement::s_dummyKeys;



ElemTemplateElement::ElemTemplateElement(
			StylesheetConstructionContext&	/* constructionContext */,
			Stylesheet&						stylesheetTree,
			int								lineNumber,
			int								columnNumber,
			int								xslToken) :
	XalanElement(),
	PrefixResolver(),
	m_finishedConstruction(false),
	m_namespacesHandler(stylesheetTree.getNamespacesHandler(),
						stylesheetTree.getNamespaces(),
						stylesheetTree.getXSLTNamespaceURI()),
	m_stylesheet(stylesheetTree),
	m_lineNumber(lineNumber),
	m_columnNumber(columnNumber),
	m_defaultSpace(true),	
	m_xslToken(xslToken),
	m_parentNode(0),
	m_nextSibling(0),
	m_previousSibling(0),
	m_firstChild(0),
	m_surrogateChildren(*this),
	m_fakeAttributes(),
	m_baseIndentifier(stylesheetTree.getCurrentIncludeBaseIdentifier())
{
}



ElemTemplateElement::~ElemTemplateElement()
{
	delete m_nextSibling;

	delete m_firstChild;
}



bool
ElemTemplateElement::isWhitespace() const
{
	return false;
}



bool
ElemTemplateElement::isAttrOK(
			int						tok,
			const XalanDOMChar*		attrName,
			const AttributeList&	/* atts */,
			int						/* which */) const
{
    bool isXMLNS = (Constants::TATTRNAME_XMLNSDEF == tok) 
		|| startsWith(attrName, Constants::ATTRNAME_XMLNS);

    return isXMLNS;  
}



bool
ElemTemplateElement::isAttrOK(
			const XalanDOMChar*				attrName,
			const AttributeList&			atts,
			int								which,
			StylesheetConstructionContext&	constructionContext) const
{
    return m_stylesheet.isAttrOK(attrName, atts, which, constructionContext);
}


void
ElemTemplateElement::processSpaceAttr(
			const AttributeList&	atts,
			int						which)
{
	const XalanDOMChar*	const	spaceVal = atts.getValue(which);

	if(equals(spaceVal, Constants::ATTRVAL_DEFAULT) == true)
	{
		m_defaultSpace = true;
	}
	else if(equals(spaceVal, Constants::ATTRVAL_PRESERVE) == true)
	{
		m_defaultSpace = false;
	}
	else
	{
		error(TranscodeFromLocalCodePage("xml:space has an illegal value: ") + spaceVal);
	}
}



bool
ElemTemplateElement::processSpaceAttr(
			const XalanDOMChar*		aname, 
			const AttributeList&	atts,
			int						which)
{
    const bool	isSpaceAttr = equals(aname, Constants::ATTRNAME_XMLSPACE);

    if(isSpaceAttr == true)
    {
		const XalanDOMChar*	const	spaceVal = atts.getValue(which);

		if(equals(spaceVal, Constants::ATTRVAL_DEFAULT))
		{
			m_defaultSpace = true;
		}
		else if(equals(spaceVal, Constants::ATTRVAL_PRESERVE) == true)
		{
			m_defaultSpace = false;
		}
		else
		{
			error(TranscodeFromLocalCodePage("xml:space has an illegal value: ") + spaceVal);
		}
    }

    return isSpaceAttr;
}



bool
ElemTemplateElement::isValidNCName(const XalanDOMString&	s)
{
    const unsigned int	len = length(s);

	if (len == 0)
	{
		return false;
	}
	else
	{
		XalanDOMChar	c = charAt(s, 0);

		if(!(XalanXMLChar::isLetter(c) || (c == XalanUnicode::charLowLine)))
		  return false;

		if(len > 1)
		{
			for(unsigned int i = 1; i < len; i++)
			{
				c = charAt(s, i); 

				if(!(XalanXMLChar::isLetter(c) ||
					 XalanXMLChar::isDigit(c) ||
					 XalanXMLChar::isExtender(c) ||
					 XalanXMLChar::isCombiningChar(c) ||
					 c == XalanUnicode::charLowLine ||
					 c == XalanUnicode::charHyphenMinus ||
					 c == XalanUnicode::charFullStop))
				{
					return false;
				}
			}
		}

		return true;
	}
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
ElemTemplateElement::executeChildren(StylesheetExecutionContext&		executionContext) const
{
	StylesheetExecutionContext::PushAndPopElementFrame	thePushAndPop(executionContext, this);

    for (ElemTemplateElement* node = m_firstChild; node != 0; node = node->m_nextSibling) 
    {
		node->execute(executionContext);
    }
}



void
ElemTemplateElement::executeChildren(
		StylesheetExecutionContext&		executionContext,
		XalanNode*						sourceNode) const
{
	const XPathExecutionContext::CurrentNodeSetAndRestore	theCurrentNodeSetAndRestore(executionContext, sourceNode);

	executeChildren(executionContext);
}



void
ElemTemplateElement::childrenToString(
			StylesheetExecutionContext&		executionContext,			
			XalanDOMString&					result) const
{
	reserve(result, length(result) + 1024);

	// Create a print writer and formatter to generate the children as
	// a string.
	DOMStringPrintWriter		thePrintWriter(result);

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



void
ElemTemplateElement::transformSelectedChildren(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement&		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						sourceNodeContext,			
			const XPath&					selectPattern,
			int								xslToken,
			int								selectStackFrameIndex) const
{
	// @@ JMD: Now in method processSortKeys in java ...
	if(Constants::ELEMNAME_APPLY_TEMPLATES == xslToken ||
	   Constants::ELEMNAME_FOREACH == xslToken)
	{
		const ElemForEach* foreach =
#if defined(XALAN_OLD_STYLE_CASTS)
			(const ElemForEach*)&xslInstruction;
#else
			static_cast<const ElemForEach*>(&xslInstruction);
#endif

		const unsigned int	nChildren = foreach->getSortElems().size();

		if (nChildren == 0)
		{
			doTransformSelectedChildren(
					executionContext,
					xslInstruction,
					theTemplate,
					sourceNodeContext,
					selectPattern,
					s_dummyKeys,
					selectStackFrameIndex);
		}
		else
		{
			NodeSorter::NodeSortKeyVectorType	keys;

			// Reserve the space now...
			keys.reserve(nChildren);

			// Get some temporary strings to use for evaluting the AVTs...
			XPathExecutionContext::GetAndReleaseCachedString	theTemp1(executionContext);

			XalanDOMString&		langString = theTemp1.get();

			XPathExecutionContext::GetAndReleaseCachedString	theTemp2(executionContext);

			XalanDOMString&		scratchString = theTemp2.get();

			// March backwards, performing a sort on each xsl:sort child.
			// Probably not the most efficient method.
			for(unsigned int i = 0; i < nChildren; i++)
			{
				const ElemSort* const	sort = foreach->getSortElems()[i];
				assert(sort != 0);

				const AVT* avt = sort->getLangAVT();

				if(0 != avt)
				{
					avt->evaluate(langString, sourceNodeContext, xslInstruction, executionContext);
				}

				avt = sort->getDataTypeAVT();

				if(0 != avt)
				{
					avt->evaluate(scratchString, sourceNodeContext, xslInstruction, executionContext);
				}			

				const bool	treatAsNumbers = !isEmpty(scratchString) && equals(scratchString, Constants::ATTRVAL_DATATYPE_NUMBER) ?
					true : false;

				clear(scratchString);

				avt = sort->getOrderAVT();

				if(0 != avt)
				{
					avt->evaluate(scratchString, sourceNodeContext, xslInstruction, executionContext);
				}			

				const bool	descending = !isEmpty(scratchString) && equals(scratchString, Constants::ATTRVAL_ORDER_DESCENDING) ?
					true : false;

				clear(scratchString);

				assert(sort->getSelectPattern() != 0);

				keys.push_back(
					NodeSortKey(
						executionContext, 
						*sort->getSelectPattern(), 
						treatAsNumbers, 
						descending, 
						langString, 
						xslInstruction));
			}

			doTransformSelectedChildren(
					executionContext,
					xslInstruction,
					theTemplate,
					sourceNodeContext,
					selectPattern,
					keys,
					selectStackFrameIndex);
		}
	}
	else
	{
		doTransformSelectedChildren(
				executionContext,
				xslInstruction,
				theTemplate,
				sourceNodeContext,
				selectPattern,
				s_dummyKeys,
				selectStackFrameIndex);
	}
}



void
ElemTemplateElement::doTransformSelectedChildren(
			StylesheetExecutionContext&					executionContext,
			const ElemTemplateElement&					xslInstruction,
			const ElemTemplateElement*					theTemplate,
			XalanNode*									sourceNodeContext,
			const XPath&								selectPattern,
			const NodeSorter::NodeSortKeyVectorType&	keys,
			int											selectStackFrameIndex) const
{
	typedef StylesheetExecutionContext::SetAndRestoreCurrentStackFrameIndex		SetAndRestoreCurrentStackFrameIndex;

	XObjectPtr	theXObject;

	{
		SetAndRestoreCurrentStackFrameIndex		theSetAndRestore(
					executionContext,
					selectStackFrameIndex);

		theXObject = 
					selectPattern.execute(
						sourceNodeContext,
						xslInstruction,
						executionContext);
	}

	// @@ JMD: Should this be an assert ??
	if (theXObject.null() == false)
	{
		const NodeRefListBase&	sourceNodes = theXObject->nodeset();

		if(0 != executionContext.getTraceListeners())
		{
			executionContext.fireSelectEvent(
					SelectionEvent(executionContext, 
						sourceNodeContext,
						*this,
						StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("select")),
						selectPattern,
						theXObject));
		}

		const unsigned int	nNodes = sourceNodes.getLength();

		if (nNodes > 0)
		{
			doTransformSelectedChildren(
					executionContext,
					xslInstruction,
					theTemplate,
					sourceNodeContext,
					selectStackFrameIndex,
					keys,
					sourceNodes,
					nNodes);
		}
	}
}



void
ElemTemplateElement::doTransformSelectedChildren(
			StylesheetExecutionContext&					executionContext,
			const ElemTemplateElement&					xslInstruction,
			const ElemTemplateElement*					theTemplate,
			XalanNode*									sourceNodeContext,
			int											selectStackFrameIndex,
			const NodeSorter::NodeSortKeyVectorType&	keys,
			const NodeRefListBase&						sourceNodes,
			unsigned int								sourceNodesCount) const
{
	if (keys.size() > 0)
	{
		typedef StylesheetExecutionContext::SetAndRestoreCurrentStackFrameIndex		SetAndRestoreCurrentStackFrameIndex;
		typedef StylesheetExecutionContext::ContextNodeListSetAndRestore			ContextNodeListSetAndRestore;
		typedef StylesheetExecutionContext::BorrowReturnMutableNodeRefList			BorrowReturnMutableNodeRefList;

		BorrowReturnMutableNodeRefList	sortedSourceNodes(executionContext);

		*sortedSourceNodes = sourceNodes;

		{
			NodeSorter	sorter;

			SetAndRestoreCurrentStackFrameIndex		theStackFrameSetAndRestore(
					executionContext,
					selectStackFrameIndex);

			ContextNodeListSetAndRestore			theContextNodeListSetAndRestore(
					executionContext,
					sourceNodes);

			sorter.sort(executionContext, *sortedSourceNodes, keys);
		}

		doTransformSelectedChildren(
			executionContext,
			xslInstruction,
			theTemplate,
			sourceNodeContext,
			*sortedSourceNodes,
			sourceNodesCount);
	}
	else
	{
		doTransformSelectedChildren(
			executionContext,
			xslInstruction,
			theTemplate,
			sourceNodeContext,
			sourceNodes,
			sourceNodesCount);
	}
}



void
ElemTemplateElement::doTransformSelectedChildren(
			StylesheetExecutionContext&			executionContext,
			const ElemTemplateElement&			xslInstruction,
			const ElemTemplateElement*			theTemplate,
			XalanNode*							sourceNodeContext,
			const NodeRefListBase&				sourceNodes,
			unsigned int						sourceNodesCount) const
{
	if(executionContext.getTraceSelects() == true)
	{
		executionContext.traceSelect(
			xslInstruction,
			sourceNodes);
	}

	// Create an object to set and restore the context node list...
	StylesheetExecutionContext::ContextNodeListSetAndRestore	theSetAndRestore(
				executionContext,
				sourceNodes);

	for(unsigned int i = 0; i < sourceNodesCount; i++) 
	{
		XalanNode* const		childNode = sourceNodes.item(i);
		assert(childNode != 0);

		if(XalanNode::DOCUMENT_NODE != childNode->getNodeType() && childNode->getOwnerDocument() == 0)
		{
			error("Child node does not have an owner document!");
		}

		transformChild(
				executionContext,
				xslInstruction,
				theTemplate,
				sourceNodeContext, 
				childNode);
	}
}



bool
ElemTemplateElement::transformChild(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement&		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						selectContext,
			XalanNode*						child) const
{
	const XalanNode::NodeType	nodeType = child->getNodeType();

	if(0 == theTemplate)
	{
		// Find the XSL template that is the best match for the 
		// element, and call buildResultFromTemplate.
		const Stylesheet*	foundStylesheet = 0;

		const bool			isApplyImports = xslInstruction.getXSLToken() == Constants::ELEMNAME_APPLY_IMPORTS;

		const Stylesheet*	stylesheetTree = isApplyImports ?
								&xslInstruction.getStylesheet() :
								&getStylesheet().getStylesheetRoot();

		theTemplate = stylesheetTree->findTemplate(
						executionContext,
						child,
						*executionContext.getCurrentMode(),
						isApplyImports,
						foundStylesheet);
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
		executionContext.resetCurrentState(child);
				
		if(theTemplate == getStylesheet().getStylesheetRoot().getDefaultTextRule())
		{
			switch(nodeType)
			{
			case XalanNode::CDATA_SECTION_NODE:
			case XalanNode::TEXT_NODE:
				executionContext.cloneToResultTree(
							*child, false, false, false);
				break;

			case XalanNode::ATTRIBUTE_NODE:
				{
					const XalanAttr* const	attr =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const XalanAttr*)child;
#else
						static_cast<const XalanAttr*>(child);
#endif

					const XalanDOMString&	val = attr->getValue();

					executionContext.characters(toCharArray(val), 
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

		executionContext.resetCurrentState(selectContext);
	}

	return true;
}



void
ElemTemplateElement::error(const XalanDOMString&	msg) const
{
	XalanDOMString errMsg("ElemTemplateElement error: " + msg);

	throw SAXException(c_wstr(errMsg));
}



void
ElemTemplateElement::error(const char*	msg) const
{
	error(TranscodeFromLocalCodePage(msg));
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
	return &m_surrogateChildren;
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
	return &m_fakeAttributes;
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
	//should not be called
	assert(false);	

	return 0;
}



XalanNode*
ElemTemplateElement::insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild)
{
#if defined(XALAN_OLD_STYLE_CASTS) || !defined(XALAN_RTTI_AVAILABLE)
	return insertBeforeElem((ElemTemplateElement*)newChild,
							(ElemTemplateElement*)refChild);
#else
	return insertBeforeElem(dynamic_cast<ElemTemplateElement*>(newChild),
							dynamic_cast<ElemTemplateElement*>(refChild));
#endif
}



XalanNode*
ElemTemplateElement::replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild)
{
#if defined(XALAN_OLD_STYLE_CASTS) || !defined(XALAN_RTTI_AVAILABLE)
	return replaceChildElem((ElemTemplateElement*)newChild,
							(ElemTemplateElement*)oldChild);
#else
	return replaceChildElem(dynamic_cast<ElemTemplateElement*>(newChild),
							dynamic_cast<ElemTemplateElement*>(oldChild));
#endif
}



XalanNode*
ElemTemplateElement::removeChild(XalanNode*		oldChild)
{
	assert(oldChild != 0);

	XalanNode*				ret = 0;

	// first try the common, easy cases
	if (oldChild == 0 || oldChild->getParentNode() != this)
	{
		throw XalanDOMException(XalanDOMException::NOT_FOUND_ERR);
	}
	else
	{
		ElemTemplateElement*	pTest = m_firstChild;
		assert(pTest != 0);

		if (pTest == oldChild)
		{
			ElemTemplateElement* const	nextChild =
				pTest->getNextSiblingElem();

			if (nextChild != 0)
			{
				nextChild->setPreviousSiblingElem(0);
			}

			pTest->setNextSiblingElem(0);
			m_firstChild = nextChild;

			ret = pTest;
		}
		else
		{
			// now we walk this singly-linked list, peeling one ahead, since we need be
			// able to patch up the list

			while (pTest->getNextSibling() != 0 && pTest->getNextSibling() != oldChild)
				pTest = pTest->getNextSiblingElem();

			ret = pTest->getNextSibling();

			if (pTest->getNextSibling() != 0)
				pTest->setNextSiblingElem(pTest->getNextSiblingElem()->getNextSiblingElem());
		}
	}

	return ret;
}


/** 
 * Throw a template element error.
 * 
 * @param msg Description of the error that occured.
 */

XalanNode*
ElemTemplateElement::appendChild(XalanNode*		oldChild)
{
#if defined(XALAN_OLD_STYLE_CASTS) || !defined(XALAN_RTTI_AVAILABLE)
	return appendChildElem((ElemTemplateElement*)oldChild);
#else
	return appendChildElem(dynamic_cast<ElemTemplateElement*>(oldChild));
#endif
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
ElemTemplateElement::supports(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
ElemTemplateElement::getNamespaceURI() const
{
	// $$ ToDo: Is this the same value as PrefixResolver::getURI()?
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
ElemTemplateElement::postConstruction(const NamespacesHandler&	theParentHandler)
{
	m_namespacesHandler.postConstruction(getElementName(), &theParentHandler);

    for (ElemTemplateElement* node = m_firstChild; node != 0; node = node->m_nextSibling) 
    {
		node->postConstruction(m_namespacesHandler);
	}
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



const XalanDOMString&
ElemTemplateElement::getNamespaceForPrefix(const XalanDOMString&	prefix) const
{
	return getNamespaceForPrefixInternal(prefix, false);
}



const XalanDOMString&
ElemTemplateElement::getNamespaceForPrefixInternal(
			const XalanDOMString&	prefix,
			bool					fReportError) const
{
    const XalanDOMString*	nameSpace = &DOMServices::s_emptyString;

	if (isEmpty(prefix) == false)
	{
		bool			fEmptyIsError = true;

		if(m_finishedConstruction == true)
		{
			if (equals(prefix, DOMServices::s_XMLString) == true)
			{
				nameSpace = &DOMServices::s_XMLNamespaceURI;
			}
			else if (equals(prefix, DOMServices::s_XMLNamespace) == true)
			{
				fEmptyIsError = false;
			}
			else
			{
				nameSpace = &getNamespacesHandler().getNamespace(prefix);

				if(isEmpty(*nameSpace) == true)
				{
					if (m_parentNode != 0)
					{
						nameSpace = &m_parentNode->getNamespaceForPrefixInternal(prefix, false);
					}

					// Try one last time with the stylesheet...
					if(isEmpty(*nameSpace) == true)
					{
						nameSpace = &getStylesheet().getNamespaceForPrefix(prefix);
					}
				}
			}
		}
		else
		{
			nameSpace = &getStylesheet().getNamespaceForPrefixFromStack(prefix);
		}

		if(fReportError == true && fEmptyIsError == true && isEmpty(*nameSpace) == true)
		{
			error("Cannot resolve namespace prefix: " + prefix);
		}
	}

    return *nameSpace;
}



const XalanDOMString&
ElemTemplateElement::getURI() const
{
	return m_baseIndentifier;
}



bool
ElemTemplateElement::childTypeAllowed(int	/* xslToken */) const
{
	return true;
}
