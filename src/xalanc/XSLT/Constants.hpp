/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#if !defined(XALAN_CONSTANTS_HEADER_GUARD)
#define XALAN_CONSTANTS_HEADER_GUARD



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * Primary constants used in the XSLTProcessor classes.
 */
class XALAN_XSLT_EXPORT Constants
{
public:

	/**
	 * Perform initialization of statics -- must be called before any
	 * processing occurs.  See class XSLTInit.
	 */
	static void
	initialize();

	/**
	 * Perform termination of statics.  See class XSLTInit.
	 */
	static void
	terminate();

	/**
	 * Literals for XSL element names.  Note that there are more
	 * names than IDs, because some names map to the same ID.
	 */
	// Result tree counting
	static const XalanDOMString&	ELEMNAME_ANY_STRING;
	static const XalanDOMString&	ELEMNAME_APPLY_IMPORTS_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_APPLY_TEMPLATES_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_ATTRIBUTE_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_CALLTEMPLATE_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_CHOOSE_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_COMMENT_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_COPY_OF_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_COPY_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_ELEMENT_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_FALLBACK_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_FOREACH_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_HTML_STRING;
	static const XalanDOMString&	ELEMNAME_IF_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_MESSAGE_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_NUMBER_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_OTHERWISE_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_SORT_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_TEMPLATE_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_TEXT_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_VALUEOF_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_VARIABLE_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_WHEN_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_PARAM_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_PI_WITH_PREFIX_STRING;
	static const XalanDOMString&	ELEMNAME_WITHPARAM_WITH_PREFIX_STRING;
  
	/*---------------------------------------------
	 * Literals for XSL attribute names.
	 */
	static const XalanDOMString&	ATTRNAME_CASEORDER;
	static const XalanDOMString&	ATTRNAME_COUNT;
	static const XalanDOMString&	ATTRNAME_DATATYPE;
	static const XalanDOMString&	ATTRNAME_DECIMALSEPARATOR;
	static const XalanDOMString&	ATTRNAME_DEFAULT;
	static const XalanDOMString&	ATTRNAME_DEFAULTSPACE;
	static const XalanDOMString&	ATTRNAME_DIGIT;
	static const XalanDOMString&	ATTRNAME_DIGITGROUPSEP;
	static const XalanDOMString&	ATTRNAME_ELEMENTS;
	static const XalanDOMString&	ATTRNAME_ESCAPE_URLS;
	static const XalanDOMString&	ATTRNAME_EXCLUDE_RESULT_PREFIXES;
	static const XalanDOMString&	ATTRNAME_EXTENSIONELEMENTPREFIXES;
	static const XalanDOMString&	ATTRNAME_FORMAT;
	static const XalanDOMString&	ATTRNAME_FROM;
	static const XalanDOMString&	ATTRNAME_GROUPINGSEPARATOR;
	static const XalanDOMString&	ATTRNAME_GROUPINGSIZE;
	static const XalanDOMString&	ATTRNAME_HREF;
	static const XalanDOMString&	ATTRNAME_ID;
	static const XalanDOMString&	ATTRNAME_INDENTAMOUNT;
	static const XalanDOMString&	ATTRNAME_INDENTRESULT;
	static const XalanDOMString&	ATTRNAME_INFINITY;
	static const XalanDOMString&	ATTRNAME_LANG;
	static const XalanDOMString&	ATTRNAME_LETTERVALUE;
	static const XalanDOMString&	ATTRNAME_MATCH;
	static const XalanDOMString&	ATTRNAME_MINUSSIGN;
	static const XalanDOMString&	ATTRNAME_MODE;
	static const XalanDOMString&	ATTRNAME_NAME;
	static const XalanDOMString&	ATTRNAME_NAMESPACE;
	static const XalanDOMString&	ATTRNAME_NAN;
	static const XalanDOMString&	ATTRNAME_NDIGITSPERGROUP;
	static const XalanDOMString&	ATTRNAME_OMIT_META_TAG;
	static const XalanDOMString&	ATTRNAME_ORDER;
	static const XalanDOMString&	ATTRNAME_PATTERNSEPARATOR;
	static const XalanDOMString&	ATTRNAME_PERCENT;
	static const XalanDOMString&	ATTRNAME_PERMILLE;
	static const XalanDOMString&	ATTRNAME_PREFIX;
	static const XalanDOMString&	ATTRNAME_PRIORITY;
	static const XalanDOMString&	ATTRNAME_RESULTNS;
	static const XalanDOMString&	ATTRNAME_RESULT_PREFIX;
	static const XalanDOMString&	ATTRNAME_SELECT;
	static const XalanDOMString&	ATTRNAME_STYLESHEET_PREFIX;
	static const XalanDOMString&	ATTRNAME_TERMINATE;
	static const XalanDOMString&	ATTRNAME_TEST;
	static const XalanDOMString&	ATTRNAME_TYPE;
	static const XalanDOMString&	ATTRNAME_USE;
	static const XalanDOMString&	ATTRNAME_USEATTRIBUTESETS;
	static const XalanDOMString&	ATTRNAME_VALUE;
	static const XalanDOMString&	ATTRNAME_VERSION;
	static const XalanDOMString&	ATTRNAME_SPACE;
	static const XalanDOMString&	ATTRNAME_ZERODIGIT;
 
	static const XalanDOMString&	ATTRTYPE_CDATA;

	  // For space-att
	static const XalanDOMString&	ATTRVAL_PRESERVE;
	static const XalanDOMString&	ATTRVAL_STRIP;
  
	  // For indent-result
	static const XalanDOMString&	ATTRVAL_YES;
	static const XalanDOMString&	ATTRVAL_NO;
  
	// Output stuff
	static const XalanDOMString&	ATTRNAME_OUTPUT_METHOD;
	static const XalanDOMString&	ATTRNAME_DISABLE_OUTPUT_ESCAPING;
	static const XalanDOMString&	ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS;
	static const XalanDOMString&	ATTRNAME_OUTPUT_DOCTYPE_PUBLIC;
	static const XalanDOMString&	ATTRNAME_OUTPUT_DOCTYPE_SYSTEM;
	static const XalanDOMString&	ATTRNAME_OUTPUT_ENCODING;
	static const XalanDOMString&	ATTRNAME_OUTPUT_INDENT;
	static const XalanDOMString&	ATTRNAME_OUTPUT_MEDIATYPE;
	static const XalanDOMString&	ATTRNAME_OUTPUT_STANDALONE;
	static const XalanDOMString&	ATTRNAME_OUTPUT_VERSION;
	static const XalanDOMString&	ATTRNAME_OUTPUT_OMITXMLDECL;

	static const XalanDOMString&	ATTRVAL_OUTPUT_METHOD_HTML;
	static const XalanDOMString&	ATTRVAL_OUTPUT_METHOD_XML;
	static const XalanDOMString&	ATTRVAL_OUTPUT_METHOD_TEXT;

	static const XalanDOMString&	ATTRVAL_THIS;

  // Stuff for sorting
	static const XalanDOMString&	ATTRVAL_DATATYPE_TEXT;
	static const XalanDOMString&	ATTRVAL_DATATYPE_NUMBER;
	static const XalanDOMString&	ATTRVAL_ORDER_ASCENDING;
	static const XalanDOMString&	ATTRVAL_ORDER_DESCENDING;
	static const XalanDOMString&	ATTRVAL_CASEORDER_UPPER;
	static const XalanDOMString&	ATTRVAL_CASEORDER_LOWER;

	// Stuff for xsl:decimal-format...
	static const XalanDOMString&	ATTRVAL_INFINITY;
	static const XalanDOMString&	ATTRVAL_NAN;

	static const XalanDOMString&	ATTRVAL_SIMPLE;

	// Default whitespace separator string for StringTokenizer.
	static const XalanDOMString&	DEFAULT_WHITESPACE_SEPARATOR_STRING;

	// Value for default text rule
	static const XalanDOMString&	ATTRVAL_DEFAULT_TEXT_RULE;

	static const XalanDOMString&	ATTRVAL_DEFAULT;

	// For Stylesheet-prefix and result-prefix in xsl:namespace-alias 
	static const XalanDOMString&	ATTRVAL_DEFAULT_PREFIX;

	// This is used for trace reporting.
	static const XalanDOMString&	PSEUDONAME_NODE;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_CONSTANTS_HEADER_GUARD
