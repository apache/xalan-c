/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000-2002 The Apache Software Foundation.  All rights 
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

#include "ICUBridgeCollationCompareFunctorImpl.hpp"
#include "ICUBridge.hpp"



#include <Include/XalanAutoPtr.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



// this is the ICU's macro for using namespace ...
U_NAMESPACE_USE



const StylesheetExecutionContextDefault::DefaultCollationCompareFunctor		ICUBridgeCollationCompareFunctorImpl::s_defaultFunctor;



inline Collator*
createCollator(UErrorCode&	theStatus)
{
#if defined(XALAN_ICU_DEFAULT_LOCALE_PROBLEM)
	return Collator::createInstance(Locale::US, theStatus);
#else
	return Collator::createInstance(theStatus);
#endif
}



ICUBridgeCollationCompareFunctorImpl::ICUBridgeCollationCompareFunctorImpl() :
	m_isValid(false),
	m_defaultCollator(0)
{
	UErrorCode	theStatus = U_ZERO_ERROR;

	m_defaultCollator = createCollator(theStatus);

	if (theStatus == U_ZERO_ERROR ||
	    (theStatus >= U_ERROR_INFO_START && theStatus < U_ERROR_INFO_LIMIT))
	{
		m_isValid = true;
	}
}



ICUBridgeCollationCompareFunctorImpl::~ICUBridgeCollationCompareFunctorImpl()
{
	delete m_defaultCollator;
}



inline int
doCompare(
			const Collator&			theCollator,
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS)
{
#if defined(XALAN_USE_WCHAR_CAST_HACK)
	return theCollator.compare(
				(const wchar_t*)theLHS,
				length(theLHS),
				(const wchar_t*)theRHS,
				length(theRHS));
#else
	return theCollator.compare(
				theLHS,
				length(theLHS),
				theRHS,
				length(theRHS));
#endif
}



inline UColAttributeValue
caseOrderConvert(ICUBridgeCollationCompareFunctorImpl::eCaseOrder	theCaseOrder)
{
	switch(theCaseOrder)
	{
	case StylesheetExecutionContext::eLowerFirst:
		return UCOL_LOWER_FIRST;
		break;

	case StylesheetExecutionContext::eUpperFirst:
		return UCOL_UPPER_FIRST;
		break;

	case StylesheetExecutionContext::eDefault:
		break;

	default:
		assert(false);
		break;
	}

	return UCOL_DEFAULT;
}



int
ICUBridgeCollationCompareFunctorImpl::doDefaultCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			eCaseOrder				theCaseOrder) const
{
	if (isValid() == false)
	{
		return s_defaultFunctor(theLHS, theRHS, theCaseOrder);
	}
	else if (theCaseOrder == StylesheetExecutionContext::eDefault)
	{
		assert(m_defaultCollator != 0);

		return doCompare(*m_defaultCollator, theLHS, theRHS);
	}
	else
	{
		UErrorCode	theStatus = U_ZERO_ERROR;

		XalanAutoPtr<Collator>	theCollator(createCollator(theStatus));

		if (theStatus == U_ZERO_ERROR ||
			(theStatus >= U_ERROR_INFO_START && theStatus < U_ERROR_INFO_LIMIT))
		{
			theCollator->setAttribute(
					UCOL_CASE_FIRST,
					caseOrderConvert(theCaseOrder),
					theStatus);

			return doCompare(*theCollator.get(), theLHS, theRHS);
		}
		else
		{
			return s_defaultFunctor(theLHS, theRHS, theCaseOrder);
		}
	}
}



int
ICUBridgeCollationCompareFunctorImpl::operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			eCaseOrder				theCaseOrder) const
{
	return doDefaultCompare(theLHS, theRHS, theCaseOrder);
}



inline Collator*
getCollator(
			const XalanDOMChar*		theLocale,
			UErrorCode&				theStatus)
{
	const XalanDOMString::size_type		theLength = length(theLocale);

	if (theLength >= ULOC_FULLNAME_CAPACITY)
	{
		theStatus = U_ILLEGAL_ARGUMENT_ERROR;

		return 0;
	}
	else
	{
#if defined(XALAN_NON_ASCII_PLATFORM)
		CharVectorType	theVector;

		TranscodeToLocalCodePage(theLocale, theVector, true);

		const char* const	theBuffer = c_str(theVector);
#else
		char	theBuffer[ULOC_FULLNAME_CAPACITY];

		// Since language names must be ASCII, this
		// is the cheapest way to "transcode"...
		for (unsigned int i = 0; i <= theLength; ++i)
		{
			theBuffer[i] = char(theLocale[i]);
		}
#endif
		return Collator::createInstance(
					Locale::createFromName(theBuffer),
					theStatus);
	}
}



int
ICUBridgeCollationCompareFunctorImpl::operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			const XalanDOMChar*		theLocale,
			eCaseOrder				theCaseOrder) const
{
	UErrorCode	theStatus = U_ZERO_ERROR;

	XalanAutoPtr<Collator>	theCollator(getCollator(theLocale, theStatus));

	if (theStatus == U_ZERO_ERROR ||
	    (theStatus >= U_ERROR_INFO_START && theStatus < U_ERROR_INFO_LIMIT))
	{
		assert(theCollator.get() != 0);

		theCollator->setAttribute(
				UCOL_CASE_FIRST,
				caseOrderConvert(theCaseOrder),
				theStatus);

#if defined(XALAN_USE_WCHAR_CAST_HACK)
		return theCollator->compare(
					(const wchar_t*)theLHS,
					length(theLHS),
					(const wchar_t*)theRHS,
					length(theRHS));
#else
		return theCollator->compare(
					theLHS,
					length(theLHS),
					theRHS,
					length(theRHS));
#endif
	}
	else
	{
		return s_defaultFunctor(theLHS, theRHS, theCaseOrder);
	}
}
