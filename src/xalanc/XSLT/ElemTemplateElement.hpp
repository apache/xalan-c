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
#if !defined(XALAN_ELEMTEMPLATEELEMENT_HEADER_GUARD)
#define XALAN_ELEMTEMPLATEELEMENT_HEADER_GUARD 



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrefixResolver.hpp>
#include <xalanc/PlatformSupport/XalanLocator.hpp>



#include <xalanc/XPath/XalanQNameByReference.hpp>



#include <xalanc/XSLT/NamespacesHandler.hpp>



XALAN_DECLARE_XERCES_CLASS(AttributeList)
XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER AttributeList	AttributeListType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator			LocatorType;



class ElemTemplate;
class ElemTextLiteral;
class NamespacesHandler;
class Stylesheet;
class StylesheetConstructionContext;
class StylesheetExecutionContext;
class XPath;


class XALAN_XSLT_EXPORT ElemTemplateElement : public PrefixResolver
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

	/**
	 * Special constructor used by dummy elements which do not exist in the
	 * final stylesheet.
	 * 
	 * @param constructionContext  context when object constructed
	 * @param stylesheetTree	   owning stylesheet
	 * @param baseURI              The base URI of the XSLT file where the element occurs.
	 * @param lineNumber		   line in the XSLT file where the element occurs
	 * @param columnNumber		   column index in the XSLT file where the
	 *							   element occurs
	 * @param xslToken			   an integer representing the type of instance.
	 */
	ElemTemplateElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet& 					stylesheetTree,
			int 							xslToken,
			const XalanDOMString&			baseURI = XalanDOMString(),
			int 							lineNumber = XalanLocator::getUnknownValue(),
			int 							columnNumber = XalanLocator::getUnknownValue());

	virtual
	~ElemTemplateElement();

	/** 
	 * Get the Locator for the node.  This may return 0.
	 * 
	 * @return The Locator for the node.
	 */
	virtual const LocatorType*
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
			const AttributeListType&		atts,
			int 							which,
			StylesheetConstructionContext&	constructionContext) const;

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
			const AttributeListType&		atts,
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

	/** 
	 * Get a string for the name of the element.  Useful for debugging purposes,
	 * and error reporting.
	 * 
	 * @return A string containing the name of the element.
	 */
	virtual const XalanDOMString&
	getElementName() const = 0;

	/** 
	 * Get the QName associated with any name attribute of this element.
	 * If the element has no name attribute, this will be an empty QName.
	 * 
	 * @return A string containing the name of the element.
	 */
	virtual const XalanQName&
	getNameAttribute() const;

	/** 
	 * Called during compilation when an instance is not parented by
	 * another element, and thus, is a child of the stylesheet..
	 * 
	 * @param constructionContext  The current construction context
	 * @param theStylesheet The owning stylesheet
	 * @return nothing
	 */
	virtual void
	addToStylesheet(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						theStylesheet);

	/** 
	 * Called during compilation to process xsl:sort elements in
	 * the stylesheet.
	 *
	 * @param constructionContext  The current construction context
	 * @param theStylesheet The owning stylesheet
	 * @param atts The list of attributes for the sort element
	 * @param locator A Locator instance for error reporting, if available.
	 *
	 * @return nothing
	 */
	virtual void
	processSortElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						theStylesheet,
			const AttributeListType&		atts,
			const LocatorType*				locator = 0);

	/**
	 * Sets a flag indicating this is the default template
	 *
	 * @param value The value of flag.
	 */
	virtual void
	setDefaultTemplate(bool		value);

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
	 * Set the flag indicating construction of the element is completed.
	 *
	 * @param fValue  The value to set
	 */
	void
	setFinishedConstruction(bool	fValue)
	{
		setFlag(eFinishedConstruction, fValue);
	}

	/** 
	 * Get the flag indicating construction of the element is completed.
	 */
	bool
	getFinishedConstruction() const
	{
		return getFlag(eFinishedConstruction);
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

	/**
	 * Get XPath pattern/expression of one of the element atribute
	 *
	 * @index	number of the attribute. If an attribute has more
	 *				then one attribute with pattern/expression,the order of 
	 *				the returned expressions are undefined
	 *
	 * @return pointer or null 
	 */
	virtual	const XPath*
	getXPath(unsigned int	index = 0) const;

	// These interfaces are inherited from PrefixResolver...

	virtual const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString& prefix) const;

	virtual const XalanDOMString&
	getURI() const;

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

	bool
	hasParams() const
	{
		return getFlag(eHasParams);
	}

	bool
	hasVariables() const
	{
		return getFlag(eHasVariables);
	}

	bool
	hasSingleTextChild() const
	{
		return getFlag(eHasSingleTextChild);
	}

	bool
	hasChildren() const
	{
		return m_firstChild != 0 ? true : false;
	}

	bool
	hasDirectTemplate() const
	{
		return getFlag(eHasDirectTemplate);
	}

	bool
	canGenerateAttributes() const
	{
		return getFlag(eCanGenerateAttributes);
	}

	bool
	isDefaultTemplate() const
	{
		return getFlag(eDefaultTemplate);
	}

protected:

	void
	canGenerateAttributes(bool	value)
	{
		setFlag(eCanGenerateAttributes, value);
	}

	void
	hasPrefix(bool	value)
	{
		setFlag(eHasPrefix, value);
	}

	bool
	hasPrefix() const
	{
		return getFlag(eHasPrefix);
	}

	void
	preserveSpace(bool	value)
	{
		setFlag(eSpacePreserve, value);
	}

	bool
	preserveSpace() const
	{
		return getFlag(eSpacePreserve);
	}

	void
	disableOutputEscaping(bool	value)
	{
		setFlag(eDisableOutputEscaping, value);
	}

	bool
	disableOutputEscaping() const
	{
		return getFlag(eDisableOutputEscaping);
	}

	/**
	 * Process the exclude-result-prefixes or the extension-element-prefixes
	 * attributes, for the purpose of prefix exclusion.
	 *
	 * @param constructionContext  context when object consructed
	 * @param stylesheetTree The current Stylesheet object.
	 * @param localName The local name of the attribute.
	 * @param attrValue The value of the attribute.
	 */
	bool
	processPrefixControl(
			StylesheetConstructionContext&	constructionContext,
			const Stylesheet&				stylesheetTree,
			const XalanDOMString&			localName,
			const XalanDOMChar*				attrValue);

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
	 * @param xslInstruction The calling element
	 * @param template The template to use if xsl:for-each, or null.
	 * @param child The source context node.
	 */
	virtual void
	transformChild(
			StylesheetExecutionContext& 	executionContext,
			const ElemTemplateElement&		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						child) const;

	/**
	 * Given an element, find the corresponding
	 * template and process the contents.
	 * 
	 * @param executionContext The current execution context
	 * @param xslInstruction The calling element
	 * @param template The template to use if xsl:for-each, or null.
	 * @param child The source context node.
	 * @param nodeType The type of child.
	 */
	virtual void
	transformChild(
			StylesheetExecutionContext& 	executionContext,
			const ElemTemplateElement&		xslInstruction,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						child,
			XalanNode::NodeType				nodeType) const;

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

	/**
	 * Called after construction is completed.  This is a hook for
	 * deriving classes to handle post-construction with the
	 * instances HamespaceHandler instance, which is otherwise only
	 * available through a const accessor.
	 */
	virtual void
	namespacesPostConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler,
			NamespacesHandler&				theHandler);

	static const XalanDOMString 	s_emptyString;

	const NamespacesHandler&
	getNamespaces() const
	{
		return m_namespacesHandler;
	}

private:

	enum eFlags
	{ 
		eHasParams = 1,
		eHasSingleTextChild = 2,
		eHasVariables = 4,
		eHasDirectTemplate = 8,
		eCanGenerateAttributes = 16,
		eDefaultTemplate = 32,
		eSpacePreserve = 64,
		eFinishedConstruction = 128,
		eHasPrefix = 256,
		eDisableOutputEscaping = 512
	};

	bool
	getFlag(eFlags	theFlag) const
	{
		return m_flags & theFlag ? true : false;
	}

	void
	setFlag(
			eFlags	theFlag,
			bool	theValue)
	{
		if (theValue == true)
		{
			m_flags |= theFlag;
		}
		else
		{
			m_flags &= ~theFlag;
		}
	}

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

	/*
	 * This object handles all result tree namespace processing.
	 */
	NamespacesHandler		m_namespacesHandler;

	const int				m_lineNumber;
	const int				m_columnNumber;

	const int				m_xslToken;

	ElemTemplateElement*	m_parentNode;
	ElemTemplateElement*	m_nextSibling;
	ElemTemplateElement*	m_previousSibling;

	union
	{
		ElemTemplateElement*	m_firstChild;
		const ElemTemplate* 	m_directTemplate;
		const ElemTextLiteral*	m_textLiteralChild;
	};

	const XalanDOMString&	m_baseIndentifier;

	LocatorProxy			m_locatorProxy;

	unsigned short			m_flags;

	static const XalanQNameByReference		s_emptyQName;

	// Not implemented...
	ElemTemplateElement(const ElemTemplateElement&);

	ElemTemplateElement&
	operator=(const ElemTemplateElement&);

	bool
	operator==(const ElemTemplateElement&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMTEMPLATEELEMENT_HEADER_GUARD
