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



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanDecimalFormatSymbols.hpp>



#include <memory>



#include <unicode/coll.h>
#include <unicode/dcfmtsym.h>
#include <unicode/decimfmt.h>



#if defined(XALAN_NO_NAMESPACES)
	typedef vector<UChar>					UCharVectorType;
#else
	typedef std::vector<UChar>				UCharVectorType;
#endif



const UnicodeString
ICUBridge::XalanDOMCharStringToUnicodeString(const XalanDOMChar*	theString)
{
	if (theString == 0)
	{
		return UnicodeString();
	}
	else
	{
#if defined(XALAN_ICU_BRIDGE_UCHAR_MISMATCH)

		// Create a buffer to copy out the UnicodeString data...
		UCharVectorType		theBuffer;

		const unsigned int	theLength = length(theString);

		// Resize the buffer appropriately...
		theBuffer.reserve(theLength);

		// Copy the data, truncating each character...
		for (unsigned int i = 0; i < theLength; ++i)
		{
			// There should be no truncation, since XalanDOMChars
			// hold UTF-16 code points, but assert, just in case...
			assert(theString[i] == UChar(theString[i]));
			theBuffer.push_back(theString[i]);
		}

		return UnicodeString(&theBuffer[0], theBuffer.size());

#else

		return UnicodeString(theString, length(theString));

#endif
	}
}



const UnicodeString
ICUBridge::XalanDOMStringToUnicodeString(const XalanDOMString&	theString)
{
	// Just call up to the XalanDOMChar* version...
	return XalanDOMCharStringToUnicodeString(c_wstr(theString));
}



const XalanDOMString
ICUBridge::UnicodeStringToXalanDOMString(const UnicodeString&		theString)
{
#if defined(XALAN_ICU_BRIDGE_UCHAR_MISMATCH)

	// If XalanDOMChar is larger than the ICU's UChar, we have to more work...
	const int32_t	theLength = theString.length();

	// Create a buffer...
	XalanDOMCharVectorType	theBuffer;

	// Reserve the appropriate amount of space...
	theBuffer.reserve(theLength);

	// Copy the data...
	for (int32_t i = 0; i < theLength; ++i)
	{
		theBuffer.push_back(theString[i]);
	}

	return XalanDOMString(&theBuffer[0], theBuffer.size());

#else

	// Create a buffer to copy out the UnicodeString data...
	UCharVectorType		theResult;

	const int32_t	theLength = theString.length();

	// Resize the buffer appropriately...
	theResult.resize(theLength);

	// Extract the data...
	theString.extract(0, theLength, &theResult[0]);

	return XalanDOMString(&theResult[0], theResult.size());

#endif
}



void
ICUBridge::UnicodeStringToXalanDOMString(
			const UnicodeString&	theString,
			XalanDOMString&			theResult)
{
#if defined(XALAN_ICU_BRIDGE_UCHAR_MISMATCH)
	
	// If XalanDOMChar is larger than the ICU's UChar, we have to more work.
	// Don't bother to provide the optimized version, just call to the
	// previous function.

	theResult = UnicodeStringToXalanDOMString(theString);

#else

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<UChar>					UCharVectorType;
#else
	typedef std::vector<UChar>				UCharVectorType;
#endif

	// Create a buffer to copy out the UnicodeString data...
	UCharVectorType		theBuffer;

	const int32_t	theLength = theString.length();

	// Resize the buffer appropriately...
	theBuffer.resize(theLength);

	// Extract the data...
	theString.extract(0, theLength, &theBuffer[0]);

	theResult = XalanDOMString(&theBuffer[0], theBuffer.size());

#endif
}



unsigned long
ICUBridge::FormatNumber(
			const XalanDOMString&				thePattern,
			double								theNumber,
			const XalanDecimalFormatSymbols*	theXalanDFS,
			XalanDOMString&						theResult)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::auto_ptr;
#endif

	UErrorCode				theStatus = U_ZERO_ERROR;

	// Use an auto_ptr, to keep this safe until we construct the DecimalFormat instance.
	const auto_ptr<DecimalFormatSymbols>	theDFS(new DecimalFormatSymbols(theStatus));

	if (theStatus == U_ZERO_ERROR ||
		theStatus == U_USING_DEFAULT_ERROR)
	{
		if (theXalanDFS != 0)
		{
			// We got a XalanDecimalFormatSymbols, so set the
			// corresponding data in the ICU DecimalFormatSymbols.
			theDFS->setZeroDigit(theXalanDFS->getZeroDigit());
			theDFS->setGroupingSeparator(theXalanDFS->getGroupingSeparator());
			theDFS->setDecimalSeparator(theXalanDFS->getDecimalSeparator());
			theDFS->setPerMill(theXalanDFS->getPerMill());
			theDFS->setPercent(theXalanDFS->getPercent());
			theDFS->setDigit(theXalanDFS->getDigit());
			theDFS->setPatternSeparator(theXalanDFS->getPatternSeparator());

			theDFS->setInfinity(XalanDOMStringToUnicodeString(theXalanDFS->getInfinity()));
			theDFS->setNaN(XalanDOMStringToUnicodeString(theXalanDFS->getNaN()));
		//	theDFS->setPlusSign(theZeroDigitChar);
			theDFS->setMinusSign(theXalanDFS->getMinusSign());
		//	theDFS->setExponentialSymbol(theZeroDigitChar);
			theDFS->setCurrencySymbol(XalanDOMStringToUnicodeString(theXalanDFS->getCurrencySymbol()));
			theDFS->setInternationalCurrencySymbol(XalanDOMStringToUnicodeString(theXalanDFS->getInternationalCurrencySymbol()));
			theDFS->setMonetaryDecimalSeparator(theXalanDFS->getMonetaryDecimalSeparator());
		}

		UnicodeString	theUnicodeResult;

		// Construct a DecimalFormat.  Note that we release the auto_ptr, since the
		// DecimalFormat will adopt the DecimalFormatSymbols instance.
		DecimalFormat	theFormatter(XalanDOMStringToUnicodeString(thePattern), theDFS.release(), theStatus);

		if (theStatus == U_ZERO_ERROR ||
			theStatus == U_USING_DEFAULT_ERROR)
		{
			// Do the format...
			theFormatter.format(theNumber, theUnicodeResult);

			UnicodeStringToXalanDOMString(theUnicodeResult, theResult);

			theStatus = U_ZERO_ERROR;
		}
	}

	return theStatus;
}



int
ICUBridge::collationCompare(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS)
{
	// Just call to the XalanDOMChar* version...
	return collationCompare(c_wstr(theLHS), c_wstr(theRHS));
}



int
ICUBridge::collationCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::auto_ptr;
#endif

	UErrorCode				theStatus = U_ZERO_ERROR;

	// Create a collcator, and keep it in an auto_ptr
	const auto_ptr<Collator>	theCollator(Collator::createInstance(theStatus));

	if (theStatus == U_ZERO_ERROR || theStatus == U_USING_DEFAULT_ERROR)
	{
		// OK, do the compare...
		return theCollator->compare(
					XalanDOMCharStringToUnicodeString(theLHS),
					XalanDOMCharStringToUnicodeString(theRHS));
	}
	else
	{
		// If creating the ICU Collator failed, fall back to the default...
		return collationCompare(theLHS, theRHS);
	}
}
