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
#include "XalanEXSLTCommon.hpp"
#include "XalanEXSLTCommonImpl.hpp"



#include <xalanc/PlatformSupport/XalanUnicode.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPathEnvSupportDefault.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar  XalanEXSLTFunctionObjectType::s_booleanString[] =
{
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    0
};



const XalanDOMChar  XalanEXSLTFunctionObjectType::s_externalString[] =
{
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    0
};



const XalanDOMChar  XalanEXSLTFunctionObjectType::s_nodeSetString[] =
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



const XalanDOMChar  XalanEXSLTFunctionObjectType::s_numberString[] =
{
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    0
};



const XalanDOMChar  XalanEXSLTFunctionObjectType::s_rtfString[] =
{
    XalanUnicode::charLetter_R,
    XalanUnicode::charLetter_T,
    XalanUnicode::charLetter_F,
    0
};



const XalanDOMChar  XalanEXSLTFunctionObjectType::s_stringString[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    0
};



static const XalanDOMChar   s_objectTypeFunctionName[] =
{
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_j,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_t,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_y,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_e,
    0
};


XObjectPtr
XalanEXSLTFunctionObjectType::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    // Make sure nothing's happened to our strings and that
    // they were actually initialized...
    assert(XalanDOMString::equals(m_boolean, s_booleanString) == true);
    assert(XalanDOMString::equals(m_external, s_externalString) == true);
    assert(XalanDOMString::equals(m_nodeSet, s_nodeSetString) == true);
    assert(XalanDOMString::equals(m_number, s_numberString) == true);
    assert(XalanDOMString::equals(m_rtf, s_rtfString) == true);
    assert(XalanDOMString::equals(m_string, s_stringString) == true);

    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

    const XalanDOMString*   theResult = &m_external;

    switch(args[0]->getType())
    {
    case XObject::eTypeBoolean:
        theResult = &m_boolean;
        break;

    case XObject::eTypeNodeSet:
        theResult = &m_nodeSet;
        break;

    case XObject::eTypeNumber:
        theResult = &m_number;
        break;

    case XObject::eTypeResultTreeFrag:
        theResult = &m_rtf;
        break;

    case XObject::eTypeString:
        theResult = &m_string;
        break;

    default:
        break;
    }

    assert(theResult != 0);

    return executionContext.getXObjectFactory().createStringReference(*theResult);
}



const XalanDOMString&
XalanEXSLTFunctionObjectType::getError(XalanDOMString&  theResult) const
{
     return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_objectTypeFunctionName);
}



static const XalanDOMChar   s_commonNamespace[] =
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
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    0
};



static const XalanDOMChar   s_nodeSetFunctionName[] =
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



static const XalanEXSLTFunctionNodeSet      s_nodesetFunction;

// Note this is a special constructor of XalanEXSLTFunctionObjectType which
// allocates no memory.  It is only used here, so we can have table-based
// initialization, but not have any memory allocation.

static const XalanEXSLTFunctionObjectType   s_objectTypeFunction(XalanMemMgrs::getDummyMemMgr(), 1);



static const XalanEXSLTCommonFunctionsInstaller::FunctionTableEntry     theFunctionTable[] =
{
    { s_nodeSetFunctionName, &s_nodesetFunction },
    { s_objectTypeFunctionName, &s_objectTypeFunction },
    { 0, 0 }
};



void
XalanEXSLTCommonFunctionsInstaller::installLocal(XPathEnvSupportDefault&    theSupport)
{
    doInstallLocal(s_commonNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTCommonFunctionsInstaller::installGlobal(MemoryManager& theManager)
{
    doInstallGlobal( theManager, s_commonNamespace, theFunctionTable);
}



void
XalanEXSLTCommonFunctionsInstaller::uninstallLocal(XPathEnvSupportDefault&  theSupport)
{
    doUninstallLocal(s_commonNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTCommonFunctionsInstaller::uninstallGlobal(MemoryManager& theManager)
{
    doUninstallGlobal(theManager, s_commonNamespace, theFunctionTable);
}



XALAN_CPP_NAMESPACE_END
