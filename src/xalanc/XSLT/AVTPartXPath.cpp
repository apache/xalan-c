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
#include "AVTPartXPath.hpp"



#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathFactory.hpp>
#include <xalanc/XPath/XPathProcessor.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * Construct a simple AVT part.
 * @param val A pure string section of an AVT.
 */
AVTPartXPath::AVTPartXPath(const XPath*     xpath) :
    AVTPart(),
    m_pXPath(xpath)
{
}



void
AVTPartXPath::evaluate(
            XalanDOMString&         buf,
            XalanNode*              contextNode,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext) const
{
    m_pXPath->execute(contextNode, prefixResolver, executionContext, buf);
}



void
AVTPartXPath::evaluate(
            XalanDOMString&         buf,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext) const
{
    m_pXPath->execute(prefixResolver, executionContext, buf);
}



XALAN_CPP_NAMESPACE_END
