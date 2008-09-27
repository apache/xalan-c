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
#if !defined(XALAN_AVTPARTSIMPLE_HEADER_GUARD)
#define XALAN_AVTPARTSIMPLE_HEADER_GUARD 



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include "AVTPart.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class StylesheetConstructionContext;



/**
 * Simple string part of a complex AVT.
 */
class AVTPartSimple: public AVTPart
{
public:

    /**
     * Construct a simple Attribute Value Template (AVT) part.
     *
     * @param constructionContext  context when object constructed
     * @param val A pure string section of an AVT
     * @param len The length of val
     */
    AVTPartSimple(
            StylesheetConstructionContext&  constructionContext,
            const XalanDOMChar*             val,
            XalanDOMString::size_type       len);


    // These methods are inherited from AVTPart ...

    virtual void
    evaluate(
            XalanDOMString&         buf,
            XalanNode*              contextNode,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext) const;

    virtual void
    evaluate(
            XalanDOMString&         buf,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext) const;

private:

    /**
     * Simple string value;
     */
    const XalanDOMChar* const           m_val;

    const XalanDOMString::size_type     m_len;
};



XALAN_CPP_NAMESPACE_END



#endif  //XALAN_AVTPARTSIMPLE_HEADER_GUARD 
