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
#include "XalanSourceTreeComment.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeDocumentFragment.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeProcessingInstruction.hpp"
#include "XalanSourceTreeText.hpp"
#include "XalanSourceTreeHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString		s_emptyString;



XalanSourceTreeComment::XalanSourceTreeComment(
			const XalanDOMString&		theData,
			XalanSourceTreeDocument*	theOwnerDocument,
			XalanNode*					theParentNode,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling,
			IndexType					theIndex) :
	XalanComment(),
	m_data(theData),
	m_ownerDocument(theOwnerDocument),
	m_parentNode(theParentNode),
	m_previousSibling(thePreviousSibling),
	m_nextSibling(theNextSibling),
	m_index(theIndex)
{
}



XalanSourceTreeComment::~XalanSourceTreeComment()
{
}



XalanSourceTreeComment::XalanSourceTreeComment(
			const XalanSourceTreeComment&	theSource,
			bool							/* deep */) :
	XalanComment(theSource),
	m_data(theSource.m_data),
	m_parentNode(0),
	m_previousSibling(0),
	m_nextSibling(0),
	m_index(0)
{
}



const XalanDOMString&
XalanSourceTreeComment::getNodeName() const
{
	return s_nameString;
}



const XalanDOMString&
XalanSourceTreeComment::getNodeValue() const
{
	return m_data;
}



XalanSourceTreeComment::NodeType
XalanSourceTreeComment::getNodeType() const
{
	return COMMENT_NODE;
}



XalanNode*
XalanSourceTreeComment::getParentNode() const
{
	assert(m_ownerDocument != 0);

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
XalanSourceTreeComment::getChildNodes() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeComment::getFirstChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeComment::getLastChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeComment::getPreviousSibling() const
{
	return m_previousSibling;
}



XalanNode*
XalanSourceTreeComment::getNextSibling() const
{
	return m_nextSibling;
}



const XalanNamedNodeMap*
XalanSourceTreeComment::getAttributes() const
{
	return 0;
}



XalanDocument*
XalanSourceTreeComment::getOwnerDocument() const
{
	assert(m_ownerDocument != 0);

	return m_ownerDocument;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeComment*
#endif
XalanSourceTreeComment::cloneNode(bool	/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeComment::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeComment::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeComment::removeChild(XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeComment::appendChild(XalanNode*	/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
XalanSourceTreeComment::hasChildNodes() const
{
	return false;
}



void
XalanSourceTreeComment::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeComment::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeComment::isSupported(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
XalanSourceTreeComment::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeComment::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeComment::getLocalName() const
{
	return s_emptyString;
}



void
XalanSourceTreeComment::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeComment::isIndexed() const
{
	return true;
}



XalanSourceTreeComment::IndexType
XalanSourceTreeComment::getIndex() const
{
	return m_index;
}



const XalanDOMString&
XalanSourceTreeComment::getData() const
{
	return m_data;
}



unsigned int
XalanSourceTreeComment::getLength() const
{
	assert(unsigned(length(m_data)) == length(m_data));

	return unsigned(length(m_data));
}



XalanDOMString
XalanSourceTreeComment::substringData(
			unsigned int	offset,
			unsigned int	count) const
{
	return m_data.substr(offset, count);
}



void
XalanSourceTreeComment::appendData(const XalanDOMString&	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeComment::insertData(
			unsigned int			/* offset */,
			const  XalanDOMString& 	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeComment::deleteData(
			unsigned int	/* offset */,
			unsigned int	/* count */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeComment::replaceData(
			unsigned int			/* offset */,
			unsigned int			/* count */,
			const XalanDOMString&	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeComment::setParent(XalanSourceTreeElement*	theParent)
{
	m_parentNode = theParent;
}



void
XalanSourceTreeComment::setParent(XalanSourceTreeDocumentFragment*	theParent)
{
	m_parentNode = theParent;
}



void
XalanSourceTreeComment::setPreviousSibling(XalanSourceTreeComment*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeComment::setPreviousSibling(XalanSourceTreeElement*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeComment::setPreviousSibling(XalanSourceTreeProcessingInstruction*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeComment::setPreviousSibling(XalanSourceTreeText*		thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeComment::appendSiblingNode(XalanSourceTreeComment*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeComment::appendSiblingNode(XalanSourceTreeElement*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeComment::appendSiblingNode(XalanSourceTreeProcessingInstruction*		theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeComment::appendSiblingNode(XalanSourceTreeText*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



static XalanDOMString	s_staticNameString;



const XalanDOMString&	XalanSourceTreeComment::s_nameString = s_staticNameString;



static const XalanDOMChar	s_comment[] =
{
	XalanUnicode::charNumberSign,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	0
};



void
XalanSourceTreeComment::initialize()
{
	s_staticNameString = s_comment;
}



void
XalanSourceTreeComment::terminate()
{
	releaseMemory(s_staticNameString);
}



XALAN_CPP_NAMESPACE_END
