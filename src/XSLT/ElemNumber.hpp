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
#if !defined(XALAN_ELEMNUMBER_HEADER_GUARD)
#define XALAN_ELEMNUMBER_HEADER_GUARD 

/**
 * $Id$
 * 
 * $State$
 * 
 * @author Myriam Midy (Myriam_Midy @lotus.com 
 */

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"

// Base class header file.
#include "ElemTemplateElement.hpp"


// Just locale.h in G++
#if ! defined(__GNUC__)
#include <locale>
#endif



#include <dom/DOMString.hpp>



#include "DecimalToRoman.hpp"



class NumberFormat;
class QName;
class XPath;
class XPathExecutionContext;



class ElemNumber: public ElemTemplateElement
{
public:

	typedef std::vector<int> IntArrayType;

	ElemNumber(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const DOMString&				name,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber);

	virtual int
	getXSLToken() const; 

	virtual void
	execute(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					sourceTree, 
			const DOM_Node&					sourceNode,
			const QName&					mode) const;

	/**
	 * Add a child to the child list.
	 * <!ELEMENT xsl:apply-imports EMPTY>
	 */
	virtual NodeImpl*
	appendChild(NodeImpl*	newChild);

protected:

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
	findAncestor(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern,
			const DOM_Node&					context,
			const DOM_Element&				namespaceContext) const;

	  /**
	   * Given a 'from' pattern (ala xsl:number), a match pattern 
	   * and a context, find the first ancestor that matches the 
	   * pattern (including the context handed in).
	   * @param matchPatternString The match pattern.
	   * @param node The node that "." expresses.
	   * @param namespaceContext The context in which namespaces in the 
	   * queries are supposed to be expanded.
	   */
	DOM_Node findPrecedingOrAncestorOrSelf(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern,
			const DOM_Node&					context,
			const DOM_Element&				namespaceContext) const;

	/**
	 * Get the count match pattern, or a default value.
	 */
	const XPath*
	getCountMatchPattern(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					contextNode) const;

	/**
	 * Given an XML source node, get the count according to the 
	 * parameters set up by the xsl:number attributes.
	 */
	DOMString
	getCountString(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					sourceTree, 
			const DOM_Node&					sourceNode) const;

	/**
	 * from any position in the tree, return the 
	 * next node in the tree, assuming preorder 
	 * traversal preceded, or null if at the end.
	 */
	static DOM_Node
	getNextInTree(
			const DOM_Node&		pos,
			const DOM_Node&		from);

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
	int
	getNumberInTree(
			XPathExecutionContext&	executionContext,
			const XPath*			countMatchPattern, 
			const DOM_Node&			pos, 
			const DOM_Node&			from, 
			const DOM_Node&			target,
			int						countFrom) const;

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
	int
	getSiblingNumber(
			StylesheetExecutionContext&		executionContext,
			const XPath*					countMatchPattern, 
			const DOM_Node&					target) const;

	/**
	 * Count the ancestors, up to the root, that match the 
	 * pattern.
	 * @param patterns if non-null, count only nodes 
	 * that match this pattern, if null count all ancestors.
	 * @param node Count this node and it's ancestors.
	 * @return The number of ancestors that match the pattern.
	 */
	int
	countMatchingAncestors(	
			StylesheetExecutionContext&		executionContext,
			const XPath*					patterns,
			const DOM_Node&					node) const;

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
	IntArrayType
	getAncestorNumbers(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern, 
			const DOM_Node&					node) const;

#if ! defined(__GNUC__)
	/**
	 * Get the locale we should be using.
	 */
	std::locale
	getLocale(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					contextNode) const;
#endif

	NumberFormat*
	getNumberFormatter(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					contextNode) const;

	/**
	 * Format a vector of numbers into a formatted string.
	 * @param xslNumberElement Element that takes %conversion-atts; attributes.
	 * @param list Array of one or more integer numbers.
	 * @return String that represents list according to 
	 * %conversion-atts; attributes.
	 * TODO: Optimize formatNumberList so that it caches the last count and
	 * reuses that info for the next count.
	 */
	DOMString
	formatNumberList(	
			StylesheetExecutionContext&		executionContext,
			const IntArrayType&				theList, 
			const DOM_Node&					contextNode) const;

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
	static DOMString
	int2alphaCount(
			int					val,
			const DOMString&	table);

	/**
	 * Convert a long integer into roman numerals.
	 * @param val Value to convert.
	 * @param prefixesAreOK true_ to enable prefix notation (e.g. 4 = "IV"),
	 * false_ to disable prefix notation (e.g. 4 = "IIII").
	 * @return Roman numeral string.
	 * @see DecimalToRoman
	 * @see m_romanConvertTable
	 */
	static DOMString
	long2roman(
			long	val,
			bool	prefixesAreOK);


private:

	/*
	 * Get Formatted number
	 */
	DOMString 
	getFormattedNumber(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					contextNode,
			XMLCh							numberType,
			int								numberWidth,
			int								listElement) const;

	const XPath*	m_countMatchPattern;
	const XPath*	m_fromMatchPattern;
	const XPath*	m_valueExpr;

	int				m_level; // = Constants.NUMBERLEVEL_SINGLE;
	
	DOMString		m_format_avt;
	DOMString		m_lang_avt;
	DOMString		m_lettervalue_avt;
	DOMString		m_groupingSeparator_avt;
	DOMString		m_groupingSize_avt;

	/**
	* Chars for converting integers into alpha counts.
	* @see XSLTEngineImpl#int2alphaCount
	*/
	static const DOMString			s_alphaCountTable;

	/**
	 * Table to help in converting decimals to roman numerals.
	 * @see XSLTEngineImpl#DecimalToRoman
	 * @see XSLTEngineImpl#long2roman
	 */
	static const DecimalToRoman		s_romanConvertTable[];
};



#endif	// XALAN_ELEMNUMBER_HEADER_GUARD
