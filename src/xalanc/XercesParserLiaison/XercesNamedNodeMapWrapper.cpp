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
#include "XercesNamedNodeMapWrapper.hpp"



#include <algorithm>
#include <cassert>



#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNode.hpp>



#include "XercesDocumentWrapper.hpp"
#include "XercesDOMWrapperException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesNamedNodeMapWrapper::XercesNamedNodeMapWrapper(
			const DOMNamedNodeMapType*		theXercesNamedNodeMap,
			const XercesWrapperNavigator&	theNavigator) :
	m_xercesNamedNodeMap(theXercesNamedNodeMap),
	m_navigator(theNavigator)
{
	assert(m_xercesNamedNodeMap != 0);
}



XercesNamedNodeMapWrapper::~XercesNamedNodeMapWrapper()
{
}



XalanNode*
XercesNamedNodeMapWrapper::setNamedItem(XalanNode* 	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanNode*
XercesNamedNodeMapWrapper::item(unsigned int	index) const
{
	return m_navigator.mapNode(m_xercesNamedNodeMap->item(index));
}



XalanNode*
XercesNamedNodeMapWrapper::getNamedItem(const XalanDOMString& 	name) const
{
	return m_navigator.mapNode(m_xercesNamedNodeMap->getNamedItem(name.c_str()));
}



unsigned int
XercesNamedNodeMapWrapper::getLength() const
{
	return m_xercesNamedNodeMap->getLength();
}



XalanNode*
XercesNamedNodeMapWrapper::removeNamedItem(const XalanDOMString&		/* name */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanNode*
XercesNamedNodeMapWrapper::getNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	return m_navigator.mapNode(
			m_xercesNamedNodeMap->getNamedItemNS(
				namespaceURI.c_str(),
				localName.c_str()));
}



XalanNode*
XercesNamedNodeMapWrapper::setNamedItemNS(XalanNode*		/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanNode*
XercesNamedNodeMapWrapper::removeNamedItemNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XALAN_CPP_NAMESPACE_END
