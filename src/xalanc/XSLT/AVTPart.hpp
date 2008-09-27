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
#if !defined(XALAN_AVTPART_HEADER_GUARD)
#define XALAN_AVTPART_HEADER_GUARD 



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class XalanDOMString;
class XalanNode;
class PrefixResolver;
class XPathExecutionContext;



/**
 * Class to hold a part, either a string or XPath, 
 * of an Attribute Value Template.
 */
class AVTPart
{
public:

    AVTPart();

    virtual
    ~AVTPart();

    /**
     * Append the value to the buffer.
     *
     * @param buf              buffer to write into
     * @param contextNode      current context node
     * @param prefixResolver   prefix resolver to use
     * @param executionContext execution context
     */
    virtual void
    evaluate(
            XalanDOMString&         buf,
            XalanNode*              contextNode,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext) const = 0;

    /**
     * Append the value to the buffer.
     *
     * @param buf              buffer to write into
     * @param prefixResolver   prefix resolver to use
     * @param executionContext execution context
     */
    virtual void
    evaluate(
            XalanDOMString&         buf,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext) const = 0;
};



XALAN_CPP_NAMESPACE_END



#endif //XALAN_AVTPART_HEADER_GUARD 
