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

#include "ElemPriv.hpp"
#include "NumeratorFormatter.hpp"

const DOMString ElemNumber::m_alphaCountTable = "ZABCDEFGHIJKLMNOPQRSTUVWXY"; 

const DecimalToRoman ElemNumber::m_romanConvertTable[] = 
{
	DecimalToRoman(1000, "M", 900, "CM"),        
	DecimalToRoman(500, "D", 400, "CD"),
	DecimalToRoman(100L, "C", 90L, "XC"),        
	DecimalToRoman(50L, "L", 40L, "XL"),
	DecimalToRoman(10L, "X", 9L, "IX"),        
	DecimalToRoman(5L, "V", 4L, "IV"),
	DecimalToRoman(1L, "I", 1L, "I")    
};


ElemNumber::ElemNumber(
	XSLTEngineImpl& processor,
	Stylesheet& stylesheetTree,
	const DOMString& name, 
	const AttributeList& atts,
	int lineNumber, 
	int	columnNumber) :
		ElemTemplateElement(processor, stylesheetTree, name, lineNumber, columnNumber),	
		m_pCountMatchPattern(0),
		m_pFromMatchPattern(0),
		m_pValueExpr(0),
		m_level(Constants::NUMBERLEVEL_SINGLE),
		m_format_avt(DOMString()),
		m_lang_avt(DOMString()),  
		m_lettervalue_avt(DOMString()),
		m_groupingSeparator_avt(DOMString()),
		m_groupingSize_avt(DOMString())
	
{
	const int nAttrs = atts.getLength();

	for(int i = 0; i < nAttrs; i++)
	{
		const DOMString aname(atts.getName(i));

		if(equals(aname,Constants::ATTRNAME_LEVEL))
		{
			const DOMString levelValue = atts.getValue(i);

			if(! isEmpty(levelValue))
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
		else if(equals(aname,Constants::ATTRNAME_COUNT))
		{
			m_pCountMatchPattern = processor.createMatchPattern(atts.getValue(i), *this);
		}
		else if(equals(aname,Constants::ATTRNAME_FROM))
		{
			m_pFromMatchPattern = processor.createMatchPattern(atts.getValue(i), *this);
		}
		else if(equals(aname,Constants::ATTRNAME_VALUE))
		{
			m_pValueExpr = getStylesheet().getProcessor()->createXPath(atts.getValue(i), *this);
		}
		else if(equals(aname,Constants::ATTRNAME_FORMAT))
		{
			m_format_avt = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_LANG))
		{
			m_lang_avt = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_LETTERVALUE))
		{
			processor.warn(Constants::ATTRNAME_LETTERVALUE + " not supported yet!");
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
		else if(!isAttrOK(aname, atts, i))
		{
			processor.error(name + " has an illegal attribute: " + aname);
		}
	}
}

	
int ElemNumber::getXSLToken() const 
{
	return Constants::ELEMNAME_NUMBER;
}
	

void ElemNumber::execute(
	XSLTEngineImpl& processor, 
	const DOM_Node& sourceTree, 
	const DOM_Node& sourceNode,
	const QName& mode)
{
	ElemTemplateElement::execute(processor, sourceTree, sourceNode, mode);

	DOMString countString = getCountString(processor, sourceTree, sourceNode);
	
	if (! isEmpty(countString))
	processor.characters(toCharArray(countString), 0, length(countString));
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
DOM_Node ElemNumber::findAncestor(
	XSLTEngineImpl&	processor, 
	XPath* fromMatchPattern, 
	XPath* countMatchPattern,
	const DOM_Node& context, 
	const DOM_Element& /*namespaceContext*/)
{
	DOM_Node contextCopy(context);

	while(0 != contextCopy)
	{
		if(0 != fromMatchPattern)
		{
			if(fromMatchPattern->getMatchScore(contextCopy) != XPath::s_MatchScoreNone)
			{
				contextCopy = DOM_Node();
				break;
			}
		}
		
		if(0 != countMatchPattern)
		{
			if(countMatchPattern->getMatchScore(context) != XPath::s_MatchScoreNone)
			{
				break;
			}
		}
		
		contextCopy = processor.getXPathSupport().getParentOfNode(contextCopy);
	}

	return contextCopy;
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
DOM_Node ElemNumber::findPrecedingOrAncestorOrSelf(
	XSLTEngineImpl&	processor, 
	XPath* fromMatchPattern, 
	XPath* countMatchPattern,
	const DOM_Node& context, 
	const DOM_Element& /*namespaceContext*/)
{  
	DOM_Node contextCopy(context);
    
	while(0 != contextCopy)
	{
		if(0 != fromMatchPattern)
		{
			if(fromMatchPattern->getMatchScore(contextCopy) != XPath::s_MatchScoreNone)
			{
				contextCopy = DOM_Node();
				break;
			}
		}
		
		if(0 != countMatchPattern)
		{
			if(countMatchPattern->getMatchScore(contextCopy) != XPath::s_MatchScoreNone)
			{
				break;
			}
		}
		
		DOM_Node prevSibling = contextCopy.getPreviousSibling();
		if(0 == prevSibling)
		{
			contextCopy = processor.getXPathSupport().getParentOfNode(contextCopy);
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
XPath* ElemNumber::getCountMatchPattern(
	XSLTEngineImpl&	processor, 
	const DOM_Node& contextNode)
{
	XPath* countMatchPattern = m_pCountMatchPattern;

	if(0 == countMatchPattern)
	{
		switch( contextNode.getNodeType())
		{
		case DOM_Node::ELEMENT_NODE:
			countMatchPattern = processor.createMatchPattern(contextNode.getNodeName(),
				*this);
			break;
		case DOM_Node::ATTRIBUTE_NODE:
			countMatchPattern = processor.createMatchPattern(DOMString("@") + contextNode.getNodeName(),
				*this);
			break;
		case DOM_Node::CDATA_SECTION_NODE:
		case DOM_Node::TEXT_NODE:
			countMatchPattern = processor.createMatchPattern(DOMString("text()"), 
				*this);
			break;
		case DOM_Node::COMMENT_NODE:
			countMatchPattern = processor.createMatchPattern(DOMString("comment()"), 
				*this);
			break;
		case DOM_Node::DOCUMENT_NODE:
			countMatchPattern = processor.createMatchPattern(DOMString("/"), 
				*this);
			break;
		case DOM_Node::PROCESSING_INSTRUCTION_NODE:
			countMatchPattern = processor.createMatchPattern(DOMString("pi(") + 
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


/**
 * Given an XML source node, get the count according to the 
 * parameters set up by the xsl:number attributes.
 */
DOMString ElemNumber::getCountString(
	XSLTEngineImpl& processor, 
	const DOM_Node& /*sourceTree*/, 
	const DOM_Node& sourceNode)
{
	IntArrayType list;
	
	if(0 != m_pValueExpr)
	{
		XObject* countObj = m_pValueExpr->execute(sourceNode, *this, processor.getContextNodeList());

		int count = static_cast<int>(countObj->num());

		list = IntArrayType(1);
		list[0] = count;
	}
	else
	{      
		XPath* countMatchPattern = getCountMatchPattern(processor, sourceNode);
		
		if((Constants::NUMBERLEVEL_ANY == m_level) || 
			(Constants::NUMBERLEVEL_SINGLE == m_level))
		{
			list = IntArrayType(1);

			if(Constants::NUMBERLEVEL_SINGLE == m_level)
			{
				DOM_Node target = findAncestor(processor, m_pFromMatchPattern, 
					countMatchPattern, sourceNode, DOM_UnimplementedElement(this));

				if(0 == target) 
					target = processor.getXPathSupport().getParentOfNode(sourceNode);

				if(0 != target)
				{
					list[0] = getSiblingNumber(processor, countMatchPattern, target);
				}
				else
				{
					processor.warn(DOM_Node(), sourceNode,
						DOMString("Warning: count attribute does not match an ancestor in xsl:number! Target = ") 
							+ sourceNode.getNodeName());
				}
			}
			else // if NUMBERLEVEL_ANY
			{
				DOM_Node from;
				if(0 != m_pFromMatchPattern)
				{
					from= findPrecedingOrAncestorOrSelf(processor, 0, m_pFromMatchPattern, 
						sourceNode, DOM_UnimplementedElement(this));

					if(0 == from)
					{
						from = sourceNode;
					}
				}
				else
				{
					from = sourceNode.getOwnerDocument();
				}

				DOM_Node fromPos = (from != sourceNode) ? getNextInTree(from, from) : from;

				list[0] = getNumberInTree(countMatchPattern, fromPos, from, sourceNode, 0);
			}
		}
		else // if NUMBERLEVEL_MULTI
		{
			list = getAncestorNumbers(processor, m_pFromMatchPattern,
				countMatchPattern, sourceNode);
		}
	}
	return (list.size()>0) ? formatNumberList(processor, list, sourceNode) : DOMString();
}

/**
 * from any position in the tree, return the 
 * next node in the tree, assuming preorder 
 * traversal preceded, or null if at the end.
 */
DOM_Node ElemNumber::getNextInTree(const DOM_Node& pos, const DOM_Node& from)
{
	DOM_Node posCopy(pos);

	DOM_Node nextNode(posCopy.getFirstChild());

	while(0 == nextNode)
	{
		nextNode = posCopy.getNextSibling();
		if(0 == nextNode)
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


/**
 * Get a number that represents a node based on the
 * position of that node among within the document tree,  
 * and taking into account the count and from patterns as 
 * specified in the XSL specification.
 * @param fromMatchPattern if non-null, where to 
 * start counting from.
 * @param countMatchPattern if non-null, count only nodes 
 * that match this pattern.
 * @param target count this node and preceding nodes.
 * @return A number that counts target and preceding 
 * nodes that qualify.
 */
int ElemNumber::getNumberInTree(	
	XPath* countMatchPattern, 
	const DOM_Node& pos, 
	const DOM_Node&	from, 
	const DOM_Node&	target,
	int countFrom)
{
	DOM_Node posCopy(pos);

	int count = countFrom;
	if(0 != posCopy)
	{
		do
		{          
			if( (0 == countMatchPattern) || 
				(countMatchPattern->getMatchScore(posCopy) != XPath::s_MatchScoreNone))
			{
				count++;
			}
		}

		while((posCopy != target) && 
			(0 != (posCopy = getNextInTree(posCopy, from))));
	}
	
	return count;
}


/**
 * Get a number that represents a node based on the
 * position of that node among it's siblings, and 
 * taking into account the count and from patterns.
 * @param fromMatchPattern if non-null, where to 
 * start counting from.
 * @param countMatchPattern if non-null, count only nodes 
 * that match this pattern.
 * @param target count this node and preceding siblings.
 * @return A number that counts target and preceding 
 * siblings that qualify.
 */
int ElemNumber::getSiblingNumber(
	XSLTEngineImpl&	processor, 
	XPath* countMatchPattern, 
	const DOM_Node& target)
{
	// TODO: If target is an Attr, implement special handling. 
	DOM_NodeList siblings = processor.getXPathSupport().
		getParentOfNode(target).getChildNodes();

	int nNodes = siblings.getLength();
	int number = 0; // return val

	for(int i = 0; i < nNodes; i++)
	{
		DOM_Node child = siblings.item(i);
		if(child == target)
		{
			number++; // always count the target
			break;
		}
		else if((0 == countMatchPattern) || 
			countMatchPattern->getMatchScore(child) != 	XPath::s_MatchScoreNone)
		{
			number++;
		}
	}
	
	return number;
}


/**
 * Count the ancestors, up to the root, that match the 
 * pattern.
 * @param patterns if non-null, count only nodes 
 * that match this pattern, if null count all ancestors.
 * @param node Count this node and it's ancestors.
 * @return The number of ancestors that match the pattern.
 */
int ElemNumber::countMatchingAncestors(
	XSLTEngineImpl& processor, 
	XPath* patterns, 
	const DOM_Node& node)
{
	int count = 0; // return val
	
	DOM_Node nodeCopy(node);

	while( 0 != nodeCopy )
	{
		if(0 != patterns)
		{
			if(patterns->getMatchScore(nodeCopy) != XPath::s_MatchScoreNone)
			{
				count++;
			}  
		}
		else
		{
			count++;
		}
		nodeCopy = processor.getXPathSupport().getParentOfNode(nodeCopy);
	}

	return count;
}


/**
 * Climb up the ancestor tree, collecting sibling position 
 * numbers (as modified by the fromMatchPattern and 
 * countMatchPattern patterns).
 * @param fromMatchPattern if non-null, where to 
 * start counting from in each sibling list.
 * @param countMatchPattern if non-null, count only ancestors 
 * and siblings that match this pattern.
 * @param node count this node and ancestors that match the countMatchPattern.
 * @return An array of ints of length that matches exactly the number 
 * of ancestors that match countMatchPattern.
 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
 * the error condition is severe enough to halt processing.
 */
ElemNumber::IntArrayType ElemNumber::getAncestorNumbers(
	XSLTEngineImpl& processor,
	XPath* fromMatchPattern,
	XPath* countMatchPattern, 
	const DOM_Node& node)
{
	DOM_Node nodeCopy(node);

	int nMatchingAncestors = 
		countMatchingAncestors(	processor, 
		countMatchPattern, 
		nodeCopy);

	IntArrayType counts;

	if(nMatchingAncestors > 0)
	{
		counts = IntArrayType(nMatchingAncestors);
		int countIndex = counts.size() - 1; // position to put count into
		while( 0 != nodeCopy )
		{
			bool countIt = false;
			if(0 != countMatchPattern)
			{
				if(countMatchPattern->getMatchScore(nodeCopy) != XPath::s_MatchScoreNone)
				{
					countIt = true;
				} 
			}
			else
			{
				countIt = true;
			}
			if(countIt)
			{
				DOM_Node target = findAncestor(processor, 
					fromMatchPattern, 
					countMatchPattern, 
					nodeCopy, 
					DOM_UnimplementedElement(this));

				if(0 == target) 
					target = nodeCopy;
				
				counts[countIndex] = getSiblingNumber(processor, countMatchPattern, target);
				countIndex--;
			}
			nodeCopy = processor.getXPathSupport().getParentOfNode(nodeCopy);
		} // end while
	} // end if nMatchingAncestors > 0
	return counts;
}


/**
 * Get the locale we should be using.
 */
std::locale ElemNumber::getLocale(XSLTEngineImpl& /*processor*/, const DOM_Node& /*contextNode*/)
{
	//TODO
	return std::locale();
}


NumberFormat* ElemNumber::getNumberFormatter(
	XSLTEngineImpl&	processor, 
	const DOM_Node&	contextNode)
{
	std::locale loc = getLocale(processor, contextNode);
    
    // Helper to format local specific numbers to strings.
    NumberFormat* formatter = new NumberFormat();
    
    DOMString digitGroupSepValue = (!isEmpty(m_groupingSeparator_avt))
                                  ?  processor.evaluateAttrVal(contextNode, DOM_UnimplementedElement(this), 
									m_groupingSeparator_avt) : DOMString();
    
    DOMString nDigitsPerGroupValue = (!isEmpty(m_groupingSize_avt))
                                  ?  processor.evaluateAttrVal(contextNode, DOM_UnimplementedElement(this), 
									m_groupingSize_avt) : DOMString();

    // TODO: Handle digit-group attributes
    if( !isEmpty(digitGroupSepValue) || !isEmpty(nDigitsPerGroupValue) )
    {
		formatter->setGroupingUsed(true);
		formatter->setGroupingSeparator(m_groupingSeparator_avt);
		formatter->setGroupingSize(m_groupingSize_avt);
    }
    
    return formatter;
}


/**
 * Format a vector of numbers into a formatted string.
 * @param xslNumberElement Element that takes %conversion-atts; attributes.
 * @param list Array of one or more integer numbers.
 * @return String that represents list according to 
 * %conversion-atts; attributes.
 * TODO: Optimize formatNumberList so that it caches the last count and
 * reuses that info for the next count.
 */
DOMString ElemNumber::formatNumberList(
	XSLTEngineImpl& processor, 
	IntArrayType theList, 
	const DOM_Node& contextNode)
{
	DOMString formattedNumber;
	int nNumbers = theList.size();
	XMLCh numberType = '1';
	int numberWidth = 1;
	DOMString formatToken;
	DOMString sepString;
	DOMString lastSepString;

	DOMString formatValue = (!isEmpty(m_format_avt)) 
		? processor.evaluateAttrVal(contextNode, DOM_UnimplementedElement(this), m_format_avt) 
		: DOMString();

	if(isEmpty(formatValue)) 
		formatValue = DOMString("1");
	
	NumeratorFormatter::NumberFormatStringTokenizer formatTokenizer(formatValue);

	std::locale loc = getLocale(processor, contextNode);

	for(int i = 0; i < nNumbers; i++)
	{
		while(formatTokenizer.hasMoreTokens())
		{
			formatToken = formatTokenizer.nextToken();
			if(isLetterOrDigit(charAt(formatToken, formatToken.length()-1)))
			{
				numberWidth = formatToken.length();
				numberType = charAt(formatToken, numberWidth-1);
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
							numberWidth = formatToken.length();
							numberType = charAt(formatToken, numberWidth-1);
							break; // from inner while loop
						}

					}
					break; // from while(tokenizer.hasMoreTokens())
				}
			}

		} // end while

		formattedNumber += getFormattedNumber(processor, contextNode,
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
	if(! isEmpty(lastSepString))
	{
		formattedNumber += lastSepString;
	}
	return formattedNumber;  
}

DOMString ElemNumber::getFormattedNumber(
		XSLTEngineImpl& processor, const DOM_Node& contextNode,
		XMLCh numberType, int numberWidth, int listElement)
{

	std::auto_ptr<NumberFormat> formatter(getNumberFormatter(processor, contextNode));

	DOMString padString = formatter->format(0);
	DOMString lookahead; // next token
	
	DOMString formattedNumber;
	switch(numberType)
	{
		case 'A':
			formattedNumber += int2alphaCount(listElement, m_alphaCountTable);
			break;
		case 'a':
			formattedNumber += toLowerCase(int2alphaCount(listElement, m_alphaCountTable));
			break;
		case 'I':
			formattedNumber += long2roman(listElement, true);
			break;
		case 'i':
			formattedNumber += toLowerCase(long2roman(listElement, true));
			break;
		default: // "1"
			DOMString numString = formatter->format(listElement);
			int nPadding = numberWidth - numString.length();
			for(int k = 0; k < nPadding; k++)
			{
				formattedNumber += padString;
			}
			formattedNumber += numString;
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
DOMString ElemNumber::int2alphaCount(int val, const DOMString& table)
{
	int radix = table.length();
	
	// Create a buffer to hold the result
	// TODO:  size of the table can be determined by computing
	// logs of the radix.  For now, we fake it.  
	const int buflen = 100;
	XMLCh buf[buflen+1];
	memset(buf,'\0',sizeof(XMLCh)*(buflen+1));
	
	// next character to set in the buffer
	int charPos = buflen-1 ;    // work backward through buf[]
	
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
		lookupIndex  = (val+correction) % radix;  
		
		// shift input by one "column"
		val = (val / radix);
		
		// if the next value we'd put out would be a leading zero, we're done.
		if (lookupIndex == 0 && val == 0)
			break;
		
		// put out the next character of output
		buf[charPos--] = static_cast<char>(table.charAt(lookupIndex));
	}
	while (val > 0);

	DOMString retStr(buf+ charPos+1, (buflen - charPos -1));

	return retStr;
}


/**
 * Convert a long integer into roman numerals.
 * @param val Value to convert.
 * @param prefixesAreOK true_ to enable prefix notation (e.g. 4 = "IV"),
 * false_ to disable prefix notation (e.g. 4 = "IIII").
 * @return Roman numeral string.
 * @see DecimalToRoman
 * @see m_romanConvertTable
 */
DOMString ElemNumber::long2roman(long val, bool prefixesAreOK)
{
	if(val <= 0)
	{
		return DOMString( "#E(" + LongToDOMString(val) +")" );
	}
	
	DOMString roman;
	int place = 0;

	if (val <= 3999L)
	{
		do      
		{
			while (val >= m_romanConvertTable[place].m_postValue)            
			{
				roman += m_romanConvertTable[place].m_postLetter;
				val -= m_romanConvertTable[place].m_postValue;
			}
			if (prefixesAreOK)            
			{
				if (val >= m_romanConvertTable[place].m_preValue)                  
				{
					roman += m_romanConvertTable[place].m_preLetter;
					val -= m_romanConvertTable[place].m_preValue;
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


