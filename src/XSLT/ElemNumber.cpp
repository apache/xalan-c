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

XalanDOMString	s_elalphaCountTable;



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
	m_format_avt(0),
	m_lang_avt(0),
	m_lettervalue_avt(0),
	m_groupingSeparator_avt(0),
	m_groupingSize_avt(0)
	
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
			m_format_avt = new AVT(aname, atts.getType(i),
						atts.getValue(i), *this, constructionContext);

		}
		else if(equals(aname, Constants::ATTRNAME_LANG))
		{
			m_lang_avt = new AVT(aname, atts.getType(i),
						atts.getValue(i), *this, constructionContext);
		}
		else if(equals(aname, Constants::ATTRNAME_LETTERVALUE))
		{
			if (equals("traditional", atts.getValue(i)))
				constructionContext.warn(Constants::ATTRNAME_LETTERVALUE +
						" value 'traditional' not supported yet!");
			m_lettervalue_avt = new AVT(aname, atts.getType(i),
						atts.getValue(i), *this, constructionContext);
		} 
		else if(equals(aname,Constants::ATTRNAME_GROUPINGSEPARATOR))
		{
			m_groupingSeparator_avt = new AVT(aname, atts.getType(i),
						atts.getValue(i), *this, constructionContext);
		}
		else if(equals(aname,Constants::ATTRNAME_GROUPINGSIZE))
		{
			m_groupingSize_avt = new AVT(aname, atts.getType(i),
						atts.getValue(i), *this, constructionContext);
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(name + " has an illegal attribute: " + aname);
		}
	}
}

	
ElemNumber::~ElemNumber()
{
	delete	m_format_avt;
	delete	m_lang_avt;
	delete	m_lettervalue_avt;
	delete	m_groupingSeparator_avt;
	delete	m_groupingSize_avt;
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
			if(fromMatchPattern->getMatchScore(contextCopy, *this,
						executionContext) !=
							XPath::s_MatchScoreNone)
			{
				break;
			}
		}
		
		if(0 != countMatchPattern)
		{
			if(countMatchPattern->getMatchScore(contextCopy, *this,
						executionContext) !=
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
			if(fromMatchPattern->getMatchScore(contextCopy, *this,
						executionContext) !=
							XPath::s_MatchScoreNone)
			{
				contextCopy = 0;
				break;
			}
		}
		
		if(0 != countMatchPattern)
		{
			if(countMatchPattern->getMatchScore(contextCopy, *this,
						executionContext) !=
							XPath::s_MatchScoreNone)
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
			// Now go down the chain of children of this sibling 
			contextCopy = prevSibling->getLastChild();
			if (contextCopy == 0)
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
			countMatchPattern =
				executionContext.createMatchPattern(contextNode->getNodeName(), *this);
			break;

		case XalanNode::ATTRIBUTE_NODE:
			countMatchPattern = executionContext.createMatchPattern(
						XalanDOMString(XALAN_STATIC_UCODE_STRING("@")) + contextNode->getNodeName(),
						*this);
			break;

		case XalanNode::CDATA_SECTION_NODE:
		case XalanNode::TEXT_NODE:
			countMatchPattern = executionContext.createMatchPattern(
					XalanDOMString(XALAN_STATIC_UCODE_STRING("text()")), *this);
			break;

		case XalanNode::COMMENT_NODE:
			countMatchPattern = executionContext.createMatchPattern(
					XalanDOMString(XALAN_STATIC_UCODE_STRING("comment()")), *this);
			break;

		case XalanNode::DOCUMENT_NODE:
			countMatchPattern = executionContext.createMatchPattern(
					XalanDOMString(XALAN_STATIC_UCODE_STRING("/")), *this);
			break;

		case XalanNode::PROCESSING_INSTRUCTION_NODE:
			countMatchPattern = executionContext.createMatchPattern(
				XalanDOMString(XALAN_STATIC_UCODE_STRING("pi(")) + 
				contextNode->getNodeName() +
				XalanDOMString(XALAN_STATIC_UCODE_STRING(")")), *this);
			break;

		default:
			break;
		}
	}
	return countMatchPattern;
}



XalanDOMString
ElemNumber::getCountString(
			StylesheetExecutionContext&		executionContext,
			XalanNode*							/* sourceTree */, 
			XalanNode*							sourceNode) const
{
	assert(sourceNode != 0);
	IntArrayType	numberList;
	CountersTable ctable;

	if(0 != m_valueExpr)
	{
		const XObject* const	countObj =
			m_valueExpr->execute(sourceNode,
								 *this,
								 executionContext);

		numberList.push_back(static_cast<int>(countObj->num()));
	}
	else
	{
		if(Constants::NUMBERLEVEL_ANY == m_level)
		{
			numberList.push_back(ctable.countNode(
						executionContext, 
						this,
						sourceNode));
		}
		else
		{
			MutableNodeRefList ancestors = getMatchingAncestors(executionContext, sourceNode,
				Constants::NUMBERLEVEL_SINGLE == m_level);

			const unsigned int	lastIndex = ancestors.getLength();

			if(lastIndex > 0)
			{
				for(unsigned int i = 0; i < lastIndex; i++)
				{
					XalanNode* const target = ancestors.item(lastIndex - i -1);
					numberList.push_back(ctable.countNode(
						executionContext,
						this,
						target));
				}
			}
		}
	}

	return numberList.size() > 0 ? formatNumberList(executionContext, numberList, sourceNode) : XalanDOMString();
}

XalanNode*
ElemNumber::getPreviousNode(
		StylesheetExecutionContext&		executionContext,
		XalanNode* pos) const
{    
	const XPath* countMatchPattern = getCountMatchPattern(executionContext, pos);
	if(Constants::NUMBERLEVEL_ANY == m_level)
	{
		const XPath* fromMatchPattern = m_fromMatchPattern;

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
				if((0 != next) && ((((0 != fromMatchPattern) &&
						(fromMatchPattern->getMatchScore(next, *this,
						executionContext) !=
									 XPath::s_MatchScoreNone))) || 
							(next->getNodeType() == XalanNode::DOCUMENT_NODE)))
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

			if((0 != pos) && ((0 == countMatchPattern) ||
						(countMatchPattern->getMatchScore(pos, *this,
						executionContext) !=
						 XPath::s_MatchScoreNone)))
			{
				break;
			}
		}
	}
	else // NUMBERLEVEL_MULTI or NUMBERLEVEL_SINGLE
	{
		while(0 != pos)
		{            
			pos = pos->getPreviousSibling();
			if((0 != pos) && ((0 == countMatchPattern) ||
						(countMatchPattern->getMatchScore(pos, *this,
						executionContext) !=
						 XPath::s_MatchScoreNone)))
			{
				break;
			}
		}
	}
	return pos;
}


XalanNode*
ElemNumber::getTargetNode(
		StylesheetExecutionContext&		executionContext,
		XalanNode*						sourceNode) const
{
	XalanNode* target = 0;
	const XPath* countMatchPattern =
		getCountMatchPattern(executionContext, sourceNode);
	if(Constants::NUMBERLEVEL_ANY == m_level)
	{
		target = findPrecedingOrAncestorOrSelf(executionContext,
				m_fromMatchPattern, countMatchPattern, sourceNode, this);
	}
	else
	{
		target = findAncestor(executionContext, m_fromMatchPattern,
				countMatchPattern, sourceNode, this);
	}
	return target;
}


/**
 * Get the ancestors, up to the root, that match the
 * pattern.
 * @param patterns if non-0, count only nodes
 * that match this pattern, if 0 count all ancestors.
 * @param node Count this node and it's ancestors.
 * @return The number of ancestors that match the pattern.
 */
MutableNodeRefList
ElemNumber::getMatchingAncestors(
		StylesheetExecutionContext&		executionContext,
		XalanNode* node, 
		bool stopAtFirstFound) const
{
	MutableNodeRefList ancestors;
	const XPath* countMatchPattern = getCountMatchPattern(executionContext, node);
	while( 0 != node )
	{
		if((0 != m_fromMatchPattern) &&
				(m_fromMatchPattern->getMatchScore(node, *this, executionContext) !=
				 XPath::s_MatchScoreNone))
		{ 
			// The following if statement gives level="single" different 
			// behavior from level="multiple", which seems incorrect according 
			// to the XSLT spec.  For now we are leaving this in to replicate 
			// the same behavior in XT, but, for all intents and purposes we 
			// think this is a bug, or there is something about level="single" 
			// that we still don't understand.
			if(!stopAtFirstFound)
				break;
		}  

		if(0 == countMatchPattern)
			error(XalanDOMString("Programmers error! countMatchPattern should never be 0!"));

		if(countMatchPattern->getMatchScore(node, *this, executionContext) !=
				XPath::s_MatchScoreNone)
		{
			ancestors.addNode(node);
			if(stopAtFirstFound)
				break;
		}
		node = executionContext.getParentOfNode(*node);
	}
	return ancestors;
} // end getMatchingAncestors method


#if !defined(XALAN_NO_LOCALES)

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

	XalanDOMString	digitGroupSepValue;
	if (0 != m_groupingSeparator_avt)
		 m_groupingSeparator_avt->evaluate(digitGroupSepValue, contextNode,
				 *this, executionContext);
									 
	XalanDOMString	nDigitsPerGroupValue;
	if (0 != m_groupingSize_avt)
		m_groupingSize_avt->evaluate(nDigitsPerGroupValue, contextNode, *this,
				executionContext);

    // TODO: Handle digit-group attributes
	// 7.7.1 If one is empty, it is ignored	(numb81 conf test)
	if(!isEmpty(digitGroupSepValue) && !isEmpty(nDigitsPerGroupValue))
	{
		formatter->setGroupingUsed(true);
		formatter->setGroupingSeparator(digitGroupSepValue);
		formatter->setGroupingSize(nDigitsPerGroupValue);
	}

	return formatter.release();
}


//@@ JMD: this is different from the java version, but seems to work, so I'll
//leave it alone
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

	// Pathological cases
	if (nNumbers == 0) return formattedNumber;
	if (contextNode == 0) return formattedNumber;

	XalanDOMString	formatValue;
	if (m_format_avt != 0)
		 m_format_avt->evaluate(formatValue, contextNode, *this, executionContext);

	if(isEmpty(formatValue)) 
		formatValue = XALAN_STATIC_UCODE_STRING("1");

	NumberFormatStringTokenizer		formatTokenizer(formatValue);

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
	it = tokenVector.end()-1;
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

	std::auto_ptr<NumberFormat> formatter(
			getNumberFormatter(executionContext, contextNode));

	XalanDOMString	padString = formatter->format(0);
	XalanDOMString letterVal;
	if (m_lettervalue_avt != 0)
		m_lettervalue_avt->evaluate(letterVal, contextNode, *this,
				executionContext);

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
			executionContext.error(LongToDOMString(numberType) + " format not supported yet!");
			break;
		// Handle the special case of Greek letters for now
		case 0x03B1:
			if (isEmpty(s_elalphaCountTable))
			{
				// Start at lower case Greek letters in entity reference table in
				// FormatterToHTML
				s_elalphaCountTable += 962;
				for (unsigned short i = 1, j=938; i < 25; i++, j++)
					s_elalphaCountTable += j;
			}
			formattedNumber += int2alphaCount(listElement, s_elalphaCountTable);
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

XalanDOMString ElemNumber::int2singlealphaCount(int val, 
		const XalanDOMString&	table)
{
	const int		radix = length(table);

	// TODO:  throw error on out of range input
	if (val > radix)
		return XalanDOMString(XALAN_STATIC_UCODE_STRING("#E(") +
				LongToDOMString(val) +
				XALAN_STATIC_UCODE_STRING(")"));
	else
		return XalanDOMString(charAt(table, val-1));

}

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
	memset(buf, 0, (buflen + 1) * sizeof(XalanDOMChar));

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
		buf[charPos--] = charAt(table, lookupIndex);
	}
	while (val > 0);

	const XalanDOMString	retStr(buf + charPos + 1, (buflen - charPos - 1));

	return retStr;
}

XalanDOMString ElemNumber::tradAlphaCount(int	/* val */)
{
//	@@ JMD: We don't do languages yet, so this is just a placeholder
	assert(0);
	return XalanDOMString();	// To keep compiler happy
}


XalanDOMString
ElemNumber::long2roman(
			long	val,
			bool	prefixesAreOK)
{
	if(val < 0)
	{
		return XalanDOMString(XALAN_STATIC_UCODE_STRING("#E(") +
								LongToDOMString(val) +
								XALAN_STATIC_UCODE_STRING(")"));
	}
	// Make this match the conformance test
	else if(val == 0)
	{
		return XalanDOMString("0");
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


/*
 *				NumberFormatStringTokenizer Class Implementation
 */

ElemNumber::NumberFormatStringTokenizer::NumberFormatStringTokenizer(
			const XalanDOMString&	theStr) :
	m_currentPosition(0),
	m_maxPosition(length(theStr)),
	m_str(theStr)
{
}



/*
@@ Obsolete ??
void
ElemNumber::NumberFormatStringTokenizer::setString(const XalanDOMString&	theString)
{
	m_str = theString;

	m_currentPosition = 0;
	m_maxPosition = length(theString);
}

*/

// @@ JMD: This seemed to be working OK in previous version and is
// functionally equivalent to java, so I left it alone.  Other java methods do
// not seem to be needed in this implementation

XalanDOMString
ElemNumber::NumberFormatStringTokenizer::nextToken() 
{
	if (m_currentPosition >= m_maxPosition) 
	{
		// $$$ Todo: Implement!
//				throw new NoSuchElementException();
	}

	const int	start = m_currentPosition;

	if (isLetterOrDigit(charAt(m_str, m_currentPosition)))
	{
		while ((m_currentPosition < m_maxPosition) &&
				isLetterOrDigit(charAt(m_str, m_currentPosition))) 
			m_currentPosition++;
	}
	else
	{
		while ((m_currentPosition < m_maxPosition) &&
				!isLetterOrDigit(charAt(m_str, m_currentPosition))) 
			m_currentPosition++;
	}

	// @@ This wasn't working right when start=current=0 with DOMStrings
	// need to check it with XalanDOMString's
	return substring(m_str, start, m_currentPosition);
}



int
ElemNumber::NumberFormatStringTokenizer::countTokens() const
{
	int 	count = 0;
	int 	currpos = m_currentPosition;

	// Tokens consist of sequences of alphabetic characters and sequences of
	// non-alphabetic characters
	while (currpos < m_maxPosition) 
	{
		if (isLetterOrDigit(charAt(m_str, currpos)))
		{
			while ((currpos < m_maxPosition) &&
					isLetterOrDigit(charAt(m_str, currpos))) 
				currpos++;
		}
		else
		{
			while ((currpos < m_maxPosition) &&
					!isLetterOrDigit(charAt(m_str, currpos))) 
				currpos++;
		}
		count++;
	}
	return count;
}



ElemNumber::CounterVectorType& ElemNumber::CountersTable::getCounters(const ElemNumber* numberElem)
{
	return m_counterMap[numberElem];
}



void
ElemNumber::CountersTable::appendBtoFList(MutableNodeRefList& flist, MutableNodeRefList& blist)
{
	const int n = blist.getLength();

	for(int i = (n-1); i >= 0; i--)
	{
		flist.addNode(blist.item(i));
	}
}



int
ElemNumber::CountersTable::countNode(
			StylesheetExecutionContext&		support,
			const ElemNumber*				numberElem,
			XalanNode*						node)
{
	int		count = 0;

	CounterVectorType&	counters = getCounters(numberElem);

	const CounterVectorType::size_type	nCounters = counters.size();

	XalanNode* 	target = numberElem->getTargetNode(support, node);

	if(0 != target)
	{
		for(CounterVectorType::size_type i = 0; i < nCounters; i++)
		{    
			const Counter&	counter = counters[i];

			count = counter.getPreviouslyCounted(support, target);
			if(count > 0)
				return count;
		}

		// In the loop below, we collect the nodes in backwards doc order, so 
		// we don't have to do inserts, but then we store the nodes in forwards 
		// document order, so we don't have to insert nodes into that list, 
		// so that's what the appendBtoFList stuff is all about.  In cases 
		// of forward counting by one, this will mean a single node copy from 
		// the backwards list (m_newFound) to the forwards list
		// (counter.m_countNodes).
		count = 0;
		for(; 0 != target; target = numberElem->getPreviousNode(support, target))
		{   
			// First time in, we should not have to check for previous counts, 
			// since the original target node was already checked in the 
			// block above.
			if(0 != count)  
			{
				for(CounterVectorType::size_type i = 0; i < nCounters; i++)
				{
					Counter&	counter = counters[i];

					const unsigned int	cacheLen = counter.m_countNodes.getLength();

					if((cacheLen > 0) &&
							(counter.m_countNodes.item(cacheLen-1) == target))
					{
						count += cacheLen + counter.m_countNodesStartCount;
						if(cacheLen > 0)
							appendBtoFList(counter.m_countNodes, m_newFound);
						m_newFound.clear();
						return count;
					}
				}
			}
			m_newFound.addNode(target);
			count++;
		}
		// If we got to this point, then we didn't find a counter, so make 
		// one and add it to the list.
		ElemNumber::Counter counter(numberElem);
		m_countersMade++; // for diagnostics
		appendBtoFList(counter.m_countNodes, m_newFound);
		m_newFound.clear();
		counters.push_back(counter);
	}

	return count;
}



int
ElemNumber::Counter::getPreviouslyCounted(
		StylesheetExecutionContext&		support,
		const XalanNode*				node) const
{
	const int n = m_countNodes.getLength();
	int result = 0;

	for(int i = n-1;i >= 0; i--)
	{
		const XalanNode* countedNode = m_countNodes.item(i);
		if(node == countedNode)
		{
			// Since the list is in backwards order, the count is 
			// how many are in the rest of the list.
			result = i + 1 + m_countNodesStartCount;
			break;
		}

		// Try to see if the given node falls after the counted node...
		// if it does, don't keep searching backwards.
		if(support.isNodeAfter(*countedNode, *node))
			break;
	}

	return result;
}

XalanNode* ElemNumber::Counter::getLast()
{
	const int size = m_countNodes.getLength();
	return (size > 0) ? m_countNodes.item(size-1) : 0;
}
