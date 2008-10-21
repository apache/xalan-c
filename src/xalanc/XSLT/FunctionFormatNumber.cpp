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
#include "FunctionFormatNumber.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FunctionFormatNumber::FunctionFormatNumber()
{
}



FunctionFormatNumber::~FunctionFormatNumber()
{
}



XObjectPtr
FunctionFormatNumber::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const Locator*          locator) const
{
    assert(arg1.null() == false && arg2.null() == false);   
    
    const double            theNumber = arg1->num(executionContext);
    const XalanDOMString&   thePattern = arg2->str(executionContext);

    GetCachedString     theString(executionContext);

    executionContext.formatNumber(
            theNumber, 
            thePattern, 
            theString.get(), 
            context, 
            locator);   

    return executionContext.getXObjectFactory().createString(theString);
}



XObjectPtr
FunctionFormatNumber::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const XObjectPtr        arg3,
            const Locator*          locator) const
{
    assert(arg1.null() == false && arg2.null() == false && arg3.null() == false);
    
    const double                        theNumber = arg1->num(executionContext);
    const XalanDOMString&               thePattern = arg2->str(executionContext);

    const XalanDOMString&               theDFSName = arg3->str(executionContext);
    assert(theDFSName.empty() == false);

    GetCachedString     theString(executionContext);

    executionContext.formatNumber(
        theNumber, 
        thePattern,
        theDFSName,
        theString.get(),
        context, 
        locator);

    return executionContext.getXObjectFactory().createString(theString);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionFormatNumber*
#endif
FunctionFormatNumber::clone(MemoryManager&  theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionFormatNumber::getError(XalanDOMString&  theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionTakesTwoOrThreeArguments_1Param,
                "format-number()");
}



XALAN_CPP_NAMESPACE_END
