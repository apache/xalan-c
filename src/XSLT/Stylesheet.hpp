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
// $ Id: $

#if !defined(XALAN_STYLESHEET_HEADER_GUARD)
#define XALAN_STYLESHEET_HEADER_GUARD

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"


#include <list>
#include <map>
#include <vector>


#include <dom/DOM_Node.hpp>
#include <dom/DOMString.hpp>



#include <DOMSupport/UnimplementedDocument.hpp>
#include <DOMSupport/UnimplementedElement.hpp>
#include <XPath/NameSpace.hpp>
#include <XPath/QName.hpp>



#include "Arg.hpp"
#include "KeyDeclaration.hpp"



class AttributeList;
class ExtensionNSHandler;
class PrefixResolver;
class ElemAttributeSet;
class ElemTemplate;
class ElemTemplateElement;
class ElemVariable;
class KeyTable;
class NodeRefListBase;
class StylesheetConstructionContext;
class StylesheetExecutionContext;
class StylesheetRoot;
class XMLURL;
class XObject;
class XPath;
class XPathExecutionContext;



/**
 * This class represents the base stylesheet or an
 * "import" stylesheet.  "include" stylesheets are 
 * combined with the including stylesheet.
 */
class XALAN_XSLT_EXPORT Stylesheet : public UnimplementedDocument, public UnimplementedElement
{   

protected:

  /**
	* The root of the stylesheet, where all the tables common to all
	* stylesheets are kept.
   */
	StylesheetRoot&	m_stylesheetRoot;

  /**
   * Reference back to the owning XSLTProcessor object.
	* JMD: This has to be a pointer,not a reference because of setXSLProcessor
   */
//	XSLTEngineImpl*	m_processor;

	/**
	 * The base URL of the XSL document.
	 */
	DOMString	m_baseIdent;

private:	
	/**
	 * The root XSL stylesheet.
	 */
	DOM_Document	m_document;
	
	/**
	 * Table of tables of element keys. See KeyTable.
	 */
	typedef std::vector<KeyTable*>	KeyTableVectorType;
	mutable KeyTableVectorType		m_key_tables;

	/**
	 * Table of KeyDeclaration objects, which are set by the 
	 * xsl:key element.
	 */
	typedef std::vector<KeyDeclaration>		KeyDeclarationVectorType;
	KeyDeclarationVectorType				m_keyDeclarations;

  /**
   * This is set to true if an xsl:key directive is found.
   * Mainly for use by the XMLParserLiaison classes for 
   * optimized processing of ids.
   * @serial
   */
	bool m_needToBuildKeysTable;



public:	// @@ JMD: temporary, needs accessor method
	/**
	 * A vector of the -imported- XSL Stylesheets.
	 */
	typedef std::vector<Stylesheet*>			StylesheetVectorType;

	typedef	std::vector<const XMLURL*>			URLStackType;

	typedef	std::vector<NameSpace>				NamespaceVectorType;
	typedef	std::vector<NamespaceVectorType>	NamespacesStackType;

	// @@ JMD: temporary
	virtual NodeImpl* cloneNode(bool /*deep*/) { return 0; }

private:

	StylesheetVectorType		m_imports;

  /**
   * The default template to use for xsl:apply-templates when 
   * a select attribute is not found.
   */
	const XPath*				m_defaultATXpath;

	/**
	 * A stack to keep track of the result tree namespaces.
	 */
	NamespacesStackType			m_namespaces;

  /** 
   * A list of namespace declarations,
   * for mapping from prefix to namespace URI.
   */
	NamespaceVectorType			m_namespaceDecls;

  /**
	* This is pushed on the m_resultNameSpaces stack 'till a xmlns attribute is
	* found.
   */
	static const NamespaceVectorType	s_emptyNamespace;

	/**
	 * Tells if the stylesheet tables need to be rebuilt.
	 */
	bool						m_tablesAreInvalid;

  /**
	* Tells if the stylesheet is without an xsl:stylesheet and xsl:template
	* wrapper.
   */
	bool						m_isWrapperless;

  /**
   * The manufactured template if there is no wrapper.
   */
	ElemTemplate*				m_wrapperlessTemplate;
  
  /**
   * The table of extension namespaces.
   */
	typedef std::map<DOMString, ExtensionNSHandler*>	ExtensionNamespacesMapType;

	ExtensionNamespacesMapType	m_extensionNamespaces;
  
  /**
   * The first template of the template children.
   */
	ElemTemplateElement*		m_firstTemplate;
  
  /**
	* A stack of who's including who is needed in order to support "It is an
	* error if a stylesheet directly or indirectly includes itself."
   */
	//typedef	std::vector<URL>		URLStackType;
	URLStackType				m_includeStack;
  
  /** 
   * Tell if this stylesheet has the default space handling
   * turned off or on according to the xml:space attribute.
   * @serial
   */
	bool						m_defaultSpace;
  
// @@ JMD: temporary -- needs accessor
public:

  /**
   * A lookup table of all space preserving elements.
   */
  typedef std::vector<const XPath*> XPathVectorType;
  XPathVectorType				m_whitespacePreservingElements;
  
  /**
   * A lookup table of all space stripping elements.
   */
  XPathVectorType				m_whitespaceStrippingElements;

private:
  
  /**
	* Keyed on string macro names, and holding values that are macro elements
	* in the XSL DOM tree. Initialized in initMacroLookupTable, and used in
	* findNamedTemplate.
   */
  typedef std::map<QName, ElemTemplateElement*> ElemTemplateElementMapType;
  ElemTemplateElementMapType	m_namedTemplates;
  
  /**
   * Table for defined constants, keyed on the names.
   */
	typedef std::vector<ElemVariable*> ElemVariableVectorType;
	ElemVariableVectorType		m_topLevelVariables;


  /**
   * The version of XSL that was declared.
   */
	double						m_XSLTVerDeclared;
  
  

public:

	double
	getXSLTVerDeclared() const
	{
		return m_XSLTVerDeclared;
	}

	void
	setXSLTVerDeclared(double ver)
	{
		m_XSLTVerDeclared = ver;
	}

	/**
	 * Constructor for a Stylesheet needs a Document.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	Stylesheet(
			StylesheetRoot&					root,
			const DOMString&				baseIdentifier,
			StylesheetConstructionContext&	constructionContext);

	virtual
	~Stylesheet();

	const StylesheetRoot&
	getStylesheetRoot() const
	{
		return m_stylesheetRoot;
	}

	StylesheetRoot&
	getStylesheetRoot()
	{
		return m_stylesheetRoot;
	}

	const XPath*
	getDefaultATXpath () const
	{
		return m_defaultATXpath;
	}

	void
	setDefaultATXpath (XPath* defaultATXpath) 
	{
		m_defaultATXpath = defaultATXpath;
	}


	const NamespacesStackType& getNamespaces() const { return m_namespaces; }

	const NamespaceVectorType& getNamespaceDecls() const;

	void setNamespaceDecls(const NamespaceVectorType& ns);

	/*
	 * Get the top entry on the namespace stack, or 0, if
	 * there is nothing on the stack.
	 */
	const NamespaceVectorType&
	getCurrentNamespace() const;

	/**
	 * Push the namespace declarations from the current attribute 
	 * list onto the namespace stack.
	 */
	void pushNamespaces(const AttributeList& atts);

  /**
	* Pop a namespace declaration from the namespace stack.
   */
	void popNamespaces();

	// See if this is a xmlns attribute or in a non-XSLT.
	bool
	isAttrOK(
			const DOMString&				attrName,
			const AttributeList&			atts,
			int								which,
			StylesheetConstructionContext&	constructionContext) const;

	// Get the namespace from a qualified name.
	DOMString getNamespaceFromStack(const DOMString& nodeName) const;

	// Get the namespace from a prefix.
	DOMString getNamespaceForPrefix(const DOMString& prefix) const;

	// Get the namespace from a prefix.
	DOMString getNamespaceForPrefixFromStack(const DOMString& prefix) const;


	void addTemplate(ElemTemplate *tmpl);


	/**
	 * Process an attribute that has the value of 'yes' or 'no'.
	 */
	virtual bool
	getYesOrNo(
			const DOMString&				aname,
			const DOMString&				val,
			StylesheetConstructionContext&	constructionContext) const;

	/**
	 * Tell if this is the root of the stylesheet tree.
	 */
	bool
	isRoot() const
	{
		return m_isRoot;
	}

private:

	const bool	m_isRoot;

public:

// JMD added Thu Sep 30 08:20:29 EDT 1999

   // Get the base identifier with which this stylesheet is associated.
	const DOMString getBaseIdentifier() { return m_baseIdent; }
	void setBaseIdentifier(const DOMString& str) { m_baseIdent = str; }

	/**
	 * Add an attribute set to the list.
	 *
	*/
	void addAttributeSet(
		const QName&		qname, 
		ElemAttributeSet*	attrSet);

	typedef std::vector<QName> QNameVectorType;

	void
	applyAttrSets(
			const QNameVectorType&			attributeSetsNames,
            StylesheetExecutionContext&		executionContext, 
            const DOM_Node&					sourceTree,
            const DOM_Node&					sourceNode,
            const QName&					mode) const;
  
	const DOMString& getBaseURL() const
	{
		return m_baseIdent;
	}

	bool isDefaultSpaceProcessing() const
	{
		return m_defaultSpace;
	}

	void setDefaultSpaceProcessing(bool bEnabled)
	{
		m_defaultSpace = bEnabled;
	}

	StylesheetVectorType& getImports()
	{
		return m_imports;
	}

	const ElemTemplate*
	getWrapperlessTemplate()
	{
		return m_wrapperlessTemplate;
	}

	void
	setWrapperlessTemplate(ElemTemplate*	templ)
	{
		m_wrapperlessTemplate = templ;
	}

	bool isWrapperless() const
	{
		return m_isWrapperless;
	}

	void setWrapperless(bool b)
	{
		m_isWrapperless = b;
	}

	URLStackType& getIncludeStack()
	{
		return m_includeStack;
	}


	/**
	 * Process the xsl:key element.
	 * 
	 * (Notes to myself)
	 * What we need to do is:
	 * 1) As this function is called, build a table of KeyDeclarations.
	 * 2) During either XML processing, or upon request, walk the XML 
	 * document tree, and build a hash table:
	 * a) keyed by name,
	 * b) each with a value of a hashtable, keyed by the value returned by 
	 *    the use attribute,
	 * c) each with a value that is a nodelist.
	 * Thus, for a given key or keyref, look up hashtable by name, 
	 * look up the nodelist by the given reference.
	 */
	virtual void
	processKeyElement(
			ElemTemplateElement*			nsContext,
			const AttributeList&			atts,
			StylesheetConstructionContext&	constructionContext);
//		  throws XSLProcessorException
private:

#if OUT
	/**
	 * Process imports, includes, macros, templates, etc.
	 * into tables of various types.
	 * @param stylesheet Probably a stylesheet element, 
	 * though this may later be an xsl:module element.
	 */
	virtual
	void
	buildStylesheetTables(
			const DOM_Element&	stylesheet,
			const DOMString&	base);

	@@ OUT ?
	/**
	 * Get a list of the included documents into a vector.
	 */
	virtual
	void
	initIncludes(
			const DOM_Element&	includedStyles,
			const DOMString&	base);

    
	/**
	 * Get a list of the imported documents into a vector.
	 */
	virtual
	void
	initImports(const DOM_Element&	includedStyles);
#endif // OUT

  
public:
			
	/**
	 * Locate a macro via the "name" attribute.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	virtual ElemTemplateElement*
	findNamedTemplate(
			const DOMString&				name,
            StylesheetExecutionContext&		executionContext) const;
	/**
	 * Locate a macro via the "name" attribute.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	virtual ElemTemplateElement*
	findNamedTemplate(
			const QName&					qname,
            StylesheetExecutionContext&		executionContext) const;

	/**
	 * Given the name of a constant, return a string value.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	virtual	XObject*
	getTopLevelVariable(
			const DOMString&				name,
            StylesheetExecutionContext&		executionContext) const;

	/**
	 * Given a target element, find the template that best 
	 * matches in the given XSL document, according 
	 * to the rules specified in the xsl draft. 
	 * @param executionContext The current execution context.
	 * @param sourceTree Where the targetElem is to be found.
	 * @param targetElem The element that needs a rule.
	 * @return Rule that best matches targetElem.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	virtual
	ElemTemplateElement*
	findTemplate(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					sourceTree,
			const DOM_Node&					targetNode) const;


	/**
	 * Given a target element, find the template that best 
	 * matches in the given XSL document, according 
	 * to the rules specified in the xsl draft. 
	 * @param executionContext The current execution context.
	 * @param sourceTree Where the targetElem is to be found.
	 * @param targetElem The element that needs a rule.
	 * @param mode A string indicating the display mode.
	 * @param useImports means that this is an xsl:apply-imports commend.
	 * @param foundStylesheet If non-null, the Stylesheet that the found template
	 * belongs to will be returned in the foundStylesheet[0].
	 * @return Rule that best matches targetElem.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	virtual
	ElemTemplateElement*
	findTemplate(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					sourceTree, 
			const DOM_Node&					targetNode, 
			const QName&					mode,
			bool							useImports,
			const Stylesheet*&				foundStylesheet) const;

	/**
	 * A class to contain a match pattern and it's corresponding template.
	 * This class also defines a node in a match pattern linked list.
	 */
	class MatchPattern2
	{
	public:

		/**
		 * Construct a match pattern from a pattern and template.
		 * @param pat For now a Nodelist that contains old-style element patterns.
		 * @param template The node that contains the template for this pattern.
		 * @param isMatchPatternsOnly tells if pat param contains only match 
		 * patterns (for compatibility with old syntax).
		 */
		MatchPattern2(
				const DOMString&		pat,
				const XPath&			exp,
				const ElemTemplate&		theTemplate,
				int						posInStylesheet, 
				const DOMString&		targetString,
				const Stylesheet&		stylesheet); 

		~MatchPattern2();

		const Stylesheet& getStylesheet() const { return m_stylesheet; }
		const DOMString& getTargetString() const { return m_targetString; }
		const XPath& getExpression() const { return m_expression; }
		int getPositionInStylesheet() const { return m_posInStylesheet; }
		const DOMString& getPattern() const { return m_pattern; }
		const ElemTemplate& getTemplate() const { return m_template; }
		double getPriority() const { return m_priority; }

		// This is const because m_priority is mutable, which is an artifact of
		// the design and our Java heritage.
		void setPriority(double	thePriority) const { m_priority = thePriority; }

	private:
		const Stylesheet& 		m_stylesheet;
		const DOMString			m_targetString;
		const XPath&			m_expression;
		const int				m_posInStylesheet;
		const DOMString			m_pattern;
		const ElemTemplate&		m_template; // ref to the corresponding template
		
		/**
		 * Transient... only used to track priority while processing.
		 */
		mutable double				m_priority;

	private:
		// Not implemented...
		MatchPattern2();
		MatchPattern2& operator=(const MatchPattern2&);
	};

	static void
	addObjectIfNotFound(
			const MatchPattern2*				thePattern,
			std::vector<const MatchPattern2*>&	theVector);

private:

	/**
	 * This table is keyed on the target elements 
	 * of patterns, and contains linked lists of
	 * the actual patterns that match the target element
	 * to some degree of specifity.
	 */

// VC++ seems to have some issues with typedefs and scoping.
#if defined(_MSC_VER)
public:
#endif

	typedef std::list<MatchPattern2*>					PatternTableListType;
	typedef std::map<DOMString, PatternTableListType>	PatternTableMapType;

#if defined(_MSC_VER)
private:
#endif

  /**
   * This table is keyed on the target elements of patterns, and contains linked
	* lists of the actual patterns that match the target element to some degree
	* of specifity.
   */
	PatternTableMapType		m_patternTable;

	/**
	 * Table of attribute sets, keyed by set name.
	 */
	// This can't be a map, since you can have multiple attribute sets of the
	// same name, could be a multimap but why bother
	typedef std::vector<ElemAttributeSet*> AttributeSetMapType;

	AttributeSetMapType		m_attributeSets;

public:

	/**
	 * Given a source node, locate the start of a list of 
	 * possible template matches, according to its type.
	 */
	const PatternTableListType*
	locateMatchPatternList2(DOM_Node	sourceNode) const;

	/**
	 * Given an element type, locate the start of a list of 
	 * possible template matches.
	 */
	const PatternTableListType*
	locateMatchPatternList2(
			const DOMString&	sourceElementType,
			bool				tryWildCard = false) const;

	/**
	 * Given an element cloned into the result tree, and a value 
	 * from an xsl:use attribute, then add the attributes 
	 * from the defined attribute sets named.
	 * @param xslUseValue The value obtained from an xsl:use attribute.
	 * @param resultTreeElem An element cloned from a template 
	 * element.
	 */
	/**
	 * Given a valid element key, return the corresponding node list.
	 */
	const NodeRefListBase*
	getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const PrefixResolver&	resolver,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Add an extension namespace handler. This provides methods for calling
	 * an element extension as well as for function stuff (which is passed
	 * on to XPath).
	 *
	 * @param uri the URI of the extension namespace.
	 * @param nsh handler
	 */
	void addExtensionNamespace (const DOMString& uri, ExtensionNSHandler* nsh);

	/**
	 * Return the handler for a given extension namespace.
	 *
	 * @param uri the URI of the extension namespace.
	 */
	ExtensionNSHandler* lookupExtensionNSHandler (const DOMString& uri) const
	{
		const ExtensionNamespacesMapType::const_iterator	it = 
		  m_extensionNamespaces.find(uri);

		return it == m_extensionNamespaces.end() ? 0 : (*it).second;
	}

	/**
	 * Get the type of the node.  We'll pretend we're a Document.
	 */
	virtual short
	getNodeType();

	/** Unimplemented. */

/*
	DOM_DocumentType* getDoctype()
	{ error("getDoctype not supported!"); return 0; }

	DOM_DOMImplementation* getImplementation()
	{ error("getImplementation not supported!"); return 0; }

	DOM_Element* getDocumentElement()
	{ error("getDocumentElement not supported!"); return 0; }

	DOM_Element* createElement(DOMString tagName)
			// throws DOMException
	{ error("createElement not supported!"); return 0; }

	DOM_DocumentFragment* createDocumentFragment()
	{ error("createDocumentFragment not supported!"); return 0; }

	DOM_Text* createTextNode(DOMString data)
	{ error("createTextNode not supported!"); return 0; }

	DOM_Comment* createComment(DOMString data)
	{ error("createComment not supported!"); return 0; }

	 DOM_CDATASection* createCDATASection(DOMString data)
		//throws DOMException
	{ error("createCDATASection not supported!"); return 0; }

	DOM_ProcessingInstruction* createProcessingInstruction(DOMString& target, 
																				DOMString& data)
		//throws DOMException
	{ error("createProcessingInstruction not supported!"); return 0; }

	DOM_Attr* createAttribute(DOMString& name) // throws DOMException
	{ error("createAttribute not supported!"); return 0; }

	DOM_EntityReference* createEntityReference(DOMString& name) // throws DOMException
	{ error("createEntityReference not suppo->ted!"); return 0; }

	DOM_NodeList* getElementsByTagName(DOMString& tagname)
	{ error("getElementsByTagName not supported!"); return 0; }
*/

  /**
   * Set a top level variable, to be serialized with the rest of 
   * the stylesheet.
   * @param var A top-level variable declared with xsl:variable or xsl:param-variable.
   */
	void
	setTopLevelVariable(ElemVariable* var) { m_topLevelVariables.push_back(var); }

	typedef std::vector<Arg> ParamVectorType;
  
	void
	pushTopLevelVariables(
			StylesheetExecutionContext&		executionContext,
			ParamVectorType&				topLevelParams) const;

}; // end Stylesheet class definition



#endif	// XALAN_STYLESHEET_HEADER_GUARD


/*
 *	$ Log: $
 */
