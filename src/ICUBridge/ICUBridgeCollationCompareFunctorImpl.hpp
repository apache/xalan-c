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

#if !defined(ICUBRIDGE_COLLATIONCOMPAREFUNCTORIMPL_GUARD_1357924680)
#define ICUBRIDGE_COLLATIONCOMPAREFUNCTORIMPL_GUARD_1357924680



#include <ICUBridge/ICUBridgeDefinitions.hpp>



#include <list>



#include <XSLT/StylesheetExecutionContextDefault.hpp>



#include <unicode/coll.h>




XALAN_CPP_NAMESPACE_BEGIN



class XALAN_ICUBRIDGE_EXPORT ICUBridgeCollationCompareFunctorImpl : public StylesheetExecutionContextDefault::CollationCompareFunctor
{
public:

	typedef StylesheetExecutionContextDefault::eCaseOrder	eCaseOrder;


	/**
	 * Constructor.
	 * 
	 * @param fCacheCollators If true, the instance will cache collators.  This is not thread-safe, so each thread must have its own instance.
	 */
	ICUBridgeCollationCompareFunctorImpl(bool	fCacheCollators = false);

	~ICUBridgeCollationCompareFunctorImpl();

	int
	operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			eCaseOrder				theCaseOrder = StylesheetExecutionContextDefault::eDefault) const;

	int
	operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			const XalanDOMChar*		theLocale,
			eCaseOrder				theCaseOrder = StylesheetExecutionContextDefault::eDefault) const;

	bool
	isValid() const
	{
		return m_isValid;
	}

#if defined(XALAN_NO_NAMESPACES)
	typedef Collator					CollatorType;
#else
	typedef U_ICU_NAMESPACE::Collator	CollatorType;
#endif

	struct CollationCacheStruct
	{
		CollationCacheStruct(
				const XalanDOMString&	theLocale,
				CollatorType*			theCollator) :
			m_locale(theLocale),
			m_collator(theCollator)
		{
		}

		CollationCacheStruct() :
			m_locale(),
			m_collator(0)
		{
		}

		void
		swap(CollationCacheStruct&	theOther)
		{
			m_locale.swap(theOther.m_locale);

			CollatorType* const		theTemp = m_collator;

			m_collator = theOther.m_collator;

			theOther.m_collator = theTemp;
		}

		XalanDOMString	m_locale;

		CollatorType*	m_collator;

		struct CollatorDeleteFunctor
		{
			void
			operator()(CollationCacheStruct&	theStruct) const
			{
				delete theStruct.m_collator;
			}
		};

		struct CollatorFindFunctor
		{
			CollatorFindFunctor(const XalanDOMChar*	theLocale) :
				m_locale(theLocale)
			{
			}

			bool
			operator()(CollationCacheStruct&	theStruct) const
			{
				return XalanDOMString::equals(theStruct.m_locale ,m_locale);
			}

			const XalanDOMChar* const	m_locale;
		};
	};

#if defined(XALAN_NO_NAMESPACES)
	typedef list<CollationCacheStruct>			CollatorCacheListType;
#else
	typedef std::list<CollationCacheStruct>		CollatorCacheListType;
#endif

	enum { eCacheMax = 10 };

private:

	int
	doDefaultCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS) const;

	int
	doCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			const XalanDOMChar*		theLocale,
			eCaseOrder				theCaseOrder) const;

	int
	doCompareCached(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			const XalanDOMChar*		theLocale,
			eCaseOrder				theCaseOrder) const;

	int
	doCompare(
			const CollatorType&		theCollator,
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS) const;

	int
	doCompare(
			CollatorType&			theCollator,
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			eCaseOrder				theCaseOrder) const;

	CollatorType*
	getCachedCollator(const XalanDOMChar*	theLocale) const;

	void
	cacheCollator(
			CollatorType*			theCollator,
			const XalanDOMChar*		theLocale) const;


	// Data members...
	bool							m_isValid;

	CollatorType*					m_defaultCollator;

	XalanDOMString					m_defaultCollatorLocaleName;

	bool							m_cacheCollators;

	mutable CollatorCacheListType	m_collatorCache;

	const static StylesheetExecutionContextDefault::DefaultCollationCompareFunctor	s_defaultFunctor;
};



XALAN_CPP_NAMESPACE_END



#endif	// ICUBRIDGE_COLLATIONCOMPAREFUNCTORIMPL_GUARD_1357924680
