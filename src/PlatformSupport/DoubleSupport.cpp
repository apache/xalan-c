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
#include "DoubleSupport.hpp"


#if defined(NO_STD_LIMITS)
#	if defined(__GNUC__)
#		include <math.h>
#		if defined(SOLARIS)
#			include <nan.h>
#		else
#			include <bits/nan.h>
#		endif
#	else
#		error Unsupported platform!!!
#	endif
#else
#	include <limits>
#endif



#include "DOMStringHelper.hpp"
#include "XalanUnicode.hpp"



#if defined(NO_STD_LIMITS)
#	if defined(__GNUC__)

#		if defined(SOLARIS)
			static const unsigned char	__nan_bits[] = { 0x7f, 0xf0,0xc0,0x0,0x0,0x0,0x0,0x0 };
			const double				DoubleSupport::s_NaN = *reinterpret_cast<const double*>(__nan_bits);
#		else
			const double	DoubleSupport::s_NaN = NAN;
#		endif

		const double	DoubleSupport::s_positiveInfinity = HUGE_VAL;
		const double	DoubleSupport::s_negativeInfinity = -HUGE_VAL;

#	else
#		error Unsupported platform!!!
#	endif
#else

#if defined(XALAN_NO_NAMESPACES)
typedef numeric_limits<double>			NumericLimitsType;
#else
typedef std::numeric_limits<double>		NumericLimitsType;
#endif

const double	DoubleSupport::s_NaN = NumericLimitsType::quiet_NaN();
const double	DoubleSupport::s_positiveInfinity = NumericLimitsType::infinity();
const double	DoubleSupport::s_negativeInfinity = NumericLimitsType::signaling_NaN();

#endif



#if defined(XALAN_NEED_SPECIAL_NAN_SUPPORT)
const unsigned long*	DoubleSupport::s_NaNFirstDWORD =
#if defined(XALAN_OLD_STYLE_CASTS)
					(const unsigned long*)&s_NaN;
#else
					reinterpret_cast<const unsigned long*>(&s_NaN);
#endif

const unsigned long*	DoubleSupport::s_NaNSecondDWORD =
					s_NaNFirstDWORD + 1;
#endif



bool
DoubleSupport::equal(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return false;
	}
	else
	{
		return theLHS == theRHS;
	}
}



bool
DoubleSupport::notEqual(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return true;
	}
	else
	{
		return theLHS != theRHS;
	}
}



bool
DoubleSupport::lessThan(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return false;
	}
	else
	{
		return theLHS < theRHS;
	}
}



bool
DoubleSupport::lessThanOrEqual(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return false;
	}
	else
	{
		return theLHS <= theRHS;
	}
}



bool
DoubleSupport::greaterThan(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return false;
	}
	else
	{
		return theLHS > theRHS;
	}
}



bool
DoubleSupport::greaterThanOrEqual(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return false;
	}
	else
	{
		return theLHS >= theRHS;
	}
}



double
DoubleSupport::add(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return getNaN();
	}
	else
	{
		return theLHS + theRHS;
	}
}



double
DoubleSupport::subtract(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return getNaN();
	}
	else
	{
		return theLHS - theRHS;
	}
}



double
DoubleSupport::multiply(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return getNaN();
	}
	else
	{
		return theLHS * theRHS;
	}
}



double
DoubleSupport::divide(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return getNaN();
	}
	else if (theRHS != 0)
	{
		return theLHS / theRHS;
	}
	else
	{
		// These are special cases, since we can't actually
		// do the division...
		if (theLHS == 0)
		{
			// This is NaN...
			return DoubleSupport::getNaN();
		}
		else if (theLHS > 0.0)
		{
			// This is positive infinity...
			return DoubleSupport::getPositiveInfinity();
		}
		else
		{
			// This is positive infinity...
			return DoubleSupport::getNegativeInfinity();
		}
	}
}



double
DoubleSupport::modulus(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return getNaN();
	}
	else
	{
		return long(theLHS) % long(theRHS);
	}
}



double
DoubleSupport::negative(double	theDouble)
{
	if (isNaN(theDouble) == true)
	{
		return getNaN();
	}
	else
	{
		return -theDouble;
	}
}



double
DoubleSupport::toDouble(const XalanDOMString&	theString)
{
	return toDouble(c_wstr(theString));
}



void
consumeWhitespace(const XalanDOMChar*&	theString)
{
	while(*theString != 0 &&
		  isSpace(*theString))
	{
		++theString;
	}
}



#if 1

void
consumeNumbers(const XalanDOMChar*&	theString)
{
	while(*theString &&
		  *theString >= XalanUnicode::charDigit_0 &&
		  *theString <= XalanUnicode::charDigit_9)
	{
			++theString;
	}
}



double
doConvert(const XalanDOMChar*	theString)
{
	assert(theString != 0);
	assert(*theString != 0);

	bool	fError = false;
	bool	fGotDecimalPoint = false;
	bool	fGotDigit = false;
	bool	fGotMinus = false;
	bool	fGotWhitespace = false;

	const XalanDOMChar*		theCurrent = theString;

	// trim any whitespace
	consumeWhitespace(theCurrent);

	while(*theCurrent != 0 && fError == false)
	{
		switch(*theCurrent)
		{
		case XalanUnicode::charFullStop:
			if (fGotDecimalPoint == true ||	// can't have more than one...
				fGotWhitespace == true)	// can't have one after whitespace...
			{
				fError = true;
			}
			else
			{
				fGotDecimalPoint = true;

				++theCurrent;
			}
			break;

		case XalanUnicode::charHyphenMinus:
			if (fGotDecimalPoint == true ||
				fGotMinus == true ||
				fGotDigit == true ||
				fGotWhitespace == true)
			{
				// Error -- more than one, or in bad position.
				fError = true;
			}
			else
			{
				fGotMinus = true;

				++theCurrent;
			}
			break;

		case XalanUnicode::charDigit_0:
		case XalanUnicode::charDigit_1:
		case XalanUnicode::charDigit_2:
		case XalanUnicode::charDigit_3:
		case XalanUnicode::charDigit_4:
		case XalanUnicode::charDigit_5:
		case XalanUnicode::charDigit_6:
		case XalanUnicode::charDigit_7:
		case XalanUnicode::charDigit_8:
		case XalanUnicode::charDigit_9:
			if (fGotWhitespace == true)
			{
				fError = true;
			}
			else
			{
				fGotDigit = true;

				consumeNumbers(theCurrent);
			}
			break;

		case XalanUnicode::charSpace:
		case XalanUnicode::charCR:
		case XalanUnicode::charHTab:
		case XalanUnicode::charLF:
			if (fGotWhitespace == true)
			{
				fError = true;
			}
			else
			{
				fGotWhitespace = true;

				consumeWhitespace(theCurrent);
			}
			break;

		default:
			fError = true;
			break;
		}
	}

	if (fError == true || fGotDigit == false)
	{
		return DoubleSupport::getNaN();
	}
	else
	{
		CharVectorType	theVector;

		CopyWideStringToVector(theString, theVector);

		return atof(&theVector.front());
	}
}

#else

// This version is disabled because it turns out that
// an unsigned long is not large enough to accumulate
// all values (duh!).  Perhaps on 64-bit platforms, we
// can use this code, as it's much faster.
void
accumulateNumbers(
			const XalanDOMChar*&	theString,
			double&					theResult,
			bool					fAfterDecimal)
{
	if (fAfterDecimal == false)
	{
		assert(theResult == 0.0);

		// accumulate as an integer, to avoid
		// rounding issues.  It's also much
		// faster...
		double	temp = 0;

		while(*theString &&
			  *theString >= XalanUnicode::charDigit_0 &&
			  *theString <= XalanUnicode::charDigit_9)
		{
			temp *= 10;
			temp += char(*theString) - XalanUnicode::charDigit_0;

			++theString;
		}

		theResult = temp;
	}
	else
	{
		// Accumulate a divisor, so we can divide at the end.
		double	theDivisor = 1;

		// accumulate as an integer, to avoid
		// rounding issues.  It's also much
		// faster...
		unsigned long	temp = 0;

		while(*theString &&
			  *theString >= XalanUnicode::charDigit_0 &&
			  *theString <= XalanUnicode::charDigit_9)
		{
			theDivisor *= 10;

			temp *= 10;
			temp += char(*theString) - XalanUnicode::charDigit_0;

			++theString;
		}

		if (temp > 0 && theDivisor > 1)
		{
			const double	theFactionalPart =
				double(temp) / double(theDivisor);

			theResult += theFactionalPart;
		}
	}
}



double
doConvert(const XalanDOMChar*	theString)
{
	assert(theString != 0);
	assert(*theString != 0);

	double	theResult = 0.0;

	bool	fError = false;
	bool	fGotDecimalPoint = false;
	bool	fGotDigit = false;
	bool	fGotMinus = false;
	bool	fGotWhitespace = false;

	const XalanDOMChar*		theCurrent = theString;

	// trim any whitespace
	consumeWhitespace(theCurrent);

	while(*theCurrent != 0 && fError == false)
	{
		switch(*theCurrent)
		{
		case XalanUnicode::charFullStop:
			if (fGotDecimalPoint == true ||	// can't have more than one...
				fGotWhitespace == true)	// can't have one after whitespace...
			{
				fError = true;
			}
			else
			{
				fGotDecimalPoint = true;

				++theCurrent;
			}
			break;

		case XalanUnicode::charHyphenMinus:
			if (fGotDecimalPoint == true ||
				fGotMinus == true ||
				fGotDigit == true ||
				fGotWhitespace == true)
			{
				// Error -- more than one, or in bad position.
				fError = true;
			}
			else
			{
				fGotMinus = true;

				++theCurrent;
			}
			break;

		case XalanUnicode::charDigit_0:
		case XalanUnicode::charDigit_1:
		case XalanUnicode::charDigit_2:
		case XalanUnicode::charDigit_3:
		case XalanUnicode::charDigit_4:
		case XalanUnicode::charDigit_5:
		case XalanUnicode::charDigit_6:
		case XalanUnicode::charDigit_7:
		case XalanUnicode::charDigit_8:
		case XalanUnicode::charDigit_9:
			if (fGotWhitespace == true)
			{
				fError = true;
			}
			else
			{
				fGotDigit = true;

				accumulateNumbers(theCurrent, theResult, fGotDecimalPoint);
			}
			break;

		case XalanUnicode::charSpace:
		case XalanUnicode::charCR:
		case XalanUnicode::charHTab:
		case XalanUnicode::charLF:
			if (fGotWhitespace == true)
			{
				fError = true;
			}
			else
			{
				fGotWhitespace = true;

				consumeWhitespace(theCurrent);
			}
			break;

		default:
			fError = true;
			break;
		}
	}

	if (fError == true || fGotDigit == false)
	{
		return DoubleSupport::getNaN();
	}
	else
	{
		return fGotMinus == true ? -theResult : theResult;
	}
}

#endif



double
DoubleSupport::toDouble(const XalanDOMChar*		theString)
{
	if (theString == 0 ||
		*theString == 0)
	{
		return getNaN();
	}
	else
	{
		return doConvert(theString);
	}
}
