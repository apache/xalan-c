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

/**
 * This header file defines the C++ classes to implement the Xalan Dates and
 * Times library. Each function inherits from the XalanDateTimeFunction class.
 *
 * Many of these functions use strings in the formats specified by the date and
 * time-related data types defined in [XML Schema Part 2: Datatypes].
 *
 * The following function classes are available in this library:
 * XalanDTAddFunction implementing the date:add() EXSLT function;
 * 
 * XalanDTAddDurationFunction implementing the date:add-duration(...)
 * EXSLT function;
 * 
 * XalanDTDateFunction implementing the date:date(...) EXSLT function;
 * 
 * XalanDTDateTimeFunction implementing the date:date-time() EXSLT function;
 * 
 * XalanDTDayAbbreviationFunction implementing the date:day-abbreviation(...) 
 * EXSLT function;
 * 
 * XalanDTDayInMonthFunction implementing the date:day-in-month(...) EXSLT
 * function;
 * 
 * XalanDTDayInWeekFunction implementing the date:day-in-week(...) EXSLT
 * function;
 * 
 * XalanDTDayInYearFunction implementing the date:day-in-year(...) EXSLT
 * function;
 * 
 * XalanDTDayNameFunction implementing the date:day-name(...) EXSLT function;
 * 
 * XalanDTDayOfWeekInMonthFunction implementing the
 * date:day-of-week-in-month(...) EXSLT function;
 * 
 * XalanDTDifferenceFunction implementing the date:difference(...) EXSLT
 * function;
 * 
 * XalanDTDurationFunction implementing the date:duration(...) EXSLT function;
 * 
 * XalanDTFormatDateFunction implementing the date:format-date(...) EXSLT
 * function;
 * 
 * XalanDTHourInDayFunction implementing the date:hour-in-day(...) EXSLT
 * function;
 * 
 * XalanDTLeapYearFunction implementing the date:leap-year(...) EXSLT function;
 * 
 * XalanDTMinuteInHourFunction implementing the date:minute-in-hour(...) EXSLT
 * function;
 * 
 * XalanDTMonthAbbreviationFunction implementing the
 * date:month-abbreviation(...) EXSLT function;
 * 
 * XalanDTMonthInYearFunction implementing the date:month-in-year(...) EXSLT
 * function;
 * 
 * XalanDTMonthNameFunction implementing the date:month-name(...) EXSLT
 * function;
 * 
 * XalanDTParseDateFunction implementing the date:parse-date(...) EXSLT
 * function;
 * 
 * XalanDTSecondInMinuteFunction implementing the date:second-in-minute(...)
 * EXSLT function;
 * 
 * XalanDTSecondsFunction implementing the date:seconds(...) EXSLT function;
 * 
 * XalanDTSumFunction implementing the date:sum(...) EXSLT function;
 * 
 * XalanDTTimeFunction implementing the date:time(...) EXSLT function;
 * 
 * XalanDTWeekInMonthFunction implementing the date:week-in-month(...) EXSLT
 * function;
 * 
 * XalanDTWeekInYearFunction implementing the date:week-in-year(...) EXSLT
 * function;
 * 
 * XalanDTYearFunction implementing the date:year(...) EXSLT function.
 *
 * All these functions listed above are accurately described at
 *                 http://www.exslt.org/date/index.html
 */

#ifndef _XALAN_DATETIME_FUNCTIONS_H
#define _XALAN_DATETIME_FUNCTIONS_H

#include "XalanDateTimeDefinitons.hpp"

#include "XalanDateTimeFunction.hpp"

XALAN_CPP_NAMESPACE_BEGIN

/**
 * This class inherits from XalanDateTimeFunction but do not implement any
 * function of the Xalan Dates and Times library. It is a base class for 
 * some other classes of this module
 *
 * This class must be the base class for the ones that have a function with one
 * optional argument. To do this, two functions have to be written, the first
 * one with no argument and the second one with one argument. The function with
 * no argument have to give the same return of calling the function with one
 * argument giving the current date/time as argument as returned by the
 * date:date-time() EXSLT function.
 */
class XalanDTOptionalParameterFunction
    : public XalanDateTimeFunction
{

public:

    /**
     * This function calls the execute(...) one giving as argument the current
     * date/time as returned by the date:date-time() EXSLT function.
     *
     * @param context The XPath context passed to the other function.
     *
     * @param locator The locator type passed to the other function.
     *
     * @param executionContext The XPath execution context passed to the other
     *     function.
     */
    XObjectPtr
    execute(
        XPathExecutionContext &	    executionContext,
        XalanNode *	                context,
        const LocatorType *         locator) const;

    virtual XObjectPtr
    execute(
        XPathExecutionContext &	    executionContext,
        XalanNode *	                context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const = 0;

};

/**
 * This class inherits behavior from the XalanDateTimeFunction one and
 * implements the date:add(...) EXSLT function as accurately described at
 *            http://www.exslt.org/date/functions/add/index.html
 */
class XalanDTAddFunction
    : public XalanDateTimeFunction
{

public:
    
    XalanDTAddFunction();

    virtual
    ~XalanDTAddFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg1,
        const XObjectPtr            arg2,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDateTimeFunction one and
 * implements the date:add-duration(...) EXSLT function as accurately described
 * at
 *         http://www.exslt.org/date/functions/add-duration/index.html
 */
class XalanDTAddDurationFunction
    : public XalanDateTimeFunction
{

public:
    
    XalanDTAddDurationFunction();

    virtual
    ~XalanDTAddDurationFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg1,
        const XObjectPtr            arg2,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:date(...) EXSLT function as accurately described at
 *             http://www.exslt.org/date/functions/date/index.html
 */
class XalanDTDateFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTDateFunction();

    virtual
    ~XalanDTDateFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDateTimeFunction one and
 * implements the date:date-time() EXSLT function as accurately described at
 *          http://www.exslt.org/date/functions/date-time/index.html
 */
class XalanDTDateTimeFunction
    : public XalanDateTimeFunction
{

public:
    
    XalanDTDateTimeFunction();

    virtual
    ~XalanDTDateTimeFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:day-abbreviation(...) EXSLT function as accurately
 * described at
 *       http://www.exslt.org/date/functions/day-abbreviation/index.html
 */
class XalanDTDayAbbreviationFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTDayAbbreviationFunction();

    virtual
    ~XalanDTDayAbbreviationFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};


/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:day-in-month(...) EXSLT function as accurately
 * described at
 *        http://www.exslt.org/date/functions/day-in-month/index.html
 */
class XalanDTDayInMonthFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTDayInMonthFunction();

    virtual
    ~XalanDTDayInMonthFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:day-in-week(...) EXSLT function as accurately
 * described at
 *         http://www.exslt.org/date/functions/day-in-week/index.html
 */
class XalanDTDayInWeekFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTDayInWeekFunction();

    virtual
    ~XalanDTDayInWeekFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:day-in-year(...) EXSLT function as accurately
 * described at
 *         http://www.exslt.org/date/functions/day-in-year/index.html
 */
class XalanDTDayInYearFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTDayInYearFunction();

    virtual
    ~XalanDTDayInYearFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:day-name(...) EXSLT function as accurately described
 * at
 *          http://www.exslt.org/date/functions/day-name/index.html
 */
class XalanDTDayNameFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTDayNameFunction();

    virtual
    ~XalanDTDayNameFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:day-of-week-in-month(...) EXSLT function as
 * accurately described at
 *     http://www.exslt.org/date/functions/day-of-week-in-month/index.html
 */
class XalanDTDayOfWeekInMonthFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTDayOfWeekInMonthFunction();

    virtual
    ~XalanDTDayOfWeekInMonthFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDateTimeFunction one and
 * implements the date:difference(...) EXSLT function as accurately described at
 *          http://www.exslt.org/date/functions/difference/index.html
 */
class XalanDTDifferenceFunction
    : public XalanDateTimeFunction
{

public:
    
    XalanDTDifferenceFunction();

    virtual
    ~XalanDTDifferenceFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg1,
        const XObjectPtr            arg2,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDateTimeFunction one and
 * implements the date:duration(...) EXSLT function as accurately described at
 *          http://www.exslt.org/date/functions/duration/index.html
 */
class XalanDTDurationFunction
    : public XalanDateTimeFunction
{

public:
    
    XalanDTDurationFunction();

    virtual
    ~XalanDTDurationFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const LocatorType *         locator) const;

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDateTimeFunction one and
 * implements the date:format-date(...) EXSLT function as accurately described
 * at
 *         http://www.exslt.org/date/functions/format-date/index.html
 */
class XalanDTFormatDateFunction
    : public XalanDateTimeFunction
{

public:
    
    XalanDTFormatDateFunction();

    virtual
    ~XalanDTFormatDateFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg1,
        const XObjectPtr            arg2,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:hour-in-day(...) EXSLT function as accurately
 * described at
 *         http://www.exslt.org/date/functions/hour-in-day/index.html
 */
class XalanDTHourInDayFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTHourInDayFunction();

    virtual
    ~XalanDTHourInDayFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:leap-year(...) EXSLT function as accurately described
 * at
 *          http://www.exslt.org/date/functions/leap-year/index.html
 */
class XalanDTLeapYearFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTLeapYearFunction();

    virtual
    ~XalanDTLeapYearFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:minute-in-hour(...) EXSLT function as accurately
 * described at
 *       http://www.exslt.org/date/functions/minute-in-hour/index.html
 */
class XalanDTMinuteInHourFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTMinuteInHourFunction();

    virtual
    ~XalanDTMinuteInHourFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:month-abbreviation(...) EXSLT function as accurately
 * described at
 *     http://www.exslt.org/date/functions/month-abbreviation/index.html
 */
class XalanDTMonthAbbreviationFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTMonthAbbreviationFunction();

    virtual
    ~XalanDTMonthAbbreviationFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:month-in-year(...) EXSLT function as accurately
 * described at
 *       http://www.exslt.org/date/functions/month-in-year/index.html
 */
class XalanDTMonthInYearFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTMonthInYearFunction();

    virtual
    ~XalanDTMonthInYearFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *	                context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:month-name(...) EXSLT function as accurately
 * described at
 *         http://www.exslt.org/date/functions/month-name/index.html
 */
class XalanDTMonthNameFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTMonthNameFunction();

    virtual
    ~XalanDTMonthNameFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *	                context,
        const XObjectPtr            arg,
        const LocatorType *	        locator) const;

};

/**
 * This class inherits behavior from the XalanDateTimeFunction one and
 * implements the date:parse-date(...) EXSLT function as accurately described at
 *         http://www.exslt.org/date/functions/parse-date/index.html
 */
class XalanDTParseDateFunction
    : public XalanDateTimeFunction
{

public:
    
    XalanDTParseDateFunction();

    virtual
    ~XalanDTParseDateFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *	                context,
        const XObjectPtr            arg1,
        const XObjectPtr            arg2,
        const LocatorType *	        locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:second-in-minute(...) EXSLT function as accurately
 * described at
 *      http://www.exslt.org/date/functions/second-in-minute/index.html
 */
class XalanDTSecondInMinuteFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTSecondInMinuteFunction();

    virtual
    ~XalanDTSecondInMinuteFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *	                context,
        const XObjectPtr            arg,
        const LocatorType *	        locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:seconds(...) EXSLT function as accurately described
 * at
 *          http://www.exslt.org/date/functions/seconds/index.html
 */
class XalanDTSecondsFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTSecondsFunction();

    virtual
    ~XalanDTSecondsFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *	        locator) const;

};

/**
 * This class inherits behavior from the XalanDateTimeFunction one and
 * implements the date:sum(...) EXSLT function as accurately described at
 *            http://www.exslt.org/date/functions/sum/index.html
 */
class XalanDTSumFunction
    : public XalanDateTimeFunction
{

public:
    
    XalanDTSumFunction();

    virtual
    ~XalanDTSumFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &         executionContext,
        XalanNode *                     context,
        const XObjectArgVectorType &    args,
        const LocatorType*              locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:time(...) EXSLT function as accurately described at
 *            http://www.exslt.org/date/functions/time/index.html
 */
class XalanDTTimeFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTTimeFunction();

    virtual
    ~XalanDTTimeFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *	                context,
        const XObjectPtr            arg,
        const LocatorType *	        locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:week-in-month(...) EXSLT function as accurately
 * described at
 *        http://www.exslt.org/date/functions/week-in-month/index.html
 */
class XalanDTWeekInMonthFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTWeekInMonthFunction();

    virtual
    ~XalanDTWeekInMonthFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *	                context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:week-in-year(...) EXSLT function as accurately
 * described at
 *        http://www.exslt.org/date/functions/week-in-year/index.html
 */
class XalanDTWeekInYearFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTWeekInYearFunction();

    virtual
    ~XalanDTWeekInYearFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

};

/**
 * This class inherits behavior from the XalanDTOptionalParameterFunction one
 * and implements the date:year(...) EXSLT function as accurately described at
 *           http://www.exslt.org/date/functions/year/index.html
 */
class XalanDTYearFunction
    : public XalanDTOptionalParameterFunction
{

public:
    
    XalanDTYearFunction();

    virtual
    ~XalanDTYearFunction();

    XObjectPtr
    execute(
        XPathExecutionContext &	    executionContext,
        XalanNode *	                context,
        const XObjectPtr            arg,
        const LocatorType *	        locator) const;

};

XALAN_CPP_NAMESPACE_END

#endif