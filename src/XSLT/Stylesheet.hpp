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
 *
 * $ Id: $
 *
 */

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
class MatchPattern2;



/**
 * This class represents the base stylesheet or an "import" stylesheet.
 * "include" stylesheets are combined with the including stylesheet.
 */
class XALAN_XSLT_EXPORT Stylesheet : public UnimplementedDocument, public UnimplementedElement
{   

public:

#if defined(XALAN_NO_NAMESPACES)
#	define XALAN_STD
#else
#	define XALAN_STD std::
#endif

typedef XALAN_STD map<DOMString, ExtensionNSHandler*> ExtensionNamespacesMapType;
typedef XALAN_STD map<QName, ElemTemplateElement*>    ElemTemplateElementMapType;
typedef XALAN_STD vector<Arg>                         ParamVectorType;
typedef XALAN_STD vector<ElemAttributeSet*>           AttributeSetMapType;
typedef XALAN_STD vector<ElemVariable*>               ElemVariableVectorType;
typedef XALAN_STD vector<KeyDeclaration>              KeyDeclarationVectorType;
typedef XALAN_STD vector<KeyTable*>                   KeyTableVectorType;
typedef XALAN_STD vector<NameSpace>                   NamespaceVectorType;
typedef XALAN_STD vector<NamespaceVectorType>         NamespacesStackType;
typedef XALAN_STD vector<QName>                       QNameVectorType;
typedef XALAN_STD vector<Stylesheet*>                 StylesheetVectorType;
typedef XALAN_STD vector<const XMLURL*>               URLStackType;
typedef XALAN_STD vector<const XPath*>                XPathVectorType;

#undef XALAN_STD

	virtual NodeImpl* cloneNode(bool /*deep*/) { return 0; }

  /**
   * A lookup table of all space preserving elements.
   */
  XPathVectorType				m_whitespacePreservingElements;
  
  /**
   * A lookup table of all space stripping elements.
   */
  XPathVectorType				m_whitespaceStrippingElements;

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
	 * Retrieve the default XPath for apply-template
	 * 
	 * @return pointer to default XPath
	 */
	const XPath*
	getDefaultATXpath () const
	{
		return m_defaultATXpath;
	}

	/**
	 * Set the default XPath for apply-template
	 * 
	 * @param defaultATXpath pointer to new default XPath
	 */
	void
	setDefaultATXpath (XPath* defaultATXpath) 
	{
		m_defaultATXpath = defaultATXpath;
	}


	/**
	 * Retrieve the stack of namespace lists
	 * 
	 * @return vector of namespace vectors
	 */
	const NamespacesStackType& getNamespaces() const
	{ 
		return m_namespaces;
	}

	/**
	 * Retrieve the list of namespace declarations currently in effect
	 * 
	 * @return vector of namespace vectors
	 */
	const NamespaceVectorType& getNamespaceDecls() const;

	/**
	 * Set the list of namespace declarations currently in effect
	 * 
	 * @param ns vector of namespace vectors
	 */
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
	 * 
	 * @param atts attribute list constaining namespaces
	 */
	void pushNamespaces(const AttributeList& atts);

  /**
	* Pop a namespace declaration from the namespace stack.
   */
	void popNamespaces();

  /** 
   * See if this is a xmlns attribute, and, if so, process it.
   * 
   * @param attrName qualified name of attribute
   * @param atts     attribute list where the element comes from (not used at 
   *                 this time)
   * @param which    index into the attribute list (not used at this time)
   * @return         true if this is a namespace name
   */
	bool
	isAttrOK(
			const DOMString&				attrName,
			const AttributeList&			atts,
			int								which,
			StylesheetConstructionContext&	constructionContext) const;

	/**
	 * Get the namespace from a qualified name.
	 * 
	 * @param nodeName name of node
	 * @return namespace string for node
	 */
	DOMString getNamespaceFromStack(const DOMString& nodeName) const;

	/**
	 * Get the namespace from a prefix by searching the current namespace list.
	 * 
	 * @param prefix prefix to search
	 * @return namespace corresponding to prefix
	 */
	DOMString getNamespaceForPrefix(const DOMString& prefix) const;

	/**
	 * Get the namespace from a prefix by searching the stack of namespace
	 * lists.
	 * 
	 * @param prefix prefix to search
	 * @return namespace corresponding to prefix
	 */
	DOMString getNamespaceForPrefixFromStack(const DOMString& prefix) const;

	/**
	 * Add a template to the list of names templates
	 * 
	 * @param tmpl template to add
	 */
	void addTemplate(ElemTemplate *tmpl);

	/**
	 * Process an attribute that has the value of 'yes' or 'no'.
	 * 
	 * @param aname name of attribute
	 * @param val value
	 * @param constructionContext context for evaluation
	 * @return true if value equals string constant for "yes," false otherwise
	 */
	virtual bool
	getYesOrNo(
			const DOMString&				aname,
			const DOMString&				val,
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
	const DOMString
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
	setBaseIdentifier(const DOMString& str)
	{
		m_baseIdent = str;
	}

	/**
	 * Add an attribute set to the list.
	 *
	 * @param qname   qualified name of attribute set
	 * @param attrSet pointer to attribute set to add
	 */
	void addAttributeSet(
		const QName&		qname, 
		ElemAttributeSet*	attrSet);

	/**
	 * Apply the set of named attributes to a node in a given context with a
	 * given mode
	 *
	 * @param attributeSetsNames list of attribute set names
	 * @param executionContext   current execution context
	 * @param sourceTree         node for source tree
	 * @param sourceNode         source node
	 * @param mode               execution mode
	 */
	void
	applyAttrSets(
			const QNameVectorType&			attributeSetsNames,
            StylesheetExecutionContext&		executionContext, 
            const DOM_Node&					sourceTree,
            const DOM_Node&					sourceNode,
            const QName&					mode) const;
  
	/**
	 * Determine whether default whitespace processing is in effect
	 * 
	 * @return number of immediate children of this node
	 */
	bool isDefaultSpaceProcessing() const
	{
		return m_defaultSpace;
	}

	/**
	 * Set whether default whitespace processing is in effect
	 * 
	 * @param bEnabled true if default processing should be enabled
	 */
	void setDefaultSpaceProcessing(bool bEnabled)
	{
		m_defaultSpace = bEnabled;
	}

	/**
	 * Retrieve the list of imported stylesheets
	 * 
	 * @return vector of imported stylesheets
	 */
	StylesheetVectorType& getImports()
	{
		return m_imports;
	}

	/**
	 * Retrieve the manufactured template to use if there is no wrapper.
	 * 
	 * @return pointer to template
	 */
	const ElemTemplate*
	getWrapperlessTemplate()
	{
		return m_wrapperlessTemplate;
	}

	/**
	 * Set the manufactured template to use if there is no wrapper.
	 * 
	 * @param templ template to use
	 */
	void
	setWrapperlessTemplate(ElemTemplate*	templ)
	{
		m_wrapperlessTemplate = templ;
	}

	/**
	 * whether there is a wrapper template
	 * 
	 * @return true is there is a wrapper
	 */
	bool isWrapperless() const
	{
		return m_isWrapperless;
	}

	/**
	 * Set whether there is a wrapper template
	 * 
	 * @param b true is there is a wrapper
	 */
	void setWrapperless(bool b)
	{
		m_isWrapperless = b;
	}

	/**
	 * Retrieve the stack of who's including who
	 * 
	 * @return stack of includes
	 */
	URLStackType& getIncludeStack()
	{
		return m_includeStack;
	}

	/**
	 * Process the xsl:key element.
	 * 
	 * @param nsContext           element providing context for namespaces
	 * @param atts                attribute list for element
	 * @param constructionContext context for evaluation
	 */
	/*
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
  
	/**
	 * Locate a template via the "name" attribute.
	 * 
	 * @param name               name of template
	 * @param executionContext   current execution context
	 * @return pointer to template found or 0 if none found
	 */
	virtual ElemTemplateElement*
	findNamedTemplate(
			const DOMString&				name,
            StylesheetExecutionContext&		executionContext) const;
	/**
	 * Locate a template via the "name" attribute.
	 * 
	 * @param name               qualified name of template
	 * @param executionContext   current execution context
	 * @return pointer to template found or 0 if none found
	 */
	virtual ElemTemplateElement*
	findNamedTemplate(
			const QName&					qname,
            StylesheetExecutionContext&		executionContext) const;

	/**
	 * Given the name of a variable, return its corresponding XObject
	 *
	 * @param name               name of variable
	 * @param executionContext   current execution context
	 * @return pointer to object
	 */
	virtual	XObject*
	getTopLevelVariable(
			const DOMString&				name,
            StylesheetExecutionContext&		executionContext) const;

	/**
	 * Given a target element, find the template that best matches in the given
	 * XSL document, according to the rules specified in the xsl draft. 
	 *
	 * @param executionContext current execution context
	 * @param sourceTree       where the targetElem is to be found
	 * @param targetNode       element that needs a rule
	 * @return                 pointer to rule that best matches targetNode
	 */
	virtual
	ElemTemplateElement*
	findTemplate(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					sourceTree,
			const DOM_Node&					targetNode) const;


	/**
	 * Given a target element, find the template that best matches in the given
	 * XSL document, according to the rules specified in the xsl draft. 
	 *
	 * @param executionContext current execution context
	 * @param sourceTree        where the targetElem is to be found
	 * @param targetElem        element that needs a rule
	 * @param mode              string indicating the display mode
	 * @param useImports        means that this is an xsl:apply-imports commend
	 * @param foundStylesheet   if non-null, the Stylesheet that the found
	 *                          template belongs to will be returned in the
	 *                          foundStylesheet[0]
	 * @return pointer to rule that best matches targetElem
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
		 *
		 * @param pat string that contains element pattern
		 * @param exp XPath expression for pattern
		 * @param theTemplate node that contains the template for this pattern
		 * @param posInStylesheet position in stylesheet
		 * @param targetString target string
		 * @param stylesheet stylesheet for pattern
		 */
		MatchPattern2(
				const DOMString&		pat,
				const XPath&			exp,
				const ElemTemplate&		theTemplate,
				int						posInStylesheet, 
				const DOMString&		targetString,
				const Stylesheet&		stylesheet); 

		~MatchPattern2();

		/**
		 * Retrieve stylesheet associated with pattern.
		 * 
		 * @return stylesheet for pattern
		 */
		const Stylesheet& getStylesheet() const
		{
			return m_stylesheet;
		}
		
		/**
		 * Retrieve string for target.
		 * 
		 * @return target string
		 */
		const DOMString& getTargetString() const
		{ 
			return m_targetString;
		}
		
		/**
		 * Retrieve expression associated with pattern.
		 * 
		 * @return XPath expression for pattern
		 */
		const XPath& getExpression() const
		{
			return m_expression;
		}
		
		/**
		 * Retrieve position of pattern in stylesheet.
		 * 
		 * @return position in stylesheet
		 */
		int getPositionInStylesheet() const
		{
			return m_posInStylesheet;
		}
		
		/**
		 * Retrieve pattern string.
		 * 
		 * @return string that contains element pattern
		 */
		const DOMString& getPattern() const
		{
			return m_pattern;
		}
		
		/**
		 * Retrieve node that contains the template for this pattern.
		 * 
		 * @return template node
		 */
		const ElemTemplate& getTemplate() const
		{
			return m_template;
		}
		
		/**
		 * Retrieve priority of pattern.
		 * 
		 * @return priority of pattern
		 */
		double getPriority() const
		{
			return m_priority;
		}

		/**
		 * Set priority of pattern.
		 * 
		 * @param thePriority priority of pattern
		 */
		// This is const because m_priority is mutable, which is an artifact of
		// the design and our Java heritage.
		void setPriority(double	thePriority) const
		{ 
			m_priority = thePriority;
		}

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

		// Not implemented...
		MatchPattern2();
		MatchPattern2& operator=(const MatchPattern2&);
	};

#if defined(XALAN_NO_NAMESPACES)
	typedef list<MatchPattern2*>                 PatternTableListType;
	typedef vector<const MatchPattern2*>         PatternTableVectorType;
	typedef map<DOMString, PatternTableListType> PatternTableMapType;
#else
	typedef std::list<MatchPattern2*>                 PatternTableListType;
	typedef std::vector<const MatchPattern2*>         PatternTableVectorType;
	typedef std::map<DOMString, PatternTableListType> PatternTableMapType;
#endif

	/**
	 * Add object to list of match patterns if not already there.
	 *
	 * @param thePattern pattern to add
	 * @param theVector  vector of patterns to add to
	 */
	static void
	addObjectIfNotFound(
			const MatchPattern2*				thePattern,
	PatternTableVectorType&	theVector);

	/**
	 * Given a source node, locate the start of a list of possible template
	 * matches, according to its type.
	 *
	 * @param sourceNode source node for search of match patterns
	 */
	const PatternTableListType*
	locateMatchPatternList2(DOM_Node	sourceNode) const;

	/**
	 * Given an element type, locate the start of a list of 
	 * possible template matches, possibly trying wild card matches.
	 *
	 * @param sourceElementType type of element to search
	 * @param tryWildCard       if true, use wild card matching
	 */
	const PatternTableListType*
	locateMatchPatternList2(
			const DOMString&	sourceElementType,
			bool				tryWildCard = false) const;

	/**
	 * Given a valid element key, return the corresponding node list.
	 *
	 * @param doc              source document
	 * @param name             name of the key, which must match the 'name'
	 *                         attribute on xsl:key
	 * @param ref              value that must match the value found by the
	 *                         'match' attribute on xsl:key
	 * @param resolver         resolver for namespace resolution
	 * @param executionContext current execution context
	 * @return if the name was not declared with xsl:key, this will return
	 * null, if the identifier is not found, it will return an empty node set,
	 * otherwise it will return a nodeset of nodes.
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
	 * an element extension as well as for function calls (which is passed
	 * on to XPath).
	 *
	 * @param uri the URI of the extension namespace
	 * @param nsh handler
	 */
	void addExtensionNamespace (const DOMString& uri, ExtensionNSHandler* nsh);

	/**
	 * Return the handler for a given extension namespace.
	 *
	 * @param uri the URI of the extension namespace.
	 * @return pointer to extension handler
	 */
	ExtensionNSHandler* lookupExtensionNSHandler (const DOMString& uri) const
	{
		const ExtensionNamespacesMapType::const_iterator	it = 
		  m_extensionNamespaces.find(uri);

		return it == m_extensionNamespaces.end() ? 0 : (*it).second;
	}

	/**
	 * Get the type of the node.
	 *
	 * @return number corresponding to node type
	 */
	virtual short
	getNodeType();

	/* Unimplemented. */

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
	* Set a top level variable, to be serialized with the rest of the
	* stylesheet.
	*
	* @param var top-level variable declared with "xsl:variable" or
	*            xsl:param-variable.
   */
	void
	setTopLevelVariable(ElemVariable* var)
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
			StylesheetExecutionContext&		executionContext,
			ParamVectorType&				topLevelParams) const;

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
	mutable KeyTableVectorType		m_key_tables;

	/**
	 * Table of KeyDeclaration objects, which are set by the 
	 * xsl:key element.
	 */
	KeyDeclarationVectorType				m_keyDeclarations;

  /**
   * This is set to true if an xsl:key directive is found.
   * Mainly for use by the XMLParserLiaison classes for 
   * optimized processing of ids.
   * @serial
   */
	bool m_needToBuildKeysTable;

	/**
	 * A vector of the -imported- XSL Stylesheets.
	 */
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
	ExtensionNamespacesMapType	m_extensionNamespaces;

  
  /**
   * The first template of the template children.
   */
	ElemTemplateElement*		m_firstTemplate;
  
  /**
	* A stack of who's including who is needed in order to support "It is an
	* error if a stylesheet directly or indirectly includes itself."
   */
	URLStackType				m_includeStack;
  
  /** 
   * Tell if this stylesheet has the default space handling
   * turned off or on according to the xml:space attribute.
   * @serial
   */
	bool						m_defaultSpace;
  
  /**
	* Keyed on string macro names, and holding values that are macro elements
	* in the XSL DOM tree. Initialized in initMacroLookupTable, and used in
	* findNamedTemplate.
   */
  ElemTemplateElementMapType	m_namedTemplates;
  
  /**
   * Table for defined constants, keyed on the names.
   */
	ElemVariableVectorType		m_topLevelVariables;


  /**
   * The version of XSL that was declared.
   */
	double						m_XSLTVerDeclared;

	const bool	m_isRoot;

	/**
	 * This table is keyed on the target elements 
	 * of patterns, and contains linked lists of
	 * the actual patterns that match the target element
	 * to some degree of specifity.
	 */

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
	AttributeSetMapType		m_attributeSets;

}; // end Stylesheet class definition



#endif	// XALAN_STYLESHEET_HEADER_GUARD


/*
 *	$ Log: $
 */
