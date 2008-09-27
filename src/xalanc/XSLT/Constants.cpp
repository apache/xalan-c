/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Constants.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>


XALAN_USING_XALAN(XalanDOMString)

XALAN_USING_XALAN(XalanMemMgrs)


static XalanDOMString       ATTRNAME_CASEORDER(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_COUNT(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_DATATYPE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_DECIMALSEPARATOR(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_DEFAULT(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_DEFAULTSPACE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_DIGIT(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_DIGITGROUPSEP(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_DISABLE_OUTPUT_ESCAPING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_ELEMENTS(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_EXCLUDE_RESULT_PREFIXES(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_EXTENSIONELEMENTPREFIXES(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_ESCAPE_URLS(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_FORMAT(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_FROM(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_GROUPINGSEPARATOR(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_GROUPINGSIZE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_HREF(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_ID(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_INDENTAMOUNT(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_INDENTRESULT(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_INFINITY(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_LANG(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_LETTERVALUE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_MATCH(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_MINUSSIGN(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_MODE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_NAME(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_NAMESPACE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_NAN(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_NDIGITSPERGROUP(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_OMIT_META_TAG(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_ORDER(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_OUTPUT_DOCTYPE_PUBLIC(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_OUTPUT_DOCTYPE_SYSTEM(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_OUTPUT_ENCODING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_OUTPUT_INDENT(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_OUTPUT_MEDIATYPE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_OUTPUT_METHOD(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_OUTPUT_OMITXMLDECL(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_OUTPUT_STANDALONE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_OUTPUT_VERSION(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_PATTERNSEPARATOR(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_PERCENT(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_PERMILLE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_PREFIX(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_PRIORITY(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_RESULTNS(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_RESULT_PREFIX(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_SELECT(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_STYLESHEET_PREFIX(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_TERMINATE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_TEST(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_TYPE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_USE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_USEATTRIBUTESETS(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_VALUE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_VERSION(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_SPACE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_XMLSPACE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRNAME_ZERODIGIT(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString       ATTRTYPE_CDATA(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString       ATTRVAL_CASEORDER_LOWER(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_CASEORDER_UPPER(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_DATATYPE_NUMBER(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_DATATYPE_TEXT(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_DEFAULT(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_DEFAULT_PREFIX(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_DEFAULT_TEXT_RULE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_INFINITY(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_NAN(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_NO(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_ORDER_ASCENDING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_ORDER_DESCENDING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_OUTPUT_METHOD_HTML(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_OUTPUT_METHOD_TEXT(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_OUTPUT_METHOD_XML(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_PRESERVE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_SIMPLE(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_STRIP(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_THIS(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ATTRVAL_YES(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString       DEFAULT_WHITESPACE_SEPARATOR_STRING(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString       ELEMNAME_ANY_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_APPLY_IMPORTS_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_APPLY_TEMPLATES_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_ATTRIBUTE_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_CALLTEMPLATE_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_CHOOSE_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_COMMENT_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_COPY_OF_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_COPY_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_DISPLAYIF_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_ELEMENT_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_FALLBACK_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_FOREACH_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_HTML_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_IF_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_IMPORT_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_INCLUDE_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_KEY_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_MESSAGE_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_NUMBER_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_OTHERWISE_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_PARAM_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_PI_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_PRESERVESPACE_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_SORT_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_STRIPSPACE_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_TEMPLATE_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_TEXT_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_VALUEOF_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_VARIABLE_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_WHEN_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString       ELEMNAME_WITHPARAM_WITH_PREFIX_STRING(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString       PSEUDONAME_NODE(XalanMemMgrs::getDummyMemMgr());



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString&       Constants::ATTRNAME_CASEORDER = ::ATTRNAME_CASEORDER;
const XalanDOMString&       Constants::ATTRNAME_COUNT = ::ATTRNAME_COUNT;
const XalanDOMString&       Constants::ATTRNAME_DATATYPE = ::ATTRNAME_DATATYPE;
const XalanDOMString&       Constants::ATTRNAME_DECIMALSEPARATOR = ::ATTRNAME_DECIMALSEPARATOR;
const XalanDOMString&       Constants::ATTRNAME_DEFAULT = ::ATTRNAME_DEFAULT;
const XalanDOMString&       Constants::ATTRNAME_DEFAULTSPACE = ::ATTRNAME_DEFAULTSPACE;
const XalanDOMString&       Constants::ATTRNAME_DIGIT = ::ATTRNAME_DIGIT;
const XalanDOMString&       Constants::ATTRNAME_DIGITGROUPSEP = ::ATTRNAME_DIGITGROUPSEP;
const XalanDOMString&       Constants::ATTRNAME_DISABLE_OUTPUT_ESCAPING  = ::ATTRNAME_DISABLE_OUTPUT_ESCAPING;
const XalanDOMString&       Constants::ATTRNAME_ELEMENTS = ::ATTRNAME_ELEMENTS;
const XalanDOMString&       Constants::ATTRNAME_ESCAPE_URLS = ::ATTRNAME_ESCAPE_URLS;
const XalanDOMString&       Constants::ATTRNAME_EXCLUDE_RESULT_PREFIXES = ::ATTRNAME_EXCLUDE_RESULT_PREFIXES;
const XalanDOMString&       Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES = ::ATTRNAME_EXTENSIONELEMENTPREFIXES;
const XalanDOMString&       Constants::ATTRNAME_FORMAT = ::ATTRNAME_FORMAT;
const XalanDOMString&       Constants::ATTRNAME_FROM = ::ATTRNAME_FROM;
const XalanDOMString&       Constants::ATTRNAME_GROUPINGSEPARATOR = ::ATTRNAME_GROUPINGSEPARATOR;
const XalanDOMString&       Constants::ATTRNAME_GROUPINGSIZE = ::ATTRNAME_GROUPINGSIZE;
const XalanDOMString&       Constants::ATTRNAME_HREF = ::ATTRNAME_HREF;
const XalanDOMString&       Constants::ATTRNAME_ID = ::ATTRNAME_ID;
const XalanDOMString&       Constants::ATTRNAME_INDENTAMOUNT = ::ATTRNAME_INDENTAMOUNT;
const XalanDOMString&       Constants::ATTRNAME_INDENTRESULT = ::ATTRNAME_INDENTRESULT;
const XalanDOMString&       Constants::ATTRNAME_INFINITY = ::ATTRNAME_INFINITY;
const XalanDOMString&       Constants::ATTRNAME_LANG = ::ATTRNAME_LANG;
const XalanDOMString&       Constants::ATTRNAME_LETTERVALUE = ::ATTRNAME_LETTERVALUE;
const XalanDOMString&       Constants::ATTRNAME_MATCH = ::ATTRNAME_MATCH;
const XalanDOMString&       Constants::ATTRNAME_MINUSSIGN = ::ATTRNAME_MINUSSIGN;
const XalanDOMString&       Constants::ATTRNAME_MODE = ::ATTRNAME_MODE;
const XalanDOMString&       Constants::ATTRNAME_NAME = ::ATTRNAME_NAME;
const XalanDOMString&       Constants::ATTRNAME_NAMESPACE = ::ATTRNAME_NAMESPACE;
const XalanDOMString&       Constants::ATTRNAME_NAN = ::ATTRNAME_NAN;
const XalanDOMString&       Constants::ATTRNAME_NDIGITSPERGROUP = ::ATTRNAME_NDIGITSPERGROUP;
const XalanDOMString&       Constants::ATTRNAME_OMIT_META_TAG = ::ATTRNAME_OMIT_META_TAG;
const XalanDOMString&       Constants::ATTRNAME_ORDER = ::ATTRNAME_ORDER;
const XalanDOMString&       Constants::ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS  = ::ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS;
const XalanDOMString&       Constants::ATTRNAME_OUTPUT_DOCTYPE_PUBLIC = ::ATTRNAME_OUTPUT_DOCTYPE_PUBLIC;
const XalanDOMString&       Constants::ATTRNAME_OUTPUT_DOCTYPE_SYSTEM = ::ATTRNAME_OUTPUT_DOCTYPE_SYSTEM;
const XalanDOMString&       Constants::ATTRNAME_OUTPUT_ENCODING = ::ATTRNAME_OUTPUT_ENCODING;
const XalanDOMString&       Constants::ATTRNAME_OUTPUT_INDENT = ::ATTRNAME_OUTPUT_INDENT;
const XalanDOMString&       Constants::ATTRNAME_OUTPUT_MEDIATYPE = ::ATTRNAME_OUTPUT_MEDIATYPE;
const XalanDOMString&       Constants::ATTRNAME_OUTPUT_METHOD = ::ATTRNAME_OUTPUT_METHOD;
const XalanDOMString&       Constants::ATTRNAME_OUTPUT_OMITXMLDECL = ::ATTRNAME_OUTPUT_OMITXMLDECL;
const XalanDOMString&       Constants::ATTRNAME_OUTPUT_STANDALONE  = ::ATTRNAME_OUTPUT_STANDALONE;
const XalanDOMString&       Constants::ATTRNAME_OUTPUT_VERSION = ::ATTRNAME_OUTPUT_VERSION;
const XalanDOMString&       Constants::ATTRNAME_PATTERNSEPARATOR = ::ATTRNAME_PATTERNSEPARATOR;
const XalanDOMString&       Constants::ATTRNAME_PERCENT = ::ATTRNAME_PERCENT;
const XalanDOMString&       Constants::ATTRNAME_PERMILLE = ::ATTRNAME_PERMILLE;
const XalanDOMString&       Constants::ATTRNAME_PREFIX = ::ATTRNAME_PREFIX;
const XalanDOMString&       Constants::ATTRNAME_PRIORITY = ::ATTRNAME_PRIORITY;
const XalanDOMString&       Constants::ATTRNAME_RESULTNS = ::ATTRNAME_RESULTNS;
const XalanDOMString&       Constants::ATTRNAME_RESULT_PREFIX = ::ATTRNAME_RESULT_PREFIX;
const XalanDOMString&       Constants::ATTRNAME_SELECT = ::ATTRNAME_SELECT;
const XalanDOMString&       Constants::ATTRNAME_STYLESHEET_PREFIX = ::ATTRNAME_STYLESHEET_PREFIX;
const XalanDOMString&       Constants::ATTRNAME_TERMINATE = ::ATTRNAME_TERMINATE;
const XalanDOMString&       Constants::ATTRNAME_TEST = ::ATTRNAME_TEST;
const XalanDOMString&       Constants::ATTRNAME_TYPE = ::ATTRNAME_TYPE;
const XalanDOMString&       Constants::ATTRNAME_USE = ::ATTRNAME_USE;
const XalanDOMString&       Constants::ATTRNAME_USEATTRIBUTESETS = ::ATTRNAME_USEATTRIBUTESETS;
const XalanDOMString&       Constants::ATTRNAME_VALUE = ::ATTRNAME_VALUE;
const XalanDOMString&       Constants::ATTRNAME_VERSION = ::ATTRNAME_VERSION;
const XalanDOMString&       Constants::ATTRNAME_SPACE = ::ATTRNAME_SPACE;
const XalanDOMString&       Constants::ATTRNAME_XMLSPACE = ::ATTRNAME_XMLSPACE;
const XalanDOMString&       Constants::ATTRNAME_ZERODIGIT = ::ATTRNAME_ZERODIGIT;

const XalanDOMString&       Constants::ATTRTYPE_CDATA = ::ATTRTYPE_CDATA;

const XalanDOMString&       Constants::ATTRVAL_CASEORDER_LOWER = ::ATTRVAL_CASEORDER_LOWER;
const XalanDOMString&       Constants::ATTRVAL_CASEORDER_UPPER = ::ATTRVAL_CASEORDER_UPPER;
const XalanDOMString&       Constants::ATTRVAL_DATATYPE_NUMBER = ::ATTRVAL_DATATYPE_NUMBER;
const XalanDOMString&       Constants::ATTRVAL_DATATYPE_TEXT = ::ATTRVAL_DATATYPE_TEXT;
const XalanDOMString&       Constants::ATTRVAL_DEFAULT = ::ATTRVAL_DEFAULT;
const XalanDOMString&       Constants::ATTRVAL_DEFAULT_PREFIX = ::ATTRVAL_DEFAULT_PREFIX;
const XalanDOMString&       Constants::ATTRVAL_DEFAULT_TEXT_RULE = ::ATTRVAL_DEFAULT_TEXT_RULE;
const XalanDOMString&       Constants::ATTRVAL_INFINITY = ::ATTRVAL_INFINITY;
const XalanDOMString&       Constants::ATTRVAL_NAN = ::ATTRVAL_NAN;
const XalanDOMString&       Constants::ATTRVAL_NO = ::ATTRVAL_NO;
const XalanDOMString&       Constants::ATTRVAL_ORDER_ASCENDING = ::ATTRVAL_ORDER_ASCENDING;
const XalanDOMString&       Constants::ATTRVAL_ORDER_DESCENDING = ::ATTRVAL_ORDER_DESCENDING;
const XalanDOMString&       Constants::ATTRVAL_OUTPUT_METHOD_HTML = ::ATTRVAL_OUTPUT_METHOD_HTML;
const XalanDOMString&       Constants::ATTRVAL_OUTPUT_METHOD_TEXT = ::ATTRVAL_OUTPUT_METHOD_TEXT;
const XalanDOMString&       Constants::ATTRVAL_OUTPUT_METHOD_XML = ::ATTRVAL_OUTPUT_METHOD_XML;
const XalanDOMString&       Constants::ATTRVAL_PRESERVE = ::ATTRVAL_PRESERVE;
const XalanDOMString&       Constants::ATTRVAL_SIMPLE = ::ATTRVAL_SIMPLE;
const XalanDOMString&       Constants::ATTRVAL_STRIP = ::ATTRVAL_STRIP;
const XalanDOMString&       Constants::ATTRVAL_THIS = ::ATTRVAL_THIS;
const XalanDOMString&       Constants::ATTRVAL_YES = ::ATTRVAL_YES;

const XalanDOMString&       Constants::DEFAULT_WHITESPACE_SEPARATOR_STRING = ::DEFAULT_WHITESPACE_SEPARATOR_STRING;

const XalanDOMString&       Constants::ELEMNAME_ANY_STRING = ::ELEMNAME_ANY_STRING;
const XalanDOMString&       Constants::ELEMNAME_APPLY_IMPORTS_WITH_PREFIX_STRING = ::ELEMNAME_APPLY_IMPORTS_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_APPLY_TEMPLATES_WITH_PREFIX_STRING = ::ELEMNAME_APPLY_TEMPLATES_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING = ::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_ATTRIBUTE_WITH_PREFIX_STRING = ::ELEMNAME_ATTRIBUTE_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_CALLTEMPLATE_WITH_PREFIX_STRING = ::ELEMNAME_CALLTEMPLATE_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_CHOOSE_WITH_PREFIX_STRING = ::ELEMNAME_CHOOSE_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_COMMENT_WITH_PREFIX_STRING = ::ELEMNAME_COMMENT_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_COPY_OF_WITH_PREFIX_STRING = ::ELEMNAME_COPY_OF_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_COPY_WITH_PREFIX_STRING = ::ELEMNAME_COPY_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING = ::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_ELEMENT_WITH_PREFIX_STRING = ::ELEMNAME_ELEMENT_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_FALLBACK_WITH_PREFIX_STRING = ::ELEMNAME_FALLBACK_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_FOREACH_WITH_PREFIX_STRING = ::ELEMNAME_FOREACH_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_HTML_STRING = ::ELEMNAME_HTML_STRING;
const XalanDOMString&       Constants::ELEMNAME_IF_WITH_PREFIX_STRING = ::ELEMNAME_IF_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_IMPORT_WITH_PREFIX_STRING = ::ELEMNAME_IMPORT_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_INCLUDE_WITH_PREFIX_STRING = ::ELEMNAME_INCLUDE_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_KEY_WITH_PREFIX_STRING = ::ELEMNAME_KEY_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_MESSAGE_WITH_PREFIX_STRING = ::ELEMNAME_MESSAGE_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_NUMBER_WITH_PREFIX_STRING = ::ELEMNAME_NUMBER_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_OTHERWISE_WITH_PREFIX_STRING = ::ELEMNAME_OTHERWISE_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_PARAM_WITH_PREFIX_STRING = ::ELEMNAME_PARAM_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_PI_WITH_PREFIX_STRING = ::ELEMNAME_PI_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_PRESERVESPACE_WITH_PREFIX_STRING = ::ELEMNAME_PRESERVESPACE_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_SORT_WITH_PREFIX_STRING = ::ELEMNAME_SORT_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_STRIPSPACE_WITH_PREFIX_STRING = ::ELEMNAME_STRIPSPACE_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_TEMPLATE_WITH_PREFIX_STRING = ::ELEMNAME_TEMPLATE_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_TEXT_WITH_PREFIX_STRING = ::ELEMNAME_TEXT_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_VALUEOF_WITH_PREFIX_STRING = ::ELEMNAME_VALUEOF_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_VARIABLE_WITH_PREFIX_STRING = ::ELEMNAME_VARIABLE_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_WHEN_WITH_PREFIX_STRING = ::ELEMNAME_WHEN_WITH_PREFIX_STRING;
const XalanDOMString&       Constants::ELEMNAME_WITHPARAM_WITH_PREFIX_STRING = ::ELEMNAME_WITHPARAM_WITH_PREFIX_STRING;

const XalanDOMString&       Constants::PSEUDONAME_NODE = ::PSEUDONAME_NODE;



void
Constants::initialize(MemoryManager&  theManager)
{

    ::ATTRNAME_CASEORDER.reset( theManager, "case-order");
    ::ATTRNAME_COUNT.reset( theManager, "count");
    ::ATTRNAME_DATATYPE.reset( theManager, "data-type");
    ::ATTRNAME_DECIMALSEPARATOR.reset( theManager, "decimal-separator");
    ::ATTRNAME_DEFAULT.reset( theManager, "default");
    ::ATTRNAME_DEFAULTSPACE.reset( theManager, "default-space");
    ::ATTRNAME_DIGIT.reset( theManager, "digit");
    ::ATTRNAME_DIGITGROUPSEP.reset( theManager, "digit-group-sep");
    ::ATTRNAME_DISABLE_OUTPUT_ESCAPING .reset( theManager, "disable-output-escaping");
    ::ATTRNAME_ELEMENTS.reset( theManager, "elements");
    ::ATTRNAME_ESCAPE_URLS.reset( theManager, "use-url-escaping");
    ::ATTRNAME_EXCLUDE_RESULT_PREFIXES.reset( theManager, "exclude-result-prefixes");
    ::ATTRNAME_EXTENSIONELEMENTPREFIXES.reset( theManager, "extension-element-prefixes");
    ::ATTRNAME_FORMAT.reset( theManager, "format");
    ::ATTRNAME_FROM.reset( theManager, "from");
    ::ATTRNAME_GROUPINGSEPARATOR.reset( theManager, "grouping-separator");
    ::ATTRNAME_GROUPINGSIZE.reset( theManager, "grouping-size");
    ::ATTRNAME_HREF.reset( theManager, "href");
    ::ATTRNAME_ID.reset( theManager, "id");
    ::ATTRNAME_INDENTAMOUNT.reset( theManager, "indent-amount");
    ::ATTRNAME_INDENTRESULT.reset( theManager, "indent-result");
    ::ATTRNAME_INFINITY.reset( theManager, "infinity");
    ::ATTRNAME_LANG.reset( theManager, "lang");
    ::ATTRNAME_LETTERVALUE.reset( theManager, "letter-value");
    ::ATTRNAME_MATCH.reset( theManager, "match");
    ::ATTRNAME_MINUSSIGN.reset( theManager, "minus-sign");
    ::ATTRNAME_MODE.reset( theManager, "mode");
    ::ATTRNAME_NAME.reset( theManager, "name");
    ::ATTRNAME_NAMESPACE.reset( theManager, "namespace");
    ::ATTRNAME_NAN.reset( theManager, "NaN");
    ::ATTRNAME_NDIGITSPERGROUP.reset( theManager, "n-digits-per-group");
    ::ATTRNAME_OMIT_META_TAG.reset( theManager, "omit-meta-tag");
    ::ATTRNAME_ORDER.reset( theManager, "order");
    ::ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS .reset( theManager, "cdata-section-elements");
    ::ATTRNAME_OUTPUT_DOCTYPE_PUBLIC.reset( theManager, "doctype-public");
    ::ATTRNAME_OUTPUT_DOCTYPE_SYSTEM.reset( theManager, "doctype-system");
    ::ATTRNAME_OUTPUT_ENCODING.reset( theManager, "encoding");
    ::ATTRNAME_OUTPUT_INDENT.reset( theManager, "indent");
    ::ATTRNAME_OUTPUT_MEDIATYPE.reset( theManager, "media-type");
    ::ATTRNAME_OUTPUT_METHOD.reset( theManager, "method");
    ::ATTRNAME_OUTPUT_OMITXMLDECL.reset( theManager, "omit-xml-declaration");
    ::ATTRNAME_OUTPUT_STANDALONE .reset( theManager, "standalone");
    ::ATTRNAME_OUTPUT_VERSION.reset( theManager, "version");
    ::ATTRNAME_PATTERNSEPARATOR.reset( theManager, "pattern-separator");
    ::ATTRNAME_PERCENT.reset( theManager, "percent");
    ::ATTRNAME_PERMILLE.reset( theManager, "per-mille");
    ::ATTRNAME_PREFIX.reset( theManager, "prefix");
    ::ATTRNAME_PRIORITY.reset( theManager, "priority");
    ::ATTRNAME_RESULTNS.reset( theManager, "result-ns");
    ::ATTRNAME_RESULT_PREFIX.reset( theManager, "result-prefix");
    ::ATTRNAME_SELECT.reset( theManager, "select");
    ::ATTRNAME_STYLESHEET_PREFIX.reset( theManager, "stylesheet-prefix");
    ::ATTRNAME_TERMINATE.reset( theManager, "terminate");
    ::ATTRNAME_TEST.reset( theManager, "test");
    ::ATTRNAME_TYPE.reset( theManager, "type");
    ::ATTRNAME_USE.reset( theManager, "use");
    ::ATTRNAME_USEATTRIBUTESETS.reset( theManager, "use-attribute-sets");
    ::ATTRNAME_VALUE.reset( theManager, "value");
    ::ATTRNAME_VERSION.reset( theManager, "version");
    ::ATTRNAME_SPACE.reset( theManager, "space");
    ::ATTRNAME_XMLSPACE.reset( theManager, "xml:space");
    ::ATTRNAME_ZERODIGIT.reset( theManager, "zero-digit");

    ::ATTRTYPE_CDATA.reset( theManager, "CDATA");

    ::ATTRVAL_CASEORDER_LOWER.reset( theManager, "lower-first");
    ::ATTRVAL_CASEORDER_UPPER.reset( theManager, "upper-first");
    ::ATTRVAL_DATATYPE_NUMBER.reset( theManager, "number");
    ::ATTRVAL_DATATYPE_TEXT.reset( theManager, "text");
    ::ATTRVAL_DEFAULT.reset( theManager, "default");
    ::ATTRVAL_DEFAULT_PREFIX.reset( theManager, "#default");
    ::ATTRVAL_DEFAULT_TEXT_RULE.reset( theManager, "text() | @*");
    ::ATTRVAL_INFINITY.reset( theManager, "Infinity");
    ::ATTRVAL_NAN.reset( theManager, "NaN");
    ::ATTRVAL_NO.reset( theManager, "no");
    ::ATTRVAL_ORDER_ASCENDING.reset( theManager, "ascending");
    ::ATTRVAL_ORDER_DESCENDING.reset( theManager, "descending");
    ::ATTRVAL_OUTPUT_METHOD_HTML.reset( theManager, "html");
    ::ATTRVAL_OUTPUT_METHOD_TEXT.reset( theManager, "text");
    ::ATTRVAL_OUTPUT_METHOD_XML.reset( theManager, "xml");
    ::ATTRVAL_PRESERVE.reset( theManager, "preserve");
    ::ATTRVAL_SIMPLE.reset( theManager, "simple");
    ::ATTRVAL_STRIP.reset( theManager, "strip");
    ::ATTRVAL_THIS.reset( theManager, ".");
    ::ATTRVAL_YES.reset( theManager, "yes");

    ::DEFAULT_WHITESPACE_SEPARATOR_STRING.reset( theManager, " \t\n\r");

    ::ELEMNAME_ANY_STRING.reset( theManager, "any");
    ::ELEMNAME_APPLY_IMPORTS_WITH_PREFIX_STRING.reset( theManager, "xsl:apply-imports");
    ::ELEMNAME_APPLY_TEMPLATES_WITH_PREFIX_STRING.reset( theManager, "xsl:apply-templates");
    ::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING.reset( theManager, "xsl:attribute-set");
    ::ELEMNAME_ATTRIBUTE_WITH_PREFIX_STRING.reset( theManager, "xsl:attribute");
    ::ELEMNAME_CALLTEMPLATE_WITH_PREFIX_STRING.reset( theManager, "xsl:call-template");
    ::ELEMNAME_CHOOSE_WITH_PREFIX_STRING.reset( theManager, "xsl:choose");
    ::ELEMNAME_COMMENT_WITH_PREFIX_STRING.reset( theManager, "xsl:comment");
    ::ELEMNAME_COPY_OF_WITH_PREFIX_STRING.reset( theManager, "xsl:copy-of");
    ::ELEMNAME_COPY_WITH_PREFIX_STRING.reset( theManager, "xsl:copy");
    ::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING.reset( theManager, "xsl:decimal-format");
    ::ELEMNAME_DISPLAYIF_STRING.reset( theManager, "display-if");
    ::ELEMNAME_ELEMENT_WITH_PREFIX_STRING.reset( theManager, "xsl:element");
    ::ELEMNAME_FALLBACK_WITH_PREFIX_STRING.reset( theManager, "xsl:fallback");
    ::ELEMNAME_FOREACH_WITH_PREFIX_STRING.reset( theManager, "xsl:for-each");
    ::ELEMNAME_HTML_STRING.reset( theManager, "HTML");
    ::ELEMNAME_IF_WITH_PREFIX_STRING.reset( theManager, "xsl:if");
    ::ELEMNAME_IMPORT_WITH_PREFIX_STRING.reset( theManager, "xsl:import");
    ::ELEMNAME_INCLUDE_WITH_PREFIX_STRING.reset( theManager, "xsl:include");
    ::ELEMNAME_KEY_WITH_PREFIX_STRING.reset( theManager, "xsl:key");
    ::ELEMNAME_MESSAGE_WITH_PREFIX_STRING.reset( theManager, "xsl:message");
    ::ELEMNAME_NUMBER_WITH_PREFIX_STRING.reset( theManager, "xsl:number");
    ::ELEMNAME_OTHERWISE_WITH_PREFIX_STRING.reset( theManager, "xsl:otherwise");
    ::ELEMNAME_PARAM_WITH_PREFIX_STRING.reset( theManager, "xsl:param");
    ::ELEMNAME_PI_WITH_PREFIX_STRING.reset( theManager, "xsl:processing-instruction");
    ::ELEMNAME_PRESERVESPACE_WITH_PREFIX_STRING.reset( theManager, "xsl:preserve-space");
    ::ELEMNAME_SORT_WITH_PREFIX_STRING.reset( theManager, "xsl:sort");
    ::ELEMNAME_STRIPSPACE_WITH_PREFIX_STRING.reset( theManager, "xsl:strip-space");
    ::ELEMNAME_TEMPLATE_WITH_PREFIX_STRING.reset( theManager, "xsl:template");
    ::ELEMNAME_TEXT_WITH_PREFIX_STRING.reset( theManager, "xsl:text");
    ::ELEMNAME_VALUEOF_WITH_PREFIX_STRING.reset( theManager, "xsl:value-of");
    ::ELEMNAME_VARIABLE_WITH_PREFIX_STRING.reset( theManager, "xsl:variable");
    ::ELEMNAME_WHEN_WITH_PREFIX_STRING.reset( theManager, "xsl:when");
    ::ELEMNAME_WITHPARAM_WITH_PREFIX_STRING.reset( theManager, "xsl:with-param");

    ::PSEUDONAME_NODE.reset( theManager, "node()");
}



void
Constants::terminate()
{
    MemoryManager& theManager = XalanMemMgrs::getDummyMemMgr();

    releaseMemory(::ATTRNAME_CASEORDER, theManager );
    releaseMemory(::ATTRNAME_COUNT, theManager );
    releaseMemory(::ATTRNAME_DATATYPE, theManager );
    releaseMemory(::ATTRNAME_DECIMALSEPARATOR, theManager );
    releaseMemory(::ATTRNAME_DEFAULT, theManager );
    releaseMemory(::ATTRNAME_DEFAULTSPACE, theManager );
    releaseMemory(::ATTRNAME_DIGIT, theManager );
    releaseMemory(::ATTRNAME_DIGITGROUPSEP, theManager );
    releaseMemory(::ATTRNAME_DISABLE_OUTPUT_ESCAPING, theManager );
    releaseMemory(::ATTRNAME_ELEMENTS, theManager );
    releaseMemory(::ATTRNAME_ESCAPE_URLS, theManager );
    releaseMemory(::ATTRNAME_EXCLUDE_RESULT_PREFIXES, theManager );
    releaseMemory(::ATTRNAME_EXTENSIONELEMENTPREFIXES, theManager );
    releaseMemory(::ATTRNAME_FORMAT, theManager );
    releaseMemory(::ATTRNAME_FROM, theManager );
    releaseMemory(::ATTRNAME_GROUPINGSEPARATOR, theManager );
    releaseMemory(::ATTRNAME_GROUPINGSIZE, theManager );
    releaseMemory(::ATTRNAME_HREF, theManager );
    releaseMemory(::ATTRNAME_ID, theManager );
    releaseMemory(::ATTRNAME_INDENTAMOUNT, theManager );
    releaseMemory(::ATTRNAME_INDENTRESULT, theManager );
    releaseMemory(::ATTRNAME_INFINITY, theManager );
    releaseMemory(::ATTRNAME_LANG, theManager );
    releaseMemory(::ATTRNAME_LETTERVALUE, theManager );
    releaseMemory(::ATTRNAME_MATCH, theManager );
    releaseMemory(::ATTRNAME_MINUSSIGN, theManager );
    releaseMemory(::ATTRNAME_MODE, theManager );
    releaseMemory(::ATTRNAME_NAME, theManager );
    releaseMemory(::ATTRNAME_NAMESPACE, theManager );
    releaseMemory(::ATTRNAME_NAN, theManager );
    releaseMemory(::ATTRNAME_NDIGITSPERGROUP, theManager );
    releaseMemory(::ATTRNAME_OMIT_META_TAG, theManager );
    releaseMemory(::ATTRNAME_ORDER, theManager );
    releaseMemory(::ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS, theManager );
    releaseMemory(::ATTRNAME_OUTPUT_DOCTYPE_PUBLIC, theManager );
    releaseMemory(::ATTRNAME_OUTPUT_DOCTYPE_SYSTEM, theManager );
    releaseMemory(::ATTRNAME_OUTPUT_ENCODING, theManager );
    releaseMemory(::ATTRNAME_OUTPUT_INDENT, theManager );
    releaseMemory(::ATTRNAME_OUTPUT_MEDIATYPE, theManager );
    releaseMemory(::ATTRNAME_OUTPUT_METHOD, theManager );
    releaseMemory(::ATTRNAME_OUTPUT_OMITXMLDECL, theManager );
    releaseMemory(::ATTRNAME_OUTPUT_STANDALONE, theManager );
    releaseMemory(::ATTRNAME_OUTPUT_VERSION, theManager );
    releaseMemory(::ATTRNAME_PATTERNSEPARATOR, theManager );
    releaseMemory(::ATTRNAME_PERCENT, theManager );
    releaseMemory(::ATTRNAME_PERMILLE, theManager );
    releaseMemory(::ATTRNAME_PREFIX, theManager );
    releaseMemory(::ATTRNAME_PRIORITY, theManager );
    releaseMemory(::ATTRNAME_RESULTNS, theManager );
    releaseMemory(::ATTRNAME_RESULT_PREFIX, theManager );
    releaseMemory(::ATTRNAME_SELECT, theManager );
    releaseMemory(::ATTRNAME_STYLESHEET_PREFIX, theManager );
    releaseMemory(::ATTRNAME_TERMINATE, theManager );
    releaseMemory(::ATTRNAME_TEST, theManager );
    releaseMemory(::ATTRNAME_TYPE, theManager );
    releaseMemory(::ATTRNAME_USE, theManager );
    releaseMemory(::ATTRNAME_USEATTRIBUTESETS, theManager );
    releaseMemory(::ATTRNAME_VALUE, theManager );
    releaseMemory(::ATTRNAME_VERSION, theManager );
    releaseMemory(::ATTRNAME_SPACE, theManager );
    releaseMemory(::ATTRNAME_XMLSPACE, theManager );
    releaseMemory(::ATTRNAME_ZERODIGIT, theManager );

    releaseMemory(::ATTRTYPE_CDATA, theManager );

    releaseMemory(::ATTRVAL_CASEORDER_LOWER, theManager );
    releaseMemory(::ATTRVAL_CASEORDER_UPPER, theManager );
    releaseMemory(::ATTRVAL_DATATYPE_NUMBER, theManager );
    releaseMemory(::ATTRVAL_DATATYPE_TEXT, theManager );
    releaseMemory(::ATTRVAL_DEFAULT, theManager );
    releaseMemory(::ATTRVAL_DEFAULT_PREFIX, theManager );
    releaseMemory(::ATTRVAL_DEFAULT_TEXT_RULE, theManager );
    releaseMemory(::ATTRVAL_INFINITY, theManager );
    releaseMemory(::ATTRVAL_NAN, theManager );
    releaseMemory(::ATTRVAL_NO, theManager );
    releaseMemory(::ATTRVAL_ORDER_ASCENDING, theManager );
    releaseMemory(::ATTRVAL_ORDER_DESCENDING, theManager );
    releaseMemory(::ATTRVAL_OUTPUT_METHOD_HTML, theManager );
    releaseMemory(::ATTRVAL_OUTPUT_METHOD_TEXT, theManager );
    releaseMemory(::ATTRVAL_OUTPUT_METHOD_XML, theManager );
    releaseMemory(::ATTRVAL_PRESERVE, theManager );
    releaseMemory(::ATTRVAL_SIMPLE, theManager );
    releaseMemory(::ATTRVAL_STRIP, theManager );
    releaseMemory(::ATTRVAL_THIS, theManager );
    releaseMemory(::ATTRVAL_YES, theManager );

    releaseMemory(::DEFAULT_WHITESPACE_SEPARATOR_STRING, theManager );

    releaseMemory(::ELEMNAME_ANY_STRING, theManager );
    releaseMemory(::ELEMNAME_APPLY_IMPORTS_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_APPLY_TEMPLATES_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_ATTRIBUTE_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_CALLTEMPLATE_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_CHOOSE_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_COMMENT_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_COPY_OF_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_COPY_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_DISPLAYIF_STRING, theManager );
    releaseMemory(::ELEMNAME_ELEMENT_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_FALLBACK_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_FOREACH_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_HTML_STRING, theManager );
    releaseMemory(::ELEMNAME_IF_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_IMPORT_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_INCLUDE_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_KEY_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_MESSAGE_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_NUMBER_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_OTHERWISE_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_PARAM_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_PI_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_PRESERVESPACE_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_SORT_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_STRIPSPACE_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_TEMPLATE_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_TEXT_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_VALUEOF_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_VARIABLE_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_WHEN_WITH_PREFIX_STRING, theManager );
    releaseMemory(::ELEMNAME_WITHPARAM_WITH_PREFIX_STRING, theManager );

    releaseMemory(::PSEUDONAME_NODE, theManager );
}



XALAN_CPP_NAMESPACE_END
