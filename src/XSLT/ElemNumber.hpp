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
  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
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



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



// Base class header file.
#include <XSLT/ElemTemplateElement.hpp>



#include <XPath/MutableNodeRefList.hpp>



#include <XSLT/DecimalToRoman.hpp>
#include <XSLT/XalanNumberingResourceBundle.hpp>



class AVT;
class CountersTable;
class XalanNumberFormat;
class XPath;
class XPathExecutionContext;



class ElemNumber: public ElemTemplateElement
{
private:

	struct Counter;

public:

	enum eLevel
	{
		eSingle,
		eMultiple,
		eAny,
	};

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<DecimalToRoman>					DecimalToRomanVectorType;
	typedef vector<int>								IntArrayType;
	typedef map<XalanDOMChar,
				XalanNumberingResourceBundle,
				less<XalanDOMChar> >				NumberingResourceBundleMapType;
#else
	typedef std::vector<DecimalToRoman>				DecimalToRomanVectorType;
	typedef std::vector<int>						IntArrayType;
	typedef std::map<XalanDOMChar,
					 XalanNumberingResourceBundle>	NumberingResourceBundleMapType;
#endif

	/**
	 * Perform static initialization.  See class XSLTInit.
	 */
	static void
	initialize();

	/**
	 * Perform static shut down.  See class XSLTInit.
	 */
	static void
	terminate();

	/**
	 * Construct an object corresponding to an "xsl:number" element
	 * 
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param atts                list of attributes for element
	 * @param lineNumber				line number in document
	 * @param columnNumber			column number in document
	 */
	ElemNumber(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber);

	virtual
	~ElemNumber();

	// These methods are inherited from ElemTemplateElement ...
	
	virtual const XalanDOMString&
	getElementName() const;

	virtual void
	execute(StylesheetExecutionContext&		executionContext) const;

	/**
	 * Get the previous node to be counted.
	 */
	XalanNode* getPreviousNode(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						pos) const;

	/**
	 * Get the target node that will be counted..
	 */
	XalanNode* getTargetNode(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceNode) const;

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
	XalanNode*
	findAncestor(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern,
			XalanNode*						context,
			const XalanElement*				namespaceContext) const;

	/**
	* Given a 'from' pattern (ala xsl:number), a match pattern 
	* and a context, find the first ancestor that matches the 
	* pattern (including the context handed in).
	* @param matchPatternString The match pattern.
	* @param node The node that "." expresses.
	* @param namespaceContext The context in which namespaces in the 
	* queries are supposed to be expanded.
	*/
	XalanNode*
	findPrecedingOrAncestorOrSelf(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern,
			XalanNode*						context,
			const XalanElement*				namespaceContext) const;

	/**
	 * Get the count match pattern, or a default value.
	 */
	const XPath*
	getCountMatchPattern(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						contextNode) const;

	/**
	 * Given an XML source node, get the count according to the 
	 * parameters set up by the xsl:number attributes.
	 */
	void
	getCountString(
			StylesheetExecutionContext&		executionContext,
			XalanDOMString&					theResult) const;

	void
	getCountString(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceNode,
			const MutableNodeRefList&		ancestors,
			CountersTable&					ctable,
			int								numberList[],
			NodeRefListBase::size_type		numberListLength,
			XalanDOMString&					theResult) const;

	/**
	 * Get the ancestors, up to the root, that match the
	 * pattern.
	 * @param patterns if non-0, count only nodes
	 * that match this pattern, if 0 count all ancestors.
	 * @param executionContext The current execution context.
	 * @param node Count this node and it's ancestors.
	 * @param stopAtFirstFound If true, only get the first matching ancestor
	 * @param ancestors The ancestors that match the pattern.
	 */
	void
	getMatchingAncestors(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						node, 
			bool							stopAtFirstFound,
			MutableNodeRefList&				ancestors) const;

	/**
	 * Get a formatter.
	 * @param executionContext The current execution context.
	 * @param contextNode The current context node.
	 * @return A new XalanNumberFormat instance.  The caller owns the memory.
	 */
	XalanNumberFormat*
	getNumberFormatter(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						contextNode) const;

	/**
	 * Format an array of integers into a formatted string.
	 *
	 * @param executionContext The current execution context.
	 * @param theList Array of one or more integer numbers.
	 * @param theListLength The length of the array.
	 * @param contextNode The context node.
	 * @param formattedNumber The formatted number result.
	 */
	void
	formatNumberList(	
			StylesheetExecutionContext&		executionContext,
			const int						theList[],
			NodeRefListBase::size_type		theListLength,
			XalanNode*						contextNode,
			XalanDOMString&					formattedNumber) const;

	/**
	 * Convert a long integer into alphabetic counting, in other words
	 * count using the sequence A B C ... Z.
	 * @param val Value to convert -- must be greater than zero.
	 * @param table a table containing one character for each digit in the radix
	 * @param theResult A string representing alpha count of number.
	 * @see XSLTEngineImpl#DecimalToRoman
	 *
	 * Note that the radix of the conversion is inferred from the size
	 * of the table.
	 */
	static void
	int2singlealphaCount(
			int						val,
			const XalanDOMString&	table,
			XalanDOMString&			theResult);
		
	/**
	 * Convert a long integer into alphabetic counting, in other words 
	 * count using the sequence A B C ... Z AA AB AC.... etc.
	 * @param val Value to convert -- must be greater than zero.
	 * @param table a table containing one character for each digit in the radix
	 * @param result returns the stringrepresenting alpha count of number.
	 * @see XSLTEngineImpl#DecimalToRoman
	 * 
	 * Note that the radix of the conversion is inferred from the size
	 * of the table.
	 */
	static void
	int2alphaCount(
			int						val,
			const XalanDOMString&	table,
			XalanDOMString&			theResult);

	/**
	 * Convert a long integer into traditional alphabetic counting, in other words
	 * count using the traditional numbering.
	 * @param val Value to convert -- must be greater than zero.
	 * @param table a table containing one character for each digit in the radix
	 * @return String representing alpha count of number.
	 * @see XSLProcessor#DecimalToRoman
	 *
	 * Note that the radix of the conversion is inferred from the size
	 * of the table.
	 */
	static void
	tradAlphaCount(
			int					val,
			XalanDOMString&		theResult);

	/**
	 * Convert a long integer into roman numerals.
	 * @param val Value to convert.
	 * @param prefixesAreOK true to enable prefix notation (e.g. 4 = "IV"), false to disable prefix notation (e.g. 4 = "IIII").
	 * @param theResult The formatted Roman numeral string.
	 * @see DecimalToRoman
	 * @see m_romanConvertTable
	 */
	static void
	long2roman(
			long				val,
			bool				prefixesAreOK,
			XalanDOMString&		theResult);

private:

	/**
	 * Convert a long integer into roman numerals.
	 * @param executionContext The current execution context.
	 * @param contextNode The current context node.
	 * @param val Value to convert.
	 * @param prefixesAreOK true to enable prefix notation (e.g. 4 = "IV"), false to disable prefix notation (e.g. 4 = "IIII").
	 * @param theResult The formatted Roman numeral string.
	 */
	void
	long2roman(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						contextNode,
			long							val,
			bool							prefixesAreOK,
			XalanDOMString&					theResult) const;

	void
	evaluateLetterValueAVT(
			StylesheetExecutionContext&		executionContext,
			XalanNode* 						contextNode,
			XalanDOMString&					value) const;

	void
	traditionalAlphaCount(
			int										theValue,
			const XalanNumberingResourceBundle&		theResourceBundle,
			XalanDOMString&							theResult) const;

	/*
	 * Get Formatted number
	 */
	void
	getFormattedNumber(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						contextNode,
			XalanDOMChar					numberType,
			XalanDOMString::size_type		numberWidth,
			int								listElement,
			XalanDOMString&					theResult) const;

	const XPath*	m_countMatchPattern;
	const XPath*	m_fromMatchPattern;
	const XPath*	m_valueExpr;

	int				m_level; // = Constants.NUMBERLEVEL_SINGLE;

	const AVT*		m_format_avt;
	const AVT*		m_lang_avt;
	const AVT*		m_lettervalue_avt;
	const AVT*		m_groupingSeparator_avt;
	const AVT*		m_groupingSize_avt;

	/**
 	 * The string "@".
 	 */
 	static const XalanDOMString&			s_atString;

		/**
 	 * The string "text()".
 	 */
 	static const XalanDOMString&			s_textString;

	/**
 	 * The string "comment()".
 	 */
 	static const XalanDOMString&			s_commentString;

	/**
 	 * The string "/".
 	 */
 	static const XalanDOMString&			s_slashString;

	/**
 	 * The string "pi(".
 	 */
 	static const XalanDOMString&			s_piString;

	/**
 	 * The string ")".
 	 */
 	static const XalanDOMString&			s_leftParenString;

	/**
 	 * The string ".".
 	 */
 	static const XalanDOMString&			s_dotString;

	/**
 	 * The string "1".
 	 */
 	static const XalanDOMString&			s_oneString;

	/**
 	 * The string ".".
 	 */
 	static const XalanDOMString&			s_defaultSeparatorString;

	/**
 	 * The string "level".
 	 */
 	static const XalanDOMChar				s_levelString[];

	/**
 	 * The string "multiple".
 	 */
 	static const XalanDOMChar				s_multipleString[];

	/**
 	 * The string "any".
 	 */
 	static const XalanDOMChar				s_anyString[];

	/**
 	 * The string "single".
 	 */
 	static const XalanDOMChar				s_singleString[];

	/**
 	 * The string "alphabetic".
 	 */
 	static const XalanDOMChar				s_alphabeticString[];

	/**
 	 * The string "traditional".
 	 */
 	static const XalanDOMChar				s_traditionalString[];

	/**
	* Chars for converting integers into alpha counts.
	* @see XSLTEngineImpl#int2alphaCount
	*/
	static const XalanDOMString&			s_alphaCountTable;

	static const XalanDOMString&			s_elalphaCountTable;

	/**
	 * Table to help in converting decimals to roman numerals.
	 * @see XSLTEngineImpl#DecimalToRoman
	 * @see XSLTEngineImpl#long2roman
	 */
	static const DecimalToRomanVectorType&	s_romanConvertTable;

	/**
	 * A map of supported numbering resource bundles.
	 */
	static const NumberingResourceBundleMapType&	s_resourceBundles;


public:

	/**
	 * This class returns tokens using non-alphanumberic characters as
	 * delimiters. 
	 */
	class NumberFormatStringTokenizer
	{
	public:

		typedef XalanDOMString::size_type	size_type;

		/**
		 * Construct a NumberFormatStringTokenizer.
		 *
		 * @param theString string to tokenize
		 */
		NumberFormatStringTokenizer(const XalanDOMString&	theString);

		/**
		 * Sets the string to tokenize.
		 *
		 * @param theString  new string to tokenize
		 */
		void
		setString(const XalanDOMString&	theString);

		/**
		 * Reset tokenizer so that nextToken() starts from the beginning.
		 */
		void
		reset()
		{
			m_currentPosition = 0;
		}

		/**
		 * Retrieve the next token to be parsed; behavior is undefined if there
		 * are no more tokens
		 * 
		 * @return next token string
		 */
		XalanDOMString
		nextToken();

		/**
		 * Retrieve the next token to be parsed.
		 * 
		 * @param theToken The next token string
		 */
		void
		nextToken(XalanDOMString&	theToken);

		/**
		 * Determine if there are tokens remaining
		 * 
		 * @return true if there are more tokens
		 */
		bool
		hasMoreTokens() const
		{
			return m_currentPosition >= m_maxPosition ? false : true;
		}

		/**
		 * Count the number of tokens yet to be parsed
		 * 
		 * @return number of remaining tokens
		 */
		size_type
		countTokens() const;

	private:

		size_type				m_currentPosition;

		size_type				m_maxPosition;

		const XalanDOMString*	m_string;
	};
	
private:
};



#endif	// XALAN_ELEMNUMBER_HEADER_GUARD
