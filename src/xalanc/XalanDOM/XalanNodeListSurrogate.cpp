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
#include "XalanNodeListSurrogate.hpp"



#include "XalanNode.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanNodeListSurrogate::XalanNodeListSurrogate(const XalanNode&		theNode) :
	XalanNodeList(),
	m_node(&theNode)
{
}



XalanNodeListSurrogate::XalanNodeListSurrogate(const XalanNodeListSurrogate&	theSource) :
	XalanNodeList(theSource),
	m_node(theSource.m_node)
{
}



XalanNodeListSurrogate::~XalanNodeListSurrogate()
{
}



XalanNode*
XalanNodeListSurrogate::item(unsigned int	index) const
{
	XalanNode*	theCurrentChild = m_node->getFirstChild();

	for(unsigned int i = 0; i < index && theCurrentChild != 0; ++i)
	{
		theCurrentChild = theCurrentChild->getNextSibling();
	}

	return theCurrentChild;
}



unsigned int
XalanNodeListSurrogate::getLength() const
{
	unsigned int	theLength = 0;

	XalanNode*	theCurrentChild = m_node->getFirstChild();

	while(theCurrentChild != 0)
	{
		++theLength;
		theCurrentChild = theCurrentChild->getNextSibling();
	}

	return theLength;
}



XALAN_CPP_NAMESPACE_END
