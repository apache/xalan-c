/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#if !defined(XALAN_ELEMNUMBER_HEADER_GUARD)
#define XALAN_ELEMNUMBER_HEADER_GUARD 



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



// Base class header file.
#include <xalanc/XSLT/ElemTemplateElement.hpp>



#include <xalanc/XPath/NodeRefListBase.hpp>



#include <xalanc/XSLT/CountersTable.hpp>
#include <xalanc/XSLT/DecimalToRoman.hpp>
#include <xalanc/XSLT/XalanNumberingResourceBundle.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class AVT;
class MutableNodeRefList;
class XalanNumberFormat;
class XPath;
class XPathExecutionContext;



class ElemNumber: public ElemTemplateElement
{
public:

	typedef CountersTable::CountType	CountType;

	enum eLevel
	{
		eSingle,
		eMultiple,
		eAny
	};

	typedef XalanVector<CountType>		CountTypeArrayType;

	/**
	 * Perform static initialization.  See class XSLTInit.
	 */
	static void
	initialize(MemoryManagerType&  theManager);

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
	 * @param id				  The unique ID within the stylesheet for this xsl:number element
	 */
	ElemNumber(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber,
			unsigned long					id);

    static ElemNumber*
    create(
            MemoryManagerType& theManager,
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber,
			unsigned long					id);

	virtual
	~ElemNumber();

	// These methods are inherited from ElemTemplateElement ...

	virtual const XalanDOMString&
	getElementName() const;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
	virtual const ElemTemplateElement*
	startElement(StylesheetExecutionContext&		executionContext) const;
#else
	virtual void
	execute(StylesheetExecutionContext&		executionContext) const;
#endif

	unsigned long
	getID() const
	{
		return m_id;
	}

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

	virtual	const XPath*
	getXPath(unsigned int	index = 0) const;

protected:

	/**
	 * Given a 'from' pattern (ala xsl:number), a match pattern 
	 * and a context, find the first ancestor that matches the 
	 * pattern (including the context handed in).
	 *
	 * @param executionContext The current execution context
	 * @param matchPatternString The match pattern.
	 * @param matchPatternString The count match pattern.
	 * @param node The context node
	 * @return A pointer to the matched node.
	 */
	XalanNode*
	findAncestor(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern,
			XalanNode*						context) const;

	/**
	 * Given a 'from' pattern (ala xsl:number), a match pattern 
	 * and a context, find the first ancestor that matches the 
	 * pattern (including the context handed in).
	 *
	 * @param executionContext The current execution context
	 * @param matchPatternString The match pattern.
	 * @param matchPatternString The count match pattern.
	 * @param node The context node
	 * @return A pointer to the matched node.
	 */
	XalanNode*
	findPrecedingOrAncestorOrSelf(
			StylesheetExecutionContext&		executionContext,
			const XPath*					fromMatchPattern,
			const XPath*					countMatchPattern,
			XalanNode*						context) const;

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
			const MutableNodeRefList&		ancestors,
			CountersTable&					ctable,
			CountType						numberList[],
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
	 * @return A new XalanNumberFormat instance.  The caller owns the memory.
	 */
	XalanNumberFormat*
	getNumberFormatter(StylesheetExecutionContext&		executionContext) const;

	/**
	 * Format an array of integrals into a formatted string.
	 *
	 * @param executionContext The current execution context.
	 * @param theList Array of one or more integer numbers.
	 * @param theListLength The length of the array.
	 * @param formattedNumber The formatted number result.
	 */
	void
	formatNumberList(	
			StylesheetExecutionContext&		executionContext,
			const CountType					theList[],
			NodeRefListBase::size_type		theListLength,
			XalanDOMString&					formattedNumber) const;

	/**
	 * Convert an intergral into alphabetic counting, in other words
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
			CountType				val,
			const XalanDOMString&	table,
			XalanDOMString&			theResult);
		
	/**
	 * Convert an integral into alphabetic counting, in other words 
	 * count using the sequence A B C ... Z AA AB AC.... etc.
	 * @param val Value to convert -- must be greater than zero.
	 * @param table a table containing one character for each digit in the radix
	 * @param length the table length
	 * @param result returns the stringrepresenting alpha count of number.
	 * @see XSLTEngineImpl#DecimalToRoman
	 * 
	 * Note that the radix of the conversion is inferred from the size
	 * of the table.
	 */
	static void
	int2alphaCount(
			CountType					val,
			const XalanDOMChar			table[],
			XalanDOMString::size_type	length,
			XalanDOMString&				theResult);

	/**
	 * Convert an integral into roman numerals.
	 * @param val Value to convert.
	 * @param prefixesAreOK true to enable prefix notation (e.g. 4 = "IV"), false to disable prefix notation (e.g. 4 = "IIII").
	 * @param theResult The formatted Roman numeral string.
	 * @see DecimalToRoman
	 * @see m_romanConvertTable
	 */
	static void
	toRoman(
			CountType			val,
			bool				prefixesAreOK,
			XalanDOMString&		theResult);

private:

	void
	evaluateLetterValueAVT(
			StylesheetExecutionContext&		executionContext,
			XalanDOMString&					value) const;

	void
	traditionalAlphaCount(
			CountType								theValue,
			const XalanNumberingResourceBundle&		theResourceBundle,
			XalanDOMString&							theResult) const;

	/*
	 * Get Formatted number
	 */
	void
	getFormattedNumber(
			StylesheetExecutionContext&		executionContext,
			XalanDOMChar					numberType,
			XalanDOMString::size_type		numberWidth,
			CountType						listElement,
			XalanDOMString&					theResult) const;

	const XPath*	m_countMatchPattern;
	const XPath*	m_fromMatchPattern;
	const XPath*	m_valueExpr;

	CountType		m_level; // = Constants.NUMBERLEVEL_SINGLE;

	const AVT*		m_format_avt;
	const AVT*		m_lang_avt;
	const AVT*		m_lettervalue_avt;
	const AVT*		m_groupingSeparator_avt;
	const AVT*		m_groupingSize_avt;

	const unsigned long		m_id;

	/**
 	 * The string "@".
 	 */
 	static const XalanDOMChar				s_atString[];

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
 	 * The string "processing-instruction(".
 	 */
 	static const XalanDOMChar				s_piString[];

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
 	 * The string "#error".
 	 */
 	static const XalanDOMChar				s_errorString[];

	/**
	* Chars for converting integers into alpha counts.
	*/
	static const XalanDOMChar				s_alphaCountTable[];

	static const XalanDOMString::size_type	s_alphaCountTableSize;

	static const XalanDOMChar				s_elalphaCountTable[];

	static const XalanDOMString::size_type	s_elalphaCountTableSize;

	/**
	 * Table to help in converting decimals to roman numerals.
	 * @see XSLTEngineImpl#DecimalToRoman
	 * @see XSLTEngineImpl#long2roman
	 */
	static const DecimalToRoman				s_romanConvertTable[];

    static const size_t                     s_romanConvertTableSize;
	/**
	 * numbering resource bundle for Greek numbering.
	 */
	static const XalanNumberingResourceBundle&	s_elalphaResourceBundle;


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



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMNUMBER_HEADER_GUARD
