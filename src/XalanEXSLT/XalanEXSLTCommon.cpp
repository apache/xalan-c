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
#include "XalanEXSLTCommon.hpp"
#include "XalanEXSLTCommonImpl.hpp"



#include <PlatformSupport/XalanUnicode.hpp>



#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>



const XalanDOMChar	XalanEXSLTFunctionObjectType::s_booleanString[] =
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



const XalanDOMChar	XalanEXSLTFunctionObjectType::s_externalString[] =
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



const XalanDOMChar	XalanEXSLTFunctionObjectType::s_nodeSetString[] =
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



const XalanDOMChar	XalanEXSLTFunctionObjectType::s_numberString[] =
{
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	0
};



const XalanDOMChar	XalanEXSLTFunctionObjectType::s_rtfString[] =
{
	XalanUnicode::charLetter_R,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_F,
	0
};



const XalanDOMChar	XalanEXSLTFunctionObjectType::s_stringString[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	0
};



XObjectPtr
XalanEXSLTFunctionObjectType::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const
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
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

	const XalanDOMString*	theResult = &m_external;

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



const XalanDOMString
XalanEXSLTFunctionObjectType::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT object-type() function accepts one argument"));
}



static const XalanDOMChar	s_commonNamespace[] =
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



static const XalanDOMChar	s_nodeSetFunctionName[] =
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



static const XalanDOMChar	s_objectTypeFunctionName[] =
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



static const XalanEXSLTFunctionNodeSet		s_nodesetFunction;
// Note this is a special constructor of XalanEXSLTFunctionObjectType which
// allocates no memory.  It is only used here, so we can have table-based
// initialization, but not have any memory allocation.
static const XalanEXSLTFunctionObjectType	s_objectTypeFunction(1);



static const XalanEXSLTCommonFunctionsInstaller::FunctionTableEntry		theFunctionTable[] =
{
	{ s_nodeSetFunctionName, &s_nodesetFunction },
	{ s_objectTypeFunctionName, &s_objectTypeFunction },
	{ 0, 0 }
};



void
XalanEXSLTCommonFunctionsInstaller::installLocal(XPathEnvSupportDefault&	theSupport)
{
	doInstallLocal(s_commonNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTCommonFunctionsInstaller::installGlobal()
{
	doInstallGlobal(s_commonNamespace, theFunctionTable);
}



void
XalanEXSLTCommonFunctionsInstaller::uninstallLocal(XPathEnvSupportDefault&	theSupport)
{
	doUninstallLocal(s_commonNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTCommonFunctionsInstaller::uninstallGlobal()
{
	doUninstallGlobal(s_commonNamespace, theFunctionTable);
}
