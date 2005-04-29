/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#if !defined(EXSLT_DYNAMICIMPL_HEADER_GUARD_1357924680)
#define EXSLT_DYNAMICIMPL_HEADER_GUARD_1357924680



#include "XalanEXSLTDefinitions.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/Function.hpp>



#include <xalanc/XalanExtensions/FunctionEvaluate.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionEvaluate : public FunctionEvaluate
{
public:

    typedef FunctionEvaluate    ParentType;

    XalanEXSLTFunctionEvaluate() :
        FunctionEvaluate()
    {
    }

    virtual
    ~XalanEXSLTFunctionEvaluate()
    {
    }

#if !defined(XALAN_NO_USING_DECLARATION)
    using ParentType::execute;
#endif

    virtual XObjectPtr
    execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const LocatorType*              locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    virtual Function*
#else
    virtual XalanEXSLTFunctionEvaluate*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    virtual const XalanDOMString&
    getError(XalanDOMString&    theResult) const
    {
        return XalanMessageLoader::getMessage(
                    theResult,
                    XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                    "evaluate");
    }

private:

    // Not implemented...
    XalanEXSLTFunctionEvaluate&
    operator=(const XalanEXSLTFunctionEvaluate&);

    bool
    operator==(const XalanEXSLTFunctionEvaluate&) const;
};



XALAN_CPP_NAMESPACE_END



#endif  // EXSLT_DYNAMICIMPL_HEADER_GUARD_1357924680
