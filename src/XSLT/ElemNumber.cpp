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
#include <PlatformSupport/DoubleSupport.hpp>
#include <PlatformSupport/XalanNumberFormat.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



#include <Include/XalanAutoPtr.hpp>



#include <XPath/XPath.hpp>
#include <XPath/XObjectFactory.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



#if !defined(XALAN_NO_NAMESPACES)
using std::vector;
#endif



ElemNumber::ElemNumber(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMChar*				name,
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
				error("Bad value on level attribute." + XalanDOMString(levelValue));
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
			constructionContext.error(XalanDOMString(name) + " has an illegal attribute: " + aname);
		}
	}
}

	
ElemNumber::~ElemNumber()
{
#if defined(XALAN_CANNOT_DELETE_CONST)
	delete	(AVT*)m_format_avt;
	delete	(AVT*)m_lang_avt;
	delete	(AVT*)m_lettervalue_avt;
	delete	(AVT*)m_groupingSeparator_avt;
	delete	(AVT*)m_groupingSize_avt;
#else
	delete	m_format_avt;
	delete	m_lang_avt;
	delete	m_lettervalue_avt;
	delete	m_groupingSeparator_avt;
	delete	m_groupingSize_avt;
#endif
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
	const XPath*	countMatchPattern = 0;

	switch(contextNode->getNodeType())
	{
	case XalanNode::ELEMENT_NODE:
		countMatchPattern =
				executionContext.createMatchPattern(contextNode->getNodeName(), *this);
		break;

	case XalanNode::ATTRIBUTE_NODE:
		countMatchPattern = executionContext.createMatchPattern(
						s_atString + contextNode->getNodeName(),
						*this);
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
		countMatchPattern = executionContext.createMatchPattern(
				s_piString + 
				contextNode->getNodeName() +
				s_leftParenString, *this);
		break;

	default:
		break;
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

	if(0 != m_valueExpr)
	{
		const XObjectPtr	countObj(m_valueExpr->execute(sourceNode, *this, executionContext));
		assert(countObj.null() == false);

		const double	theValue = countObj->num();

		if (DoubleSupport::isNaN(theValue) == true)
		{
			numberList.push_back(0);
		}
		else
		{
			numberList.push_back(int(theValue));
		}
	}
	else
	{
		CountersTable	ctable;

		if(Constants::NUMBERLEVEL_ANY == m_level)
		{
			numberList.push_back(ctable.countNode(
						executionContext, 
						this,
						sourceNode));
		}
		else
		{
			const MutableNodeRefList	ancestors = getMatchingAncestors(executionContext, sourceNode,
				Constants::NUMBERLEVEL_SINGLE == m_level);

			const unsigned int	lastIndex = ancestors.getLength();

			if(lastIndex > 0)
			{
				for(unsigned int i = 0; i < lastIndex; i++)
				{
					XalanNode* const target = ancestors.item(lastIndex - i - 1);

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
	// Create an XPathGuard, since we may need to
	// create a new XPath...
	StylesheetExecutionContext::XPathGuard	xpathGuard(
			executionContext);

	const XPath*	countMatchPattern = m_countMatchPattern;

	if (countMatchPattern == 0)
	{
		// Get the XPath...
		xpathGuard.reset(getCountMatchPattern(executionContext, pos));

		countMatchPattern = xpathGuard.get();
	}

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

	// Create an XPathGuard, since we may need to
	// create a new XPath...
	StylesheetExecutionContext::XPathGuard	xpathGuard(
			executionContext);

	const XPath*	countMatchPattern = m_countMatchPattern;

	if (countMatchPattern == 0)
	{
		// Get the XPath...
		xpathGuard.reset(getCountMatchPattern(executionContext, sourceNode));

		countMatchPattern = xpathGuard.get();
	}

	if(Constants::NUMBERLEVEL_ANY == m_level)
	{
		target = findPrecedingOrAncestorOrSelf(
				executionContext,
				m_fromMatchPattern,
				countMatchPattern,
				sourceNode,
				this);
	}
	else
	{
		target = findAncestor(
				executionContext,
				m_fromMatchPattern,
				countMatchPattern,
				sourceNode,
				this);
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

	// Create an XPathGuard, since we may need to
	// create a new XPath...
	StylesheetExecutionContext::XPathGuard	xpathGuard(
			executionContext);

	const XPath*	countMatchPattern = m_countMatchPattern;

	if (countMatchPattern == 0)
	{
		// Get the XPath...
		xpathGuard.reset(getCountMatchPattern(executionContext, node));

		countMatchPattern = xpathGuard.get();
	}

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
			error(TranscodeFromLocalCodePage("Programmer error! countMatchPattern should never be 0!"));

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



XalanNumberFormat*
ElemNumber::getNumberFormatter(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						contextNode) const
{
    // Helper to format local specific numbers to strings.
	XalanAutoPtr<XalanNumberFormat>		formatter(executionContext.createXalanNumberFormat());

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
		formatter->setGroupingSize(DOMStringToUnsignedLong(nDigitsPerGroupValue));
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
	XalanDOMChar	numberType(XalanUnicode::charDigit_1);
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

	if (tokenVector.size() > 0)
	{
		it = tokenVector.begin();

		if(!isXMLLetterOrDigit(charAt(*it, 0)))
		{
			leaderStr = *it;
			tokenVector.erase(it);
		}

		if (tokenVector.size() > 0)
		{
			it = tokenVector.end() - 1;

			if(!isXMLLetterOrDigit(charAt(*it, 0)))
			{
				trailerStr = *it;
				tokenVector.erase(it);
			}
		}
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
			// $$$ ToDo: This assert is commented out until we get
			// out character classification problems fixed.
			// assert(isXMLLetterOrDigit(charAt((*it), 0)));
			formatToken = *it++;
			numberWidth = length(formatToken);
			numberType = charAt(formatToken, numberWidth - 1);
		}
		if (it != tokenVector.end())
		{
			// $$$ ToDo: This assert is commented out until we get
			// out character classification problems fixed.
			//assert(!isXMLLetterOrDigit(charAt((*it), 0)));
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



bool
ElemNumber::evaluateLetterValueAVT(
			StylesheetExecutionContext&		executionContext,
			XalanNode* 						contextNode,
			const XalanDOMString&			compareValue) const
{
	if (m_lettervalue_avt == 0)
	{
		return false;
	}
	else
	{
		XalanDOMString letterVal;

		m_lettervalue_avt->evaluate(
				letterVal,
				contextNode,
				*this,
				executionContext);

		return equals(compareValue, letterVal);
	}
}



XalanDOMString
ElemNumber::traditionalAlphaCount(
			int										theValue,
			const XalanNumberingResourceBundle&		theResourceBundle) const
{
	typedef XalanNumberingResourceBundle::IntVectorType			IntVectorType;
	typedef XalanNumberingResourceBundle::DigitsTableVectorType	DigitsTableVectorType;
	typedef XalanNumberingResourceBundle::eNumberingMethod		eNumberingMethod;
	typedef XalanNumberingResourceBundle::eMultiplierOrder		eMultiplierOrder;

	// if this number is larger than the largest number we can represent, error!
	//if (val > theResourceBundle.getMaxNumericalValue())
	//return XSLTErrorResources.ERROR_STRING;
	XalanDOMCharVectorType	table;

	// index in table of the last character that we stored
	IntVectorType::size_type	lookupIndex = 1;  // start off with anything other than zero to make correction work

	// Create a buffer to hold the result
	// TODO:  size of the table can be detereined by computing
	// logs of the radix.  For now, we fake it.
	XalanDOMChar	buf[100];

	//some languages go left to right(ie. english), right to left (ie. Hebrew),
	//top to bottom (ie.Japanese), etc... Handle them differently
	//String orientation = thisBundle.getString(Constants.LANG_ORIENTATION);

	// next character to set in the buffer
	int charPos = 0;

	// array of number groups: ie.1000, 100, 10, 1
	const IntVectorType&	groups = theResourceBundle.getNumberGroups();

	const IntVectorType::size_type	groupsSize = groups.size();

	// array of tables of hundreds, tens, digits. Indexes into the vector
	// returned by 
	const IntVectorType&	tables = theResourceBundle.getDigitsTableTable();

	const DigitsTableVectorType&	digitsTable = theResourceBundle.getDigitsTable();

	// some languages have additive alphabetical notation,
	// some multiplicative-additive, etc... Handle them differently.
	const eNumberingMethod	numbering = theResourceBundle.getNumberingMethod();

	// do multiplicative part first
	if (numbering == XalanNumberingResourceBundle::eMultiplicativeAdditive)
	{
		const eMultiplierOrder	mult_order = theResourceBundle.getMultiplierOrder();

		const IntVectorType&	multiplier = theResourceBundle.getMultipliers();

		const IntVectorType::size_type	multiplierSize = multiplier.size();

		const XalanDOMCharVectorType&	zeroChar = theResourceBundle.getZeroChar();

		const XalanDOMCharVectorType::size_type		zeroCharSize = zeroChar.size();

		const XalanDOMCharVectorType&	multiplierChars = theResourceBundle.getMultiplierChars();

		IntVectorType::size_type	i = 0;

		// skip to correct multiplier
		while (i < multiplierSize && theValue < multiplier[i])
		{
			i++;
		}

		do
		{
			if (i >= multiplierSize)
				break;			  //number is smaller than multipliers

			// some languages (ie chinese) put a zero character (and only one) when
			// the multiplier is multiplied by zero. (ie, 1001 is 1X1000 + 0X100 + 0X10 + 1)
			// 0X100 is replaced by the zero character, we don't need one for 0X10
			if (theValue < multiplier[i])
			{
				if (zeroCharSize == 0)
				{
					i++;
				} 
				else
				{
					if (buf[charPos - 1] != zeroChar[0])
					{
						buf[charPos++] = zeroChar[0];
					}

					i++;
				}
			}
			else if (theValue >= multiplier[i])
			{

				int mult = theValue / multiplier[i];

				theValue = theValue % multiplier[i];		 // save this.

				IntVectorType::size_type	k = 0;

				while (k < groupsSize)
				{
					lookupIndex = 1;				 // initialize for each table
			
					if (mult / groups[k] <= 0) 		 // look for right table
					{
						k++;
					}
					else
					{
						assert(digitsTable.size() > tables[k]);

						// get the table
						const XalanDOMCharVectorType&	THEletters =
								digitsTable[tables[k]];

						const XalanDOMCharVectorType::size_type		THElettersSize =
							THEletters.size();

						table.resize(THElettersSize + 1);					

						const IntVectorType::size_type	tableSize = table.size();

						XalanDOMCharVectorType::size_type	j = 0;

						for (; j < THElettersSize; j++)
						{
							table[j + 1] = THEletters[j];
						}

						table[0] = THEletters[j - 1];	 // don't need this

						// index in "table" of the next char to emit
						lookupIndex  = mult / groups[k];

						//this should not happen
						if (lookupIndex == 0 && mult == 0)
						{
							break;
						}

						assert(i < multiplierChars.size());

						const XalanDOMChar	multiplierChar = multiplierChars[i];

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
									buf[charPos++] =  table[lookupIndex];
								}

								buf[charPos++] =	multiplierChar ;
							}

							break;		 // all done!
						}
						else
						{
							return XALAN_STATIC_UCODE_STRING("#error");
						}
					} //end else
				} // end while	

				i++;

			} // end else if
		} // end do while
		while ( i < multiplierSize);		
	}

	// Now do additive part...

	IntVectorType::size_type	count = 0;

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
			const XalanDOMCharVectorType&	theletters =
								digitsTable[tables[count]];

			const XalanDOMCharVectorType::size_type		thelettersSize =
							theletters.size();

			table.resize(thelettersSize + 1);
		
			const IntVectorType::size_type	tableSize = table.size();

			XalanDOMCharVectorType::size_type	j = 0;

			// need to start filling the table up at index 1
			for (0; j < thelettersSize; j++)
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
				buf[charPos++] = table[lookupIndex];	// left to right or top to bottom					
			}
			else
			{
				return XALAN_STATIC_UCODE_STRING("#error");
			}

			count++;
		}
	} // end while

	// String s = new String(buf, 0, charPos);
	return XalanDOMString(buf, charPos);
}



XalanDOMString
ElemNumber::getFormattedNumber(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						contextNode,
			XalanDOMChar					numberType,
			int								numberWidth,
			int								listElement) const
{
	switch(numberType)
	{
		case XalanUnicode::charLetter_A:
			return int2alphaCount(listElement, s_alphaCountTable);
			break;

		case XalanUnicode::charLetter_a:
			return toLowerCase(int2alphaCount(listElement, s_alphaCountTable));
			break;

		case XalanUnicode::charLetter_I:
			return long2roman(listElement, true);
			break;

		case XalanUnicode::charLetter_i:
			return toLowerCase(long2roman(listElement, true));
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
			return XalanDOMString();
			break;

		// Handle the special case of Greek letters for now
		case 0x03B1:
			return int2alphaCount(listElement, s_elalphaCountTable);
			break;

		default: // "1"
			{
				StylesheetExecutionContext::XalanNumberFormatAutoPtr	formatter(
						getNumberFormatter(executionContext, contextNode));

				XalanDOMString	numString =
					formatter->format(listElement);

				const unsigned int	lengthNumString = length(numString);

				const int	nPadding = numberWidth - lengthNumString;

				if (nPadding > 0)
				{
					const XalanDOMString	padString = formatter->format(0);

					reserve(numString, nPadding * length(padString) + lengthNumString + 1);

					for(int i = 0; i < nPadding; i++)
					{
						insert(numString, 0, padString);
					}
				}

				return numString;
			}
			break;
	}
}



XalanDOMString
ElemNumber::int2singlealphaCount(
		int						val, 
		const XalanDOMString&	table)
{
	const int		radix = length(table);

	// TODO:  throw error on out of range input
	if (val > radix)
	{
		return XalanDOMString(XALAN_STATIC_UCODE_STRING("#E(") +
				LongToDOMString(val) +
				XALAN_STATIC_UCODE_STRING(")"));
	}
	else
	{
		const XalanDOMChar	theChar = charAt(table, val - 1);

		return XalanDOMString(&theChar, 1);
	}
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
		return XalanDOMString(XALAN_STATIC_UCODE_STRING("0"));
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

	if (isXMLLetterOrDigit(charAt(m_str, m_currentPosition)))
	{
		while ((m_currentPosition < m_maxPosition) &&
				isXMLLetterOrDigit(charAt(m_str, m_currentPosition))) 
			m_currentPosition++;
	}
	else
	{
		while ((m_currentPosition < m_maxPosition) &&
				!isXMLLetterOrDigit(charAt(m_str, m_currentPosition))) 
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
		if (isXMLLetterOrDigit(charAt(m_str, currpos)))
		{
			while ((currpos < m_maxPosition) &&
					isXMLLetterOrDigit(charAt(m_str, currpos))) 
				currpos++;
		}
		else
		{
			while ((currpos < m_maxPosition) &&
					!isXMLLetterOrDigit(charAt(m_str, currpos))) 
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

	for(int i = n - 1; i >= 0; i--)
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



static const XalanDOMChar	alphaCountTable[] =
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



static const XalanDOMChar	elalphaCountTable[] =
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

static XalanDOMString								s_atString;

static XalanDOMString								s_textString;

static XalanDOMString								s_commentString;

static XalanDOMString								s_slashString;

static XalanDOMString								s_piString;

static XalanDOMString								s_leftParenString;

static XalanDOMString								s_dotString;

static XalanDOMString								s_oneString;

static XalanDOMString								s_elalphaCountTable;

static XalanDOMString								s_alphaCountTable;

static ElemNumber::DecimalToRomanVectorType			s_romanConvertTable;

static ElemNumber::NumberingResourceBundleMapType	s_resourceBundles;

const XalanDOMString&	ElemNumber::s_atString = ::s_atString;

const XalanDOMString&	ElemNumber::s_textString = ::s_textString;

const XalanDOMString&	ElemNumber::s_commentString = ::s_commentString;

const XalanDOMString&	ElemNumber::s_slashString = ::s_slashString;

const XalanDOMString&	ElemNumber::s_piString = ::s_piString;

const XalanDOMString&	ElemNumber::s_leftParenString = ::s_leftParenString;

const XalanDOMString&	ElemNumber::s_dotString = ::s_dotString;

const XalanDOMString&	ElemNumber::s_oneString = ::s_oneString;

const XalanDOMString&	ElemNumber::s_elalphaCountTable = ::s_elalphaCountTable;

const XalanDOMString&	ElemNumber::s_alphaCountTable = ::s_alphaCountTable;

const ElemNumber::DecimalToRomanVectorType&		ElemNumber::s_romanConvertTable =
				::s_romanConvertTable;

const ElemNumber::NumberingResourceBundleMapType&	ElemNumber::s_resourceBundles =
				::s_resourceBundles;

void
ElemNumber::initialize()
{
	::s_atString = XALAN_STATIC_UCODE_STRING("@");

	::s_textString = XALAN_STATIC_UCODE_STRING("text()");

	::s_commentString = XALAN_STATIC_UCODE_STRING("comment()");

	::s_slashString = XALAN_STATIC_UCODE_STRING("/");

	::s_piString = XALAN_STATIC_UCODE_STRING("pi(");

	::s_leftParenString = XALAN_STATIC_UCODE_STRING(")");

	::s_dotString = XALAN_STATIC_UCODE_STRING(".");

	::s_oneString = XALAN_STATIC_UCODE_STRING("1");

	::s_alphaCountTable = alphaCountTable;

	::s_elalphaCountTable = elalphaCountTable;

	::s_romanConvertTable.reserve(7);

	::s_romanConvertTable.push_back(
		DecimalToRoman(
			1000L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("M")),
			900L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("CM"))));

	::s_romanConvertTable.push_back(
		DecimalToRoman(
			500L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("D")),
			400,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("CD"))));

	::s_romanConvertTable.push_back(
		DecimalToRoman(
			100L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("C")),
			90L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("XC"))));

	::s_romanConvertTable.push_back(
		DecimalToRoman(
			50L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("L")),
			40L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("XL"))));

	::s_romanConvertTable.push_back(
		DecimalToRoman(
			10L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("X")),
			9L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("IX"))));

	::s_romanConvertTable.push_back(
		DecimalToRoman(
			5L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("V")),
			4L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("IV"))));

	::s_romanConvertTable.push_back(
		DecimalToRoman(
			1L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("I")),
			1L,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("I"))));
}



void
ElemNumber::terminate()
{
	clear(::s_atString);
	clear(::s_textString);
	clear(::s_commentString);
	clear(::s_slashString);
	clear(::s_piString);
	clear(::s_leftParenString);
	clear(::s_dotString);
	clear(::s_oneString);

	clear(::s_alphaCountTable);
	clear(::s_elalphaCountTable);

	DecimalToRomanVectorType().swap(::s_romanConvertTable);

	NumberingResourceBundleMapType().swap(::s_resourceBundles);
}
