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
#if !defined(XALAN_NUMERATORFORMATTER_HEADER_GUARD)
#define XALAN_NUMERATORFORMATTER_HEADER_GUARD



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <cstddef>
#include <memory>
#include <vector>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOMString.hpp>



#include "DecimalToRoman.hpp"



class XSLTProcessor;


/**
 * Class to help in converting enumerated numbers 
 * into strings, using the XSL conversion attributes.
 * Having this in a class helps to not have to extract 
 * the attributes everytime.
 */
class NumeratorFormatter
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<int> IntVectorType;
	typedef vector<DecimalToRoman>	DecimalToRomanVectorType;
#else
	typedef std::vector<int> IntVectorType;
	typedef std::vector<DecimalToRoman>	DecimalToRomanVectorType;
#endif


	/**
	 * Construct a NumeratorFormatter using an element that contains XSL number
	 * conversion attributes - format, letter-value, xml:lang, digit-group-sep,
	 * n-digits-per-group, and sequence-src.
	 */
	NumeratorFormatter(
			DOM_Element		xslNumberElement = DOM_Element(),
			XSLTProcessor*	xslProcessor = 0);

	virtual
	~NumeratorFormatter();

	/**
	 * Process the attributes of the node with number formatting attributes.
	 * I'd like to do this at construction time, but I have to do it every time
	 * because the elements may have attribute templates embedded in them, and
	 * so must be processed only when the node is actually called.
	 * TODO: Can I just do NumeratorFormatter.processAttributes up front now?
	 *
	 * @param contextNode current node in the source tree
	 */
	virtual void
	processAttributes(const DOM_Node&	contextNode);
  
	/**
	 * Format a vector of numbers into a formatted string.
	 *
	 * TODO: Optimize formatNumberList so that it caches the last count and
	 * reuses that info for the next count.
	 *
	 * @param xslNumberElement Element that takes %conversion-atts; attributes.
	 * @param list vector of one or more integer numbers
	 * @return String that represents list according to "conversion-atts"
	 *         attributes
	 */
	DOMString
		formatNumberList(const IntVectorType&		theList);

	/**
	 * Retrieve the associated stylesheet processor
	 * 
	 * @return pointer to XSL processor in use
	 */
	XSLTProcessor*
	getXSLProcessor() const
	{
		return m_xslProcessor;
	}

	/**
	 * Set the stylesheet processor to use
	 * 
	 * @param theProcessor pointer to XSL processor
	 */
	void
	setXSLProcessor(XSLTProcessor*	theProcessor)
	{
		m_xslProcessor = theProcessor;
	}

	/**
	 * This class returns tokens using non-alphanumberic characters as
	 * delimiters. 
	 */
	class NumberFormatStringTokenizer
	{
	public:

		/**
		 * Construct a NumberFormatStringTokenizer.
		 *
		 * @param theStr string to tokenize
		 */
		explicit
		NumberFormatStringTokenizer(const DOMString&	theStr = DOMString());

		/**
		 * Sets the string to tokenize.
		 *
		 * @param theString  new string to tokenize
		 */
		void
		setString(const DOMString&	theString);

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
		DOMString
		nextToken();

		/**
		 * Determine if there are tokens remaining
		 * 
		 * @return true if there are more tokens
		 */
		bool
		hasMoreTokens() const
		{
			return (m_currentPosition >= m_maxPosition) ? false : true;
		}

		/**
		 * Count the number of tokens yet to be parsed
		 * 
		 * @return number of remaining tokens
		 */
		int
		countTokens() const;

	private:

		int			m_currentPosition;
		int			m_maxPosition;
		DOMString	m_str;
	}; // end NumberFormatStringTokenizer

	NumberFormatStringTokenizer		m_formatTokenizer;

	/**
	 * Table to help in converting decimals to roman numerals.
	 */
	static const DecimalToRomanVectorType	m_romanConvertTable;

	/**
	 * Chars for converting integers into alpha counts.
	 */
	static const XMLCh		m_alphaCountTable[];
	static const size_t		m_alphaCountTableSize;

protected:

	/**
	 * Convert a long integer into alphabetic counting, in other words 
	 * count using the sequence A B C ... Z AA AB AC.... etc.
	 * @param val Value to convert -- must be greater than zero.
	 * @param table a table containing one character for each digit in the radix
	 * @return String representing alpha count of number.
	 * @see XSLTProcessor#DecimalToRoman
	 * 
	 * Note that the radix of the conversion is inferred from the size
	 * of the table.
	 */
	DOMString
	int2alphaCount(
			int				val, 
			const XMLCh		table[],
			size_t			theTableSize);

	/**
	 * Convert a long integer into roman numerals.
	 * @param val Value to convert.
	 * @param prefixesAreOK true_ to enable prefix notation (e.g. 4 = "IV"),
	 * false_ to disable prefix notation (e.g. 4 = "IIII").
	 * @return Roman numeral string.
	 * @see DecimalToRoman
	 * @see m_romanConvertTable
	 */
	DOMString
	long2roman(
			long	val,
			bool	prefixesAreOK);

	XSLTProcessor*	m_xslProcessor;
	DOM_Element		m_xslNumberElement;

// $$$ ToDo: How do we implement this in C++?
//		NumberFormatStringTokenizer m_formatTokenizer;
//		Locale m_locale;
//		java.text.NumberFormat m_formatter;

};



#endif	// XALAN_NUMERATORFORMATTER_HEADER_GUARD
