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
#if !defined(FUNCTIONICUFORMATNUMBERFUNCTOR_HEADER_GUARD_1357924680)
#define FUNCTIONICUFORMATNUMBERFUNCTOR_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/ICUBridge/ICUBridgeDefinitions.hpp>



#include <xalanc/PlatformSupport/XalanDecimalFormatSymbols.hpp>



#include <xalanc/XSLT/StylesheetExecutionContextDefault.hpp>



#include <xalanc/XPath/XPathExecutionContext.hpp>



#include <unicode/decimfmt.h>



#include <list>



XALAN_CPP_NAMESPACE_BEGIN

typedef StylesheetExecutionContextDefault::FormatNumberFunctor FormatNumberFunctor;
// Class that implements the XSLT function format-number using the ICU.
//
class XALAN_ICUBRIDGE_EXPORT ICUFormatNumberFunctor : public FormatNumberFunctor
{
public:

	ICUFormatNumberFunctor();

	virtual
	~ICUFormatNumberFunctor();

	virtual void
		operator() (
		XPathExecutionContext&				executionContext,
		double								theNumber,
		const XalanDOMString&				thePattern,
		const XalanDecimalFormatSymbols*	theDFS,
		XalanDOMString&						theResult,
		const XalanNode*					context = 0,
		const LocatorType*					locator = 0) const;
	
#if defined(XALAN_HAS_CPP_NAMESPACE)
	typedef U_ICU_NAMESPACE::DecimalFormat	DecimalFormatType;
#else
	typedef DecimalFormat					DecimalFormatType;
#endif

	class UnlocalizePatternFunctor
	{
	public:
		UnlocalizePatternFunctor(const XalanDecimalFormatSymbols&	theDFS):
			m_DFS(theDFS)
		{
		}

		XalanDOMString
		operator()(const XalanDOMString&	thePattern) const;

	private:
		const XalanDecimalFormatSymbols& m_DFS;
	};

	struct DecimalFormatCacheStruct
	{
		DecimalFormatCacheStruct(
				const XalanDecimalFormatSymbols&	theDFS,
				DecimalFormatType*					theFormatter):
			m_DFS(theDFS),
			m_formatter(theFormatter)
		{
		}

		DecimalFormatCacheStruct() :
			m_DFS(),
			m_formatter(0)
		{
		}

#if defined(XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION)
		bool
		operator<(const DecimalFormatCacheStruct&  theRHS) const
		{
			return this < &theRHS;
		}

		bool
		operator==(const DecimalFormatCacheStruct&	theRHS) const
		{
			return this == &theRHS;
		}
#endif

		XalanDecimalFormatSymbols	m_DFS;

		DecimalFormatType *	m_formatter;

		struct DecimalFormatDeleteFunctor
		{
			void
			operator()(DecimalFormatCacheStruct&	theStruct) const
			{
				delete theStruct.m_formatter;
			}
		};

		struct DecimalFormatFindFunctor
		{
			DecimalFormatFindFunctor(const XalanDecimalFormatSymbols*	theDFS) :
				m_DFS(theDFS)
			{
			}

			bool
			operator()(DecimalFormatCacheStruct&	theStruct) const
			{
				return theStruct.m_DFS == (*m_DFS);
			}

			const XalanDecimalFormatSymbols * const	m_DFS;
		};
	};

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef list<DecimalFormatCacheStruct>			DecimalFormatCacheListType;
#else
	typedef std::list<DecimalFormatCacheStruct>		DecimalFormatCacheListType;
#endif

	DecimalFormatType * getCachedDecimalFormat(const XalanDecimalFormatSymbols &theDFS) const;

	bool
	doFormat(	
		double								theNumber,
		const XalanDOMString&				thePattern,
		XalanDOMString&						theResult,
		const XalanDecimalFormatSymbols*	theDFS = 0) const;

	bool
	doICUFormat(
		double								theNumber,
		const XalanDOMString&				thePattern,
		XalanDOMString&						theResult,
		DecimalFormatType*					theFormatter = 0) const;

	void
	cacheDecimalFormat(
		DecimalFormatType*					theFormatter,
		const XalanDecimalFormatSymbols&	theDFS) const;

	DecimalFormat *
	createDecimalFormat(
		const XalanDecimalFormatSymbols&	theXalanDFS) const;

	enum { eCacheMax = 10 };

	DecimalFormatType * m_defaultDecimalFormat;
	
	mutable DecimalFormatCacheListType m_decimalFormatCache;

	ICUFormatNumberFunctor&
	operator=(const ICUFormatNumberFunctor&);

	bool
	operator==(const ICUFormatNumberFunctor&) const;

};

XALAN_CPP_NAMESPACE_END

#endif	// FUNCTIONICUFORMATNUMBERFUNCTOR_HEADER_GUARD_1357924680
