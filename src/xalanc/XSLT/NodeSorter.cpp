/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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

// Class header file.
#include "NodeSorter.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>



#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



NodeSorter::NodeSorter(MemoryManager& theManager) :
    m_numberResultsCache(theManager),
    m_stringResultsCache(theManager),
    m_keys(theManager),
    m_scratchVector(theManager)
{
}



NodeSorter::~NodeSorter()
{
}



void
NodeSorter::sort(StylesheetExecutionContext&    executionContext)
{
    assert(m_scratchVector.empty() == false);

    // Make sure the caches are cleared when we're done...
    const CollectionClearGuard<NumberResultsCacheType>  guard1(m_numberResultsCache);
    const CollectionClearGuard<StringResultsCacheType>  guard2(m_stringResultsCache);

    NodeSortKeyCompare  theComparer(
                    executionContext,
                    *this,
                    m_scratchVector,
                    m_keys);

    XALAN_USING_STD(stable_sort)

    // Use the stl sort algorithm, which will use our compare functor,
    // which returns true if first less than second
    stable_sort(
            m_scratchVector.begin(),
            m_scratchVector.end(),
            theComparer);
}



void
NodeSorter::sort(
            StylesheetExecutionContext&     executionContext,
            MutableNodeRefList&             theList)
{
    if (m_keys.empty() == false)
    {
        const NodeRefListBase::size_type    theLength = theList.getLength();

        // Copy the nodes to a vector...
        assert(m_scratchVector.empty() == true);

        // Make sure the scratch vector is cleared when we're done...
        CollectionClearGuard<NodeVectorType>    guard(m_scratchVector);

        m_scratchVector.reserve(theLength);

        NodeRefListBase::size_type  i = 0;

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



static inline int
doCollationCompare(
            StylesheetExecutionContext&         executionContext,
            const XalanDOMString&               theLHS,
            const XalanDOMString&               theRHS,
            const XalanDOMString&               theLanguage,
            XalanCollationServices::eCaseOrder  theCaseOrder)
{
    if (theLanguage.empty() == true)
    {
        return executionContext.collationCompare(
                theLHS,
                theRHS,
                theCaseOrder);
    }
    else
    {
        return executionContext.collationCompare(
                theLHS,
                theRHS,
                theLanguage,
                theCaseOrder);
    }
}



int
NodeSorter::NodeSortKeyCompare::compare(
                first_argument_type     theLHS,
                second_argument_type    theRHS,
                XalanSize_t             theKeyIndex) const
{
    assert(theLHS.m_node != 0 && theRHS.m_node != 0);
    assert(theKeyIndex < m_nodeSortKeys.size());

    int                 theResult = 0;

    const NodeSortKey&  theKey = m_nodeSortKeys[theKeyIndex];

    // Compare as numbers
    if(theKey.getTreatAsNumbers() == false)
    {
        // Compare as strings...
        const XalanDOMString&   theLHSString =
            getStringResult(theKey, theKeyIndex, theLHS);

        const XalanDOMString&   theRHSString =
            getStringResult(theKey, theKeyIndex, theRHS);

        theResult = doCollationCompare(
                m_executionContext,
                theLHSString,
                theRHSString,
                theKey.getLanguageString(),
                theKey.getCaseOrder());
    }
    else
    {
        const double    n1Num = getNumberResult(theKey, theKeyIndex, theLHS);
        const double    n2Num = getNumberResult(theKey, theKeyIndex, theRHS);

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



inline double
getResult(
            const XPath*            theXPath,
            XalanNode*              theNode,
            const PrefixResolver&   thePrefixResolver,
            XPathExecutionContext&  theExecutionContext)
{
    typedef XPathExecutionContext::GetCachedString  GetCachedString;

    if (theXPath == 0)
    {
        assert(theNode != 0);

        const GetCachedString   temp(theExecutionContext);

        DOMServices::getNodeData(*theNode, theExecutionContext, temp.get());

        return DoubleSupport::toDouble(temp.get(), theExecutionContext.getMemoryManager());
    }
    else
    {
        double  theResult;

        theXPath->execute(
                theNode,
                thePrefixResolver,
                theExecutionContext,
                theResult);

        return theResult;
    }
}



double
NodeSorter::NodeSortKeyCompare::getNumberResult(
                const NodeSortKey&      theKey,
                XalanSize_t             theKeyIndex,
                first_argument_type     theEntry) const
{
    assert(theKey.getPrefixResolver() != 0);

    const XPath* const  xpath = theKey.getSelectPattern();

    typedef NodeSorter::NumberResultsCacheType  NumberResultsCacheType;

    NumberResultsCacheType&     theCache =
            m_sorter.m_numberResultsCache;

    if (theCache.empty() == true)
    {
        theCache.resize(m_nodeSortKeys.size());
    }

    // We need a dummy value to indicate that a slot has
    // never been evaluated.  0 is probably a bad idea,
    // as is NaN, since that would be fairly common with
    // values that are not convertible to a number.  This
    // is just a not-so-random number...
    const double    theDummyValue = 135792468.0L;

    if (theCache[theKeyIndex].empty() == false)
    {
        if (DoubleSupport::equal(theCache[theKeyIndex][theEntry.m_position], theDummyValue) == true)
        {
            theCache[theKeyIndex][theEntry.m_position] =
                getResult(
                    xpath,
                    theEntry.m_node,
                    *theKey.getPrefixResolver(),
                    m_executionContext);
        }
    }
    else
    {
        theCache[theKeyIndex].resize(m_nodes.size(), 0);

        XALAN_USING_STD(fill)

        // Fill with the dummy value...
        fill(
            theCache[theKeyIndex].begin(),
            theCache[theKeyIndex].end(),
            theDummyValue);

        theCache[theKeyIndex][theEntry.m_position] =
            getResult(
                xpath,
                theEntry.m_node,
                *theKey.getPrefixResolver(),
                m_executionContext);
    }

    return theCache[theKeyIndex][theEntry.m_position];
}



#if defined(XALAN_NODESORTER_CACHE_XOBJECTS)

inline void
getResult(
            const XPath*            theXPath,
            XalanNode*              theNode,
            const PrefixResolver&   thePrefixResolver,
            XPathExecutionContext&  theExecutionContext,
            XObjectPtr&             theResult)
{
    if (theXPath == 0)
    {
        assert(theNode != 0);

        theResult = theExecutionContext.getXObjectFactory().createNodeSet(theNode);     
    }
    else
    {
        theResult = theXPath->execute(
                theNode,
                thePrefixResolver,
                theExecutionContext);
    }
}

inline bool
notCached(const XObjectPtr&     theEntry)
{
    return theEntry.null();
}

inline bool
isCached(const XObjectPtr&  theEntry)
{
    return !theEntry.null();
}

inline const XalanDOMString&
cacheValue(const XObjectPtr&    theEntry)
{
    return theEntry->str();
}

#else

inline void
getResult(
            const XPath*            theXPath,
            XalanNode*              theNode,
            const PrefixResolver&   thePrefixResolver,
            XPathExecutionContext&  theExecutionContext,
            XalanDOMString&         theResult)
{
    if (theXPath == 0)
    {
        assert(theNode != 0);

        DOMServices::getNodeData(
                *theNode,
                theExecutionContext,
                theResult);
    }
    else
    {
        theXPath->execute(
                theNode,
                thePrefixResolver,
                theExecutionContext,
                theResult);
    }
}

inline bool
notCached(const XalanDOMString&     theEntry)
{
    return theEntry.empty();
}

inline bool
isCached(const XalanDOMString&  /* theEntry */)
{
    return true;
}

inline const XalanDOMString&
cacheValue(const XalanDOMString&    theEntry)
{
    return theEntry;
}

#endif



const XalanDOMString&
NodeSorter::NodeSortKeyCompare::getStringResult(
                const NodeSortKey&      theKey,
                XalanSize_t             theKeyIndex,
                first_argument_type     theEntry) const
{
    assert(theKey.getPrefixResolver() != 0);

    const XPath* const  xpath = theKey.getSelectPattern();

    typedef NodeSorter::StringResultsCacheType  StringResultsCacheType;

    StringResultsCacheType&     theCache =
            m_sorter.m_stringResultsCache;

    if (theCache.empty() == true)
    {
        theCache.resize(m_nodeSortKeys.size());
    }

    if (theCache[theKeyIndex].empty() == false)
    {
        if (notCached(theCache[theKeyIndex][theEntry.m_position]) == true)
        {
            getResult(
                xpath,
                theEntry.m_node,
                *theKey.getPrefixResolver(),
                m_executionContext,
                theCache[theKeyIndex][theEntry.m_position]);
        }
    }
    else
    {

        theCache[theKeyIndex].resize(m_nodes.size());

        getResult(
            xpath,
            theEntry.m_node,
            *theKey.getPrefixResolver(),
            m_executionContext,
            theCache[theKeyIndex][theEntry.m_position]);
    }

    assert(isCached(theCache[theKeyIndex][theEntry.m_position]) == true);

    return cacheValue(theCache[theKeyIndex][theEntry.m_position]);
}



XALAN_CPP_NAMESPACE_END
