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



#include <memory>
#include <set>



#include <dom/DOM_Document.hpp>



#include <XalanDOM/XalanDocument.hpp>



#include <XercesParserLiaison/XercesToXalanNodeMap.hpp>
#include <XercesParserLiaison/XercesBridgeNavigator.hpp>
#include <XercesParserLiaison/XercesDocumentNamedNodeListCache.hpp>
#include <XercesParserLiaison/XercesNodeListBridge.hpp>



class XalanDOMImplementation;
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
	 * the parameter buildBridgeNodes must be true.  Otherwise, the bridge
	 * nodes will be built on demand, a process which is not synchronized.
	 * This could cause serious problems if multiple threads tried to visit
	 * an unbuilt node at the same time.
	 *
	 * @param theXercesDocument The Xerces document to bridge
	 * @param buildBridgeNodes If true, all of the bridge nodes will be built during construction.
	 *
	 */
	XercesDocumentBridge(
			const DOM_Document&		theXercesDocument,
			bool					buildBridgeNodes = true);

	virtual
	~XercesDocumentBridge();

	// These interfaces are inherited from XalanNode...

	virtual XalanDOMString
	getNodeName() const;

	virtual XalanDOMString
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

	virtual XalanDOMString
	getNamespaceURI() const;

	virtual XalanDOMString
	getPrefix() const;

	virtual XalanDOMString
	getLocalName() const;

	virtual void
	setPrefix(const XalanDOMString& prefix);

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
	XalanNode*
	mapNode(const DOM_Node& 	theXercesNode) const;

	DOM_Node
	mapNode(const XalanNode* 	theXalanNode) const;

	XalanNode*
	mapNode(NodeImpl* 	theXercesNodeImpl) const;

	DOM_Attr
	mapNode(const XalanAttr* 	theXalanNode) const;

	NodeImpl*
	mapNodeToImpl(const XalanNode* 	theXalanNode) const;

	XalanAttr*
	mapNode(const DOM_Attr& 	theXercesNode) const;

	XalanElement*
	mapNode(const DOM_Element& 	theXercesNode) const;

	DOM_Document
	getXercesDocument() const
	{
		return m_xercesDocument;
	}

private:

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
	createBridgeNode(const DOM_Node&	theXercesNode) const;

	XercesElementBridge*
	createBridgeNode(const DOM_Element& 	theXercesNode) const;

	XercesDocumentFragmentBridge*
	createBridgeNode(const DOM_DocumentFragment&	theXercesNode) const;

	XercesTextBridge*
	createBridgeNode(const DOM_Text&	theXercesNode) const;

	XercesCommentBridge*
	createBridgeNode(const DOM_Comment&		theXercesNode) const;

	XercesCDATASectionBridge*
	createBridgeNode(const DOM_CDATASection&	theXercesNode) const;

	XercesProcessingInstructionBridge*
	createBridgeNode(const DOM_ProcessingInstruction&	theXercesNode) const;

	XercesAttrBridge*
	createBridgeNode(const DOM_Attr&	theXercesNode) const;

	XercesEntityBridge*
	createBridgeNode(const DOM_Entity&	theXercesNode) const;

	XercesEntityReferenceBridge*
	createBridgeNode(const DOM_EntityReference&		theXercesNode) const;

	XercesNotationBridge*
	createBridgeNode(const DOM_Notation&	theXercesNode) const;

	// $$$ ToDo: This is because DOM_Document::getElementById() is not
	// const...
	mutable DOM_Document					m_xercesDocument;

	XercesBridgeNavigator					m_navigator;

	XercesNodeListBridge					m_children;

	XercesDocumentNamedNodeListCache		m_cachedNodeLists;

	mutable XercesToXalanNodeMap			m_nodeMap;

#if defined(XALAN_NO_NAMESPACES)
	auto_ptr<XalanDOMImplementation>		m_domImplementation;

	typedef set<XalanNode*>					NodeSetType;
#else
	std::auto_ptr<XalanDOMImplementation>	m_domImplementation;

	typedef std::set<XalanNode*>			NodeSetType;
#endif

	mutable NodeSetType						m_nodes;

	XercesDocumentTypeBridge* 				m_doctype;
};



#endif	// !defined(XERCESDOCUMENTBRIDGE_HEADER_GUARD_1357924680)
