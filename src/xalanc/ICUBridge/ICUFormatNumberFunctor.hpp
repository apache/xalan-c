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
