/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(XERCESDOCUMENTBRIDGE_HEADER_GUARD_1357924680)
#define XERCESDOCUMENTBRIDGE_HEADER_GUARD_1357924680



#include <XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <deque>
#include <set>
#include <vector>



#include <dom/DOM_Document.hpp>



#include <XalanDOM/XalanDocument.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <XalanDOM/XalanDOMImplementation.hpp>
#include <PlatformSupport/XalanDOMStringPool.hpp>
#endif



#include <Include/XalanAutoPtr.hpp>



#include <DOMSupport/TreeWalker.hpp>



#include <XercesParserLiaison/XercesToXalanNodeMap.hpp>
#include <XercesParserLiaison/XercesBridgeNavigator.hpp>
#include <XercesParserLiaison/XercesNodeListBridge.hpp>
#include <XercesParserLiaison/XercesTreeWalker.hpp>
#include <XercesParserLiaison/XercesElementBridgeAllocator.hpp>
#include <XercesParserLiaison/XercesTextBridgeAllocator.hpp>
#include <XercesParserLiaison/XercesAttributeBridgeAllocator.hpp>



class XalanDOMStringPool;
class XercesAttrBridge;
class XercesCommentBridge;
class XercesCDATASectionBridge;
class XercesDocumentFragmentBridge;
class XercesDocumentTypeBridge;
class XercesElementBridge;
class XercesEntityBridge;
class XercesEntityReferenceBridge;
class XercesProcessingInstructionBridge;
class XercesNotationBridge;
class XercesTextBridge;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDocumentBridge : public XalanDocument
{
public:

	friend class XercesBridgeNavigator;

	/**
	 *
	 * Constructor for XercesDocumentBridge.
	 *
	 * If the document will be shared amongst multiple threads of execution,
	 * the parameter buildBridge must be true.  Otherwise, the bridge
	 * nodes will be built on demand, a process which is not synchronized.
	 * This could cause serious problems if multiple threads tried to visit
	 * an unbuilt node at the same time.
	 *
	 * @param theXercesDocument The Xerces document to bridge
	 * @param threadSafe If true, the tree can safely be shared amongst multiple threads.  (Also implies buildBridge == true)
	 * @param buildBridge If true, all of the bridge nodes will be built during construction.
	 *
	 */
	XercesDocumentBridge(
			const DOM_Document&		theXercesDocument,
			bool					threadSafe = true,
			bool					buildBridge = true);

	virtual
	~XercesDocumentBridge();

	// These interfaces are inherited from XalanNode...

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
	getFirstChild() const;

	virtual XalanNode*
	getLastChild() const;

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
	virtual XercesDocumentBridge*
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
	hasChildNodes() const;

	virtual void
	setNodeValue(const XalanDOMString&		nodeValue);

	virtual void
	normalize();

	virtual bool
	supports(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const;

	virtual const XalanDOMString&
	getNamespaceURI() const;

	virtual const XalanDOMString&
	getPrefix() const;

	virtual const XalanDOMString&
	getLocalName() const;

	virtual void
	setPrefix(const XalanDOMString& prefix);

	virtual bool
	isIndexed() const;

	virtual unsigned long
	getIndex() const;

	virtual XalanElement*
	createElement(const XalanDOMString& tagName);

	virtual XalanDocumentFragment*
	createDocumentFragment();

	virtual XalanText*
	createTextNode(const XalanDOMString&	data);

	virtual XalanComment*
	createComment(const XalanDOMString& data);

	virtual XalanCDATASection*
	createCDATASection(const XalanDOMString&	data);

	virtual XalanProcessingInstruction*
	createProcessingInstruction(
			const XalanDOMString&	target,
			const XalanDOMString&	data);

	virtual XalanAttr*
	createAttribute(const XalanDOMString&	name);

	virtual XalanEntityReference*
	createEntityReference(const XalanDOMString& name);

	virtual XalanDocumentType*
	getDoctype() const;

	virtual XalanDOMImplementation*
	getImplementation() const;

	virtual XalanElement*
	getDocumentElement() const;

	virtual XalanNodeList*
	getElementsByTagName(const XalanDOMString&		tagname) const;

	virtual XalanNode*
	importNode(
			XalanNode*	importedNode,
			bool		deep);

	virtual XalanElement*
	createElementNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	qualifiedName);

	virtual XalanAttr*
	createAttributeNS(
			const XalanDOMString& namespaceURI,
			const XalanDOMString& qualifiedName);

	virtual XalanNodeList*
	getElementsByTagNameNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;

	virtual XalanElement*
	getElementById(const XalanDOMString&	elementId) const;

	
	// These are some special interfaces to manage relationships between
	// our nodes and Xerces nodes.

	/**
	 * Destroy the entire bridge structure that connects
	 * the Xerces document to this XercesDocumentBridge
	 * instance.  This will invalidate any pointers to
	 * any nodes in the document (except, of course, the
	 * document itself).
	 */
	void
	destroyBridge();

	/**
	 * Rebuild the entire bridge structure that connects
	 * the Xerces document to this XercesDocumentBridge
	 * instance.  This destroys the bridge before
	 * rebuilding.
	 */
	void
	rebuildBridge();

	XalanNode*
	mapNode(const DOM_Node& 	theXercesNode) const;

	XalanAttr*
	mapNode(const DOM_Attr& 	theXercesNode) const;

	XalanElement*
	mapNode(const DOM_Element& 	theXercesNode) const;

	DOM_Node
	mapNode(const XalanNode* 	theXalanNode) const;

	DOM_Attr
	mapNode(const XalanAttr* 	theXalanNode) const;

	NodeImpl*
	mapNodeToImpl(const XalanNode* 	theXalanNode) const;

	/**
	 *
	 * Get the Xerces DOM_Document that this XercesDocument represents.
	 *
	 * @return the Xerces DOM_Document instance.
	 *
	 */
	DOM_Document
	getXercesDocument() const
	{
		return m_xercesDocument;
	}

	/**
	 * Build the entire bridge structure.  This should be done before any
	 * processing begins, if the tree will be shared amongst multiple
	 * threads.
	 */
	void
	buildBridgeNodes();

#if defined(XALAN_NO_NAMESPACES)
	typedef deque<XercesBridgeNavigator>	NavigatorBridgeVectorType;

	typedef deque<XalanNode*>				NodeVectorType;
#else
	typedef std::deque<XercesBridgeNavigator>	NavigatorBridgeVectorType;

	typedef std::deque<XalanNode*>				NodeVectorType;
#endif

	// Helper class to walk the tree and build everything...
	class BuildBridgeTreeWalker : public XercesTreeWalker
	{
	public:

		typedef NavigatorBridgeVectorType	NavigatorBridgeVectorInnerType;

		BuildBridgeTreeWalker(
				XercesDocumentBridge*			theDocument,
				XercesBridgeNavigator*			theDocumentNavigator,
				NavigatorBridgeVectorInnerType&	theNavigators,
				unsigned long					theStartIndex);

		virtual
		~BuildBridgeTreeWalker();

		struct NavigatorStackEntryType
		{
			NavigatorStackEntryType(
						XercesBridgeNavigator*	theNavigator = 0,
						XalanNode*				theNode = 0) :
				m_navigator(theNavigator),
				m_node(theNode)
			{
			}

			XercesBridgeNavigator*	m_navigator;

			XalanNode*				m_node;
		};

	#if defined(XALAN_NO_NAMESPACES)
		typedef vector<NavigatorStackEntryType>			NavigatorStackType;
	#else
		typedef std::vector<NavigatorStackEntryType>	NavigatorStackType;
	#endif

	protected:

		virtual void
		startNode(const DOM_Node&	node);

		virtual void
		endNode(const DOM_Node&	node);

	private:

		XercesDocumentBridge*			m_document;

		NavigatorBridgeVectorInnerType&	m_navigators;

		unsigned long					m_currentIndex;

		NavigatorStackType				m_parentNavigatorStack;

		NavigatorStackType				m_siblingNavigatorStack;
	};


	/**
	 * Get a pooled string.  If the string is not in the pool,
	 * add it.
	 *
	 * @param theString The string to pool.
	 * @return A const reference to the pooled string.
	 */
	const XalanDOMString&
	getPooledString(const XalanDOMString&	theString) const;

	/**
	 * Get a pooled string.  If the string is not in the pool,
	 * add it.
	 *
	 * @param theString The string to pool.
	 * @return A const reference to the pooled string.
	 */
	const XalanDOMString&
	getPooledString(const XalanDOMChar*		theString) const;

private:

	XalanNode*
	mapNode(NodeImpl* 	theXercesNodeImpl) const;

	// Destruction API...
	void
	destroyBridgeNode(XalanNode*	theNode);

	// Not implemented...
	XercesDocumentBridge(const XercesDocumentBridge&	theSource);

	XercesDocumentBridge&
	operator=(const XercesDocumentBridge&	theRHS);

	bool
	operator==(const XercesDocumentBridge&	theRHS) const;

	// Private delete function...
	void
	destroyNode(XalanNode*	theNode);

	// More internal implementation stuff...
	XalanNode*
	internalCloneNode(
			const XalanNode*	theXalanNode,
			const DOM_Node&		theXercesNode,
			bool				deep);

	// Factory methods for our implementation nodes...
	XalanNode*
	createBridgeNode(
			const DOM_Node&	theXercesNode,
			unsigned long	theIndex,
			bool			mapNode) const;

	XercesDocumentTypeBridge*
	createBridgeNode(
			const DOM_DocumentType&		theDoctype,
			unsigned long				theIndex,
			bool						mapNode) const;

	XercesElementBridge*
	createBridgeNode(
			const DOM_Element& 	theXercesNode,
			unsigned long		theIndex,
			bool				mapNode) const;

	XercesDocumentFragmentBridge*
	createBridgeNode(
			const DOM_DocumentFragment&		theXercesNode,
			unsigned long					theIndex,
			bool							mapNode) const;

	XercesTextBridge*
	createBridgeNode(
			const DOM_Text&		theXercesNode,
			unsigned long		theIndex,
			bool				mapNode) const;

	XercesCommentBridge*
	createBridgeNode(
			const DOM_Comment&	theXercesNode,
			unsigned long		theIndex,
			bool				mapNode) const;

	XercesCDATASectionBridge*
	createBridgeNode(
			const DOM_CDATASection&		theXercesNode,
			unsigned long				theIndex,
			bool						mapNode) const;

	XercesProcessingInstructionBridge*
	createBridgeNode(
			const DOM_ProcessingInstruction&	theXercesNode,
			unsigned long						theIndex,
			bool								mapNode) const;

	XercesAttrBridge*
	createBridgeNode(
			const DOM_Attr&		theXercesNode,
			unsigned long		theIndex,
			bool				mapNode) const;

	XercesEntityBridge*
	createBridgeNode(
			const DOM_Entity&	theXercesNode,
			unsigned long		theIndex,
			bool				mapNode) const;

	XercesEntityReferenceBridge*
	createBridgeNode(
			const DOM_EntityReference&	theXercesNode,
			unsigned long				theIndex,
			bool						mapNode) const;

	XercesNotationBridge*
	createBridgeNode(
			const DOM_Notation&		theXercesNode,
			unsigned long			theIndex,
			bool					mapNode) const;

	XercesBridgeNavigator&
	pushNavigator(bool	mappingMode) const;

	// This is a private helper class for building the tree...
	friend class BuildBridgeTreeWalker;

	// $$$ ToDo: This is because DOM_Document::getElementById() is not
	// const...
	mutable DOM_Document					m_xercesDocument;

	XalanElement*							m_documentElement;

	XercesNodeListBridge					m_children;

	mutable XercesToXalanNodeMap			m_nodeMap;

	XalanAutoPtr<XalanDOMImplementation>	m_domImplementation;

	mutable NavigatorBridgeVectorType		m_navigators;

	// Our navigator will be the first entry in m_navigators,
	// but we'll cache this so access is faster...
	XercesBridgeNavigator&					m_navigator;

	mutable NodeVectorType					m_nodes;

	mutable XercesDocumentTypeBridge* 		m_doctype;

	bool									m_mappingMode;

	bool									m_indexValid;

	mutable XercesElementBridgeAllocator	m_elementAllocator;

	mutable XercesTextBridgeAllocator		m_textAllocator;

	mutable XercesAttributeBridgeAllocator	m_attributeAllocator;

	XalanAutoPtr<XalanDOMStringPool>		m_stringPool;
};



#endif	// !defined(XERCESDOCUMENTBRIDGE_HEADER_GUARD_1357924680)
