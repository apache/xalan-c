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
#if !defined(XALANSOURCETREEELEMENTNA_HEADER_GUARD_1357924680)
#define XALANSOURCETREEELEMENTNA_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanDOM/XalanEmptyNamedNodeMap.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeElement.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeElementNA : public XalanSourceTreeElement
{
public:

	/**
	 * Constructor.
	 *
	 * @param theTagName The tag name of the element
	 * @param theOwnerDocument The document that owns the instance
	 * @param theParentNode The parent node, if any.
	 * @param thePreviousSibling The previous sibling, if any.
	 * @param theNextSibling The next sibling, if any.
	 * @param theIndex The document-order index of the node.
	 */
	XalanSourceTreeElementNA(
            MemoryManagerType&          theManager,
			const XalanDOMString&		theTagName,
			XalanSourceTreeDocument*	theOwnerDocument,
			XalanNode*					theParentNode = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0,
			IndexType					theIndex = 0);

	virtual
	~XalanSourceTreeElementNA();


	virtual const XalanNamedNodeMap*
	getAttributes() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanNode*
#else
	virtual XalanSourceTreeElementNA*
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

	XalanSourceTreeElementNA*
	clone(bool	deep) const
	{
        MemoryManagerType& theManager = const_cast<XalanSourceTreeElementNA*>(this)->getMemoryManager();

        return XalanSourceTreeElementNA::create(theManager, *this, deep);
	}

protected:

	XalanSourceTreeElementNA(
            MemoryManagerType&                  theManager,
			const XalanSourceTreeElementNA&		theSource,
			bool								deep = false);

	static XalanSourceTreeElementNA*
    create(
            MemoryManagerType&                  theManager,
			const XalanSourceTreeElementNA&		theSource,
			bool								deep = false);

	static const XalanEmptyNamedNodeMap		s_emptyAttributes;

private:

	// Not implemented...
	XalanSourceTreeElementNA&
	operator=(const XalanSourceTreeElementNA&	theSource);

	bool
	operator==(const XalanSourceTreeElementNA&	theRHS) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANSOURCETREEELEMENTNA_HEADER_GUARD_1357924680)
