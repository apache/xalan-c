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
#if !defined(STYLESHEETCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680)
#define STYLESHEETCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



// Base class header file...
#include <PlatformSupport/ExecutionContext.hpp>



#include <XalanDOM/XalanDOMString.hpp>



class PrefixResolver;
class StylesheetRoot;
class XMLURL;
class XPath;
class XSLTInputSource;



//
// An abstract class which provides support for constructing the internal
// representation  of a stylesheet.
//
class XALAN_XSLT_EXPORT StylesheetConstructionContext : public ExecutionContext
{
public:

	explicit
	StylesheetConstructionContext();

	virtual
	~StylesheetConstructionContext();


	/**
	 * Reset the StylesheetConstructionContext instance.  Any existing objects
	 * created by the instance will be destroyed.
	 */
	virtual void
	reset() = 0;

	/**
	 * Create a new StylesheetRoot instance.  The StylesheetConstructionContext
	 * instance owns the StylesheetRoot instance, and will delete it when asked
	 * or when the StylesheetConstructionContext instance is destroyed.
	 *
	 * @param theBaseIdentifier A URI to the stylesheet file.
	 * @return A pointer to a new StylesheetRoot instance.
	 */
	virtual StylesheetRoot*
	create(const XalanDOMString&	theBaseIdentifier) = 0;

	/**
	 * Create a new StylesheetRoot instance.  The StylesheetConstructionContext
	 * instance owns the StylesheetRoot instance, and will delete it when asked
	 * or when the StylesheetConstructionContext instance is destroyed.
	 *
	 * @param theInputSource A reference to the input source.
	 * @return A pointer to a new StylesheetRoot instance.
	 */
	virtual StylesheetRoot*
	create(XSLTInputSource&		theInputSource) = 0;

	/**
	 * Destroy a StylesheetRoot instance.  If this StylesheetConstructionContext
	 * instance does not own the StylesheetRoot, it will not delete the
	 * StylesheetRoot.
	 *
	 * @param theStylesheet A pointer to the StylesheetRoot instance to delete.
	 */
	virtual void
	destroy(StylesheetRoot*		theStylesheetRoot) = 0;

	/**
	 * Given an XSL tag name, return an integer token that corresponds to
	 * ELEMNAME_XXX constants defined in Constants.hpp.
	 *
	 * @param name a probable xsl:xxx element name
	 * @return Constants.ELEMNAME_XXX token, or -1 if in xsl or Xalan namespace,
	 * -2 if not in known namespace
	 */
	virtual int
	getAttrTok(const XalanDOMString&	name) const = 0;

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @return pointer to fully qualified URI
	 */
	virtual XMLURL*
	getURLFromString(const XalanDOMString&	urlString) = 0;

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param base base location for URI
	 * @return pointer to fully qualified URI
	 */
	virtual XMLURL*
	getURLFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) = 0;

	/**
	 * Retrieve the prefix for XSL namespace URI's, for example,
	 * "http://www.w3.org/1999/XSL/Transform" 
	 * 
	 * @return prefix string
	 */
	virtual const XalanDOMString&
	getXSLNameSpaceURLPre() const = 0;

	/**
	 * Retrieve the URI for the current XSL namespace, for example,
	 * "http://www.w3.org/1999/XSL/Transform/1.0" 
	 * 
	 * @return URI string
	 */
	virtual const XalanDOMString&
	getXSLNameSpaceURL() const = 0;

	/**
	 * Create and initialize an xpath for a match pattern and return it. This
	 * is to be used by stylesheet elements that need an XPath that is
	 * guaranteed to persist while it lives.
	 *
	 * @param str      string to match
	 * @param resolver resolver for namespace resolution
	 * @return XPath for match pattern
	 */
	virtual XPath*
	createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver) = 0;

	/**
	 * Create and initialize an xpath and return it. This is to be used by
	 * stylesheet elements that need an XPath that is guaranteed to persist
	 * while it lives.
	 *
	 * @param str      string to match
	 * @param resolver resolver for namespace resolution
	 * @return XPath for string matched
	 */
	virtual XPath*
	createXPath(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver) = 0;

	// These interfaces are inherited from ExecutionContext...

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const = 0;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const = 0;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const = 0;

};



#endif	// STYLESHEETCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680
