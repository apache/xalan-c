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
#if !defined(XALAN_STYLESHEETROOT_HEADER_GUARD)
#define XALAN_STYLESHEETROOT_HEADER_GUARD 



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include "Stylesheet.hpp"



#include <XalanDOM/XalanDOMString.hpp>



#include <XMLSupport/FormatterListener.hpp>



class StylesheetConstructionContext;
class XMLURL;
class XSLTResultTarget;



/**
 * This acts as the stylesheet root of the stylesheet 
 * tree, and holds values that are shared by all 
 * stylesheets in the tree.
 */
class XALAN_XSLT_EXPORT StylesheetRoot : public Stylesheet
{
public:

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

	/**
	 * Transform the source tree to the output in the given result tree target.
	 *
	 * @param inputSource  The input source tree
	 * @param outputTarget The output source tree
	 * @param constructionContext context for construction of object
	 */
	void
	process(
			XalanNode*						sourceTree, 
			XSLTResultTarget&				outputTarget,
			StylesheetExecutionContext&		executionContext) const;

	/** 
	 * Return the output method that was specified in the stylesheet. 
	 * The returned value is one of FormatterLister::eFormat values.
	 *
	 * @return value of output method
	 */
	FormatterListener::eFormat
	getOutputMethod() const;

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
	XalanDOMString
	getOutputVersion() const;

	/**
	 * Determine if output indenting was specified in the
	 * xsl:output element
	 *
	 * @return true to indent
	 */
	bool 
	getOutputIndent() const;

	/**
	 * Get the output encoding string that was specified in the
	 * xsl:output element
	 *
	 * @return encoding string
	 */
	XalanDOMString 
	getOutputEncoding() const;

	/**
	 * Get the java output encoding string that was specified in the
	 * xsl:output element
	 *
	 * @return encoding string
	 */
	XalanDOMString 
	getJavaOutputEncoding() const;

	/**
	 * Get the doctype-system-id string that was specified in the 
	 * xsl:output element
	 *
	 * @return document type string
	 */
	XalanDOMString 
	getOutputDoctypeSystem() const;

	/**
	 * Get the media-type string that was specified in the 
	 * xsl:output element
	 *
	 * @return media type string
	 */
	XalanDOMString 
	getOutputMediaType() const;
	
	/**
	 * Get the doctype-public-id string that was specified in the 
	 * xsl:output element
	 *
	 * @return document type public id string
	 */
	XalanDOMString 
	getOutputDoctypePublic() const;

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
	XalanDOMString
	getOutputStandalone() const
	{
		return m_standalone;
	}

	/**
	 * Get the template representing the default rule for text.
	 *
	 * @return pointer to template rule for text
	 */
	ElemTemplate* 
	getDefaultTextRule() const
	{
		return m_defaultTextRule;
	}

	/**
	 * Get the template representing the default rule.
	 *
	 * @return pointer to default template rule
	 */
	ElemTemplate* 
	getDefaultRule() const
	{
		return m_defaultRule;
	}

	/**
	 * Get the template representing the default root rule.
	 *
	 * @return pointer to default root template rule
	 */
	ElemTemplate* 
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
			const AttributeList&			atts,
			StylesheetConstructionContext&	constructionContext);

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
	XalanDOMString m_version;

	/**
	 * indent-result is by default no, which means an XSL processor must not
	 * change the whitespace on output.
	 */
	bool m_indentResult;

	/**
	 * The encoding attribute specifies the preferred encoding to use 
	 * for outputting the result tree. 
	 */
	XalanDOMString m_encoding;

	/**
	 * The media-type attribute is applicable for the xml output method. 
	 * The default value for the media-type attribute is text/xml.
	 */
	XalanDOMString m_mediatype;

	/**
	 * If the doctype-system-id attribute is specified, the xml output method should 
	 * output a document type declaration immediately before the first element. 
	 * The name following <!DOCTYPE should be the name of the first element. 
	 */
	XalanDOMString m_doctypeSystem;

	/**
	 * If doctype-public-id attribute is also specified, then the xml output 
	 * method should output PUBLIC followed by the public identifier and then 
	 * the system identifier; otherwise, it should output SYSTEM followed by 
	 * the system identifier. The internal subset should be empty. The 
	 * doctype-public-id attribute should be ignored unless the doctype-system-id 
	 * attribute is specified.
	 */
	XalanDOMString m_doctypePublic;

	/**
	 * Tells whether or not to output an XML declaration.
	 */
	bool m_omitxmlDecl;

	/**
	 * Tells what the xmldecl should specify for the standalone value.
     */
	XalanDOMString m_standalone;

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
		m_indentResult = bIndent;
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

	/**
	 * Retrieve list of CDATA section elements.
	 * 
	 * @return vector of elements
	 */
	const QNameVectorType&
	getCDATASectionElems() const
	{
		return m_cdataSectionElems;
	}

private:

	/**
	 * The URL that belongs to the result namespace.
	 * @serial
	 */
	XalanDOMString	m_resultNameSpaceURL;

	/**
	 * The output method as specified in xsl:output.
	 */
	FormatterListener::eFormat	m_outputMethod;

	/**
	 * List of qnames that specifies elements that should be formatted 
	 * as CDATA.
	 */
	QNameVectorType		m_cdataSectionElems;

	/**
	 * A stack of who's importing who is needed in order to support 
	 * "It is an error if a stylesheet directly or indirectly imports 
	 * itself. Apart from this, the case where a stylesheet with a 
	 * particular URI is imported in multiple places is not treated 
	 * specially."
	 */
	URLStackType m_importStack;


	/**
	 * The default template to use for text nodes if we don't find 
	 * anything else.  This is initialized in initDefaultRule().
	 * @serial
	 */
	ElemTemplate* m_defaultTextRule;

	/**
	 * The default template to use if we don't find anything
	 * else.  This is initialized in initDefaultRule().
	 * @serial
	 */
	ElemTemplate* m_defaultRule;

	/**
	 * The default template to use for the root if we don't find 
	 * anything else.  This is initialized in initDefaultRule().
	 * We kind of need this because the defaultRule isn't good 
	 * enough because it doesn't supply a document context.
	 * For now, I default the root document element to "HTML".
	 * Don't know if this is really a good idea or not.
	 * I suspect it is not.
	 * @serial
	 */
	ElemTemplate* m_defaultRootRule;

};



#endif	// XALAN_STYLESHEETROOT_HEADER_GUARD
