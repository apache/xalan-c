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
#if !defined(XALAN_STYLESHEETROOT_HEADER_GUARD)
#define XALAN_STYLESHEETROOT_HEADER_GUARD 



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include "Stylesheet.hpp"



#include <xalanc/Include/XalanMap.hpp>
#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/FormatterListener.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class ElemAttributeSet;
class StylesheetConstructionContext;
class XalanText;
class XSLTResultTarget;


typedef XalanVector<ElemAttributeSet*> 		AttributeSetVectorTypeDecl;
XALAN_USES_MEMORY_MANAGER(AttributeSetVectorTypeDecl)

/**
 * This acts as the stylesheet root of the stylesheet 
 * tree, and holds values that are shared by all 
 * stylesheets in the tree.
 */
class XALAN_XSLT_EXPORT StylesheetRoot : public Stylesheet
{
public:

	typedef XalanVector<const XalanQName*> 		XalanQNameVectorType;

	typedef AttributeSetVectorTypeDecl			AttributeSetVectorType;

	typedef XalanMap<const XalanQName*,
			    AttributeSetVectorType>			AttributeSetMapType;

	/**
	 * Construct a Stylesheet from a Document.
	 *
	 * @param baseIdentifier document identifier
	 * @param constructionContext context for construction of object
	 */
	StylesheetRoot(
        const XalanDOMString&			baseIdentifier,
		StylesheetConstructionContext&	constructionContext);    

	virtual 
	~StylesheetRoot();

    static StylesheetRoot*
    create(
        MemoryManagerType&              theManager,
        const XalanDOMString&			baseIdentifier,
		StylesheetConstructionContext&	constructionContext);

    MemoryManagerType&
    getMemoryManager()
    {
        return m_version.getMemoryManager();
    }
	/**
	 * Called after construction is completed.
	 */
	virtual void
	postConstruction(StylesheetConstructionContext&		constructionContext);

	/**
	 * Transform the source tree to the output in the given result tree target.
	 *
	 * @param inputSource  The input source
	 * @param outputTarget The output result target
	 * @param constructionContext context for construction of object
	 */
	void
	process(
			XalanNode*						sourceTree,
			XSLTResultTarget&				outputTarget,
			StylesheetExecutionContext&		executionContext) const;

	/**
	 * Have the stylesheet create the appropriate FormatterListener,
	 * based on the XSLTResultTarget provided.  setupFormatterListener
	 * also calls setFormatterListener() on the execution context
	 * instance.  setupFormatterListener() is done automatically by
	 * process(), but this is provided as an escape, to allow for
	 * changing the FormatterListener on-the-fly.
	 *
	 * @param outputTarget The output source tree
	 * @param constructionContext context for construction of object
	 * @return a pointer to the new FormatterListener instance.
	 */
	FormatterListener*
	setupFormatterListener(
			XSLTResultTarget&				outputTarget,
			StylesheetExecutionContext&		executionContext) const;

	/** 
	 * Return the output method that was specified in the stylesheet. 
	 * The returned value is one of FormatterLister::eFormat values.
	 *
	 * @return value of output method
	 */
	FormatterListener::eFormat
	getOutputMethod() const
	{
		return m_outputMethod;
	}

	bool
	isOutputMethodSet() const
	{
		return getOutputMethod() == FormatterListener::OUTPUT_METHOD_NONE ? false : true;
	}

	/**
	 * Get the output version string that was specified in the
	 * xsl:output element
	 *
	 * @return output version string
	 */
	XalanDOMString&
	getOutputVersion(XalanDOMString& theResult) const
	{ 
        theResult.assign(m_version);
         
         return theResult;
	}

	/**
	 * Determine if output indenting was specified in the
	 * xsl:output element
	 *
	 * @return true to indent
	 */
	bool 
	getOutputIndent() const
	{
        return m_indentResult == eIndentNoImplicit ||
               m_indentResult == eIndentNoExplicit ? false : true;
	}

	/**
	 * Determine if output indenting should be
	 * enabled for HTML output.  Semantically,
     * this implies the output method is implicit,
     * not explicit, but we don't really check that.
	 *
	 * @return true to indent
	 */
	bool 
	getHTMLOutputIndent() const
	{
        return m_indentResult == eIndentNoExplicit ? false : true;
	}

	/**
	 * Get the output encoding string that was specified in the
	 * xsl:output element
	 *
	 * @return encoding string
	 */
	XalanDOMString& 
	getOutputEncoding(XalanDOMString& theResult) const
	{
        theResult.assign(m_encoding);
        
        return theResult;
	}

	/**
	 * Get the media-type string that was specified in the 
	 * xsl:output element
	 *
	 * @return media type string
	 */
	XalanDOMString& 
	getOutputMediaType(XalanDOMString& theResult) const
	{
        theResult.assign(m_mediatype);
        
        return theResult;
	}

	/**
	 * Get the doctype-system-id string that was specified in the 
	 * xsl:output element
	 *
	 * @return document type string
	 */
	XalanDOMString &
	getOutputDoctypeSystem(XalanDOMString& theResult) const
	{
        theResult.assign(m_doctypeSystem);
        
        return theResult;
	}

	/**
	 * Get the doctype-public-id string that was specified in the 
	 * xsl:output element
	 *
	 * @return document type public id string
	 */
	XalanDOMString& 
	getOutputDoctypePublic(XalanDOMString& theResult) const
	{
        theResult.assign(m_doctypePublic);
        
        return theResult;
	}

	/**
	 * Determine whether to output XML declarations.
	 *
	 * @return true to output declarations
	 */
	bool
	getOmitOutputXMLDecl() const
	{
		return m_omitxmlDecl;
	}

	/**
	 * Get the standalone string that was specified in the 
	 * xsl:output element.(either "yes" or "no")
	 *
	 * @return standalone string
	 */
	XalanDOMString&
	getOutputStandalone(XalanDOMString& theResult) const
	{
        theResult.assign(m_standalone);
        
        return theResult;
	}

	/**
	 * Get the template representing the default rule for text.
	 *
	 * @return pointer to template rule for text
	 */
	ElemTemplateElement*
	getDefaultTextRule() const
	{
		return m_defaultTextRule;
	}

	/**
	 * Get the template representing the default rule.
	 *
	 * @return pointer to default template rule
	 */
	ElemTemplateElement*
	getDefaultRule() const
	{
		return m_defaultRule;
	}

	/**
	 * Get the template representing the default root rule.
	 *
	 * @return pointer to default root template rule
	 */
	ElemTemplateElement*
	getDefaultRootRule() const
	{
		return m_defaultRootRule;
	}

	/**
	 * Process the "xsl:output" element.
	 *
	 * @param name                name of element
	 * @param atts                attribute list for element
	 * @param constructionContext context for construction of object
	 */
	void 
	processOutputSpec(
			const XalanDOMChar*				name, 
			const AttributeListType&		atts,
			StylesheetConstructionContext&	constructionContext);

	/**
	 * Retrieve the stack of imported stylesheets.
	 * 
	 * @return stack of URIs for stylesheets
	 */
	URLStackType&
	getImportStack()
	{
		return m_importStack;
	}

	/**
	 * Retrieve the stack of imported stylesheets.
	 * 
	 * @return const stack of URIs for stylesheets
	 */
	const URLStackType&
	getImportStack() const
	{
		return m_importStack;
	}

	/**
	 * Change the value of the flag for indenting results.
	 * 
	 * @param bIndent true to indent results
	 */
	void
	setIndentResult(bool bIndent)
	{
        m_indentResult = bIndent == true ? eIndentYesExplicit : eIndentNoExplicit;
	}

	/**
	 * Change the value of the output method, one of the
	 * FormatterListener::eFormat values.
	 * 
	 * @param meth new method number
	 */
	void
	setOutputMethod(FormatterListener::eFormat	meth)
	{
		m_outputMethod = meth;
	}

	bool
	hasCDATASectionElements() const
	{
		assert(m_hasCDATASectionElems == false && m_cdataSectionElems.empty() == true ||
			   m_hasCDATASectionElems == true && m_cdataSectionElems.empty() == false);

		return m_hasCDATASectionElems;
	}

	/**
	 * Determine if a QName is in the list of CDATA section
	 * element QNames.
	 * 
	 * @param theQName The QName of the element to check.
	 * @return true or false
	 */
	bool
	isCDATASectionElementName(const XalanQName&		theQName) const;

	/**
	 * Given a valid element key, return the corresponding node list.
	 *
	 * @param doc			   source document
	 * @param name			   qname of the key, which must match the 'name'
	 *						   attribute on xsl:key
	 * @param ref			   value that must match the value found by the
	 *						   'match' attribute on xsl:key
	 * @param resolver		   resolver for namespace resolution
	 * @param nodelist         A node list to contain the nodes found
	 * @param executionContext current execution context
	 */
	void
	getNodeSetByKey(
			XalanDocument*					doc,
			const XalanQName&				qname,
			const XalanDOMString&			ref,
			const PrefixResolver&			resolver,
			MutableNodeRefList&				nodelist,
			StylesheetExecutionContext&		executionContext,
			KeyTablesTableType&				theKeysTable) const;

	unsigned long
	getNextElemNumberID()
	{
		return m_elemNumberNextID++;
	}

	unsigned long
	getElemNumberCount() const
	{
		return m_elemNumberNextID;
	}

	bool
	hasPreserveOrStripSpaceElements() const
	{
		return m_whitespaceElements.empty() == false;
	}

	bool
	shouldStripSourceNode(const XalanText&	textNode) const;

	void
	addAttributeSet(ElemAttributeSet&	theAttributeSet);
	
#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
	/**
	 * Get the nth attribute set with the specified name.
	 *  
	 * @param execution		context for executing this stylesheet
	 * @param theQName		the name of the attribute set
	 * @param matchingIndex	index of the attribute set with the specified name
	 * @param theLocator	the locator
	 * @returns a pointer to the attribute, 0 if no matching attribute set 
	 */
	const ElemAttributeSet*
	getAttributeSet(
			StylesheetExecutionContext&		theExecutionContext,
			const XalanQName&				theQName,
			size_type						matchingIndex,
			const LocatorType*				theLocator) const;
#else
	void
	executeAttributeSet(
			StylesheetExecutionContext&		theExecutionContext,
			const XalanQName&				theQName,
			const LocatorType*				theLocator) const;
#endif


private:

	/**
	 * Choose the encoding to use.
	 *
	 * @param outputTarget The output result target
	 *
	 * @return The chosen encoding
	 */
	const XalanDOMString&
	getEncoding(const XSLTResultTarget&		outputTarget) const;

	/**
	 * Create the default rule if needed.
	 *
	 * @param constructionContext context for construction of object
	 */
	void 
	initDefaultRule(StylesheetConstructionContext&	constructionContext);

	/**
	 * The version tells the version of XML to be used for outputting the result tree,
	 * as specified in xsl:output.
	 */
	XalanDOMString	m_version;

    enum eIndentType { eIndentNoImplicit, eIndentNoExplicit, eIndentYesImplicit, eIndentYesExplicit };

	/**
	 * indent-result is by default no, which means an XSL processor must not
	 * change the whitespace on output.
	 */
	eIndentType		m_indentResult;

	/**
	 * The encoding attribute specifies the preferred encoding to use 
	 * for outputting the result tree. 
	 */
	XalanDOMString	m_encoding;

	/**
	 * The media-type attribute is applicable for the xml output method. 
	 * The default value for the media-type attribute is text/xml.
	 */
	XalanDOMString	m_mediatype;

	/**
	 * If the doctype-system-id attribute is specified, the xml output method should 
	 * output a document type declaration immediately before the first element. 
	 * The name following <!DOCTYPE should be the name of the first element. 
	 */
	XalanDOMString	m_doctypeSystem;

	/**
	 * If doctype-public-id attribute is also specified, then the xml output 
	 * method should output PUBLIC followed by the public identifier and then 
	 * the system identifier; otherwise, it should output SYSTEM followed by 
	 * the system identifier. The internal subset should be empty. The 
	 * doctype-public-id attribute should be ignored unless the doctype-system-id 
	 * attribute is specified.
	 */
	XalanDOMString	m_doctypePublic;

	/**
	 * Tells whether or not to output an XML declaration.
	 */
	bool			m_omitxmlDecl;

	/**
	 * Tells what the xmldecl should specify for the standalone value.
     */
	XalanDOMString	m_standalone;

	/**
	 * The URL that belongs to the result namespace.
	 * @serial
	 */
	XalanDOMString				m_resultNameSpaceURL;

	/**
	 * The output method as specified in xsl:output.
	 */
	FormatterListener::eFormat	m_outputMethod;

	/**
	 * Vector of qnames that specifies elements that should be formatted 
	 * as CDATA.
	 */
	XalanQNameVectorType		m_cdataSectionElems;

	bool						m_hasCDATASectionElems;

	/**
	 * A stack of who's importing whom is needed in order to detect 
	 * a recursive include or import, which is an error.
	 */
	URLStackType				m_importStack;


	/**
	 * The default template to use for text nodes if we don't find 
	 * anything else.  This is initialized in initDefaultRule().
	 */
	ElemTemplateElement*		m_defaultTextRule;

	/**
	 * The default template to use if we don't find anything
	 * else.  This is initialized in initDefaultRule().
	 */
	ElemTemplateElement*		m_defaultRule;

	/**
	 * The default template to use for the root if we don't find 
	 * anything else.  This is initialized in initDefaultRule().
	 */
	ElemTemplateElement*		m_defaultRootRule;

	/**
	 * This is set to true if an xsl:key directive is found.
	 */
	bool						m_needToBuildKeysTable;

	/**
	 * This is set to true if URLs should be escaped in HTML output (the default)
	 */
	bool						m_outputEscapeURLs;

	/**
	 * The amount to indent.  The default is -1, which indicates not to indent.
	 */
	int							m_indentAmount;

	/**
	 * This is set to true if we should omit the META tag in HTML output (the default is false)
	 */
	bool						m_omitMETATag;

	/**
	 * This is set to true if we should omit the META tag in HTML output (the default is false)
	 */
	unsigned long				m_elemNumberNextID;

	/**
	 * A lookup table of all attribute sets.
	 */
	AttributeSetMapType			m_attributeSetsMap;


	// Not implemented...
    StylesheetRoot(const StylesheetRoot&);

    StylesheetRoot&
    operator=(const StylesheetRoot&);

    bool
    operator==(const StylesheetRoot&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_STYLESHEETROOT_HEADER_GUARD
