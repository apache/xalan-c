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



#include <PlatformSupport/DoubleSupport.hpp>



#include <unicode/coll.h>
#include <unicode/dcfmtsym.h>
#include <unicode/decimfmt.h>



#include <Include/XalanAutoPtr.hpp>



#if defined(XALAN_NO_NAMESPACES)
	typedef vector<UChar>					UCharVectorType;
#else
	typedef std::vector<UChar>				UCharVectorType;
#endif



#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
inline void
doCopyData(
			const XalanDOMChar*		theString,
			unsigned int			theStringLength,
			XalanDOMChar*			theBuffer)
{
	// Copy the data, truncating each character...
	for (unsigned int i = 0; i < theStringLength; ++i)
	{
		// There should be no truncation, since XalanDOMChars
		// hold UTF-16 code points, but assert, just in case...
		assert(theString[i] == UChar(theString[i]));

		theBuffer[i] = theString[i];
	}

}
#endif



// Use a stack-based buffer up to this size.
const unsigned int	theStackBufferSize = 200u;



const UnicodeString
ICUBridge::XalanDOMCharStringToUnicodeString(const XalanDOMChar*	theString)
{
	if (theString == 0)
	{
		return UnicodeString();
	}
	else
	{
#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)

		const unsigned int	theLength = length(theString);

		if (theStackBufferSize > theLength)
		{
			XalanDOMChar	theBuffer[theStackBufferSize];

			doCopyData(theString, theLength, theBuffer);

#if U_SIZEOF_WCHAR_T==2
			return UnicodeString((wchar_t*)&theBuffer[0], theLength);
#else
			return UnicodeString(&theBuffer[0], theLength);
#endif
		}
		else
		{
			// Create a buffer to copy out the UnicodeString data...
			UCharVectorType		theBuffer;

			// Resize the buffer appropriately...
			theBuffer.resize(theLength);		

#if U_SIZEOF_WCHAR_T==2
			doCopyData(theString, theLength, (XalanDOMChar*)&theBuffer[0]);
#else
			doCopyData(theString, theLength, &theBuffer[0]);
#endif

			assert(theLength == theBuffer.size());

			return UnicodeString(&theBuffer[0], theLength);
		}
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
ICUBridge::UnicodeStringToXalanDOMString(const UnicodeString&	theString)
{
	const int32_t	theLength = theString.length();

#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)

	// If XalanDOMChar is larger than the ICU's UChar, we have to more work...
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

	if (theStackBufferSize > theLength)
	{
		UChar	theBuffer[theStackBufferSize];

		// Extract the data...
		theString.extract(0, theLength, theBuffer);

		return XalanDOMString(theBuffer, theLength);
	}
	else
	{
		// Create a buffer to copy out the UnicodeString data...
		UCharVectorType		theBuffer;

		// Resize the buffer appropriately...
		theBuffer.resize(theLength);

		// Extract the data...
		theString.extract(0, theLength, &theBuffer[0]);

		assert(theLength == int32_t(theBuffer.size()));

		return XalanDOMString(&theBuffer[0], theLength);
	}
#endif
}



void
ICUBridge::UnicodeStringToXalanDOMString(
			const UnicodeString&	theString,
			XalanDOMString&			theResult)
{
#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
	
	// If XalanDOMChar is larger than the ICU's UChar, we have to more work.
	// Don't bother to provide the optimized version, just call to the
	// previous function.

	theResult = UnicodeStringToXalanDOMString(theString);

#else

	const int32_t	theLength = theString.length();

	if (theStackBufferSize > theLength)
	{
		UChar	theBuffer[theStackBufferSize];

		// Extract the data...
		theString.extract(0, theLength, theBuffer);

		theResult = XalanDOMString(theBuffer, theLength);
	}
	else
	{
#if defined(XALAN_NO_NAMESPACES)
		typedef vector<UChar>		UCharVectorType;
#else
		typedef std::vector<UChar>	UCharVectorType;
#endif

		// Create a buffer to copy out the UnicodeString data...
		UCharVectorType		theBuffer;

		// Resize the buffer appropriately...
		theBuffer.resize(theLength);

		// Extract the data...
		theString.extract(0, theLength, &theBuffer[0]);

		theResult = XalanDOMString(&theBuffer[0], theBuffer.size());
	}
#endif
}



static void
doFormatNumber(
			const XalanDOMString&				thePattern,
			double								theNumber,
			const XalanDecimalFormatSymbols&	theXalanDFS,
			UErrorCode&							theStatus,
			XalanDOMString&						theResult)
{
	if (theStatus == U_ZERO_ERROR ||
		theStatus == U_USING_DEFAULT_ERROR)
	{
		// Use a XalanAutoPtr, to keep this safe until we construct the DecimalFormat instance.
		XalanAutoPtr<DecimalFormatSymbols>	theDFS(new DecimalFormatSymbols(theStatus));

		// We got a XalanDecimalFormatSymbols, so set the
		// corresponding data in the ICU DecimalFormatSymbols.
		theDFS->setZeroDigit(theXalanDFS.getZeroDigit());
		theDFS->setGroupingSeparator(theXalanDFS.getGroupingSeparator());
		theDFS->setDecimalSeparator(theXalanDFS.getDecimalSeparator());
		theDFS->setPerMill(theXalanDFS.getPerMill());
		theDFS->setPercent(theXalanDFS.getPercent());
		theDFS->setDigit(theXalanDFS.getDigit());
		theDFS->setPatternSeparator(theXalanDFS.getPatternSeparator());

		theDFS->setInfinity(ICUBridge::XalanDOMStringToUnicodeString(theXalanDFS.getInfinity()));
		theDFS->setNaN(ICUBridge::XalanDOMStringToUnicodeString(theXalanDFS.getNaN()));
	//	theDFS->setPlusSign(theZeroDigitChar);
		theDFS->setMinusSign(theXalanDFS.getMinusSign());
	//	theDFS->setExponentialSymbol(theZeroDigitChar);
		theDFS->setCurrencySymbol(ICUBridge::XalanDOMStringToUnicodeString(theXalanDFS.getCurrencySymbol()));
		theDFS->setInternationalCurrencySymbol(ICUBridge::XalanDOMStringToUnicodeString(theXalanDFS.getInternationalCurrencySymbol()));
		theDFS->setMonetaryDecimalSeparator(theXalanDFS.getMonetaryDecimalSeparator());

		UnicodeString	theUnicodeResult;

		// Construct a DecimalFormat.  Note that we release the XalanAutoPtr, since the
		// DecimalFormat will adopt the DecimalFormatSymbols instance.
		DecimalFormat	theFormatter(ICUBridge::XalanDOMStringToUnicodeString(thePattern), theDFS.release(), theStatus);

		if (theStatus == U_ZERO_ERROR ||
		    (theStatus >= U_ERROR_INFO_START && theStatus < U_ERROR_INFO_LIMIT))
		{
			// Do the format...
			theFormatter.format(theNumber, theUnicodeResult);

			ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult, theResult);

			theStatus = U_ZERO_ERROR;
		}
	}
}



unsigned long
ICUBridge::FormatNumber(
			const XalanDOMString&				thePattern,
			double								theNumber,
			const XalanDecimalFormatSymbols*	theXalanDFS,
			XalanDOMString&						theResult)
{
	UErrorCode	theStatus = U_ZERO_ERROR;

	if (theXalanDFS == 0)
	{
		XalanDecimalFormatSymbols	theDefaultSymbols;

		doFormatNumber(
				thePattern,
				theNumber,
				theDefaultSymbols,
				theStatus,
				theResult);
	}
	else
	{
		doFormatNumber(
				thePattern,
				theNumber,
				*theXalanDFS,
				theStatus,
				theResult);
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
	UErrorCode				theStatus = U_ZERO_ERROR;

	// Create a collator, and keep it in an XalanAutoPtr...
	const XalanAutoPtr<Collator>	theCollator(Collator::createInstance(theStatus));

	if (theStatus == U_ZERO_ERROR || theStatus == U_USING_DEFAULT_ERROR)
	{
		// OK, do the compare...
		return theCollator->compare(
#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
					XalanDOMCharStringToUnicodeString(theLHS),
					XalanDOMCharStringToUnicodeString(theRHS));
#else
					theLHS,
					length(theLHS),
					theRHS,
					length(theRHS));
#endif
	}
	else
	{
		// If creating the ICU Collator failed, fall back to the default...
		return collationCompare(theLHS, theRHS);
	}
}
