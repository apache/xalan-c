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
#include "XercesWrapperToXalanNodeMap.hpp"



#include <cassert>



#include <xercesc/dom/DOMDocument.hpp>



#include <xalanc/XalanDOM/XalanDOMException.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesWrapperToXalanNodeMap::XercesWrapperToXalanNodeMap() :
	m_xercesMap()
{
}



XercesWrapperToXalanNodeMap::~XercesWrapperToXalanNodeMap()
{
}



void
XercesWrapperToXalanNodeMap::addAssociation(
			const DOMNodeType*	theXercesNode,
			XalanNode*			theXalanNode)
{
	m_xercesMap.insert(XercesNodeMapType::value_type(theXercesNode, theXalanNode));
}



void
XercesWrapperToXalanNodeMap::clear()
{
	m_xercesMap.clear();
}



XalanNode*
XercesWrapperToXalanNodeMap::getNode(const DOMNodeType*		theXercesNode) const
{
	const XercesNodeMapType::const_iterator		i =
				m_xercesMap.find(theXercesNode);

	if (i == m_xercesMap.end())
	{
		return 0;
	}
	else
	{
		return (*i).second;
	}
}



XALAN_CPP_NAMESPACE_END
