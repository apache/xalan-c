/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
#include "ElemNumber.hpp"



#include <sax/AttributeList.hpp>



#include <XalanDOM/XalanDocument.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/NumberFormat.hpp>



#include <XPath/XPath.hpp>



#include "Constants.hpp"
#include "NumeratorFormatter.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"

#if !defined(XALAN_NO_NAMESPACES)
	using std::vector;
#endif


const XalanDOMString	ElemNumber::s_alphaCountTable(XALAN_STATIC_UCODE_STRING("ZABCDEFGHIJKLMNOPQRSTUVWXY"));



const DecimalToRoman	ElemNumber::s_romanConvertTable[] = 
{
	DecimalToRoman(1000, XALAN_STATIC_UCODE_STRING("M"), 900, XALAN_STATIC_UCODE_STRING("CM")),
	DecimalToRoman(500, XALAN_STATIC_UCODE_STRING("D"), 400, XALAN_STATIC_UCODE_STRING("CD")),
	DecimalToRoman(100L, XALAN_STATIC_UCODE_STRING("C"), 90L, XALAN_STATIC_UCODE_STRING("XC")),
	DecimalToRoman(50L, XALAN_STATIC_UCODE_STRING("L"), 40L, XALAN_STATIC_UCODE_STRING("XL")),
	DecimalToRoman(10L, XALAN_STATIC_UCODE_STRING("X"), 9L, XALAN_STATIC_UCODE_STRING("IX")),
	DecimalToRoman(5L, XALAN_STATIC_UCODE_STRING("V"), 4L, XALAN_STATIC_UCODE_STRING("IV")),
	DecimalToRoman(1L, XALAN_STATIC_UCODE_STRING("I"), 1L, XALAN_STATIC_UCODE_STRING("I"))
};



ElemNumber::ElemNumber(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMString&			name,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						name,
						lineNumber,
						columnNumber,
						Constants::ELEMNAME_NUMBER),	
	m_countMatchPattern(0),
	m_fromMatchPattern(0),
	m_valueExpr(0),
	m_level(Constants::NUMBERLEVEL_SINGLE),
	m_format_avt(),
	m_lang_avt(),  
	m_lettervalue_avt(),
	m_groupingSeparator_avt(),
	m_groupingSize_avt()
	
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_LEVEL))
		{
			const XalanDOMChar*	const	levelValue = atts.getValue(i);

			if(equals(Constants::ATTRVAL_MULTI, levelValue))
				m_level = Constants::NUMBERLEVEL_MULTI;
			else if(equals(levelValue,Constants::ATTRVAL_ANY))
				m_level = Constants::NUMBERLEVEL_ANY;
			else if(equals(levelValue,Constants::ATTRVAL_SINGLE))
				m_level = Constants::NUMBERLEVEL_SINGLE;
			else
				error(XalanDOMString("Bad value on level attribute: ") + levelValue);
		}
		else if(equals(aname, Constants::ATTRNAME_COUNT))
		{
			m_countMatchPattern = constructionContext.createMatchPattern(atts.getValue(i), *this);
		}
		else if(equals(aname, Constants::ATTRNAME_FROM))
		{
			m_fromMatchPattern = constructionContext.createMatchPattern(atts.getValue(i), *this);
		}
		else if(equals(aname, Constants::ATTRNAME_VALUE))
		{
			m_valueExpr = constructionContext.createXPath(atts.getValue(i), *this);
		}
		else if(equals(aname, Constants::ATTRNAME_FORMAT))
		{
			m_format_avt = atts.getValue(i);
		}
		else if(equals(aname, Constants::ATTRNAME_LANG))
		{
			m_lang_avt = atts.getValue(i);
		}
		else if(equals(aname, Constants::ATTRNAME_LETTERVALUE))
		{
			constructionContext.warn(Constants::ATTRNAME_LETTERVALUE + " not supported yet!");

			m_lettervalue_avt = atts.getValue(i);
		} 
		else if(equals(aname,Constants::ATTRNAME_GROUPINGSEPARATOR))
		{
			m_groupingSeparator_avt = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_GROUPINGSIZE))
		{
			m_groupingSize_avt = atts.getValue(i);
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(name + " has an illegal attribute: " + aname);
		}
	}
}



void
ElemNumber::execute(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const
{
	ElemTemplateElement::execute(executionContext, sourceTree, sourceNode, mode);

	const XalanDOMString	countString =
		getCountString(executionContext, sourceTree, sourceNode);

	if (!isEmpty(countString))
	{
		executionContext.characters(toCharArray(countString), 0, length(countString));
	}
}



XalanNode*
ElemNumber::findAncestor(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern,
			XalanNode*						context,
			const XalanElement*				/* namespaceContext */) const
{
	XalanNode*	contextCopy = context;

	while(contextCopy != 0)
	{
		if(0 != fromMatchPattern)
		{
			if(fromMatchPattern->getMatchScore(contextCopy,
											   *this,
											   executionContext.getXPathExecutionContext()) !=
							XPath::s_MatchScoreNone)
			{
				break;
			}
		}
		
		if(0 != countMatchPattern)
		{
			if(countMatchPattern->getMatchScore(context,
											    *this,
												executionContext.getXPathExecutionContext()) !=
							XPath::s_MatchScoreNone)
			{
				break;
			}
		}
		
		contextCopy = executionContext.getParentOfNode(*contextCopy);
	}

	return contextCopy;
}					



XalanNode*
ElemNumber::findPrecedingOrAncestorOrSelf(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern,
			XalanNode*						context,
			const XalanElement*				/* namespaceContext */) const
{  
	XalanNode*	contextCopy = context;

	while(contextCopy != 0)
	{
		if(0 != fromMatchPattern)
		{
			if(fromMatchPattern->getMatchScore(contextCopy,
											   *this,
											   executionContext.getXPathExecutionContext()) != XPath::s_MatchScoreNone)
			{
				contextCopy = 0;
				break;
			}
		}
		
		if(0 != countMatchPattern)
		{
			if(countMatchPattern->getMatchScore(contextCopy,
											    *this,
											    executionContext.getXPathExecutionContext()) != XPath::s_MatchScoreNone)
			{
				break;
			}
		}

		XalanNode* const	prevSibling = contextCopy->getPreviousSibling();

		if(prevSibling == 0)
		{
			contextCopy = executionContext.getParentOfNode(*contextCopy);
		}
		else
		{
			contextCopy = prevSibling;
		}
	}

	return contextCopy;
}



const XPath*
ElemNumber::getCountMatchPattern(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						contextNode) const
{
	const XPath*	countMatchPattern = m_countMatchPattern;

	if(0 == countMatchPattern)
	{
		switch(contextNode->getNodeType())
		{
		case XalanNode::ELEMENT_NODE:
			countMatchPattern = executionContext.createMatchPattern(contextNode->getNodeName(),
				*this);
			break;

		case XalanNode::ATTRIBUTE_NODE:
			countMatchPattern = executionContext.createMatchPattern(XalanDOMString(XALAN_STATIC_UCODE_STRING("@")) +
																	contextNode->getNodeName(),
				*this);
			break;

		case XalanNode::CDATA_SECTION_NODE:
		case XalanNode::TEXT_NODE:
			countMatchPattern = executionContext.createMatchPattern(XalanDOMString(XALAN_STATIC_UCODE_STRING("text()")),
				*this);
			break;

		case XalanNode::COMMENT_NODE:
			countMatchPattern = executionContext.createMatchPattern(XalanDOMString(XALAN_STATIC_UCODE_STRING("comment()")),
				*this);
			break;

		case XalanNode::DOCUMENT_NODE:
			countMatchPattern = executionContext.createMatchPattern(XalanDOMString(XALAN_STATIC_UCODE_STRING("/")),
				*this);
			break;

		case XalanNode::PROCESSING_INSTRUCTION_NODE:
			countMatchPattern = executionContext.createMatchPattern(XalanDOMString(XALAN_STATIC_UCODE_STRING("pi(")) + 
				contextNode->getNodeName() + XalanDOMString(XALAN_STATIC_UCODE_STRING(")")),
				*this);
			break;

		default:
			assert(false);
			break;
		}
	}

	return countMatchPattern;
}



XalanDOMString
ElemNumber::getCountString(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						/* sourceTree */, 
			XalanNode*						sourceNode) const
{
	assert(sourceNode != 0);
	IntArrayType	numberList;

	IntArrayType	list;

	if(0 != m_valueExpr)
	{
		const XObject* const	countObj =
			m_valueExpr->execute(sourceNode,
								 *this,
								 executionContext.getXPathExecutionContext());

		numberList.push_back(static_cast<int>(countObj->num()));
	}
	else
	{
		const XPath* const	countMatchPattern =
			getCountMatchPattern(executionContext, sourceNode);

		if((Constants::NUMBERLEVEL_ANY == m_level) || 
			(Constants::NUMBERLEVEL_SINGLE == m_level))
		{
			if(Constants::NUMBERLEVEL_SINGLE == m_level)
			{
				XalanNode*	target =
					findAncestor(executionContext,
								 m_fromMatchPattern, 
								 countMatchPattern,
								 sourceNode,
								 this);

				if(target == 0) 
					target = executionContext.getParentOfNode(*sourceNode);

				if(target != 0)
				{
					numberList.push_back(getSiblingNumber(executionContext, countMatchPattern, target));
				}
				else
				{
					executionContext.warn(XalanDOMString("Warning: count attribute does not match an ancestor in xsl:number! Target = ") 
													+ sourceNode->getNodeName(),
										  sourceNode,
										  0);
				}
			}
			else // if NUMBERLEVEL_ANY
			{
				XalanNode*	from = 0;

				if(0 != m_fromMatchPattern)
				{
// @@ JMD: was as below, which looked wrong to me based on java code and the
// meaning of the arguments. The sad fact is that the number of tests we
// passed went DOWN after this change
// 					from = findPrecedingOrAncestorOrSelf(executionContext, 0, m_fromMatchPattern, 
					from = findPrecedingOrAncestorOrSelf(executionContext, m_fromMatchPattern, countMatchPattern,
						sourceNode, this);

					if(from == 0)
					{
						from = sourceNode;
					}
				}
				else
				{
					from = sourceNode->getOwnerDocument();
				}

				XalanNode* const	fromPos = (from != sourceNode) ? getNextInTree(from, from) : from;

				numberList.push_back(getNumberInTree(executionContext.getXPathExecutionContext(), countMatchPattern, fromPos, from, sourceNode, 0));
			}
		}
		else // if NUMBERLEVEL_MULTI
		{
			 numberList = getAncestorNumbers(executionContext, m_fromMatchPattern,
			 	countMatchPattern, sourceNode);
		}
	}

	return numberList.size() > 0 ? formatNumberList(executionContext, numberList, sourceNode) : XalanDOMString();
}



XalanNode*
ElemNumber::getNextInTree(
			XalanNode*	pos,
			XalanNode*	from)
{
	assert(pos != 0);
	assert(from != 0);

	XalanNode*	posCopy = pos;

	XalanNode*	nextNode = posCopy->getFirstChild();

	while(nextNode == 0)
	{
		nextNode = posCopy->getNextSibling();

		if(nextNode == 0)
		{
			posCopy = posCopy->getParentNode();

			if(posCopy == from)
			{
				break;
			}
		}
	}

	return nextNode;
}



unsigned int
ElemNumber::getNumberInTree(	
			XPathExecutionContext&	executionContext,
			const XPath*			countMatchPattern, 
			XalanNode*				pos, 
			XalanNode*				from, 
			XalanNode*				target,
			int						countFrom) const
{
	XalanNode*	posCopy = pos;

	int			count = countFrom;

	if(posCopy != 0)
	{
		do
		{          
			if( (0 == countMatchPattern) || 
				(countMatchPattern->getMatchScore(posCopy,
												  *this,
												  executionContext) != XPath::s_MatchScoreNone))
			{
				count++;
			}
		}

		while(posCopy != target && 
				(posCopy = getNextInTree(posCopy, from)) != 0);
	}

	return count;
}



unsigned int
ElemNumber::getSiblingNumber(
			StylesheetExecutionContext&		executionContext,
			const XPath*					countMatchPattern, 
			XalanNode*						target) const
{
	assert(target != 0);

	int number = 0;

	const XalanNode* const	theParent = executionContext.getParentOfNode(*target);
	assert(theParent != 0);

	// TODO: If target is an Attr, implement special handling. 
	XalanNode*		child = theParent->getFirstChild();

	while(child != 0)
	{
		if(child == target)
		{
			number++; // always count the target
			break;
		}
		else if(0 == countMatchPattern || 
				countMatchPattern->getMatchScore(child,
												 *this,
												 executionContext.getXPathExecutionContext()) != XPath::s_MatchScoreNone)
		{
			number++;
		}

		child = child->getNextSibling();
	}

	return number;
}



unsigned int
ElemNumber::countMatchingAncestors(
			StylesheetExecutionContext&		executionContext,
			const XPath*					patterns,
			XalanNode*						node) const
{
	int			count = 0;

	XalanNode*	nodeCopy = node;

	while(nodeCopy != 0)
	{
		if(0 != patterns)
		{
			if(patterns->getMatchScore(nodeCopy,
									   *this,
									   executionContext.getXPathExecutionContext()) != XPath::s_MatchScoreNone)
			{
				count++;
			}  
		}
		else
		{
			count++;
		}

		nodeCopy = executionContext.getParentOfNode(*nodeCopy);
	}

	return count;
}



ElemNumber::IntArrayType
ElemNumber::getAncestorNumbers(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern, 
			XalanNode*						node) const
{
	XalanNode*			nodeCopy = node;

	const unsigned int	nMatchingAncestors =
				countMatchingAncestors(executionContext,
									   countMatchPattern,
									   nodeCopy);

	IntArrayType	counts(nMatchingAncestors);

	if(nMatchingAncestors > 0)
	{
		int		countIndex = counts.size() - 1; // position to put count into

		while(nodeCopy != 0)
		{
			bool	countIt = false;

			if(0 != countMatchPattern)
			{
				if(countMatchPattern->getMatchScore(nodeCopy,
													*this,
													executionContext.getXPathExecutionContext()) != XPath::s_MatchScoreNone)
				{
					countIt = true;
				} 
			}
			else
			{
				countIt = true;
			}

			if(countIt == true)
			{
				XalanNode*	target =
						findAncestor(executionContext,
									 fromMatchPattern,
									 countMatchPattern,
									 nodeCopy,
									 this);

				if(target == 0)
					target = nodeCopy;

				counts[countIndex] = getSiblingNumber(executionContext, countMatchPattern, target);
				countIndex--;
			}

			nodeCopy = executionContext.getParentOfNode(*nodeCopy);
		} // end while
	} // end if nMatchingAncestors > 0

	return counts;
}



#if ! defined(__GNUC__)

std::locale
ElemNumber::getLocale(
			StylesheetExecutionContext&		/* executionContext */,
			XalanNode*						/* contextNode */) const
{
	//TODO
	return std::locale();
}

#endif


NumberFormat*
ElemNumber::getNumberFormatter(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						contextNode) const
{
#if ! defined(__GNUC__)
	std::locale loc = getLocale(executionContext, contextNode);
#endif
    
    // Helper to format local specific numbers to strings.
	std::auto_ptr<NumberFormat>		formatter(new NumberFormat);

	const XalanDOMString	digitGroupSepValue = (!isEmpty(m_groupingSeparator_avt))
                                  ?  executionContext.evaluateAttrVal(contextNode,
																	  *this,
																	  m_groupingSeparator_avt) :
									 XalanDOMString();

    const XalanDOMString	nDigitsPerGroupValue = (!isEmpty(m_groupingSize_avt))
                                  ?  executionContext.evaluateAttrVal(contextNode,
																	  *this, 
																	  m_groupingSize_avt) :
									 XalanDOMString();

    // TODO: Handle digit-group attributes
    if(!isEmpty(digitGroupSepValue) || !isEmpty(nDigitsPerGroupValue))
    {
		formatter->setGroupingUsed(true);
		formatter->setGroupingSeparator(m_groupingSeparator_avt);
		formatter->setGroupingSize(m_groupingSize_avt);
    }
    
    return formatter.release();
}



XalanDOMString
ElemNumber::formatNumberList(
			StylesheetExecutionContext&		executionContext,
			const IntArrayType&				theList,
			XalanNode*						contextNode) const
{
	const IntArrayType::size_type	nNumbers = theList.size();
	XalanDOMChar	numberType('1');
	int			numberWidth = 1;

	XalanDOMString	formattedNumber;
	XalanDOMString	formatToken;
	XalanDOMString	sepString(XALAN_STATIC_UCODE_STRING("."));
	XalanDOMString	lastSepString;

	XalanDOMString	formatValue = !isEmpty(m_format_avt)
		? executionContext.evaluateAttrVal(contextNode,
										   *this,
										   m_format_avt)
		: XalanDOMString();

	if(isEmpty(formatValue)) 
		formatValue = XALAN_STATIC_UCODE_STRING("1");

	NumeratorFormatter::NumberFormatStringTokenizer		formatTokenizer(formatValue);

#if ! defined(__GNUC__)
	std::locale		loc = getLocale(executionContext, contextNode);
#endif

	typedef vector<XalanDOMString>		StringVectorType;
	typedef StringVectorType::iterator	StringVectorTypeIterator;

	// Construct an array of tokens.  We need to be able to check if the last
	// token in non-alphabetic, in which case the penultimate non-alphabetic is
	// the repeating separator
	StringVectorType tokenVector;
	while(formatTokenizer.hasMoreTokens())
		tokenVector.push_back(formatTokenizer.nextToken());

	// Get rid of the leading and trailing non-alphabetics, save for later
	XalanDOMString leaderStr;
	XalanDOMString trailerStr;
	StringVectorTypeIterator it;
	it = tokenVector.begin();
	if(! isLetterOrDigit(charAt((*it), 0)))
	{
		leaderStr = *it;
		tokenVector.erase(it);
	}
	it += tokenVector.size()-1;
	if(! isLetterOrDigit(charAt((*it), 0)))
	{
		trailerStr = *it;
		tokenVector.erase(it);
	}

	// Now we're left with a sequence of alpha,non-alpha tokens, format them
	// with the corresponding entry in the format string, or the last one if no
	// more matching ones
	formattedNumber = leaderStr;
	it = tokenVector.begin();
	for(unsigned int i = 0; i < nNumbers; i++)
	{
		if (it != tokenVector.end())
		{
			assert(isLetterOrDigit(charAt((*it), 0)));
			formatToken = *it++;
			numberWidth = length(formatToken);
			numberType = charAt(formatToken, numberWidth - 1);
		}
		if (it != tokenVector.end())
		{
			assert(!isLetterOrDigit(charAt((*it), 0)));
			sepString = *it++;
		}			
		formattedNumber += getFormattedNumber(executionContext, contextNode,
				numberType, numberWidth, theList[i]);
		// All but the last one
		if (i < nNumbers-1)
			formattedNumber += sepString;
	}
	formattedNumber += trailerStr;

	return formattedNumber;  
}



XalanDOMString
ElemNumber::getFormattedNumber(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						contextNode,
			XalanDOMChar					numberType,
			int								numberWidth,
			int								listElement) const
{

	std::auto_ptr<NumberFormat> formatter(getNumberFormatter(executionContext, contextNode));

	XalanDOMString	padString = formatter->format(0);
	XalanDOMString	lookahead;

	XalanDOMString	formattedNumber;

	switch(numberType)
	{
		case 'A':
			formattedNumber += int2alphaCount(listElement, s_alphaCountTable);
			break;
		case 'a':
			formattedNumber += toLowerCase(int2alphaCount(listElement, s_alphaCountTable));
			break;
		case 'I':
			formattedNumber += long2roman(listElement, true);
			break;
		case 'i':
			formattedNumber += toLowerCase(long2roman(listElement, true));
			break;

		default: // "1"
			{
				const XalanDOMString		numString =
					formatter->format(listElement);

				const int	nPadding = numberWidth - length(numString);

				for(int i = 0; i < nPadding; i++)
				{
					formattedNumber += padString;
				}

				formattedNumber += numString;
			}
			break;
	}

	return formattedNumber;  
}



/**
 * Convert a long integer into alphabetic counting, in other words 
 * count using the sequence A B C ... Z AA AB AC.... etc.
 * @param val Value to convert -- must be greater than zero.
 * @param table a table containing one character for each digit in the radix
 * @return String representing alpha count of number.
 * @see XSLTEngineImpl#DecimalToRoman
 * 
 * Note that the radix of the conversion is inferred from the size
 * of the table.
 */
XalanDOMString
ElemNumber::int2alphaCount(
			int						val,
			const XalanDOMString&	table)
{
	const int		radix = length(table);

	// Create a buffer to hold the result
	// TODO:  size of the table can be determined by computing
	// logs of the radix.  For now, we fake it.  
	const int		buflen = 100;

	XalanDOMChar	buf[buflen + 1];
	memset(buf, 0, buflen + 1);

	// next character to set in the buffer
	int charPos = buflen - 1 ;    // work backward through buf[]

	// index in table of the last character that we stored
	int lookupIndex = 1;  // start off with anything other than zero to make correction work
	
	//						Correction number
	//
	//	Correction can take on exactly two values:
	//
	//		0	if the next character is to be emitted is usual
	//
	//      radix - 1 
	//			if the next char to be emitted should be one less than
	//			you would expect 
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

	int correction = 0;

	// TODO:  throw error on out of range input

	do
	{
		// most of the correction calculation is explained above,  the reason for the
		// term after the "|| " is that it correctly propagates carries across
		// multiple columns.  
		correction = ((lookupIndex == 0) || 
			(correction != 0 && lookupIndex == radix-1 )) ? (radix-1) : 0;

		// index in "table" of the next char to emit
		lookupIndex  = (val + correction) % radix;  

		// shift input by one "column"
		val = (val / radix);

		// if the next value we'd put out would be a leading zero, we're done.
		if (lookupIndex == 0 && val == 0)
			break;

		// put out the next character of output
		buf[charPos--] = static_cast<char>(charAt(table, lookupIndex));
	}
	while (val > 0);

	XalanDOMString retStr(buf + charPos + 1, (buflen - charPos - 1));

	return retStr;
}



XalanDOMString
ElemNumber::long2roman(
			long	val,
			bool	prefixesAreOK)
{
	if(val <= 0)
	{
		return XalanDOMString(XALAN_STATIC_UCODE_STRING("#E(") +
								LongToDOMString(val) +
								XALAN_STATIC_UCODE_STRING(")"));
	}

	XalanDOMString	roman;

	int			place = 0;

	if (val <= 3999L)
	{
		do      
		{
			while (val >= s_romanConvertTable[place].m_postValue)            
			{
				roman += s_romanConvertTable[place].m_postLetter;
				val -= s_romanConvertTable[place].m_postValue;
			}
			if (prefixesAreOK)            
			{
				if (val >= s_romanConvertTable[place].m_preValue)                  
				{
					roman += s_romanConvertTable[place].m_preLetter;
					val -= s_romanConvertTable[place].m_preValue;
				}
			} 
			place++;      
		}
		while (val > 0);
	}
	else
	{
		roman = XALAN_STATIC_UCODE_STRING("#error");
	}

	return roman;
}
