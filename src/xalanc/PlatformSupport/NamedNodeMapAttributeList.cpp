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

#include "NamedNodeMapAttributeList.hpp"


#include <xalanc/XalanDOM/XalanAttr.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



#include "DOMStringHelper.hpp"
#include "XalanUnicode.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar	NamedNodeMapAttributeList::s_typeString[] = 
{
	XalanUnicode::charLetter_C,
	XalanUnicode::charLetter_D,
	XalanUnicode::charLetter_A,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_A,
	0
};



NamedNodeMapAttributeList::NamedNodeMapAttributeList(const XalanNamedNodeMap&	theMap) :
	ParentType(),
	m_nodeMap(theMap),
	m_lastIndex(theMap.getLength() - 1)
{
}



NamedNodeMapAttributeList::~NamedNodeMapAttributeList()
{
}



unsigned int
NamedNodeMapAttributeList::getLength() const
{
	return m_nodeMap.getLength();
}



const XMLCh*
NamedNodeMapAttributeList::getName(const unsigned int index) const
{
	const XalanNode* const	theAttribute = m_nodeMap.item(m_lastIndex - index);
	assert(theAttribute != 0);

	return c_wstr(theAttribute->getNodeName());
}



const XMLCh*
NamedNodeMapAttributeList::getType(const unsigned int /* index */) const
{
	assert(length(s_typeString) > 0);

	return s_typeString;
}



const XMLCh*
NamedNodeMapAttributeList::getValue(const unsigned int index) const
{
	const XalanNode* const	theAttribute = m_nodeMap.item(m_lastIndex - index);
	assert(theAttribute != 0);

	return c_wstr(theAttribute->getNodeValue());
}



const XMLCh*
NamedNodeMapAttributeList::getType(const XMLCh* const /* name */) const
{
	assert(length(s_typeString) > 0);

	return c_wstr(s_typeString);
}



const XMLCh*
NamedNodeMapAttributeList::getValue(const XMLCh* const name) const
{
	const XalanNode*	theNode = m_nodeMap.getNamedItem(XalanDOMString(name));

	if (theNode == 0)
	{
		return 0;
	}
	else
	{
		return c_wstr(theNode->getNodeValue());
	}
}



const XMLCh* 
NamedNodeMapAttributeList::getValue(const char* const name) const
{
	return getValue(c_wstr(TranscodeFromLocalCodePage(name)));
}



XALAN_CPP_NAMESPACE_END
