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
#include "XercesBridgeNavigator.hpp"



#include <XalanDOM/XalanNode.hpp>



#include "XercesAttrBridge.hpp"
#include "XercesDocumentBridge.hpp"
#include "XercesTextBridge.hpp"
#include "XercesDOMException.hpp"



// I'm using this to distinguish between null nodes, which are valid, and
// an uninitialized cached node address.  This is probably bogus, and I'll
// probably just change this to 0, but this is experimental anyway...
#if defined(XALAN_OLD_STYLE_CASTS)
static XalanNode* const		invalidNodeAddress = (XalanNode*)1;
#else
static XalanNode* const		invalidNodeAddress = reinterpret_cast<XalanNode*>(1);
#endif



XercesBridgeNavigator::XercesBridgeNavigator(
			XercesDocumentBridge*	theOwnerDocument,
			bool					mappingMode) :
	m_ownerDocument(theOwnerDocument),
	m_parentNode(mappingMode == true ? invalidNodeAddress : 0),
	m_previousSibling(mappingMode == true ? invalidNodeAddress : 0),
	m_nextSibling(mappingMode == true ? invalidNodeAddress : 0),
	m_firstChild(mappingMode == true ? invalidNodeAddress : 0),
	m_lastChild(mappingMode == true ? invalidNodeAddress : 0),
	m_index(UINT_MAX)
{
}



XercesBridgeNavigator::XercesBridgeNavigator(const XercesBridgeNavigator&	theSource) :
	m_ownerDocument(theSource.m_ownerDocument),
	m_parentNode(theSource.m_parentNode),
	m_previousSibling(theSource.m_previousSibling),
	m_nextSibling(theSource.m_nextSibling),
	m_firstChild(theSource.m_firstChild),
	m_lastChild(theSource.m_lastChild),
	m_index(theSource.m_index)
{
}



XercesBridgeNavigator::~XercesBridgeNavigator()
{
}



XalanNode*
XercesBridgeNavigator::mapNode(const DOM_Node&		theXercesNode) const
{
	return m_ownerDocument->mapNode(theXercesNode);
}



XalanAttr*
XercesBridgeNavigator::mapNode(const DOM_Attr&		theXercesNode) const
{
	return m_ownerDocument->mapNode(theXercesNode);
}



DOM_Node
XercesBridgeNavigator::mapNode(const XalanNode*		theXalanNode) const
{
	return m_ownerDocument->mapNode(theXalanNode);
}



DOM_Attr
XercesBridgeNavigator::mapNode(const XalanAttr*		theXalanNode) const
{
	return m_ownerDocument->mapNode(theXalanNode);
}



XalanNode*
XercesBridgeNavigator::getParentNode(const DOM_Node&	theXercesNode) const
{
	if (m_parentNode == invalidNodeAddress)
	{
#if defined(XALAN_NO_MUTABLE)
		((XercesBridgeNavigator*)this)->m_parentNode = m_ownerDocument->mapNode(theXercesNode.getParentNode());
#else
		m_parentNode = m_ownerDocument->mapNode(theXercesNode.getParentNode());
#endif
	}

	return m_parentNode;
}



XalanNode*
XercesBridgeNavigator::getPreviousSibling(const DOM_Node&	theXercesNode) const
{
	if (m_previousSibling == invalidNodeAddress)
	{
#if defined(XALAN_NO_MUTABLE)
		((XercesBridgeNavigator*)this)->m_previousSibling = m_ownerDocument->mapNode(theXercesNode.getPreviousSibling());
#else
		m_previousSibling = m_ownerDocument->mapNode(theXercesNode.getPreviousSibling());
#endif
	}

	return m_previousSibling;
}



XalanNode*
XercesBridgeNavigator::getNextSibling(const DOM_Node&	theXercesNode) const
{
	if (m_nextSibling == invalidNodeAddress)
	{
#if defined(XALAN_NO_MUTABLE)
		((XercesBridgeNavigator*)this)->m_nextSibling = m_ownerDocument->mapNode(theXercesNode.getNextSibling());
#else
		m_nextSibling = m_ownerDocument->mapNode(theXercesNode.getNextSibling());
#endif
	}

	return m_nextSibling;
}



XalanNode*
XercesBridgeNavigator::getFirstChild(const DOM_Node&	theXercesNode) const
{
	if (m_firstChild == invalidNodeAddress)
	{
#if defined(XALAN_NO_MUTABLE)
		((XercesBridgeNavigator*)this)->m_firstChild = m_ownerDocument->mapNode(theXercesNode.getFirstChild());
#else
		m_firstChild = m_ownerDocument->mapNode(theXercesNode.getFirstChild());
#endif
	}

	return m_firstChild;
}



XalanNode*
XercesBridgeNavigator::getLastChild(const DOM_Node&	theXercesNode) const
{
	if (m_lastChild == invalidNodeAddress)
	{
#if defined(XALAN_NO_MUTABLE)
		((XercesBridgeNavigator*)this)->m_lastChild = m_ownerDocument->mapNode(theXercesNode.getLastChild());
#else
		m_lastChild = m_ownerDocument->mapNode(theXercesNode.getLastChild());
#endif
	}

	return m_lastChild;
}



XalanNode*
XercesBridgeNavigator::insertBefore(
			DOM_Node&		theXercesParent,
			XalanNode*		newChild,
			XalanNode*		refChild) const
{
	assert(newChild != 0);

	// Get the corresponding Xerces nodes...
	const DOM_Node	theNewChild = m_ownerDocument->mapNode(newChild);
	const DOM_Node	theRefChild = m_ownerDocument->mapNode(refChild);

	try
	{
		const DOM_Node	theXercesResult =
			theXercesParent.insertBefore(theNewChild, theRefChild);
		assert(m_ownerDocument->mapNode(theXercesResult) == newChild);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return newChild;
}



XalanNode*
XercesBridgeNavigator::	replaceChild(
			DOM_Node&	theXercesParent,
			XalanNode*	newChild,
			XalanNode*	oldChild) const
{
	assert(newChild != 0);
	assert(oldChild != 0);

	// Get the corresponding Xerces nodes...
	const DOM_Node	theNewChild = m_ownerDocument->mapNode(newChild);
	const DOM_Node	theOldChild = m_ownerDocument->mapNode(oldChild);

	try
	{
		const DOM_Node	theXercesResult =
			theXercesParent.replaceChild(theNewChild, theOldChild);
		assert(m_ownerDocument->mapNode(theXercesResult) == oldChild);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return oldChild;
}



XalanNode*
XercesBridgeNavigator::removeChild(
			DOM_Node&	theXercesParent,
			XalanNode*	oldChild) const
{
	assert(oldChild != 0);

	// Get the corresponding Xerces nodes...
	const DOM_Node	theOldChild = m_ownerDocument->mapNode(oldChild);

	try
	{
		const DOM_Node	theXercesResult =
			theXercesParent.removeChild(theOldChild);
		assert(m_ownerDocument->mapNode(theXercesResult) == oldChild);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return oldChild;
}



XalanNode*
XercesBridgeNavigator::appendChild(
			DOM_Node&	theXercesParent,
			XalanNode*	newChild) const
{
	return insertBefore(theXercesParent, newChild, 0);
}




XalanElement*
XercesBridgeNavigator::getOwnerElement(const DOM_Attr&		theXercesAttr) const
{
	return m_ownerDocument->mapNode(theXercesAttr.getOwnerElement());
}



XalanNode*
XercesBridgeNavigator::cloneNode(
			const XalanNode*	theXalanNode,
			const DOM_Node&		theXercesNode,
			bool				deep) const
{
	return m_ownerDocument->internalCloneNode(theXalanNode, theXercesNode, deep);
}



XalanText*
XercesBridgeNavigator::splitText(
			DOM_Text&		theXercesText,
			unsigned int	offset) const
{
	XalanText*	theXalanText = 0;

	try
	{
		DOM_Text	theNewXercesText = theXercesText.splitText(offset);
		assert(theXercesText.isNull() == false);

		theXalanText = m_ownerDocument->createBridgeNode(theNewXercesText, 0, true);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theXalanText;
}
