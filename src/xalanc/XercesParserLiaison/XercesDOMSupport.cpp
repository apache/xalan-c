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
#include "XercesDOMSupport.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanAttr.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesDOMSupport::XercesDOMSupport() :
	DOMSupport(),
	m_domSupportDefault()
{
}



XercesDOMSupport::~XercesDOMSupport()
{
}



void
XercesDOMSupport::reset()
{
	m_domSupportDefault.reset();
}



const XalanDOMString&
XercesDOMSupport::getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const
{
	return m_domSupportDefault.getUnparsedEntityURI(theName, theDocument);
}



bool
XercesDOMSupport::isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const
{
	return DOMServices::isNodeAfter(node1, node2);
}



XALAN_CPP_NAMESPACE_END
