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

#include "Constants.hpp"



#include <PlatformSupport/DOMStringHelper.hpp>



const XalanDOMString		Constants::ELEMNAME_ANCHOR_STRING(XALAN_STATIC_UCODE_STRING("anchor"));
const XalanDOMString		Constants::ELEMNAME_ANY_STRING(XALAN_STATIC_UCODE_STRING("any"));
const XalanDOMString		Constants::ELEMNAME_APPLY_IMPORTS_STRING(XALAN_STATIC_UCODE_STRING("apply-imports"));
const XalanDOMString		Constants::ELEMNAME_APPLY_TEMPLATES_STRING(XALAN_STATIC_UCODE_STRING("apply-templates"));
const XalanDOMString		Constants::ELEMNAME_ARG_STRING(XALAN_STATIC_UCODE_STRING("arg"));
const XalanDOMString		Constants::ELEMNAME_ATTRIBUTESET_STRING(XALAN_STATIC_UCODE_STRING("attribute-set"));
const XalanDOMString		Constants::ELEMNAME_ATTRIBUTE_STRING(XALAN_STATIC_UCODE_STRING("attribute"));
const XalanDOMString		Constants::ELEMNAME_CALLTEMPLATEARG_STRING(XALAN_STATIC_UCODE_STRING("invoke-arg"));
const XalanDOMString		Constants::ELEMNAME_CALLTEMPLATE_STRING(XALAN_STATIC_UCODE_STRING("call-template"));
const XalanDOMString		Constants::ELEMNAME_CALL_STRING(XALAN_STATIC_UCODE_STRING("call"));
const XalanDOMString		Constants::ELEMNAME_CHILDREN_STRING(XALAN_STATIC_UCODE_STRING("children"));
const XalanDOMString		Constants::ELEMNAME_CHOOSE_STRING(XALAN_STATIC_UCODE_STRING("choose"));
const XalanDOMString		Constants::ELEMNAME_COMMENT_STRING(XALAN_STATIC_UCODE_STRING("comment"));
const XalanDOMString		Constants::ELEMNAME_CONSTRUCT_STRING(XALAN_STATIC_UCODE_STRING("construct"));
const XalanDOMString		Constants::ELEMNAME_CONTENTS_STRING(XALAN_STATIC_UCODE_STRING("contents"));
const XalanDOMString		Constants::ELEMNAME_COPY_OF_STRING(XALAN_STATIC_UCODE_STRING("copy-of"));
const XalanDOMString		Constants::ELEMNAME_COPY_STRING(XALAN_STATIC_UCODE_STRING("copy"));
const XalanDOMString		Constants::ELEMNAME_COUNTERINCREMENT_STRING(XALAN_STATIC_UCODE_STRING("counter-increment"));
const XalanDOMString		Constants::ELEMNAME_COUNTERRESET_STRING(XALAN_STATIC_UCODE_STRING("counter-reset"));
const XalanDOMString		Constants::ELEMNAME_COUNTERSCOPE_STRING(XALAN_STATIC_UCODE_STRING("counter-scope"));
const XalanDOMString		Constants::ELEMNAME_COUNTERS_STRING(XALAN_STATIC_UCODE_STRING("counters"));
const XalanDOMString		Constants::ELEMNAME_COUNTER_STRING(XALAN_STATIC_UCODE_STRING("counter"));
const XalanDOMString		Constants::ELEMNAME_DECIMALFORMAT_STRING(XALAN_STATIC_UCODE_STRING("decimal-format"));
const XalanDOMString		Constants::ELEMNAME_DISPLAYIF_STRING(XALAN_STATIC_UCODE_STRING("display-if"));
const XalanDOMString		Constants::ELEMNAME_ELEMENT_STRING(XALAN_STATIC_UCODE_STRING("element"));
const XalanDOMString		Constants::ELEMNAME_EMPTY_STRING(XALAN_STATIC_UCODE_STRING("empty"));
const XalanDOMString		Constants::ELEMNAME_EVAL_STRING(XALAN_STATIC_UCODE_STRING("eval"));
const XalanDOMString		Constants::ELEMNAME_EXPECTEDCHILDREN_STRING(XALAN_STATIC_UCODE_STRING("expectedchildren"));
const XalanDOMString		Constants::ELEMNAME_EXTENSIONHANDLER_STRING(XALAN_STATIC_UCODE_STRING("code-dispatcher"));
const XalanDOMString		Constants::ELEMNAME_EXTENSION_STRING(XALAN_STATIC_UCODE_STRING("functions"));
const XalanDOMString		Constants::ELEMNAME_FALLBACK_STRING(XALAN_STATIC_UCODE_STRING("fallback"));
const XalanDOMString		Constants::ELEMNAME_FOREACH_STRING(XALAN_STATIC_UCODE_STRING("for-each"));
const XalanDOMString		Constants::ELEMNAME_IF_STRING(XALAN_STATIC_UCODE_STRING("if"));
const XalanDOMString		Constants::ELEMNAME_IMPORT_STRING(XALAN_STATIC_UCODE_STRING("import"));
const XalanDOMString		Constants::ELEMNAME_INCLUDE_STRING(XALAN_STATIC_UCODE_STRING("include"));
const XalanDOMString		Constants::ELEMNAME_KEY_STRING(XALAN_STATIC_UCODE_STRING("key"));
const XalanDOMString		Constants::ELEMNAME_LOCALE_STRING(XALAN_STATIC_UCODE_STRING("locale"));
const XalanDOMString		Constants::ELEMNAME_MESSAGE_STRING(XALAN_STATIC_UCODE_STRING("message"));
const XalanDOMString		Constants::ELEMNAME_NSALIAS_STRING(XALAN_STATIC_UCODE_STRING("namespace-alias"));

const XalanDOMString		Constants::ELEMNAME_NUMBER_STRING(XALAN_STATIC_UCODE_STRING("number"));
const XalanDOMString		Constants::ELEMNAME_OTHERWISE_STRING(XALAN_STATIC_UCODE_STRING("otherwise"));
const XalanDOMString		Constants::ELEMNAME_PARAM_STRING(XALAN_STATIC_UCODE_STRING("param"));
const XalanDOMString		Constants::ELEMNAME_PRESERVESPACE_STRING(XALAN_STATIC_UCODE_STRING("preserve-space"));
const XalanDOMString		Constants::ELEMNAME_ROOT_STRING(XALAN_STATIC_UCODE_STRING("root"));
const XalanDOMString		Constants::ELEMNAME_SORT_STRING(XALAN_STATIC_UCODE_STRING("sort"));
const XalanDOMString		Constants::ELEMNAME_STRIPSPACE_STRING(XALAN_STATIC_UCODE_STRING("strip-space"));
const XalanDOMString		Constants::ELEMNAME_STYLESHEET_STRING(XALAN_STATIC_UCODE_STRING("stylesheet"));
const XalanDOMString		Constants::ELEMNAME_TARGETATTRIBUTE_STRING(XALAN_STATIC_UCODE_STRING("target-attribute"));
const XalanDOMString		Constants::ELEMNAME_TARGETCOMMENT_STRING(XALAN_STATIC_UCODE_STRING("target-comment"));
const XalanDOMString		Constants::ELEMNAME_TARGETELEMENT_STRING(XALAN_STATIC_UCODE_STRING("target-element"));
const XalanDOMString		Constants::ELEMNAME_TARGETPI_STRING(XALAN_STATIC_UCODE_STRING("target-pi"));
const XalanDOMString		Constants::ELEMNAME_TARGETTEXT_STRING(XALAN_STATIC_UCODE_STRING("target-text"));
const XalanDOMString		Constants::ELEMNAME_TEMPLATE_STRING(XALAN_STATIC_UCODE_STRING("template"));
const XalanDOMString		Constants::ELEMNAME_TEXT_STRING(XALAN_STATIC_UCODE_STRING("text"));
const XalanDOMString		Constants::ELEMNAME_TRANSFORM_STRING(XALAN_STATIC_UCODE_STRING("transform"));
const XalanDOMString		Constants::ELEMNAME_URL_STRING(XALAN_STATIC_UCODE_STRING("uri"));
const XalanDOMString		Constants::ELEMNAME_USE_STRING(XALAN_STATIC_UCODE_STRING("use"));
const XalanDOMString		Constants::ELEMNAME_VALUEOF_STRING(XALAN_STATIC_UCODE_STRING("value-of"));
const XalanDOMString		Constants::ELEMNAME_VARIABLE_STRING(XALAN_STATIC_UCODE_STRING("variable"));
const XalanDOMString		Constants::ELEMNAME_WHEN_STRING(XALAN_STATIC_UCODE_STRING("when"));
const XalanDOMString		Constants::ELEMNAME_COMPONENT_STRING(XALAN_STATIC_UCODE_STRING("component"));
const XalanDOMString		Constants::ELEMNAME_CSSSTYLECONVERSION_STRING(XALAN_STATIC_UCODE_STRING("css-style-conversion"));
const XalanDOMString		Constants::ELEMNAME_OUTPUT_STRING(XALAN_STATIC_UCODE_STRING("output"));
const XalanDOMString		Constants::ELEMNAME_PARAMVARIABLE_OLD_STRING(XALAN_STATIC_UCODE_STRING("param-variable"));
const XalanDOMString		Constants::ELEMNAME_PARAMVARIABLE_STRING(XALAN_STATIC_UCODE_STRING("param"));
const XalanDOMString		Constants::ELEMNAME_PI_OLD_STRING(XALAN_STATIC_UCODE_STRING("pi"));
const XalanDOMString		Constants::ELEMNAME_PI_STRING(XALAN_STATIC_UCODE_STRING("processing-instruction"));
const XalanDOMString		Constants::ELEMNAME_SCRIPT_STRING(XALAN_STATIC_UCODE_STRING("script"));
const XalanDOMString		Constants::ELEMNAME_WITHPARAM_STRING(XALAN_STATIC_UCODE_STRING("with-param"));



const XalanDOMString		Constants::ATTRNAME_CLASS(XALAN_STATIC_UCODE_STRING("class"));
const XalanDOMString		Constants::ATTRNAME_ATTRIBUTE(XALAN_STATIC_UCODE_STRING("attribute"));
const XalanDOMString		Constants::ATTRNAME_ATTRIBUTE_SET(XALAN_STATIC_UCODE_STRING("attribute-set"));
const XalanDOMString		Constants::ATTRNAME_AMOUNT(XALAN_STATIC_UCODE_STRING("amount"));
const XalanDOMString		Constants::ATTRNAME_ANCESTOR(XALAN_STATIC_UCODE_STRING("ancestor"));
const XalanDOMString		Constants::ATTRNAME_CASEORDER(XALAN_STATIC_UCODE_STRING("case-order"));
const XalanDOMString		Constants::ATTRNAME_CONDITION(XALAN_STATIC_UCODE_STRING("condition"));
const XalanDOMString		Constants::ATTRNAME_COPYTYPE(XALAN_STATIC_UCODE_STRING("copy-type"));
const XalanDOMString		Constants::ATTRNAME_COUNT(XALAN_STATIC_UCODE_STRING("count"));
const XalanDOMString		Constants::ATTRNAME_DATATYPE(XALAN_STATIC_UCODE_STRING("data-type"));
const XalanDOMString		Constants::ATTRNAME_DECIMALSEPARATOR(XALAN_STATIC_UCODE_STRING("decimal-separator"));
const XalanDOMString		Constants::ATTRNAME_DEFAULTSPACE(XALAN_STATIC_UCODE_STRING("default-space"));
const XalanDOMString		Constants::ATTRNAME_DEFAULT(XALAN_STATIC_UCODE_STRING("default"));
const XalanDOMString		Constants::ATTRNAME_DEPTH(XALAN_STATIC_UCODE_STRING("with-children"));
const XalanDOMString		Constants::ATTRNAME_DIGIT(XALAN_STATIC_UCODE_STRING("digit"));
const XalanDOMString		Constants::ATTRNAME_DIGITGROUPSEP(XALAN_STATIC_UCODE_STRING("digit-group-sep"));
const XalanDOMString		Constants::ATTRNAME_ELEMENT(XALAN_STATIC_UCODE_STRING("element"));
const XalanDOMString		Constants::ATTRNAME_ELEMENTS(XALAN_STATIC_UCODE_STRING("elements"));
const XalanDOMString		Constants::ATTRNAME_EXPR(XALAN_STATIC_UCODE_STRING("expr"));
const XalanDOMString		Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES(XALAN_STATIC_UCODE_STRING("extension-element-prefixes"));
const XalanDOMString		Constants::ATTRNAME_FORMAT(XALAN_STATIC_UCODE_STRING("format"));
const XalanDOMString		Constants::ATTRNAME_FROM(XALAN_STATIC_UCODE_STRING("from"));
const XalanDOMString		Constants::ATTRNAME_GROUPINGSEPARATOR(XALAN_STATIC_UCODE_STRING("grouping-separator"));
const XalanDOMString		Constants::ATTRNAME_GROUPINGSIZE(XALAN_STATIC_UCODE_STRING("grouping-size"));
const XalanDOMString		Constants::ATTRNAME_HREF(XALAN_STATIC_UCODE_STRING("href"));
const XalanDOMString		Constants::ATTRNAME_ID(XALAN_STATIC_UCODE_STRING("id"));
const XalanDOMString		Constants::ATTRNAME_IMPORTANCE(XALAN_STATIC_UCODE_STRING("importance"));
const XalanDOMString		Constants::ATTRNAME_INDENTRESULT(XALAN_STATIC_UCODE_STRING("indent-result"));
const XalanDOMString		Constants::ATTRNAME_INFINITY(XALAN_STATIC_UCODE_STRING("infinity"));
const XalanDOMString		Constants::ATTRNAME_LETTERVALUE(XALAN_STATIC_UCODE_STRING("letter-value"));
const XalanDOMString		Constants::ATTRNAME_LEVEL(XALAN_STATIC_UCODE_STRING("level"));
const XalanDOMString		Constants::ATTRNAME_LANG(XALAN_STATIC_UCODE_STRING("lang"));
//const XalanDOMString	Constants::ATTRNAME_MACRO(XALAN_STATIC_UCODE_STRING("macro"));
const XalanDOMString		Constants::ATTRNAME_MATCH(XALAN_STATIC_UCODE_STRING("match"));
const XalanDOMString		Constants::ATTRNAME_MINUSSIGN(XALAN_STATIC_UCODE_STRING("minus-sign"));
const XalanDOMString		Constants::ATTRNAME_MODE(XALAN_STATIC_UCODE_STRING("mode"));
const XalanDOMString		Constants::ATTRNAME_NAME(XALAN_STATIC_UCODE_STRING("name"));
const XalanDOMString		Constants::ATTRNAME_NAMESPACE(XALAN_STATIC_UCODE_STRING("namespace"));
const XalanDOMString		Constants::ATTRNAME_NAN(XALAN_STATIC_UCODE_STRING("NaN"));
const XalanDOMString		Constants::ATTRNAME_NDIGITSPERGROUP(XALAN_STATIC_UCODE_STRING("n-digits-per-group"));
const XalanDOMString		Constants::ATTRNAME_ONLY(XALAN_STATIC_UCODE_STRING("only"));
const XalanDOMString		Constants::ATTRNAME_ORDER(XALAN_STATIC_UCODE_STRING("order"));
const XalanDOMString		Constants::ATTRNAME_PATTERNSEPARATOR(XALAN_STATIC_UCODE_STRING("pattern-separator"));
const XalanDOMString		Constants::ATTRNAME_PERCENT(XALAN_STATIC_UCODE_STRING("percent"));
const XalanDOMString		Constants::ATTRNAME_PERMILLE(XALAN_STATIC_UCODE_STRING("per-mille"));
const XalanDOMString		Constants::ATTRNAME_PRIORITY(XALAN_STATIC_UCODE_STRING("priority"));
const XalanDOMString		Constants::ATTRNAME_REFID(XALAN_STATIC_UCODE_STRING("refID"));
const XalanDOMString		Constants::ATTRNAME_RESULTNS(XALAN_STATIC_UCODE_STRING("result-ns"));
const XalanDOMString		Constants::ATTRNAME_RESULT_PREFIX(XALAN_STATIC_UCODE_STRING("result-prefix"));
const XalanDOMString		Constants::ATTRNAME_SELECT(XALAN_STATIC_UCODE_STRING("select"));
const XalanDOMString		Constants::ATTRNAME_SEQUENCESRC(XALAN_STATIC_UCODE_STRING("sequence-src"));
const XalanDOMString		Constants::ATTRNAME_STYLE(XALAN_STATIC_UCODE_STRING("style"));
const XalanDOMString		Constants::ATTRNAME_STYLESHEET_PREFIX(XALAN_STATIC_UCODE_STRING("stylesheet-prefix"));
const XalanDOMString		Constants::ATTRNAME_TEST(XALAN_STATIC_UCODE_STRING("test"));
const XalanDOMString		Constants::ATTRNAME_TOSTRING(XALAN_STATIC_UCODE_STRING("to-string"));
const XalanDOMString		Constants::ATTRNAME_TYPE(XALAN_STATIC_UCODE_STRING("type"));
const XalanDOMString		Constants::ATTRNAME_USE(XALAN_STATIC_UCODE_STRING("use"));
const XalanDOMString		Constants::ATTRNAME_USEATTRIBUTESETS(XALAN_STATIC_UCODE_STRING("use-attribute-sets"));
const XalanDOMString		Constants::ATTRNAME_VALUE(XALAN_STATIC_UCODE_STRING("value"));
const XalanDOMString		Constants::ATTRNAME_XMLNSDEF(XALAN_STATIC_UCODE_STRING("xmlns"));
const XalanDOMString		Constants::ATTRNAME_XMLNS(XALAN_STATIC_UCODE_STRING("xmlns:"));
const XalanDOMString		Constants::ATTRNAME_XMLSPACE(XALAN_STATIC_UCODE_STRING("xml:space"));
const XalanDOMString		Constants::ATTRNAME_NS(XALAN_STATIC_UCODE_STRING("ns"));
const XalanDOMString		Constants::ATTRNAME_CLASSID(XALAN_STATIC_UCODE_STRING("classid"));
const XalanDOMString		Constants::ATTRNAME_ARCHIVE(XALAN_STATIC_UCODE_STRING("archive"));
const XalanDOMString		Constants::ATTRNAME_CODETYPE(XALAN_STATIC_UCODE_STRING("type"));
const XalanDOMString		Constants::ATTRNAME_CODEBASE(XALAN_STATIC_UCODE_STRING("codebase"));
const XalanDOMString		Constants::ATTRNAME_METHOD(XALAN_STATIC_UCODE_STRING("calls"));
const XalanDOMString		Constants::ATTRVAL_PRESERVE(XALAN_STATIC_UCODE_STRING("preserve"));
const XalanDOMString		Constants::ATTRVAL_STRIP(XALAN_STATIC_UCODE_STRING("strip"));
const XalanDOMString		Constants::ATTRVAL_YES(XALAN_STATIC_UCODE_STRING("yes"));
const XalanDOMString		Constants::ATTRVAL_NO(XALAN_STATIC_UCODE_STRING("no"));
const XalanDOMString		Constants::ATTRVAL_ALPHABETIC(XALAN_STATIC_UCODE_STRING("alphabetic"));
const XalanDOMString		Constants::ATTRVAL_OTHER(XALAN_STATIC_UCODE_STRING("other"));
const XalanDOMString		Constants::ATTRVAL_SINGLE(XALAN_STATIC_UCODE_STRING("single"));
const XalanDOMString		Constants::ATTRVAL_MULTI(XALAN_STATIC_UCODE_STRING("multiple"));
const XalanDOMString		Constants::ATTRVAL_ANY(XALAN_STATIC_UCODE_STRING("any"));
const XalanDOMString		Constants::ATTRNAME_OUTPUT_METHOD(XALAN_STATIC_UCODE_STRING("method"));
const XalanDOMString		Constants::ATTRNAME_DISABLE_OUTPUT_ESCAPING (XALAN_STATIC_UCODE_STRING("disable-output-escaping"));
const XalanDOMString		Constants::ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS (XALAN_STATIC_UCODE_STRING("cdata-section-elements"));
const XalanDOMString		Constants::ATTRNAME_OUTPUT_DOCTYPE_PUBLIC(XALAN_STATIC_UCODE_STRING("doctype-public"));
const XalanDOMString		Constants::ATTRNAME_OUTPUT_DOCTYPE_SYSTEM(XALAN_STATIC_UCODE_STRING("doctype-system"));
const XalanDOMString		Constants::ATTRNAME_OUTPUT_ENCODING(XALAN_STATIC_UCODE_STRING("encoding"));
const XalanDOMString		Constants::ATTRNAME_OUTPUT_INDENT(XALAN_STATIC_UCODE_STRING("indent"));
const XalanDOMString		Constants::ATTRNAME_OUTPUT_MEDIATYPE(XALAN_STATIC_UCODE_STRING("media-type"));
const XalanDOMString		Constants::ATTRNAME_OUTPUT_STANDALONE (XALAN_STATIC_UCODE_STRING("standalone"));
const XalanDOMString		Constants::ATTRNAME_OUTPUT_VERSION(XALAN_STATIC_UCODE_STRING("version"));
//const XalanDOMString		Constants::ATTRNAME_OUTPUT_XMLDECL(XALAN_STATIC_UCODE_STRING("xml-declaration"));
const XalanDOMString		Constants::ATTRNAME_OUTPUT_OMITXMLDECL(XALAN_STATIC_UCODE_STRING("omit-xml-declaration"));
const XalanDOMString		Constants::ATTRNAME_ZERODIGIT(XALAN_STATIC_UCODE_STRING("zero-digit"));

const XalanDOMString		Constants::ATTRVAL_OUTPUT_METHOD_HTML(XALAN_STATIC_UCODE_STRING("html"));
const XalanDOMString		Constants::ATTRVAL_OUTPUT_METHOD_XML(XALAN_STATIC_UCODE_STRING("xml"));
const XalanDOMString		Constants::ATTRVAL_OUTPUT_METHOD_TEXT(XALAN_STATIC_UCODE_STRING("text"));
const XalanDOMString		Constants::ATTRVAL_THIS(XALAN_STATIC_UCODE_STRING("."));
const XalanDOMString		Constants::ATTRVAL_PARENT(XALAN_STATIC_UCODE_STRING(".."));
const XalanDOMString		Constants::ATTRVAL_ANCESTOR(XALAN_STATIC_UCODE_STRING("ancestor"));
const XalanDOMString		Constants::ATTRVAL_ID(XALAN_STATIC_UCODE_STRING("id"));
const XalanDOMString		Constants::ATTRVAL_DATATYPE_TEXT(XALAN_STATIC_UCODE_STRING("text"));
const XalanDOMString		Constants::ATTRVAL_DATATYPE_NUMBER(XALAN_STATIC_UCODE_STRING("number"));
const XalanDOMString		Constants::ATTRVAL_ORDER_ASCENDING(XALAN_STATIC_UCODE_STRING("ascending"));
const XalanDOMString		Constants::ATTRVAL_ORDER_DESCENDING(XALAN_STATIC_UCODE_STRING("descending"));
const XalanDOMString		Constants::ATTRVAL_CASEORDER_UPPER(XALAN_STATIC_UCODE_STRING("upper-first"));
const XalanDOMString		Constants::ATTRVAL_CASEORDER_LOWER(XALAN_STATIC_UCODE_STRING("lower-first"));
const XalanDOMString		Constants::ATTRVAL_DEFAULT_PREFIX(XALAN_STATIC_UCODE_STRING("#default"));

const XalanDOMString		Constants::ATTRVAL_INFINITY(XALAN_STATIC_UCODE_STRING("Infinity"));
const XalanDOMString		Constants::ATTRVAL_NAN(XALAN_STATIC_UCODE_STRING("NaN"));

const XalanDOMString		Constants::DEFAULT_DECIMAL_FORMAT(XALAN_STATIC_UCODE_STRING("#default"));

const XalanDOMString		Constants::PSEUDONAME_ANY(XALAN_STATIC_UCODE_STRING("*"));
const XalanDOMString		Constants::PSEUDONAME_ROOT(XALAN_STATIC_UCODE_STRING("/"));
const XalanDOMString		Constants::PSEUDONAME_TEXT(XALAN_STATIC_UCODE_STRING("#text"));
const XalanDOMString		Constants::PSEUDONAME_COMMENT(XALAN_STATIC_UCODE_STRING("#comment"));
const XalanDOMString		Constants::PSEUDONAME_PI(XALAN_STATIC_UCODE_STRING("#pi"));
const XalanDOMString		Constants::PSEUDONAME_OTHER(XALAN_STATIC_UCODE_STRING("*"));
