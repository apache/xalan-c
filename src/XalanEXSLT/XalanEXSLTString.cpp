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
#include "XalanEXSLTString.hpp"
#include "XalanEXSLTStringImpl.hpp"



#include <PlatformSupport/DoubleSupport.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



#include <DOMSupport/DOMServices.hpp>



#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar	XalanEXSLTFunctionAlign::s_centerString[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	0
};



const XalanDOMChar	XalanEXSLTFunctionAlign::s_rightString[] =
{
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_t,
	0
};



XObjectPtr
XalanEXSLTFunctionAlign::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	const XObjectArgVectorType::size_type	theSize = args.size();

	if (theSize != 2 && theSize != 3)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false && args[1].null() == false && (theSize == 2 || args[2].null() == false));

	const XalanDOMString&	theTargetString = args[0]->str();
	const XalanDOMString&	thePaddingString = args[1]->str();

	const XalanDOMString::size_type		theTargetStringLength = theTargetString.length();
	const XalanDOMString::size_type		thePaddingStringLength = thePaddingString.length();

	if (theTargetStringLength == thePaddingStringLength)
	{
		return XObjectPtr(args[0]);
	}
	else
	{
		XPathExecutionContext::GetAndReleaseCachedString	theGuard(executionContext);

		XalanDOMString&		theResult = theGuard.get();

		if (theTargetStringLength > thePaddingStringLength)
		{
			theResult.assign(theTargetString, 0, thePaddingStringLength);
		}
		else
		{
			theResult.reserve(thePaddingStringLength);

			enum eAlignment { eCenter, eLeft, eRight };

			eAlignment	theAlignment = eLeft;

			if (theSize == 3)
			{
				const XalanDOMString&	theAlignmentString = args[2]->str();

				if (equals(
							s_centerString,
							theAlignmentString.c_str(),
							sizeof(s_centerString) / sizeof(s_centerString[0]) - 1) == true)
				{
					theAlignment = eCenter;
				}
				else if (equals(
							s_rightString,
							theAlignmentString.c_str(),
							sizeof(s_rightString) / sizeof(s_rightString[0]) - 1) == true)
				{
					theAlignment = eRight;
				}
			}

			if (theAlignment == eLeft)
			{
				theResult = theTargetString;

				theResult.append(thePaddingString, theTargetStringLength, thePaddingStringLength - theTargetStringLength);
			}
			else if (theAlignment == eRight)
			{
				theResult.assign(thePaddingString, 0, thePaddingStringLength - theTargetStringLength);
				theResult.append(theTargetString);
			}
			else if (theAlignment == eCenter)
			{
				const XalanDOMString::size_type		theStartIndex =
					(thePaddingStringLength - theTargetStringLength) / 2;

				theResult.assign(thePaddingString, 0, theStartIndex);

				theResult.append(theTargetString);

				theResult.append(
					thePaddingString,
					theTargetStringLength + theStartIndex,
					thePaddingStringLength - theTargetStringLength - theStartIndex);
			}
		}

		return executionContext.getXObjectFactory().createString(theResult);
	}
}



static const XalanDOMString		s_emptyString;



XObjectPtr
XalanEXSLTFunctionConcat::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

	const NodeRefListBase&				theNodeSet = args[0]->nodeset();
	const NodeRefListBase::size_type	theLength = theNodeSet.getLength();

	if (theLength == 0)
	{
		return executionContext.getXObjectFactory().createStringReference(s_emptyString);
	}
	else
	{
		XPathExecutionContext::GetAndReleaseCachedString	theGuard(executionContext);

		XalanDOMString&		theResult = theGuard.get();

		for(NodeRefListBase::size_type i = 0; i < theLength; ++i)
		{
			assert(theNodeSet.item(i) != 0);

			DOMServices::getNodeData(*theNodeSet.item(i), theResult);
		}

		return executionContext.getXObjectFactory().createString(theResult);
	}
}



const XalanDOMChar	XalanEXSLTFunctionPadding::s_spaceString[] =
{
	XalanUnicode::charSpace,
	0
};



XObjectPtr
XalanEXSLTFunctionPadding::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	assert(m_space == s_spaceString);

	const XObjectArgVectorType::size_type	theSize = args.size();

	if (theSize != 1 && theSize != 2)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false && (theSize == 1 || args[1].null() == false));

	const double						theLength = DoubleSupport::round(args[0]->num());
	const XalanDOMString&				thePaddingString = theSize == 2 ? args[1]->str() : m_space;
	const XalanDOMString::size_type		thePaddingStringLength = thePaddingString.length();

	if (theLength == 0.0 || thePaddingStringLength == 0)
	{
		return executionContext.getXObjectFactory().createStringReference(s_emptyString);
	}
	else
	{
		XPathExecutionContext::GetAndReleaseCachedString	theGuard(executionContext);

		XalanDOMString&		theResult = theGuard.get();

		if (thePaddingStringLength == 1)
		{
			theResult.assign(XalanDOMString::size_type(theLength), thePaddingString[0]);
		}
		else
		{
			XalanDOMString::size_type	theRemainingLength = XalanDOMString::size_type(theLength);

			for(;;)
			{
				if (theRemainingLength > thePaddingStringLength)
				{
					theResult.append(thePaddingString);

					theRemainingLength -= thePaddingStringLength;
				}
				else
				{
					theResult.append(
						thePaddingString,
						0,
						XalanDOMString::size_type(theRemainingLength));

					break;
				}
			}
		}

		return executionContext.getXObjectFactory().createString(theResult);
	}
}



static const XalanDOMChar	s_stringNamespace[] =
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
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_s,
	0
};



static const XalanDOMChar	s_alignFunctionName[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_n,
	0
};



static const XalanDOMChar	s_concatFunctionName[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	0
};



static const XalanDOMChar	s_paddingFunctionName[] =
{
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	0
};



static const XalanEXSLTFunctionAlign	s_alignFunction;
static const XalanEXSLTFunctionConcat	s_concatFunction;
// Note this is a special constructor of XalanEXSLTFunctionPadding which
// allocates no memory.  It is only used here, so we can have table-based
// initialization, but not have any memory allocation.
static const XalanEXSLTFunctionPadding	s_paddingFunction(1);



static const XalanEXSLTStringFunctionsInstaller::FunctionTableEntry		theFunctionTable[] =
{
	{ s_alignFunctionName, &s_alignFunction },
	{ s_concatFunctionName, &s_concatFunction },
	{ s_paddingFunctionName, &s_paddingFunction },
	{ 0, 0 }
};



void
XalanEXSLTStringFunctionsInstaller::installLocal(XPathEnvSupportDefault&		theSupport)
{
	doInstallLocal(s_stringNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTStringFunctionsInstaller::installGlobal()
{
	doInstallGlobal(s_stringNamespace, theFunctionTable);
}



void
XalanEXSLTStringFunctionsInstaller::uninstallLocal(XPathEnvSupportDefault&	theSupport)
{
	doUninstallLocal(s_stringNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTStringFunctionsInstaller::uninstallGlobal()
{
	doUninstallGlobal(s_stringNamespace, theFunctionTable);
}



XALAN_CPP_NAMESPACE_END
