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
#if !defined(FUNCTIONSUBSTRINGBEFORE_HEADER_GUARD_1357924680)
#define FUNCTIONSUBSTRINGBEFORE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file...
#include <xalanc/XPath/Function.hpp>



namespace XALAN_CPP_NAMESPACE {



/**
 * XPath implementation of "substring-before" function.
 */
class XALAN_XPATH_EXPORT FunctionSubstringBefore : public Function
{
public:

    typedef Function    ParentType;

    FunctionSubstringBefore();

    virtual
    ~FunctionSubstringBefore();

    // These methods are inherited from Function ...

    virtual XObjectPtr
    execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const Locator*          locator) const;

    using ParentType::execute;

    virtual FunctionSubstringBefore*
    clone(MemoryManager& theManager) const;

protected:
    virtual const XalanDOMString&
    getError(XalanDOMString& theResult) const;

private:

    // Not implemented...
    FunctionSubstringBefore&
    operator=(const FunctionSubstringBefore&);

    bool
    operator==(const FunctionSubstringBefore&) const;
};



}



#endif  // FUNCTIONSUBSTRINGBEFORE_HEADER_GUARD_1357924680
