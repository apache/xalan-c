/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#include "XalanEXSLTMath.hpp"
#include "XalanEXSLTMathImpl.hpp"



#include <cmath>



#include <PlatformSupport/DoubleSupport.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



#include <DOMSupport/DOMServices.hpp>



#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>



template<class FunctionType>
XObjectPtr
findNodes(
			XPathExecutionContext&	executionContext,
			const NodeRefListBase&	theNodeSet,
			FunctionType			theCompareFunction)
{
	const NodeRefListBase::size_type	theLength = theNodeSet.getLength();

	XPathExecutionContext::BorrowReturnMutableNodeRefList	theNodes(executionContext);

	theNodes->setDocumentOrder();

	if (theLength != 0)
	{
		const XPathExecutionContext::GetAndReleaseCachedString	theGuard(executionContext);

		XalanDOMString&		theStringValue = theGuard.get();

		XalanNode*			theCurrentNode = theNodeSet.item(0);
		assert(theCurrentNode != 0);

		theNodes->addNode(theCurrentNode);

		DOMServices::getNodeData(*theCurrentNode, theStringValue);

		double	theNumericValue = DOMStringToDouble(theStringValue);

		for (NodeRefListBase::size_type i = 1; i < theLength; ++i)
		{
			theCurrentNode = theNodeSet.item(i);
			assert(theCurrentNode != 0);

			DOMServices::getNodeData(*theCurrentNode, theStringValue);

			const double	theCurrent = DOMStringToDouble(theStringValue);

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

			theStringValue.clear();
		}
	}

	return executionContext.getXObjectFactory().createNodeSet(theNodes);
}



XObjectPtr
XalanEXSLTFunctionHighest::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

	return findNodes(executionContext, args[0]->nodeset(), DoubleSupport::greaterThan);
}



const XalanDOMString
XalanEXSLTFunctionHighest::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT highest() function accepts one argument"));
}



XObjectPtr
XalanEXSLTFunctionLowest::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

	return findNodes(executionContext, args[0]->nodeset(), DoubleSupport::lessThan);
}



const XalanDOMString
XalanEXSLTFunctionLowest::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT lowest() function accepts one argument"));
}



template<class FunctionType>
XObjectPtr
findValue(
			XPathExecutionContext&	executionContext,
			const NodeRefListBase&	theNodeSet,
			FunctionType			theCompareFunction)
{
	const NodeRefListBase::size_type	theLength = theNodeSet.getLength();

	if (theLength == 0)
	{
		return executionContext.getXObjectFactory().createNumber(DoubleSupport::getNaN());
	}
	else
	{
		const XPathExecutionContext::GetAndReleaseCachedString	theGuard(executionContext);

		XalanDOMString&		theStringValue = theGuard.get();

		assert(theNodeSet.item(0) != 0);

		DOMServices::getNodeData(*theNodeSet.item(0), theStringValue);

		double	theResult = DOMStringToDouble(theStringValue);

		for (NodeRefListBase::size_type i = 1; i < theLength; ++i)
		{
			assert(theNodeSet.item(i) != 0);

			DOMServices::getNodeData(*theNodeSet.item(i), theStringValue);

			const double	theCurrent = DOMStringToDouble(theStringValue);

			if (DoubleSupport::isNaN(theCurrent) == true)
			{
				theResult = theCurrent;

				break;
			}
			else if (theCompareFunction(theCurrent, theResult) == true)
			{
				theResult = theCurrent;
			}

			theStringValue.clear();
		}

		return executionContext.getXObjectFactory().createNumber(theResult);
	}
}



XObjectPtr
XalanEXSLTFunctionMin::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

	return findValue(executionContext, args[0]->nodeset(), DoubleSupport::lessThan);
}



const XalanDOMString
XalanEXSLTFunctionMin::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT min() function accepts one argument"));
}



XObjectPtr
XalanEXSLTFunctionMax::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

	return findValue(executionContext, args[0]->nodeset(), DoubleSupport::greaterThan);
}



const XalanDOMString
XalanEXSLTFunctionMax::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT max() function accepts one argument"));
}



XObjectPtr
XalanEXSLTFunctionAbs::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::fabs;
#endif

	return executionContext.getXObjectFactory().createNumber(fabs(args[0]->num()));
}



const XalanDOMString
XalanEXSLTFunctionAbs::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT abs() function accepts one argument"));
}



XObjectPtr
XalanEXSLTFunctionAcos::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::acos;
#endif

	return executionContext.getXObjectFactory().createNumber(acos(args[0]->num()));
}



const XalanDOMString
XalanEXSLTFunctionAcos::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT acos() function accepts one argument"));
}



XObjectPtr
XalanEXSLTFunctionAsin::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::asin;
#endif

	return executionContext.getXObjectFactory().createNumber(asin(args[0]->num()));
}



const XalanDOMString
XalanEXSLTFunctionAsin::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT asin() function accepts one argument"));
}



XObjectPtr
XalanEXSLTFunctionAtan::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::atan;
#endif

	return executionContext.getXObjectFactory().createNumber(atan(args[0]->num()));
}



const XalanDOMString
XalanEXSLTFunctionAtan::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT atan() function accepts one argument"));
}



XObjectPtr
XalanEXSLTFunctionAtan2::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 2)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false && args[1].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::atan2;
#endif

	return executionContext.getXObjectFactory().createNumber(atan2(args[0]->num(), args[1]->num()));
}



const XalanDOMString
XalanEXSLTFunctionAtan2::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT atan2() function accepts two arguments"));
}



const XalanDOMChar	XalanEXSLTFunctionConstant::s_eString[] =
{
	XalanUnicode::charLetter_E,
	0
};



const XalanDOMChar	XalanEXSLTFunctionConstant::s_ln10String[] =
{
	XalanUnicode::charLetter_L,
	XalanUnicode::charLetter_N,
	XalanUnicode::charDigit_1,
	XalanUnicode::charDigit_0,
	0
};



const XalanDOMChar	XalanEXSLTFunctionConstant::s_ln2String[] =
{
	XalanUnicode::charLetter_L,
	XalanUnicode::charLetter_N,
	XalanUnicode::charDigit_2,
	0
};



const XalanDOMChar	XalanEXSLTFunctionConstant::s_log2EString[] =
{
	XalanUnicode::charLetter_L,
	XalanUnicode::charLetter_O,
	XalanUnicode::charLetter_G,
	XalanUnicode::charDigit_2,
	XalanUnicode::charLetter_E,
	0
};



const XalanDOMChar	XalanEXSLTFunctionConstant::s_piString[] =
{
	XalanUnicode::charLetter_P,
	XalanUnicode::charLetter_I,
	0
};



const XalanDOMChar	XalanEXSLTFunctionConstant::s_sqrt1_2String[] =
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



const XalanDOMChar	XalanEXSLTFunctionConstant::s_sqrt2String[] =
{
	XalanUnicode::charLetter_S,
	XalanUnicode::charLetter_Q,
	XalanUnicode::charLetter_R,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_R,
	XalanUnicode::charDigit_2,
	0
};



const char	XalanEXSLTFunctionConstant::s_eValueString[] =
	"2.71828182845904523536028747135266249775724709369996";

const char	XalanEXSLTFunctionConstant::s_ln10ValueString[] =
	"2.302585092994046";

const char	XalanEXSLTFunctionConstant::s_ln2ValueString[] =
	"0.69314718055994530941723212145817656807550013436025";

const char	XalanEXSLTFunctionConstant::s_log2EValueString[] =
	"1.4426950408889633";

const char	XalanEXSLTFunctionConstant::s_piValueString[] =
	"3.1415926535897932384626433832795028841971693993751";

const char	XalanEXSLTFunctionConstant::s_sqrt1_2ValueString[] =
	"0.7071067811865476";

const char	XalanEXSLTFunctionConstant::s_sqrt2ValueString[] =
	"1.41421356237309504880168872420969807856967187537694";



XObjectPtr
doConvert(
			XPathExecutionContext&	executionContext,
			const char*				theValue,
			size_t					theSize,
			double					thePrecision)
{
	return executionContext.getXObjectFactory().createNumber(
		DoubleSupport::toDouble(
			XalanDOMString(theValue, XalanDOMString::size_type(thePrecision <= theSize ? thePrecision : theSize))));
}



XObjectPtr
XalanEXSLTFunctionConstant::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 2)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false && args[1].null() == false);

	const XalanDOMString&	theConstant = args[0]->str();
	const double			thePrecision = DoubleSupport::round(args[1]->num());

	if (equals(
				s_eString,
				theConstant.c_str(),
				sizeof(s_eString) / sizeof(s_eString[0]) - 1) == true)
	{
		return doConvert(executionContext, s_eValueString, sizeof(s_eValueString) - 1, thePrecision);
	}
	else if (equals(
				s_ln10String,
				theConstant.c_str(),
				sizeof(s_ln10String) / sizeof(s_ln10String[0]) - 1) == true)
	{
		return doConvert(executionContext, s_ln10ValueString, sizeof(s_ln10ValueString) - 1, thePrecision);
	}
	else if (equals(
				s_ln2String,
				theConstant.c_str(),
				sizeof(s_ln2String) / sizeof(s_ln2String[0]) - 1)  == true)
	{
		return doConvert(executionContext, s_ln2ValueString, sizeof(s_ln2ValueString) - 1, thePrecision);
	}
	else if (equals(
				s_log2EString,
				theConstant.c_str(),
				sizeof(s_log2EString) / sizeof(s_log2EString[0]) - 1) == true)
	{
		return doConvert(executionContext, s_log2EValueString, sizeof(s_log2EValueString) - 1, thePrecision);
	}
	else if (equals(
				s_piString,
				theConstant.c_str(),
				sizeof(s_piString) / sizeof(s_piString[0]) - 1) == true)
	{
		return doConvert(executionContext, s_piValueString, sizeof(s_piValueString) - 1, thePrecision);
	}
	else if (equals(
				s_sqrt1_2String,
				theConstant.c_str(),
				sizeof(s_sqrt1_2String) / sizeof(s_sqrt1_2String[0]) - 1) == true)
	{
		return doConvert(executionContext, s_sqrt1_2ValueString, sizeof(s_sqrt1_2ValueString) - 1, thePrecision);
	}
	else if (equals(
				s_sqrt2String,
				theConstant.c_str(),
				sizeof(s_sqrt2String) / sizeof(s_sqrt2String[0]) - 1) == true)
	{
		return doConvert(executionContext, s_sqrt2ValueString, sizeof(s_sqrt2ValueString) - 1, thePrecision);
	}
	else
	{
		return executionContext.getXObjectFactory().createNumber(DoubleSupport::getNaN());
	}
}



XObjectPtr
XalanEXSLTFunctionCos::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::cos;
#endif

	return executionContext.getXObjectFactory().createNumber(cos(args[0]->num()));
}



const XalanDOMString
XalanEXSLTFunctionCos::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT cos() function accepts one argument"));
}



XObjectPtr
XalanEXSLTFunctionExp::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::exp;
#endif

	return executionContext.getXObjectFactory().createNumber(exp(args[0]->num()));
}



const XalanDOMString
XalanEXSLTFunctionExp::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT exp() function accepts one argument"));
}



XObjectPtr
XalanEXSLTFunctionLog::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::log;
#endif

	return executionContext.getXObjectFactory().createNumber(log(args[0]->num()));
}



const XalanDOMString
XalanEXSLTFunctionLog::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT log() function accepts one argument"));
}



XObjectPtr
XalanEXSLTFunctionPower::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 2)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false && args[1].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::pow;
#endif

	return executionContext.getXObjectFactory().createNumber(pow(args[0]->num(), args[1]->num()));
}



const XalanDOMString
XalanEXSLTFunctionPower::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT power() function accepts two arguments"));
}



XObjectPtr
XalanEXSLTFunctionSin::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::sin;
#endif

	return executionContext.getXObjectFactory().createNumber(sin(args[0]->num()));
}



const XalanDOMString
XalanEXSLTFunctionSin::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT sin() function accepts one argument"));
}



XObjectPtr
XalanEXSLTFunctionSqrt::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::sqrt;
#endif

	return executionContext.getXObjectFactory().createNumber(sqrt(args[0]->num()));
}



const XalanDOMString
XalanEXSLTFunctionSqrt::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT sqrt() function accepts one argument"));
}



XObjectPtr
XalanEXSLTFunctionTan::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::tan;
#endif

	return executionContext.getXObjectFactory().createNumber(tan(args[0]->num()));
}



const XalanDOMString
XalanEXSLTFunctionTan::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT tan() function accepts one argument"));
}



static const XalanDOMChar	s_mathNamespace[] =
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



static const XalanDOMChar	s_absFunctionName[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_s,
	0
};



static const XalanDOMChar	s_acosFunctionName[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_s,
	0
};



static const XalanDOMChar	s_asinFunctionName[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	0
};



static const XalanDOMChar	s_atanFunctionName[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	0
};



static const XalanDOMChar	s_atan2FunctionName[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	XalanUnicode::charDigit_2,
	0
};



static const XalanDOMChar	s_constantFunctionName[] =
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



static const XalanDOMChar	s_cosFunctionName[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_s,
	0
};



static const XalanDOMChar	s_expFunctionName[] =
{
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_p,
	0
};



static const XalanDOMChar	s_highestFunctionName[] =
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



static const XalanDOMChar	s_logFunctionName[] =
{
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_g,
	0
};



static const XalanDOMChar	s_lowestFunctionName[] =
{
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_w,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	0
};



static const XalanDOMChar	s_maxFunctionName[] =
{
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_x,
	0
};



static const XalanDOMChar	s_minFunctionName[] =
{
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	0
};



static const XalanDOMChar	s_powFunctionName[] =
{
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_w,
	0
};



static const XalanDOMChar	s_sinFunctionName[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	0
};



static const XalanDOMChar	s_sqrtFunctionName[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_q,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_t,
	0
};



static const XalanDOMChar	s_tanFunctionName[] =
{
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	0
};



static const XalanEXSLTFunctionAbs			s_absFunction;
static const XalanEXSLTFunctionAcos			s_acosFunction;
static const XalanEXSLTFunctionAsin			s_asinFunction;
static const XalanEXSLTFunctionAtan			s_atanFunction;
static const XalanEXSLTFunctionAtan			s_atan2Function;
static const XalanEXSLTFunctionConstant		s_constantFunction;
static const XalanEXSLTFunctionCos			s_cosFunction;
static const XalanEXSLTFunctionExp			s_expFunction;
static const XalanEXSLTFunctionHighest		s_highestFunction;
static const XalanEXSLTFunctionLog			s_logFunction;
static const XalanEXSLTFunctionLowest		s_lowestFunction;
static const XalanEXSLTFunctionMax			s_maxFunction;
static const XalanEXSLTFunctionMin			s_minFunction;
static const XalanEXSLTFunctionPower		s_powFunction;
static const XalanEXSLTFunctionSin			s_sinFunction;
static const XalanEXSLTFunctionSqrt			s_sqrtFunction;
static const XalanEXSLTFunctionTan			s_tanFunction;



static const XalanEXSLTMathFunctionsInstaller::FunctionTableEntry	theFunctionTable[] =
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
	{ s_powFunctionName, &s_powFunction },
	{ s_sinFunctionName, &s_sinFunction },
	{ s_sqrtFunctionName, &s_sqrtFunction },
	{ s_tanFunctionName, &s_tanFunction },
	{ 0, 0 }
};



void
XalanEXSLTMathFunctionsInstaller::installLocal(XPathEnvSupportDefault&		theSupport)
{
	doInstallLocal(s_mathNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTMathFunctionsInstaller::installGlobal()
{
	doInstallGlobal(s_mathNamespace, theFunctionTable);
}



void
XalanEXSLTMathFunctionsInstaller::uninstallLocal(XPathEnvSupportDefault&	theSupport)
{
	doUninstallLocal(s_mathNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTMathFunctionsInstaller::uninstallGlobal()
{
	doUninstallGlobal(s_mathNamespace, theFunctionTable);
}
