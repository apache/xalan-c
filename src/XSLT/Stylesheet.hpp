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
 *
 * $ Id: $
 *
 */

#if !defined(XALAN_STYLESHEET_HEADER_GUARD)
#define XALAN_STYLESHEET_HEADER_GUARD



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <deque>
#include <map>
#include <vector>



#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanEmptyNamedNodeMap.hpp>
#include <XalanDOM/XalanNodeListSurrogate.hpp>


#include <PlatformSupport/PrefixResolver.hpp>



#include <XPath/NameSpace.hpp>
#include <XPath/XalanQNameByReference.hpp>
#include <XPath/XPath.hpp>



#include <XSLT/NamespacesHandler.hpp>
#include <XSLT/KeyDeclaration.hpp>
#include <XSLT/StylesheetExecutionContext.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class ExtensionNSHandler;
class XalanDecimalFormatSymbols;
class ElemAttributeSet;
class ElemDecimalFormat;
class ElemTemplate;
class ElemTemplateElement;
class ElemVariable;
class KeyTable;
class NodeRefListBase;
class PrefixResolver;
class StylesheetConstructionContext;
class StylesheetRoot;
class XalanQName;
class XObject;
class StylesheetExecutionContext;



/**
 * This class represents the base stylesheet or an "import" stylesheet.
 * "include" stylesheets are combined with the including stylesheet.
 */
class XALAN_XSLT_EXPORT Stylesheet : public XalanDocument, protected PrefixResolver
{	

public:

#if defined(XALAN_STRICT_ANSI_HEADERS)
	typedef std::size_t		size_type;
#else
	typedef size_t			size_type;
#endif

	typedef StylesheetExecutionContext::ParamVectorType		ParamVectorType;
	typedef XalanQName::NamespaceVectorType					NamespaceVectorType;
	typedef XalanQName::NamespacesStackType					NamespacesStackType;

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef map<XalanDOMString,
				ExtensionNSHandler*,
				less<XalanDOMString> >				ExtensionNamespacesMapType;
	typedef map<XalanQNameByReference,
				const ElemTemplate*,
				less<XalanQName> >					ElemTemplateMapType;
	typedef vector<ElemAttributeSet*> 				AttributeSetVectorType;
	typedef vector<ElemVariable*> 					ElemVariableVectorType;
	typedef vector<KeyDeclaration>					KeyDeclarationVectorType;
	typedef map<const XalanNode*,
				KeyTable*,
				less<const XalanNode*> >			KeyTablesTableType;
	typedef vector<Stylesheet*>						StylesheetVectorType;
	typedef vector<XalanDOMString>					URLStackType;
	typedef vector<ElemDecimalFormat*>				ElemDecimalFormatVectorType;
#else
	typedef std::map<XalanDOMString, ExtensionNSHandler*>	ExtensionNamespacesMapType;
	typedef std::map<XalanQNameByReference,
					 const ElemTemplate*,
					 std::less<XalanQName> >				ElemTemplateMapType;
	typedef std::vector<ElemAttributeSet*> 					AttributeSetVectorType;
	typedef std::vector<ElemVariable*> 						ElemVariableVectorType;
	typedef std::vector<KeyDeclaration>						KeyDeclarationVectorType;
	typedef std::map<const XalanNode*, KeyTable*>			KeyTablesTableType;
	typedef std::vector<Stylesheet*>						StylesheetVectorType;
	typedef std::vector<XalanDOMString>						URLStackType;
	typedef std::vector<ElemDecimalFormat*>					ElemDecimalFormatVectorType;
#endif

	/**
	 * Constructor for a Stylesheet needs a Document.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	Stylesheet(
			StylesheetRoot& 				root,
			const XalanDOMString&			baseIdentifier,
			StylesheetConstructionContext&	constructionContext);

	virtual
	~Stylesheet();

	/**
	 * Retrieve XSLT version number
	 * 
	 * @return number representing XSLT version
	 */
	double
	getXSLTVerDeclared() const
	{
		return m_XSLTVerDeclared;
	}

	/**
	 * Set XSLT version number
	 * 
	 * @param ver number representing XSLT version
	 */
	void
	setXSLTVerDeclared(double ver)
	{
		m_XSLTVerDeclared = ver;
	}

	/**
	 * Retrieve the root stylesheet object
	 * 
	 * @return const reference to the stylesheet object
	 */
	const StylesheetRoot&
	getStylesheetRoot() const
	{
		return m_stylesheetRoot;
	}

	/**
	 * Retrieve the root stylesheet object
	 * 
	 * @return reference to the stylesheet object
	 */
	StylesheetRoot&
	getStylesheetRoot()
	{
		return m_stylesheetRoot;
	}

	/**
	 * Retrieve the stack of namespace lists
	 * 
	 * @return vector of namespace vectors
	 */
	const NamespacesStackType&
	getNamespaces() const
	{ 
		return m_namespaces;
	}

	/**
	 * Retrieve the stack of namespace lists
	 * 
	 * @return vector of namespace vectors
	 */
	NamespacesStackType&
	getNamespaces()
	{ 
		return m_namespaces;
	}

	const NamespacesHandler&
	getNamespacesHandler() const
	{
		return m_namespacesHandler;
	}

	NamespacesHandler&
	getNamespacesHandler()
	{
		return m_namespacesHandler;
	}

	/**
	 * Retrieve the list of namespace declarations currently in effect
	 * 
	 * @return vector of namespace vectors
	 */
	const NamespaceVectorType&
	getNamespaceDecls() const
	{
		return m_namespaceDecls;
	}

	/**
	 * Retrieve the list of namespace declarations currently in effect
	 * 
	 * @return vector of namespace vectors
	 */
	NamespaceVectorType&
	getNamespaceDecls()
	{
		return m_namespaceDecls;
	}

	/**
	 * Set the list of namespace declarations currently in effect
	 * 
	 * @param ns vector of namespace vectors
	 */
	void
	setNamespaceDecls(const NamespaceVectorType& ns)
	{
		m_namespaceDecls = ns;
	}

	/** 
	 * Push the namespace declarations from the current attribute 
	 * list onto the namespace stack.
	 * 
	 * @param atts attribute list constaining namespaces
	 */
	void
	pushNamespaces(const AttributeListType& 	atts);

	/**
	 * Pop a namespace declaration from the namespace stack.
	 */
	void
	popNamespaces()
	{
		assert(m_namespaces.empty() == false);

		m_namespaces.pop_back(); 
	}

	/**
	 * Called after construction is completed.
	 */
	virtual void
	postConstruction(StylesheetConstructionContext&		constructionContext);

	/** 
	 * See if this is a xmlns attribute, and, if so, process it.
	 * 
	 * @param attrName qualified name of attribute
	 * @param atts	   attribute list where the element comes from (not used at 
	 *				   this time)
	 * @param which    index into the attribute list (not used at this time)
	 * @return		   true if this is a namespace name
	 */
	bool
	isAttrOK(
			const XalanDOMChar*				attrName,
			const AttributeListType&		atts,
			int 							which,
			StylesheetConstructionContext&	constructionContext) const;

	/**
	 * Get the namespace from a qualified name.
	 * 
	 * @param nodeName name of node
	 * @return namespace string for node, or null if not found.
	 */
	const XalanDOMString*
	getNamespaceFromStack(const XalanDOMString& 	nodeName) const
	{
		return getNamespaceFromStack(c_wstr(nodeName));
	}

	/**
	 * Get the namespace from a qualified name.
	 * 
	 * @param nodeName name of node
	 * @return namespace string for node, or null if not found.
	 */
	const XalanDOMString*
	getNamespaceFromStack(const XalanDOMChar* 	nodeName) const;

	/**
	 * Get the namespace from a prefix by searching the stack of namespace
	 * lists.
	 * 
	 * @param prefix prefix to search
	 * @return namespace corresponding to prefix, or null if not found.
	 */
	const XalanDOMString*
	getNamespaceForPrefixFromStack(const XalanDOMString&	prefix) const
	{
		return XalanQName::getNamespaceForPrefix(m_namespaces, prefix);
	}

	/**
	 * Get the namespace from a prefix by searching the stack of namespace
	 * lists.
	 * 
	 * @param prefix prefix to search
	 * @return namespace corresponding to prefix, or null if not found.
	 */
	const XalanDOMString*
	getNamespaceForPrefixFromStack(const XalanDOMChar*	prefix) const
	{
		assert(prefix != 0);

		return XalanQName::getNamespaceForPrefix(m_namespaces, XalanDOMString(prefix));
	}

	/**
	 * See if a namespace should be excluded.
	 * 
	 * @param theConstructionContext the current construction context.
	 * @param theValue the prefix of the namespace.
	 * @return
	 */
	void
	processExcludeResultPrefixes(
		StylesheetConstructionContext&	theConstructionContext,
		const XalanDOMChar*				theValue)
	{
		m_namespacesHandler.processExcludeResultPrefixes(
				theConstructionContext,
				theValue,
				m_namespaces);
	}

	/**
	 * Add a template to the list of names templates
	 * 
	 * @param theTemplate template to add
	 * @param constructionContext context for construction
	 */
	void
	addTemplate(
			ElemTemplate*					theTemplate,
			StylesheetConstructionContext&	constructionContext);

	/**
	 * Process an attribute that has the value of 'yes' or 'no'.
	 * 
	 * @param aname name of attribute
	 * @param val value
	 * @param constructionContext context for construction
	 * @return true if value equals string constant for "yes," false otherwise
	 */
	bool
	getYesOrNo(
			const XalanDOMChar*				aname,
			const XalanDOMChar*				val,
			StylesheetConstructionContext&	constructionContext) const;

	/**
	 * Tell if this is the root of the stylesheet tree.
	 * 
	 * @return true if it is the root
	 */
	bool
	isRoot() const
	{
		return m_isRoot;
	}

	/**
	 * Retrieve the base identifier with which this stylesheet is associated.
	 * 
	 * @return string for base identifier
	 */
	const XalanDOMString&
	getBaseIdentifier() const
	{
		return m_baseIdent;
	}

	/**
	 * Set the base identifier with which this stylesheet is associated.
	 * 
	 * @param str string for base identifier
	 */
	void
	setBaseIdentifier(const XalanDOMString&		str)
	{
		m_baseIdent = str;
	}

	/**
	 * Retrieve the base identifier for the most recently
	 * included stylesheet.  This will return the same value
	 * as getBaseIdentifier(), if no include is being
	 * processed.
	 * 
	 * @return string for base identifier
	 */
	const XalanDOMString&
	getCurrentIncludeBaseIdentifier() const
	{
		return m_includeStack.empty() == true ? getBaseIdentifier() : m_includeStack.back();
	}

	/**
	 * Process an xsl:namespace-alias element.
	 *
	 * @param name   the element name.
	 * @param attrs	 the current attribute list
	 * @param constructionContext  the active construction context
	 */
	void
	processNSAliasElement(
			const XalanDOMChar*				name,
			const AttributeListType&		atts,
			StylesheetConstructionContext&	constructionContext);

	/**
	 * Process an xsl:decimal-format element.
	 *
	 * @param elemDecimalFormat   the element
	 */
	void
	processDecimalFormatElement(
			StylesheetConstructionContext&	constructionContext,
			const AttributeListType&		atts,
			const LocatorType*				locator = 0);

	/**
	 * Retrieve the XalanDecimalFormatSymbols instance associated with
	 * the QName.
	 *
	 * @param theQName the QName for the lookup
	 * @return a pointer to the matching instance, or 0 if none was found
	 */
	const XalanDecimalFormatSymbols*
	getDecimalFormatSymbols(const XalanQName&	theQName) const;

	/**
	 * Add an attribute set to the list.
	 *
	 * @param attrSet pointer to attribute set to add
	 */
	void
	addAttributeSet(ElemAttributeSet*	attrSet)
	{
		assert(attrSet != 0);

		m_attributeSets.push_back(attrSet);
	}

	/**
	 * Apply the set of named attributes to a node in a given context with a
	 * given mode
	 *
	 * @param attributeSetsNames The vector of attribute set names
	 * @param attributeSetsNamesCount The size of the vector
	 * @param executionContext	 The current execution context
	 * @param sourceNode		 The source node
	 */
	void
	applyAttrSets(
			const XalanQName**				attributeSetsNames,
			size_type						attributeSetsNamesCount,
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						sourceNode) const;

	/**
	 * Add an imported stylesheet.
	 *
	 * @param theStylesheet The stylesheet to add.
	 */
	void
	addImport(Stylesheet*	theStylesheet)
	{
		m_imports.insert(m_imports.begin(), theStylesheet);
	}

	/**
	 * whether there is a wrapper template
	 * 
	 * @return true is there is a wrapper
	 */
	bool
	isWrapperless() const
	{
		return m_isWrapperless;
	}

	ElemTemplateElement*
	initWrapperless(
			StylesheetConstructionContext&	constructionContext,
			const LocatorType*				locator);

	/**
	 * Retrieve the stack of who's including who
	 * 
	 * @return stack of includes
	 */
	URLStackType&
	getIncludeStack()
	{
		return m_includeStack;
	}

	/**
	 * Process the xsl:key element.
	 * 
	 * @param nsContext The PrefixResolver instance for namespace prefixes.
	 * @param atts The attribute list for element.
	 * #param locator The Locator instance for error reporting, if any.  May be 0.    
	 * @param constructionContext The current construction context.
	 */
	void
	processKeyElement(
			const PrefixResolver&			nsContext,
			const AttributeListType&		atts,
			const LocatorType*				locator,
			StylesheetConstructionContext&	constructionContext);

	/**
	 * Locate a template via the "name" attribute.
	 * 
	 * @param name				 qualified name of template
	 * @return pointer to template found or 0 if none found
	 */
	const ElemTemplate*
	findNamedTemplate(const XalanQName&		qname) const;

	/**
	 * Given a target element, find the template that best matches in the given
	 * XSL document, according to the rules specified in the xsl draft. 
	 *
	 * @param executionContext current execution context
	 * @param targetNode	   element that needs a rule
	 * @return				   pointer to rule that best matches targetNode
	 */
	const ElemTemplate*
	findTemplate(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						targetNode) const
	{
		return findTemplate(executionContext, targetNode, s_emptyQName, false);
	}

	/**
	 * Given a target element, find the template that best matches in the given
	 * XSL document, according to the rules specified in the xsl draft. 
	 *
	 * @param executionContext current execution context
	 * @param targetElem        element that needs a rule
	 * @param mode              string indicating the display mode
	 * @param onlyUseImports    only use imports, do not use any templates from the stylesheet itself
	 * @return pointer to rule that best matches targetElem
	 */
	const ElemTemplate*
	findTemplate(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						targetNode, 
			const XalanQName&				mode,
			bool							onlyUseImports) const;

	/**
	 * A class to contain a match pattern and it's corresponding template.
	 * This class also defines a node in a match pattern linked list.
	 */
	class MatchPattern2
	{
	public:

		typedef XPath::eMatchScore	eMatchScore;

		/**
		 * Construct a match pattern from a pattern and template.
		 *
		 * @param theTemplate node that contains the template for this pattern
		 * @param posInStylesheet position in stylesheet
		 * @param targetString target string
		 * @param matchPattern the match pattern
		 * @param pattern the pattern string
		 * @param priority the default priority
		 */
		MatchPattern2(
				const ElemTemplate&		theTemplate,
				size_type 				posInStylesheet,
				const XalanDOMString&	targetString,
				const XPath&			matchPattern,
				const XalanDOMString&	pattern,
				eMatchScore				priority) :
			m_template(&theTemplate),
			m_posInStylesheet(posInStylesheet),
			m_targetString(targetString),
			m_matchPattern(&matchPattern),
			m_pattern(&pattern),
			m_priority(priority)
		{
		}

		MatchPattern2() :
			m_template(0),
			m_posInStylesheet(0),
			m_targetString(),
			m_matchPattern(0),
			m_pattern(0),
			m_priority(XPath::eMatchScoreNone)
		{
		}

		~MatchPattern2()
		{
		}

		/**
		 * Retrieve string for target.
		 * 
		 * @return target string
		 */
		const XalanDOMString&
		getTargetString() const
		{
			return m_targetString;
		}

		/**
		 * Retrieve the match pattern associated with pattern.
		 * 
		 * @return XPath for pattern
		 */
		const XPath*
		getExpression() const
		{
			return m_matchPattern;
		}

		/**
		 * Retrieve position of pattern in stylesheet.
		 * 
		 * @return position in stylesheet
		 */
		size_type
		getPositionInStylesheet() const
		{
			return m_posInStylesheet;
		}

		/**
		 * Retrieve pattern string.
		 * 
		 * @return string that contains element pattern
		 */
		const XalanDOMString*
		getPattern() const
		{
			return m_pattern;
		}

		/**
		 * Retrieve node that contains the template for this pattern.
		 * 
		 * @return template node
		 */
		const ElemTemplate*
		getTemplate() const
		{
			return m_template;
		}

		eMatchScore
		getDefaultPriority() const
		{
			return m_priority;
		}

		double
		getPriorityOrDefault() const;

	private:

		const ElemTemplate*		m_template;
		size_type				m_posInStylesheet;
		XalanDOMString			m_targetString;
		const XPath*			m_matchPattern;
		const XalanDOMString*	m_pattern;
		eMatchScore				m_priority;
	};

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<const MatchPattern2*>		PatternTableListType;

	typedef vector<const MatchPattern2*>		PatternTableVectorType;

	typedef map<XalanDOMString,
			    PatternTableListType,
				less<XalanDOMString> >			PatternTableMapType;

	typedef deque<MatchPattern2>				MatchPattern2Container;
#else
	typedef std::vector<const MatchPattern2*>	PatternTableListType;

	typedef std::vector<const MatchPattern2*>	PatternTableVectorType;

	typedef std::map<XalanDOMString,
					 PatternTableListType>		PatternTableMapType;

	typedef std::deque<MatchPattern2>			MatchPattern2Container;
#endif

	/**
	 * Add object to vector of match patterns if not already there.
	 *
	 * @param thePattern pattern to add
	 * @param theVector  vector of patterns to add to
	 */
	static void
	addObjectIfNotFound(
			const MatchPattern2*		thePattern,
			PatternTableVectorType& 	theVector);

	/**
	 * Add object to array of match patterns if not already there.
	 * theArraySize size will be incremented if the pattern was
	 * added.
	 *
	 * @param thePattern pattern to add
	 * @param theArray  vector of patterns to add to
	 * @param theArraySize The size of the array
	 */
	static void
	addObjectIfNotFound(
			const MatchPattern2*	thePattern,
			const MatchPattern2* 	theArray[],
			unsigned int&			theArraySize);

	/**
	 * Given a name, locate the start of a list of 
	 * possible templates that match that name.  If
	 * none match, then use the default list.
	 *
	 * @param theName The name to match
	 */
	const PatternTableListType*
	locateElementMatchPatternList2(const XalanDOMString&	theName) const;

	/**
	 * Given a name, locate the start of a list of 
	 * possible templates that match that name.  If
	 * none match, then use the default list.
	 *
	 * @param theName The name to match
	 */
	const PatternTableListType*
	locateAttributeMatchPatternList2(const XalanDOMString&	theName) const;

	/**
	 * Given a XalanNode, locate the start of a list of 
	 * possible templates that match it.
	 *
	 * @param XalanNode The node to match
	 */
	const PatternTableListType*
	locateMatchPatternList2(const XalanNode&	theNode) const;

	/**
	 * Add an extension namespace handler. This provides methods for calling
	 * an element extension as well as for function calls (which is passed
	 * on to XPath).
	 *
	 * @param constructionContext The current construction context.
	 * @param uri The namespace URI of the extension.
	 */
	void
	processExtensionNamespace(
			StylesheetConstructionContext&	theConstructionContext,
			const XalanDOMString&			uri);

	/**
	 * Return the handler for a given extension namespace.
	 *
	 * @param uri the URI of the extension namespace.
	 * @return pointer to extension handler
	 */
	ExtensionNSHandler*
	lookupExtensionNSHandler(const XalanDOMString&	uri) const
	{
		const ExtensionNamespacesMapType::const_iterator	it = 
		  m_extensionNamespaces.find(uri);

		return it == m_extensionNamespaces.end() ? 0 : (*it).second;
	}

	/**
	 * Set a top level variable.
	 *
	 * @param var top-level variable declared with "xsl:variable" or
	 *			  xsl:param-variable.
	 */
	void
	setTopLevelVariable(ElemVariable*	var)
	{
		m_topLevelVariables.push_back(var);
	}

	/**
	 * Set a list of top level variables in the specified execution context
	 * stylesheet.
	 *
	 * @param executionContext current execution context
	 * @param topLevelParams   list of top level parameters
	 */
	void
	pushTopLevelVariables(
			StylesheetExecutionContext& 	executionContext,
			const ParamVectorType&			topLevelParams) const;

	// These interfaces are inherited from XalanDocument...

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
	virtual Stylesheet*
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
	createEntityReference(const XalanDOMString &name);

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

	virtual bool
	isIndexed() const;

	// These interfaces are inherited from PrefixResolver...

	virtual const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString& 	prefix) const;

	virtual const XalanDOMString&
	getURI() const;

	const XalanDOMString&
	getXSLTNamespaceURI() const
	{
		return m_XSLTNamespaceURI;
	}

	void
	setXSLTNamespaceURI(const XalanDOMString&	theURI)
	{
		m_XSLTNamespaceURI = theURI;
	}

protected:

	/**
	 * The root of the stylesheet tree.
	 */
	StylesheetRoot& 					m_stylesheetRoot;

	/**
	 * The base URL of the XSL document.
	 */
	XalanDOMString						m_baseIdent;

	/**
	 * Table of KeyDeclaration objects, which are set by the 
	 * xsl:key element.
	 */
	KeyDeclarationVectorType			m_keyDeclarations;

	static const XalanQNameByReference	s_emptyQName;

private:	

	// Not defined...
	Stylesheet(const Stylesheet&);

	Stylesheet&
	operator=(const Stylesheet&);

	bool
	operator==(const Stylesheet&) const;


	/**
	 * Given a target element, find the template that best matches in the given
	 * stylesheet, using only imports
	 *
	 * @param executionContext current execution context
	 * @param targetElem        element that needs a rule
	 * @param mode              string indicating the display mode
	 * @return pointer to rule that best matches targetElem
	 */
	const ElemTemplate*
	findTemplateInImports(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						targetNode, 
			const XalanQName&				mode) const;

	/**
	 * The full XSLT Namespace URI.  To be replaced by the one actually
	 * found.
	 */
	XalanDOMString							m_XSLTNamespaceURI;

	/**
	 * A vector of the -imported- XSL Stylesheets.
	 */
	StylesheetVectorType					m_imports;

	StylesheetVectorType::size_type			m_importsSize;

	/**
	 * A stack to keep track of the result tree namespaces.
	 */
	NamespacesStackType 					m_namespaces;

	/** 
	 * A list of namespace declarations,
	 * for mapping from prefix to namespace URI.
	 */
	NamespaceVectorType 					m_namespaceDecls;

	/**
	 * Tells if the stylesheet is without an xsl:stylesheet and xsl:template
	 * wrapper.
	 */
	bool									m_isWrapperless;

	/**
	 * The table of extension namespaces.
	 */
	ExtensionNamespacesMapType				m_extensionNamespaces;

	/**
	 * The first template of the template children.
	 */
	ElemTemplate*							m_firstTemplate;

	/**
	 * A stack of who's including who is needed in order to support "It is an
	 * error if a stylesheet directly or indirectly includes itself."
	 */
	URLStackType							m_includeStack;

	/**
	 * Keyed on string macro names, and holding values that are macro elements
	 * in the XSL DOM tree. Initialized in initMacroLookupTable, and used in
	 * findNamedTemplate.
	 */
	ElemTemplateMapType						m_namedTemplates;

	/**
	 * Table for defined constants, keyed on the names.
	 */
	ElemVariableVectorType					m_topLevelVariables;

	/**
	 * The version of XSL that was declared.
	 */
	double									m_XSLTVerDeclared;

	const bool								m_isRoot;

	/**
	 * This table is keyed on the target elements of patterns, and contains linked
	 * lists of the actual patterns that match the target element to some degree
	 * of specifity.
	 */
	PatternTableMapType 						m_elementPatternTable;

	const PatternTableMapType::const_iterator	m_elementPatternTableEnd;

	PatternTableListType						m_elementAnyPatternList;

	/**
	 * This table is keyed on the target attributes of patterns, and contains linked
	 * lists of the actual patterns that match the target attribute to some degree
	 * of specifity.
	 */
	PatternTableMapType 						m_attributePatternTable;

	const PatternTableMapType::const_iterator	m_attributePatternTableEnd;

	PatternTableListType						m_attributeAnyPatternList;

	/**
	 * These tables are for text, comment, root, and PI node templates.
	 */
	PatternTableListType					m_textPatternList;

	PatternTableListType					m_commentPatternList;

	PatternTableListType					m_rootPatternList;

	PatternTableListType					m_piPatternList;

	/**
	 * This table is for patterns that match "node()".
	 */
	PatternTableListType					m_nodePatternList;

	/**
	 * This will hold all of the MatchPattern2 instances for the
	 * stylesheet.
	 */
	MatchPattern2Container					m_matchPattern2Container;

	/**
	 * This caches the number of possible patterns we can match.
	 */
	MatchPattern2Container::size_type		m_patternCount;

	AttributeSetVectorType 					m_attributeSets;

	/**
	 * This caches the number of attribute sets.
	 */
	AttributeSetVectorType::size_type		m_attributeSetsSize;

	XalanNodeListSurrogate					m_surrogateChildren;

	ElemDecimalFormatVectorType				m_elemDecimalFormats;

	NamespacesHandler						m_namespacesHandler;

	static const XalanDOMString				s_emptyString;

	static const XalanEmptyNamedNodeMap		s_fakeAttributes;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_STYLESHEET_HEADER_GUARD
