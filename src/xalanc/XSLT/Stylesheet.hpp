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

#if !defined(XALAN_STYLESHEET_HEADER_GUARD)
#define XALAN_STYLESHEET_HEADER_GUARD



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanMap.hpp>



#include <xalanc/PlatformSupport/PrefixResolver.hpp>



#include <xalanc/XPath/NameSpace.hpp>
#include <xalanc/XPath/XalanQNameByReference.hpp>
#include <xalanc/XPath/XPath.hpp>



#include <xalanc/XSLT/NamespacesHandler.hpp>
#include <xalanc/XSLT/KeyDeclaration.hpp>
#include <xalanc/XSLT/StylesheetConstructionContext.hpp>
#include <xalanc/XSLT/StylesheetExecutionContext.hpp>
#include <xalanc/XSLT/XalanSpaceNodeTester.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class ExtensionNSHandler;
class XalanDecimalFormatSymbols;
class ElemDecimalFormat;
class ElemTemplate;
class ElemTemplateElement;
class ElemVariable;
class KeyTable;
class NodeRefListBase;
class PrefixResolver;
class StylesheetConstructionContext;
class StylesheetRoot;
class XalanMatchPatternData;
class XalanQName;
class XObject;
class StylesheetExecutionContext;


typedef XalanVector<const XalanMatchPatternData*>	PatternTableVectorTypeDecl;
XALAN_USES_MEMORY_MANAGER(PatternTableVectorTypeDecl)


/**
 * This class represents the base stylesheet or an "import" stylesheet.
 * "include" stylesheets are combined with the including stylesheet.
 */
class XALAN_XSLT_EXPORT Stylesheet : protected PrefixResolver
{	

public:

    typedef StylesheetConstructionContext::size_type        size_type;
	typedef StylesheetExecutionContext::ParamVectorType		ParamVectorType;
	typedef XalanQName::NamespaceVectorType					NamespaceVectorType;
	typedef XalanQName::NamespacesStackType					NamespacesStackType;

	typedef XalanVector<ElemVariable*> 					ElemVariableVectorType;
	typedef XalanVector<KeyDeclaration>					KeyDeclarationVectorType;
	typedef XalanVector<Stylesheet*>					StylesheetVectorType;
	typedef XalanVector<XalanDOMString>					URLStackType;
	typedef XalanVector<ElemDecimalFormat*>				ElemDecimalFormatVectorType;
	typedef XalanVector<XalanSpaceNodeTester>			WhitespaceElementsVectorType;
    typedef PatternTableVectorTypeDecl					PatternTableVectorType;
	
	typedef  XalanMap<XalanDOMString, ExtensionNSHandler*>		ExtensionNamespacesMapType;

    typedef  XalanMap<XalanQNameByReference,
						 const ElemTemplate* >		ElemTemplateMapType;

    typedef XalanMap<const XalanNode*, KeyTable* >	KeyTablesTableType;

	typedef XalanMap<XalanDOMString, PatternTableVectorType> PatternTableMapType;

    typedef StylesheetConstructionContext::GetAndReleaseCachedString    GetAndReleaseCachedString;

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

    static Stylesheet*
    create(MemoryManagerType& theManager,
            StylesheetRoot& 				root,
			const XalanDOMString&			baseIdentifier,
			StylesheetConstructionContext&	constructionContext);

    MemoryManagerType&
    getMemoryManager()
    {
        return m_elementPatternTable.getMemoryManager();
    }

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

	void
	addWhitespaceElement(const XalanSpaceNodeTester&	theTester);

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
	getNamespaceFromStack(const XalanDOMString& 	nodeName,
                            XalanDOMString&         theBuffer) const
	{
		return getNamespaceFromStack(c_wstr(nodeName), theBuffer);
	}

	/**
	 * Get the namespace from a qualified name.
	 * 
	 * @param nodeName name of node
	 * @return namespace string for node, or null if not found.
	 */
	const XalanDOMString*
	getNamespaceFromStack(const XalanDOMChar* 	nodeName,
                            XalanDOMString&     theBuffer) const;

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

		return XalanQName::getNamespaceForPrefix(m_namespaces, (const XalanDOMChar*)prefix);
	}

	/**
	 * Get the namespace for a prefix, and report an error if it wasn't found.
	 * 
	 * @param prefix prefix to search
	 * @param constructionContext The current construction context
	 * @return namespace corresponding to prefix, or null if not found.
	 */
	const XalanDOMString*
	getNamespaceForPrefix(
			const XalanDOMString&			prefix,
			StylesheetConstructionContext&	constructionContext) const;

	/**
	 * Get the namespace for a prefix, and report an error if it wasn't found.
	 * 
	 * @param prefix prefix to search
	 * @param constructionContext The current construction context
	 * @return namespace corresponding to prefix, or null if not found.
	 */
	const XalanDOMString*
	getNamespaceForPrefix(
			const XalanDOMChar*				prefix,
			StylesheetConstructionContext&	constructionContext) const;

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
		assert(targetNode != 0);

		return findTemplate(
				executionContext,
				targetNode,
				targetNode->getNodeType(),
				s_emptyQName,
				false);
	}

	/**
	 * Given a target element, find the template that best matches in the given
	 * XSL document, according to the rules specified in the xsl draft. 
	 *
	 * @param executionContext current execution context
	 * @param targetNode        node that needs a rule
	 * @param targetNodeType    the type of targetNode
	 * @param mode              string indicating the mode
	 * @param onlyUseImports    only use imports, do not use any templates from the stylesheet itself
	 * @return pointer to rule that best matches targetElem
	 */
	const ElemTemplate*
	findTemplate(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						targetNode,
			XalanNode::NodeType				targetNodeType,
			const XalanQName&				mode,
			bool							onlyUseImports) const;

	/**
	 * Add object to vector of match patterns if not already there.
	 *
	 * @param thePattern pattern to add
	 * @param theVector  vector of patterns to add to
	 */
	static void
	addObjectIfNotFound(
			const XalanMatchPatternData*    thePattern,
			PatternTableVectorType& 	    theVector);

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
			const XalanMatchPatternData*    thePattern,
			const XalanMatchPatternData* 	theArray[],
			unsigned int&			        theArraySize);

	/**
	 * Given a name, locate the start of a list of 
	 * possible templates that match that name.  If
	 * none match, then use the default list.
	 *
	 * @param theName The name to match
	 */
	const PatternTableVectorType*
	locateElementMatchPatternDataList(const XalanDOMString&	    theName) const;

	/**
	 * Given a name, locate the start of a list of 
	 * possible templates that match that name.  If
	 * none match, then use the default list.
	 *
	 * @param theName The name to match
	 */
	const PatternTableVectorType*
	locateAttributeMatchPatternDataList(const XalanDOMString&	theName) const;

	/**
	 * Given a XalanNode, locate the start of a list of 
	 * possible templates that match it.
	 *
	 * @param XalanNode The node to match
	 */
	const PatternTableVectorType*
	locateMatchPatternDataList(
			const XalanNode&		theNode,
			XalanNode::NodeType		targetNodeType) const;

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

	const ElemTemplate*	
	getFirstTemplate() const	
	{
		return m_firstTemplate;
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

	WhitespaceElementsVectorType		m_whitespaceElements;

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
	 * @param targetNode        node that needs a rule
	 * @param targetNodeType    the type of targetNode
	 * @param mode              string indicating the mode
	 * @return pointer to rule that best matches targetElem
	 */
	const ElemTemplate*
	findTemplateInImports(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						targetNode,
			XalanNode::NodeType				targetNodeType,
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

	/**
	 * This table is keyed on the target elements of patterns, and contains linked
	 * lists of the actual patterns that match the target element to some degree
	 * of specifity.
	 */
	PatternTableMapType 						m_elementPatternTable;

	const PatternTableMapType::const_iterator	m_elementPatternTableEnd;

	PatternTableVectorType						m_elementAnyPatternList;

	/**
	 * This table is keyed on the target attributes of patterns, and contains linked
	 * lists of the actual patterns that match the target attribute to some degree
	 * of specifity.
	 */
	PatternTableMapType 						m_attributePatternTable;

	const PatternTableMapType::const_iterator	m_attributePatternTableEnd;

	PatternTableVectorType						m_attributeAnyPatternList;

	/**
	 * These tables are for text, comment, root, and PI node templates.
	 */
	PatternTableVectorType					m_textPatternList;

	PatternTableVectorType					m_commentPatternList;

	PatternTableVectorType					m_rootPatternList;

	PatternTableVectorType					m_piPatternList;

	/**
	 * This table is for patterns that match "node()".
	 */
	PatternTableVectorType					m_nodePatternList;

	size_type	                            m_patternCount;

	ElemDecimalFormatVectorType				m_elemDecimalFormats;

	NamespacesHandler						m_namespacesHandler;

	static const XalanDOMString				s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_STYLESHEET_HEADER_GUARD
