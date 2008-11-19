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
#include "ElemNumber.hpp"



#include <cstring>



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanNumberFormat.hpp>
#include <xalanc/PlatformSupport/XalanSimplePrefixResolver.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>

#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>
#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/ElementPrefixResolverProxy.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemNumber::ElemNumber(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber, 
            XalanFileLoc                    columnNumber,
            unsigned long                   id) :
    ElemTemplateElement(constructionContext,
                        stylesheetTree,
                        lineNumber,
                        columnNumber,
                        StylesheetConstructionContext::ELEMNAME_NUMBER),    
    m_countMatchPattern(0),
    m_fromMatchPattern(0),
    m_valueExpr(0),
    m_level(eSingle),
    m_format_avt(0),
    m_lang_avt(0),
    m_lettervalue_avt(0),
    m_groupingSeparator_avt(0),
    m_groupingSize_avt(0),
    m_id(id)
    
{
    const XalanSize_t  nAttrs = atts.getLength();

    for (XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (equals(aname, s_levelString))
        {
            const XalanDOMChar* const   levelValue = atts.getValue(i);

            if (equals(s_multipleString, levelValue))
            {
                m_level = eMultiple;
            }
            else if (equals(s_anyString, levelValue))
            {
                m_level = eAny;
            }
            else if (equals(s_singleString, levelValue))
            {
                m_level = eSingle;
            }
            else
            {
                error(
                    constructionContext,
                    XalanMessages::ElementHasIllegalAttributeValue_3Param,
                    Constants::ELEMNAME_NUMBER_WITH_PREFIX_STRING.c_str(),
                    aname,
                    levelValue);
            }
        }
        else if (equals(aname, Constants::ATTRNAME_COUNT))
        {
            m_countMatchPattern = constructionContext.createMatchPattern(getLocator(), atts.getValue(i), *this);
        }
        else if (equals(aname, Constants::ATTRNAME_FROM))
        {
            m_fromMatchPattern = constructionContext.createMatchPattern(getLocator(), atts.getValue(i), *this);
        }
        else if (equals(aname, Constants::ATTRNAME_VALUE))
        {
            m_valueExpr = constructionContext.createXPath(getLocator(), atts.getValue(i), *this);
        }
        else if (equals(aname, Constants::ATTRNAME_FORMAT))
        {
            m_format_avt =
                constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
        }
        else if (equals(aname, Constants::ATTRNAME_LANG))
        {
            m_lang_avt =
                constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
        }
        else if (equals(aname, Constants::ATTRNAME_LETTERVALUE))
        {
            m_lettervalue_avt =
                constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
        }
        else if (equals(aname,Constants::ATTRNAME_GROUPINGSEPARATOR))
        {
            m_groupingSeparator_avt =
                constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
        }
        else if (equals(aname,Constants::ATTRNAME_GROUPINGSIZE))
        {
            m_groupingSize_avt =
                constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
        }
        else if (isAttrOK(
                    aname,
                    atts,
                    i,
                    constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                Constants::ELEMNAME_NUMBER_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }
}



ElemNumber*
ElemNumber::create(
            MemoryManager&                  theManager,
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber, 
            XalanFileLoc                    columnNumber,
            unsigned long                   id)
{
    typedef ElemNumber  ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                constructionContext,
                                stylesheetTree,
                                atts,
                                lineNumber,
                                columnNumber,
                                id);

    theGuard.release();

    return theResult;
}



ElemNumber::~ElemNumber()
{
}



const XalanDOMString&
ElemNumber::getElementName() const
{
    return Constants::ELEMNAME_NUMBER_WITH_PREFIX_STRING;
}



typedef XPathExecutionContext::GetCachedString  GetCachedString;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemNumber::startElement(StylesheetExecutionContext&        executionContext) const
{
    ElemTemplateElement::startElement(executionContext);

    const GetCachedString   theGuard(executionContext);

    XalanDOMString&         countString = theGuard.get();

    getCountString(executionContext, countString);

    if (!countString.empty())
    {
        executionContext.characters(
            countString.c_str(), 0, countString.length());
    }
    
    return 0;
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemNumber::execute(StylesheetExecutionContext&     executionContext) const
{
    ElemTemplateElement::execute(executionContext);

    const GetCachedString   theGuard(executionContext);

    XalanDOMString&         countString = theGuard.get();

    getCountString(executionContext, countString);

    if (!countString.empty())
    {
        executionContext.characters(countString.c_str(), 0, countString.length());
    }
}
#endif



XalanNode*
ElemNumber::findAncestor(
            StylesheetExecutionContext&     executionContext,
            const XPath*                    fromMatchPattern,
            const XPath*                    countMatchPattern,
            XalanNode*                      context) const
{
    XalanNode*  contextCopy = context;

    while (contextCopy != 0)
    {
        if (0 != fromMatchPattern)
        {
            if (fromMatchPattern->getMatchScore(
                    contextCopy,
                    *this,
                    executionContext) != XPath::eMatchScoreNone)
            {
                break;
            }
        }
        
        if (0 != countMatchPattern)
        {
            if(countMatchPattern->getMatchScore(
                    contextCopy,
                    *this,
                    executionContext) != XPath::eMatchScoreNone)
            {
                break;
            }
        }
        
        contextCopy = DOMServices::getParentOfNode(*contextCopy);
    }

    return contextCopy;
}                   



XalanNode*
ElemNumber::findPrecedingOrAncestorOrSelf(
            StylesheetExecutionContext&     executionContext,
            const XPath*                    fromMatchPattern,
            const XPath*                    countMatchPattern,
            XalanNode*                      context) const
{  
    XalanNode*  thePos = context;

    while (thePos != 0)
    {
        if (0 != fromMatchPattern)
        {
            if (fromMatchPattern->getMatchScore(
                    thePos,
                    *this,
                    executionContext) != XPath::eMatchScoreNone)
            {
                thePos = 0;
                break;
            }
        }

        if (0 != countMatchPattern)
        {
            if (countMatchPattern->getMatchScore(
                    thePos,
                    *this,
                    executionContext) != XPath::eMatchScoreNone)
            {
                break;
            }
        }

        XalanNode* const    previousSibling = thePos->getPreviousSibling();

        if (previousSibling == 0)
        {
            // Move up to the parent if thePos is the first child
            // of its parent.  Note that we don't descend into the
            // parent, since that's where we came from.
            thePos = DOMServices::getParentOfNode(*thePos);
        }
        else
        {
            thePos = previousSibling;

            // Descend into the subtree all the way to the
            // last node, which we reach by looking down
            // the last child chain.
            XalanNode* lastChild = thePos->getLastChild();

            while (lastChild != 0)
            {
                thePos = lastChild;
                lastChild = thePos->getLastChild();
            }
        }
    }

    return thePos;
}



const XPath*
ElemNumber::getCountMatchPattern(
            StylesheetExecutionContext&     executionContext,
            XalanNode*                      contextNode) const
{
    const XPath*    countMatchPattern = 0;

    switch (contextNode->getNodeType())
    {
    case XalanNode::ELEMENT_NODE:
        {
            // Check to see if we have any fiddling to do with the match pattern
            // we create...
            const XalanDOMString&   theNamespaceURI = contextNode->getNamespaceURI();
            const XalanDOMString&   theNodeName = contextNode->getNodeName();

            if (theNamespaceURI.empty() == true)
            {
                // We can pass any PrefixResolver instance, so just
                // pass ourself...
                countMatchPattern =
                        executionContext.createMatchPattern(theNodeName, *this);
            }
            else if (theNodeName.length() != contextNode->getLocalName().length())
            {
                // OK, there's a prefix, so create a prefix resolver so the
                // prefix can be properly resolved...
                const XalanElement* const   theElement =
                    static_cast<const XalanElement*>(contextNode);

                    const ElementPrefixResolverProxy    theProxy( theElement, executionContext.getMemoryManager());

                countMatchPattern =
                        executionContext.createMatchPattern(theNodeName, theProxy);
            }
            else
            {
                // OK, this is ugly.  We have to get a unique prefix and
                // construct a match pattern with that prefix...
                const GetCachedString   thePrefix(executionContext);

                executionContext.getUniqueNamespaceValue(thePrefix.get());

                const GetCachedString   theMatchPatternString(executionContext);

                theMatchPatternString.get().assign(thePrefix.get());
                theMatchPatternString.get().append(1, XalanUnicode::charColon);
                theMatchPatternString.get().append(theNodeName);

                // Use this class to resolve the synthesized prefix to the
                // appropriate namespace URI.  We could see if a prefix is
                // already in scope for the namespace URI, but it would take
                // more effort to find that out than it would to just
                // create this simple resolver.
                const XalanSimplePrefixResolver     theResolver(
                                                        thePrefix.get(),
                                                        theNamespaceURI,
                                                        getURI());

                countMatchPattern =
                        executionContext.createMatchPattern(
                            theMatchPatternString.get(),
                            theResolver);
            }
        }
        break;

    case XalanNode::ATTRIBUTE_NODE:
        {
            const XalanAttr* const  theAttribute =
                static_cast<const XalanAttr*>(contextNode);

            assert(theAttribute->getOwnerElement() != 0);

            const XalanDOMString&   theNodeName = theAttribute->getNodeName();

            const ElementPrefixResolverProxy    theProxy(theAttribute->getOwnerElement(), executionContext.getMemoryManager());

            const GetCachedString   theMatchPatternString(executionContext);

            theMatchPatternString.get().assign(s_atString);
            theMatchPatternString.get().append(theNodeName);

            countMatchPattern = executionContext.createMatchPattern(
                            theMatchPatternString.get(),
                            theProxy);
        }
        break;

    case XalanNode::CDATA_SECTION_NODE:
    case XalanNode::TEXT_NODE:
        countMatchPattern = executionContext.createMatchPattern(
                    s_textString, *this);
        break;

    case XalanNode::COMMENT_NODE:
        countMatchPattern = executionContext.createMatchPattern(
                    s_commentString, *this);
        break;

    case XalanNode::DOCUMENT_NODE:
        countMatchPattern = executionContext.createMatchPattern(
                    s_slashString, *this);
        break;

    case XalanNode::PROCESSING_INSTRUCTION_NODE:
        {
            const GetCachedString   theMatchPatternString(executionContext);

            theMatchPatternString.get() = s_piString;
            theMatchPatternString.get().append(contextNode->getNodeName());
            theMatchPatternString.get().append(1, XalanUnicode::charRightParenthesis);

            countMatchPattern = executionContext.createMatchPattern(
                    theMatchPatternString.get(),
                    *this);
        }
        break;

    default:
        break;
    }

    return countMatchPattern;
}



inline void
ElemNumber::getCountString(
            StylesheetExecutionContext&     executionContext,
            const MutableNodeRefList&       ancestors,
            CountersTable&                  ctable,
            CountType                       numberList[],
            NodeRefListBase::size_type      numberListLength,
            XalanDOMString&                 theResult) const
{
    for (NodeRefListBase::size_type i = 0; i < numberListLength; i++)
    {
        XalanNode* const target = ancestors.item(numberListLength - i - 1);

        numberList[i] = ctable.countNode(
                            executionContext,
                            *this,
                            target);
    }

    formatNumberList(
            executionContext,
            numberList,
            numberListLength,
            theResult);
}



void
ElemNumber::getCountString(
            StylesheetExecutionContext&     executionContext,
            XalanDOMString&                 theResult) const
{
    XalanNode* sourceNode = executionContext.getCurrentNode();

    assert(sourceNode != 0);

    if (0 != m_valueExpr)
    {
        double  theValue;

        m_valueExpr->execute(*this, executionContext, theValue);

        // Note these checks are in a specific order...
        if (DoubleSupport::isNaN(theValue) == true ||
            DoubleSupport::isPositiveInfinity(theValue) == true ||
            DoubleSupport::isNegativeInfinity(theValue) == true ||
            DoubleSupport::lessThan(theValue, 0.5) == true)
        {
            NumberToDOMString(theValue, theResult);
        }
        else
        {
            const CountType   theNumber =
                CountType(DoubleSupport::round(theValue));

            formatNumberList(
                    executionContext,
                    &theNumber,
                    1,
                    theResult);
        }
    }
    else
    {
        CountersTable&  ctable = executionContext.getCountersTable();

        if (eAny == m_level)
        {
            const CountType     theNumber =
                ctable.countNode(executionContext, *this, sourceNode);

            if (theNumber != 0)
            {
                formatNumberList(
                    executionContext,
                    &theNumber,
                    1,
                    theResult);
            }
        }
        else
        {
            typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

            BorrowReturnMutableNodeRefList  ancestors(executionContext);

            getMatchingAncestors(
                executionContext,
                sourceNode,
                eSingle == m_level,
                *ancestors.get());

            const NodeRefListBase::size_type    lastIndex = ancestors->getLength();

            if (lastIndex > 0)
            {
                const NodeRefListBase::size_type    theStackArrayThreshold = 100;

                if (lastIndex < theStackArrayThreshold)
                {
                    CountType   numberList[theStackArrayThreshold];

                    getCountString(
                        executionContext,
                        *ancestors.get(),
                        ctable,
                        numberList,
                        lastIndex,
                        theResult);
                }
                else
                {
                    CountTypeArrayType  numberList(executionContext.getMemoryManager());


                    numberList.resize(lastIndex, 0);

                    getCountString(
                        executionContext,
                        *ancestors.get(),
                        ctable,
                        &*numberList.begin(),
                        lastIndex,
                        theResult);
                }
            }
        }
    }
}



XalanNode*
ElemNumber::getPreviousNode(
            StylesheetExecutionContext&     executionContext,
            XalanNode*                      pos) const
{
    // Create an XPathGuard, since we may need to
    // create a new XPath...
    StylesheetExecutionContext::XPathGuard  xpathGuard(
            executionContext);

    const XPath*    countMatchPattern = m_countMatchPattern;

    if (countMatchPattern == 0)
    {
        // Get the XPath...
        xpathGuard.reset(getCountMatchPattern(executionContext, pos));

        countMatchPattern = xpathGuard.get();
    }

    if (eAny == m_level)
    {
        const XPath* const  fromMatchPattern = m_fromMatchPattern;

        // Do a backwards document-order walk 'till a node is found that matches 
        // the 'from' pattern, or a node is found that matches the 'count' pattern, 
        // or the top of the tree is found.
        while(0 != pos)
        {
            // Get the previous sibling, if there is no previous sibling, 
            // then count the parent, but if there is a previous sibling, 
            // dive down to the lowest right-hand (last) child of that sibling.
            XalanNode* next = pos->getPreviousSibling();

            if(0 == next)
            {
                next = pos->getParentNode();

                if(0 != next &&
                   next->getNodeType() == XalanNode::DOCUMENT_NODE ||
                   (0 != fromMatchPattern &&
                        fromMatchPattern->getMatchScore(
                            next,
                            *this,
                            executionContext) != XPath::eMatchScoreNone))
                {
                    pos = 0; // return 0 from function.

                    break; // from while loop
                }
            }
            else
            {
                // dive down to the lowest right child.
                XalanNode* child = next;

                while(0 != child)
                {
                    child = next->getLastChild();

                    if(0 != child)
                        next = child;
                }
            }

            pos = next;

            if(0 != pos &&
               (0 == countMatchPattern ||
                countMatchPattern->getMatchScore(
                        pos,
                        *this,
                        executionContext) != XPath::eMatchScoreNone))
            {
                break;
            }
        }
    }
    else // NUMBERLEVEL_MULTI or NUMBERLEVEL_SINGLE
    {
        while (0 != pos)
        {            
            pos = pos->getPreviousSibling();

            if (0 != pos &&
                (0 == countMatchPattern ||
                 countMatchPattern->getMatchScore(
                        pos,
                        *this,
                        executionContext) != XPath::eMatchScoreNone))
            {
                break;
            }
        }
    }

    return pos;
}



XalanNode*
ElemNumber::getTargetNode(
        StylesheetExecutionContext&     executionContext,
        XalanNode*                      sourceNode) const
{
    XalanNode* target = 0;

    // Create an XPathGuard, since we may need to
    // create a new XPath...
    StylesheetExecutionContext::XPathGuard  xpathGuard(
            executionContext);

    const XPath*    countMatchPattern = m_countMatchPattern;

    if (countMatchPattern == 0)
    {
        // Get the XPath...
        xpathGuard.reset(getCountMatchPattern(executionContext, sourceNode));

        countMatchPattern = xpathGuard.get();
    }

    if (eAny == m_level)
    {
        target = findPrecedingOrAncestorOrSelf(
                executionContext,
                m_fromMatchPattern,
                countMatchPattern,
                sourceNode);
    }
    else
    {
        target = findAncestor(
                executionContext,
                m_fromMatchPattern,
                countMatchPattern,
                sourceNode);
    }

    return target;
}



void
ElemNumber::getMatchingAncestors(
            StylesheetExecutionContext&     executionContext,
            XalanNode*                      node, 
            bool                            stopAtFirstFound,
            MutableNodeRefList&             ancestors) const
{
    // Create an XPathGuard, since we may need to
    // create a new XPath...
    StylesheetExecutionContext::XPathGuard  xpathGuard(executionContext);

    const XPath*    countMatchPattern = m_countMatchPattern;

    if (countMatchPattern == 0)
    {
        // Get the XPath...
        xpathGuard.reset(getCountMatchPattern(executionContext, node));

        countMatchPattern = xpathGuard.get();
    }

    while (0 != node)
    {
        if (0 != m_fromMatchPattern &&
            m_fromMatchPattern->getMatchScore(
                node,
                *this,
                executionContext) != XPath::eMatchScoreNone)
        {
            // The following if statement gives level="single" different 
            // behavior from level="multiple", which seems incorrect according 
            // to the XSLT spec.  For now we are leaving this in to replicate 
            // the same behavior in XT, but, for all intents and purposes we 
            // think this is a bug, or there is something about level="single" 
            // that we still don't understand.
            if(!stopAtFirstFound)
            {
                break;
            }
        }

        assert(0 != countMatchPattern);

        if(countMatchPattern->getMatchScore(node, *this, executionContext) !=
                XPath::eMatchScoreNone)
        {
            ancestors.addNode(node);

            if (stopAtFirstFound)
            {
                break;
            }
        }

        node = DOMServices::getParentOfNode(*node);
    }
}



XalanNumberFormat*
ElemNumber::getNumberFormatter(StylesheetExecutionContext&  executionContext) const
{
    // Helper to format local specific numbers to strings.

    XalanMemMgrAutoPtr<XalanNumberFormat>   formatter(executionContext.createXalanNumberFormat());

    typedef XPathExecutionContext::GetCachedString  GetCachedString;

    const GetCachedString   theGuard1(executionContext);

    XalanDOMString&         digitGroupSepValue = theGuard1.get();

    if (0 != m_groupingSeparator_avt)
    {
        m_groupingSeparator_avt->evaluate(digitGroupSepValue, *this, executionContext);
    }
                                     
    if (digitGroupSepValue.length() > 1)
    {
        error(
            executionContext,
            XalanMessages::GroupingSeparatorValueMustBeOneCharacterLength);
    }

    GetCachedString     theGuard2(executionContext);

    XalanDOMString&     nDigitsPerGroupValue = theGuard2.get();

    if (0 != m_groupingSize_avt)
    {
        m_groupingSize_avt->evaluate(nDigitsPerGroupValue, *this, executionContext);
    }

    // 7.7.1 If one is empty, it is ignored (numb81 conf test)
    if (!digitGroupSepValue.empty() && !nDigitsPerGroupValue.empty())
    {
        formatter->setGroupingUsed(true);
        formatter->setGroupingSeparator(digitGroupSepValue);
        formatter->setGroupingSize(DOMStringToUnsignedLong(nDigitsPerGroupValue));
    }

    return formatter.releasePtr();
}



void
ElemNumber::formatNumberList(
            StylesheetExecutionContext&     executionContext,
            const CountType                 theList[],
            NodeRefListBase::size_type      theListLength,
            XalanDOMString&                 theResult) const
{
    XalanDOMChar    numberType = XalanUnicode::charDigit_1;

    XalanDOMString::size_type   numberWidth = 1;

    typedef XalanVector<XalanDOMString> StringVectorType;
    typedef StringVectorType::iterator  StringVectorTypeIterator;

    // Construct an array of tokens.  We need to be able to check if the last
    // token in non-alphabetic, in which case the penultimate non-alphabetic is
    // the repeating separator.
    //
    // We should be able to replace this with a vector of the indexes in
    // the evaluated string where the tokens start.  But for now, this will
    // have to do...
    StringVectorType    tokenVector(executionContext. getMemoryManager()) ;

    {
        const GetCachedString   theGuard1(executionContext);

        XalanDOMString&         formatValue = theGuard1.get();

        if (m_format_avt != 0)
        {
             m_format_avt->evaluate(formatValue, *this, executionContext);
        }

        if (formatValue.empty() == true)
        {
            formatValue = XalanUnicode::charDigit_1;
        }

        NumberFormatStringTokenizer                     formatTokenizer(formatValue);

        const NumberFormatStringTokenizer::size_type    theTokenCount = formatTokenizer.countTokens();

        tokenVector.resize(theTokenCount);

        // Tokenize directly into the vector...
        for (NumberFormatStringTokenizer::size_type i = 0; i < theTokenCount; ++i)
        {
            formatTokenizer.nextToken(tokenVector[i]);
        }

        assert(theTokenCount == tokenVector.size());
    }

    // These are iterators which will either point to tokenVector.end(),
    // or the appropriate string in the vector...
    StringVectorTypeIterator        leaderStrIt = tokenVector.end();
    StringVectorTypeIterator        trailerStrIt = leaderStrIt;
    StringVectorTypeIterator        sepStringIt = leaderStrIt;
    const StringVectorTypeIterator  endIt = leaderStrIt;

    StringVectorTypeIterator    it = tokenVector.begin();

    const StringVectorType::size_type   theVectorSize =
        tokenVector.size();

    if (theVectorSize > 0)
    {
        if (!isXMLLetterOrDigit((*it)[0]))
        {
            leaderStrIt = it;

            // Move the iterator up one, so it
            // points at the first numbering token...
            ++it;
        }

        if (theVectorSize > 1)
        {
            if (!isXMLLetterOrDigit(tokenVector.back()[0]))
            {
                // Move the iterator back one, so it's pointing
                // at the trailing string...
                --trailerStrIt;
            }
        }
    }

    // Now we're left with a sequence of alpha,non-alpha tokens, format them
    // with the corresponding entry in the format string, or the last one if no
    // more matching ones

    if (leaderStrIt != endIt)
    {
        theResult += *leaderStrIt;
    }

    const GetCachedString   theGuard2(executionContext);

    XalanDOMString&         theIntermediateResult = theGuard2.get();

    for (NodeRefListBase::size_type i = 0; i < theListLength; i++)
    {
        if (it != trailerStrIt)
        {
            assert(isXMLLetterOrDigit((*it)[0]));

            numberWidth = it->length();

            numberType = (*it)[numberWidth - 1];

            ++it;
        }

        if (it != trailerStrIt)
        {
            assert(!isXMLLetterOrDigit((*it)[0]));

            sepStringIt = it;

            ++it;
        }

        getFormattedNumber(
            executionContext,
            numberType,
            numberWidth,
            theList[i],
            theIntermediateResult);

        theResult += theIntermediateResult;

        // All but the last one
        if (i < theListLength - 1)
        {
            if (sepStringIt != endIt)
            {
                theResult += *sepStringIt;
            }
            else
            {
                theResult += XalanUnicode::charFullStop;
            }

            theIntermediateResult.clear();
        }
    }

    if (trailerStrIt != endIt)
    {
        theResult += *trailerStrIt;
    }
}



void
ElemNumber::evaluateLetterValueAVT(
            StylesheetExecutionContext&     executionContext,
            XalanDOMString&                 value) const
{
    if (m_lettervalue_avt == 0)
    {
        value.clear();
    }
    else
    {
        m_lettervalue_avt->evaluate(
                value,
                *this,
                executionContext);
    }
}



void
ElemNumber::traditionalAlphaCount(
            CountType                               theValue,
            const XalanNumberingResourceBundle&     theResourceBundle,
            XalanDOMString&                         theResult) const
{
    typedef XalanNumberingResourceBundle::NumberTypeVectorType      NumberTypeVectorType;
    typedef XalanNumberingResourceBundle::DigitsTableVectorType DigitsTableVectorType;
    typedef XalanNumberingResourceBundle::eNumberingMethod      eNumberingMethod;
    typedef XalanNumberingResourceBundle::eMultiplierOrder      eMultiplierOrder;

    bool    fError = false;

    // if this number is larger than the largest number we can represent, error!
    //if (val > theResourceBundle.getMaxNumericalValue())
    //return XSLTErrorResources.ERROR_STRING;
    XalanDOMCharVectorType  table(theResult.getMemoryManager());

    // index in table of the last character that we stored
    NumberTypeVectorType::size_type lookupIndex = 1;  // start off with anything other than zero to make correction work

    // Create a buffer to hold the result
    // TODO:  size of the table can be determined by computing
    // logs of the radix.  For now, we fake it.
    XalanDOMChar    buf[100];

    //some languages go left to right(ie. english), right to left (ie. Hebrew),
    //top to bottom (ie.Japanese), etc... Handle them differently
    //String orientation = thisBundle.getString(Constants.LANG_ORIENTATION);

    // next character to set in the buffer
    XalanDOMString::size_type   charPos = 0;

    // array of number groups: ie.1000, 100, 10, 1
    const NumberTypeVectorType& groups = theResourceBundle.getNumberGroups();

    const NumberTypeVectorType::size_type   groupsSize = groups.size();

    // array of tables of hundreds, tens, digits. Indexes into the vectors
    // in the digits table.
    const NumberTypeVectorType& tables = theResourceBundle.getDigitsTableTable();

    const DigitsTableVectorType&    digitsTable = theResourceBundle.getDigitsTable();

    assert(tables.size() == digitsTable.size());

    // some languages have additive alphabetical notation,
    // some multiplicative-additive, etc... Handle them differently.
    const eNumberingMethod  numbering = theResourceBundle.getNumberingMethod();

    // do multiplicative part first
    if (numbering == XalanNumberingResourceBundle::eMultiplicativeAdditive)
    {
        const eMultiplierOrder  mult_order = theResourceBundle.getMultiplierOrder();

        const NumberTypeVectorType& multiplier = theResourceBundle.getMultipliers();

        const NumberTypeVectorType::size_type   multiplierSize = multiplier.size();

        const XalanDOMCharVectorType&   zeroChar = theResourceBundle.getZeroChar();

        const XalanDOMCharVectorType::size_type     zeroCharSize = zeroChar.size();

        const XalanDOMCharVectorType&   multiplierChars = theResourceBundle.getMultiplierChars();

        CountTypeArrayType::size_type   i = 0;

        // skip to correct multiplier
        while (i < multiplierSize && theValue < multiplier[i])
        {
            ++i;
        }

        do
        {
            if (i >= multiplierSize)
            {
                break;            //number is smaller than multipliers
            }

            // some languages (ie chinese) put a zero character (and only one) when
            // the multiplier is multiplied by zero. (ie, 1001 is 1X1000 + 0X100 + 0X10 + 1)
            // 0X100 is replaced by the zero character, we don't need one for 0X10
            if (theValue < multiplier[i])
            {
                if (zeroCharSize == 0)
                {
                    ++i;
                } 
                else
                {
                    if (charPos == 0 ||
                        buf[charPos - 1] != zeroChar[0])
                    {
                        buf[charPos++] = zeroChar[0];
                    }

                    ++i;
                }
            }
            else if (theValue >= multiplier[i])
            {

                const CountType     mult = theValue / multiplier[i];

                theValue = theValue % multiplier[i];         // save this.

                CountTypeArrayType::size_type   k = 0;

                while (k < groupsSize)
                {
                    lookupIndex = 1;                 // initialize for each table
            
                    if (mult / groups[k] <= 0)       // look for right table
                    {
                        k++;
                    }
                    else
                    {
                        assert(digitsTable.size() > DigitsTableVectorType::size_type(tables[k]));

                        // get the table
                        const XalanDOMCharVectorType&   THEletters =
                                digitsTable[tables[k]];

                        const XalanDOMCharVectorType::size_type     THElettersSize =
                            THEletters.size();

                        table.resize(THElettersSize + 1);                   

                        const CountTypeArrayType::size_type tableSize = table.size();

                        XalanDOMCharVectorType::size_type   j = 0;

                        for (; j < THElettersSize; j++)
                        {
                            table[j + 1] = THEletters[j];
                        }

                        table[0] = THEletters[j - 1];    // don't need this

                        // index in "table" of the next char to emit
                        lookupIndex  = mult / groups[k];

                        //this should not happen
                        if (lookupIndex == 0 && mult == 0)
                        {
                            break;
                        }

                        assert(i < multiplierChars.size());

                        const XalanDOMChar  multiplierChar = multiplierChars[i];

                        // put out the next character of output 
                        if (lookupIndex < tableSize)
                        {
                            if(mult_order == XalanNumberingResourceBundle::ePrecedes)
                            {
                                buf[charPos++] = multiplierChar;
                                buf[charPos++] = table[lookupIndex];
                            }
                            else
                            {
                                // don't put out 1 (ie 1X10 is just 10)
                                if (lookupIndex == 1 && i == multiplierSize - 1)
                                {
                                }
                                else
                                {
                                    buf[charPos++] = table[lookupIndex];
                                }

                                buf[charPos++] = multiplierChar;
                            }

                            break;       // all done!
                        }
                        else
                        {
                            fError = true;

                            break;
                        }
                    } //end else
                } // end while  

                i++;

            } // end else if
        } // end do while
        while (i < multiplierSize && fError == false);      
    }

    // Now do additive part...
    CountTypeArrayType::size_type   count = 0;

    // do this for each table of hundreds, tens, digits...
    while (count < groupsSize)
    {
        if (theValue / groups[count] <= 0)
        {
            // look for correct table
            count++;
        }
        else
        {
            const XalanDOMCharVectorType&   theletters =
                                digitsTable[tables[count]];

            const XalanDOMCharVectorType::size_type     thelettersSize =
                            theletters.size();

            table.resize(thelettersSize + 1);
        
            const CountTypeArrayType::size_type tableSize = table.size();

            XalanDOMCharVectorType::size_type   j = 0;

            // need to start filling the table up at index 1
            for (; j < thelettersSize; j++)
            {
                table[j + 1] = theletters[j];
            }

            table[0] = theletters[j - 1];  // don't need this

            // index in "table" of the next char to emit
            lookupIndex  = theValue / groups[count];

            // shift input by one "column"
            theValue = theValue % groups[count];

            // this should not happen
            if (lookupIndex == 0 && theValue == 0)
                break;                  

            if (lookupIndex < tableSize)
            {
                // put out the next character of output 
                buf[charPos++] = table[lookupIndex];    // left to right or top to bottom                   
            }
            else
            {
                fError = true;

                break;
            }

            count++;
        }
    } // end while

    if (fError == true)
    {
        theResult = s_errorString;
    }
    else
    {
        theResult.assign(buf, charPos);
    }
}



const XalanDOMChar  elalphaNumberType = 0x03B1;



void
ElemNumber::getFormattedNumber(
            StylesheetExecutionContext&     executionContext,
            XalanDOMChar                    numberType,
            XalanDOMString::size_type       numberWidth,
            CountType                       listElement,
            XalanDOMString&                 theResult) const
{
    switch(numberType)
    {
        case XalanUnicode::charLetter_A:
            int2alphaCount(listElement, s_alphaCountTable, s_alphaCountTableSize, theResult);
            break;

        case XalanUnicode::charLetter_a:
            {
                int2alphaCount(listElement, s_alphaCountTable, s_alphaCountTableSize, theResult);

                toLowerCaseASCII(theResult);
            }
            break;

        case XalanUnicode::charLetter_I:
            toRoman(listElement, true, theResult);
            break;

        case XalanUnicode::charLetter_i:
            toRoman(listElement, true, theResult);
            toLowerCaseASCII(theResult);
            break;

        case 0x3042:
        case 0x3044:
        case 0x30A2:
        case 0x30A4:
        case 0x4E00:
        case 0x58F9:
        case 0x0E51:
        case 0x05D0:
        case 0x10D0:
        case 0x0430:
            {
                StylesheetExecutionContext::GetCachedString     theGuard(executionContext);

                error(
                    executionContext,
                    XalanMessages::NumberingFormatNotSupported_1Param,
                    NumberToHexDOMString(numberType, theGuard.get()));
                break;
            }

        // Handle the special case of Greek letters for now
        case elalphaNumberType:
            {
                const GetCachedString   theGuard(executionContext);

                XalanDOMString&     letterVal = theGuard.get();

                evaluateLetterValueAVT(executionContext, letterVal);

                if (equals(letterVal, s_traditionalString) == true)
                {
                    traditionalAlphaCount(listElement, s_elalphaResourceBundle, theResult);
                }
                else if (equals(letterVal, s_alphabeticString) == true)
                {
                    int2alphaCount(listElement, s_elalphaCountTable, s_elalphaCountTableSize, theResult);
                }
                else
                {
                    error(
                        executionContext,
                        XalanMessages::LegalValuesForLetterValue);
                }
            }
            break;

        default: // "1"
            {
                StylesheetExecutionContext::XalanNumberFormatAutoPtr    formatter(
                        executionContext.getMemoryManager(),
                        getNumberFormatter(executionContext));

                formatter->format(listElement, theResult);

                const XalanDOMString::size_type     lengthNumString = theResult.length();

                if (numberWidth > lengthNumString)
                {
                    const XalanDOMString::size_type nPadding = numberWidth - lengthNumString;

                    const GetCachedString   theGuard(executionContext);

                    XalanDOMString&     padString = theGuard.get();

                    formatter->format(0, padString);

                    theResult.reserve(nPadding * padString.length() + lengthNumString + 1);

                    for (XalanDOMString::size_type i = 0; i < nPadding; i++)
                    {
                        theResult.insert(0, padString);
                    }
                }
            }
            break;
    }
}



void
ElemNumber::int2singlealphaCount(
        CountType               val, 
        const XalanDOMString&   table,
        XalanDOMString&         theResult)
{
    assert(int(table.length()) == table.length());

    const CountType     radix = table.length();

    // TODO:  throw error on out of range input
    if (val > radix)
    {
        theResult = s_errorString;
    }
    else
    {
        const XalanDOMChar  theChar = table[val - 1];

        theResult.assign(1, theChar);
    }
}



void
ElemNumber::int2alphaCount(
            CountType                   val,
            const XalanDOMChar          table[],
            XalanDOMString::size_type   length,
            XalanDOMString&             theResult)
{
    assert(length != 0);

    const CountType     radix = length;

    // Create a buffer to hold the result
    // TODO:  size of the table can be determined by computing
    // logs of the radix.  For now, we fake it.  
    const size_t    buflen = 100;

    XalanDOMChar    buf[buflen + 1];

#if defined(XALAN_STRICT_ANSI_HEADERS)
    std::memset(buf, 0, (buflen + 1) * sizeof(XalanDOMChar));
#else
    memset(buf, 0, (buflen + 1) * sizeof(XalanDOMChar));
#endif

    // next character to set in the buffer
    XalanDOMString::size_type   charPos = buflen - 1 ;    // work backward through buf[]

    // index in table of the last character that we stored
    size_t  lookupIndex = 1;  // start off with anything other than zero to make correction work
    
    //                      Correction number
    //
    //  Correction can take on exactly two values:
    //
    //      0   if the next character is to be emitted is usual
    //
    //      radix - 1 
    //          if the next char to be emitted should be one less than
    //          you would expect 
    //          
    // For example, consider radix 10, where 1="A" and 10="J"
    //
    // In this scheme, we count: A, B, C ...   H, I, J (not A0 and certainly
    // not AJ), A1
    //
    // So, how do we keep from emitting AJ for 10?  After correctly emitting the
    // J, lookupIndex is zero.  We now compute a correction number of 9 (radix-1).
    // In the following line, we'll compute (val+correction) % radix, which is,
    // (val+9)/10.  By this time, val is 1, so we compute (1+9) % 10, which 
    // is 10 % 10 or zero.  So, we'll prepare to emit "JJ", but then we'll
    // later suppress the leading J as representing zero (in the mod system, 
    // it can represent either 10 or zero).  In summary, the correction value of
    // "radix-1" acts like "-1" when run through the mod operator, but with the 
    // desireable characteristic that it never produces a negative number.

    CountType   correction = 0;

    // TODO:  throw error on out of range input

    do
    {
        // most of the correction calculation is explained above,  the reason for the
        // term after the "|| " is that it correctly propagates carries across
        // multiple columns.  
        correction = ((lookupIndex == 0) || 
            (correction != 0 && lookupIndex == radix - 1 )) ? (radix - 1) : 0;

        // index in "table" of the next char to emit
        lookupIndex  = (val + correction) % radix;  

        // shift input by one "column"
        val = (val / radix);

        // if the next value we'd put out would be a leading zero, we're done.
        if (lookupIndex == 0 && val == 0)
        {
            break;
        }

        // put out the next character of output
        buf[charPos--] = table[lookupIndex];
    }
    while (val > 0);

    theResult.assign(buf + charPos + 1, buflen - charPos - 1);
}



void
ElemNumber::toRoman(
            CountType           val,
            bool                prefixesAreOK,
            XalanDOMString&     theResult)
{
    if(val == 0)
    {
        theResult = XalanUnicode::charDigit_0;
    }
    else if (val > 3999)
    {
        theResult = s_errorString;
    }
    else
    {
        theResult.clear();

        size_t  place = 0;

        DecimalToRoman::ValueType   localValue = val;

        do
        {
            assert(place < s_romanConvertTableSize);

            const DecimalToRoman&   theCurrent =
                s_romanConvertTable[place];

            while (localValue >= theCurrent.m_postValue)
            {
                theResult += theCurrent.m_postLetter;
                localValue -= theCurrent.m_postValue;
            }

            if (prefixesAreOK)
            {
                if (localValue >= theCurrent.m_preValue)
                {
                    theResult += theCurrent.m_preLetter;
                    localValue -= theCurrent.m_preValue;
                }
            } 

            ++place;
        }
        while (localValue > 0);

        assert(localValue == 0);
    }
}



ElemNumber::NumberFormatStringTokenizer::NumberFormatStringTokenizer(
            const XalanDOMString&   theString) :
    m_currentPosition(0),
    m_maxPosition(theString.length()),
    m_string(&theString)
{
}



void
ElemNumber::NumberFormatStringTokenizer::setString(const XalanDOMString&    theString)
{
    m_string = &theString;

    m_currentPosition = 0;
    m_maxPosition = theString.length();
}



void
ElemNumber::NumberFormatStringTokenizer::nextToken(XalanDOMString&  theToken)
{
    if (m_currentPosition >= m_maxPosition) 
    {
        theToken.clear();
    }

    const size_type     start = m_currentPosition;

    if (isXMLLetterOrDigit((*m_string)[m_currentPosition]))
    {
        while (m_currentPosition < m_maxPosition &&
               isXMLLetterOrDigit((*m_string)[m_currentPosition]))
        {
            m_currentPosition++;
        }
    }
    else
    {
        while (m_currentPosition < m_maxPosition &&
               !isXMLLetterOrDigit((*m_string)[m_currentPosition]))
        {
            m_currentPosition++;
        }
    }

    substring(*m_string, theToken, start, m_currentPosition);
}



ElemNumber::NumberFormatStringTokenizer::size_type
ElemNumber::NumberFormatStringTokenizer::countTokens() const
{
    size_type   count = 0;
    size_type   currpos = m_currentPosition;

    // Tokens consist of sequences of alphabetic characters and sequences of
    // non-alphabetic characters
    while (currpos < m_maxPosition) 
    {
        if (isXMLLetterOrDigit((*m_string)[currpos]))
        {
            while (currpos < m_maxPosition &&
                   isXMLLetterOrDigit((*m_string)[currpos]))
            {
                currpos++;
            }
        }
        else
        {
            while (currpos < m_maxPosition &&
                   !isXMLLetterOrDigit((*m_string)[currpos]))
            {
                currpos++;
            }
        }

        count++;
    }

    return count;
}


#define ELEMNUMBER_SIZE(str)    ((sizeof(str) / sizeof(str[0]) - 1))


const XalanDOMChar  ElemNumber::s_alphaCountTable[] =
{
    XalanUnicode::charLetter_Z,
    XalanUnicode::charLetter_A,
    XalanUnicode::charLetter_B,
    XalanUnicode::charLetter_C,
    XalanUnicode::charLetter_D,
    XalanUnicode::charLetter_E,
    XalanUnicode::charLetter_F,
    XalanUnicode::charLetter_G,
    XalanUnicode::charLetter_H,
    XalanUnicode::charLetter_I,
    XalanUnicode::charLetter_J,
    XalanUnicode::charLetter_K,
    XalanUnicode::charLetter_L,
    XalanUnicode::charLetter_M,
    XalanUnicode::charLetter_N,
    XalanUnicode::charLetter_O,
    XalanUnicode::charLetter_P,
    XalanUnicode::charLetter_Q,
    XalanUnicode::charLetter_R,
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_T,
    XalanUnicode::charLetter_U,
    XalanUnicode::charLetter_V,
    XalanUnicode::charLetter_W,
    XalanUnicode::charLetter_X,
    XalanUnicode::charLetter_Y,
    0
};

const XalanDOMString::size_type     ElemNumber::s_alphaCountTableSize =
        ELEMNUMBER_SIZE(s_alphaCountTable);



const XalanDOMChar  ElemNumber::s_elalphaCountTable[] =
{
    0x03c9,
    0x03b1,
    0x03b2,
    0x03b3,
    0x03b4,
    0x03b5,
    0x03b6,
    0x03b7,
    0x03b8,
    0x03b9,
    0x03ba,
    0x03bb,
    0x03bc,
    0x03bd,
    0x03be,
    0x03bf,
    0x03c0,
    0x03c1,
    0x03c2,
    0x03c3,
    0x03c4,
    0x03c5,
    0x03c6,
    0x03c7,
    0x03c8,
    0
};


const XalanDOMString::size_type     ElemNumber::s_elalphaCountTableSize =
        ELEMNUMBER_SIZE(s_elalphaCountTable);



static XalanDOMString   s_staticTextString(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString   s_staticCommentString(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString   s_staticSlashString(XalanMemMgrs::getDummyMemMgr());



const XalanDOMChar      ElemNumber::s_atString[] =
{
    XalanUnicode::charAmpersand,
    0
};

const XalanDOMString&   ElemNumber::s_textString = s_staticTextString;

const XalanDOMString&   ElemNumber::s_commentString = s_staticCommentString;

const XalanDOMString&   ElemNumber::s_slashString = s_staticSlashString;

const XalanDOMChar      ElemNumber::s_piString[] =
{
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLeftParenthesis,
    0
};

const XalanDOMChar      ElemNumber::s_levelString[] =
{
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_v,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_l,
    0
};

const XalanDOMChar      ElemNumber::s_multipleString[] =
{
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    0
};

const XalanDOMChar      ElemNumber::s_anyString[] =
{
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_y,
    0
};

const XalanDOMChar      ElemNumber::s_singleString[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    0
};

const XalanDOMChar      ElemNumber::s_alphabeticString[] =
{
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_c,
    0
};

const XalanDOMChar      ElemNumber::s_traditionalString[] =
{
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    0
};



const XalanDOMChar      ElemNumber::s_errorString[] =
{
    XalanUnicode::charNumberSign,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_r,
    0
};



const DecimalToRoman    ElemNumber::s_romanConvertTable[] =
{    {
        1000,
        { XalanUnicode::charLetter_M, 0 },
        900,
        { XalanUnicode::charLetter_C, XalanUnicode::charLetter_M, 0 }
    },

    {
        500,
        { XalanUnicode::charLetter_D, 0 },
        400,
        { XalanUnicode::charLetter_C, XalanUnicode::charLetter_D, 0 }
    },

    {
        100,
        { XalanUnicode::charLetter_C, 0 },
        90,
        { XalanUnicode::charLetter_X, XalanUnicode::charLetter_C, 0 }
    },

    {
        50,
        { XalanUnicode::charLetter_L, 0 },
        40,
        { XalanUnicode::charLetter_X, XalanUnicode::charLetter_L, 0 }
    },

    {
        10,
        { XalanUnicode::charLetter_X, 0 },
        9,
        { XalanUnicode::charLetter_I, XalanUnicode::charLetter_X, 0 }
    },

    {
        5,
        { XalanUnicode::charLetter_V, 0 },
        4,
        { XalanUnicode::charLetter_I, XalanUnicode::charLetter_V, 0 }
    },

    {
        1,
        { XalanUnicode::charLetter_I, 0 },
        1,
        { XalanUnicode::charLetter_I, 0 }
    }
};


const size_t    ElemNumber::s_romanConvertTableSize = sizeof(s_romanConvertTable) / sizeof(s_romanConvertTable[0]);


static XalanNumberingResourceBundle     s_staticElalphaResourceBundle(XalanMemMgrs::getDummyMemMgr());

const XalanNumberingResourceBundle& ElemNumber::s_elalphaResourceBundle =
        s_staticElalphaResourceBundle;



static void
initializeTraditionalElalphaBundle(
            MemoryManager&              theManager, 
            XalanNumberingResourceBundle&   theBundle)
{


    // The following are a bunch of static data the comprise the contents of the bundle.
    static const XalanDOMChar   elalphaAlphabet[] =
    {
        0x03b1, 0x03b2, 0x03b3, 0x03b4, 0x03b5, 0x03b6, 0x03b7,  0x03b8,
        0x03b9, 0x03ba, 0x03bb, 0x03bc, 0x03bd, 0x03be, 0x03bf, 0x03c0,
        0x03c1, 0x03c2, 0x03c3, 0x03c4, 0x03c5, 0x03c6, 0x03c7, 0x03c8,
        0x03c9, 0
    };

    static const XalanDOMChar   elalphaTraditionalAlphabet[] =
    {
        XalanUnicode::charLetter_A, XalanUnicode::charLetter_B, XalanUnicode::charLetter_C,
        XalanUnicode::charLetter_D, XalanUnicode::charLetter_E, XalanUnicode::charLetter_F,
        XalanUnicode::charLetter_G, XalanUnicode::charLetter_H, XalanUnicode::charLetter_I,
        XalanUnicode::charLetter_J, XalanUnicode::charLetter_K, XalanUnicode::charLetter_L,
        XalanUnicode::charLetter_M, XalanUnicode::charLetter_N, XalanUnicode::charLetter_O,
        XalanUnicode::charLetter_P, XalanUnicode::charLetter_Q, XalanUnicode::charLetter_R,
        XalanUnicode::charLetter_S, XalanUnicode::charLetter_T, XalanUnicode::charLetter_U,
        XalanUnicode::charLetter_V, XalanUnicode::charLetter_W, XalanUnicode::charLetter_X,
        XalanUnicode::charLetter_Y, XalanUnicode::charLetter_Z, 0
    };

    typedef XalanNumberingResourceBundle::NumberType    NumberType;

    static const NumberType     elalphaNumberGroups[] =
    {
        100, 10, 1
    };

    const size_t    elalphaNumberGroupsCount = sizeof(elalphaNumberGroups) / sizeof(elalphaNumberGroups[0]);

    static const NumberType     elalphaMultipliers[] = { 1000 };

    const size_t    elalphaMultipliersCount = sizeof(elalphaMultipliers) / sizeof(elalphaMultipliers[0]);

    static const XalanDOMChar   elalphaMultiplierChars[] = { 0x03d9, 0 };

    static const XalanDOMChar   elalphaDigits[] =
    {
        0x03b1, 0x03b2, 0x03b3, 0x03b4, 0x03b5, 0x03db, 0x03b6, 0x03b7, 0x03b8, 0
    };

    static const XalanDOMChar   elalphaTens[] =
    {
        0x03b9, 0x03ba, 0x03bb, 0x03bc, 0x03bd, 0x03be, 0x03bf, 0x03c0, 0x03df, 0
    };

    static const XalanDOMChar   elalphaHundreds[] =
    {
        0x03c1, 0x03c2, 0x03c4, 0x03c5, 0x03c6, 0x03c7, 0x03c8, 0x03c9, 0x03e1, 0
    };

    typedef XalanNumberingResourceBundle::DigitsTableVectorType     DigitsTableVectorType;
    typedef XalanNumberingResourceBundle::NumberTypeVectorType          NumberTypeVectorType;

    // Create the table of characters for the various digit positions...
    DigitsTableVectorType           theElalphaDigitsTable(theManager);

    // Since we know the size, create the empty vectors in the table...
    theElalphaDigitsTable.resize(3);

    // Swap the empty tables with temporary ones...
    XalanDOMCharVectorType(
        elalphaDigits,
        elalphaDigits + length(elalphaDigits),
        theManager).swap(theElalphaDigitsTable[0]);

    XalanDOMCharVectorType(
        elalphaTens,
        elalphaTens + length(elalphaTens),
        theManager).swap(theElalphaDigitsTable[1]);

    XalanDOMCharVectorType(
        elalphaHundreds,
        elalphaHundreds + length(elalphaHundreds),
        theManager).swap(theElalphaDigitsTable[2]);

    // This table will indicate which positions the vectors of digits are in
    // the table...
    NumberTypeVectorType    theDigitsTableTable(theManager);

    theDigitsTableTable.reserve(3);

    // Push the positions on in reverse order, since that's how things work...
    theDigitsTableTable.push_back(2);
    theDigitsTableTable.push_back(1);
    theDigitsTableTable.push_back(0);

    const XalanDOMString    theLanguageString("el", theManager);

    // Create an instance...
    XalanNumberingResourceBundle    theElaphaBundle(
        theLanguageString,
        theLanguageString,
        theLanguageString,
        XalanDOMCharVectorType(
            elalphaAlphabet,
            elalphaAlphabet + length(elalphaAlphabet),
            theManager),
        XalanDOMCharVectorType(
            elalphaTraditionalAlphabet,
            elalphaTraditionalAlphabet + length(elalphaTraditionalAlphabet),
            theManager),
        XalanNumberingResourceBundle::eLeftToRight,
        XalanNumberingResourceBundle::eMultiplicativeAdditive,
        XalanNumberingResourceBundle::ePrecedes,
        ~NumberType(0),
        NumberTypeVectorType(
            elalphaNumberGroups,
            elalphaNumberGroups + elalphaNumberGroupsCount,
            theManager),
        NumberTypeVectorType(
            elalphaMultipliers,
            elalphaMultipliers + elalphaMultipliersCount,
            theManager),
        XalanDOMCharVectorType(theManager),
        XalanDOMCharVectorType(
            elalphaMultiplierChars,
            elalphaMultiplierChars + length(elalphaMultiplierChars),
            theManager),
        theElalphaDigitsTable,
        theDigitsTableTable,
        theManager);

    // Swap it with the one (this avoids making a copy...)
    theBundle.swap(theElaphaBundle);
}



static const XalanDOMChar   s_textStringChars[] =
{
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLeftParenthesis,
    XalanUnicode::charRightParenthesis,
    0
};



static const XalanDOMChar   s_commentStringChars[] =
{
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLeftParenthesis,
    XalanUnicode::charRightParenthesis,
    0
};



static const XalanDOMChar   s_solidusStringChars[] =
{
    XalanUnicode::charSolidus,
    0
};



void
ElemNumber::initialize(MemoryManager&  theManager)
{
    s_staticTextString.reset(theManager, s_textStringChars);

    s_staticCommentString.reset(theManager, s_commentStringChars);

    s_staticSlashString.reset(theManager, s_solidusStringChars);

    initializeTraditionalElalphaBundle(theManager, s_staticElalphaResourceBundle);
}



void
ElemNumber::terminate()
{
    MemoryManager& theManager = XalanMemMgrs::getDummyMemMgr();

    releaseMemory(s_staticTextString, theManager);
    releaseMemory(s_staticCommentString, theManager);
    releaseMemory(s_staticSlashString, theManager);

    XalanNumberingResourceBundle(theManager).swap(s_staticElalphaResourceBundle);
}



const XPath*
ElemNumber::getXPath(XalanSize_t    index) const
{
    const XPath*    result = 0;

    switch(index)
    {
    case 0:
        result = m_valueExpr;
        break;

    case 1:
        result = m_countMatchPattern;
        break;

    case 2:
        result = m_fromMatchPattern;
        break;

    default:
        break;
    }

    return result;
}



XALAN_CPP_NAMESPACE_END
