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
namespace Constants
{
	/**
	 * IDs for XSL element types. These are associated 
	 * with the string literals in the XSLTProcessor class.
	 * Don't change the numbers.
	 */
	static const int ELEMNAME_UNDEFINED = -1;
	static const int ELEMNAME_WITHPARAM = 2;
	static const int ELEMNAME_ADDATTRIBUTE = 4;
	static const int ELEMNAME_ANCHOR = 22;
	// static const int ELEMNAME_ANCHOR_PATTERN = 23;
	static const int ELEMNAME_APPLY_TEMPLATES = 50;
	static const int ELEMNAME_USE = 34;
	static const int ELEMNAME_CHILDREN = 6;
	static const int ELEMNAME_CHOOSE = 37;
	static const int ELEMNAME_COMMENT = 59; // my own
	static const int ELEMNAME_CONSTRUCT = 7; // my own
	static const int ELEMNAME_CONTENTS = 8;
	static const int ELEMNAME_COPY = 9;
	static const int ELEMNAME_COPY_OF = 74;
	static const int ELEMNAME_DEFINEATTRIBUTESET = 40;
	// static const int ELEMNAME_DEFINECONSTANT = 29;
	// static const int ELEMNAME_DEFINEMACRO = 10;
	static const int ELEMNAME_DEFINESCRIPT = 11;
	static const int ELEMNAME_DISPLAYIF = 12; // my own
	static const int ELEMNAME_EMPTY = 14;
	static const int ELEMNAME_EVAL = 15;
	static const int ELEMNAME_EXPECTEDCHILDREN = 16;
	static const int ELEMNAME_EXTENSION = 54;
	static const int ELEMNAME_EXTENSIONHANDLER = 63;
	static const int ELEMNAME_FOREACH = 28;
	static const int ELEMNAME_KEY = 31;
	static const int ELEMNAME_IF = 36;
	static const int ELEMNAME_IMPORT = 26;
	static const int ELEMNAME_INCLUDE = 27;
	static const int ELEMNAME_CALLTEMPLATE = 17;
	static const int ELEMNAME_PARAMVARIABLE = 41;
	static const int ELEMNAME_NUMBER = 35;
	static const int ELEMNAME_OTHERWISE = 39;
	static const int ELEMNAME_PI = 58;
	static const int ELEMNAME_PRESERVESPACE = 33;
	static const int ELEMNAME_REMOVEATTRIBUTE = 5;
	static const int ELEMNAME_TEMPLATE = 19;
	static const int ELEMNAME_SORT = 64;
	static const int ELEMNAME_STRIPSPACE = 32;
	static const int ELEMNAME_STYLESHEET = 25;
	static const int ELEMNAME_TEXT = 42;
	static const int ELEMNAME_VALUEOF = 30;
	static const int ELEMNAME_WHEN = 38;
  
	// Pattern by example support
	static const int ELEMNAME_ROOT = 44;
	static const int ELEMNAME_ANY = 45;
	static const int ELEMNAME_ELEMENT = 46;
	static const int ELEMNAME_TARGETELEMENT = 47;
	static const int ELEMNAME_ATTRIBUTE = 48;
	static const int ELEMNAME_TARGETATTRIBUTE = 49;
	static const int ELEMNAME_URL = 52; // my own

	static const int ELEMNAME_CALL = 55; // my own
	static const int ELEMNAME_PARAM = 56;
	static const int ELEMNAME_FALLBACK = 57; // my own

	static const int ELEMNAME_TARGETPI = 60; // my own
	static const int ELEMNAME_TARGETCOMMENT = 61; // my own
	static const int ELEMNAME_TARGETTEXT = 62; // my own

	static const int ELEMNAME_CSSSTYLECONVERSION = 65; // my own

	static const int ELEMNAME_COUNTER = 66;
	static const int ELEMNAME_COUNTERS = 67;
	static const int ELEMNAME_COUNTERINCREMENT = 68;
	static const int ELEMNAME_COUNTERRESET = 69;
	static const int ELEMNAME_COUNTERSCOPE = 71;
	static const int ELEMNAME_APPLY_IMPORTS = 72;

	static const int ELEMNAME_VARIABLE = 73;
	static const int ELEMNAME_MESSAGE = 75;
	static const int ELEMNAME_LOCALE = 76;

	static const int ELEMNAME_LITERALRESULT = 77;
	static const int ELEMNAME_TEXTLITERALRESULT = 78;

	static const int ELEMNAME_EXTENSIONCALL = 79;

	static const int ELEMNAME_OUTPUT = 80;
	static const int ELEMNAME_COMPONENT = 81;
	static const int ELEMNAME_SCRIPT = 82;
  
  // Next free number: 83


	/**
	 * Literals for XSL element names.  Note that there are more
	 * names than IDs, because some names map to the same ID.
	 */


	// Result tree counting
	static const DOMString ELEMNAME_ANCHOR_STRING = "anchor";
	static const DOMString ELEMNAME_ANY_STRING = "any"; // pattern-by-example support
	static const DOMString ELEMNAME_APPLY_IMPORTS_STRING = "apply-imports";
	static const DOMString ELEMNAME_APPLY_TEMPLATES_STRING = "apply-templates";
	static const DOMString ELEMNAME_ARG_STRING = "arg";
	static const DOMString ELEMNAME_ATTRIBUTESET_STRING = "attribute-set";
	static const DOMString ELEMNAME_ATTRIBUTE_STRING = "attribute"; // pattern-by-example support
	static const DOMString ELEMNAME_CALLTEMPLATEARG_STRING = "invoke-arg";
	static const DOMString ELEMNAME_CALLTEMPLATE_STRING = "call-template";
	static const DOMString ELEMNAME_CALL_STRING = "call";
	static const DOMString ELEMNAME_CHILDREN_STRING = "children";
	static const DOMString ELEMNAME_CHOOSE_STRING = "choose";
	static const DOMString ELEMNAME_COMMENT_STRING = "comment";
	static const DOMString ELEMNAME_CONSTRUCT_STRING = "construct"; // my own
	static const DOMString ELEMNAME_CONTENTS_STRING = "contents";
	static const DOMString ELEMNAME_COPY_OF_STRING = "copy-of";
	static const DOMString ELEMNAME_COPY_STRING = "copy";
	static const DOMString ELEMNAME_COUNTERINCREMENT_STRING = "counter-increment";
	static const DOMString ELEMNAME_COUNTERRESET_STRING = "counter-reset";
	static const DOMString ELEMNAME_COUNTERSCOPE_STRING = "counter-scope";
	static const DOMString ELEMNAME_COUNTERS_STRING = "counters";
	static const DOMString ELEMNAME_COUNTER_STRING = "counter";
	static const DOMString ELEMNAME_DISPLAYIF_STRING = "display-if"; // my own
	static const DOMString ELEMNAME_ELEMENT_STRING = "element"; // pattern-by-example support
	static const DOMString ELEMNAME_EMPTY_STRING = "empty";
	static const DOMString ELEMNAME_EVAL_STRING = "eval";
	static const DOMString ELEMNAME_EXPECTEDCHILDREN_STRING = "expectedchildren";
	static const DOMString ELEMNAME_EXTENSIONHANDLER_STRING = "code-dispatcher";
	static const DOMString ELEMNAME_EXTENSION_STRING = "functions";
	static const DOMString ELEMNAME_FALLBACK_STRING = "fallback";
	static const DOMString ELEMNAME_FOREACH_STRING = "for-each";
	static const DOMString ELEMNAME_IF_STRING = "if";
	static const DOMString ELEMNAME_IMPORT_STRING = "import";
	static const DOMString ELEMNAME_INCLUDE_STRING = "include";
	static const DOMString ELEMNAME_KEY_STRING = "key";
	static const DOMString ELEMNAME_LOCALE_STRING = "locale";
	static const DOMString ELEMNAME_MESSAGE_STRING = "message";
	static const DOMString ELEMNAME_NUMBER_STRING = "number";
	static const DOMString ELEMNAME_OTHERWISE_STRING = "otherwise";
	static const DOMString ELEMNAME_PARAM_STRING = "param";
	static const DOMString ELEMNAME_PRESERVESPACE_STRING = "preserve-space";
	static const DOMString ELEMNAME_ROOT_STRING = "root"; // pattern-by-example support
	static const DOMString ELEMNAME_SORT_STRING = "sort";
	static const DOMString ELEMNAME_STRIPSPACE_STRING = "strip-space";
	static const DOMString ELEMNAME_STYLESHEET_STRING = "stylesheet";
	static const DOMString ELEMNAME_TARGETATTRIBUTE_STRING = "target-attribute"; // pattern-by-example support
	static const DOMString ELEMNAME_TARGETCOMMENT_STRING = "target-comment";
	static const DOMString ELEMNAME_TARGETELEMENT_STRING = "target-element"; // pattern-by-example support
	static const DOMString ELEMNAME_TARGETPI_STRING = "target-pi";
	static const DOMString ELEMNAME_TARGETTEXT_STRING = "target-text";
	static const DOMString ELEMNAME_TEMPLATE_STRING = "template";
	static const DOMString ELEMNAME_TEXT_STRING = "text";
	static const DOMString ELEMNAME_TRANSFORM_STRING = "transform";
	static const DOMString ELEMNAME_URL_STRING = "uri"; // pattern-by-example support
	static const DOMString ELEMNAME_USE_STRING = "use";
	static const DOMString ELEMNAME_VALUEOF_STRING = "value-of";
	static const DOMString ELEMNAME_VARIABLE_STRING = "variable";
	static const DOMString ELEMNAME_WHEN_STRING = "when";
	static const DOMString ELEMNAME_COMPONENT_STRING = "component";
	static const DOMString ELEMNAME_CSSSTYLECONVERSION_STRING = "css-style-conversion";
	static const DOMString ELEMNAME_OUTPUT_STRING = "output";
	static const DOMString ELEMNAME_PARAMVARIABLE_OLD_STRING = "param-variable";
	static const DOMString ELEMNAME_PARAMVARIABLE_STRING = "param";
	static const DOMString ELEMNAME_PI_OLD_STRING = "pi";
	static const DOMString ELEMNAME_PI_STRING = "processing-instruction";
	static const DOMString ELEMNAME_SCRIPT_STRING = "script";
	static const DOMString ELEMNAME_WITHPARAM_STRING = "with-param";
  
  

	/*---------------------------------------------
	 * Literals for XSL attribute names.
	 */
	static const DOMString ATTRNAME_CLASS = "class";
	static const DOMString ATTRNAME_ATTRIBUTE = "attribute";
	static const DOMString ATTRNAME_ATTRIBUTE_SET = "attribute-set";
	static const DOMString ATTRNAME_AMOUNT = "amount";
	static const DOMString ATTRNAME_ANCESTOR = "ancestor";
	static const DOMString ATTRNAME_CASEORDER = "case-order";
	static const DOMString ATTRNAME_CONDITION = "condition";
	static const DOMString ATTRNAME_COPYTYPE = "copy-type";
	static const DOMString ATTRNAME_COUNT = "count";
	static const DOMString ATTRNAME_DATATYPE = "data-type";
	static const DOMString ATTRNAME_DEFAULTSPACE = "default-space";
	static const DOMString ATTRNAME_DEFAULT = "default";
	static const DOMString ATTRNAME_DEPTH = "with-children";
	static const DOMString ATTRNAME_DIGITGROUPSEP = "digit-group-sep";
	static const DOMString ATTRNAME_ELEMENT = "element";
	static const DOMString ATTRNAME_ELEMENTS = "elements";
	static const DOMString ATTRNAME_EXPR = "expr";
	static const DOMString ATTRNAME_EXTENSIONELEMENTPREFIXES = "extension-element-prefixes";
	static const DOMString ATTRNAME_FORMAT = "format";
	static const DOMString ATTRNAME_FROM = "from";
	static const DOMString ATTRNAME_GROUPINGSEPARATOR = "grouping-separator";
	static const DOMString ATTRNAME_GROUPINGSIZE = "grouping-size";
	static const DOMString ATTRNAME_HREF = "href";
	static const DOMString ATTRNAME_ID = "id";
	static const DOMString ATTRNAME_IMPORTANCE = "importance";
	static const DOMString ATTRNAME_INDENTRESULT = "indent-result";
	static const DOMString ATTRNAME_LETTERVALUE = "letter-value";
	static const DOMString ATTRNAME_LEVEL = "level";
	static const DOMString ATTRNAME_LANG = "lang";
	// static const DOMString ATTRNAME_MACRO = "macro";
	static const DOMString ATTRNAME_MATCH = "match";
	static const DOMString ATTRNAME_MODE = "mode";
	static const DOMString ATTRNAME_NAME = "name";
	static const DOMString ATTRNAME_NAMESPACE = "namespace";
	static const DOMString ATTRNAME_NDIGITSPERGROUP = "n-digits-per-group";
	static const DOMString ATTRNAME_ONLY = "only";
	static const DOMString ATTRNAME_ORDER = "order";
	static const DOMString ATTRNAME_PRIORITY = "priority";
	static const DOMString ATTRNAME_REFID = "refID";
	static const DOMString ATTRNAME_RESULTNS = "result-ns";
	static const DOMString ATTRNAME_SELECT = "select";
	static const DOMString ATTRNAME_SEQUENCESRC = "sequence-src";
	static const DOMString ATTRNAME_STYLE = "style";
	static const DOMString ATTRNAME_TEST = "test";
	static const DOMString ATTRNAME_TOSTRING = "to-string";
	static const DOMString ATTRNAME_TYPE = "type";
	static const DOMString ATTRNAME_USE = "use";
	static const DOMString ATTRNAME_USEATTRIBUTESETS = "use-attribute-sets";
	static const DOMString ATTRNAME_VALUE = "value";
	static const DOMString ATTRNAME_XMLNSDEF = "xmlns";
	static const DOMString ATTRNAME_XMLNS = "xmlns:";
	static const DOMString ATTRNAME_XMLSPACE = "xml:space";
  
	  // Atributes on the functions element
	static const DOMString ATTRNAME_NS = "ns";
	static const DOMString ATTRNAME_CLASSID = "classid";
	static const DOMString ATTRNAME_ARCHIVE = "archive";
	static const DOMString ATTRNAME_CODETYPE = "type";
	static const DOMString ATTRNAME_CODEBASE = "codebase";
	static const DOMString ATTRNAME_METHOD = "calls";
  
	  // For space-att
	static const DOMString ATTRVAL_PRESERVE = "preserve";
	static const DOMString ATTRVAL_STRIP = "strip";
  
	  // For indent-result
	static const DOMString ATTRVAL_YES = "yes";
	static const DOMString ATTRVAL_NO = "no";
  
	  // For letter-value attribute (part of conversion attributes).
	static const DOMString ATTRVAL_ALPHABETIC = "alphabetic";
	static const DOMString ATTRVAL_OTHER = "other";
  
	  // For level attribute in xsl:number.
	static const DOMString ATTRVAL_SINGLE = "single";
	static const DOMString ATTRVAL_MULTI = "multiple";
	static const DOMString ATTRVAL_ANY = "any";

	// Output stuff -- JMD
	static const DOMString ATTRNAME_OUTPUT_METHOD = "method"; // qname, 
	static const DOMString ATTRNAME_DISABLE_OUTPUT_ESCAPING  = "disable-output-escaping";
	static const DOMString ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS  = "cdata-section-elements";
	static const DOMString ATTRNAME_OUTPUT_DOCTYPE_PUBLIC = "doctype-public";
	static const DOMString ATTRNAME_OUTPUT_DOCTYPE_SYSTEM = "doctype-system";
	static const DOMString ATTRNAME_OUTPUT_ENCODING = "encoding";
	static const DOMString ATTRNAME_OUTPUT_INDENT = "indent";
	static const DOMString ATTRNAME_OUTPUT_MEDIATYPE = "media-type";
	static const DOMString ATTRNAME_OUTPUT_STANDALONE  = "standalone";
	static const DOMString ATTRNAME_OUTPUT_VERSION = "version";
	static const DOMString ATTRNAME_OUTPUT_XMLDECL = "xml-declaration";
	static const DOMString ATTRVAL_OUTPUT_METHOD_HTML = "html";
	static const DOMString ATTRVAL_OUTPUT_METHOD_XML = "xml";
	static const DOMString ATTRVAL_OUTPUT_METHOD_TEXT = "text";

	/*
	 * Integer equivalents for above
	 */
	static const int NUMBERLEVEL_SINGLE = 1;
	static const int NUMBERLEVEL_MULTI = 2;
	static const int NUMBERLEVEL_ANY = 3;

	static const int MAX_MULTI_COUNTING_DEPTH = 32;

  // some stuff for my patterns-by-example
	static const DOMString ATTRVAL_THIS = ".";
	static const DOMString ATTRVAL_PARENT = "..";
	static const DOMString ATTRVAL_ANCESTOR = "ancestor";
	static const DOMString ATTRVAL_ID = "id";


	static const int TATTRNAME_OUTPUT_METHOD = 1;
  static const int TATTRNAME_AMOUNT = 2;
  static const int TATTRNAME_ANCESTOR = 3;
  static const int TATTRNAME_ARCHIVE = 4;
  static const int TATTRNAME_ATTRIBUTE = 5;
  static const int TATTRNAME_ATTRIBUTE_SET = 6;
  static const int TATTRNAME_CASEORDER = 7;
  static const int TATTRNAME_CLASS = 8;
  static const int TATTRNAME_CLASSID = 9;
  static const int TATTRNAME_CODEBASE = 10;
  static const int TATTRNAME_CODETYPE = 11;
  static const int TATTRNAME_CONDITION = 12;
  static const int TATTRNAME_COPYTYPE = 13;
  static const int TATTRNAME_COUNT = 14;
  static const int TATTRNAME_DATATYPE = 15;
  static const int TATTRNAME_DEFAULT = 16;
  static const int TATTRNAME_DEFAULTSPACE = 17;
  static const int TATTRNAME_DEPTH = 18;
  static const int TATTRNAME_DIGITGROUPSEP = 19;
  static const int TATTRNAME_DISABLE_OUTPUT_ESCAPING  = 20;
  static const int TATTRNAME_ELEMENT = 21;
  static const int TATTRNAME_ELEMENTS = 22;
  static const int TATTRNAME_EXPR = 23;
  static const int TATTRNAME_EXTENSIONELEMENTPREFIXES = 24;
  static const int TATTRNAME_FORMAT = 25;
  static const int TATTRNAME_FROM = 26;
  static const int TATTRNAME_GROUPINGSEPARATOR = 27;
  static const int TATTRNAME_GROUPINGSIZE = 28;
  static const int TATTRNAME_HREF = 29;
  static const int TATTRNAME_ID = 30;
  static const int TATTRNAME_IMPORTANCE = 31;
  static const int TATTRNAME_INDENTRESULT = 32;
  static const int TATTRNAME_LANG = 33;
  static const int TATTRNAME_LETTERVALUE = 34;
  static const int TATTRNAME_LEVEL = 35;
  static const int TATTRNAME_MATCH = 36;
  static const int TATTRNAME_METHOD = 37;
  static const int TATTRNAME_MODE = 38;
  static const int TATTRNAME_NAME = 39;
  static const int TATTRNAME_NAMESPACE = 40;
  static const int TATTRNAME_NDIGITSPERGROUP = 41;
  static const int TATTRNAME_NS = 42;
  static const int TATTRNAME_ONLY = 43;
  static const int TATTRNAME_ORDER = 44;
  static const int TATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS  = 45;
  static const int TATTRNAME_OUTPUT_DOCTYPE_PUBLIC = 46;
  static const int TATTRNAME_OUTPUT_DOCTYPE_SYSTEM = 47;
  static const int TATTRNAME_OUTPUT_ENCODING = 48;
  static const int TATTRNAME_OUTPUT_INDENT = 49;
  static const int TATTRNAME_OUTPUT_MEDIATYPE = 50;
  static const int TATTRNAME_OUTPUT_STANDALONE  = 51;
  static const int TATTRNAME_OUTPUT_VERSION = 52;
  static const int TATTRNAME_OUTPUT_XMLDECL = 53;
  static const int TATTRNAME_PRIORITY = 54;
  static const int TATTRNAME_REFID = 55;
  static const int TATTRNAME_RESULTNS = 56;
  static const int TATTRNAME_SELECT = 57;
  static const int TATTRNAME_SEQUENCESRC = 58;
  static const int TATTRNAME_STYLE = 59;
  static const int TATTRNAME_TEST = 60;
  static const int TATTRNAME_TOSTRING = 61;
  static const int TATTRNAME_TYPE = 62;
  static const int TATTRNAME_USE = 63;
  static const int TATTRNAME_USEATTRIBUTESETS = 64;
  static const int TATTRNAME_VALUE = 65;
  static const int TATTRNAME_XMLNSDEF = 66;
  static const int TATTRNAME_XMLNS = 67;
  static const int TATTRNAME_XMLSPACE = 68;

  // These are used mainly for keys in the pattern lookup table,
  // for those nodes that don't have unique lookup values
	static const DOMString PSEUDONAME_ANY = "*";
	static const DOMString PSEUDONAME_ROOT = "/";
	static const DOMString PSEUDONAME_TEXT = "#text";
	static const DOMString PSEUDONAME_COMMENT = "#comment";
	static const DOMString PSEUDONAME_PI = "#pi";
	static const DOMString PSEUDONAME_OTHER = "*";

  // Stuff for sorting
	static const DOMString ATTRVAL_DATATYPE_TEXT = "text";
	static const DOMString ATTRVAL_DATATYPE_NUMBER = "number";

	static const DOMString ATTRVAL_ORDER_ASCENDING = "ascending";
	static const DOMString ATTRVAL_ORDER_DESCENDING = "descending";

	static const DOMString ATTRVAL_CASEORDER_UPPER = "upper-first";
	static const DOMString ATTRVAL_CASEORDER_LOWER = "lower-first";

}

#endif	// XALAN_CONSTANTS_HEADER_GUARD
