/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
#include <PlatformSupport/PrefixResolver.hpp>
#include <PlatformSupport/XalanLocator.hpp>



#include <XSLT/NamespacesHandler.hpp>



class AttributeList;
class ElemTemplate;
class NamespacesHandler;
class Stylesheet;
class StylesheetConstructionContext;
class StylesheetExecutionContext;



class XALAN_XSLT_EXPORT ElemTemplateElement : public XalanElement, public PrefixResolver
{
public:

	/**
	 * Construct a template element instance.
	 * 
	 * @param constructionContext  context when object constructed
	 * @param stylesheetTree	   owning stylesheet
	 * @param lineNumber		   line in the XSLT file where the element occurs
	 * @param columnNumber		   column index in the XSLT file where the
	 *							   element occurs
	 * @param xslToken			   an integer representing the type of instance.
	 */
	ElemTemplateElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet& 					stylesheetTree,
			int 							lineNumber,
			int 							columnNumber,
			int 							xslToken);

	virtual
	~ElemTemplateElement();

	/** 
	 * Get the Locator for the node.  This may return 0.
	 * 
	 * @return The Locator for the node.
	 */
	virtual const Locator*
	getLocator() const;

	/** 
	 * See if this is a xmlns attribute or in a non-XSLT.
	 * 
	 * @param attrName qualified name of attribute
	 * @param atts	  attribute list where the element comes from (not used at 
	 *				  this time)
	 * @param which	  index into the attribute list (not used at this time)
	 * @param constructionContext The current construction context
	 * @return		  true if this is a namespace name
	 */
	virtual bool
	isAttrOK(
			const XalanDOMChar* 			attrName,
			const AttributeList&			atts,
			int 							which,
			StylesheetConstructionContext&	constructionContext) const;

	/** 
	 * Tell whether or not this is a xml:space attribute and, if so, process it.
	 * 
	 * @param atts	attribute list that owns the attribute
	 * @param which index of the attribute into the attribute list
	 * @param constructionContext The current construction context
	 * @return		true if this is a xml:space attribute
	 */
	void
	processSpaceAttr(
			const AttributeList&			atts,
			int 							which,
			StylesheetConstructionContext&	constructionContext);

	/** 
	 * Tell whether or not this is a xml:space attribute and, if so, process it.
	 * 
	 * @param aname  name of the attribute in question
	 * @param atts	 attribute list that owns the attribute
	 * @param which  index of the attribute into the attribute list
	 * @param constructionContext The current construction context
	 * @return		 true if this is a xml:space attribute
	 */
	bool
	processSpaceAttr(
			const XalanDOMChar* 			aname,
			const AttributeList&			atts,
			int 							which,
			StylesheetConstructionContext&	constructionContext);

	/** 
	 * Validate that the string is an NCName.
	 * 
	 * @param s name in question
	 * @return	true if the string is a valid NCName according to XML rules
	 * @see http://www.w3.org/TR/REC-xml-names#NT-NCName
	 */
	static bool
	isValidNCName(const XalanDOMString& 	s);

	/** 
	 * Execute the element's primary function.	Subclasses of this function may
	 * recursively execute down the element tree.
	 * 
	 * @param executionContext	The current execution context
	 */
	virtual void
	execute(StylesheetExecutionContext& 	executionContext) const;

	/** 
	 * Process the children of a template.
	 * 
	 * @param processor  XSLT processor instance
	 */
	virtual void
	executeChildren(StylesheetExecutionContext& 	executionContext) const;

	/** 
	 * Set the current node and process the children of a template.
	 * 
	 * @param processor  XSLT processor instance
	 * @param sourceNode current node
	 */
	virtual void
	executeChildren(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						sourceNode) const;

	/**
	 * Take the contents of a template element, process it, and
	 * convert it to a string.	Returns a const reference to
	 * the resulting string value.	Note that this may _not_ be
	 * the XalanDOMString instance passed in, if an optimization
	 * is available to return a pre-existing string.
	 * 
	 * @param executionContext	The current execution context
	 * @param result result of executing the elements children
	 * @result a reference to a string containing the result.
	 */
	const XalanDOMString&
	childrenToString(
			StylesheetExecutionContext& 	executionContext,
			XalanDOMString& 				result) const;

	/**
	 * Take the contents of a template element, process it, and
	 * convert it to a string.	Then, add an attribute to the
	 * result tree using the provided name and the string value.
	 * 
	 * @param executionContext	The current execution context
	 * @param theName The name for the result attribute
	 */
	void
	childrenToResultAttribute(
			StylesheetExecutionContext& 	executionContext,
			const XalanDOMString&			theName) const;

	/**
	 * Take the contents of a template element, process it, and
	 * convert it to a string.	Then, add a comment to the
	 * result tree using the string value.
	 * 
	 * @param executionContext	The current execution context
	 */
	void
	childrenToResultComment(StylesheetExecutionContext& executionContext) const;

	/**
	 * Take the contents of a template element, process it, and
	 * convert it to a string.	Then, add a processing instruction
	 * to the result tree using the string value, and the provided
	 * target.
	 * 
	 * @param executionContext	The current execution context
	 * @param theName The name for the result attribute
	 */
	void
	childrenToResultPI(
			StylesheetExecutionContext& 	executionContext,
			const XalanDOMString&			theTarget) const;

	/** 
	 * Get an integer representation of the element type.
	 * 
	 * @return integer representation of the element, defined in the Constants
	 *		   class
	 * @see class Constants
	 */
	int
	getXSLToken() const
	{
		return m_xslToken;
	}

	/** 
	 * Get the line number in the stylesheet where the element appears.
	 * Returns -1 if the information is not available.
	 *
	 * @return the line number in the stylesheet
	 */
	int
	getLineNumber() const
	{
		return m_lineNumber;
	}

	/** 
	 * Get the column number in the stylesheet where the element appears.
	 * Returns -1 if the information is not available.
	 *
	 * @return the column number in the stylesheet
	 */
	int
	getColumnNumber() const
	{
		return m_columnNumber;
	}

	/** 
	 * Tell if the element will generate text which is XML whitespace.
	 * 
	 * @return true if the text is pure whitespace
	 */
	virtual bool
	isWhitespace() const;

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
	const NamespacesHandler&
	getNamespacesHandler() const
	{
		return m_namespacesHandler;
	}

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
	postConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler);

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
	setNextSiblingElem(ElemTemplateElement* 	theSibling);

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
	setPreviousSiblingElem(ElemTemplateElement* 	theSibling);

	/**
	 * Get the parent node.
	 * 
	 * @return parent node of this node
	 */
	virtual ElemTemplateElement*
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
	setPrefix(const XalanDOMString& prefix);

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
	setAttributeNode(XalanAttr* 	newAttr);

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

	virtual const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString& prefix) const;

	virtual const XalanDOMString&
	getURI() const;


	// These optimization interfaces are new to ElemTemplateElement...
	bool
	hasParams() const
	{
		return m_optimizationFlags & eHasParams ? true : false;
	}

	bool
	hasVariables() const
	{
		return m_optimizationFlags & eHasVariables ? true : false;
	}

	bool
	hasSingleTextChild() const
	{
		return m_optimizationFlags & eHasSingleTextChild ? true : false;
	}

	bool
	hasChildren() const
	{
		return m_firstChild != 0 ? true : false;
	}

	bool
	hasDirectTemplate() const
	{
		return m_optimizationFlags & eHasDirectTemplate ? true : false;
	}

	/**
	 *
	 * Determine whether or not the instance can generate
	 */
	bool
	canGenerateAttributes() const
	{
		return m_optimizationFlags & eCanGenerateAttributes ? true : false;
	}

	class LocatorProxy : public XalanLocator
	{
	public:

		LocatorProxy(const ElemTemplateElement& 	theElement);

		virtual
		~LocatorProxy();

		virtual const XMLCh*
		getPublicId() const;

		virtual const XMLCh*
		getSystemId() const;

		virtual size_type
		getLineNumber() const;

		virtual size_type
		getColumnNumber() const;

	private:

		// Not implemented...
		LocatorProxy(const LocatorProxy&);

		LocatorProxy&
		operator=(const LocatorProxy&);

		bool
		operator==(const LocatorProxy&) const;

		// data members...
		const ElemTemplateElement&	m_element;
	};

protected:

	void
	canGenerateAttributes(bool	value)
	{
		if (value == true)
		{
			m_optimizationFlags |= eCanGenerateAttributes;
		}
		else
		{
			m_optimizationFlags &= ~eCanGenerateAttributes;
		}
	}

	/**
	 * Get the namespace for a given prefix.
	 * 
	 * @param prefix The prefix to search for
	 * @param fExists If not null, set to true if the namespace was defined.
	 * @return The namespace string.
	 */
	const XalanDOMString*
	getNamespaceForPrefixInternal(const XalanDOMString& 	prefix) const;

	/**
	 * Given an element, find the corresponding
	 * template and process the contents.
	 * 
	 * @param executionContext The current execution context
	 * @param xslInstruction The calling element (deprecated -- I dont think we 
	 *		need this).
	 * @param template The template to use if xsl:for-each, or null.
	 * @param child The source context node.
	 * @return true if applied a template, false if not.
	 */
	bool
	transformChild(
			StylesheetExecutionContext& 	executionContext,
			const ElemTemplateElement&		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						child) const;

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

	bool					m_finishedConstruction;

	/*
	 * This object handles all result tree namespace processing.
	 */
	NamespacesHandler		m_namespacesHandler;

	static const XalanDOMString 	s_emptyString;

private:

	/** 
	 * Take the contents of a template element, process it, and
	 * convert it to a string.
	 * 
	 * @param executionContext	The current execution context
	 * @param result result of executing the elements children
	 * @result a reference to a string containing the result.
	 */
	XalanDOMString&
	doChildrenToString(
			StylesheetExecutionContext& 	executionContext, 
			XalanDOMString& 				result) const;

	Stylesheet& 			m_stylesheet;

	const int				m_lineNumber;
	const int				m_columnNumber;

	bool					m_defaultSpace;

	const int				m_xslToken;

	ElemTemplateElement*	m_parentNode;
	ElemTemplateElement*	m_nextSibling;
	ElemTemplateElement*	m_previousSibling;

	union
	{
		ElemTemplateElement*	m_firstChild;
		const ElemTemplate* 	m_directTemplate;
	};

	XalanNodeListSurrogate	m_surrogateChildren;

	const XalanDOMString	m_baseIndentifier;

	enum { eHasParams = 1,
		   eHasSingleTextChild = 2,
		   eHasVariables = 4,
		   eHasDirectTemplate = 8,
		   eCanGenerateAttributes = 16 };

	unsigned				m_optimizationFlags;

	LocatorProxy			m_locatorProxy;

	static const XalanEmptyNamedNodeMap 	s_fakeAttributes;

	// Not implemented...
	ElemTemplateElement(const ElemTemplateElement&);

	ElemTemplateElement&
	operator=(const ElemTemplateElement&);

	bool
	operator==(const ElemTemplateElement&) const;
};



#endif	// XALAN_ELEMTEMPLATEELEMENT_HEADER_GUARD
