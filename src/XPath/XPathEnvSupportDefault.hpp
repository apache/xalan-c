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
#if !defined(XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680)
#define XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <vector>



#include <XPath/XPathEnvSupport.hpp>



/**
 * Dummy class in order to make the XPath object happy 
 * for diagnostic purposes.
 */
class XALAN_XPATH_EXPORT XPathEnvSupportDefault : public XPathEnvSupport
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XObject*> XObjectPtrVectorType;
	typedef map<DOMString, DOM_Document>	SourceDocsTableType;
#else
	typedef std::vector<XObject*> XObjectPtrVectorType;
	typedef std::map<DOMString, DOM_Document>	SourceDocsTableType;
#endif


	XPathEnvSupportDefault();

	virtual
	~XPathEnvSupportDefault();

	// These interfaces are inherited from XPathEnvSupport...

	virtual const NodeRefListBase*
	getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const DOM_Element&		nscontext,
			XPathExecutionContext&	executionContext) const;

	virtual const NodeRefListBase*
	getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const PrefixResolver&	resolver,
			XPathExecutionContext&	executionContext) const;

	virtual XObject*
	getVariable(
			XObjectFactory&		factory,
			const QName&		name) const;

	virtual DOM_Document
	parseXML(
			const DOMString&	urlString,
			const DOMString&	base) const;

	virtual DOM_Document
	getSourceDocument(const DOMString&	theURI) const;

	virtual void
	setSourceDocument(
			const DOMString&		theURI,
			const DOM_Document&		theDocument);

	virtual DOMString
	findURIFromDoc(const DOM_Document&	owner) const;

	virtual DOM_Document
	getDOMFactory() const;

	virtual bool
	functionAvailable(
			const DOMString&	theNamespace, 
			const DOMString&	extensionName) const;

	// $$$ ToDo: How do we implement this?
	virtual XObject*
	extFunction(
			XPathExecutionContext&			executionContext,
			const DOMString&				theNamespace,
			const DOMString&				extensionName, 
			const std::vector<XObject*>&	argVec) const;

	virtual XLocator*
	getXLocatorFromNode(const DOM_Node&		node) const;

	virtual void
	associateXLocatorToNode(
			const DOM_Node&		node,
			XLocator*			xlocator) const;

	virtual bool
	shouldStripSourceNode(const DOM_Node&	node) const;

	virtual bool
	problem(
			eSource				where,
			eClassification		classification,
			const DOM_Node&		styleNode,
			const DOM_Node&		sourceNode,
			const DOMString&	msg,
			int					lineNo,
			int					charOffset) const;

	virtual bool
	problem(
			eSource					where,
			eClassification			classification,
			const PrefixResolver*	resolver,
			const DOM_Node&			sourceNode,
			const DOMString&		msg,
			int						lineNo,
			int						charOffset) const;

	virtual XPathEnvSupport*
	GetExtendedEnvSupport() const;

	virtual XPathEnvSupport*
	SetExtendedEnvSupport(XPathEnvSupport*	theExtendedSupport);

	// These interfaces are inherited from Resettable...

	virtual void
	reset();

private:

	// These are not implemented...
	XPathEnvSupportDefault(const XPathEnvSupportDefault&);

	XPathEnvSupportDefault&
	operator=(const XPathEnvSupportDefault&);

	bool
	operator==(const XPathEnvSupportDefault&) const;

	// Data members...

	XPathEnvSupport*		m_extendedSupport;

	// Table for storing source tree documents, which are keyed by
	// URL.
	SourceDocsTableType		m_sourceDocs;
};



#endif	// XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680
