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

/**
 * $Id$
 * 
 * $State$
 * 
 * @author Myriam Midy (Myriam_Midy @lotus.com)
 */

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"


#include <vector>


#include <dom/DOMString.hpp>
#include <dom/DOM_Node.hpp>



#include <DOMSupport/UnimplementedElement.hpp>
#include <XPath/PrefixResolver.hpp>
#include <XPath/NameSpace.hpp>



class AttributeList;
class QName;
class Stylesheet;
class StylesheetConstructionContext;
class StylesheetExecutionContext;
class XPath;


/** 
 * An instance of this class represents an element inside
 * an xsl:template class.  It has a single "execute" method
 * which is expected to perform the given action on the
 * result tree.
 *
 * @see Stylesheet
 */

class ElemTemplateElement : public UnimplementedElement, public PrefixResolver
{
public:
	/** Construct a template element instance.
	 * 
	 * @param processor The XSLT Processor.
	 * @param stylesheetTree The owning stylesheet.
	 * @param name The name of the element.
	 * @param atts The element attributes.
	 * @param lineNumber The line in the XSLT file that the element occurs on.
	 * @param columnNumber The column index in the XSLT file that the element occurs on.
	 */
	ElemTemplateElement (
		StylesheetConstructionContext&	constructionContext,
        Stylesheet&						stylesheetTree,
        const DOMString&				name, 
        int								lineNumber,
		int								columnNumber);

	virtual
	~ElemTemplateElement();


	/** 
	* Given a namespace, get the corresponding prefix.
	*/
	DOMString getNamespaceForPrefix(const DOMString& prefix) const;

	/** 
	* See if this is a xmlns attribute, and, if so, process it.
	* 
	* @param attrName Qualified name of attribute.
	* @param atts The attribute list where the element comes from (not used at 
	*      this time).
	* @param which The index into the attribute list (not used at this time).
	* @return True if this is a namespace name.
	*/
	bool isAttrOK(
			int						tok,
			const DOMString&		attrName,
			const AttributeList&	atts,
			int						which) const;

	/** 
	* See if this is a xmlns attribute or in a non-XSLT.
	* 
	* @param attrName Qualified name of attribute.
	* @param atts The attribute list where the element comes from (not used at 
	*      this time).
	* @param which The index into the attribute list (not used at this time).
	* @return True if this attribute should not be flagged as an error.
	*/
	bool
	isAttrOK(
			const DOMString&				attrName,
			const AttributeList&			atts,
			int								which,
			StylesheetConstructionContext&	constructionContext) const;

	/** 
	 * Tell whether or not this is a xml:space attribute and, if so, process it.
	 * 
	 * @param aname The name of the attribute in question.
	 * @param atts The attribute list that owns the attribute.
	 * @param which The index of the attribute into the attribute list.
	 * @return True if this is a xml:space attribute.
	 */
	void processSpaceAttr(const AttributeList& atts, int which);

	/** 
	 * Tell whether or not this is a xml:space attribute and, if so, process it.
	 * 
	 * @param aname The name of the attribute in question.
	 * @param atts The attribute list that owns the attribute.
	 * @param which The index of the attribute into the attribute list.
	 * @return True if this is a xml:space attribute.
	 */
	bool processSpaceAttr(const DOMString& aname, 
		const AttributeList& atts, int which);


	/** 
	 * Validate that the string is an NCName.
	 * 
	 * @param s The name in question.
	 * @return True if the string is a valid NCName according to XML rules.
	 * @see http://www.w3.org/TR/REC-xml-names#NT-NCName
	 */
	static bool isValidNCName(const DOMString& s);


	/** Execute the element's primary function.  Subclasses of this
	 * function may recursivly execute down the element tree.
	 * 
	 * @param processor The XSLT Processor.
	 * @param sourceTree The input source tree.
	 * @param sourceNode The current context node.
	 * @param mode The current mode.
	 */
	virtual	void
	execute(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					sourceTree, 
			const DOM_Node&					sourceNode,
			const QName&					mode) const;

	/** 
	 * Process the children of a template.
	 * 
	 * @param processor The XSLT processor instance.
	 * @param sourceTree The input source tree.
	 * @param sourceNode The current context node.
	 * @param mode The current mode.
	 */
	void
	executeChildren(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					sourceTree, 
			const DOM_Node&					sourceNode,
			const QName&					mode) const;

	/** 
	 * Take the contents of a template element, process it, and
	 * convert it to a string.
	 * 
	 * @exception XSLProcessorException Thrown from one of the child execute  
	 *     methods.
	 * @exception java.net.MalformedURLException Might be thrown from the       
	 *      document() function, or from xsl:include or xsl:import.
	 * @exception java.io.FileNotFoundException Might be thrown from the        
	 *      document() function, or from xsl:include or xsl:import.
	 * @exception java.io.IOException Might be thrown from the  document()      
	 *      function, or from xsl:include or xsl:import.
	 * @exception SAXException Might be thrown from the  document() function, or
	 *      from xsl:include or xsl:import.
	 * @param processor The XSLT processor instance.
	 * @param sourceTree The primary source tree.
	 * @param sourceNode The current source node context.
	 * @param mode The current mode.
	 * @return The stringized result of executing the elements children.
	 */
	DOMString childrenToString(StylesheetExecutionContext& executionContext, 
		const DOM_Node&	sourceTree, const DOM_Node& sourceNode,
		const QName& mode) const;


	/** 
	 * Get an integer representation of the element type.
	 * 
	 * @return An integer representation of the element, defined in the 
	 *     Constants class.
	 * @see class Constants
	 */
	virtual int	getXSLToken() const = 0;

	/** 
	 * Tell if the string is whitespace.
	 * 
	 * @param string The string in question.
	 * @return True if the string is pure whitespace.
	 */
	static bool isWhiteSpace(const DOMString& theString);

	/** 
	 * Throw a template element runtime error.  
	 * (Note: should we throw a SAXException instead?)
	 * 
	 * @param msg Description of the error that occured.
	 **/
	virtual	void error(const DOMString& msg) const;


protected:
	/** 
	 * Perform a query if needed, and call transformChild for each child.
	 * 
	 * @param stylesheetTree The owning stylesheet tree.
	 * @param xslInstruction The stylesheet element context (depricated -- I do 
	 *      not think we need this).
	 * @param template The owning template context.
	 * @param sourceTree The input source tree.
	 * @param sourceNodeContext The current source node context.
	 * @param mode The current mode.
	 * @param selectPattern The XPath with which to perform the selection.
	 * @param xslToken The current XSLT instruction (depricated -- I do not     
	 *     think we want this).
	 */
	void transformSelectedChildren(
		StylesheetExecutionContext& executionContext,
		const Stylesheet& stylesheetTree, 
		const ElemTemplateElement& xslInstruction, // xsl:apply-templates or xsl:for-each
		const ElemTemplateElement* theTemplate, // The template to copy to the result tree
		const DOM_Node&	sourceTree, 
		const DOM_Node&	sourceNodeContext, 
		const QName& mode, 
		const XPath* selectPattern, 
		int xslToken) const;


	/**
	 * Given an element and mode, find the corresponding
	 * template and process the contents.
	 * 
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
	bool transformChild(
		StylesheetExecutionContext& executionContext,
		const Stylesheet& stylesheetTree,
		const ElemTemplateElement* xslInstruction, // xsl:apply-templates or xsl:for-each
		const ElemTemplateElement* theTemplate, // may be null
		const DOM_Node&	sourceTree, const DOM_Node& selectContext,
		const DOM_Node&	child, const QName&	mode,
		int	xslToken) const;


  // Implemented DOM Element methods.

public:

	/** 
	 * Add a child to the child list.
	 * 
	 * @exception DOMException 
	 * @param newChild 
	 */
	virtual NodeImpl* appendChild(NodeImpl* newChild);

	/*
	 *	Remove a node from the child list
	 */
	virtual NodeImpl* removeChild(NodeImpl *oldChild);

	/** 
	 * Tell if there are child nodes.
	 */
	virtual	bool hasChildNodes();

	/**
	 * Get the type of the node.
	 */
	virtual	short getNodeType();

	/** Get the parent.
	*/
	virtual	NodeImpl* getParentNode();

	virtual void setParentNode(NodeImpl* elem);

	/** Return the nodelist (same reference).
	*/
	virtual NodeListImpl* getChildNodes();

	/** Get the first child
	 */
	virtual NodeImpl* getFirstChild();

	/** Get the last child.
	 */
	virtual	NodeImpl*getLastChild();


	/** Get the next sibling or return null.
	*/
	virtual NodeImpl* getNextSibling();

	/** Set the next sibling.
	*/
	virtual	void setNextSibling(NodeImpl* elem);

	/**
	 * NodeList method: Count the immediate children of this node
	 * 
	 * @return int
	 */
	virtual	unsigned int getLength();

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual NodeImpl*
#else
	virtual ElemTemplateElement*
#endif
	cloneNode(bool deep);

	// Type-safe getters...

	/** Get the first child
	 */
	virtual ElemTemplateElement*
	getFirstChild() const;

	/** Get the next sibling or return null.
	*/
	virtual ElemTemplateElement*
	getNextSibling() const;

	virtual	ElemTemplateElement*
	getParentNode() const;

	/**
	 * NodeList method: Return the Nth immediate child of this node, or
	 * null if the index is out of bounds.
	 * 
	 * @param index 
	 * @return org.w3c.dom.Node
	 */
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual NodeImpl*
#else
	virtual ElemTemplateElement*
#endif
	item(unsigned long	i);

	const Stylesheet&
	getStylesheet() const
	{
		return m_stylesheet;
	}

	/** Return the element name.
	*/
	virtual DOMString
	getTagName();

	/** Return the node name.
	*/
	virtual DOMString
	getNodeName();

	int
	getLineNumber() const
	{
		return m_lineNumber;
	}

	int
	getColumnNumber() const
	{
		return m_columnNumber;
	}

	typedef	std::vector<NameSpace>		NamespaceVectorType;

	const NamespaceVectorType&
	getNameSpace() const
	{
		return m_namespaces;
	}

	void
	setFinishedConstruction(bool bFinished)
	{
		m_finishedConstruction = bFinished;
	}

private:

	const Stylesheet&		m_stylesheet;
	const int				m_lineNumber;
	const int				m_columnNumber;
	NamespaceVectorType 	m_namespaces;
	bool					m_defaultSpace;
	bool					m_finishedConstruction;
	const DOMString			m_elemName;
	ElemTemplateElement*	m_parentNode;
	ElemTemplateElement*	m_nextSibling;
	ElemTemplateElement*	m_firstChild;
};



#endif	// XALAN_ELEMTEMPLATEELEMENT_HEADER_GUARD
