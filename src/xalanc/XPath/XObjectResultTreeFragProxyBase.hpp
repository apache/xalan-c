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
#if !defined(XOBJECTRESULTTREEFRAGPROXYBASE_HEADER_GUARD_1357924680)
#define XOBJECTRESULTTREEFRAGPROXYBASE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>

#include <xalanc/Include/XalanMemoryManagement.hpp>


#include <xalanc/XalanDOM/XalanNodeList.hpp>
#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>



#include <xalanc/XPath/XObjectResultTreeFragProxyText.hpp>




XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XObjectResultTreeFragProxyBase : public XalanDocumentFragment
{
public:

	XObjectResultTreeFragProxyBase();

	virtual
	~XObjectResultTreeFragProxyBase();


	// These interfaces are inherited from XalanDocumentFragment...

	virtual const XalanDOMString&
	getNodeName() const;

	virtual const XalanDOMString&
	getNodeValue() const;

	virtual NodeType
	getNodeType() const;

	virtual XalanNode*
	getParentNode() const;

	virtual const XalanNodeList*
	getChildNodes() const;

	virtual XalanNode*
	getFirstChild() const = 0;

	virtual XalanNode*
	getLastChild() const = 0;

	virtual XalanNode*
	getPreviousSibling() const;

	virtual XalanNode*
	getNextSibling() const;

	virtual const XalanNamedNodeMap*
	getAttributes() const;

	virtual XalanDocument*
	getOwnerDocument() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanNode*
#else
	virtual XObjectResultTreeFragProxyBase*
#endif
	cloneNode(bool deep) const;

	virtual XalanNode*
	insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild);

	virtual XalanNode*
	replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild);

	virtual XalanNode*
	removeChild(XalanNode*	oldChild);

	virtual XalanNode*
	appendChild(XalanNode*	newChild);

	virtual bool
	hasChildNodes() const = 0;

	virtual void
	setNodeValue(const XalanDOMString&	nodeValue);

	virtual void
	normalize();

	virtual bool
	isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const;

	virtual const XalanDOMString&
	getNamespaceURI() const;

	virtual const XalanDOMString&
	getPrefix() const;

	virtual const XalanDOMString&
	getLocalName() const;

	virtual void
	setPrefix(const XalanDOMString&		prefix);

	virtual bool
	isIndexed() const;

	virtual IndexType
	getIndex() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanDocumentFragment*
#else
	virtual XObjectResultTreeFragProxyBase*
#endif
	clone(bool	deep) const;

protected:

	XObjectResultTreeFragProxyBase(const XObjectResultTreeFragProxyBase&	theRHS);

private:

	// Not implemented...
	XObjectResultTreeFragProxyBase&
	operator=(const XObjectResultTreeFragProxyBase&	theRHS);

	bool
	operator==(const XObjectResultTreeFragProxyBase&	theRHS);


	// Data members...
	static const XalanDOMString				s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// XOBJECTRESULTTREEFRAGPROXYBASE_HEADER_GUARD_1357924680
