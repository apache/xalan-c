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
#if !defined(DOUBLESUPPORT_HEADER_GUARD_1357924680)
#define DOUBLESUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



class XALAN_PLATFORMSUPPORT_EXPORT DoubleSupport
{
public:

	// Use these functions to determine if a value represents one of these
	// values.  It seems that under the IA32 architecture, NaN will compare
	// as equal to any number, which is a big problem.  Hence these helper
	// functions.

	/**
	 * Determine if target is not a number
	 * 
	 * @param theNumber target number
	 * @return true if target represents the "not a number" value
	 */
	static bool
	isNaN(double	theNumber)
	{
#if !defined(XALAN_NEED_SPECIAL_NAN_SUPPORT)
		return theNumber == s_NaN;
#else
		// Compare the two DWORDs of the double as unsigned longs.
		const unsigned long* const	theFirstDWORD =
			reinterpret_cast<const unsigned long*>(&theNumber);

		const unsigned long* const	theSecondDWORD =
							theFirstDWORD + 1;

		return *theFirstDWORD == *s_NaNFirstDWORD &&
			   *theSecondDWORD == *s_NaNSecondDWORD;
#endif
	}

	/**
	 * Determine if target is positive infinity
	 * 
	 * @param theNumber target number
	 * @return true if target represents the value for positive infinity
	 */
	static bool
	isPositiveInfinity(double	theNumber)
	{
#if !defined(XALAN_NEED_SPECIAL_NAN_SUPPORT)
		return theNumber == s_positiveInfinity;
#else
		return !isNaN(theNumber) && theNumber == s_positiveInfinity;
#endif
	}

	/**
	 * Determine if target is negative infinity
	 * 
	 * @param theNumber target number
	 * @return true if target represents the value for negative infinity
	 */
	static bool
	isNegativeInfinity(double	theNumber)
	{
#if !defined(XALAN_NEED_SPECIAL_NAN_SUPPORT)
		return theNumber == s_negativeInfinity;
#else
		return !isNaN(theNumber) && theNumber == s_negativeInfinity;
#endif
	}

	// These can be used to initialize values, but should not
	// be used to do equality comparisons, as == may fail on
	// some platforms.
	//

	/**
	 * Double value that represents "not a number"
	 * 
	 * @return "not a number" value
	 */
	static double
	getNaN()
	{
		return s_NaN;
	}

	/**
	 * Double value that represents positive infinity
	 * 
	 * @return positive infinity value
	 */
	static double
	getPositiveInfinity()
	{
		return s_positiveInfinity;
	}

	/**
	 * Double value that represents negative infinity
	 * 
	 * @return negative infinity value
	 */
	static double
	getNegativeInfinity()
	{
		return s_negativeInfinity;
	}

private:


	static const double				s_NaN;
	static const double				s_positiveInfinity;
	static const double				s_negativeInfinity;

#if defined(XALAN_NEED_SPECIAL_NAN_SUPPORT)
	static const unsigned long*		s_NaNFirstDWORD;
	static const unsigned long*		s_NaNSecondDWORD;
#endif
};



#endif	// DOUBLESUPPORT_HEADER_GUARD_1357924680
