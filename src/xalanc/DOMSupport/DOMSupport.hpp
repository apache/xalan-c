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
#if !defined(DOMSUPPORT_HEADER_GUARD_1357924680)
#define DOMSUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/DOMSupport/DOMSupportDefinitions.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanAttr;
class XalanDocument;
class XalanDOMString;
class XalanElement;
class XalanNode;
class XalanText;



class XALAN_DOMSUPPORT_EXPORT DOMSupport
{
public:

	DOMSupport();

	virtual
	~DOMSupport();

	// These interfaces are inherited from Resettable...

	virtual void
	reset() = 0;

	// These interfaces are new to DOMSupport...

	/**
	 * Retrieves the URI of the named unparsed entity
	 * from the supplied document.
	 * 
	 * @param theName The name of the entity
	 * @param theDocument The document that contains the entity
	 * @return The URI of the entity
	 */
	virtual const XalanDOMString&
	getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const = 0;

	/**
	 * Determine if a node is after another node, in document order.
	 *
	 * @param node1 The first node
	 * @param node2 The second node
	 * @return true if node1 one is after node2, or false if it is not.
	 */
	virtual bool
	isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const = 0;
};



XALAN_CPP_NAMESPACE_END



#endif	// DOMSUPPORT_HEADER_GUARD_1357924680
