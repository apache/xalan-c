/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
#include "XalanExtensions.hpp"

#include "FunctionDifference.hpp"
#include "FunctionDistinct.hpp"
#include "FunctionEvaluate.hpp"
#include "FunctionHasSameNodes.hpp"
#include "FunctionIntersection.hpp"
#include "FunctionNodeSet.hpp"



#include <xalanc/XPath/XPathEnvSupportDefault.hpp>



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMChar	s_extensionsNamespace[] =
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



static const XalanDOMChar	s_differenceFunctionName[] =
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



static const XalanDOMChar	s_distinctFunctionName[] =
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



static const XalanDOMChar	s_evaluateFunctionName[] =
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



static const XalanDOMChar	s_hasSameNodesFunctionName[] =
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



static const XalanDOMChar	s_intersectionFunctionName[] =
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



static const XalanDOMChar	s_nodeSetFunctionName1[] =
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



static const XalanDOMChar	s_nodeSetFunctionName2[] =
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



static const FunctionDifference		s_differenceFunction;
static const FunctionDistinct		s_distinctFunction;
static const FunctionEvaluate		s_evaluateFunction;
static const FunctionHasSameNodes	s_hasSameNodesFunction;
static const FunctionIntersection	s_intersectionFunction;
static const FunctionNodeSet		s_nodesetFunction(true);



static const XalanExtensionsInstaller::FunctionTableEntry	theFunctionTable[] =
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
XalanExtensionsInstaller::installLocal(XPathEnvSupportDefault&	theSupport)
{
	doInstallLocal(s_extensionsNamespace, theFunctionTable, theSupport);
}



void
XalanExtensionsInstaller::installGlobal()
{
	doInstallGlobal(s_extensionsNamespace, theFunctionTable);
}



void
XalanExtensionsInstaller::uninstallLocal(XPathEnvSupportDefault&	theSupport)
{
	doUninstallLocal(s_extensionsNamespace, theFunctionTable, theSupport);
}



void
XalanExtensionsInstaller::uninstallGlobal()
{
	doUninstallGlobal(s_extensionsNamespace, theFunctionTable);
}



void
XalanExtensionsInstaller::doInstallLocal(
			const XalanDOMChar*			theNamespace,
			const FunctionTableEntry	theFunctionTable[],
			XPathEnvSupportDefault&		theSupport)
{
	assert(theNamespace != 0);

	const XalanDOMString	theNamespaceString(theNamespace);
	XalanDOMString			theFunctionName;

	for(const FunctionTableEntry* theEntry = theFunctionTable; theEntry->theFunctionName != 0; ++theEntry)
	{
		assert(theEntry->theFunction != 0);

		theFunctionName = theEntry->theFunctionName;

		theSupport.installExternalFunctionLocal(theNamespaceString, theFunctionName, *theEntry->theFunction);
	}
}



void
XalanExtensionsInstaller::doInstallGlobal(
			const XalanDOMChar*			theNamespace,
			const FunctionTableEntry	theFunctionTable[])
{
	assert(theNamespace != 0);

	const XalanDOMString	theNamespaceString(theNamespace);
	XalanDOMString			theFunctionName;

	for(const FunctionTableEntry* theEntry = theFunctionTable; theEntry->theFunctionName != 0; ++theEntry)
	{
		assert(theEntry->theFunction != 0);

		theFunctionName = theEntry->theFunctionName;

		XPathEnvSupportDefault::installExternalFunctionGlobal(theNamespaceString, theFunctionName, *theEntry->theFunction);
	}
}



void
XalanExtensionsInstaller::doUninstallLocal(
			const XalanDOMChar*			theNamespace,
			const FunctionTableEntry	theFunctionTable[],
			XPathEnvSupportDefault&		theSupport)
{
	assert(theNamespace != 0);

	const XalanDOMString	theNamespaceString(theNamespace);
	XalanDOMString			theFunctionName;

	for(const FunctionTableEntry* theEntry = theFunctionTable; theEntry->theFunctionName != 0; ++theEntry)
	{
		theFunctionName = theEntry->theFunctionName;

		theSupport.uninstallExternalFunctionLocal(theNamespaceString, theFunctionName);
	}
}



void
XalanExtensionsInstaller::doUninstallGlobal(
			const XalanDOMChar*			theNamespace,
			const FunctionTableEntry	theFunctionTable[])
{
	assert(theNamespace != 0);

	const XalanDOMString	theNamespaceString(theNamespace);
	XalanDOMString			theFunctionName;

	for(const FunctionTableEntry* theEntry = theFunctionTable; theEntry->theFunctionName != 0; ++theEntry)
	{
		theFunctionName = theEntry->theFunctionName;

		XPathEnvSupportDefault::uninstallExternalFunctionGlobal(theNamespaceString, theFunctionName);
	}
}



XALAN_CPP_NAMESPACE_END
