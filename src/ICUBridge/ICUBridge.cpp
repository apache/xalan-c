/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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

#include <ICUBridge/ICUBridge.hpp>



#include <memory>



#include <unicode/coll.h>
#include <unicode/dcfmtsym.h>
#include <unicode/decimfmt.h>



unsigned long
ICUBridge::FormatNumber(
			const UTF16VectorType&		thePattern,
			double						theNumber,
			UTF16VectorType&			theResult)
{
	const UnicodeString		theUnicodePattern(&thePattern[0], thePattern.size());

	UErrorCode				theStatus = U_ZERO_ERROR;

	UnicodeString			theUnicodeResult;

	DecimalFormat			theFormatter(theUnicodePattern, theStatus);

	if (theStatus == U_ZERO_ERROR ||
		theStatus == U_USING_DEFAULT_ERROR)
	{
		// Do the format...
		theFormatter.format(theNumber, theUnicodeResult);

		const int32_t	theLength = theUnicodeResult.length();

		// Resize the vector to the appropriate length...
		theResult.clear();
		theResult.resize(theLength);

		theUnicodeResult.extract(0, theLength, &theResult[0]);

		theStatus = U_ZERO_ERROR;
	}

	return theStatus;
}



unsigned long
ICUBridge::FormatNumber(
			const UTF16VectorType&	thePattern,
			double					theNumber,
			const UTF16VectorType&	theCurrencySymbolString,
			unsigned short			theDecimalSeparatorChar,
			unsigned short			theDigitChar,
			unsigned short			theGroupingSeparatorChar,
			const UTF16VectorType&	theInfinityString,
			const UTF16VectorType&	theInternationalCurrencySymbolString,
			unsigned short			theMinusSignChar,
			unsigned short			theMonetaryDecimalSeparatorChar,
			const UTF16VectorType&	theNaNString,
			unsigned short			thePatternSeparatorChar,
			unsigned short			thePercentChar,
			unsigned short			thePerMillChar,
			unsigned short			theZeroDigitChar,
			UTF16VectorType&		theResult)
{
	const UnicodeString		theUnicodePattern(&thePattern[0], thePattern.size());

	UErrorCode				theStatus = U_ZERO_ERROR;

	UnicodeString			theUnicodeResult;

	DecimalFormatSymbols	theDFS(theStatus);

	theDFS.setZeroDigit(theZeroDigitChar);
	theDFS.setGroupingSeparator(theGroupingSeparatorChar);
	theDFS.setDecimalSeparator(theDecimalSeparatorChar);
	theDFS.setPerMill(thePerMillChar);
	theDFS.setPercent(thePercentChar);
	theDFS.setDigit(theDigitChar);
	theDFS.setPatternSeparator(thePatternSeparatorChar);
	theDFS.setInfinity(UnicodeString(&theInfinityString[0], theInfinityString.size()));
	theDFS.setNaN(UnicodeString(&theNaNString[0], theNaNString.size()));
//	theDFS.setPlusSign(theZeroDigitChar);
	theDFS.setMinusSign(theMinusSignChar);
//	theDFS.setExponentialSymbol(theZeroDigitChar);
	theDFS.setCurrencySymbol(UnicodeString(&theCurrencySymbolString[0], theCurrencySymbolString.size()));
	theDFS.setInternationalCurrencySymbol(UnicodeString(&theInternationalCurrencySymbolString[0], theInternationalCurrencySymbolString.size()));
	theDFS.setMonetaryDecimalSeparator(theMonetaryDecimalSeparatorChar);

	if (theStatus == U_ZERO_ERROR)
	{
		DecimalFormat			theFormatter(theUnicodePattern, theDFS, theStatus);

		if (theStatus == U_ZERO_ERROR)
		{
			// Do the format...
			theFormatter.format(theNumber, theUnicodeResult);

			const int32_t	theLength = theUnicodeResult.length();

			// Resize the vector to the appropriate length...
			theResult.clear();
			theResult.resize(theLength);

			theUnicodeResult.extract(0, theLength, &theResult[0]);
		}
	}

	return theStatus;
}



int
ICUBridge::strLength(const UnicodeCharType*		theString)
{
	if (theString == 0)
	{
		return 0;
	}
	else
	{
		const UnicodeCharType*	current = theString;

		while(*current)
		{
			++current;
		}

		return current - theString;
	}
}



int
ICUBridge::collationCompare(
			const UnicodeCharType*		theLHS,
			const UnicodeCharType*		theRHS)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::auto_ptr;
#endif

	const UnicodeString		theUnicodeLHS(theLHS, strLength(theLHS));
	const UnicodeString		theUnicodeRHS(theRHS, strLength(theRHS));

	UErrorCode				theStatus = U_ZERO_ERROR;

	auto_ptr<Collator>	theCollator(Collator::createInstance(theStatus));

	if (theStatus == U_ZERO_ERROR || theStatus == U_USING_DEFAULT_ERROR)
	{
		return theCollator->compare(
					theUnicodeLHS,
					theUnicodeRHS);
	}
	else
	{
		return theUnicodeLHS.compare(theUnicodeRHS);
	}
}
