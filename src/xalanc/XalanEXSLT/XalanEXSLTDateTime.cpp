/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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

#include "XalanEXSLTDateTime.hpp"
#include "XalanEXSLTDateTimeImpl.hpp"



#include <ctime>
#include <cstdio>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>


XALAN_CPP_NAMESPACE_BEGIN


#define MAX_DATE_TIME_LEN  100



static const XalanEXSLTFunctionDateTime			s_dateTimeFunction;


static const XalanDOMChar	s_dateTimeFunctionName[] =
{
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	0
};

static const XalanEXSLTDateTimeFunctionsInstaller::FunctionTableEntry	theFunctionTable[] =
{
	{ s_dateTimeFunctionName, &s_dateTimeFunction },
	{ 0, 0 }
};

static const XalanDOMChar	s_dateTimeNamespace[] =
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
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_d,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	0
};


XObjectPtr
XalanEXSLTFunctionDateTime::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	if (args.size() != 0)
	{
		executionContext.error(getError(), context, locator);
	}

	XPathExecutionContext::GetAndReleaseCachedString	theGuard(executionContext);

	XalanDOMString&		theResult = theGuard.get();

	time_t long_time;

	time( &long_time );
	
	const struct tm*	strctTime = localtime(&long_time);

	assert (strctTime != 0 );

	//save hours - the struct going to be overrided
	const int	iHours = strctTime->tm_hour;

	char stringTime[MAX_DATE_TIME_LEN + 1];
	
	const size_t	result = strftime(stringTime, MAX_DATE_TIME_LEN, "%Y-%m-%dT%H:%M:%S", strctTime);

	if (result == 0)
	{
		theResult.clear();
	}
	else
	{
		// Microsoft doesn't fully support the strftime definition. Let's complete the job manually
		strctTime = gmtime(&long_time);

		assert (strctTime != 0 );

		char timeZone[MAX_DATE_TIME_LEN+1];

		sprintf(timeZone , "%2.2d:00", iHours - strctTime->tm_hour);

		theResult.assign(stringTime);
		theResult.append(timeZone);
	}

	return executionContext.getXObjectFactory().createString(theResult);
}



const XalanDOMString
XalanEXSLTFunctionDateTime::getError() const
{
		return XalanMessageLoader::getMessage(XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param, s_dateTimeFunctionName);
}



void
XalanEXSLTDateTimeFunctionsInstaller::installLocal(XPathEnvSupportDefault&		theSupport)
{
	doInstallLocal(s_dateTimeNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTDateTimeFunctionsInstaller::installGlobal()
{
	doInstallGlobal(s_dateTimeNamespace, theFunctionTable);

}



void
XalanEXSLTDateTimeFunctionsInstaller::uninstallLocal(XPathEnvSupportDefault&	theSupport)
{
	doUninstallLocal(s_dateTimeNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTDateTimeFunctionsInstaller::uninstallGlobal()
{
	doUninstallGlobal(s_dateTimeNamespace, theFunctionTable);
}



XALAN_CPP_NAMESPACE_END
