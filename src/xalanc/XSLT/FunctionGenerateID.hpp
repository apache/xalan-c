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
#if !defined(FUNCTIONGENERATEID_HEADER_GUARD_1357924680)
#define FUNCTIONGENERATEID_HEADER_GUARD_1357924680

// Base header file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XPath/Function.hpp>



XALAN_CPP_NAMESPACE_BEGIN



// Implementation of the XSLT function generate-id().
//
class XALAN_XSLT_EXPORT FunctionGenerateID : public Function
{
public:

    typedef Function    ParentType;

    FunctionGenerateID();

    virtual
    ~FunctionGenerateID();

    // These methods are inherited from Function ...
    
    using ParentType::execute;

    virtual XObjectPtr
    execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const Locator*          locator) const;

    virtual XObjectPtr
    execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XObjectPtr        arg1,
            const Locator*          locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    virtual Function*
#else
    virtual FunctionGenerateID*
#endif
    clone(MemoryManager& theManager) const;

protected:

    virtual const XalanDOMString&
    getError(XalanDOMString& theResult) const;

private:

    // Not implemented...
    FunctionGenerateID& operator=(const FunctionGenerateID&);

    bool
    operator==(const FunctionGenerateID&) const;

    static const XalanDOMString         s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif  // FUNCTIONGENERATEID_HEADER_GUARD_1357924680
