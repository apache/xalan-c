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



/** 
 * @param processor The XSLT Processor.
 * @param stylesheetTree The owning stylesheet.
 * @param name The name of the element.
 * @param atts The element attributes.
 * @param lineNumber The line in the XSLT file that the element occurs on.
 * @param columnNumber The column index in the XSLT file that the element occurs on.
 * @exception SAXException Never.
 */
ElemTemplateElement::ElemTemplateElement(
			StylesheetConstructionContext&	/* constructionContext */,
			Stylesheet&						stylesheetTree, 
			const XalanDOMString&			name,
			int								lineNumber,
			int								columnNumber,
			int								xslToken) :
	XalanElement(),
	PrefixResolver(),
	m_stylesheet(stylesheetTree),
	m_lineNumber(lineNumber),
	m_columnNumber(columnNumber),
	m_defaultSpace(true),
	m_finishedConstruction(false),
	m_elemName(name),
	m_xslToken(xslToken),
	m_parentNode(0),
	m_nextSibling(0),
	m_previousSibling(0),
	m_firstChild(0),
	m_surrogateChildren(*this),
	m_fakeAttributes(),
	m_baseIndentifier(stylesheetTree.getCurrentIncludeBaseIdentifier())
{
	assert(length(m_baseIndentifier) > 0);
}



ElemTemplateElement::~ElemTemplateElement()
{
	delete m_nextSibling;

	delete m_firstChild;
}



const NamespacesHandler&
ElemTemplateElement::getNamespacesHandler() const
{
	if (m_parentNode != 0)
	{
		return m_parentNode->getNamespacesHandler();
	}
	else
	{
		return m_stylesheet.getNamespacesHandler();
	}
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
		error(XalanDOMString("xml:space has an illegal value: ") + spaceVal);
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
			error(XalanDOMString("xml:space has an illegal value: ") + spaceVal);
		}
    }

    return isSpaceAttr;
}



bool
ElemTemplateElement::isValidNCName(const XalanDOMString&	s)
{
    const unsigned int	len = length(s);

    XalanDOMChar		c = charAt(s, 0);

    if(!(isLetterOrDigit(c) || (c == XalanUnicode::charLowLine)))
      return false;

    if(len > 0)
    {
		for(unsigned int i = 1; i < len; i++)
		{
			c = charAt(s, i); 

			if(!(isLetterOrDigit(c) ||
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



void
ElemTemplateElement::execute(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const
{
	if(0 != executionContext.getTraceListeners())
    {
		executionContext.fireTraceEvent(
			TracerEvent(executionContext, sourceTree, sourceNode, mode, *this));
	}    
}



void
ElemTemplateElement::executeChildren(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree, 
			XalanNode*						sourceNode,
			const QName&					mode) const
{
	StylesheetExecutionContext::PushAndPopElementFrame	thePushAndPop(executionContext, this);

    for (ElemTemplateElement* node = m_firstChild; node != 0; node = node->m_nextSibling) 
    {
		node->execute(executionContext, sourceTree, sourceNode, mode);
    }
}



XalanDOMString
ElemTemplateElement::childrenToString(
			StylesheetExecutionContext&		executionContext, 
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const
{
	// Create a print writer and formatter to generate the children as
	// a string.
	DOMStringPrintWriter		thePrintWriter;

	// Create a FormatterToText.
	FormatterToText				theFormatter(thePrintWriter);

	// Create an object to set and restore the execution state.
	StylesheetExecutionContext::ExecutionStateSetAndRestore		theStateSetAndRestore(
					executionContext,
					&theFormatter);

	executeChildren(executionContext, sourceTree, sourceNode, mode);

	return thePrintWriter.getString();
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
			const Stylesheet&				stylesheetTree,
			const ElemTemplateElement&		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						sourceTree,
			XalanNode*						sourceNodeContext,
			const QName&					mode,
			const XPath*					selectPattern,
			int								xslToken,
			int								selectStackFrameIndex) const
{
	typedef StylesheetExecutionContext::SetAndRestoreCurrentStackFrameIndex		SetAndRestoreCurrentStackFrameIndex;

	// Sort the nodes according to the xsl:sort method
	const int	tok = xslInstruction.getXSLToken();

	NodeSorter::NodeSortKeyVectorType	keys;

	// @@ JMD: Now in method processSortKeys in java ...
	if((Constants::ELEMNAME_APPLY_TEMPLATES == tok) ||
		(Constants::ELEMNAME_FOREACH == tok))
	{
		const ElemForEach* foreach =
#if defined(XALAN_OLD_STYLE_CASTS)
			(const ElemForEach*)&xslInstruction;
#else
			static_cast<const ElemForEach*>(&xslInstruction);
#endif

		const unsigned int nChildren = foreach->getSortElems().size();

		// Reserve the space now...
		keys.reserve(nChildren);

		// March backwards, performing a sort on each xsl:sort child.
		// Probably not the most efficient method.
		for(unsigned int i = 0; i < nChildren; i++)
		{
			ElemSort* sort = foreach->getSortElems()[i];
			assert(sort != 0);
			
			const XalanDOMString langString = (!isEmpty(sort->getLangAVT())) ? 
				executionContext.evaluateAttrVal(sourceNodeContext, *sort, sort->getLangAVT()): XalanDOMString();

			const XalanDOMString dataTypeString = executionContext.evaluateAttrVal(sourceNodeContext, *sort, sort->getDataTypeAVT());

			bool treatAsNumbers = ((!isEmpty(dataTypeString)) && equals(dataTypeString,Constants::ATTRVAL_DATATYPE_NUMBER)) ? 
				true : false;

			const XalanDOMString	orderString = executionContext.evaluateAttrVal(sourceNodeContext, *sort, sort->getOrderAVT());

			bool descending = ((!isEmpty(orderString)) &&  equals(orderString,Constants::ATTRVAL_ORDER_DESCENDING))? 
				true : false;

			assert(sort->getSelectPattern() != 0);

			NodeSortKey key(executionContext, 
				*sort->getSelectPattern(), 
				treatAsNumbers, 
				descending, 
				langString, 
				xslInstruction);

			keys.push_back(key);
		}
	}

/*
	@@@ JMD: This is newer java code that is not implemented in C++; so, the
	manipulation of the current stack frame index really does nothing now, it's
	just a placeholder for future implementation

    // We can only do callbacks if the node list isn't sorted.
    NodeCallback callback = (null == keys) ? this : null;
*/

	if (0 != selectPattern)
	{
		const XObject*	theXObject = 0;

		{
			SetAndRestoreCurrentStackFrameIndex		theSetAndRestore(
					executionContext,
					selectStackFrameIndex);

	/*
		@@@ JMD: This is newer java code that is not implemented in C++; the
		callback mechanism may affect the correct positioning of the stack frame and
		may be why the parameters aren't working right

		  // Optimization note: is there a way we can keep from creating 
		  // a new callback context every time?
		  TemplateElementContext callbackContext 
			= (null != callback) 
				 ? new TemplateElementContext(stylesheetTree, xslInstruction,
						 template, sourceNodeContext, mode, xslToken, tcontext,
						 savedCurrentStackFrameIndex) : null;
	*/

			theXObject = 
					selectPattern->execute(
						sourceNodeContext,
						xslInstruction,
						executionContext);
		}

		// @@ JMD: Should this be an assert ??
		if (0 != theXObject)
		{
			const XObjectGuard	result(
					executionContext.getXObjectFactory(),
					theXObject);

			const NodeRefListBase&	sourceNodes = result->nodeset();

			if(0 != executionContext.getTraceListeners())
			{
				executionContext.fireSelectEvent(
						SelectionEvent(executionContext, 
							sourceNodeContext,
							*this,
							XALAN_STATIC_UCODE_STRING("select"),
							*selectPattern,
							result.get()));
			}

			const unsigned int	nNodes = sourceNodes.getLength();

			if (nNodes > 0)
			{
				doTransformSelectedChildren(
						executionContext,
						stylesheetTree,
						xslInstruction,
						theTemplate,
						sourceTree,
						sourceNodeContext,
						mode,
						xslToken,
						selectStackFrameIndex,
						keys,
						sourceNodes,
						nNodes);
			}
		}
	}
	else if (keys.size() > 0)
	{
		const XalanNodeList* const	childNodes =
						sourceNodeContext->getChildNodes();

		const unsigned int	nNodes = childNodes->getLength();

		if (nNodes > 0)
		{
			doTransformSelectedChildren(
					executionContext,
					stylesheetTree,
					xslInstruction,
					theTemplate,
					sourceTree,
					sourceNodeContext,
					mode,
					xslToken,
					selectStackFrameIndex,
					keys,
					*childNodes,
					nNodes);
			}
	}
}



void
ElemTemplateElement::doTransformSelectedChildren(
			StylesheetExecutionContext&					executionContext,
			const Stylesheet&							stylesheetTree,
			const ElemTemplateElement&					xslInstruction,
			const ElemTemplateElement*					theTemplate,
			XalanNode*									sourceTree,
			XalanNode*									sourceNodeContext,
			const QName&								mode,
			int											xslToken,
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
			stylesheetTree,
			xslInstruction,
			theTemplate,
			sourceTree,
			sourceNodeContext,
			mode,
			xslToken,
			*sortedSourceNodes,
			sourceNodesCount);
	}
	else
	{
		doTransformSelectedChildren(
			executionContext,
			stylesheetTree,
			xslInstruction,
			theTemplate,
			sourceTree,
			sourceNodeContext,
			mode,
			xslToken,
			sourceNodes,
			sourceNodesCount);
	}
}



void
ElemTemplateElement::doTransformSelectedChildren(
			StylesheetExecutionContext&					executionContext,
			const Stylesheet&							stylesheetTree,
			const ElemTemplateElement&					xslInstruction,
			const ElemTemplateElement*					theTemplate,
			XalanNode*									sourceTree,
			XalanNode*									sourceNodeContext,
			const QName&								mode,
			int											xslToken,
			int											selectStackFrameIndex,
			const NodeSorter::NodeSortKeyVectorType&	keys,
			const XalanNodeList&						childNodes,
			unsigned int								childNodeCount) const
{
	typedef StylesheetExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

	BorrowReturnMutableNodeRefList	sourceNodes(executionContext);

	*sourceNodes = &childNodes;

	doTransformSelectedChildren(
			executionContext,
			stylesheetTree,
			xslInstruction,
			theTemplate,
			sourceTree,
			sourceNodeContext,
			mode,
			xslToken,
			selectStackFrameIndex,
			keys,
			*sourceNodes,
			childNodeCount);
}



void
ElemTemplateElement::doTransformSelectedChildren(
			StylesheetExecutionContext&			executionContext,
			const Stylesheet&					stylesheetTree,
			const ElemTemplateElement&			xslInstruction,
			const ElemTemplateElement*			theTemplate,
			XalanNode*							sourceTree,
			XalanNode*							sourceNodeContext,
			const QName&						mode,
			int									xslToken,
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

		XalanDocument* const	ownerDoc = childNode->getOwnerDocument();

		if(XalanNode::DOCUMENT_NODE != childNode->getNodeType() && ownerDoc == 0)
		{
			error(XalanDOMString("Child node does not have an owner document!"));
		}

		transformChild(
				executionContext,
				stylesheetTree,
				&xslInstruction,
				theTemplate,
				sourceTree,
				sourceNodeContext, 
				childNode,
				mode,
				xslToken);
	}
}



bool
ElemTemplateElement::transformChild(
			StylesheetExecutionContext& executionContext,
			const Stylesheet&			stylesheet_tree, 
			const ElemTemplateElement*	/* xslInstruction */,
			const ElemTemplateElement*	theTemplate,
			XalanNode*					sourceTree, 
			XalanNode*					selectContext,
			XalanNode*					child,
			const QName&				mode,
			int							xslToken) const
{
	const XalanNode::NodeType	nodeType = child->getNodeType();

	if(0 == theTemplate)
	{
		// Find the XSL template that is the best match for the 
		// element, and call buildResultFromTemplate.
		const Stylesheet*	foundStylesheet = 0;

		const bool			isApplyImports = xslToken == Constants::ELEMNAME_APPLY_IMPORTS;

		const Stylesheet*	stylesheetTree = isApplyImports ?
								&stylesheet_tree :
								&getStylesheet().getStylesheetRoot();

		theTemplate = stylesheetTree->findTemplate(
						executionContext,
						sourceTree,
						child,
						mode,
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
		executionContext.resetCurrentState(sourceTree, child);
				
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

					const XalanDOMString	val = attr->getValue();

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
							   sourceTree,
							   child, 
								mode,
								*theTemplate);

				executionContext.fireTraceEvent(te);
			}

			theTemplate->executeChildren(executionContext, 
										 sourceTree,
										 child,
										 mode);
		}

		executionContext.resetCurrentState(sourceTree, selectContext);
	}

	return true;
}



void
ElemTemplateElement::error(const XalanDOMString&	msg) const
{
	XalanDOMString errMsg("ElemTemplateElement error: " + msg);

	throw SAXException(toCharArray(errMsg));
}



XalanDOMString
ElemTemplateElement::getNodeName() const
{
	return m_elemName;
}



XalanDOMString
ElemTemplateElement::getNodeValue() const
{
	return XalanDOMString();
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



XalanDOMString
ElemTemplateElement::getNamespaceURI() const
{
	// $$ ToDo: Is this the same value as PrefixResolver::getURI()?
	return XalanDOMString();
}



XalanDOMString
ElemTemplateElement::getPrefix() const
{
	return XalanDOMString();
}



XalanDOMString
ElemTemplateElement::getLocalName() const
{
	return XalanDOMString();
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



XalanDOMString
ElemTemplateElement::getXSLTData() const
{
	return DOMString();
}



XalanDOMString
ElemTemplateElement::getTagName() const
{
	return m_elemName;
}



XalanDOMString
ElemTemplateElement::getAttribute(const XalanDOMString&		/* name */) const
{
	return XalanDOMString();
}



XalanAttr*
ElemTemplateElement::getAttributeNode(const XalanDOMString&		/* name */) const
{
	return 0;
}



void
ElemTemplateElement::postConstruction(const NamespacesHandler&	theParentHandler)
{
    for (ElemTemplateElement* node = m_firstChild; node != 0; node = node->m_nextSibling) 
    {
		node->postConstruction(theParentHandler);
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



XalanDOMString
ElemTemplateElement::getAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	return XalanDOMString();
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
	return 0;

	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanNodeList*
ElemTemplateElement::getElementsByTagNameNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	return 0;
}



XalanDOMString
ElemTemplateElement::getNamespaceForPrefix(const XalanDOMString&	prefix) const
{
	return getNamespaceForPrefixInternal(prefix, true);
}



XalanDOMString
ElemTemplateElement::getNamespaceForPrefixInternal(
			const XalanDOMString&	prefix,
			bool					fReportError) const
{
    XalanDOMString	nameSpace;

	if (isEmpty(prefix) == false)
	{
		bool			fEmptyIsError = true;

		if(m_finishedConstruction == true)
		{
			 if (equals(prefix, DOMServices::s_XMLString) == true)
			 {
				 nameSpace = DOMServices::s_XMLNamespaceURI;
			 }
			 else if (equals(prefix, DOMServices::s_XMLNamespace) == true)
			 {
				 fEmptyIsError = false;
			 }
			 else
			 {
				 nameSpace = getNamespacesHandler().getNamespace(prefix);

				if(isEmpty(nameSpace) == true)
				{
					 if (m_parentNode != 0)
					 {
						nameSpace = m_parentNode->getNamespaceForPrefixInternal(prefix, false);
					 }
				}
			}
		}
		else
		{
			nameSpace = getStylesheet().getNamespaceForPrefixFromStack(prefix);
		}

		if(fReportError == true && fEmptyIsError == true && isEmpty(nameSpace) == true)
		{
			error("Can not resolve namespace prefix: " + prefix);
		}
	}

    return nameSpace;
}



XalanDOMString
ElemTemplateElement::getURI() const
{
	return m_baseIndentifier;
}



bool
ElemTemplateElement::childTypeAllowed(int	/* xslToken */) const
{
	return true;
}
