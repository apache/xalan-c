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
#if !defined(XALAN_CONSTANTS_HEADER_GUARD)
#define XALAN_CONSTANTS_HEADER_GUARD

/**
 * $State$
 * 
 * @author Scott Boag (scott_boag@lotus.com)
 * @author David N. Bertoni (dbertoni@lotus.com)
 */
 


// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <dom/DOMString.hpp>



/**
 * Primary constants used in the XSLTProcessor classes.
 */
class XALAN_XSLT_EXPORT Constants
{
public:

	/**
	 * IDs for XSL element types. These are associated 
	 * with the string literals in the XSLTProcessor class.
	 * Don't change the numbers.
	 */
	enum eElementName
	{
		ELEMNAME_UNDEFINED = -1,
		ELEMNAME_WITHPARAM = 2,
		ELEMNAME_ADDATTRIBUTE = 4,
		ELEMNAME_ANCHOR = 22,
		// ELEMNAME_ANCHOR_PATTERN = 23,
		ELEMNAME_APPLY_TEMPLATES = 50,
		ELEMNAME_USE = 34,
		ELEMNAME_CHILDREN = 6,
		ELEMNAME_CHOOSE = 37,
		ELEMNAME_COMMENT = 59, // my own
		ELEMNAME_CONSTRUCT = 7, // my own
		ELEMNAME_CONTENTS = 8,
		ELEMNAME_COPY = 9,
		ELEMNAME_COPY_OF = 74,
		ELEMNAME_DEFINEATTRIBUTESET = 40,
		// ELEMNAME_DEFINECONSTANT = 29,
		// ELEMNAME_DEFINEMACRO = 10,
		ELEMNAME_DEFINESCRIPT = 11,
		ELEMNAME_DISPLAYIF = 12, // my own
		ELEMNAME_EMPTY = 14,
		ELEMNAME_EVAL = 15,
		ELEMNAME_EXPECTEDCHILDREN = 16,
		ELEMNAME_EXTENSION = 54,
		ELEMNAME_EXTENSIONHANDLER = 63,
		ELEMNAME_FOREACH = 28,
		ELEMNAME_KEY = 31,
		ELEMNAME_IF = 36,
		ELEMNAME_IMPORT = 26,
		ELEMNAME_INCLUDE = 27,
		ELEMNAME_CALLTEMPLATE = 17,
		ELEMNAME_PARAMVARIABLE = 41,
		ELEMNAME_NUMBER = 35,
		ELEMNAME_OTHERWISE = 39,
		ELEMNAME_PI = 58,
		ELEMNAME_PRESERVESPACE = 33,
		ELEMNAME_REMOVEATTRIBUTE = 5,
		ELEMNAME_TEMPLATE = 19,
		ELEMNAME_SORT = 64,
		ELEMNAME_STRIPSPACE = 32,
		ELEMNAME_STYLESHEET = 25,
		ELEMNAME_TEXT = 42,
		ELEMNAME_VALUEOF = 30,
		ELEMNAME_WHEN = 38,
  
		// Pattern by example support
		ELEMNAME_ROOT = 44,
		ELEMNAME_ANY = 45,
		ELEMNAME_ELEMENT = 46,
		ELEMNAME_TARGETELEMENT = 47,
		ELEMNAME_ATTRIBUTE = 48,
		ELEMNAME_TARGETATTRIBUTE = 49,
		ELEMNAME_URL = 52, // my own

		ELEMNAME_CALL = 55, // my own
		ELEMNAME_PARAM = 56,
		ELEMNAME_FALLBACK = 57, // my own

		ELEMNAME_TARGETPI = 60, // my own
		ELEMNAME_TARGETCOMMENT = 61, // my own
		ELEMNAME_TARGETTEXT = 62, // my own

		ELEMNAME_CSSSTYLECONVERSION = 65, // my own

		ELEMNAME_COUNTER = 66,
		ELEMNAME_COUNTERS = 67,
		ELEMNAME_COUNTERINCREMENT = 68,
		ELEMNAME_COUNTERRESET = 69,
		ELEMNAME_COUNTERSCOPE = 71,
		ELEMNAME_APPLY_IMPORTS = 72,

		ELEMNAME_VARIABLE = 73,
		ELEMNAME_MESSAGE = 75,
		ELEMNAME_LOCALE = 76,

		ELEMNAME_LITERALRESULT = 77,
		ELEMNAME_TEXTLITERALRESULT = 78,

		ELEMNAME_EXTENSIONCALL = 79,

		ELEMNAME_OUTPUT = 80,
		ELEMNAME_COMPONENT = 81,
		ELEMNAME_SCRIPT = 82
  
	  // Next free number: 83
	};

	/**
	 * Literals for XSL element names.  Note that there are more
	 * names than IDs, because some names map to the same ID.
	 */

	// Result tree counting
	static const DOMString	ELEMNAME_ANCHOR_STRING;
	static const DOMString	ELEMNAME_ANY_STRING; // pattern-by-example support
	static const DOMString	ELEMNAME_APPLY_IMPORTS_STRING;
	static const DOMString	ELEMNAME_APPLY_TEMPLATES_STRING;
	static const DOMString	ELEMNAME_ARG_STRING;
	static const DOMString	ELEMNAME_ATTRIBUTESET_STRING;
	static const DOMString	ELEMNAME_ATTRIBUTE_STRING; // pattern-by-example support
	static const DOMString	ELEMNAME_CALLTEMPLATEARG_STRING;
	static const DOMString	ELEMNAME_CALLTEMPLATE_STRING;
	static const DOMString	ELEMNAME_CALL_STRING;
	static const DOMString	ELEMNAME_CHILDREN_STRING;
	static const DOMString	ELEMNAME_CHOOSE_STRING;
	static const DOMString	ELEMNAME_COMMENT_STRING;
	static const DOMString	ELEMNAME_CONSTRUCT_STRING; // my own
	static const DOMString	ELEMNAME_CONTENTS_STRING;
	static const DOMString	ELEMNAME_COPY_OF_STRING;
	static const DOMString	ELEMNAME_COPY_STRING;
	static const DOMString	ELEMNAME_COUNTERINCREMENT_STRING;
	static const DOMString	ELEMNAME_COUNTERRESET_STRING;
	static const DOMString	ELEMNAME_COUNTERSCOPE_STRING;
	static const DOMString	ELEMNAME_COUNTERS_STRING;
	static const DOMString	ELEMNAME_COUNTER_STRING;
	static const DOMString	ELEMNAME_DISPLAYIF_STRING; // my own
	static const DOMString	ELEMNAME_ELEMENT_STRING;  // pattern-by-example support
	static const DOMString	ELEMNAME_EMPTY_STRING;
	static const DOMString	ELEMNAME_EVAL_STRING;
	static const DOMString	ELEMNAME_EXPECTEDCHILDREN_STRING;
	static const DOMString	ELEMNAME_EXTENSIONHANDLER_STRING;
	static const DOMString	ELEMNAME_EXTENSION_STRING;
	static const DOMString	ELEMNAME_FALLBACK_STRING;
	static const DOMString	ELEMNAME_FOREACH_STRING;
	static const DOMString	ELEMNAME_IF_STRING;
	static const DOMString	ELEMNAME_IMPORT_STRING;
	static const DOMString	ELEMNAME_INCLUDE_STRING;
	static const DOMString	ELEMNAME_KEY_STRING;
	static const DOMString	ELEMNAME_LOCALE_STRING;
	static const DOMString	ELEMNAME_MESSAGE_STRING;
	static const DOMString	ELEMNAME_NUMBER_STRING;
	static const DOMString	ELEMNAME_OTHERWISE_STRING;
	static const DOMString	ELEMNAME_PARAM_STRING;
	static const DOMString	ELEMNAME_PRESERVESPACE_STRING;
	static const DOMString	ELEMNAME_ROOT_STRING; // pattern-by-example support
	static const DOMString	ELEMNAME_SORT_STRING;
	static const DOMString	ELEMNAME_STRIPSPACE_STRING;
	static const DOMString	ELEMNAME_STYLESHEET_STRING;
	static const DOMString	ELEMNAME_TARGETATTRIBUTE_STRING; // pattern-by-example support
	static const DOMString	ELEMNAME_TARGETCOMMENT_STRING;
	static const DOMString	ELEMNAME_TARGETELEMENT_STRING; // pattern-by-example support
	static const DOMString	ELEMNAME_TARGETPI_STRING;
	static const DOMString	ELEMNAME_TARGETTEXT_STRING;
	static const DOMString	ELEMNAME_TEMPLATE_STRING;
	static const DOMString	ELEMNAME_TEXT_STRING;
	static const DOMString	ELEMNAME_TRANSFORM_STRING;
	static const DOMString	ELEMNAME_URL_STRING; // pattern-by-example support
	static const DOMString	ELEMNAME_USE_STRING;
	static const DOMString	ELEMNAME_VALUEOF_STRING;
	static const DOMString	ELEMNAME_VARIABLE_STRING;
	static const DOMString	ELEMNAME_WHEN_STRING;
	static const DOMString	ELEMNAME_COMPONENT_STRING;
	static const DOMString	ELEMNAME_CSSSTYLECONVERSION_STRING;
	static const DOMString	ELEMNAME_OUTPUT_STRING;
	static const DOMString	ELEMNAME_PARAMVARIABLE_OLD_STRING;
	static const DOMString	ELEMNAME_PARAMVARIABLE_STRING;
	static const DOMString	ELEMNAME_PI_OLD_STRING;
	static const DOMString	ELEMNAME_PI_STRING;
	static const DOMString	ELEMNAME_SCRIPT_STRING;
	static const DOMString	ELEMNAME_WITHPARAM_STRING;
  
  
	/*---------------------------------------------
	 * Literals for XSL attribute names.
	 */
	static const DOMString	ATTRNAME_CLASS;
	static const DOMString	ATTRNAME_ATTRIBUTE;
	static const DOMString	ATTRNAME_ATTRIBUTE_SET;
	static const DOMString	ATTRNAME_AMOUNT;
	static const DOMString	ATTRNAME_ANCESTOR;
	static const DOMString	ATTRNAME_CASEORDER;
	static const DOMString	ATTRNAME_CONDITION;
	static const DOMString	ATTRNAME_COPYTYPE;
	static const DOMString	ATTRNAME_COUNT;
	static const DOMString	ATTRNAME_DATATYPE;
	static const DOMString	ATTRNAME_DEFAULTSPACE;
	static const DOMString	ATTRNAME_DEFAULT;
	static const DOMString	ATTRNAME_DEPTH;
	static const DOMString	ATTRNAME_DIGITGROUPSEP;
	static const DOMString	ATTRNAME_ELEMENT;
	static const DOMString	ATTRNAME_ELEMENTS;
	static const DOMString	ATTRNAME_EXPR;
	static const DOMString	ATTRNAME_EXTENSIONELEMENTPREFIXES;
	static const DOMString	ATTRNAME_FORMAT;
	static const DOMString	ATTRNAME_FROM;
	static const DOMString	ATTRNAME_GROUPINGSEPARATOR;
	static const DOMString	ATTRNAME_GROUPINGSIZE;
	static const DOMString	ATTRNAME_HREF;
	static const DOMString	ATTRNAME_ID;
	static const DOMString	ATTRNAME_IMPORTANCE;
	static const DOMString	ATTRNAME_INDENTRESULT;
	static const DOMString	ATTRNAME_LETTERVALUE;
	static const DOMString	ATTRNAME_LEVEL;
	static const DOMString	ATTRNAME_LANG;
	// static const DOMString	ATTRNAME_MACRO;
	static const DOMString	ATTRNAME_MATCH;
	static const DOMString	ATTRNAME_MODE;
	static const DOMString	ATTRNAME_NAME;
	static const DOMString	ATTRNAME_NAMESPACE;
	static const DOMString	ATTRNAME_NDIGITSPERGROUP;
	static const DOMString	ATTRNAME_ONLY;
	static const DOMString	ATTRNAME_ORDER;
	static const DOMString	ATTRNAME_PRIORITY;
	static const DOMString	ATTRNAME_REFID;
	static const DOMString	ATTRNAME_RESULTNS;
	static const DOMString	ATTRNAME_SELECT;
	static const DOMString	ATTRNAME_SEQUENCESRC;
	static const DOMString	ATTRNAME_STYLE;
	static const DOMString	ATTRNAME_TEST;
	static const DOMString	ATTRNAME_TOSTRING;
	static const DOMString	ATTRNAME_TYPE;
	static const DOMString	ATTRNAME_USE;
	static const DOMString	ATTRNAME_USEATTRIBUTESETS;
	static const DOMString	ATTRNAME_VALUE;
	static const DOMString	ATTRNAME_XMLNSDEF;
	static const DOMString	ATTRNAME_XMLNS;
	static const DOMString	ATTRNAME_XMLSPACE;
 
	  // Atributes on the functions element
	static const DOMString	ATTRNAME_NS;
	static const DOMString	ATTRNAME_CLASSID;
	static const DOMString	ATTRNAME_ARCHIVE;
	static const DOMString	ATTRNAME_CODETYPE;
	static const DOMString	ATTRNAME_CODEBASE;
	static const DOMString	ATTRNAME_METHOD;
  
	  // For space-att
	static const DOMString	ATTRVAL_PRESERVE;
	static const DOMString	ATTRVAL_STRIP;
  
	  // For indent-result
	static const DOMString	ATTRVAL_YES;
	static const DOMString	ATTRVAL_NO;
  
	  // For letter-value attribute (part of conversion attributes).
	static const DOMString	ATTRVAL_ALPHABETIC;
	static const DOMString	ATTRVAL_OTHER;
  
	  // For level attribute in xsl:number.
	static const DOMString	ATTRVAL_SINGLE;
	static const DOMString	ATTRVAL_MULTI;
	static const DOMString	ATTRVAL_ANY;

	// Output stuff
	static const DOMString	ATTRNAME_OUTPUT_METHOD;
	static const DOMString	ATTRNAME_DISABLE_OUTPUT_ESCAPING;
	static const DOMString	ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS;
	static const DOMString	ATTRNAME_OUTPUT_DOCTYPE_PUBLIC;
	static const DOMString	ATTRNAME_OUTPUT_DOCTYPE_SYSTEM;
	static const DOMString	ATTRNAME_OUTPUT_ENCODING;
	static const DOMString	ATTRNAME_OUTPUT_INDENT;
	static const DOMString	ATTRNAME_OUTPUT_MEDIATYPE;
	static const DOMString	ATTRNAME_OUTPUT_STANDALONE;
	static const DOMString	ATTRNAME_OUTPUT_VERSION;
//	static const DOMString	ATTRNAME_OUTPUT_XMLDECL;
   static const DOMString	ATTRNAME_OUTPUT_OMITXMLDECL;

	static const DOMString	ATTRVAL_OUTPUT_METHOD_HTML;
	static const DOMString	ATTRVAL_OUTPUT_METHOD_XML;
	static const DOMString	ATTRVAL_OUTPUT_METHOD_TEXT;

	// some stuff for my patterns-by-example
	static const DOMString	ATTRVAL_THIS;
	static const DOMString	ATTRVAL_PARENT;
	static const DOMString	ATTRVAL_ANCESTOR;
	static const DOMString	ATTRVAL_ID;

  // Stuff for sorting
	static const DOMString	ATTRVAL_DATATYPE_TEXT;
	static const DOMString	ATTRVAL_DATATYPE_NUMBER;
	static const DOMString	ATTRVAL_ORDER_ASCENDING;
	static const DOMString	ATTRVAL_ORDER_DESCENDING;
	static const DOMString	ATTRVAL_CASEORDER_UPPER;
	static const DOMString	ATTRVAL_CASEORDER_LOWER;

	enum eNumberLevel
	{
		/*
		 * Integer equivalents for above
		 */
		NUMBERLEVEL_SINGLE = 1,
		NUMBERLEVEL_MULTI = 2,
		NUMBERLEVEL_ANY = 3,

		MAX_MULTI_COUNTING_DEPTH = 32
	};

	enum eAttributeName
	{
		TATTRNAME_OUTPUT_METHOD = 1,
		TATTRNAME_AMOUNT = 2,
		TATTRNAME_ANCESTOR = 3,
		TATTRNAME_ARCHIVE = 4,
		TATTRNAME_ATTRIBUTE = 5,
		TATTRNAME_ATTRIBUTE_SET = 6,
		TATTRNAME_CASEORDER = 7,
		TATTRNAME_CLASS = 8,
		TATTRNAME_CLASSID = 9,
		TATTRNAME_CODEBASE = 10,
		TATTRNAME_CODETYPE = 11,
		TATTRNAME_CONDITION = 12,
		TATTRNAME_COPYTYPE = 13,
		TATTRNAME_COUNT = 14,
		TATTRNAME_DATATYPE = 15,
		TATTRNAME_DEFAULT = 16,
		TATTRNAME_DEFAULTSPACE = 17,
		TATTRNAME_DEPTH = 18,
		TATTRNAME_DIGITGROUPSEP = 19,
		TATTRNAME_DISABLE_OUTPUT_ESCAPING= 20,
		TATTRNAME_ELEMENT = 21,
		TATTRNAME_ELEMENTS = 22,
		TATTRNAME_EXPR = 23,
		TATTRNAME_EXTENSIONELEMENTPREFIXES = 24,
		TATTRNAME_FORMAT = 25,
		TATTRNAME_FROM = 26,
		TATTRNAME_GROUPINGSEPARATOR = 27,
		TATTRNAME_GROUPINGSIZE = 28,
		TATTRNAME_HREF = 29,
		TATTRNAME_ID = 30,
		TATTRNAME_IMPORTANCE = 31,
		TATTRNAME_INDENTRESULT = 32,
		TATTRNAME_LANG = 33,
		TATTRNAME_LETTERVALUE = 34,
		TATTRNAME_LEVEL = 35,
		TATTRNAME_MATCH = 36,
		TATTRNAME_METHOD = 37,
		TATTRNAME_MODE = 38,
		TATTRNAME_NAME = 39,
		TATTRNAME_NAMESPACE = 40,
		TATTRNAME_NDIGITSPERGROUP = 41,
		TATTRNAME_NS = 42,
		TATTRNAME_ONLY = 43,
		TATTRNAME_ORDER = 44,
		TATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS= 45,
		TATTRNAME_OUTPUT_DOCTYPE_PUBLIC = 46,
		TATTRNAME_OUTPUT_DOCTYPE_SYSTEM = 47,
		TATTRNAME_OUTPUT_ENCODING = 48,
		TATTRNAME_OUTPUT_INDENT = 49,
		TATTRNAME_OUTPUT_MEDIATYPE = 50,
		TATTRNAME_OUTPUT_STANDALONE= 51,
		TATTRNAME_OUTPUT_VERSION = 52,
		TATTRNAME_OUTPUT_OMITXMLDECL = 53,
		// TATTRNAME_OUTPUT_XMLDECL = 53,
		TATTRNAME_PRIORITY = 54,
		TATTRNAME_REFID = 55,
		TATTRNAME_RESULTNS = 56,
		TATTRNAME_SELECT = 57,
		TATTRNAME_SEQUENCESRC = 58,
		TATTRNAME_STYLE = 59,
		TATTRNAME_TEST = 60,
		TATTRNAME_TOSTRING = 61,
		TATTRNAME_TYPE = 62,
		TATTRNAME_USE = 63,
		TATTRNAME_USEATTRIBUTESETS = 64,
		TATTRNAME_VALUE = 65,
		TATTRNAME_XMLNSDEF = 66,
		TATTRNAME_XMLNS = 67,
		TATTRNAME_XMLSPACE = 68
	};

  // These are used mainly for keys in the pattern lookup table,
  // for those nodes that don't have unique lookup values
	static const DOMString	PSEUDONAME_ANY;
	static const DOMString	PSEUDONAME_ROOT;
	static const DOMString	PSEUDONAME_TEXT;
	static const DOMString	PSEUDONAME_COMMENT;
	static const DOMString	PSEUDONAME_PI;
	static const DOMString	PSEUDONAME_OTHER;

};


#endif	// XALAN_CONSTANTS_HEADER_GUARD
