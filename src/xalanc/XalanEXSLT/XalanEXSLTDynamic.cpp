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
#include "XalanEXSLTDynamic.hpp"
#include "XalanEXSLTDynamicImpl.hpp"



#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPathEnvSupportDefault.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XObjectPtr
XalanEXSLTFunctionEvaluate::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    try
    {
        return ParentType::execute(executionContext, context, args, locator);
    }
    catch(const XSLException&)
    {
    }

    XPathExecutionContext::BorrowReturnMutableNodeRefList   theGuard(executionContext);

    return executionContext.getXObjectFactory().createNodeSet(theGuard);
}



static const XalanDOMChar   s_dynamicNamespace[] =
{
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_p,
    XalanUnicode::charColon,
    XalanUnicode::charSolidus,
    XalanUnicode::charSolidus,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_t,
    XalanUnicode::charFullStop,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_g,
    XalanUnicode::charSolidus,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_y,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_c,
    0
};



static const XalanDOMChar   s_evaluateFunctionName[] =
{
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_v,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    0
};



static const XalanEXSLTFunctionEvaluate     s_evaluateFunction;



static const XalanEXSLTDynamicFunctionsInstaller::FunctionTableEntry    theFunctionTable[] =
{
    { s_evaluateFunctionName, &s_evaluateFunction},
    { 0, 0 }
};



void
XalanEXSLTDynamicFunctionsInstaller::installLocal(XPathEnvSupportDefault&   theSupport)
{
    doInstallLocal(s_dynamicNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTDynamicFunctionsInstaller::installGlobal(MemoryManager& theManager)
{
    doInstallGlobal(theManager, s_dynamicNamespace, theFunctionTable);

}



void
XalanEXSLTDynamicFunctionsInstaller::uninstallLocal(XPathEnvSupportDefault& theSupport)
{
    doUninstallLocal(s_dynamicNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTDynamicFunctionsInstaller::uninstallGlobal(MemoryManager& theManager)
{
    doUninstallGlobal(theManager, s_dynamicNamespace, theFunctionTable);

}



XALAN_CPP_NAMESPACE_END
