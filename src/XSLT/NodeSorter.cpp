/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
/**
 * 
 * @author Scott Boag (scott_boag@lotus.com)
 * @author David N. Bertoni (david_n_bertoni@lotus.com)
 */



// Class header file.
#include "NodeSorter.hpp"



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DoubleSupport.hpp>



#include <XPath/XObjectFactory.hpp>
#include <XPath/XPath.hpp>



#include "StylesheetExecutionContext.hpp"



NodeSorter::NodeSorter() :
	m_numberResultsCache(),
	m_stringResultsCache(),
	m_keys(),
	m_scratchVector()
{
}



NodeSorter::~NodeSorter()
{
}



void
NodeSorter::sort(StylesheetExecutionContext&	executionContext)
{
	assert(m_scratchVector.size() > 0);

	// Make sure the caches are cleared when we're done...
	CollectionClearGuard<NumberResultsCacheType>	guard1(m_numberResultsCache);
	CollectionClearGuard<StringResultsCacheType>	guard2(m_stringResultsCache);

	NodeSortKeyCompare	theComparer(
					executionContext,
					*this,
					m_scratchVector,
					m_keys);

#if !defined(XALAN_NO_NAMESPACES)
	using std::stable_sort;
#endif

	// Use the stl sort algorithm, which will use our compare functor,
	// which returns true if first less than second
	stable_sort(
			m_scratchVector.begin(),
			m_scratchVector.end(),
			theComparer);
}



void
NodeSorter::sort(
			StylesheetExecutionContext&		executionContext,
			MutableNodeRefList&				theList)
{
	if (m_keys.size() > 0)
	{
		const NodeRefListBase::size_type	theLength = theList.getLength();

		// Copy the nodes to a vector...
		assert(m_scratchVector.size() == 0);

		// Make sure the scratch vector is cleared when we're done...
		CollectionClearGuard<NodeVectorType>	guard(m_scratchVector);

		m_scratchVector.reserve(theLength);

		unsigned int	i = 0;

		for (; i < theLength; ++i)
		{
			m_scratchVector.push_back(NodeVectorType::value_type(theList.item(i), i));
		}

		// Do the sort...
		sort(executionContext);
		assert(m_scratchVector.size() == NodeVectorType::size_type(theLength));

		// Copy the nodes back to the list in sorted order.
		theList.clear();

		for (i = 0; i < theLength; ++i)
		{
			theList.addNode(m_scratchVector[i].m_node);
		}

		assert(theList.getLength() == theLength);
	}
}



inline StylesheetExecutionContext::eCaseOrder
caseOrderConvert(NodeSortKey::eCaseOrder	theCaseOrder)
{
	switch(theCaseOrder)
	{
	case NodeSortKey::eLowerFirst:
		return StylesheetExecutionContext::eLowerFirst;
		break;

	case NodeSortKey::eUpperFirst:
		return StylesheetExecutionContext::eUpperFirst;
		break;

	case NodeSortKey::eDefault:
		break;

	default:
		assert(false);
		break;
	}

	return StylesheetExecutionContext::eDefault;
}



static inline int
doCollationCompare(
			StylesheetExecutionContext&		executionContext,
			const XalanDOMString&			theLHS,
			const XalanDOMString&			theRHS,
			const XalanDOMString&			theLanguage,
			NodeSortKey::eCaseOrder			theCaseOrder)
{
	if (length(theLanguage) == 0)
	{
		return executionContext.collationCompare(
				theLHS,
				theRHS,
				caseOrderConvert(theCaseOrder));
	}
	else
	{
		return executionContext.collationCompare(
				theLHS,
				theRHS,
				theLanguage,
				caseOrderConvert(theCaseOrder));
	}
}



int
NodeSorter::NodeSortKeyCompare::compare(
				first_argument_type		theLHS,
				second_argument_type	theRHS,
				unsigned int			theKeyIndex) const
{
	assert(theLHS.m_node != 0 && theRHS.m_node != 0);
	assert(theKeyIndex < m_nodeSortKeys.size());

	int					theResult = 0;

	const NodeSortKey&	theKey = m_nodeSortKeys[theKeyIndex];

	// Compare as numbers
	if(theKey.getTreatAsNumbers() == false)
	{
		// Compare as strings
		const XalanDOMString&	theLHSString =
			getStringResult(theKey, theKeyIndex, theLHS)->str();

		const XalanDOMString&	theRHSString =
			getStringResult(theKey, theKeyIndex, theRHS)->str();

		theResult = doCollationCompare(
				m_executionContext,
				theLHSString,
				theRHSString,
				theKey.getLanguageString(),
				theKey.getCaseOrder());
	}
	else
	{
		double	n1Num = getNumberResult(theKey, theKeyIndex, theLHS);
		double	n2Num = getNumberResult(theKey, theKeyIndex, theRHS);

		// Always order NaN before anything else...
		if (DoubleSupport::isNaN(n1Num) == true)
		{
			if (DoubleSupport::isNaN(n2Num) == false)
			{
				theResult = -1;
			}
		}
		else if (DoubleSupport::isNaN(n2Num) == true)
		{
			theResult = 1;
		}
		else if (DoubleSupport::lessThan(n1Num, n2Num) == true)
		{
			theResult = -1;
		}
		else if (DoubleSupport::greaterThan(n1Num, n2Num) == true)
		{
			theResult = 1;
		}
	}

	// If they're not equal, the flip things if the
	// order is descending...
	if (theResult != 0)
    {
		if (theKey.getDescending() == true)
		{
			theResult = -theResult;
		}
	}
	else if(theKeyIndex + 1 < m_nodeSortKeys.size())
	{
		// They're equal, so process the next key, if any...
		theResult = compare(theLHS, theRHS, theKeyIndex + 1);
	}

	return theResult;
}



double
NodeSorter::NodeSortKeyCompare::getNumberResult(
				const NodeSortKey&		theKey,
				unsigned int			theKeyIndex,
				first_argument_type		theEntry) const
{
	const XPath* const	xpath = theKey.getSelectPattern();
	assert(xpath != 0);

	typedef	NodeSorter::NumberResultsCacheType	NumberResultsCacheType;

	NumberResultsCacheType&		theCache =
			m_sorter.m_numberResultsCache;

	if (theCache.size() == 0)
	{
		theCache.resize(m_nodeSortKeys.size());
	}

	// We need a dummy value to indicate that a slot has
	// never been evaluated.  0 is probably a bad idea,
	// as is NaN, since that would be fairly common with
	// values that are not convertible to a number.  This
	// is just a not-so-random number...
	const double	theDummyValue = 135792468.0L;

	if (theCache[theKeyIndex].size() != 0)
	{
		if (DoubleSupport::equal(theCache[theKeyIndex][theEntry.m_position], theDummyValue) == true)
		{
			theCache[theKeyIndex][theEntry.m_position] =
				xpath->execute(theEntry.m_node, *theKey.getPrefixResolver(), m_executionContext)->num();
		}

		return theCache[theKeyIndex][theEntry.m_position];
	}
	else
	{
		theCache[theKeyIndex].resize(m_nodes.size());

#if !defined(XALAN_NO_NAMESPACES)
		using std::fill;
#endif

		// Fill with the dummy value...
		fill(
			theCache[theKeyIndex].begin(),
			theCache[theKeyIndex].end(),
			theDummyValue);

		const XObjectPtr	result(xpath->execute(theEntry.m_node, *theKey.getPrefixResolver(), m_executionContext));
		assert(result.null() == false);

		const double	theResult = result->num();

		theCache[theKeyIndex][theEntry.m_position] = theResult;

		return theResult;
	}
}



const XObjectPtr&
NodeSorter::NodeSortKeyCompare::getStringResult(
				const NodeSortKey&		theKey,
				unsigned int			theKeyIndex,
				first_argument_type		theEntry) const
{
	assert(theKey.getPrefixResolver() != 0);

	const XPath* const	xpath = theKey.getSelectPattern();
	assert(xpath != 0);

	typedef	NodeSorter::StringResultsCacheType	StringResultsCacheType;

	StringResultsCacheType&		theCache =
			m_sorter.m_stringResultsCache;

	if (theCache.size() == 0)
	{
		theCache.resize(m_nodeSortKeys.size());
	}

	if (theCache[theKeyIndex].size() != 0)
	{
		if (theCache[theKeyIndex][theEntry.m_position].null() == true)
		{
			theCache[theKeyIndex][theEntry.m_position] =
				xpath->execute(theEntry.m_node, *theKey.getPrefixResolver(), m_executionContext);
		}

		assert(theCache[theKeyIndex][theEntry.m_position].null() == false);

		return theCache[theKeyIndex][theEntry.m_position];
	}
	else
	{
		theCache[theKeyIndex].resize(m_nodes.size());

		theCache[theKeyIndex][theEntry.m_position] =
			xpath->execute(theEntry.m_node, *theKey.getPrefixResolver(), m_executionContext);

		assert(theCache[theKeyIndex][theEntry.m_position].null() == false);

		return theCache[theKeyIndex][theEntry.m_position];
	}
}
