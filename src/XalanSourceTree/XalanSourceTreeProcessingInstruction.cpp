/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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
#include "XalanSourceTreeProcessingInstruction.hpp"



#include <XalanDOM/XalanDOMException.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeDocumentFragment.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeText.hpp"
#include "XalanSourceTreeHelper.hpp"



static const XalanDOMString		s_emptyString;



XalanSourceTreeProcessingInstruction::XalanSourceTreeProcessingInstruction(
			const XalanDOMString&		theTarget,
			const XalanDOMString&		theData,
			XalanSourceTreeDocument*	theOwnerDocument,
			XalanNode*					theParentNode,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling,
			unsigned int				theIndex) :
	XalanProcessingInstruction(),
	m_target(theTarget),
	m_data(theData),
	m_ownerDocument(theOwnerDocument),
	m_parentNode(theParentNode),
	m_previousSibling(thePreviousSibling),
	m_nextSibling(theNextSibling),
	m_index(theIndex)
{
}



XalanSourceTreeProcessingInstruction::~XalanSourceTreeProcessingInstruction()
{
}



XalanSourceTreeProcessingInstruction::XalanSourceTreeProcessingInstruction(
			const XalanSourceTreeProcessingInstruction&		theSource,
			bool											/* deep */) :
	XalanProcessingInstruction(theSource),
	m_target(theSource.m_target),
	m_data(theSource.m_data),
	m_parentNode(0),
	m_previousSibling(0),
	m_nextSibling(0),
	m_index(0)
{
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getNodeName() const
{
	return m_target;
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getNodeValue() const
{
	return m_data;
}



XalanSourceTreeProcessingInstruction::NodeType
XalanSourceTreeProcessingInstruction::getNodeType() const
{
	return PROCESSING_INSTRUCTION_NODE;
}



XalanNode*
XalanSourceTreeProcessingInstruction::getParentNode() const
{
	if (m_parentNode != 0)
	{
		return m_parentNode;
	}
	else
	{
		return m_ownerDocument;
	}
}



const XalanNodeList*
XalanSourceTreeProcessingInstruction::getChildNodes() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeProcessingInstruction::getFirstChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeProcessingInstruction::getLastChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeProcessingInstruction::getPreviousSibling() const
{
	return m_previousSibling;
}



XalanNode*
XalanSourceTreeProcessingInstruction::getNextSibling() const
{
	return m_nextSibling;
}



const XalanNamedNodeMap*
XalanSourceTreeProcessingInstruction::getAttributes() const
{
	return 0;
}



XalanDocument*
XalanSourceTreeProcessingInstruction::getOwnerDocument() const
{
	assert(m_ownerDocument != 0);

	return m_ownerDocument;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeProcessingInstruction*
#endif
XalanSourceTreeProcessingInstruction::cloneNode(bool	deep) const
{
	return new XalanSourceTreeProcessingInstruction(*this, deep);
}



XalanNode*
XalanSourceTreeProcessingInstruction::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeProcessingInstruction::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeProcessingInstruction::removeChild(XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeProcessingInstruction::appendChild(XalanNode*	/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
XalanSourceTreeProcessingInstruction::hasChildNodes() const
{
	return false;
}



void
XalanSourceTreeProcessingInstruction::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeProcessingInstruction::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeProcessingInstruction::supports(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getLocalName() const
{
	return s_emptyString;
}



void
XalanSourceTreeProcessingInstruction::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeProcessingInstruction::isIndexed() const
{
	return true;
}



unsigned long
XalanSourceTreeProcessingInstruction::getIndex() const
{
	return m_index;
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getTarget() const
{
	return m_target;
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getData() const
{
	return m_data;
}



void
XalanSourceTreeProcessingInstruction::setData(const XalanDOMString&		/* theData */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeProcessingInstruction::setParent(XalanSourceTreeElement*	theParent)
{
	m_parentNode = theParent;
}



void
XalanSourceTreeProcessingInstruction::setParent(XalanSourceTreeDocumentFragment*	theParent)
{
	m_parentNode = theParent;
}



void
XalanSourceTreeProcessingInstruction::setPreviousSibling(XalanSourceTreeComment*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeProcessingInstruction::setPreviousSibling(XalanSourceTreeElement*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeProcessingInstruction::setPreviousSibling(XalanSourceTreeProcessingInstruction*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeProcessingInstruction::setPreviousSibling(XalanSourceTreeText*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeProcessingInstruction::appendSiblingNode(XalanSourceTreeComment*		theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeProcessingInstruction::appendSiblingNode(XalanSourceTreeElement*		theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeProcessingInstruction::appendSiblingNode(XalanSourceTreeProcessingInstruction*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeProcessingInstruction::appendSiblingNode(XalanSourceTreeText*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}
