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
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
#if !defined(STYLESHEETCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680)
#define STYLESHEETCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <vector>



#include <PlatformSupport/ArenaAllocator.hpp>
#include <PlatformSupport/XalanArrayAllocator.hpp>
#include <PlatformSupport/XalanDOMStringPool.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION) || (XALAN_ALLINONE_BUILD_DLL)
#include <XPath/XPathProcessor.hpp>
#endif



#include <Include/XalanAutoPtr.hpp>



#include <PlatformSupport/XalanDOMStringCache.hpp>



#include <XPath/XalanQNameByReference.hpp>
#include <XPath/XalanQNameByValue.hpp>
#include <XPath/XalanQNameByValueAllocator.hpp>



// Base class header file...
#include <XSLT/StylesheetConstructionContext.hpp>



#include <XSLT/AVT.hpp>
#include <XSLT/XalanAVTAllocator.hpp>
#include <XSLT/AVTPartSimple.hpp>
#include <XSLT/XalanAVTPartSimpleAllocator.hpp>
#include <XSLT/AVTPartXPath.hpp>
#include <XSLT/XalanAVTPartXPathAllocator.hpp>



class XPathEnvSupport;
class XPathFactory;
class XPathProcessor;
class XSLTEngineImpl;



/**
 *
 * An default implementation of an abtract class which provides support for
 * constructing the internal representation  of a stylesheet.
 *
 */
class XALAN_XSLT_EXPORT StylesheetConstructionContextDefault : public StylesheetConstructionContext
{
public:

	typedef XalanArrayAllocator<XalanDOMChar>			XalanDOMCharVectorAllocatorType;
	typedef XalanArrayAllocator<const void*>			PointerVectorAllocatorType;
	typedef XalanDOMCharVectorAllocatorType::size_type	VectorAllocatorSizeType;

    // Default size for vector allocation.
	enum {
			eDefaultXalanDOMCharVectorBlockSize = 1024,
			eDefaultAVTBlockSize = 128,
			eDefaultAVTPartSimpleBlockSize = 128,
			eDefaultAVTPartXPathBlockSize = 128,
			eDefaultXalanQNameByValueBlockSize = 32,
			eDefaultPointerVectorBlockSize = 512 };

	/*
	 * Construct an instance.  If the stylesheet(s) constructed is/are meant to be reused (a.k.a. "compiled"),
	 * the XObjectFactory and XPathFactory instance must exist for the lifetime of the construction context
	 * and, therefore, for the lifetime of the stylesheet(s).  Otherwise, XObject and XPath instance will be
	 * destroyed when the corresponding factories are destryed, leaving pointers to destroyed objects in the.
	 * stylesheet(s).
	 *
	 * @param processor a reference to an XSLTEngineImpl instance.  Used for error reporting.
	 * @param xpathFactory a reference to an XPathFactory instance.  See comments above for important details.
	 * @param theXalanDOMCharVectorAllocatorBlockSize The block size to use for allocating vectors of XalanDOMChars
	 * @param theAVTAllocatorBlockSize The block size to use for allocating AVT instances.
	 * @param theAVTPartSimpleAllocatorBlockSize The block size to use for allocating AVTPartSimple instances.
	 * @param theAVTPartXPathAllocatorBlockSize The block size to use for allocating AVTPartXPath instances.
	 * @param theXalanQNameByValueAllocatorBlockSize The block size to use for allocating XalanQNameByValue instances.
	 * @param thePointerVectorAllocatorBlockSize The block size to use for allocating vectors of pointers.
	 */
	StylesheetConstructionContextDefault(
			XSLTEngineImpl&							processor,
			XPathFactory&							xpathFactory,
			VectorAllocatorSizeType					theXalanDOMCharVectorAllocatorBlockSize = eDefaultXalanDOMCharVectorBlockSize,
			XalanAVTAllocator::size_type			theAVTAllocatorBlockSize = eDefaultAVTBlockSize,
			XalanAVTPartSimpleAllocator::size_type	theAVTPartSimpleAllocatorBlockSize = eDefaultAVTPartSimpleBlockSize,
			XalanAVTPartXPathAllocator::size_type	theAVTPartXPathAllocatorBlockSize = eDefaultAVTPartXPathBlockSize,
			XalanQNameByValueAllocator::size_type	theXalanQNameByValueAllocatorBlockSize = eDefaultXalanQNameByValueBlockSize,
			VectorAllocatorSizeType					thePointerVectorAllocatorBlockSize = eDefaultPointerVectorBlockSize);

	virtual
	~StylesheetConstructionContextDefault();


	virtual void
	error(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement*	styleNode = 0) const;

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const;

	virtual void
	error(
			const char*					msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement* 	styleNode = 0) const;

	virtual void
	error(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const;

	virtual void
	warn(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement* 	styleNode = 0) const;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const;

	virtual void
	warn(
			const char*					msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement* 	styleNode = 0) const;

	virtual void
	warn(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const;

	virtual void
	message(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement* 	styleNode = 0) const;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const;

	virtual void
	message(
			const char*					msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement* 	styleNode = 0) const;

	virtual void
	message(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const;

	// These interfaces are inherited from StylesheetConstructionContext...

	virtual void
	reset();

	virtual StylesheetRoot*
	create(const XalanDOMString&	theBaseIdentifier);

	virtual StylesheetRoot*
	create(const XSLTInputSource&	theInputSource);

	virtual Stylesheet*
	create(
			StylesheetRoot&			theStylesheetRoot,
			const XalanDOMString&	theBaseIdentifier);

	virtual void
	destroy(StylesheetRoot*		theStylesheetRoot);

	virtual URLAutoPtrType
	getURLFromString(const XalanDOMString&	urlString);

	virtual XalanDOMString
	getURLStringFromString(const XalanDOMString&	urlString);

	virtual URLAutoPtrType
	getURLFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base);

	virtual XalanDOMString
	getURLStringFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base);

	virtual const XalanDOMString&
	getXSLTNamespaceURI() const;

	virtual XPath*
	createMatchPattern(
			const Locator*			locator,
			const XalanDOMString&	str,
			const PrefixResolver&	resolver);

	virtual XPath*
	createMatchPattern(
			const Locator*			locator,
			const XalanDOMChar*		str,
			const PrefixResolver&	resolver);

	virtual XPath*
	createXPath(
			const Locator*			locator,
			const XalanDOMString&	str,
			const PrefixResolver&	resolver);

	virtual XPath*
	createXPath(
			const Locator*				locator,
			const XalanDOMChar*			str,
			XalanDOMString::size_type	len,
			const PrefixResolver&		resolver);

	virtual XPath*
	createXPath(
			const Locator*			locator,
			const XalanDOMChar*		str,
			const PrefixResolver&	resolver);

	virtual const Locator*
	getLocatorFromStack() const;

	virtual void
	pushLocatorOnStack(const Locator*	locator);

	virtual void
	popLocatorStack();

	virtual const XalanDOMString&
	getXalanXSLNameSpaceURL() const;

	virtual XalanDocument*
	parseXML(
			const XalanDOMString&	urlString,
			DocumentHandler*		docHandler, 
			XalanDocument*			docToRegister);

	virtual bool
	isXMLSpaceAttribute(
			const XalanDOMChar*		theAttributeName,
			const Stylesheet&		theStylesheet,
			const Locator*			theLocator = 0);

	virtual bool
	isXSLUseAttributeSetsAttribute(
			const XalanDOMChar*		theAttributeName,
			const Stylesheet&		theStylesheet,
			const Locator*			theLocator = 0);

	virtual bool
	isValidQName(
			const XalanDOMChar*		theName,
			const Stylesheet&		theStylesheet,
			const Locator*			theLocator = 0);

	virtual int
	getElementToken(const XalanDOMString&	name) const;

	virtual double
	getXSLTVersionSupported() const;

	virtual const XalanDOMString&
	getPooledString(const XalanDOMString&	theString);

	virtual const XalanDOMString&
	getPooledString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

	virtual XalanDOMString&
	getCachedString();

	virtual bool
	releaseCachedString(XalanDOMString&		theString);

	virtual XalanDOMChar*
	allocateXalanDOMCharVector(XalanDOMString::size_type	theLength);

	virtual XalanDOMChar*
	allocateXalanDOMCharVector(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos,
			bool						fTerminate = true);

	virtual const AVT*
	createAVT(
			const Locator*					locator,
			const XalanDOMChar*				name,
			const XalanDOMChar*				stringedValue,
			const PrefixResolver&			resolver);

	virtual const AVTPart*
	createAVTPart(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

	virtual const AVTPart*
	createAVTPart(
			const Locator*				locator,
			const XalanDOMChar*			str,
			XalanDOMString::size_type	len,
			const PrefixResolver&		resolver);

	virtual const AVT**
	allocateAVTPointerVector(size_type	theLength);

	virtual const AVTPart**
	allocateAVTPartPointerVector(size_type	theLength);

	virtual const XalanQName*
	createXalanQName(
			const XalanDOMString&		qname,
			const NamespacesStackType&	namespaces,
			const Locator*				locator = 0,
			bool						fUseDefault = false);

	virtual const XalanQName*
	createXalanQName(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces,
			const Locator*				locator = 0,
			bool						fUseDefault = false);

	virtual const XalanQName**
	tokenizeQNames(
			size_type&					count,
			const XalanDOMChar*			qnameTokens,
			const NamespacesStackType&	namespaces,
			const Locator*				locator = 0,
			bool						fUseDefault = false);

	static int
	getElementNameToken(const XalanDOMString&	name);

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<StylesheetRoot*>			StylesheetVectorType;
#else
	typedef std::vector<StylesheetRoot*>	StylesheetVectorType;
#endif

private:

	const AVT**
	doAllocateAVTPointerVector(size_type	theSize)
	{
		assert(sizeof(AVT**) == sizeof(PointerVectorAllocatorType::value_type));

#if defined(XALAN_OLD_STYLE_CASTS)
		return (const AVT**)m_pointerVectorAllocator.allocate(theSize);
#else
		return reinterpret_cast<const AVT**>(m_pointerVectorAllocator.allocate(theSize));
#endif
	}

	const AVTPart**
	doAllocateAVTPartPointerVector(size_type	theSize)
	{
		assert(sizeof(AVTPart**) == sizeof(PointerVectorAllocatorType::value_type));

#if defined(XALAN_OLD_STYLE_CASTS)
		return (const AVTPart**)m_pointerVectorAllocator.allocate(theSize);
#else
		return reinterpret_cast<const AVTPart**>(m_pointerVectorAllocator.allocate(theSize));
#endif
	}

	const XalanQName**
	doAllocateXalanQNamePointerVector(size_type	theSize)
	{
		assert(sizeof(XalanQName**) == sizeof(PointerVectorAllocatorType::value_type));

#if defined(XALAN_OLD_STYLE_CASTS)
		return (const XalanQName**)m_pointerVectorAllocator.allocate(theSize);
#else
		return reinterpret_cast<const XalanQName**>(m_pointerVectorAllocator.allocate(theSize));
#endif
	}


	XSLTEngineImpl&							m_processor;

	XPathFactory&							m_xpathFactory;

	typedef XalanAutoPtr<XPathProcessor>	XPathProcessAutoPtr;

	XPathProcessAutoPtr						m_xpathProcessor;

	StylesheetVectorType					m_stylesheets;

	XalanDOMStringPool						m_stringPool;

	XalanDOMCharVectorAllocatorType			m_xalanDOMCharVectorAllocator;

	mutable XalanDOMString					m_tempBuffer;

	XalanQNameByValue						m_scratchQName;

	XalanDOMStringCache						m_stringCache;

	XalanAVTAllocator						m_avtAllocator;

	XalanAVTPartSimpleAllocator				m_avtPartSimpleAllocator;

	XalanAVTPartXPathAllocator				m_avtPartXPathAllocator;

	XalanQNameByValueAllocator				m_xalanQNameByValueAllocator;

	const XalanQNameByReference				m_useAttributeSetsQName;

	PointerVectorAllocatorType				m_pointerVectorAllocator;

	static const XalanQNameByReference	s_spaceAttrQName;

	// Static strings for stylesheet compilation...

	// The string "if"
	static const XalanDOMChar	s_if[];

	// The string "key"
	static const XalanDOMChar	s_key[];

	// The string "copy"
	static const XalanDOMChar	s_copy[];

	// The string "sort"
	static const XalanDOMChar	s_sort[];

	// The string "text"
	static const XalanDOMChar	s_text[];

	// The string "when"
	static const XalanDOMChar	s_when[];

	// The string "empty"
	static const XalanDOMChar	s_empty[];

	// The string "param"
	static const XalanDOMChar	s_param[];

	// The string "choose"
	static const XalanDOMChar	s_choose[];

	// The string "import"
	static const XalanDOMChar	s_import[];

	// The string "number"
	static const XalanDOMChar	s_number[];

	// The string "output"
	static const XalanDOMChar	s_output[];

	// The string "comment"
	static const XalanDOMChar	s_comment[];

	// The string "copy-of"
	static const XalanDOMChar	s_copyOf[];

	// The string "element"
	static const XalanDOMChar	s_element[];

	// The string "include"
	static const XalanDOMChar	s_include[];

	// The string "message"
	static const XalanDOMChar	s_message[];

	// The string "fallback"
	static const XalanDOMChar	s_fallback[];

	// The string "for-each"
	static const XalanDOMChar	s_forEach[];

	// The string "template"
	static const XalanDOMChar	s_template[];

	// The string "value-of"
	static const XalanDOMChar	s_valueOf[];

	// The string "variable"
	static const XalanDOMChar	s_variable[];

	// The string "attribute"
	static const XalanDOMChar	s_attribute[];

	// The string "otherwise"
	static const XalanDOMChar	s_otherwise[];

	// The string "transform"
	static const XalanDOMChar	s_transform[];

	// The string "stylesheet"
	static const XalanDOMChar	s_stylesheet[];

	// The string "with-param"
	static const XalanDOMChar	s_withParam[];

	// The string "strip-space"
	static const XalanDOMChar	s_stripSpace[];

	// The string "apply-imports"
	static const XalanDOMChar	s_applyImports[];

	// The string "attribute-set"
	static const XalanDOMChar	s_attributeSet[];

	// The string "call-template"
	static const XalanDOMChar	s_callTemplate[];

	// The string "decimal-format"
	static const XalanDOMChar	s_decimalFormat[];

	// The string "preserve-space"
	static const XalanDOMChar	s_preserveSpace[];

	// The string "apply-templates"
	static const XalanDOMChar	s_applyTemplates[];

	// The string "namespace-alias"
	static const XalanDOMChar	s_namespaceAlias[];

	// The string "processing-instruction"
	static const XalanDOMChar	s_processingInstruction[];

	// A struct for an array that maps stylesheet element names
	// to int tokens.
	struct ElementTokenTableEntry
	{
		const XalanDOMChar*		m_name;

		eElementToken			m_token;
	};

	static const ElementTokenTableEntry		s_elementTokenTable[];

	static const unsigned int				s_elementTokenTableSize;

	static const ElementTokenTableEntry&	s_elementTokenTableLast;

	static const ElementTokenTableEntry&	s_elementTokenTableDummy;
};



#endif	// STYLESHEETCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680
