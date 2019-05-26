/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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
#if !defined(DOUBLESUPPORT_HEADER_GUARD_1357924680)
#define DOUBLESUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#if defined(_MSC_VER)
#include <float.h>
#endif
#include <cmath>
#include <functional>

#if defined(SOLARIS)
#include <math.h>
#endif


#include <xalanc/XalanDOM/XalanDOMString.hpp>



namespace XALAN_CPP_NAMESPACE {



using xercesc::MemoryManager;



// A class to help us support IEEE 754.
class XALAN_PLATFORMSUPPORT_EXPORT DoubleSupport
{
public:

    /**
     * Perform static initialization.  See class PlatformSupportInit.
     *
     */
    static void
    initialize();
 
    /**
     * Perform static shut down.  See class PlatformSupportInit.
     */
    static void
    terminate();


    // Use these functions to determine if a value represents one of these
    // special values.  On some platforms, regular C/C++ operators don't work
    // as we need them too, so we have these helper functions.

    /**
     * Determine if target is not a number
     * 
     * @param theNumber target number
     * @return true if target represents the "not a number" value
     */
    static bool
    isNaN(double    theNumber)
    {
#if defined(_MSC_VER)
        return _isnan(theNumber) != 0;
#elif defined(XALAN_POSIX2_AVAILABLE) && !defined(CYGWIN) && !defined(MINGW)
#if defined(SOLARIS)
        return isnan(theNumber) != 0;
#else
        return std::isnan(theNumber) != 0;
#endif
#else
        return s_NaN == theNumber;
#endif
    }

    /**
     * Determine if target is positive infinity
     * 
     * @param theNumber target number
     * @return true if target represents the value for positive infinity
     */
    static bool
    isPositiveInfinity(double   theNumber)
    {
        return s_positiveInfinity == theNumber;
    }

    /**
     * Determine if target is negative infinity
     * 
     * @param theNumber target number
     * @return true if target represents the value for negative infinity
     */
    static bool
    isNegativeInfinity(double   theNumber)
    {
        return s_negativeInfinity == theNumber;
    }

    /**
     * Determine if target is positive 0.
     * 
     * @param theNumber target number
     * @return true if target represents the value for positive 0.
     */
    static bool
    isPositiveZero(double   theNumber)
    {
        return s_positiveZero == theNumber;
    }

    /**
     * Determine if target is negative 0
     * 
     * @param theNumber target number
     * @return true if target represents the value for negative 0
     */
    static bool
    isNegativeZero(double   theNumber)
    {
        return s_negativeZero == theNumber;
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
        return s_NaN.d;
    }

    /**
     * Double value that represents positive infinity
     * 
     * @return positive infinity value
     */
    static double
    getPositiveInfinity()
    {
        return s_positiveInfinity.d;
    }

    /**
     * Double value that represents negative infinity
     * 
     * @return negative infinity value
     */
    static double
    getNegativeInfinity()
    {
        return s_negativeInfinity.d;
    }

    /**
     * Compare two double values, taking into account
     * the fact that we must support IEEE 754
     *
     * @param theLHS a number to compare
     * @param theRHS a number to compare
     * @return the result of the compare
     */
    static bool
    equal(
            double  theLHS,
            double  theRHS);

    /**
     * Compare two double values, taking into account
     * the fact that we must support IEEE 754
     *
     * @param theLHS a number to compare
     * @param theRHS a number to compare
     * @return the result of the compare
     */
    static bool
    notEqual(
            double  theLHS,
            double  theRHS)
    {
        return !equal(theLHS, theRHS);
    }

    /**
     * Compare two double values, taking into account
     * the fact that we must support IEEE 754
     *
     * @param theLHS a number to compare
     * @param theRHS a number to compare
     * @return the result of the compare
     */
    static bool
    lessThan(
            double  theLHS,
            double  theRHS);

    /**
     * Compare two double values, taking into account
     * the fact that we must support IEEE 754
     *
     * @param theLHS a number to compare
     * @param theRHS a number to compare
     * @return the result of the compare
     */
    static bool
    lessThanOrEqual(
            double  theLHS,
            double  theRHS);

    /**
     * Compare two double values, taking into account
     * the fact that we must support IEEE 754
     *
     * @param theLHS a number to compare
     * @param theRHS a number to compare
     * @return the result of the compare
     */
    static bool
    greaterThan(
            double  theLHS,
            double  theRHS);

    /**
     * Compare two double values, taking into account
     * the fact that we must support IEEE 754
     *
     * @param theLHS a number to compare
     * @param theRHS a number to compare
     * @return the result of the compare
     */
    static bool
    greaterThanOrEqual(
            double  theLHS,
            double  theRHS);

    /**
     * Add two double values, taking into account
     * the fact that we must support IEEE 754
     *
     * @param theLHS a number to add
     * @param theRHS a number to add
     * @return the result of the addition
     */
    static double
    add(
            double  theLHS,
            double  theRHS);

    /**
     * Subtract two double values, taking into account
     * the fact that we must support IEEE 754
     *
     * @param theLHS a number to subtract
     * @param theRHS a number to subtract
     * @return the result of the subtraction
     */
    static double
    subtract(
            double  theLHS,
            double  theRHS);

    /**
     * Multiply two double values, taking into account
     * the fact that we must support IEEE 754
     *
     * @param theLHS a number to multiply
     * @param theRHS a number to multiply
     * @return the result of the multiplication
     */
    static double
    multiply(
            double  theLHS,
            double  theRHS);

    /**
     * Divide two double values, taking into account
     * the fact that we must support IEEE 754
     *
     * @param theLHS a number to divide
     * @param theRHS a number to divide
     * @return the result of the division
     */
    static double
    divide(
            double  theLHS,
            double  theRHS);

    /**
     * Determine the modulus two double values,
     * taking into account the fact that we must
     * support IEEE 754
     *
     * @param theLHS a number to divide
     * @param theRHS a number to divide
     * @return the result of the modulus
     */
    static double
    modulus(
            double  theLHS,
            double  theRHS);

    /**
     * Determine the negative of a double value,
     * taking into account the fact that we must
     * support IEEE 754
     *
     * @param theDouble a number to negate
     * @return the result of the negation
     */
    static double
    negative(double theDouble);

    /**
     * Return  the absolute value of theDouble. If theDouble is NaN,
     * NaN is returned 
     *
     * @param theDouble a number to fabs
     * @return the result of the fabs
     */
    static double
    abs(double theDouble);

    // Some functors to do the same thing.  This is for
    // STL integration...
    struct equalFunction
    {
        bool
        operator()(
            const double&    theLHS,
            const double&    theRHS) const
        {
            return equal(theLHS, theRHS);
        }
    };

    struct notEqualFunction
    {
        bool
        operator()(
            const double&    theLHS,
            const double&    theRHS) const
        {
            return notEqual(theLHS, theRHS);
        }
    };

    struct lessThanFunction
    {
        bool
        operator()(
            const double&    theLHS,
            const double&    theRHS) const
        {
            return lessThan(theLHS, theRHS);
        }
    };

    struct lessThanOrEqualFunction
    {
        bool
        operator()(
            const double&    theLHS,
            const double&    theRHS) const
        {
            return lessThanOrEqual(theLHS, theRHS);
        }
    };

    struct greaterThanFunction
    {
        bool
        operator()(
            const double&    theLHS,
            const double&    theRHS) const
        {
            return greaterThan(theLHS, theRHS);
        }
    };

    struct greaterThanOrEqualFunction
    {
        bool
        operator()(
            const double&    theLHS,
            const double&    theRHS) const
        {
            return greaterThanOrEqual(theLHS, theRHS);
        }
    };

    struct addFunction
    {
        double
        operator()(
            const double&    theLHS,
            const double&    theRHS) const
        {
            return add(theLHS, theRHS);
        }
    };

    struct subtractFunction
    {
        double
        operator()(
            const double&    theLHS,
            const double&    theRHS) const
        {
            return subtract(theLHS, theRHS);
        }
    };

    struct multiplyFunction
    {
        double
        operator()(
            const double&    theLHS,
            const double&    theRHS) const
        {
            return multiply(theLHS, theRHS);
        }
    };

    struct divideFunction
    {
        double
        operator()(
            const double&    theLHS,
            const double&    theRHS) const
        {
            return divide(theLHS, theRHS);
        }
    };

    struct modulusFunction
    {
        double
        operator()(
            const double&    theLHS,
            const double&    theRHS) const
        {
            return modulus(theLHS, theRHS);
        }
    };

    struct negativeFunction
    {
        double
        operator()(const double&        theDouble) const
        {
            return negative(theDouble);
        }
    };

    /**
     * Determine whether or not a string contains
     * a valid floating point number.
     *
     * @param theString The string to check.
     * @return true if the string is valid, false if not.
     */
    static bool
    isValid(const XalanDOMString&   theString);

    /**
     * Determine whether or not a string contains
     * a valid floating point number.
     *
     * @param theString The string to check.
     * @return true if the string is valid, false if not.
     */
    static bool
    isValid(const XalanDOMChar*     theString);

    /**
     * Convert a string to a double value.  Returns
     * NaN if the string is not a valid floating
     * point number.
     *
     * @param theString The string to convert.
     * @param theManager The MemoryManager instance to use.
     * @return The result of the conversion
     */
    static double
    toDouble(
            const XalanDOMString&   theString,
            MemoryManager&          theManager);

    /**
     * Convert a string to a double value.  Returns
     * NaN if the string is not a valid floating
     * point number.
     *
     * @param theString The string to convert.
     * @param theManager The MemoryManager instance to use.
     * @return The result of the conversion
     */
    static double
    toDouble(
            const XalanDOMChar*     theString,
            MemoryManager&          theManager);

    /**
     * Round a number according to the XPath
     * rules.
     *
     * @param theValue The value to round.
     * @return The result of the rounding
     */
    static double
    round(double    theValue);

    /**
     * Returns the ceiling of a number according to the XPath
     * rules.
     *
     * @param theValue The value to round.
     * @return The result of the rounding
     */
    static double
    ceiling(double  theValue)
    {
        return std::ceil(theValue);
    }

    /**
     * Returns the floor of a number according to the XPath
     * rules.
     *
     * @param theValue The value to round.
     * @return The result of the rounding
     */
    static double
    floor(double    theValue)
    {
        return std::floor(theValue);
    }

    union NumberUnion
    {
        double  d;
        struct
        {
            unsigned int    dw1;
            unsigned int    dw2;
        } dwords;

        bool
        operator==(double   theNumber) const
        {
            const NumberUnion   temp = { theNumber };

            return dwords.dw1 == temp.dwords.dw1 &&
                   dwords.dw2 == temp.dwords.dw2;
        }
    };

private:

    static const NumberUnion    s_NaN;

    static const NumberUnion    s_positiveInfinity;
    static const NumberUnion    s_negativeInfinity;
    static const NumberUnion    s_positiveZero;
    static const NumberUnion    s_negativeZero;
};



}



#endif  // DOUBLESUPPORT_HEADER_GUARD_1357924680
