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
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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
#if !defined(XALAN_ELEMTEMPLATEELEMENT_HEADER_GUARD)
#define XALAN_ELEMTEMPLATEELEMENT_HEADER_GUARD 



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <vector>
#include <map>



#include <XalanDOM/XalanDOMString.hpp>
#include <XalanDOM/XalanElement.hpp>
#include <XalanDOM/XalanEmptyNamedNodeMap.hpp>
#include <XalanDOM/XalanNodeListSurrogate.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <DOMSupport/PrefixResolver.hpp>



#include <XSLT/NodeSorter.hpp>



class AttributeList;
class NamespacesHandler;
class NodeRefListBase;
class QName;
class Stylesheet;
class StylesheetConstructionContext;
class StylesheetExecutionContext;
class XPath;



class ElemTemplateElement : public XalanElement, public PrefixResolver
{
public:

	/**
	 * Construct a template element instance.
	 * 
	 * @param constructionContext  context when object constructed
	 * @param stylesheetTree       owning stylesheet
	 * @param lineNumber           line in the XSLT file where the element occurs
	 * @param columnNumber         column index in the XSLT file where the
	 *                             element occurs
	 * @param xslToken             an integer representing the type of instance.
	 */
	ElemTemplateElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			int								lineNumber,
			int								columnNumber,
			int								xslToken);

	virtual
	~ElemTemplateElement();

	/** 
	* See if this is a xmlns attribute, and, if so, process it.
	* 
	* @param attrName qualified name of attribute
	* @param atts     attribute list where the element comes from (not used at 
	*                 this time)
	* @param which    index into the attribute list (not used at this time)
	* @return         true if this is a namespace name
	*/
	virtual bool
	isAttrOK(
			int						tok,
			const XalanDOMChar*		attrName,
			const AttributeList&	atts,
			int						which) const;

	/** 
	* See if this is a xmlns attribute or in a non-XSLT.
	* 
	* @param attrName qualified name of attribute
	* @param atts     attribute list where the element comes from (not used at 
	*                 this time)
	* @param which    index into the attribute list (not used at this time)
	* @return         true if this is a namespace name
	*/
	virtual bool
	isAttrOK(
			const XalanDOMChar*				attrName,
			const AttributeList&			atts,
			int								which,
			StylesheetConstructionContext&	constructionContext) const;

	/** 
	 * Tell whether or not this is a xml:space attribute and, if so, process it.
	 * 
	 * @param aname name of the attribute in question
	 * @param atts  attribute list that owns the attribute
	 * @param which index of the attribute into the attribute list
	 * @return      true if this is a xml:space attribute
	 */
	void
	processSpaceAttr(
			const AttributeList&	atts,
			int						which);

	/** 
	 * Tell whether or not this is a xml:space attribute and, if so, process it.
	 * 
	 * @param aname  name of the attribute in question
	 * @param atts   attribute list that owns the attribute
	 * @param which  index of the attribute into the attribute list
	 * @return       true if this is a xml:space attribute
	 */
	bool
	processSpaceAttr(
			const XalanDOMChar*		aname, 
			const AttributeList&	atts,
			int						which);

	/** 
	 * Validate that the string is an NCName.
	 * 
	 * @param s name in question
	 * @return  true if the string is a valid NCName according to XML rules
	 * @see http://www.w3.org/TR/REC-xml-names#NT-NCName
	 */
	static bool
	isValidNCName(const XalanDOMString&		s);

	/** 
	 * Execute the element's primary function.  Subclasses of this function may
	 * recursively execute down the element tree.
	 * 
	 * @param executionContext  The current execution context
	 * @param sourceTree input source tree
	 * @param sourceNode current context node
	 * @param mode       current mode
	 */
	virtual	void
	execute(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const;

	/** 
	 * Process the children of a template.
	 * 
	 * @param processor  XSLT processor instance
	 * @param sourceTree input source tree
	 * @param sourceNode current context node
	 * @param mode       current mode
	 */
	void
	executeChildren(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree, 
			XalanNode*						sourceNode,
			const QName&					mode) const;

	/** 
	 * Take the contents of a template element, process it, and
	 * convert it to a string.
	 * 
	 * @param executionContext  The current execution context
	 * @param sourceTree primary source tree
	 * @param sourceNode current source node context
	 * @param mode       current mode
	 * @param result result of executing the elements children
	 */
	void
	childrenToString(
			StylesheetExecutionContext&		executionContext, 
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode,
			XalanDOMString&					result) const;


	/** 
	 * Get an integer representation of the element type.
	 * 
	 * @return integer representation of the element, defined in the Constants
	 *         class
	 * @see class Constants
	 */
	int
	getXSLToken() const
	{
		return m_xslToken;
	}

	/** 
	 * Tell if the string is whitespace.
	 * 
	 * @param string string in question
	 * @return true if the string is pure whitespace
	 */
	static bool
	isWhitespace(const XalanDOMString& theString);

	/** 
	 * Throw a template element runtime error.  
	 * (Note: should we throw a SAXException instead?)
	 * 
	 * @param msg Description of the error that occurred
	 */
	virtual	void
	error(const XalanDOMString&		msg) const;

	/** 
	 * Throw a template element runtime error.  
	 * (Note: should we throw a SAXException instead?)
	 * 
	 * @param msg Description of the error that occurred
	 */
	virtual	void
	error(const char*	msg) const;

	/** 
	 * Get the line number where the element occurs in the xsl file.
	 * 
	 * @return line number
	 */
	int
	getLineNumber() const
	{
		return m_lineNumber;
	}

	/** 
	 * Get the column offset where the element occurs in the xsl file.
	 * 
	 * @return column number
	 */
	int
	getColumnNumber() const
	{
		return m_columnNumber;
	}

	virtual const XalanDOMString&
	getElementName() const = 0;

#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMString,
				XalanDOMString,
				less<XalanDOMString> >	StringToStringMapType;
#else
	typedef std::map<XalanDOMString,
					 XalanDOMString>	StringToStringMapType;
#endif

	/** 
	 * Get the namespaces handler for this element.
	 * 
	 * @return The element's NamespacesHandler instance.
	 */
	virtual const NamespacesHandler&
	getNamespacesHandler() const;

	/**
	 * Retrieve the stylesheet from which this element comes
	 * 
	 * @return reference to source stylesheet
	 */
	const Stylesheet&
	getStylesheet() const
	{
		return m_stylesheet;
	}

	/** 
	 * Set a flag indicating construction of the element is completed.
	 */
	void
	finishedConstruction()
	{
		m_finishedConstruction = true;
	}

	/**
	 * Called after construction is completed.
	 */
	virtual void
	postConstruction(const NamespacesHandler&	theParentHandler);

	// Type-safe getters/setters...

	/**
	 * Get the first child.
	 * 
	 * @return first child node of this node
	 */
	virtual ElemTemplateElement*
	getFirstChildElem() const;

	/**
	 * Set the first child.
	 *
	 * theChild - the new value for the first child.
	 *
	 * @return nothing
	 */
	virtual void
	setFirstChildElem(ElemTemplateElement*	theChild);

	/**
	 * Get the last child.
	 * 
	 * @return last child node of this node
	 */
	virtual ElemTemplateElement*
	getLastChildElem() const;

	/**
	 * Get the next sibling.
	 * 
	 * @return next sibling node of this node
	 */
	virtual ElemTemplateElement*
	getNextSiblingElem() const;

	/**
	 * Set the next sibling.
	 *
	 * theSibling - the new value for the next sibling.
	 *
	 * @return nothing
	 */
	virtual void
	setNextSiblingElem(ElemTemplateElement*		theSibling);

	/**
	 * Get the previous sibling.
	 * 
	 * @return previous sibling node of this node
	 */
	virtual ElemTemplateElement*
	getPreviousSiblingElem() const;

	/**
	 * Set the previous sibling.
	 *
	 * theSibling - the new value for the previous sibling.
	 *
	 * @return nothing
	 */
	virtual void
	setPreviousSiblingElem(ElemTemplateElement*		theSibling);

	/**
	 * Get the parent node.
	 * 
	 * @return parent node of this node
	 */
	virtual	ElemTemplateElement*
	getParentNodeElem() const;

	/**
	 * Set the parent node.
	 *
	 * theParent - the new value for the parent.
	 *
	 * @return nothing
	 */
	virtual void
	setParentNodeElem(ElemTemplateElement*		theParent);

	/**
	 * Append a child.
	 *
	 * theParent - the new value for the parent.
	 *
	 * @return nothing
	 */
	virtual ElemTemplateElement*
	appendChildElem(ElemTemplateElement*	newChild);

	// Type-safe getters...

	/**
	 * Append a child.
	 *
	 * @param newChild the new child to insert
	 * @param refChild the node before which to insert the new node
	 *
	 * @return newChild
	 */
	virtual ElemTemplateElement*
	insertBeforeElem(
			ElemTemplateElement*	newChild,
			ElemTemplateElement*	refChild);

	/**
	 * Replace a child.
	 *
	 * @param newChild the new child to insert
	 * @param oldChild the child to be replaced
	 *
	 * @return oldChild
	 */
	virtual ElemTemplateElement*
	replaceChildElem(
			ElemTemplateElement*	newChild,
			ElemTemplateElement*	oldChild);

	// These interfaces are inherited from XalanElement ...

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
	virtual ElemTemplateElement*
#endif
	cloneNode(bool	deep) const;

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

	/** 
	 * Add a child to the child list.
	 * 
	 * @exception DOMException 
	 * @param newChild child node to add
	 */
	virtual XalanNode*
	appendChild(XalanNode*	newChild);

	virtual bool
	hasChildNodes() const;

	virtual void
	setNodeValue(const XalanDOMString&	nodeValue);

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
	setPrefix(const XalanDOMString&	prefix);

	virtual bool
	isIndexed() const;

	virtual unsigned long
	getIndex() const;

	virtual const XalanDOMString&
	getTagName() const;

	virtual const XalanDOMString&
	getAttribute(const XalanDOMString&	name) const;

	virtual XalanAttr*
	getAttributeNode(const XalanDOMString&	name) const;

	virtual XalanNodeList*
	getElementsByTagName(const XalanDOMString&	name) const;

	virtual void
	setAttribute(
			const XalanDOMString&	name, 
			const XalanDOMString&	value);

	virtual XalanAttr*
	setAttributeNode(XalanAttr*		newAttr);

	virtual XalanAttr*
	removeAttributeNode(XalanAttr*	oldAttr);

	virtual void
	removeAttribute(const XalanDOMString&	name);

	virtual const XalanDOMString&
	getAttributeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;

	virtual void
	setAttributeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	qualifiedName,
			const XalanDOMString&	value);

	virtual void
	removeAttributeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName);

	virtual XalanAttr*
	getAttributeNodeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;

	virtual XalanAttr*
	setAttributeNodeNS(XalanAttr*	newAttr);

	virtual XalanNodeList*
	getElementsByTagNameNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;


	// These interfaces are inherited from PrefixResolver...

	virtual const XalanDOMString&
	getNamespaceForPrefix(const XalanDOMString& prefix) const;

	virtual const XalanDOMString&
	getURI() const;

protected:

	/**
	 * Get the namespace for a given prefix.
	 * 
	 * @param prefix The prefix to search for
	 * @param fReportError If true, and exception will be thrown to report the error.
	 * @return The namespace string.
	 */
	const XalanDOMString&
	getNamespaceForPrefixInternal(
			const XalanDOMString&	prefix,
			bool					fReportError) const;

	/**
	 * Perform a query if needed, and call transformChild for each child.
	 * 
	 * @param executionContext  The current execution context
	 * @param stylesheetTree The owning stylesheet tree.
	 * @param xslInstruction The stylesheet element context (deprecated -- I do 
	 *      not think we need this).
	 * @param template The owning template context.
	 * @param sourceTree The input source tree.
	 * @param sourceNodeContext The current source node context.
	 * @param mode The current mode.
	 * @param selectPattern The XPath with which to perform the selection.
	 * @param xslToken The current XSLT instruction (deprecated -- I do not     
	 *     think we want this).
	 * @param selectStackFrameIndex stack frame context for executing the
	 *                              select statement
	 */
	void
	transformSelectedChildren(
			StylesheetExecutionContext&		executionContext,
			const Stylesheet&				stylesheetTree,
			const ElemTemplateElement&		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						sourceTree,
			XalanNode*						sourceNodeContext,
			const QName&					mode,
			const XPath*					selectPattern,
			int								xslToken,
			int								selectStackFrameIndex) const;

	/**
	 * Perform a query if needed, and call transformChild for each child.
	 * 
	 * @param executionContext The current execution context
	 * @param stylesheetTree The owning stylesheet tree.
	 * @param xslInstruction The stylesheet element context (deprecated -- I do 
	 *      not think we need this).
	 * @param template The owning template context.
	 * @param sourceTree The input source tree.
	 * @param sourceNodeContext The current source node context.
	 * @param mode The current mode.
	 * @param selectPattern The XPath with which to perform the selection.
	 * @param xslToken The current XSLT instruction (deprecated -- I do not     
	 *     think we want this).
	 * @param selectStackFrameIndex stack frame context for executing the
	 *                              select statement
	 */
	void
	doTransformSelectedChildren(
			StylesheetExecutionContext&					executionContext,
			const Stylesheet&							stylesheetTree,
			const ElemTemplateElement&					xslInstruction,
			const ElemTemplateElement*					theTemplate,
			XalanNode*									sourceTree,
			XalanNode*									sourceNodeContext,
			const QName&								mode,
			int											xslToken,
			int											selectStackFrameIndex,
			const NodeSorter::NodeSortKeyVectorType&	keys,
			const NodeRefListBase&						sourceNodes,
			unsigned int								sourceNodesCount) const;

	/**
	 * Perform a query if needed, and call transformChild for each child.
	 * 
	 * @param executionContext The current execution context
	 * @param stylesheetTree The owning stylesheet tree.
	 * @param xslInstruction The stylesheet element context (deprecated -- I do 
	 *      not think we need this).
	 * @param template The owning template context.
	 * @param sourceTree The input source tree.
	 * @param sourceNodeContext The current source node context.
	 * @param mode The current mode.
	 * @param selectPattern The XPath with which to perform the selection.
	 * @param xslToken The current XSLT instruction (deprecated -- I do not     
	 *     think we want this).
	 * @param selectStackFrameIndex stack frame context for executing the
	 *                              select statement
	 */
	void
	doTransformSelectedChildren(
			StylesheetExecutionContext&					executionContext,
			const Stylesheet&							stylesheetTree,
			const ElemTemplateElement&					xslInstruction,
			const ElemTemplateElement*					theTemplate,
			XalanNode*									sourceTree,
			XalanNode*									sourceNodeContext,
			const QName&								mode,
			int											xslToken,
			int											selectStackFrameIndex,
			const NodeSorter::NodeSortKeyVectorType&	keys,
			const XalanNodeList&						childNodes,
			unsigned int								childNodeCount) const;

	void
	doTransformSelectedChildren(
			StylesheetExecutionContext&			executionContext,
			const Stylesheet&					stylesheetTree,
			const ElemTemplateElement&			xslInstruction,
			const ElemTemplateElement*			theTemplate,
			XalanNode*							sourceTree,
			XalanNode*							sourceNodeContext,
			const QName&						mode,
			int									xslToken,
			const NodeRefListBase&				sourceNodes,
			unsigned int						sourceNodesCount) const;

	/**
	 * Given an element and mode, find the corresponding
	 * template and process the contents.
	 * 
	 * @param executionContext The current execution context
	 * @param stylesheetTree The current Stylesheet object.
	 * @param xslInstruction The calling element (deprecated -- I dont think we 
	 *      need this).
	 * @param template The template to use if xsl:for-each, or null.
	 * @param sourceTree The source DOM tree.
	 * @param selectContext The selection context.
	 * @param child The source context node.
	 * @param mode The current mode, may be null.
	 * @param xslToken ELEMNAME_APPLY_TEMPLATES, ELEMNAME_APPLY_IMPORTS, or     
	 *      ELEMNAME_FOREACH.
	 * @return true if applied a template, false if not.
	 */
	bool
	transformChild(
			StylesheetExecutionContext&		executionContext,
			const Stylesheet&				stylesheetTree,
			const ElemTemplateElement*		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						sourceTree,
			XalanNode*						selectContext,
			XalanNode*						child,
			const QName&					mode,
			int								xslToken) const;

	/**
	 * Given an xsl token type, determine whether or not a child
	 * of that type is allowed.  This is so derived types can
	 * determine whether a particular type of child is allowed for
	 * the instance.  It is called from appendChildElem().
	 *
	 * The default is to allow no types (no children)
	 *
	 * @param xslToken The xsl token value of the child.
	 *
	 * @return true if the child is allowed, or false if not.
	 */
	virtual bool
	childTypeAllowed(int	xslToken) const;

protected:

	bool					m_finishedConstruction;

	static const XalanDOMString		s_emptyString;

private:

	Stylesheet&				m_stylesheet;

	const int				m_lineNumber;
	const int				m_columnNumber;

	bool					m_defaultSpace;

	const int				m_xslToken;

	ElemTemplateElement*	m_parentNode;
	ElemTemplateElement*	m_nextSibling;
	ElemTemplateElement*	m_previousSibling;
	ElemTemplateElement*	m_firstChild;

	XalanNodeListSurrogate	m_surrogateChildren;

	XalanEmptyNamedNodeMap	m_fakeAttributes;

	const XalanDOMString	m_baseIndentifier;
};



#endif	// XALAN_ELEMTEMPLATEELEMENT_HEADER_GUARD
