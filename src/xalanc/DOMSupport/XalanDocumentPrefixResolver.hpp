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
#if !defined(XALANDOCUMENTPREFIXRESOLVER_HEADER_GUARD_1357924680)
#define XALANDOCUMENTPREFIXRESOLVER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/DOMSupport/DOMSupportDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/PrefixResolver.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanMap.hpp>
#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/DOMSupport/TreeWalker.hpp>



XALAN_CPP_NAMESPACE_BEGIN





class XalanAttr;
class XalanDocument;

typedef XalanVector<const XalanNode*> 				AttributeVectorTypeDecl;
XALAN_USES_MEMORY_MANAGER(AttributeVectorTypeDecl)


class XALAN_DOMSUPPORT_EXPORT XalanDocumentPrefixResolver : public PrefixResolver
{
public:

	typedef AttributeVectorTypeDecl				        AttributeVectorType;
	typedef XalanMap<const XalanDOMString*,
					AttributeVectorType>				NamespacesMapType;

	/**
	 * Constructor.
	 *
	 * @param theDocument The XalanDocument instance to use for namespace bindings.
	 * @param theURI The base URI for the instance.  This is rarely needed, but can be provided if the URI for the XalanDocument instance is known.
	 */
	XalanDocumentPrefixResolver(
			const XalanDocument*	theDocument,
			const XalanDOMString&	theURI = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            MemoryManagerType&      theManager XALAN_DEFAULT_MEMMGR);

	virtual
	~XalanDocumentPrefixResolver();


	virtual const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString&		prefix) const;

	virtual const XalanDOMString&
	getURI() const;

	/**
	 * A utility class to walk the XalanDocument instance and look for attribute nodes that declare namespaces.
	 */
	class NamespaceNodesTreeWalker : public TreeWalker
	{
	public:

		NamespaceNodesTreeWalker(NamespacesMapType&		theMap);

		virtual
		~NamespaceNodesTreeWalker();

	protected:

		virtual bool
		startNode(const XalanNode*	node);

		virtual bool
		startNode(XalanNode*	node);

		virtual bool
		endNode(const XalanNode*	node);

		virtual bool
		endNode(XalanNode*	node);

	private:

		NamespacesMapType&	m_map;
	};


protected:

	/**
	 * This function is called when more than one binding exists for a given prefix.
	 * The default behavior uses the first binding, but a deriving class can change
	 * that behavior by overriding.
	 *
	 * @param theVector The vector associated with the namespace prefix. 
	 * @return A pointer to a const XalanDOMString containing the desired binding, or NULL.
	 */
	virtual const XalanDOMString*
	duplicateBinding(const AttributeVectorType&		theVector) const;

private:

	NamespacesMapType		m_namespaces;

	const XalanDOMString	m_uri;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANDOCUMENTPREFIXRESOLVER_HEADER_GUARD_1357924680
