/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
			XalanMessageLoader::getMessage(XalanMessages::FormatNumberFailed),
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

	XalanDOMString	nonLocalPattern = UnlocalizePatternFunctor(*theDFS)(thePattern);


	DecimalFormatType*	theFormatter = getCachedDecimalFormat(*theDFS);

	if (theFormatter != 0)
	{
		return doICUFormat(theNumber, nonLocalPattern, theResult, theFormatter);
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

			return doICUFormat(theNumber, nonLocalPattern, theResult, theDecimalFormat);
		}
		else
		{
			return doICUFormat(theNumber,nonLocalPattern,theResult);
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

    return U_SUCCESS(theStatus) ? true : false;
}

XalanDOMString
ICUFormatNumberFunctor::UnlocalizePatternFunctor::operator()(const XalanDOMString&	thePattern) const
{
	XalanDOMString theResult;

	XalanDecimalFormatSymbols defaultDFS;

	XalanDOMString::const_iterator iterator = thePattern.begin();

	while( iterator != thePattern.end())
	{
		
		if( m_DFS.getDecimalSeparator() == *iterator )
		{
			theResult.push_back(defaultDFS.getDecimalSeparator());
		}
		else if(m_DFS.getDigit() == *iterator)
		{
			theResult.push_back(defaultDFS.getDigit());
		}
		else if(m_DFS.getGroupingSeparator() == *iterator)
		{
			theResult.push_back(defaultDFS.getGroupingSeparator());
		}
		else if(m_DFS.getMinusSign() == *iterator)
		{
			theResult.push_back(defaultDFS.getMinusSign());
		}
		else if(m_DFS.getPatternSeparator() == *iterator)
		{
			theResult.push_back(defaultDFS.getPatternSeparator());
		}
		else if(m_DFS.getPercent() == *iterator)
		{
			theResult.push_back(defaultDFS.getPercent());
		}
		else if(m_DFS.getPerMill() == *iterator)
		{
			theResult.push_back(defaultDFS.getPerMill());
		}
		else if(m_DFS.getZeroDigit() == *iterator)
		{
			theResult.push_back(defaultDFS.getZeroDigit());
		}
		else
		{   
			switch(*iterator)
			{
			case XalanUnicode::charFullStop:
			case XalanUnicode::charNumberSign:
			case XalanUnicode::charComma:
			case XalanUnicode::charHyphenMinus:
			case XalanUnicode::charSemicolon:
			case XalanUnicode::charPercentSign:
			case XalanUnicode::charPerMilleSign:
			case XalanUnicode::charDigit_0:
				{
					theResult.push_back(XalanUnicode::charAmpersand);
					theResult.push_back(*iterator);
					theResult.push_back(XalanUnicode::charAmpersand);
				}
			}
		}

		iterator++;
	}

	return theResult;
}

XALAN_CPP_NAMESPACE_END
