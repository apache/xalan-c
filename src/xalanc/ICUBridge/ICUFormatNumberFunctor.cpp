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



#include <algorithm>



#include "ICUFormatNumberFunctor.hpp"



#include <xalanc/ICUBridge/ICUBridge.hpp>



#include <xalanc/Include/XalanAutoPtr.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanDecimalFormatSymbols.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XPathExecutionContext.hpp>



XALAN_CPP_NAMESPACE_BEGIN



ICUFormatNumberFunctor::ICUFormatNumberFunctor()
{
	XalanDecimalFormatSymbols defaultXalanDFS;

	m_defaultDecimalFormat = createDecimalFormat(defaultXalanDFS);
}


	
ICUFormatNumberFunctor::~ICUFormatNumberFunctor()
{
	delete m_defaultDecimalFormat;

	XALAN_USING_STD(for_each)
	for_each(
		m_decimalFormatCache.begin(),
		m_decimalFormatCache.end(),
		DecimalFormatCacheStruct::DecimalFormatDeleteFunctor());
}



void
ICUFormatNumberFunctor::operator() (
		XPathExecutionContext&				executionContext,	
		double								theNumber,
		const XalanDOMString&				thePattern,
		const XalanDecimalFormatSymbols*	theDFS,
		XalanDOMString&						theResult,
		const XalanNode*					context,
		const LocatorType*					locator) const 
{
	if (!doFormat(theNumber, thePattern, theResult, theDFS))
	{
		executionContext.warn(
			XalanMessageLoader::getMessage(XalanMessages::FormatNumberFailed_1Param),
			context,
			locator);
	}		
}



ICUFormatNumberFunctor::DecimalFormatType * 
ICUFormatNumberFunctor::getCachedDecimalFormat(const XalanDecimalFormatSymbols &theDFS) const
{
	XALAN_USING_STD(find_if)

	DecimalFormatCacheListType&		theNonConstCache =
#if defined(XALAN_NO_MUTABLE)
		(DecimalFormatCacheListType&)m_decimalFormatCache;
#else
		m_decimalFormatCache;
#endif

	DecimalFormatCacheListType::iterator i =
		find_if(
			theNonConstCache.begin(),
			theNonConstCache.end(),
			DecimalFormatCacheStruct::DecimalFormatFindFunctor(&theDFS));

	if (i == theNonConstCache.end())
	{
		return 0;
	}
	else
	{
		// Let's do a quick check to see if we found the first entry.
		// If so, we don't have to update the cache, so just return the
		// appropriate value...
		const DecimalFormatCacheListType::iterator	theBegin =
			theNonConstCache.begin();

		if (i == theBegin)
		{
			return (*i).m_formatter;
		}
		else
		{
			// Save the formatter, because splice() may invalidate
			// i.
			DecimalFormatType* const		theFormatter = (*i).m_formatter;

			// Move the entry to the beginning the cache
			theNonConstCache.splice(theBegin, theNonConstCache, i);

			return theFormatter;
		}
	}
}



bool
ICUFormatNumberFunctor::doFormat(	
		double								theNumber,
		const XalanDOMString&				thePattern,
		XalanDOMString&						theResult,
		const XalanDecimalFormatSymbols*	theDFS) const
{
	if (theDFS == 0)
	{
		return doICUFormat(theNumber,thePattern,theResult);
	}

	DecimalFormatType*	theFormatter = getCachedDecimalFormat(*theDFS);

	if (theFormatter != 0)
	{
		return doICUFormat(theNumber, thePattern, theResult, theFormatter);
	}
	else
	{
		XalanAutoPtr<DecimalFormatType>	theDecimalFormatGuard(createDecimalFormat(*theDFS));

		if (theDecimalFormatGuard.get() !=0)
		{
			// OK, there was no error, so cache the instance...
			cacheDecimalFormat(theDecimalFormatGuard.get(), *theDFS);

			// Release the collator, since it's in the cache and
			// will be controlled by the cache...
			DecimalFormatType * theDecimalFormat = theDecimalFormatGuard.release();

			return doICUFormat(theNumber, thePattern, theResult, theDecimalFormat);
		}
		else
		{
			return doICUFormat(theNumber,thePattern,theResult);
		}
	}
}



ICUFormatNumberFunctor::DecimalFormatType *
ICUFormatNumberFunctor::createDecimalFormat(
		const XalanDecimalFormatSymbols&	theXalanDFS) const
{
	UErrorCode theStatus = U_ZERO_ERROR;	
	
	// Use a XalanAutoPtr, to keep this safe until we construct the DecimalFormat instance.
	XalanAutoPtr<DecimalFormatSymbols>	theDFS(new DecimalFormatSymbols(theStatus));
	// We got a XalanDecimalFormatSymbols, so set the
	// corresponding data in the ICU DecimalFormatSymbols.
	theDFS->setSymbol(DecimalFormatSymbols::kZeroDigitSymbol, UChar(theXalanDFS.getZeroDigit()));
	theDFS->setSymbol(DecimalFormatSymbols::kGroupingSeparatorSymbol, UChar(theXalanDFS.getGroupingSeparator()));
	theDFS->setSymbol(DecimalFormatSymbols::kDecimalSeparatorSymbol, UChar(theXalanDFS.getDecimalSeparator()));
	theDFS->setSymbol(DecimalFormatSymbols::kPerMillSymbol, UChar(theXalanDFS.getPerMill()));
	theDFS->setSymbol(DecimalFormatSymbols::kPercentSymbol, UChar(theXalanDFS.getPercent()));
	theDFS->setSymbol(DecimalFormatSymbols::kDigitSymbol, UChar(theXalanDFS.getDigit()));
	theDFS->setSymbol(DecimalFormatSymbols::kPatternSeparatorSymbol, UChar(theXalanDFS.getPatternSeparator()));

	theDFS->setSymbol(DecimalFormatSymbols::kInfinitySymbol, ICUBridge::XalanDOMStringToUnicodeString(theXalanDFS.getInfinity()));
	theDFS->setSymbol(DecimalFormatSymbols::kNaNSymbol, ICUBridge::XalanDOMStringToUnicodeString(theXalanDFS.getNaN()));
	theDFS->setSymbol(DecimalFormatSymbols::kMinusSignSymbol, UChar(theXalanDFS.getMinusSign()));
	theDFS->setSymbol(DecimalFormatSymbols::kCurrencySymbol, ICUBridge::XalanDOMStringToUnicodeString(theXalanDFS.getCurrencySymbol()));
	theDFS->setSymbol(DecimalFormatSymbols::kIntlCurrencySymbol, ICUBridge::XalanDOMStringToUnicodeString(theXalanDFS.getInternationalCurrencySymbol()));
	theDFS->setSymbol(DecimalFormatSymbols::kMonetarySeparatorSymbol, UChar(theXalanDFS.getMonetaryDecimalSeparator()));

	// Construct a DecimalFormat.  Note that we release the XalanAutoPtr, since the
	// DecimalFormat will adopt the DecimalFormatSymbols instance.
	DecimalFormatType * theFormatter= new DecimalFormatType("", theDFS.release(), theStatus);

	if (U_SUCCESS(theStatus))
	{
		return theFormatter;
	} 
	else 
	{
		delete theFormatter;
		return 0;
	}
}



void
ICUFormatNumberFunctor::cacheDecimalFormat(
	DecimalFormatType *						theFormatter,
	const XalanDecimalFormatSymbols&		theDFS) const
{

	assert(theFormatter != 0);

	DecimalFormatCacheListType&		theNonConstCache =
#if defined(XALAN_NO_MUTABLE)
		(DecimalFormatCacheListType&)m_decimalFormatCache;
#else
		m_decimalFormatCache;
#endif

	// Is the cache full?
	if (theNonConstCache.size() == eCacheMax)
	{
		// Yes, so guard the collator instance, in case pop_back() throws...
		XalanAutoPtr<DecimalFormatType>	theDecimalFormatGuard(theNonConstCache.back().m_formatter);

		theNonConstCache.pop_back();
	}


	theNonConstCache.push_front(DecimalFormatCacheListType::value_type());

	DecimalFormatCacheListType::value_type&		theEntry = 
		theNonConstCache.front();

	theEntry.m_formatter = theFormatter;
	theEntry.m_DFS = theDFS;
}



bool
ICUFormatNumberFunctor::doICUFormat(
		double								theNumber,
		const XalanDOMString&				thePattern,
		XalanDOMString&						theResult,
		DecimalFormatType*					theFormatter) const
{
	UErrorCode theStatus = U_ZERO_ERROR;

	if (theFormatter == 0)
	{
		if (m_defaultDecimalFormat != 0) 
		{
			theFormatter = m_defaultDecimalFormat;
		}
		else
		{
			return false;
		}
	}

	theFormatter->applyPattern(ICUBridge::XalanDOMStringToUnicodeString(thePattern),theStatus);

	if (U_SUCCESS(theStatus))
	{
		// Do the format...
		UnicodeString	theUnicodeResult;
		theFormatter->format(theNumber, theUnicodeResult);
		ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult, theResult);
	}
	return U_SUCCESS(theStatus);
}



XALAN_CPP_NAMESPACE_END
