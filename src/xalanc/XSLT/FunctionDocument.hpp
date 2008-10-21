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
#if !defined(FUNCTIONDOCUMENT_HEADER_GUARD_1357924680)
#define FUNCTIONDOCUMENT_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XPath/Function.hpp>



XALAN_CPP_NAMESPACE_BEGIN



// Implementation of the XSLT function document().
//
class XALAN_XSLT_EXPORT FunctionDocument : public Function
{
public:

    typedef Function    ParentType;

    FunctionDocument();

    virtual
    ~FunctionDocument();

    // These methods are inherited from Function ...

    virtual XObjectPtr
    execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XObjectPtr        arg,
            const Locator*          locator) const;

    virtual XObjectPtr
    execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const Locator*          locator) const;

    using ParentType::execute;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    virtual Function*
#else
    virtual FunctionDocument*
#endif
    clone(MemoryManager& theManager) const;

protected:

    virtual const XalanDOMString&
    getError(XalanDOMString& theResult) const;

private:

    XObjectPtr
    doExecute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,            
            const XObjectPtr&       arg,
            XalanDOMString*         base,
            int                     argCount,
            const Locator*          locator,
            bool                    fNoRelativeURI = false) const;

    // Not implemented...
    FunctionDocument&
    operator=(const FunctionDocument&);

    bool
    operator==(const FunctionDocument&) const;
};



XALAN_CPP_NAMESPACE_END



#endif  // FUNCTIONDOCUMENT_HEADER_GUARD_1357924680
