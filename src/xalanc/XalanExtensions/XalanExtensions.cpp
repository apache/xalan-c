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
#include "XalanExtensions.hpp"

#include "FunctionDifference.hpp"
#include "FunctionDistinct.hpp"
#include "FunctionEvaluate.hpp"
#include "FunctionHasSameNodes.hpp"
#include "FunctionIntersection.hpp"
#include "FunctionNodeSet.hpp"



#include <xalanc/XPath/XPathEnvSupportDefault.hpp>



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMChar   s_extensionsNamespace[] =
{
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_p,
    XalanUnicode::charColon,
    XalanUnicode::charSolidus,
    XalanUnicode::charSolidus,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_l,
    XalanUnicode::charFullStop,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_e,
    XalanUnicode::charFullStop,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_g,
    XalanUnicode::charSolidus,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    0
};



static const XalanDOMChar   s_differenceFunctionName[] =
{
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_f,
    XalanUnicode::charLetter_f,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_e,
    0
};



static const XalanDOMChar   s_distinctFunctionName[] =
{
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_t,
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



static const XalanDOMChar   s_hasSameNodesFunctionName[] =
{
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_N,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_s,
    0
};



static const XalanDOMChar   s_intersectionFunctionName[] =
{
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    0
};



static const XalanDOMChar   s_nodeSetFunctionName1[] =
{
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_t,
    0
};



static const XalanDOMChar   s_nodeSetFunctionName2[] =
{
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_e,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_t,
    0
};



static const FunctionDifference     s_differenceFunction;
static const FunctionDistinct       s_distinctFunction;
static const FunctionEvaluate       s_evaluateFunction;
static const FunctionHasSameNodes   s_hasSameNodesFunction;
static const FunctionIntersection   s_intersectionFunction;
static const FunctionNodeSet        s_nodesetFunction(true);



static const XalanExtensionsInstaller::FunctionTableEntry   theFunctionTable[] =
{
    { s_differenceFunctionName, &s_differenceFunction },
    { s_distinctFunctionName, &s_distinctFunction },
    { s_evaluateFunctionName, &s_evaluateFunction },
    { s_hasSameNodesFunctionName, &s_hasSameNodesFunction },
    { s_intersectionFunctionName, &s_intersectionFunction },
    { s_nodeSetFunctionName1, &s_nodesetFunction },
    { s_nodeSetFunctionName2, &s_nodesetFunction },
    { 0, 0 }
};



void
XalanExtensionsInstaller::installLocal(XPathEnvSupportDefault&  theSupport)
{
    doInstallLocal(s_extensionsNamespace, theFunctionTable, theSupport);
}



void
XalanExtensionsInstaller::installGlobal(MemoryManager& theManager)
{
    doInstallGlobal( theManager, s_extensionsNamespace, theFunctionTable);
}



void
XalanExtensionsInstaller::uninstallLocal(XPathEnvSupportDefault&    theSupport)
{
    doUninstallLocal(s_extensionsNamespace, theFunctionTable, theSupport);
}



void
XalanExtensionsInstaller::uninstallGlobal(MemoryManager& theManager)
{
    doUninstallGlobal(theManager, s_extensionsNamespace, theFunctionTable);
}



void
XalanExtensionsInstaller::doInstallLocal(
            const XalanDOMChar*         theNamespace,
            const FunctionTableEntry    theFunctionTable[],
            XPathEnvSupportDefault&     theSupport)
{
    assert(theNamespace != 0);

    const XalanDOMString    theNamespaceString(theNamespace, theSupport.getMemoryManager());
    XalanDOMString          theFunctionName( theSupport.getMemoryManager());

    for(const FunctionTableEntry* theEntry = theFunctionTable; theEntry->theFunctionName != 0; ++theEntry)
    {
        assert(theEntry->theFunction != 0);

        theFunctionName = theEntry->theFunctionName;

        theSupport.installExternalFunctionLocal(theNamespaceString, theFunctionName, *theEntry->theFunction);
    }
}



void
XalanExtensionsInstaller::doInstallGlobal(
            MemoryManager&          theManager,
            const XalanDOMChar*         theNamespace,
            const FunctionTableEntry    theFunctionTable[])
{
    assert(theNamespace != 0);

    const XalanDOMString    theNamespaceString(theNamespace, theManager);
    XalanDOMString          theFunctionName(theManager);

    for(const FunctionTableEntry* theEntry = theFunctionTable; theEntry->theFunctionName != 0; ++theEntry)
    {
        assert(theEntry->theFunction != 0);

        theFunctionName = theEntry->theFunctionName;

        XPathEnvSupportDefault::installExternalFunctionGlobal(theNamespaceString, theFunctionName, *theEntry->theFunction);
    }
}



void
XalanExtensionsInstaller::doUninstallLocal(
            const XalanDOMChar*         theNamespace,
            const FunctionTableEntry    theFunctionTable[],
            XPathEnvSupportDefault&     theSupport)
{
    assert(theNamespace != 0);

    const XalanDOMString    theNamespaceString(theNamespace, theSupport.getMemoryManager());
    XalanDOMString          theFunctionName(theSupport.getMemoryManager());

    for(const FunctionTableEntry* theEntry = theFunctionTable; theEntry->theFunctionName != 0; ++theEntry)
    {
        theFunctionName = theEntry->theFunctionName;

        theSupport.uninstallExternalFunctionLocal(theNamespaceString, theFunctionName);
    }
}



void
XalanExtensionsInstaller::doUninstallGlobal(
            MemoryManager&          theManager,
            const XalanDOMChar*         theNamespace,
            const FunctionTableEntry    theFunctionTable[])
{
    assert(theNamespace != 0);

    const XalanDOMString    theNamespaceString(theNamespace, theManager);
    XalanDOMString          theFunctionName(theManager);

    for(const FunctionTableEntry* theEntry = theFunctionTable; theEntry->theFunctionName != 0; ++theEntry)
    {
        theFunctionName = theEntry->theFunctionName;

        XPathEnvSupportDefault::uninstallExternalFunctionGlobal(theNamespaceString, theFunctionName);
    }
}



XALAN_CPP_NAMESPACE_END
