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



#include <algorithm>
#include <cstdlib>



#include <xalanc/Include/XalanAutoPtr.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const StylesheetExecutionContextDefault::DefaultCollationCompareFunctor		ICUBridgeCollationCompareFunctorImpl::s_defaultFunctor;



inline ICUBridgeCollationCompareFunctorImpl::CollatorType*
createCollator(
			UErrorCode&			theStatus,
			const Locale&		theLocale,
			XalanDOMString*		theLocaleName = 0)
{
	typedef ICUBridgeCollationCompareFunctorImpl::CollatorType	CollatorType;

	if (theLocaleName != 0)
	{
		*theLocaleName = theLocale.getName();

		// Replace _ with -, since that's what xml:lang specifies...
		XalanDOMString::size_type	theIndex;

		while((theIndex = indexOf(*theLocaleName, XalanUnicode::charLowLine)) != theLocaleName->length())
		{
			(*theLocaleName)[theIndex] = XalanUnicode::charHyphenMinus;
		}
	}

	return CollatorType::createInstance(theLocale, theStatus);
}



inline ICUBridgeCollationCompareFunctorImpl::CollatorType*
createCollator(
			UErrorCode&			theStatus,
			XalanDOMString*		theLocaleName = 0)
{
	const char*		theLang =
#if defined(XALAN_STRICT_ANSI_HEADERS)
			std::getenv("LANG");
#else
			getenv("LANG");
#endif

	if (theLang == 0)
	{
#if defined(XALAN_ICU_DEFAULT_LOCALE_PROBLEM)
		return createCollator(theStatus, Locale::US, theLocaleName);
#else
		return createCollator(theStatus, Locale::getDefault(), theLocaleName);
#endif
	}
	else
	{
		return createCollator(theStatus, Locale(theLang), theLocaleName);
	}
}



ICUBridgeCollationCompareFunctorImpl::ICUBridgeCollationCompareFunctorImpl(bool		fCacheCollators) :
	m_isValid(false),
	m_defaultCollator(0),
	m_defaultCollatorLocaleName(),
	m_cacheCollators(fCacheCollators),
	m_collatorCache()
{
	UErrorCode	theStatus = U_ZERO_ERROR;

	m_defaultCollator = createCollator(theStatus, &m_defaultCollatorLocaleName);

	if (U_SUCCESS(theStatus))
	{
		m_isValid = true;
	}
}



ICUBridgeCollationCompareFunctorImpl::~ICUBridgeCollationCompareFunctorImpl()
{
	XALAN_USING_STD(for_each)

	delete m_defaultCollator;

	for_each(
			m_collatorCache.begin(),
			m_collatorCache.end(),
			CollationCacheStruct::CollatorDeleteFunctor());
}



int
ICUBridgeCollationCompareFunctorImpl::doCompare(
			const CollatorType&		theCollator,
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS) const
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
			const XalanDOMChar*		theRHS) const
{
	if (isValid() == false)
	{
		return s_defaultFunctor(theLHS, theRHS, StylesheetExecutionContext::eDefault);
	}
	else
	{
		assert(m_defaultCollator != 0);

		return doCompare(*m_defaultCollator, theLHS, theRHS);
	}
}



inline ICUBridgeCollationCompareFunctorImpl::CollatorType*
createCollator(
			const XalanDOMChar*		theLocale,
			UErrorCode&				theStatus)
{
	assert(theLocale != 0);

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
		return ICUBridgeCollationCompareFunctorImpl::CollatorType::createInstance(
					Locale::createFromName(theBuffer),
					theStatus);
	}
}



int
ICUBridgeCollationCompareFunctorImpl::doCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			const XalanDOMChar*		theLocale,
			eCaseOrder				theCaseOrder) const
{
	UErrorCode	theStatus = U_ZERO_ERROR;

	XalanAutoPtr<CollatorType>	theCollator(createCollator(theLocale, theStatus));

	if (U_SUCCESS(theStatus))
	{
		assert(theCollator.get() != 0);

		return doCompare(
				*theCollator.get(),
				theLHS,
				theRHS,
				theCaseOrder);
	}
	else
	{
		return s_defaultFunctor(theLHS, theRHS, theCaseOrder);
	}
}



int
ICUBridgeCollationCompareFunctorImpl::doCompareCached(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			const XalanDOMChar*		theLocale,
			eCaseOrder				theCaseOrder) const
{
	CollatorType*	theCollator = getCachedCollator(theLocale);

	if (theCollator != 0)
	{
		return doCompare(*theCollator, theLHS, theRHS, theCaseOrder);
	}
	else
	{
		UErrorCode	theStatus = U_ZERO_ERROR;

		XalanAutoPtr<CollatorType>	theCollatorGuard(createCollator(theLocale, theStatus));

		if (U_SUCCESS(theStatus))
		{
			assert(theCollatorGuard.get() != 0);

			// OK, there was no error, so cache the instance...
			cacheCollator(theCollatorGuard.get(), theLocale);

			// Release the collator, since it's in the cache and
			// will be controlled by the cache...
			theCollator = theCollatorGuard.release();

			return doCompare(
					*theCollator,
					theLHS,
					theRHS,
					theCaseOrder);
		}
		else
		{
			return s_defaultFunctor(theLHS, theRHS, theCaseOrder);
		}
	}
}



int
ICUBridgeCollationCompareFunctorImpl::doCompare(
			CollatorType&			theCollator,
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			eCaseOrder				theCaseOrder) const
{
	UErrorCode	theStatus = U_ZERO_ERROR;

	theCollator.setAttribute(
				UCOL_CASE_FIRST,
				caseOrderConvert(theCaseOrder),
				theStatus);

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



int
ICUBridgeCollationCompareFunctorImpl::operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			eCaseOrder				theCaseOrder) const
{
	if (theCaseOrder == StylesheetExecutionContext::eDefault)
	{
		return doDefaultCompare(theLHS, theRHS);
	}
	else
	{
		return doCompare(
				theLHS,
				theRHS,
				c_wstr(m_defaultCollatorLocaleName),
				theCaseOrder);
	}
}



int
ICUBridgeCollationCompareFunctorImpl::operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			const XalanDOMChar*		theLocale,
			eCaseOrder				theCaseOrder) const
{
	if (theCaseOrder == StylesheetExecutionContext::eDefault &&
		XalanDOMString::equals(m_defaultCollatorLocaleName, theLocale) == true)
	{
		return doDefaultCompare(theLHS, theRHS);
	}
	else if (m_cacheCollators == true)
	{
		return doCompareCached(theLHS, theRHS, theLocale, theCaseOrder);
	}
	else
	{
		return doCompare(theLHS, theRHS, theLocale, theCaseOrder);
	};
}



ICUBridgeCollationCompareFunctorImpl::CollatorType*
ICUBridgeCollationCompareFunctorImpl::getCachedCollator(const XalanDOMChar*		theLocale) const
{
	XALAN_USING_STD(find_if)

	CollatorCacheListType&		theNonConstCache =
#if defined(XALAN_NO_MUTABLE)
		(CollatorCacheListType&)m_collatorCache;
#else
		m_collatorCache;
#endif

	CollatorCacheListType::iterator	i =
		find_if(
			theNonConstCache.begin(),
			theNonConstCache.end(),
			CollationCacheStruct::CollatorFindFunctor(theLocale));

	if (i == theNonConstCache.end())
	{
		return 0;
	}
	else
	{
		// Let's do a quick check to see if we found the first entry.
		// If so, we don't have to update the cache, so just return the
		// appropriate value...
		const CollatorCacheListType::iterator	theBegin =
			theNonConstCache.begin();

		if (i == theBegin)
		{
			return (*i).m_collator;
		}
		else
		{
			// Save the collator, because splice() may invalidate
			// i.
			CollatorType* const		theCollator = (*i).m_collator;

			// Move the entry to the beginning the cache
			theNonConstCache.splice(theBegin, theNonConstCache, i);

			return theCollator;
		}
	}
}



void
ICUBridgeCollationCompareFunctorImpl::cacheCollator(
			CollatorType*			theCollator,
			const XalanDOMChar*		theLocale) const
{
	assert(theCollator != 0);
	assert(theLocale != 0);

	CollatorCacheListType&		theNonConstCache =
#if defined(XALAN_NO_MUTABLE)
		(CollatorCacheListType&)m_collatorCache;
#else
		m_collatorCache;
#endif

	// Is the cache full?
	if (theNonConstCache.size() == eCacheMax)
	{
		// Yes, so guard the collator instance, in case pop_back() throws...
		XalanAutoPtr<CollatorType>	theCollatorGuard(theNonConstCache.back().m_collator);

		theNonConstCache.pop_back();
	}

	theNonConstCache.push_front(CollatorCacheListType::value_type());

	CollatorCacheListType::value_type&		theEntry = 
		theNonConstCache.front();

	// Set the locale first, since that might throw an exception...
	theEntry.m_locale = theLocale;

	theEntry.m_collator = theCollator;
}



XALAN_CPP_NAMESPACE_END
