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
// Class header file.
#include "NumeratorFormatter.hpp"



#include <climits>



#include <PlatformSupport/DOMStringHelper.hpp>
#include "XSLTEngineImpl.hpp"



std::vector<DecimalToRoman>
InitializeDecimalToRomanTable()
{
	std::vector<DecimalToRoman>	theVector(7);

	theVector[0] = DecimalToRoman(1000L, "M", 900L, "CM");
	theVector[1] = DecimalToRoman(500L, "D", 400L, "CD");
	theVector[2] = DecimalToRoman(100L, "C", 90L, "XC");
	theVector[3] = DecimalToRoman(50L, "L", 40L, "XL");
	theVector[4] = DecimalToRoman(10L, "X", 9L, "IX");
	theVector[5] = DecimalToRoman(5L, "V", 4L, "IV");
	theVector[6] = DecimalToRoman(1L, "I", 1L, "I");

	return theVector;
}



const std::vector<DecimalToRoman>	NumeratorFormatter::m_romanConvertTable =
							InitializeDecimalToRomanTable();


const XMLCh						NumeratorFormatter::m_alphaCountTable[] = 
{
	'Z', // z for zero
	'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 
	'G' , 'H' , 'I' , 'J' , 'K' , 'L' , 
	'M' , 'N' , 'O' , 'P' , 'Q' , 'R' , 
	'S' , 'T' , 'U' , 'V' , 'W' , 'X' , 
	'Y'
};



const size_t					NumeratorFormatter::m_alphaCountTableSize =
					sizeof(NumeratorFormatter::m_alphaCountTable) /
							sizeof (NumeratorFormatter::m_alphaCountTable[0]);



NumeratorFormatter::NumeratorFormatter(
			DOM_Element		xslNumberElement,
			XSLTProcessor*	xslProcessor) :
	m_xslProcessor(xslProcessor),
	m_xslNumberElement(xslNumberElement),
	m_formatTokenizer()
{
}



NumeratorFormatter::~NumeratorFormatter()
{
	m_xslProcessor = 0;
}



void
NumeratorFormatter::processAttributes(const DOM_Node&	contextNode)
{
/*
	@@ java: commented out
	assert(m_xslProcessor != 0);

	// See http://www.w3.org/TR/WD-xsl#convert for the form 
	// of the format string, which tells how the numbers should be 
	// formatted.
	DOMString	formatValue =
		m_xslProcessor->getProcessedAttrVal(m_xslNumberElement, 
										    Constants::ATTRNAME_FORMAT, 
										    contextNode);

	if(0 == length(formatValue))
	{
		formatValue = "1";
	}

	m_formatTokenizer.setString(formatValue);

	// The letter-value attribute disambiguates between numbering schemes 
	// that use letters. A value of "alphabetic" specifies the alphabetic 
	// sequence; a value of "other" specifies the other sequence.
	// TODO: Handle letter-value attribute.
	const DOMString	letterValue =
		m_xslProcessor->getProcessedAttrVal(m_xslNumberElement, 
										    Constants::ATTRNAME_LETTERVALUE, 
										    contextNode);

	if(0 != length(letterValue))
	{
		m_xslProcessor->warn(DOM_Node(),
							 contextNode,
							 DOMString(Constants::ATTRNAME_LETTERVALUE) +
								" not supported yet!");
	}

	// When numbering with an alphabetic sequence, the xml:lang 
	// attribute specifies which language's alphabet is to be used.
	const DOMString		langValue =
		m_xslProcessor->getAttrVal(m_xslNumberElement, "xml:lang");

#if 0
	if(0 != length(langValue))
	{
		// Not really sure what to do about the country code, so I use the
		// default from the system.  
		// TODO: fix xml:lang handling.
		m_locale = new Locale(langValue.toUpperCase(), 
							Locale.getDefault().getDisplayCountry());
		if(null == m_locale)
		{
		  warn(null, contextNode, "Warning: Could not find locale for xml:lang="+langValue);
		  m_locale = Locale.getDefault();
		}
	  }
	  else
	  {
		m_locale = Locale.getDefault();
	  }

	  // Helper to format local specific numbers to strings.
	  m_formatter = java.text.NumberFormat.getNumberInstance(m_locale);
#endif

	// The digit-group-sep attribute gives the separator between groups 
	// of digits, and the optional n-digits-per-group specifies the 
	// number of digits per group. For example, digit-group-sep="," 
	// and n-digits-per-group="3" would produce numbers of the form 1,000,000.
	const DOMString		digitGroupSepValue =
		m_xslProcessor->getProcessedAttrVal(m_xslNumberElement, 
										    Constants::ATTRNAME_DIGITGROUPSEP, 
										    contextNode);
	  
	const DOMString		nDigitsPerGroupValue = 
		m_xslProcessor->getProcessedAttrVal(m_xslNumberElement, 
										    Constants::ATTRNAME_NDIGITSPERGROUP, 
										    contextNode);

	const long	nDigitsPerGroup = DOMStringToLong(nDigitsPerGroupValue);

	  // TODO: Handle digit-group attributes
	if(0 != length(digitGroupSepValue) || 0 != length(nDigitsPerGroupValue))
	{
		// m_formatter.setGroupingUsed(true);
	}

	// The sequence-src attribute gives the URI of a text resource 
	// that contains a whitespace separated list of the members of 
	// the numbering sequence.
	// (Used with letter-value="other", I think.)
	const DOMString		sequenceSrcValue =
		m_xslProcessor->getProcessedAttrVal(m_xslNumberElement, 
										    Constants::ATTRNAME_SEQUENCESRC, 
										    contextNode);

	if(0 != length(sequenceSrcValue))
	{
		m_xslProcessor->warn(DOM_Node(),
							 contextNode,
							 DOMString(Constants::ATTRNAME_SEQUENCESRC) +
								" not supported yet!");
	}
*/
}



DOMString
NumeratorFormatter::formatNumberList(const IntVectorType&		theList)
{
	DOMString	formattedNumber;

	const int	nNumbers = theList.size();

	XMLCh		numberType = '1';
	int			numberWidth = 1;

	DOMString	formatToken;
	DOMString	sepString;
	DOMString	lastSepString;

	// $$$ ToDo: Fix this!
//	DOMString	padString = m_formatter.format(0);
	DOMString	padString = LongToDOMString(0);

	DOMString	lookahead; // next token

	m_formatTokenizer.reset();

	for(int i = 0; i < nNumbers; i++)
	{
		while(m_formatTokenizer.hasMoreTokens() == true)
		{
			if(length(lookahead) != 0)
			{
				formatToken = lookahead;
				lookahead = "";
			}
			else
			{
				formatToken = m_formatTokenizer.nextToken();
			}

			if(isLetterOrDigit(charAt(formatToken, formatToken.length() - 1)) == true)
			{
				numberWidth = formatToken.length();
				numberType = charAt(formatToken, numberWidth - 1);
				break; // from while(m_formatTokenizer.hasMoreTokens())
			}
			else
			{
				sepString = formatToken;

				while(m_formatTokenizer.hasMoreTokens() == true)
				{
					lookahead = m_formatTokenizer.nextToken();

					if(isLetterOrDigit(charAt(lookahead, 0)) == false)
					{
						sepString += lookahead;
						lookahead = ""; // consume
					}
					else
					{
						break; // from inner while loop
					}
				}

				if(m_formatTokenizer.hasMoreTokens() == false)
				{
					lastSepString = sepString;

					// crufty bit to strip whitespace off of separator 
					// if it is the end string.
					// This probably isn't right.
					const int	lastPos = sepString.length() - 1;
			  
					int k = lastPos;

					for(; k >= 0; k--)
					{
						if(isSpace(charAt(sepString, k)) == false)
						{
							break; // from for loop
						}
					}

					sepString = substring(sepString, 0, k + 1);
				}
			}
		} // end while

		switch(numberType)
		{
		case 'A':
			formattedNumber += (sepString + int2alphaCount(theList[i],
														   m_alphaCountTable,
														   m_alphaCountTableSize));
			break;

		case 'a':
			// $$$ ToDo: Fix this!!!
//			formattedNumber += (sepString + int2alphaCount(theList[i], m_alphaCountTable).toLowerCase(m_locale));
			formattedNumber += toLowerCase(sepString + int2alphaCount(theList[i],
																	  m_alphaCountTable,
																	  m_alphaCountTableSize));
			break;

		case 'I':
			formattedNumber += (sepString + long2roman(theList[i], true));
			break;

		case 'i':
			// $$$ ToDo: Fix this!!!
//			formattedNumber += (sepString + long2roman(theList[i], true).toLowerCase(m_locale));
			formattedNumber += toLowerCase(sepString + long2roman(theList[i], true));
			break;

		default: // "1"
			{
				// $$$ ToDo: Fix this!!!
				// DOMString	numString = m_formatter.format(theList[i]);
				DOMString	numString(LongToDOMString(theList[i]));

				const int	nPadding = numberWidth - numString.length();

				formattedNumber += sepString;

				for(int k = 0; k < nPadding; k++)
				{
					formattedNumber += padString;
				}

				formattedNumber += numString;
			}
		} // end switch
	} // end for

	// Check to see if we finished up the format string...
	if(0 == length(lastSepString))
	{
		lastSepString = "";

		while(m_formatTokenizer.hasMoreTokens() == true)
		{
			lookahead = m_formatTokenizer.nextToken();

// was:			if(isLetterOrDigit(charAt(lookahead, 0)))
			if( ! isLetterOrDigit(charAt(lookahead, 0)))
			{
				lastSepString += lookahead;
			}
			else
			{
				lastSepString = "";
			}
		}
	}

	if(0 != length(lastSepString))
	{
		formattedNumber += lastSepString;
	}

	return formattedNumber;
}



DOMString
NumeratorFormatter::int2alphaCount(
			int 			val,
			const XMLCh		table[],
			size_t 			theTableSize)
{
	assert(theTableSize < INT_MAX);

	const int	radix = static_cast<int>(theTableSize);

	// Create a buffer to hold the result
	// $$$ ToDo:  size of the table can be determined by computing
	// logs of the radix.  For now, we fake it.  
	XMLCh	buf[100];

	// next character to set in the buffer
	int charPos = (sizeof(buf) / sizeof(buf[0])) - 1 ;    // work backward through buf[]
	buf[charPos--] = 0;

	// index in table of the last character that we stored
	int lookupIndex = 1;	// start off with anything other than zero to make correction work

	//						Correction number
	//
	//	Correction can take on exactly two values:
	//
	//		0	if the next character is to be emitted is usual
	//
	//	  radix - 1 
	//			if the next char to be emitted should be one less than
	//			you would expect 
	//			
	// For example, consider radix 10, where 1="A" and 10="J"
	//
	// In this scheme, we count: A, B, C ...	 H, I, J (not A0 and certainly
	// not AJ), A1
	//
	// So, how do we keep from emitting AJ for 10?	After correctly emitting the
	// J, lookupIndex is zero.	We now compute a correction number of 9 (radix-1).
	// In the following line, we'll compute (val+correction) % radix, which is,
	// (val+9)/10.	By this time, val is 1, so we compute (1+9) % 10, which 
	// is 10 % 10 or zero.	So, we'll prepare to emit "JJ", but then we'll
	// later suppress the leading J as representing zero (in the mod system, 
	// it can represent either 10 or zero).  In summary, the correction value of
	// "radix-1" acts like "-1" when run through the mod operator, but with the 
	// desireable characteristic that it never produces a negative number.

	int correction = 0;

	// TODO:	throw error on out of range input

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
			break;

		// put out the next character of output
		buf[charPos--] = table[lookupIndex];
	}
	while (val > 0);

	return DOMString(&buf[charPos + 1]);
}



DOMString
NumeratorFormatter::long2roman(
			long	val,
			bool	prefixesAreOK)
{
	DOMString	roman;

	if(val <= 0)
	{
		roman = DOMString("#E(") + DOMString(val) + DOMString(")");
	}
	else
	{
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
	}

	return roman;
} // end long2roman



NumeratorFormatter::NumberFormatStringTokenizer::NumberFormatStringTokenizer(
			const DOMString&	theStr) :
	m_currentPosition(0),
	m_maxPosition(length(theStr)),
	m_str(theStr)
{
}



void
NumeratorFormatter::NumberFormatStringTokenizer::setString(const DOMString&	theString)
{
	m_str = theString;

	m_currentPosition = 0;
	m_maxPosition = length(theString);
}



DOMString
NumeratorFormatter::NumberFormatStringTokenizer::nextToken() 
{
	if (m_currentPosition >= m_maxPosition) 
	{
		// $$$ Todo: Implement!
//				throw new NoSuchElementException();
	}

	const int	start = m_currentPosition;

	while ((m_currentPosition < m_maxPosition) && 
		   isLetterOrDigit(charAt(m_str, m_currentPosition))) 
	{
		m_currentPosition++;
	}

	if ((start == m_currentPosition) &&
		(!isLetterOrDigit(charAt(m_str, m_currentPosition)))) 
	{
		m_currentPosition++;
	}

	return substring(m_str, start, m_currentPosition);
}



int
NumeratorFormatter::NumberFormatStringTokenizer::countTokens() const
{
	int 	count = 0;
	int 	currpos = m_currentPosition;

	while (currpos < m_maxPosition) 
	{
		const int	start = currpos;

		while ((currpos < m_maxPosition) &&
				isLetterOrDigit(charAt(m_str, currpos))) 
		{
			currpos++;
		}

		if ((start == currpos) &&
			(isLetterOrDigit(charAt(m_str, currpos)) == false)) 
		{
			currpos++;
		}

		count++;
	}

	return count;
}
