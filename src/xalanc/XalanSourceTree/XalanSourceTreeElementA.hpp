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
#if !defined(XALANSOURCETREEELEMENTA_HEADER_GUARD_1357924680)
#define XALANSOURCETREEELEMENTA_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeElement.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeElementA : public XalanSourceTreeElement, private XalanNamedNodeMap
{
public:

	typedef unsigned long	AttributesCountType;

	/**
	 * Constructor.
	 *
	 * @param theTagName The tag name of the element
	 * @param theOwnerDocument The document that owns the instance
	 * @param theAttributes An array of pointers to the attribute instances for the element
	 * @param theAttributeCount The number of attributes.
	 * @param theParentNode The parent node, if any.
	 * @param thePreviousSibling The previous sibling, if any.
	 * @param theNextSibling The next sibling, if any.
	 * @param theIndex The document-order index of the node.
	 */
	XalanSourceTreeElementA(
            MemoryManagerType&          theManager,
			const XalanDOMString&		theTagName,
			XalanSourceTreeDocument*	theOwnerDocument,
			XalanSourceTreeAttr**		theAttributes,
			AttributesCountType			theAttributeCount,
			XalanNode*					theParentNode = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0,
			IndexType					theIndex = 0);

	virtual
	~XalanSourceTreeElementA();


	virtual const XalanNamedNodeMap*
	getAttributes() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanNode*
#else
	virtual XalanSourceTreeElementA*
#endif
	cloneNode(bool deep) const;

	virtual const XalanDOMString&
	getNamespaceURI() const;

	virtual const XalanDOMString&
	getPrefix() const;

	virtual const XalanDOMString&
	getLocalName() const;

	virtual const XalanDOMString&
	getAttribute(const XalanDOMString&		name) const;

	virtual XalanAttr*
	getAttributeNode(const XalanDOMString&		name) const;

	virtual const XalanDOMString&
	getAttributeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;

	virtual XalanAttr*
	getAttributeNodeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;


	// public interfaces not inherited from XalanElement...

	AttributesCountType
	getAttributeCount() const
	{
		return m_attributeCount;
	}

	XalanSourceTreeAttr*
	getAttributeByIndex(AttributesCountType		index) const
	{
		return index < m_attributeCount ? m_attributes[index] : 0;
	}

	XalanSourceTreeElementA*
	clone(bool	deep) const
	{
        MemoryManagerType& theManager = const_cast<XalanSourceTreeElementA*>(this)->getMemoryManager();

        return XalanSourceTreeElementA::create(theManager, *this, deep);
	}

protected:

	XalanSourceTreeElementA(
            MemoryManagerType&              theManager,
			const XalanSourceTreeElementA&	theSource,
			bool							deep = false);

    static XalanSourceTreeElementA*
    create(
            MemoryManagerType&              theManager,
			const XalanSourceTreeElementA&	theSource,
			bool							deep = false);

private:

	// These are from XalanNamedNodeMap...

	virtual XalanNode*
	setNamedItem(XalanNode* 	arg);

	virtual XalanNode*
	item(unsigned int	index) const;

	virtual XalanNode*
	getNamedItem(const XalanDOMString& 	name) const;

	virtual unsigned int
	getLength() const;

	virtual XalanNode*
	removeNamedItem(const XalanDOMString&	name);

	virtual XalanNode*
	getNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;

	virtual XalanNode*
	setNamedItemNS(XalanNode*	arg);

	virtual XalanNode*
	removeNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName);


	// Not implemented...
	XalanSourceTreeElementA&
	operator=(const XalanSourceTreeElementA&	theSource);

	bool
	operator==(const XalanSourceTreeElementA&	theRHS) const;


	// Data members...
	XalanSourceTreeAttr* const *	m_attributes;

	const AttributesCountType		m_attributeCount;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANSOURCETREEELEMENTA_HEADER_GUARD_1357924680)
