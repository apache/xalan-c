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
#include "XalanEXSLTMath.hpp"
#include "XalanEXSLTMathImpl.hpp"



#include <cmath>
#include <ctime>


#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPathEnvSupportDefault.hpp>



XALAN_CPP_NAMESPACE_BEGIN



template<class FunctionType>
XObjectPtr
findNodes(
            XPathExecutionContext&  executionContext,
            const NodeRefListBase&  theNodeSet,
            FunctionType            theCompareFunction)
{
    const NodeRefListBase::size_type    theLength = theNodeSet.getLength();

    XPathExecutionContext::BorrowReturnMutableNodeRefList   theNodes(executionContext);

    theNodes->setDocumentOrder();

    if (theLength != 0)
    {
        const XPathExecutionContext::GetCachedString    theGuard(executionContext);

        XalanDOMString&     theStringValue = theGuard.get();

        XalanNode*          theCurrentNode = theNodeSet.item(0);
        assert(theCurrentNode != 0);

        DOMServices::getNodeData(*theCurrentNode, executionContext, theStringValue);

        double  theNumericValue = DOMStringToDouble(theStringValue, executionContext.getMemoryManager());

        if (DoubleSupport::isNaN(theNumericValue) == false)
        {
            theNodes->addNode(theCurrentNode);

            for (NodeRefListBase::size_type i = 1; i < theLength; ++i)
            {
                theCurrentNode = theNodeSet.item(i);
                assert(theCurrentNode != 0);

                theStringValue.clear();

                DOMServices::getNodeData(*theCurrentNode, executionContext, theStringValue);

                const double    theCurrent = DOMStringToDouble(theStringValue, executionContext.getMemoryManager());

                if (DoubleSupport::isNaN(theCurrent) == true)
                {
                    theNodes->clear();

                    break;
                }
                else if (DoubleSupport::equal(theCurrent, theNumericValue) == true)
                {
                    theNodes->addNodeInDocOrder(theCurrentNode, executionContext);
                }
                else if (theCompareFunction(theCurrent, theNumericValue) == true)
                {
                    theNodes->clear();

                    theNodes->addNode(theCurrentNode);

                    theNumericValue = theCurrent;
                }
            }
        }
    }

    return executionContext.getXObjectFactory().createNodeSet(theNodes);
}


static const XalanDOMChar   s_highestFunctionName[] =
{
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_g,
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    0
};


XObjectPtr
XalanEXSLTFunctionHighest::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

    return findNodes(
                executionContext,
                args[0]->nodeset(),
                DoubleSupport::greaterThan);
}



const XalanDOMString&
XalanEXSLTFunctionHighest::getError(XalanDOMString&     theBuffer) const
{

    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_highestFunctionName);
}


static const XalanDOMChar   s_lowestFunctionName[] =
{
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_w,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    0
};


XObjectPtr
XalanEXSLTFunctionLowest::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

    return findNodes(
                executionContext,
                args[0]->nodeset(),
                DoubleSupport::lessThan);
}



const XalanDOMString&
XalanEXSLTFunctionLowest::getError(XalanDOMString& theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_lowestFunctionName);
}



template<class FunctionType>
XObjectPtr
findValue(
            XPathExecutionContext&  executionContext,
            const NodeRefListBase&  theNodeSet,
            FunctionType            theCompareFunction)
{
    const NodeRefListBase::size_type    theLength = theNodeSet.getLength();

    if (theLength == 0)
    {
        return executionContext.getXObjectFactory().createNumber(DoubleSupport::getNaN());
    }
    else
    {
        const XPathExecutionContext::GetCachedString    theGuard(executionContext);

        XalanDOMString&     theStringValue = theGuard.get();

        assert(theNodeSet.item(0) != 0);

        DOMServices::getNodeData(*theNodeSet.item(0), executionContext, theStringValue);

        double  theResult = DOMStringToDouble(theStringValue, executionContext.getMemoryManager());

        for (NodeRefListBase::size_type i = 1; i < theLength; ++i)
        {
            assert(theNodeSet.item(i) != 0);

            theStringValue.clear();

            DOMServices::getNodeData(*theNodeSet.item(i), executionContext, theStringValue);

            const double    theCurrent =
                DOMStringToDouble(
                    theStringValue,
                    executionContext.getMemoryManager());

            if (DoubleSupport::isNaN(theCurrent) == true)
            {
                theResult = theCurrent;

                break;
            }
            else if (theCompareFunction(theCurrent, theResult) == true)
            {
                theResult = theCurrent;
            }
        }

        return executionContext.getXObjectFactory().createNumber(theResult);
    }
}



static const XalanDOMChar   s_minFunctionName[] =
{
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    0
};



XObjectPtr
XalanEXSLTFunctionMin::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

    return findValue(
                executionContext,
                args[0]->nodeset(),
                DoubleSupport::lessThan);
}



const XalanDOMString&
XalanEXSLTFunctionMin::getError(XalanDOMString&     theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_minFunctionName);
}



static const XalanDOMChar   s_maxFunctionName[] =
{
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_x,
    0
};



XObjectPtr
XalanEXSLTFunctionMax::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

    return findValue(
                executionContext,
                args[0]->nodeset(),
                DoubleSupport::greaterThan);
}



const XalanDOMString&
XalanEXSLTFunctionMax::getError(XalanDOMString&     theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_maxFunctionName);
}



static const XalanDOMChar   s_absFunctionName[] =
{
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_s,
    0
};



XObjectPtr
XalanEXSLTFunctionAbs::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

    return executionContext.getXObjectFactory().createNumber(DoubleSupport::abs(args[0]->num(executionContext)));
}



const XalanDOMString&
XalanEXSLTFunctionAbs::getError(XalanDOMString&     theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_absFunctionName);
}



static const XalanDOMChar   s_randomFunctionName[] =
{
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_m,
    0
};



XObjectPtr
XalanEXSLTFunctionRandom::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.empty() == false)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::rand;
#endif

    const int   value = rand();

    double      result = 0.0;

    if (value != 0)
    {
        result = double(value) / RAND_MAX;
    }
    assert(result >= 0.0L && result <= 1.0L);

    return executionContext.getXObjectFactory().createNumber(result);
}



const XalanDOMString&
XalanEXSLTFunctionRandom::getError(XalanDOMString&  theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsNoArgument_1Param,
                s_randomFunctionName);
}



static const XalanDOMChar   s_acosFunctionName[] =
{
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_s,
    0
};



XObjectPtr
XalanEXSLTFunctionAcos::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::acos;
#endif

    return executionContext.getXObjectFactory().createNumber(acos(args[0]->num(executionContext)));
}



const XalanDOMString&
XalanEXSLTFunctionAcos::getError(XalanDOMString&    theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_acosFunctionName);
}



static const XalanDOMChar   s_asinFunctionName[] =
{
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    0
};




XObjectPtr
XalanEXSLTFunctionAsin::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::asin;
#endif

    return executionContext.getXObjectFactory().createNumber(asin(args[0]->num(executionContext)));
}



const XalanDOMString&
XalanEXSLTFunctionAsin::getError(XalanDOMString&    theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_asinFunctionName);
}



static const XalanDOMChar   s_atanFunctionName[] =
{
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    0
};



XObjectPtr
XalanEXSLTFunctionAtan::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::atan;
#endif

    return executionContext.getXObjectFactory().createNumber(atan(args[0]->num(executionContext)));
}



const XalanDOMString&
XalanEXSLTFunctionAtan::getError(XalanDOMString&    theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_atanFunctionName);
}



static const XalanDOMChar   s_atan2FunctionName[] =
{
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    XalanUnicode::charDigit_2,
    0
};



XObjectPtr
XalanEXSLTFunctionAtan2::execute(
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

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::atan2;
#endif

    return executionContext.getXObjectFactory().createNumber(
                atan2(args[0]->num(executionContext), args[1]->num(executionContext)));
}



const XalanDOMString&
XalanEXSLTFunctionAtan2::getError(XalanDOMString&   theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsTwoArguments_1Param,
                s_atan2FunctionName);
}



const XalanDOMChar  XalanEXSLTFunctionConstant::s_eString[] =
{
    XalanUnicode::charLetter_E,
    0
};



const XalanDOMChar  XalanEXSLTFunctionConstant::s_ln10String[] =
{
    XalanUnicode::charLetter_L,
    XalanUnicode::charLetter_N,
    XalanUnicode::charDigit_1,
    XalanUnicode::charDigit_0,
    0
};



const XalanDOMChar  XalanEXSLTFunctionConstant::s_ln2String[] =
{
    XalanUnicode::charLetter_L,
    XalanUnicode::charLetter_N,
    XalanUnicode::charDigit_2,
    0
};



const XalanDOMChar  XalanEXSLTFunctionConstant::s_log2EString[] =
{
    XalanUnicode::charLetter_L,
    XalanUnicode::charLetter_O,
    XalanUnicode::charLetter_G,
    XalanUnicode::charDigit_2,
    XalanUnicode::charLetter_E,
    0
};



const XalanDOMChar  XalanEXSLTFunctionConstant::s_piString[] =
{
    XalanUnicode::charLetter_P,
    XalanUnicode::charLetter_I,
    0
};



const XalanDOMChar  XalanEXSLTFunctionConstant::s_sqrt1_2String[] =
{
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_Q,
    XalanUnicode::charLetter_R,
    XalanUnicode::charLetter_T,
    XalanUnicode::charDigit_1,
    XalanUnicode::charLowLine,
    XalanUnicode::charDigit_2,
    0
};



const XalanDOMChar  XalanEXSLTFunctionConstant::s_sqrt2String[] =
{
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_Q,
    XalanUnicode::charLetter_R,
    XalanUnicode::charLetter_R,
    XalanUnicode::charLetter_T,
    XalanUnicode::charDigit_2,
    0
};



const double    XalanEXSLTFunctionConstant::s_eValues[] =
{
    0.0L,
    2.7L,
    2.71L,
    2.718L,
    2.7182L,
    2.71828L,
    2.718281L,
    2.7182818L,
    2.71828182L,
    2.718281828L,
    2.7182818284L,
    2.71828182845L,
    2.718281828459L,
    2.7182818284590L,
    2.71828182845904L,
    2.718281828459045L,
    2.7182818284590452L,
    2.71828182845904523L,
    2.718281828459045235L,
    2.7182818284590452353L,
    2.71828182845904523536L,
    2.718281828459045235360L,
    2.7182818284590452353602L,
    2.71828182845904523536028L,
    2.718281828459045235360287L,
    2.7182818284590452353602874L,
    2.71828182845904523536028747L,
    2.718281828459045235360287471L,
    2.7182818284590452353602874713L,
    2.71828182845904523536028747135L,
    2.718281828459045235360287471352L,
    2.7182818284590452353602874713526L,
    2.71828182845904523536028747135266L,
    2.718281828459045235360287471352662L,
    2.7182818284590452353602874713526624L,
    2.71828182845904523536028747135266249L,
    2.718281828459045235360287471352662497L,
    2.7182818284590452353602874713526624977L,
    2.71828182845904523536028747135266249775L,
    2.718281828459045235360287471352662497757L,
    2.7182818284590452353602874713526624977572L,
    2.71828182845904523536028747135266249775724L,
    2.718281828459045235360287471352662497757247L,
    2.7182818284590452353602874713526624977572470L,
    2.71828182845904523536028747135266249775724709L,
    2.718281828459045235360287471352662497757247093L,
    2.7182818284590452353602874713526624977572470936L,
    2.71828182845904523536028747135266249775724709369L,
    2.718281828459045235360287471352662497757247093699L,
    2.7182818284590452353602874713526624977572470936999L,
    2.71828182845904523536028747135266249775724709369996L,
};

const double    XalanEXSLTFunctionConstant::s_ln10Values[] =
{
    0.0L,
    2.3L,
    2.30L,
    2.302L,
    2.3025L,
    2.30258L,
    2.302585L,
    2.3025850L,
    2.30258509L,
    2.302585092L,
    2.3025850929L,
    2.30258509299L,
    2.302585092994L,
    2.3025850929940L,
    2.30258509299404L,
    2.302585092994046L
};

const double    XalanEXSLTFunctionConstant::s_ln2Values[] =
{
    0.0L,
    0.6L,
    0.69L,
    0.693L,
    0.6931L,
    0.69314L,
    0.693147L,
    0.6931471L,
    0.69314718L,
    0.693147180L,
    0.6931471805L,
    0.69314718055L,
    0.693147180559L,
    0.6931471805599L,
    0.69314718055994L,
    0.693147180559945L,
    0.6931471805599453L,
    0.69314718055994530L,
    0.693147180559945309L,
    0.6931471805599453094L,
    0.69314718055994530941L,
    0.693147180559945309417L,
    0.6931471805599453094172L,
    0.69314718055994530941723L,
    0.693147180559945309417232L,
    0.6931471805599453094172321L,
    0.69314718055994530941723212L,
    0.693147180559945309417232121L,
    0.6931471805599453094172321214L,
    0.69314718055994530941723212145L,
    0.693147180559945309417232121458L,
    0.6931471805599453094172321214581L,
    0.69314718055994530941723212145817L,
    0.693147180559945309417232121458176L,
    0.6931471805599453094172321214581765L,
    0.69314718055994530941723212145817656L,
    0.693147180559945309417232121458176568L,
    0.6931471805599453094172321214581765680L,
    0.69314718055994530941723212145817656807L,
    0.693147180559945309417232121458176568075L,
    0.6931471805599453094172321214581765680755L,
    0.69314718055994530941723212145817656807550L,
    0.693147180559945309417232121458176568075500L,
    0.6931471805599453094172321214581765680755001L,
    0.69314718055994530941723212145817656807550013L,
    0.693147180559945309417232121458176568075500134L,
    0.6931471805599453094172321214581765680755001343L,
    0.69314718055994530941723212145817656807550013436L,
    0.693147180559945309417232121458176568075500134360L,
    0.6931471805599453094172321214581765680755001343602L,
    0.69314718055994530941723212145817656807550013436025L
};

const double    XalanEXSLTFunctionConstant::s_log2EValues[] =
{
    0.0L,
    1.4L,
    1.44L,
    1.442L,
    1.4426L,
    1.44269L,
    1.442695L,
    1.4426950L,
    1.44269504L,
    1.442695040L,
    1.4426950408L,
    1.44269504088L,
    1.442695040888L,
    1.4426950408889L,
    1.44269504088896L,
    1.442695040888963L,
    1.4426950408889633L
};

const double    XalanEXSLTFunctionConstant::s_piValues[] =
{
    0.0L,
    3.1L,
    3.14L,
    3.141L,
    3.1415L,
    3.14159L,
    3.141592L,
    3.1415926L,
    3.14159265L,
    3.141592653L,
    3.1415926535L,
    3.14159265358L,
    3.141592653589L,
    3.1415926535897L,
    3.14159265358979L,
    3.141592653589793L,
    3.1415926535897932L,
    3.14159265358979323L,
    3.141592653589793238L,
    3.1415926535897932384L,
    3.14159265358979323846L,
    3.141592653589793238462L,
    3.1415926535897932384626L,
    3.14159265358979323846264L,
    3.141592653589793238462643L,
    3.1415926535897932384626433L,
    3.14159265358979323846264338L,
    3.141592653589793238462643383L,
    3.1415926535897932384626433832L,
    3.14159265358979323846264338327L,
    3.141592653589793238462643383279L,
    3.1415926535897932384626433832795L,
    3.14159265358979323846264338327950L,
    3.141592653589793238462643383279502L,
    3.1415926535897932384626433832795028L,
    3.14159265358979323846264338327950288L,
    3.141592653589793238462643383279502884L,
    3.1415926535897932384626433832795028841L,
    3.14159265358979323846264338327950288419L,
    3.141592653589793238462643383279502884197L,
    3.1415926535897932384626433832795028841971L,
    3.14159265358979323846264338327950288419716L,
    3.141592653589793238462643383279502884197169L,
    3.1415926535897932384626433832795028841971693L,
    3.14159265358979323846264338327950288419716939L,
    3.141592653589793238462643383279502884197169399L,
    3.1415926535897932384626433832795028841971693993L,
    3.14159265358979323846264338327950288419716939937L,
    3.141592653589793238462643383279502884197169399375L,
    3.1415926535897932384626433832795028841971693993751L
};

const double    XalanEXSLTFunctionConstant::s_sqrt1_2Values[] =
{
    0.0L,
    0.7L,
    0.70L,
    0.707L,
    0.7071L,
    0.70710L,
    0.707106L,
    0.7071067L,
    0.70710678L,
    0.707106781L,
    0.7071067811L,
    0.70710678118L,
    0.707106781186L,
    0.7071067811865L,
    0.70710678118654L,
    0.707106781186547L,
    0.7071067811865476L
};

const double    XalanEXSLTFunctionConstant::s_sqrt2Values[] =
{
    0.0L,
    1.4L,
    1.41L,
    1.414L,
    1.4142L,
    1.41421L,
    1.414213L,
    1.4142135L,
    1.41421356L,
    1.414213562L,
    1.4142135623L,
    1.41421356237L,
    1.414213562373L,
    1.4142135623730L,
    1.41421356237309L,
    1.414213562373095L,
    1.4142135623730950L,
    1.41421356237309504L,
    1.414213562373095048L,
    1.4142135623730950488L,
    1.41421356237309504880L,
    1.414213562373095048801L,
    1.4142135623730950488016L,
    1.41421356237309504880168L,
    1.414213562373095048801688L,
    1.4142135623730950488016887L,
    1.41421356237309504880168872L,
    1.414213562373095048801688724L,
    1.4142135623730950488016887242L,
    1.41421356237309504880168872420L,
    1.414213562373095048801688724209L,
    1.4142135623730950488016887242096L,
    1.41421356237309504880168872420969L,
    1.414213562373095048801688724209698L,
    1.4142135623730950488016887242096980L,
    1.41421356237309504880168872420969807L,
    1.414213562373095048801688724209698078L,
    1.4142135623730950488016887242096980785L,
    1.41421356237309504880168872420969807856L,
    1.414213562373095048801688724209698078569L,
    1.4142135623730950488016887242096980785696L,
    1.41421356237309504880168872420969807856967L,
    1.414213562373095048801688724209698078569671L,
    1.4142135623730950488016887242096980785696718L,
    1.41421356237309504880168872420969807856967187L,
    1.414213562373095048801688724209698078569671875L,
    1.4142135623730950488016887242096980785696718753L,
    1.41421356237309504880168872420969807856967187537L,
    1.414213562373095048801688724209698078569671875376L,
    1.4142135623730950488016887242096980785696718753769L,
    1.41421356237309504880168872420969807856967187537694L
};



XObjectPtr
doConvert(
            XPathExecutionContext&  executionContext,
            const double            theValues[],
            size_t                  theSize,
            double                  thePrecision)
{
    return executionContext.getXObjectFactory().createNumber(
            theValues[XalanDOMString::size_type(thePrecision <= theSize ? thePrecision : theSize)]);
}



XObjectPtr
XalanEXSLTFunctionConstant::execute(
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

    const XalanDOMString&   theConstant = args[0]->str(executionContext);
    const double            thePrecision = DoubleSupport::round(args[1]->num(executionContext));

    if (thePrecision <= 0.0L)
    {
        return executionContext.getXObjectFactory().createNumber(DoubleSupport::getNaN());
    }
    else if (equals(s_eString, theConstant) == true)
    {
        return doConvert(
                executionContext,
                s_eValues,
                sizeof(s_eValues) / sizeof(s_eValues[0]),
                thePrecision);
    }
    else if (equals(s_ln10String, theConstant) == true)
    {
        return doConvert(
                executionContext,
                s_ln10Values,
                sizeof(s_ln10Values) / sizeof(s_ln10Values[0]),
                thePrecision);
    }
    else if (equals(s_ln2String, theConstant)  == true)
    {
        return doConvert(
                executionContext,
                s_ln2Values,
                sizeof(s_ln2Values) / sizeof(s_ln2Values[0]),
                thePrecision);
    }
    else if (equals(s_log2EString, theConstant) == true)
    {
        return doConvert(
                executionContext,
                s_log2EValues,
                sizeof(s_log2EValues) / sizeof(s_log2EValues[0]),
                thePrecision);
    }
    else if (equals(s_piString, theConstant) == true)
    {
        return doConvert(
                executionContext,
                s_piValues,
                sizeof(s_piValues) / sizeof(s_piValues[0]),
                thePrecision);
    }
    else if (equals(s_sqrt1_2String, theConstant) == true)
    {
        return doConvert(
                executionContext,
                s_sqrt1_2Values,
                sizeof(s_sqrt1_2Values) / sizeof(s_sqrt1_2Values[0]),
                thePrecision);
    }
    else if (equals(s_sqrt2String, theConstant) == true)
    {
        return doConvert(
                executionContext,
                s_sqrt2Values,
                sizeof(s_sqrt2Values) / sizeof(s_sqrt2Values[0]),
                thePrecision);
    }
    else
    {
        return executionContext.getXObjectFactory().createNumber(DoubleSupport::getNaN());
    }
}



static const XalanDOMChar   s_cosFunctionName[] =
{
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_s,
    0
};



XObjectPtr
XalanEXSLTFunctionCos::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::cos;
#endif

    return executionContext.getXObjectFactory().createNumber(cos(args[0]->num(executionContext)));
}



const XalanDOMString&
XalanEXSLTFunctionCos::getError(XalanDOMString&     theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_cosFunctionName);
}



static const XalanDOMChar   s_expFunctionName[] =
{
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_p,
    0
};



XObjectPtr
XalanEXSLTFunctionExp::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::exp;
#endif

    return executionContext.getXObjectFactory().createNumber(exp(args[0]->num(executionContext)));
}



const XalanDOMString&
XalanEXSLTFunctionExp::getError(XalanDOMString&     theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_expFunctionName);
}



static const XalanDOMChar   s_logFunctionName[] =
{
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_g,
    0
};



XObjectPtr
XalanEXSLTFunctionLog::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::log;
#endif

    return executionContext.getXObjectFactory().createNumber(log(args[0]->num(executionContext)));
}



const XalanDOMString&
XalanEXSLTFunctionLog::getError(XalanDOMString&     theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_logFunctionName);
}



static const XalanDOMChar   s_powerFunctionName[] =
{
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_w,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    0
};



XObjectPtr
XalanEXSLTFunctionPower::execute(
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

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::pow;
#endif

    return executionContext.getXObjectFactory().createNumber(
                pow(
                    args[0]->num(executionContext),
                    args[1]->num(executionContext)));
}



const XalanDOMString&
XalanEXSLTFunctionPower::getError(XalanDOMString&   theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsTwoArguments_1Param,
                s_powerFunctionName);
}



static const XalanDOMChar   s_sinFunctionName[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    0
};



XObjectPtr
XalanEXSLTFunctionSin::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::sin;
#endif

    return executionContext.getXObjectFactory().createNumber(sin(args[0]->num(executionContext)));
}



const XalanDOMString&
XalanEXSLTFunctionSin::getError(XalanDOMString&     theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_sinFunctionName);

}



static const XalanDOMChar   s_sqrtFunctionName[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_q,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_t,
    0
};



XObjectPtr
XalanEXSLTFunctionSqrt::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::sqrt;
#endif

    return executionContext.getXObjectFactory().createNumber(sqrt(args[0]->num(executionContext)));
}



const XalanDOMString&
XalanEXSLTFunctionSqrt::getError(XalanDOMString&    theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_sqrtFunctionName);
}



static const XalanDOMChar   s_tanFunctionName[] =
{
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    0
};



XObjectPtr
XalanEXSLTFunctionTan::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::tan;
#endif

    return executionContext.getXObjectFactory().createNumber(tan(args[0]->num(executionContext)));
}



const XalanDOMString&
XalanEXSLTFunctionTan::getError(XalanDOMString&     theBuffer) const
{
    return XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                s_tanFunctionName);
}



static const XalanDOMChar   s_mathNamespace[] =
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
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_h,
    0
};




static const XalanDOMChar   s_constantFunctionName[] =
{
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    0
};



static const XalanEXSLTFunctionAbs          s_absFunction;
static const XalanEXSLTFunctionAcos         s_acosFunction;
static const XalanEXSLTFunctionAsin         s_asinFunction;
static const XalanEXSLTFunctionAtan         s_atanFunction;
static const XalanEXSLTFunctionAtan2        s_atan2Function;
static const XalanEXSLTFunctionConstant     s_constantFunction;
static const XalanEXSLTFunctionCos          s_cosFunction;
static const XalanEXSLTFunctionExp          s_expFunction;
static const XalanEXSLTFunctionHighest      s_highestFunction;
static const XalanEXSLTFunctionLog          s_logFunction;
static const XalanEXSLTFunctionLowest       s_lowestFunction;
static const XalanEXSLTFunctionMax          s_maxFunction;
static const XalanEXSLTFunctionMin          s_minFunction;
static const XalanEXSLTFunctionPower        s_powerFunction;
static const XalanEXSLTFunctionSin          s_sinFunction;
static const XalanEXSLTFunctionSqrt         s_sqrtFunction;
static const XalanEXSLTFunctionTan          s_tanFunction;
static const XalanEXSLTFunctionRandom       s_randomFunction;


static const XalanEXSLTMathFunctionsInstaller::FunctionTableEntry   theFunctionTable[] =
{
    { s_absFunctionName, &s_absFunction },
    { s_acosFunctionName, &s_acosFunction },
    { s_asinFunctionName, &s_asinFunction },
    { s_atanFunctionName, &s_atanFunction },
    { s_atan2FunctionName, &s_atan2Function },
    { s_constantFunctionName, &s_constantFunction },
    { s_cosFunctionName, &s_cosFunction },
    { s_expFunctionName, &s_expFunction },
    { s_highestFunctionName, &s_highestFunction },
    { s_logFunctionName, &s_logFunction },
    { s_lowestFunctionName, &s_lowestFunction },
    { s_maxFunctionName, &s_maxFunction },
    { s_minFunctionName, &s_minFunction },
    { s_powerFunctionName, &s_powerFunction },
    { s_sinFunctionName, &s_sinFunction },
    { s_sqrtFunctionName, &s_sqrtFunction },
    { s_tanFunctionName, &s_tanFunction },
    { s_randomFunctionName, &s_randomFunction },
    { 0, 0 }
};



void
XalanEXSLTMathFunctionsInstaller::installLocal(XPathEnvSupportDefault&      theSupport)
{
    doInstallLocal(s_mathNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTMathFunctionsInstaller::installGlobal(MemoryManager& theManager)
{
    doInstallGlobal(theManager, s_mathNamespace, theFunctionTable);

    // Sets the starting point for generating a series of pseudorandom integers,
    // we need it for random() EXSLT function
#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::srand;
    using std::time;
#endif
    srand( (unsigned)time( NULL ) );
}



void
XalanEXSLTMathFunctionsInstaller::uninstallLocal(XPathEnvSupportDefault&    theSupport)
{
    doUninstallLocal(s_mathNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTMathFunctionsInstaller::uninstallGlobal(MemoryManager& theManager)
{
    doUninstallGlobal(theManager, s_mathNamespace, theFunctionTable);
}



XALAN_CPP_NAMESPACE_END
