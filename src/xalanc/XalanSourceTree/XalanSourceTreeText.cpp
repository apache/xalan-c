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
#include "XalanSourceTreeText.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocumentFragment.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeProcessingInstruction.hpp"
#include "XalanSourceTreeHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString		s_emptyString;



XalanSourceTreeText::XalanSourceTreeText(
			const XalanDOMString&	theData,
			XalanNode*				theParentNode,
			XalanNode*				thePreviousSibling,
			XalanNode*				theNextSibling,
			IndexType				theIndex) :
	XalanText(),
	m_data(theData),
	m_parentNode(theParentNode),
	m_previousSibling(thePreviousSibling),
	m_nextSibling(theNextSibling),
	m_index(theIndex)
{
}



XalanSourceTreeText::~XalanSourceTreeText()
{
}



XalanSourceTreeText::XalanSourceTreeText(
			const XalanSourceTreeText&	theSource,
			bool						/* deep */) :
	XalanText(theSource),
	m_data(theSource.m_data),
	m_parentNode(0),
	m_previousSibling(0),
	m_nextSibling(0),
	m_index(0)
{
}



const XalanDOMString&
XalanSourceTreeText::getNodeName() const
{
	return s_nameString;
}



const XalanDOMString&
XalanSourceTreeText::getNodeValue() const
{
	return m_data;
}



XalanSourceTreeText::NodeType
XalanSourceTreeText::getNodeType() const
{
	return TEXT_NODE;
}



XalanNode*
XalanSourceTreeText::getParentNode() const
{
	return m_parentNode;
}



const XalanNodeList*
XalanSourceTreeText::getChildNodes() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeText::getFirstChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeText::getLastChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeText::getPreviousSibling() const
{
	return m_previousSibling;
}



XalanNode*
XalanSourceTreeText::getNextSibling() const
{
	return m_nextSibling;
}



const XalanNamedNodeMap*
XalanSourceTreeText::getAttributes() const
{
	return 0;
}



XalanDocument*
XalanSourceTreeText::getOwnerDocument() const
{
	assert(m_parentNode != 0);

	return m_parentNode->getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeText*
#endif
XalanSourceTreeText::cloneNode(bool		/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeText::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeText::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeText::removeChild(XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeText::appendChild(XalanNode*	/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
XalanSourceTreeText::hasChildNodes() const
{
	return false;
}



void
XalanSourceTreeText::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeText::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeText::isSupported(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
XalanSourceTreeText::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeText::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeText::getLocalName() const
{
	return s_emptyString;
}



void
XalanSourceTreeText::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeText::isIndexed() const
{
	return true;
}



XalanSourceTreeText::IndexType
XalanSourceTreeText::getIndex() const
{
	return m_index;
}



const XalanDOMString&
XalanSourceTreeText::getData() const
{
	return m_data;
}



unsigned int
XalanSourceTreeText::getLength() const
{
	assert(unsigned(length(m_data)) == length(m_data));

	return unsigned(length(m_data));
}



XalanDOMString
XalanSourceTreeText::substringData(
			unsigned int	offset,
			unsigned int	count) const
{
	return m_data.substr(offset, count);
}



void
XalanSourceTreeText::appendData(const XalanDOMString&	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeText::insertData(
			unsigned int			/* offset */,
			const  XalanDOMString& 	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeText::deleteData(
			unsigned int	/* offset */,
			unsigned int	/* count */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeText::replaceData(
			unsigned int			/* offset */,
			unsigned int			/* count */,
			const XalanDOMString&	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanText*
XalanSourceTreeText::splitText(unsigned int		/* offset */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
	return 0;
}



bool
XalanSourceTreeText::isIgnorableWhitespace() const
{
	return false;
}



void
XalanSourceTreeText::setParent(XalanSourceTreeElement*	theParent)
{
	m_parentNode = theParent;
}



void
XalanSourceTreeText::setParent(XalanSourceTreeDocumentFragment*	theParent)
{
	m_parentNode = theParent;
}



void
XalanSourceTreeText::setPreviousSibling(XalanSourceTreeComment*		thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeText::setPreviousSibling(XalanSourceTreeElement*		thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeText::setPreviousSibling(XalanSourceTreeProcessingInstruction*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeText::setPreviousSibling(XalanSourceTreeText*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeText::appendSiblingNode(XalanSourceTreeComment*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeText::appendSiblingNode(XalanSourceTreeElement*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeText::appendSiblingNode(XalanSourceTreeProcessingInstruction*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeText::appendSiblingNode(XalanSourceTreeText*		theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



static XalanDOMString	s_staticNameString;



const XalanDOMString&	XalanSourceTreeText::s_nameString = s_staticNameString;



const XalanDOMChar	s_text[] =
{
	XalanUnicode::charNumberSign,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_t,
	0
};



void
XalanSourceTreeText::initialize()
{
	s_staticNameString = s_text;
}



void
XalanSourceTreeText::terminate()
{
	releaseMemory(s_staticNameString);
}



XALAN_CPP_NAMESPACE_END
