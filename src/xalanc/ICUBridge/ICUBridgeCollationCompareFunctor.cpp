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

#include "ICUBridgeCollationCompareFunctor.hpp"



#include "ICUBridge.hpp"
#include "ICUBridgeCollationCompareFunctorImpl.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ICUBridgeCollationCompareFunctor::ICUBridgeCollationCompareFunctor(bool	fCacheCollators) :
	m_impl(new ICUBridgeCollationCompareFunctorImpl(fCacheCollators))
{
}



ICUBridgeCollationCompareFunctor::~ICUBridgeCollationCompareFunctor()
{
	delete m_impl;
}



int
ICUBridgeCollationCompareFunctor::operator()(
			const XalanDOMChar*					theLHS,
			const XalanDOMChar*					theRHS,
			XalanCollationServices::eCaseOrder	theCaseOrder) const
{
	return (*m_impl)(theLHS, theRHS, theCaseOrder);
}



int
ICUBridgeCollationCompareFunctor::operator()(
			const XalanDOMChar*					theLHS,
			const XalanDOMChar*					theRHS,
			const XalanDOMChar*					theLocale,
			XalanCollationServices::eCaseOrder	theCaseOrder) const
{
	return (*m_impl)(theLHS, theRHS, theLocale, theCaseOrder);
}



XALAN_CPP_NAMESPACE_END
