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


#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/NumberFormat.hpp>
#include <XPath/XPath.hpp>



#include "Constants.hpp"
#include "NumeratorFormatter.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



const DOMString			ElemNumber::s_alphaCountTable(XALAN_STATIC_UCODE_STRING("ZABCDEFGHIJKLMNOPQRSTUVWXY"));



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
			const DOMString&				name,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						name,
						lineNumber,
						columnNumber),	
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
	const int	nAttrs = atts.getLength();

	for(int i = 0; i < nAttrs; i++)
	{
		const DOMString		aname(atts.getName(i));

		if(equals(aname, Constants::ATTRNAME_LEVEL))
		{
			const DOMString levelValue = atts.getValue(i);

			if(!isEmpty(levelValue))
			{
				if(equals(Constants::ATTRVAL_MULTI, levelValue))
					m_level = Constants::NUMBERLEVEL_MULTI;
				else if(equals(levelValue,Constants::ATTRVAL_ANY))
					m_level = Constants::NUMBERLEVEL_ANY;
				else if(equals(levelValue,Constants::ATTRVAL_SINGLE))
					m_level = Constants::NUMBERLEVEL_SINGLE;
				else
					error("Bad value on level attribute: " + levelValue);
			}
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

	
int
ElemNumber::getXSLToken() const 
{
	return Constants::ELEMNAME_NUMBER;
}
	


void ElemNumber::execute(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					sourceTree, 
			const DOM_Node&					sourceNode,
			const QName&					mode) const
{
	ElemTemplateElement::execute(executionContext, sourceTree, sourceNode, mode);

	DOMString countString = getCountString(executionContext, sourceTree, sourceNode);
	
	if (!isEmpty(countString))
	{
		executionContext.characters(toCharArray(countString), 0, length(countString));
	}
}



/**
 * Add a child to the child list.
 */
NodeImpl* ElemNumber::appendChild(NodeImpl* newChild)
{
    error("Can not add " + dynamic_cast<ElemTemplateElement*>(newChild)->getTagName() + " to " + this->getTagName());

    return 0;
}



/**
 * Given a 'from' pattern (ala xsl:number), a match pattern 
 * and a context, find the first ancestor that matches the 
 * pattern (including the context handed in).
 * @param matchPatternString The match pattern.
 * @param node The node that "." expresses.
 * @param namespaceContext The context in which namespaces in the 
 * queries are supposed to be expanded.
 */
DOM_Node
ElemNumber::findAncestor(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern,
			const DOM_Node&					context,
			const DOM_Element&				/* namespaceContext */) const
{
	DOM_Node contextCopy(context);

	while(contextCopy != 0)
	{
		if(0 != fromMatchPattern)
		{
			if(fromMatchPattern->getMatchScore(contextCopy,
											   executionContext.getXPathExecutionContext()) != XPath::s_MatchScoreNone)
			{
				contextCopy = 0;
				break;
			}
		}
		
		if(0 != countMatchPattern)
		{
			if(countMatchPattern->getMatchScore(context,
												executionContext.getXPathExecutionContext()) != XPath::s_MatchScoreNone)
			{
				break;
			}
		}
		
		contextCopy = executionContext.getParentOfNode(contextCopy);
	}

	return contextCopy;
}					



DOM_Node
ElemNumber::findPrecedingOrAncestorOrSelf(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern,
			const DOM_Node&					context,
			const DOM_Element&				/* namespaceContext */) const
{  
	DOM_Node contextCopy(context);
    
	while(contextCopy != 0)
	{
		if(0 != fromMatchPattern)
		{
			if(fromMatchPattern->getMatchScore(contextCopy,
											   executionContext.getXPathExecutionContext()) != XPath::s_MatchScoreNone)
			{
				contextCopy = 0;
				break;
			}
		}
		
		if(0 != countMatchPattern)
		{
			if(countMatchPattern->getMatchScore(contextCopy,
											    executionContext.getXPathExecutionContext()) != XPath::s_MatchScoreNone)
			{
				break;
			}
		}

		DOM_Node	prevSibling = contextCopy.getPreviousSibling();

		if(prevSibling == 0)
		{
			contextCopy = executionContext.getParentOfNode(contextCopy);
		}
		else
		{
			contextCopy = prevSibling;
		}
	}

	return contextCopy;
}



/**
 * Get the count match pattern, or a default value.
 */
const XPath*
ElemNumber::getCountMatchPattern(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					contextNode) const
{
	const XPath*	countMatchPattern = m_countMatchPattern;

	if(0 == countMatchPattern)
	{
		switch(contextNode.getNodeType())
		{
		case DOM_Node::ELEMENT_NODE:
			countMatchPattern = executionContext.createMatchPattern(contextNode.getNodeName(),
				*this);
			break;

		case DOM_Node::ATTRIBUTE_NODE:
			countMatchPattern = executionContext.createMatchPattern(DOMString("@") + contextNode.getNodeName(),
				*this);
			break;

		case DOM_Node::CDATA_SECTION_NODE:
		case DOM_Node::TEXT_NODE:
			countMatchPattern = executionContext.createMatchPattern(DOMString("text()"), 
				*this);
			break;

		case DOM_Node::COMMENT_NODE:
			countMatchPattern = executionContext.createMatchPattern(DOMString("comment()"), 
				*this);
			break;

		case DOM_Node::DOCUMENT_NODE:
			countMatchPattern = executionContext.createMatchPattern(DOMString("/"), 
				*this);
			break;

		case DOM_Node::PROCESSING_INSTRUCTION_NODE:
			countMatchPattern = executionContext.createMatchPattern(DOMString("pi(") + 
				contextNode.getNodeName() + DOMString(")"),
				*this);
			break;

		default:
			assert(false);
			break;
		}
	}

	return countMatchPattern;
}



DOMString
ElemNumber::getCountString(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					/* sourceTree */, 
			const DOM_Node&					sourceNode) const
{
	IntArrayType	list;

	if(0 != m_valueExpr)
	{
		const XObject* const	countObj =
			m_valueExpr->execute(sourceNode,
								 *this,
								 executionContext.getXPathExecutionContext());

		list.push_back(static_cast<int>(countObj->num()));
	}
	else
	{      
		const XPath* const	countMatchPattern =
			getCountMatchPattern(executionContext, sourceNode);

		if((Constants::NUMBERLEVEL_ANY == m_level) || 
			(Constants::NUMBERLEVEL_SINGLE == m_level))
		{
			list.push_back(0);

			if(Constants::NUMBERLEVEL_SINGLE == m_level)
			{
				DOM_Node target = findAncestor(executionContext, m_fromMatchPattern, 
					countMatchPattern, sourceNode, DOM_UnimplementedElement(const_cast<ElemNumber*>(this)));

				if(target == 0) 
					target = executionContext.getParentOfNode(sourceNode);

				if(target != 0)
				{
					list[0] = getSiblingNumber(executionContext, countMatchPattern, target);
				}
				else
				{
					executionContext.warn(DOMString("Warning: count attribute does not match an ancestor in xsl:number! Target = ") 
													+ sourceNode.getNodeName(),
										  sourceNode,
										  DOM_Node());
				}
			}
			else // if NUMBERLEVEL_ANY
			{
				DOM_Node from;

				if(0 != m_fromMatchPattern)
				{
					from = findPrecedingOrAncestorOrSelf(executionContext, 0, m_fromMatchPattern, 
						sourceNode, DOM_UnimplementedElement(const_cast<ElemNumber*>(this)));

					if(from == 0)
					{
						from = sourceNode;
					}
				}
				else
				{
					from = sourceNode.getOwnerDocument();
				}

				DOM_Node fromPos = (from != sourceNode) ? getNextInTree(from, from) : from;

				list[0] = getNumberInTree(executionContext.getXPathExecutionContext(), countMatchPattern, fromPos, from, sourceNode, 0);
			}
		}
		else // if NUMBERLEVEL_MULTI
		{
			list = getAncestorNumbers(executionContext, m_fromMatchPattern,
				countMatchPattern, sourceNode);
		}
	}

	return list.size() > 0 ? formatNumberList(executionContext, list, sourceNode) : DOMString();
}



DOM_Node
ElemNumber::getNextInTree(
			const DOM_Node&		pos,
			const DOM_Node&		from)
{
	DOM_Node	posCopy(pos);

	DOM_Node	nextNode(posCopy.getFirstChild());

	while(nextNode == 0)
	{
		nextNode = posCopy.getNextSibling();

		if(nextNode == 0)
		{
			posCopy = posCopy.getParentNode();

			if(posCopy == from)
			{
				break;
			}
		}
	}

	return nextNode;
}



int
ElemNumber::getNumberInTree(	
			XPathExecutionContext&	executionContext,
			const XPath*			countMatchPattern, 
			const DOM_Node&			pos, 
			const DOM_Node&			from, 
			const DOM_Node&			target,
			int						countFrom) const
{
	DOM_Node posCopy(pos);

	int count = countFrom;

	if(posCopy != 0)
	{
		do
		{          
			if( (0 == countMatchPattern) || 
				(countMatchPattern->getMatchScore(posCopy,
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



int
ElemNumber::getSiblingNumber(
			StylesheetExecutionContext&		executionContext,
			const XPath*					countMatchPattern, 
			const DOM_Node&					target) const
{
	int number = 0;

	const DOM_Node	theParent = executionContext.getParentOfNode(target);
	assert(theParent != 0);

	// TODO: If target is an Attr, implement special handling. 
	DOM_NodeList	siblings = theParent.getChildNodes();

	if (siblings != 0)
	{
		const int	nNodes = siblings.getLength();

		for(int i = 0; i < nNodes; i++)
		{
			const DOM_Node	child = siblings.item(i);

			if(child == target)
			{
				number++; // always count the target
				break;
			}
			else if(0 == countMatchPattern || 
					countMatchPattern->getMatchScore(child,
													 executionContext.getXPathExecutionContext()) != XPath::s_MatchScoreNone)
			{
				number++;
			}
		}
	}

	return number;
}



int
ElemNumber::countMatchingAncestors(
			StylesheetExecutionContext&		executionContext,
			const XPath*					patterns,
			const DOM_Node&					node) const
{
	int			count = 0;

	DOM_Node	nodeCopy(node);

	while(nodeCopy != 0)
	{
		if(0 != patterns)
		{
			if(patterns->getMatchScore(nodeCopy,
									   executionContext.getXPathExecutionContext()) != XPath::s_MatchScoreNone)
			{
				count++;
			}  
		}
		else
		{
			count++;
		}

		nodeCopy = executionContext.getParentOfNode(nodeCopy);
	}

	return count;
}



ElemNumber::IntArrayType
ElemNumber::getAncestorNumbers(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern, 
			const DOM_Node&					node) const
{
	DOM_Node	nodeCopy(node);

	const int nMatchingAncestors =
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
				DOM_Node	target =
						findAncestor(executionContext,
									 fromMatchPattern,
									 countMatchPattern,
									 nodeCopy,
									 DOM_UnimplementedElement(const_cast<ElemNumber*>(this)));

				if(target == 0) 
					target = nodeCopy;

				counts[countIndex] = getSiblingNumber(executionContext, countMatchPattern, target);
				countIndex--;
			}

			nodeCopy = executionContext.getParentOfNode(nodeCopy);
		} // end while
	} // end if nMatchingAncestors > 0

	return counts;
}



std::locale
ElemNumber::getLocale(
			StylesheetExecutionContext&		/* executionContext */,
			const DOM_Node&					/* contextNode */) const
{
	//TODO
	return std::locale();
}



NumberFormat*
ElemNumber::getNumberFormatter(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					contextNode) const
{
	std::locale loc = getLocale(executionContext, contextNode);
    
    // Helper to format local specific numbers to strings.
	std::auto_ptr<NumberFormat>		formatter(new NumberFormat);

	DOM_UnimplementedElement	theNamespaceContext(const_cast<ElemNumber*>(this));

    DOMString digitGroupSepValue = (!isEmpty(m_groupingSeparator_avt))
                                  ?  executionContext.evaluateAttrVal(contextNode,
																	  theNamespaceContext,
																	  m_groupingSeparator_avt) :
										DOMString();

    DOMString nDigitsPerGroupValue = (!isEmpty(m_groupingSize_avt))
                                  ?  executionContext.evaluateAttrVal(contextNode,
																	  theNamespaceContext, 
																	  m_groupingSize_avt) :
										DOMString();

    // TODO: Handle digit-group attributes
    if(!isEmpty(digitGroupSepValue) || !isEmpty(nDigitsPerGroupValue))
    {
		formatter->setGroupingUsed(true);
		formatter->setGroupingSeparator(m_groupingSeparator_avt);
		formatter->setGroupingSize(m_groupingSize_avt);
    }
    
    return formatter.release();
}



DOMString
ElemNumber::formatNumberList(
			StylesheetExecutionContext&		executionContext,
			const IntArrayType&				theList, 
			const DOM_Node&					contextNode) const
{
	const int	nNumbers = theList.size();
	XMLCh		numberType = '1';
	int			numberWidth = 1;

	DOMString	formattedNumber;
	DOMString	formatToken;
	DOMString	sepString;
	DOMString	lastSepString;

	DOMString	formatValue = !isEmpty(m_format_avt)
		? executionContext.evaluateAttrVal(contextNode,
										   DOM_UnimplementedElement(const_cast<ElemNumber*>(this)),
										   m_format_avt)
		: DOMString();

	if(isEmpty(formatValue)) 
		formatValue = DOMString("1");
	
	NumeratorFormatter::NumberFormatStringTokenizer		formatTokenizer(formatValue);

	std::locale		loc = getLocale(executionContext, contextNode);

	for(int i = 0; i < nNumbers; i++)
	{
		while(formatTokenizer.hasMoreTokens())
		{
			formatToken = formatTokenizer.nextToken();

			if(isLetterOrDigit(charAt(formatToken, length(formatToken) - 1)))
			{
				numberWidth = length(formatToken);

				numberType = charAt(formatToken, numberWidth - 1);
				break; // from while(tokenizer.hasMoreTokens())
			}
			else
			{
				sepString = formatToken;

				formattedNumber += sepString;

				if(formatTokenizer.hasMoreTokens())
				{
					while(formatTokenizer.hasMoreTokens())
					{
						formatToken = formatTokenizer.nextToken();

						if(!isLetterOrDigit(charAt(formatToken, 0)))
						{
							lastSepString = sepString + formatToken; // possibly the last separator 				                			
						}
						else			
						{
							numberWidth = length(formatToken);
							numberType = charAt(formatToken, numberWidth - 1);
							break; // from inner while loop
						}
					}
					break; // from while(tokenizer.hasMoreTokens())
				}
			}

		} // end while

		formattedNumber += getFormattedNumber(executionContext, contextNode,
				numberType, numberWidth, theList[i]);
	}

	// Check to see if we finished up the format string...
	if(isEmpty(lastSepString))
		lastSepString = DOMString();

	while(formatTokenizer.hasMoreTokens())
	{
		formatToken = formatTokenizer.nextToken();

		if(!isLetterOrDigit(charAt(formatToken, 0)))
		{
			lastSepString += formatToken;
		}
		else
		{
			lastSepString = DOMString();
		}
	}

	if(!isEmpty(lastSepString))
	{
		formattedNumber += lastSepString;
	}

	return formattedNumber;  
}



DOMString
ElemNumber::getFormattedNumber(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					contextNode,
			XMLCh							numberType,
			int								numberWidth,
			int								listElement) const
{

	std::auto_ptr<NumberFormat> formatter(getNumberFormatter(executionContext, contextNode));

	DOMString	padString = formatter->format(0);
	DOMString	lookahead;

	DOMString	formattedNumber;

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
				const DOMString		numString =
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
DOMString
ElemNumber::int2alphaCount(
			int					val,
			const DOMString&	table)
{
	const int	radix = length(table);

	// Create a buffer to hold the result
	// TODO:  size of the table can be determined by computing
	// logs of the radix.  For now, we fake it.  
	const int	buflen = 100;

	std::vector<XMLCh>	buf(buflen + 1, 0);

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

	DOMString retStr(buf.begin() + charPos + 1, (buflen - charPos - 1));

	return retStr;
}



DOMString
ElemNumber::long2roman(
			long	val,
			bool	prefixesAreOK)
{
	if(val <= 0)
	{
		return DOMString( "#E(" + LongToDOMString(val) +")" );
	}

	DOMString	roman;

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
		roman = "#error";
	}

	return roman;
}
