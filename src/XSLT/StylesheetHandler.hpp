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

#if !defined(XALAN_STYLESHEETHANDLER_HEADER_GUARD)
#define XALAN_STYLESHEETHANDLER_HEADER_GUARD

// Base include file.	Must be first.
#include "XSLTDefinitions.hpp"



#include <vector>
#include <set>



#include <XalanDOM/XalanDOMString.hpp>



#include <XMLSupport/FormatterListener.hpp>



#include <XSLT/NamespacesHandler.hpp>
#include <XSLT/Stylesheet.hpp>



class ElemTemplate;
class ElemTemplateElement;
class ElemTextLiteral;
class ExtensionNSHandler;
class StylesheetConstructionContext;



/**
 * This class processes a stylesheet via SAX events, and inits
 * the given stylesheet.	If you need to alter the code in here, 
 * it is not for the faint-of-heart, due to the state tracking 
 * that has to be done due to the SAX event model.
 */
class XALAN_XSLT_EXPORT StylesheetHandler : public FormatterListener
{

public:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<ElemTemplateElement*>		ElemTemplateStackType;
	typedef vector<ElemTextLiteral*>			ElemTextLiteralStackType;
	typedef set<ElemTemplateElement*,
				less<ElemTemplateElement*> >	ElemTemplateSetType;
	typedef vector<bool>						BoolStackType;
#else
	typedef std::vector<ElemTemplateElement*>	ElemTemplateStackType;
	typedef std::vector<ElemTextLiteral*>		ElemTextLiteralStackType;
	typedef std::set<ElemTemplateElement*>		ElemTemplateSetType;
	typedef std::vector<bool>					BoolStackType;
#endif

	/**
	 * Stack to keep track of the current include base.
	 */
	XalanDOMString m_includeBase;

	/**
	 * Construct a StylesheetHandler ... it will add the DOM nodes 
	 * to the document fragment.
	 */
	StylesheetHandler(
			Stylesheet&						stylesheetTree,
			StylesheetConstructionContext&	constructionContext);

	virtual
	~StylesheetHandler();

	/**
	 * Receive notification of character data.
	 *
	 * <p>The Parser will call this method to report each chunk of
	 * character data.  SAX parsers may return all contiguous character
	 * data in a single chunk, or they may split it into several
	 * chunks; however, all of the characters in any single event
	 * must come from the same external entity, so that the Locator
	 * provides useful information.</p>
	 *
	 * <p>The application must not attempt to read from the array
	 * outside of the specified range.</p>
	 *
	 * <p>Note that some parsers will report whitespace using the
	 * ignorableWhitespace() method rather than this one (validating
	 * parsers must do so).</p>
	 *
	 * @param chars  pointer to characters from the XML document
	 * @param length number of characters to read from the array
	 * @exception SAXException
	 * @see #ignorableWhitespace 
	 * @see org.xml.sax.Locator
	 */
	virtual void characters (const XMLCh* const chars, const unsigned int length);

	/**
	 * Receive notification of character data. If available, when the
	 * disable-output-escaping attribute is used, output raw text without
	 * escaping.
	 *
	 * @param ch pointer to characters from the XML document
	 * @param start start position in the array
	 * @param length number of characters to read from the array
	 * @exception SAXException
	 */
	virtual void charactersRaw(const XMLCh* const chars, const unsigned int	length);

	/**
	 * Receive notification of cdata.
	 *
	 * <p>The Parser will call this method to report each chunk of
	 * character data.  SAX parsers may return all contiguous character
	 * data in a single chunk, or they may split it into several
	 * chunks; however, all of the characters in any single event
	 * must come from the same external entity, so that the Locator
	 * provides useful information.</p>
	 *
	 * <p>The application must not attempt to read from the array
	 * outside of the specified range.</p>
	 *
	 * <p>Note that some parsers will report whitespace using the
	 * ignorableWhitespace() method rather than this one (validating
	 * parsers must do so).</p>
	 *
	 * @param ch     pointer to characters from the XML document
	 * @param start  start position in the array
	 * @param length number of characters to read from the array
	 * @exception SAXException
	 * @see #ignorableWhitespace 
	 */
	virtual void cdata(const XMLCh* const ch, const unsigned int length);

	/**
	 * Receive notification of ignorable whitespace in element content.
	 *
	 * <p>Validating Parsers must use this method to report each chunk
	 * of ignorable whitespace (see the W3C XML 1.0 recommendation,
	 * section 2.10): non-validating parsers may also use this method
	 * if they are capable of parsing and using content models.</p>
	 *
	 * <p>SAX parsers may return all contiguous whitespace in a single
	 * chunk, or they may split it into several chunks; however, all of
	 * the characters in any single event must come from the same
	 * external entity, so that the Locator provides useful
	 * information.</p>
	 *
	 * <p>The application must not attempt to read from the array
	 * outside of the specified range.</p>
	 *
	 * @param chars  characters from the XML document
	 * @param start  start position in the array
	 * @param length  number of characters to read from the array
	 * @exception SAXException
	 * @see #characters
	 */
	virtual void ignorableWhitespace (const XMLCh* const chars, const unsigned int length);

	/**
	 * Receive notification of a processing instruction.
	 *
	 * <p>The Parser will invoke this method once for each processing
	 * instruction found: note that processing instructions may occur
	 * before or after the main document element.</p>
	 *
	 * <p>A SAX parser should never report an XML declaration (XML 1.0,
	 * section 2.8) or a text declaration (XML 1.0, section 4.3.1)
	 * using this method.</p>
	 *
	 * @param target pointer to processing instruction target
	 * @param data   pointer to processing instruction data, or null if none
	 *               was supplied
	 * @exception SAXException
	 */
	virtual void processingInstruction (const XMLCh* const target, const XMLCh* const data);

	/**
	 * Called when a Comment is to be constructed.
	 *
	 * @param data comment data
	 * @exception SAXException
	 */
	virtual void comment(const XMLCh* const data);

	/**
	 * Receive notification of a entityReference.
	 *
	 * @param data pointer to characters from the XML document
	 * @exception SAXException
	 */
	virtual void entityReference(const XMLCh* const data);

	// These methods are inherited DocumentHandler ...
	
	// $$$ Theoretically, shouldn't need javadoc for these, since they are
	// inherited from DocumentHandler, but let's leave them in for now -- JMD

	/**
	 * Receive an object for locating the origin of SAX document events.
	 *
	 * <p>SAX parsers are strongly encouraged (though not absolutely
	 * required) to supply a locator: if it does so, it must supply
	 * the locator to the application by invoking this method before
	 * invoking any of the other methods in the DocumentHandler
	 * interface.</p>
	 *
	 * <p>The locator allows the application to determine the end
	 * position of any document-related event, even if the parser is
	 * not reporting an error.  Typically, the application will
	 * use this information for reporting its own errors (such as
	 * character content that does not match an application's
	 * business rules).  The information returned by the locator
	 * is probably not sufficient for use with a search engine.</p>
	 *
	 * <p>Note that the locator will return correct information only
	 * during the invocation of the events in this interface.  The
	 * application should not attempt to use it at any other time.</p>
	 *
	 * @param locator object that can return the location of
	 *                any SAX document event.
	 * @see org.xml.sax.Locator
	 */
	virtual void setDocumentLocator(const Locator* const	locator);

	/**
	 * Receive notification of the beginning of a document.
	 *
	 * <p>The SAX parser will invoke this method only once, before any
	 * other methods in this interface or in DTDHandler (except for
	 * setDocumentLocator).</p>
	 *
	 * @exception SAXException
	 */
	virtual void startDocument();

	/**
	 * Receive notification of the end of a document.
	 *
	 * <p>The SAX parser will invoke this method only once, and it will
	 * be the last method invoked during the parse.  The parser shall
	 * not invoke this method until it has either abandoned parsing
	 * (because of an unrecoverable error) or reached the end of
	 * input.</p>
	 *
	 * @exception SAXException
	 */
	virtual void endDocument ();
	
	/**
	 * Receive notification of the beginning of an element.
	 *
	 * <p>The Parser will invoke this method at the beginning of every
	 * element in the XML document; there will be a corresponding
	 * endElement() event for every startElement() event (even when the
	 * element is empty). All of the element's content will be
	 * reported, in order, before the corresponding endElement()
	 * event.</p>
	 *
	 * <p>If the element name has a namespace prefix, the prefix will
	 * still be attached.  Note that the attribute list provided will
	 * contain only attributes with explicit values (specified or
	 * defaulted): #IMPLIED attributes will be omitted.</p>
	 *
	 * @param name   element type name
	 * @param atts   attributes attached to the element, if any
	 * @exception SAXException
	 * @see #endElement
	 * @see org.xml.sax.AttributeList
	 */
	virtual void startElement (const XMLCh* const name, AttributeList& attrs);
	
	/**
	 * Receive notification of the end of an element.
	 *
	 * <p>The SAX parser will invoke this method at the end of every
	 * element in the XML document; there will be a corresponding
	 * startElement() event for every endElement() event (even when the
	 * element is empty).</p>
	 *
	 * <p>If the element name has a namespace prefix, the prefix will
	 * still be attached to the name.</p>
	 *
	 * @param name element type name
	 * @exception SAXException
	 */
	virtual void endElement(const XMLCh* const name);
	
	/**
	 * This method allows the user installed Document Handler to 'reset'
	 * itself, freeing all the memory resources. The scanner calls this
	 * method before starting a new parse event.
	 */
	virtual void resetDocument();

protected:

	/** 
	 * See if this is a xmlns attribute, and, if so, process it.
	 * 
	 * @param attrName Qualified name of attribute.
	 * @param atts The attribute list where the element comes from (not used at 
	 *      this time).
	 * @param which The index into the attribute list (not used at this time).
	 * @return True if this is a namespace name.
	 */
	bool
	isAttrOK(
			const XalanDOMChar*		attrName,
			const AttributeList&	atts,
			int						which);

	/** 
	 * Tell whether or not this is a xml:space attribute and, if so, process it.
	 * 
	 * @param aname The name of the attribute in question.
	 * @param atts The attribute list that owns the attribute.
	 * @param which The index of the attribute into the attribute list.
	 * @return True if this is a xml:space attribute.
	 */
	bool
	processSpaceAttr(
			const XalanDOMChar*		aname,
			const AttributeList&	atts,
			int						which);

	/**
	 * Process xsl:import.
	 */
	void
	processImport(
			const XalanDOMChar*		name,
			const AttributeList&	atts);

	/**
	 * Process xsl:include.
	 */
	void
	processInclude(const XalanDOMChar*		name,
				   const AttributeList&		atts);

private:

	// not implemented
	StylesheetHandler(const StylesheetHandler&);

	StylesheetHandler&
	operator=(const StylesheetHandler&);

	// Utility function...
	void
	processText(
			const XMLCh* const	chars,
			const unsigned int	length);

	void
	accumulateText(
			const XMLCh* const	chars,
			const unsigned int	length);

	void
	processAccumulatedText();

	bool
	inExtensionElement() const;

	// Data members...
	XalanDOMString	m_pendingException;

	bool			m_exceptionPending;

	/**
	 * The owning stylesheet.
	 */
	Stylesheet&		m_stylesheet;

	/**
	 * The construction context.
	 */
	StylesheetConstructionContext&	m_constructionContext;

	/**
	 * The stack of elements, pushed and popped as events occur.
	 */
	ElemTemplateStackType	m_elemStack;

	/**
	 * The set of elements in m_elemStack which have already
	 * been parented.  This prevents us from deleting them
	 * twice if an exception is thrown.
	 */
	ElemTemplateSetType		m_elemStackParentedElements;

	/**
	 * The stack of stray elements, to be delete when finished.
	 */
	ElemTemplateStackType	m_strayElements;

	/**
	 * Need to keep a stack of found whitespace elements so that 
	 * whitespace elements next to non-whitespace elements can 
	 * be merged.  For instance: &lt;out> &lt;![CDATA[test]]> &lt;/out>
	 */
	ElemTextLiteralStackType m_whiteSpaceElems;

	/**
	 * The current template.
	 */
	ElemTemplate* m_pTemplate;
	
	/**
	 * The last element popped from the stack.  I'm not totally clear 
	 * anymore as to why this is needed.
	 */
	ElemTemplateElement* m_lastPopped;
	
	/**
	 * True if the process is in a template context.
	 */
	bool m_inTemplate;
	
	/**
	 * True if the stylesheet element was found, or if it was determined that 
	 * the stylesheet is wrapperless.
	 */
	bool m_foundStylesheet;
	
	/**
	 * Flag to let us know when we've found an element inside the 
	 * stylesheet that is not an xsl:import, so we can restrict imports 
	 * to being the first elements.
	 */
	bool m_foundNotImport;

	/**
	 * Accumulate character buffer to create contiguous character data
	 * where possible.
	 */
	XalanDOMString	m_accumulateText;

	BoolStackType	m_inExtensionElementStack;

	// BEGIN SANJIVA CODE
	bool m_inLXSLTScript;
	XalanDOMString m_LXSLTScriptBody;
	XalanDOMString m_LXSLTScriptLang;
	XalanDOMString m_LXSLTScriptSrcURL;
	ExtensionNSHandler* m_pLXSLTExtensionNSH;
	// END SANJIVA CODE

	/**
	 * Init the wrapperless template
	 */
	ElemTemplateElement*
	initWrapperless(
			const XalanDOMChar*		name,
			const AttributeList&	atts,
			int						lineNumber,
			int						columnNumber);


	class PushPopIncludeState
	{
	public:

		PushPopIncludeState(StylesheetHandler&		theHandler);

		~PushPopIncludeState();

	private:

		StylesheetHandler&					m_handler;

		ElemTemplateStackType				m_elemStack;

		ElemTemplateSetType					m_elemStackParentedElements;

		ElemTemplate* const					m_pTemplate;

		ElemTemplateElement* const			m_lastPopped;		

		const bool							m_inTemplate;		

		const bool							m_foundStylesheet;

		const XalanDOMString				m_XSLNameSpaceURL;

		const bool							m_foundNotImport;

		Stylesheet::NamespaceVectorType		m_namespaceDecls;

		Stylesheet::NamespacesStackType		m_namespaces;

		NamespacesHandler					m_namespacesHandler;

		BoolStackType						m_inExtensionElementStack;
	};

	friend class PushPopIncludeState;
};



#endif	// XALAN_STYLESHEETHANDLER_HEADER_GUARD
