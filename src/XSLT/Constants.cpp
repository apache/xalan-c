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



const DOMString		Constants::ELEMNAME_ANCHOR_STRING(XALAN_STATIC_UCODE_STRING("anchor"));
const DOMString		Constants::ELEMNAME_ANY_STRING(XALAN_STATIC_UCODE_STRING("any"));
const DOMString		Constants::ELEMNAME_APPLY_IMPORTS_STRING(XALAN_STATIC_UCODE_STRING("apply-imports"));
const DOMString		Constants::ELEMNAME_APPLY_TEMPLATES_STRING(XALAN_STATIC_UCODE_STRING("apply-templates"));
const DOMString		Constants::ELEMNAME_ARG_STRING(XALAN_STATIC_UCODE_STRING("arg"));
const DOMString		Constants::ELEMNAME_ATTRIBUTESET_STRING(XALAN_STATIC_UCODE_STRING("attribute-set"));
const DOMString		Constants::ELEMNAME_ATTRIBUTE_STRING(XALAN_STATIC_UCODE_STRING("attribute"));
const DOMString		Constants::ELEMNAME_CALLTEMPLATEARG_STRING(XALAN_STATIC_UCODE_STRING("invoke-arg"));
const DOMString		Constants::ELEMNAME_CALLTEMPLATE_STRING(XALAN_STATIC_UCODE_STRING("call-template"));
const DOMString		Constants::ELEMNAME_CALL_STRING(XALAN_STATIC_UCODE_STRING("call"));
const DOMString		Constants::ELEMNAME_CHILDREN_STRING(XALAN_STATIC_UCODE_STRING("children"));
const DOMString		Constants::ELEMNAME_CHOOSE_STRING(XALAN_STATIC_UCODE_STRING("choose"));
const DOMString		Constants::ELEMNAME_COMMENT_STRING(XALAN_STATIC_UCODE_STRING("comment"));
const DOMString		Constants::ELEMNAME_CONSTRUCT_STRING(XALAN_STATIC_UCODE_STRING("construct"));
const DOMString		Constants::ELEMNAME_CONTENTS_STRING(XALAN_STATIC_UCODE_STRING("contents"));
const DOMString		Constants::ELEMNAME_COPY_OF_STRING(XALAN_STATIC_UCODE_STRING("copy-of"));
const DOMString		Constants::ELEMNAME_COPY_STRING(XALAN_STATIC_UCODE_STRING("copy"));
const DOMString		Constants::ELEMNAME_COUNTERINCREMENT_STRING(XALAN_STATIC_UCODE_STRING("counter-increment"));
const DOMString		Constants::ELEMNAME_COUNTERRESET_STRING(XALAN_STATIC_UCODE_STRING("counter-reset"));
const DOMString		Constants::ELEMNAME_COUNTERSCOPE_STRING(XALAN_STATIC_UCODE_STRING("counter-scope"));
const DOMString		Constants::ELEMNAME_COUNTERS_STRING(XALAN_STATIC_UCODE_STRING("counters"));
const DOMString		Constants::ELEMNAME_COUNTER_STRING(XALAN_STATIC_UCODE_STRING("counter"));
const DOMString		Constants::ELEMNAME_DISPLAYIF_STRING(XALAN_STATIC_UCODE_STRING("display-if"));
const DOMString		Constants::ELEMNAME_ELEMENT_STRING(XALAN_STATIC_UCODE_STRING("element"));
const DOMString		Constants::ELEMNAME_EMPTY_STRING(XALAN_STATIC_UCODE_STRING("empty"));
const DOMString		Constants::ELEMNAME_EVAL_STRING(XALAN_STATIC_UCODE_STRING("eval"));
const DOMString		Constants::ELEMNAME_EXPECTEDCHILDREN_STRING(XALAN_STATIC_UCODE_STRING("expectedchildren"));
const DOMString		Constants::ELEMNAME_EXTENSIONHANDLER_STRING(XALAN_STATIC_UCODE_STRING("code-dispatcher"));
const DOMString		Constants::ELEMNAME_EXTENSION_STRING(XALAN_STATIC_UCODE_STRING("functions"));
const DOMString		Constants::ELEMNAME_FALLBACK_STRING(XALAN_STATIC_UCODE_STRING("fallback"));
const DOMString		Constants::ELEMNAME_FOREACH_STRING(XALAN_STATIC_UCODE_STRING("for-each"));
const DOMString		Constants::ELEMNAME_IF_STRING(XALAN_STATIC_UCODE_STRING("if"));
const DOMString		Constants::ELEMNAME_IMPORT_STRING(XALAN_STATIC_UCODE_STRING("import"));
const DOMString		Constants::ELEMNAME_INCLUDE_STRING(XALAN_STATIC_UCODE_STRING("include"));
const DOMString		Constants::ELEMNAME_KEY_STRING(XALAN_STATIC_UCODE_STRING("key"));
const DOMString		Constants::ELEMNAME_LOCALE_STRING(XALAN_STATIC_UCODE_STRING("locale"));
const DOMString		Constants::ELEMNAME_MESSAGE_STRING(XALAN_STATIC_UCODE_STRING("message"));
const DOMString		Constants::ELEMNAME_NUMBER_STRING(XALAN_STATIC_UCODE_STRING("number"));
const DOMString		Constants::ELEMNAME_OTHERWISE_STRING(XALAN_STATIC_UCODE_STRING("otherwise"));
const DOMString		Constants::ELEMNAME_PARAM_STRING(XALAN_STATIC_UCODE_STRING("param"));
const DOMString		Constants::ELEMNAME_PRESERVESPACE_STRING(XALAN_STATIC_UCODE_STRING("preserve-space"));
const DOMString		Constants::ELEMNAME_ROOT_STRING(XALAN_STATIC_UCODE_STRING("root"));
const DOMString		Constants::ELEMNAME_SORT_STRING(XALAN_STATIC_UCODE_STRING("sort"));
const DOMString		Constants::ELEMNAME_STRIPSPACE_STRING(XALAN_STATIC_UCODE_STRING("strip-space"));
const DOMString		Constants::ELEMNAME_STYLESHEET_STRING(XALAN_STATIC_UCODE_STRING("stylesheet"));
const DOMString		Constants::ELEMNAME_TARGETATTRIBUTE_STRING(XALAN_STATIC_UCODE_STRING("target-attribute"));
const DOMString		Constants::ELEMNAME_TARGETCOMMENT_STRING(XALAN_STATIC_UCODE_STRING("target-comment"));
const DOMString		Constants::ELEMNAME_TARGETELEMENT_STRING(XALAN_STATIC_UCODE_STRING("target-element"));
const DOMString		Constants::ELEMNAME_TARGETPI_STRING(XALAN_STATIC_UCODE_STRING("target-pi"));
const DOMString		Constants::ELEMNAME_TARGETTEXT_STRING(XALAN_STATIC_UCODE_STRING("target-text"));
const DOMString		Constants::ELEMNAME_TEMPLATE_STRING(XALAN_STATIC_UCODE_STRING("template"));
const DOMString		Constants::ELEMNAME_TEXT_STRING(XALAN_STATIC_UCODE_STRING("text"));
const DOMString		Constants::ELEMNAME_TRANSFORM_STRING(XALAN_STATIC_UCODE_STRING("transform"));
const DOMString		Constants::ELEMNAME_URL_STRING(XALAN_STATIC_UCODE_STRING("uri"));
const DOMString		Constants::ELEMNAME_USE_STRING(XALAN_STATIC_UCODE_STRING("use"));
const DOMString		Constants::ELEMNAME_VALUEOF_STRING(XALAN_STATIC_UCODE_STRING("value-of"));
const DOMString		Constants::ELEMNAME_VARIABLE_STRING(XALAN_STATIC_UCODE_STRING("variable"));
const DOMString		Constants::ELEMNAME_WHEN_STRING(XALAN_STATIC_UCODE_STRING("when"));
const DOMString		Constants::ELEMNAME_COMPONENT_STRING(XALAN_STATIC_UCODE_STRING("component"));
const DOMString		Constants::ELEMNAME_CSSSTYLECONVERSION_STRING(XALAN_STATIC_UCODE_STRING("css-style-conversion"));
const DOMString		Constants::ELEMNAME_OUTPUT_STRING(XALAN_STATIC_UCODE_STRING("output"));
const DOMString		Constants::ELEMNAME_PARAMVARIABLE_OLD_STRING(XALAN_STATIC_UCODE_STRING("param-variable"));
const DOMString		Constants::ELEMNAME_PARAMVARIABLE_STRING(XALAN_STATIC_UCODE_STRING("param"));
const DOMString		Constants::ELEMNAME_PI_OLD_STRING(XALAN_STATIC_UCODE_STRING("pi"));
const DOMString		Constants::ELEMNAME_PI_STRING(XALAN_STATIC_UCODE_STRING("processing-instruction"));
const DOMString		Constants::ELEMNAME_SCRIPT_STRING(XALAN_STATIC_UCODE_STRING("script"));
const DOMString		Constants::ELEMNAME_WITHPARAM_STRING(XALAN_STATIC_UCODE_STRING("with-param"));



const DOMString		Constants::ATTRNAME_CLASS(XALAN_STATIC_UCODE_STRING("class"));
const DOMString		Constants::ATTRNAME_ATTRIBUTE(XALAN_STATIC_UCODE_STRING("attribute"));
const DOMString		Constants::ATTRNAME_ATTRIBUTE_SET(XALAN_STATIC_UCODE_STRING("attribute-set"));
const DOMString		Constants::ATTRNAME_AMOUNT(XALAN_STATIC_UCODE_STRING("amount"));
const DOMString		Constants::ATTRNAME_ANCESTOR(XALAN_STATIC_UCODE_STRING("ancestor"));
const DOMString		Constants::ATTRNAME_CASEORDER(XALAN_STATIC_UCODE_STRING("case-order"));
const DOMString		Constants::ATTRNAME_CONDITION(XALAN_STATIC_UCODE_STRING("condition"));
const DOMString		Constants::ATTRNAME_COPYTYPE(XALAN_STATIC_UCODE_STRING("copy-type"));
const DOMString		Constants::ATTRNAME_COUNT(XALAN_STATIC_UCODE_STRING("count"));
const DOMString		Constants::ATTRNAME_DATATYPE(XALAN_STATIC_UCODE_STRING("data-type"));
const DOMString		Constants::ATTRNAME_DEFAULTSPACE(XALAN_STATIC_UCODE_STRING("default-space"));
const DOMString		Constants::ATTRNAME_DEFAULT(XALAN_STATIC_UCODE_STRING("default"));
const DOMString		Constants::ATTRNAME_DEPTH(XALAN_STATIC_UCODE_STRING("with-children"));
const DOMString		Constants::ATTRNAME_DIGITGROUPSEP(XALAN_STATIC_UCODE_STRING("digit-group-sep"));
const DOMString		Constants::ATTRNAME_ELEMENT(XALAN_STATIC_UCODE_STRING("element"));
const DOMString		Constants::ATTRNAME_ELEMENTS(XALAN_STATIC_UCODE_STRING("elements"));
const DOMString		Constants::ATTRNAME_EXPR(XALAN_STATIC_UCODE_STRING("expr"));
const DOMString		Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES(XALAN_STATIC_UCODE_STRING("extension-element-prefixes"));
const DOMString		Constants::ATTRNAME_FORMAT(XALAN_STATIC_UCODE_STRING("format"));
const DOMString		Constants::ATTRNAME_FROM(XALAN_STATIC_UCODE_STRING("from"));
const DOMString		Constants::ATTRNAME_GROUPINGSEPARATOR(XALAN_STATIC_UCODE_STRING("grouping-separator"));
const DOMString		Constants::ATTRNAME_GROUPINGSIZE(XALAN_STATIC_UCODE_STRING("grouping-size"));
const DOMString		Constants::ATTRNAME_HREF(XALAN_STATIC_UCODE_STRING("href"));
const DOMString		Constants::ATTRNAME_ID(XALAN_STATIC_UCODE_STRING("id"));
const DOMString		Constants::ATTRNAME_IMPORTANCE(XALAN_STATIC_UCODE_STRING("importance"));
const DOMString		Constants::ATTRNAME_INDENTRESULT(XALAN_STATIC_UCODE_STRING("indent-result"));
const DOMString		Constants::ATTRNAME_LETTERVALUE(XALAN_STATIC_UCODE_STRING("letter-value"));
const DOMString		Constants::ATTRNAME_LEVEL(XALAN_STATIC_UCODE_STRING("level"));
const DOMString		Constants::ATTRNAME_LANG(XALAN_STATIC_UCODE_STRING("lang"));
//const DOMString	Constants::ATTRNAME_MACRO(XALAN_STATIC_UCODE_STRING("macro"));
const DOMString		Constants::ATTRNAME_MATCH(XALAN_STATIC_UCODE_STRING("match"));
const DOMString		Constants::ATTRNAME_MODE(XALAN_STATIC_UCODE_STRING("mode"));
const DOMString		Constants::ATTRNAME_NAME(XALAN_STATIC_UCODE_STRING("name"));
const DOMString		Constants::ATTRNAME_NAMESPACE(XALAN_STATIC_UCODE_STRING("namespace"));
const DOMString		Constants::ATTRNAME_NDIGITSPERGROUP(XALAN_STATIC_UCODE_STRING("n-digits-per-group"));
const DOMString		Constants::ATTRNAME_ONLY(XALAN_STATIC_UCODE_STRING("only"));
const DOMString		Constants::ATTRNAME_ORDER(XALAN_STATIC_UCODE_STRING("order"));
const DOMString		Constants::ATTRNAME_PRIORITY(XALAN_STATIC_UCODE_STRING("priority"));
const DOMString		Constants::ATTRNAME_REFID(XALAN_STATIC_UCODE_STRING("refID"));
const DOMString		Constants::ATTRNAME_RESULTNS(XALAN_STATIC_UCODE_STRING("result-ns"));
const DOMString		Constants::ATTRNAME_SELECT(XALAN_STATIC_UCODE_STRING("select"));
const DOMString		Constants::ATTRNAME_SEQUENCESRC(XALAN_STATIC_UCODE_STRING("sequence-src"));
const DOMString		Constants::ATTRNAME_STYLE(XALAN_STATIC_UCODE_STRING("style"));
const DOMString		Constants::ATTRNAME_TEST(XALAN_STATIC_UCODE_STRING("test"));
const DOMString		Constants::ATTRNAME_TOSTRING(XALAN_STATIC_UCODE_STRING("to-string"));
const DOMString		Constants::ATTRNAME_TYPE(XALAN_STATIC_UCODE_STRING("type"));
const DOMString		Constants::ATTRNAME_USE(XALAN_STATIC_UCODE_STRING("use"));
const DOMString		Constants::ATTRNAME_USEATTRIBUTESETS(XALAN_STATIC_UCODE_STRING("use-attribute-sets"));
const DOMString		Constants::ATTRNAME_VALUE(XALAN_STATIC_UCODE_STRING("value"));
const DOMString		Constants::ATTRNAME_XMLNSDEF(XALAN_STATIC_UCODE_STRING("xmlns"));
const DOMString		Constants::ATTRNAME_XMLNS(XALAN_STATIC_UCODE_STRING("xmlns:"));
const DOMString		Constants::ATTRNAME_XMLSPACE(XALAN_STATIC_UCODE_STRING("xml:space"));
const DOMString		Constants::ATTRNAME_NS(XALAN_STATIC_UCODE_STRING("ns"));
const DOMString		Constants::ATTRNAME_CLASSID(XALAN_STATIC_UCODE_STRING("classid"));
const DOMString		Constants::ATTRNAME_ARCHIVE(XALAN_STATIC_UCODE_STRING("archive"));
const DOMString		Constants::ATTRNAME_CODETYPE(XALAN_STATIC_UCODE_STRING("type"));
const DOMString		Constants::ATTRNAME_CODEBASE(XALAN_STATIC_UCODE_STRING("codebase"));
const DOMString		Constants::ATTRNAME_METHOD(XALAN_STATIC_UCODE_STRING("calls"));
const DOMString		Constants::ATTRVAL_PRESERVE(XALAN_STATIC_UCODE_STRING("preserve"));
const DOMString		Constants::ATTRVAL_STRIP(XALAN_STATIC_UCODE_STRING("strip"));
const DOMString		Constants::ATTRVAL_YES(XALAN_STATIC_UCODE_STRING("yes"));
const DOMString		Constants::ATTRVAL_NO(XALAN_STATIC_UCODE_STRING("no"));
const DOMString		Constants::ATTRVAL_ALPHABETIC(XALAN_STATIC_UCODE_STRING("alphabetic"));
const DOMString		Constants::ATTRVAL_OTHER(XALAN_STATIC_UCODE_STRING("other"));
const DOMString		Constants::ATTRVAL_SINGLE(XALAN_STATIC_UCODE_STRING("single"));
const DOMString		Constants::ATTRVAL_MULTI(XALAN_STATIC_UCODE_STRING("multiple"));
const DOMString		Constants::ATTRVAL_ANY(XALAN_STATIC_UCODE_STRING("any"));
const DOMString		Constants::ATTRNAME_OUTPUT_METHOD(XALAN_STATIC_UCODE_STRING("method"));
const DOMString		Constants::ATTRNAME_DISABLE_OUTPUT_ESCAPING (XALAN_STATIC_UCODE_STRING("disable-output-escaping"));
const DOMString		Constants::ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS (XALAN_STATIC_UCODE_STRING("cdata-section-elements"));
const DOMString		Constants::ATTRNAME_OUTPUT_DOCTYPE_PUBLIC(XALAN_STATIC_UCODE_STRING("doctype-public"));
const DOMString		Constants::ATTRNAME_OUTPUT_DOCTYPE_SYSTEM(XALAN_STATIC_UCODE_STRING("doctype-system"));
const DOMString		Constants::ATTRNAME_OUTPUT_ENCODING(XALAN_STATIC_UCODE_STRING("encoding"));
const DOMString		Constants::ATTRNAME_OUTPUT_INDENT(XALAN_STATIC_UCODE_STRING("indent"));
const DOMString		Constants::ATTRNAME_OUTPUT_MEDIATYPE(XALAN_STATIC_UCODE_STRING("media-type"));
const DOMString		Constants::ATTRNAME_OUTPUT_STANDALONE (XALAN_STATIC_UCODE_STRING("standalone"));
const DOMString		Constants::ATTRNAME_OUTPUT_VERSION(XALAN_STATIC_UCODE_STRING("version"));
const DOMString		Constants::ATTRNAME_OUTPUT_XMLDECL(XALAN_STATIC_UCODE_STRING("xml-declaration"));
const DOMString		Constants::ATTRVAL_OUTPUT_METHOD_HTML(XALAN_STATIC_UCODE_STRING("html"));
const DOMString		Constants::ATTRVAL_OUTPUT_METHOD_XML(XALAN_STATIC_UCODE_STRING("xml"));
const DOMString		Constants::ATTRVAL_OUTPUT_METHOD_TEXT(XALAN_STATIC_UCODE_STRING("text"));
const DOMString		Constants::ATTRVAL_THIS(XALAN_STATIC_UCODE_STRING("."));
const DOMString		Constants::ATTRVAL_PARENT(XALAN_STATIC_UCODE_STRING(".."));
const DOMString		Constants::ATTRVAL_ANCESTOR(XALAN_STATIC_UCODE_STRING("ancestor"));
const DOMString		Constants::ATTRVAL_ID(XALAN_STATIC_UCODE_STRING("id"));
const DOMString		Constants::ATTRVAL_DATATYPE_TEXT(XALAN_STATIC_UCODE_STRING("text"));
const DOMString		Constants::ATTRVAL_DATATYPE_NUMBER(XALAN_STATIC_UCODE_STRING("number"));
const DOMString		Constants::ATTRVAL_ORDER_ASCENDING(XALAN_STATIC_UCODE_STRING("ascending"));
const DOMString		Constants::ATTRVAL_ORDER_DESCENDING(XALAN_STATIC_UCODE_STRING("descending"));
const DOMString		Constants::ATTRVAL_CASEORDER_UPPER(XALAN_STATIC_UCODE_STRING("upper-first"));
const DOMString		Constants::ATTRVAL_CASEORDER_LOWER(XALAN_STATIC_UCODE_STRING("lower-first"));



const DOMString		Constants::PSEUDONAME_ANY(XALAN_STATIC_UCODE_STRING("*"));
const DOMString		Constants::PSEUDONAME_ROOT(XALAN_STATIC_UCODE_STRING("/"));
const DOMString		Constants::PSEUDONAME_TEXT(XALAN_STATIC_UCODE_STRING("#text"));
const DOMString		Constants::PSEUDONAME_COMMENT(XALAN_STATIC_UCODE_STRING("#comment"));
const DOMString		Constants::PSEUDONAME_PI(XALAN_STATIC_UCODE_STRING("#pi"));
const DOMString		Constants::PSEUDONAME_OTHER(XALAN_STATIC_UCODE_STRING("*"));
