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
#if !defined(XALANDOCUMENTFRAGMENT_HEADER_GUARD_1357924680)
#define XALANDOCUMENTFRAGMENT_HEADER_GUARD_1357924680



#include <xalanc/XalanDOM/XalanDOMDefinitions.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/*
 * <meta name="usage" content="experimental"/>
 *
 * Base class for the DOM DocumentFragment interface.
 *
 * This class is experimental and subject to change!!
 */

class XALAN_DOM_EXPORT XalanDocumentFragment : public XalanNode
{
public:

	XalanDocumentFragment();

	virtual
	~XalanDocumentFragment();

	// These interfaces are inherited from XalanNode...

	virtual const XalanDOMString&
	getNodeName() const = 0;

	virtual const XalanDOMString&
	getNodeValue() const = 0;

	virtual NodeType
	getNodeType() const = 0;

	virtual XalanNode*
	getParentNode() const = 0;

	virtual const XalanNodeList*
	getChildNodes() const = 0;

	virtual XalanNode*
	getFirstChild() const = 0;

	virtual XalanNode*
	getLastChild() const = 0;

	virtual XalanNode*
	getPreviousSibling() const = 0;

	virtual XalanNode*
	getNextSibling() const = 0;

	virtual const XalanNamedNodeMap*
	getAttributes() const = 0;

	virtual XalanDocument*
	getOwnerDocument() const = 0;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanNode*
#else
	virtual XalanDocumentFragment*
#endif
	cloneNode(bool deep) const = 0;

	virtual XalanNode*
	insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild) = 0;

	virtual XalanNode*
	replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild) = 0;

	virtual XalanNode*
	removeChild(XalanNode*	oldChild) = 0;

	virtual XalanNode*
	appendChild(XalanNode*	newChild) = 0;

	virtual bool
	hasChildNodes() const = 0;

	virtual void
	setNodeValue(const XalanDOMString&	nodeValue) = 0;

	virtual void
	normalize() = 0;

	virtual bool
	isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const = 0;

	virtual const XalanDOMString&
	getNamespaceURI() const = 0;

	virtual const XalanDOMString&
	getPrefix() const = 0;

	virtual const XalanDOMString&
	getLocalName() const = 0;

	virtual void
	setPrefix(const XalanDOMString&		prefix) = 0;

	virtual bool
	isIndexed() const = 0;

	virtual IndexType
	getIndex() const = 0;

protected:

	XalanDocumentFragment(const XalanDocumentFragment&	theSource);

	XalanDocumentFragment&
	operator=(const XalanDocumentFragment&	theSource);

	bool
	operator==(const XalanDocumentFragment&		theRHS) const;

private:
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANDOCUMENTFRAGMENT_HEADER_GUARD_1357924680)
