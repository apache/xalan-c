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
#include "XalanEXSLTSet.hpp"
#include "XalanEXSLTSetImpl.hpp"



#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPathEnvSupportDefault.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XObjectPtr
XalanEXSLTFunctionHasSameNode::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 2)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false && args[1].null() == false);

    const NodeRefListBase&  nodeset1 = args[0]->nodeset();
    const NodeRefListBase&  nodeset2 = args[1]->nodeset();

    const NodeRefListBase::size_type    theLength1 = nodeset1.getLength();
    const NodeRefListBase::size_type    theLength2 = nodeset2.getLength();

    bool    fResult = false;

    if (theLength1 != 0 && theLength2 != 0)
    {
        for (NodeRefListBase::size_type i = 0; i < theLength1 && fResult == false; ++i)
        {
            XalanNode* const    theNode = nodeset1.item(i);
            assert(theNode != 0);

            if (nodeset2.indexOf(theNode) != NodeRefListBase::npos)
            {
                fResult = true;
            }
        }
    }

    return executionContext.getXObjectFactory().createBoolean(fResult);
}



typedef Function::XObjectArgVectorType  XObjectArgVectorType;


template<class PredicateType>
XObjectPtr
findNodes(
            XPathExecutionContext&          executionContext,
            const XObjectArgVectorType&     args,
            PredicateType                   thePredicate)
{
    assert(args[0].null() == false && args[1].null() == false);

    const NodeRefListBase&  nodeset1 = args[0]->nodeset();
    const NodeRefListBase&  nodeset2 = args[1]->nodeset();

    const NodeRefListBase::size_type    theLength1 = nodeset1.getLength();
    const NodeRefListBase::size_type    theLength2 = nodeset2.getLength();

    if (theLength1 == 0 || theLength2 == 0)
    {
        return args[0];
    }
    else
    {
        const XalanNode* const  theNode = nodeset2.item(0);
        assert(theNode != 0);

        XPathExecutionContext::BorrowReturnMutableNodeRefList   theNodes(executionContext);

        const NodeRefListBase::size_type    theIndex = nodeset1.indexOf(theNode);

        if (theIndex != NodeRefListBase::npos)
        {
            for (NodeRefListBase::size_type i = 0; i < theLength1; ++i)
            {
                XalanNode* const    theCurrentNode = nodeset1.item(i);
                assert(theCurrentNode != 0);

                if (thePredicate(theCurrentNode, theNode) == true)
                {
                    theNodes->addNodeInDocOrder(theCurrentNode, executionContext);
                }
            }
        }

        theNodes->setDocumentOrder();

        return executionContext.getXObjectFactory().createNodeSet(theNodes);
    }
}



struct LeadingCompareFunctor
{
    LeadingCompareFunctor(XPathExecutionContext&    executionContext) :
        m_executionContext(executionContext)
    {
    }

    bool
    operator()(
            const XalanNode*    theLHS,
            const XalanNode*    theRHS) const
    {
        assert(theLHS != 0 && theRHS != 0);

        return theLHS != theRHS && m_executionContext.isNodeAfter(*theLHS, *theRHS) == false;
    }

private:

    XPathExecutionContext&  m_executionContext;
};



XObjectPtr
XalanEXSLTFunctionLeading::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 2)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    return findNodes(executionContext, args, LeadingCompareFunctor(executionContext));
}



struct TrailingCompareFunctor
{
    TrailingCompareFunctor(XPathExecutionContext&   executionContext) :
        m_executionContext(executionContext)
    {
    }

    bool
    operator()(
            const XalanNode*    theLHS,
            const XalanNode*    theRHS) const
    {
        assert(theLHS != 0 && theRHS != 0);

        return m_executionContext.isNodeAfter(*theLHS, *theRHS) == true;
    }

private:

    XPathExecutionContext&  m_executionContext;
};



XObjectPtr
XalanEXSLTFunctionTrailing::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 2)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    return findNodes(executionContext, args, TrailingCompareFunctor(executionContext));
}



static const XalanDOMChar   s_setNamespace[] =
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
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_s,
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



static const XalanDOMChar   s_hasSameNodeFunctionName[] =
{
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_s,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_e,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_e,
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



static const XalanDOMChar   s_leadingFunctionName[] =
{
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    0
};



static const XalanDOMChar   s_trailingFunctionName[] =
{
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    0
};



static const XalanEXSLTFunctionDifference       s_differenceFunction;
static const XalanEXSLTFunctionDistinct         s_distinctFunction;
static const XalanEXSLTFunctionHasSameNode      s_hasSameNodeFunction;
static const XalanEXSLTFunctionIntersection     s_intersectionFunction;
static const XalanEXSLTFunctionLeading          s_leadingFunction;
static const XalanEXSLTFunctionTrailing         s_trailingFunction;



static const XalanEXSLTSetFunctionsInstaller::FunctionTableEntry    theFunctionTable[] =
{
    { s_differenceFunctionName, &s_differenceFunction },
    { s_distinctFunctionName, &s_distinctFunction },
    { s_hasSameNodeFunctionName, &s_hasSameNodeFunction },
    { s_intersectionFunctionName, &s_intersectionFunction },
    { s_leadingFunctionName, &s_leadingFunction },
    { s_trailingFunctionName, &s_trailingFunction },
    { 0, 0 }
};



void
XalanEXSLTSetFunctionsInstaller::installLocal(XPathEnvSupportDefault&   theSupport)
{
    doInstallLocal(s_setNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTSetFunctionsInstaller::installGlobal(MemoryManager& theManager)
{
    doInstallGlobal(theManager, s_setNamespace, theFunctionTable);
}



void
XalanEXSLTSetFunctionsInstaller::uninstallLocal(XPathEnvSupportDefault& theSupport)
{
    doUninstallLocal(s_setNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTSetFunctionsInstaller::uninstallGlobal(MemoryManager& theManager)
{
    doUninstallGlobal(theManager, s_setNamespace, theFunctionTable);
}



XALAN_CPP_NAMESPACE_END
