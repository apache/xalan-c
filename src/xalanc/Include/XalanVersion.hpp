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

#if !defined(XALANVERSION_HEADER_GUARD_1357924680)
#define XALANVERSION_HEADER_GUARD_1357924680

// ---------------------------------------------------------------------------
// X A L A N   V E R S I O N   H E A D E R   D O C U M E N T A T I O N

/**
 * User Documentation for Xalan Version Values:
 *
 *  
 *
 * Xalan  Notes:
 *
 * Xalan Committers Documentation:
 *
 *    Xalan committers normally only need to modify one or two of the 
 *    following macros:
 *
 *      XALAN_VERSION_MAJOR
 *      XALAN_VERSION_MINOR
 *      XALAN_VERSION_REVISION
 *
 *    The integer values of these macros define the Xalan version number. All
 *    other constants and preprocessor macros are automatically generated from
 *    these three definitions.
 *
 * Xalan User Documentation:
 *
 *  The following sections in the user documentation have examples based upon
 *  the following three version input values:
 *
 *    #define XALAN_VERSION_MAJOR 19
 *    #define XALAN_VERSION_MINOR 3
 *    #define XALAN_VERSION_REVISION 74
 *
 *  The minor and revision (patch level) numbers have two digits of resolution
 *  which means that '3' becomes '03' in this example. This policy guarantees
 *  that when using preprocessor macros, version 19.3.74 will be greater than
 *  version 1.94.74 since the first will expand to 190374 and the second to
 *  19474.
 *
 *  Preprocessor Macros:
 *
 *    _XALAN_VERSION defines the primary preprocessor macro that users will
 *    introduce into their code to perform conditional compilation where the
 *    version of Xalan is detected in order to enable or disable version 
 *    specific capabilities. The value of _XALAN_VERSION for the above example
 *    will be 190374. To use it a user would perform an operation such as the
 *    following:
 *
 *      #if _XALAN_VERSION >= 190374
 *        // code specific to new version of Xalan...
 *      #else
 *        // old code here...
 *      #endif
 *
 *    XALAN_FULLVERSIONSTR is a preprocessor macro that expands to a string
 *    constant whose value, for the above example, will be "19_3_74".
 *
 *    XALAN_FULLVERSIONDOT is a preprocessor macro that expands to a string
 *    constant whose value, for the above example, will be "19.3.74".
 *
 *    XALAN_VERSIONSTR is a preprocessor macro that expands to a string
 *    constant whose value, for the above example, will be "19374". This 
 *    particular macro is very dangerous if it were to be used for comparing
 *    version numbers since ordering will not be guaranteed.
 *
 *    Xalan_DLLVersionStr is a preprocessor macro that expands to a string
 *    constant whose value, for the above example, will be "19_3_74". This
 *    macro is provided for backwards compatibility to pre-1.7 versions of
 *    Xalan.
 *
 *  String Constants:
 *
 *    gXalanVersionStr is a global string constant whose value corresponds to
 *    the value "19_3" for the above example.
 *
 *    gXalanFullVersionStr is a global string constant whose value corresponds
 *    to the value "19_3_74" for the above example. 
 *
 *  Numeric Constants:
 *
 *    gXalanMajVersion is a global integer constant whose value corresponds to
 *    the major version number. For the above example its value will be 19.
 *
 *    gXalanMinVersion is a global integer constant whose value corresponds to
 *    the minor version number. For the above example its value will be 3.
 *
 *    gXalanRevision is a global integer constant whose value corresponds to
 *    the revision (patch) version number. For the above example its value will
 *    be 74.
 *
 */

// ---------------------------------------------------------------------------
// X A L A N   V E R S I O N   S P E C I F I C A T I O N

/**
 * MODIFY THESE NUMERIC VALUES TO COINCIDE WITH XALAN VERSION
 * AND DO NOT MODIFY ANYTHING ELSE IN THIS VERSION HEADER FILE
 */

#define XALAN_VERSION_MAJOR 1
#define XALAN_VERSION_MINOR 10
#define XALAN_VERSION_REVISION 0


/** DO NOT MODIFY BELOW THIS LINE */

/**
 * MAGIC THAT AUTOMATICALLY GENERATES THE FOLLOWING:
 *
 *	Xalan_DLLVersionStr, gXalanVersionStr, gXalanFullVersionStr,
 *	gXalanMajVersion, gXalanMinVersion, gXalanRevision
 */

// ---------------------------------------------------------------------------
// T W O   A R G U M E N T   C O N C A T E N A T I O N   M A C R O S

// two argument concatenation routines
#define CAT2_SEP_UNDERSCORE(a, b) #a "_" #b
#define CAT2_SEP_PERIOD(a, b) #a "." #b
#define CAT2_SEP_NIL(a, b) #a #b
#define CAT2_RAW_NUMERIC(a, b) a ## b
#define CAT2_RAW_NUMERIC_SEP_UNDERSCORE(a, b) a ## _ ## b 

// two argument macro invokers
#define INVK_CAT2_SEP_UNDERSCORE(a,b) CAT2_SEP_UNDERSCORE(a,b)
#define INVK_CAT2_SEP_PERIOD(a,b)     CAT2_SEP_PERIOD(a,b)
#define INVK_CAT2_STR_SEP_NIL(a,b)    CAT2_SEP_NIL(a,b)
#define INVK_CAT2_RAW_NUMERIC(a,b)    CAT2_RAW_NUMERIC(a,b)
#define INVK_CAT2_RAW_NUMERIC_SEP_UNDERSCORE(a,b)    CAT2_RAW_NUMERIC_SEP_UNDERSCORE(a,b)

// ---------------------------------------------------------------------------
// T H R E E   A R G U M E N T   C O N C A T E N A T I O N   M A C R O S

// three argument concatenation routines
#define CAT3_SEP_UNDERSCORE(a, b, c) #a "_" #b "_" #c
#define CAT3_SEP_PERIOD(a, b, c) #a "." #b "." #c
#define CAT3_SEP_NIL(a, b, c) #a #b #c
#define CAT3_RAW_NUMERIC(a, b, c) a ## b ## c
#define CAT3_RAW_NUMERIC_SEP_UNDERSCORE(a, b, c) a ## _ ## b ## _ ## c

// three argument macro invokers
#define INVK_CAT3_SEP_UNDERSCORE(a,b,c) CAT3_SEP_UNDERSCORE(a,b,c)
#define INVK_CAT3_SEP_PERIOD(a,b,c)     CAT3_SEP_PERIOD(a,b,c)
#define INVK_CAT3_SEP_NIL(a,b,c)        CAT3_SEP_NIL(a,b,c)
#define INVK_CAT3_RAW_NUMERIC(a,b,c)    CAT3_RAW_NUMERIC(a,b,c)
#define INVK_CAT3_RAW_NUMERIC_SEP_UNDERSCORE(a,b,c)    CAT3_RAW_NUMERIC_SEP_UNDERSCORE(a,b,c)

// ---------------------------------------------------------------------------
// C A L C U L A T E   V E R S I O N   -   E X P A N D E D   F O R M

#define MULTIPLY(factor,value) factor * value
#define CALC_EXPANDED_FORM(a,b,c) ( MULTIPLY(10000,a) + MULTIPLY(100,b) + MULTIPLY(1,c) )

// ---------------------------------------------------------------------------
// X A L A N   V E R S I O N   I N F O R M A T I O N

// Xalan version strings; these particular macros cannot be used for
// conditional compilation as they are not numeric constants

#define XALAN_FULLVERSIONSTR INVK_CAT3_SEP_UNDERSCORE(XALAN_VERSION_MAJOR,XALAN_VERSION_MINOR,XALAN_VERSION_REVISION)
#define XALAN_FULLVERSIONDOT INVK_CAT3_SEP_PERIOD(XALAN_VERSION_MAJOR,XALAN_VERSION_MINOR,XALAN_VERSION_REVISION)
#define XALAN_FULLVERSIONNUM INVK_CAT3_SEP_NIL(XALAN_VERSION_MAJOR,XALAN_VERSION_MINOR,XALAN_VERSION_REVISION)
#define XALAN_VERSIONSTR     INVK_CAT2_SEP_UNDERSCORE(XALAN_VERSION_MAJOR,XALAN_VERSION_MINOR)

// Xalan C++ Namespace string, concatenated with full version string
#define XALAN_PRODUCT xalanc
#define XALAN_CPP_NAMESPACE INVK_CAT3_RAW_NUMERIC_SEP_UNDERSCORE(XALAN_PRODUCT,XALAN_VERSION_MAJOR,XALAN_VERSION_MINOR)


// original from Xalan header
#define Xalan_DLLVersionStr XALAN_FULLVERSIONSTR

const char* const    gXalanVersionStr = XALAN_VERSIONSTR;
const char* const    gXalanFullVersionStr = XALAN_FULLVERSIONSTR;
const unsigned int   gXalanMajVersion = XALAN_VERSION_MAJOR;
const unsigned int   gXalanMinVersion = XALAN_VERSION_MINOR;
const unsigned int   gXalanRevision   = XALAN_VERSION_REVISION;

// Xalan version numeric constants that can be used for conditional
// compilation purposes.

#define _XALAN_VERSION CALC_EXPANDED_FORM (XALAN_VERSION_MAJOR,XALAN_VERSION_MINOR,XALAN_VERSION_REVISION)

#endif // XALANVERSION_HEADER_GUARD_1357924680
