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

#include "XalanDateTime.hpp"
#include "XalanDuration.hpp"

#include "XalanDateTimeMath.hpp"
#include "XalanDateTimeFunctions.hpp"
#include "XalanDateTimeExceptions.hpp"

XALAN_CPP_NAMESPACE_USE

XObjectPtr
XalanDTOptionalParameterFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const LocatorType *         locator) const
{
    XalanDOMString theArgument(XalanDateTime::fromSystem(DATE_TIME)->toString());

    return execute(executionContext,
        context,
        executionContext.getXObjectFactory().createString(theArgument),
        locator);
}

XalanDTAddFunction::XalanDTAddFunction()
    : XalanDateTimeFunction()
{
}

XalanDTAddFunction::~XalanDTAddFunction()
{
}

XObjectPtr
XalanDTAddFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg1,
    const XObjectPtr            arg2,
    const LocatorType *         locator) const
{
    XalanDateTime * dateTime;
    
    try
    {
        dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg1->str()).begin(),
            DATE_TIME,      DATE,          G_YEAR_MONTH,      G_YEAR);
    }
    catch (XalanInvalidDateTime & xidt)
    {
        // TODO Report error
    }

    XalanDuration * duration;

    try
    {
        duration = XalanDuration::fromString(
            TranscodeToLocalCodePage(arg2->str()).begin());
    }
    catch (XalanInvalidDuration & xid)
    {
        // TODO Report error
    }

    dateTime->addDuration(duration);

    XalanDOMString theResult(dateTime->toString());

    delete dateTime;
    delete duration;

    return executionContext.getXObjectFactory().createString(theResult);
}

XalanDTAddDurationFunction::XalanDTAddDurationFunction()
    : XalanDateTimeFunction()
{
}

XalanDTAddDurationFunction::~XalanDTAddDurationFunction()
{
}

XObjectPtr
XalanDTAddDurationFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg1,
    const XObjectPtr            arg2,
    const LocatorType *         locator) const
{
    XalanDuration * fDuration;
    XalanDuration * sDuration;

    char * rDuration;
    try
    {
        fDuration = XalanDuration::fromString(
            TranscodeToLocalCodePage(arg1->str()).begin());

        sDuration = XalanDuration::fromString(
            TranscodeToLocalCodePage(arg2->str()).begin());

        fDuration->add(sDuration);

        rDuration = fDuration->toString();
    }
    catch (XalanInvalidDuration & xid)
    {
        rDuration = "";
    }

    XalanDOMString theResult(rDuration);

    delete fDuration;
    delete sDuration;

    return executionContext.getXObjectFactory().createString(theResult);
}

XalanDTDateFunction::XalanDTDateFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTDateFunction::~XalanDTDateFunction()
{
}

XObjectPtr
XalanDTDateFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    char * rDate;
    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  DATE);

        dateTime->setFormatType(DATE);

        rDate = dateTime->toString();

        delete dateTime;
    }
    catch (XalanInvalidDuration & xid)
    {
        rDate = "";
    }

    XalanDOMString theResult(rDate);

    return executionContext.getXObjectFactory().createString(theResult);
}

XalanDTDateTimeFunction::XalanDTDateTimeFunction()
    : XalanDateTimeFunction()
{
}

XalanDTDateTimeFunction::~XalanDTDateTimeFunction()
{
}

XObjectPtr
XalanDTDateTimeFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const LocatorType *         locator) const
{
    XalanDOMString theResult(XalanDateTime::fromSystem(DATE_TIME)->toString());

    return executionContext.getXObjectFactory().createString(theResult);
}

XalanDTDayAbbreviationFunction::XalanDTDayAbbreviationFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTDayAbbreviationFunction::~XalanDTDayAbbreviationFunction()
{
}

XObjectPtr
XalanDTDayAbbreviationFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    const char * rDayAbbreviation;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  DATE);

        rDayAbbreviation = dateTime->getDayAbbreviation();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        rDayAbbreviation = "";
    }

    XalanDOMString theResult(rDayAbbreviation);

    return executionContext.getXObjectFactory().createString(theResult);
}

XalanDTDayInMonthFunction::XalanDTDayInMonthFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTDayInMonthFunction::~XalanDTDayInMonthFunction()
{
}

XObjectPtr
XalanDTDayInMonthFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    double dayInMonth = 0.0;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,      DATE,       G_MONTH_DAY,    G_DAY);

        dayInMonth = dateTime->getDay();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        dayInMonth = XalanDateTimeMath::NAN;
    }

    return executionContext.getXObjectFactory().createNumber(dayInMonth);
}

XalanDTDayInWeekFunction::XalanDTDayInWeekFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTDayInWeekFunction::~XalanDTDayInWeekFunction()
{
}

XObjectPtr
XalanDTDayInWeekFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    double dayInWeek = 0.0;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  DATE);

        dayInWeek = dateTime->getDayInWeek();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        dayInWeek = XalanDateTimeMath::NAN;
    }

    return executionContext.getXObjectFactory().createNumber(dayInWeek);
}

XalanDTDayInYearFunction::XalanDTDayInYearFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTDayInYearFunction::~XalanDTDayInYearFunction()
{
}

XObjectPtr
XalanDTDayInYearFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    double dayInYear = 0.0;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  DATE);

        dayInYear = dateTime->getDayInYear();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        dayInYear = XalanDateTimeMath::NAN;
    }

    return executionContext.getXObjectFactory().createNumber(dayInYear);
}

XalanDTDayNameFunction::XalanDTDayNameFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTDayNameFunction::~XalanDTDayNameFunction()
{
}

XObjectPtr
XalanDTDayNameFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    const char * dayName;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  DATE);

        dayName = dateTime->getDayName();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        dayName = "";
    }

    XalanDOMString theResult(dayName);

    return executionContext.getXObjectFactory().createString(theResult);
}

XalanDTDayOfWeekInMonthFunction::XalanDTDayOfWeekInMonthFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTDayOfWeekInMonthFunction::~XalanDTDayOfWeekInMonthFunction()
{
}

XObjectPtr
XalanDTDayOfWeekInMonthFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    double dayOfWeekInMonth = 0.0;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  DATE);

        dayOfWeekInMonth = dateTime->getDayOfWeekInMonth();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        dayOfWeekInMonth = XalanDateTimeMath::NAN;
    }

    return executionContext.getXObjectFactory().createNumber(dayOfWeekInMonth);
}

XalanDTDifferenceFunction::XalanDTDifferenceFunction()
    : XalanDateTimeFunction()
{
}

XalanDTDifferenceFunction::~XalanDTDifferenceFunction()
{
}

XObjectPtr
XalanDTDifferenceFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg1,
    const XObjectPtr            arg2,
    const LocatorType *         locator) const
{
    XalanDateTime * fDateTime;
    XalanDateTime * sDateTime;

    char * rDuration;
    try
    {
        fDateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg1->str()).begin(),
            DATE_TIME,      DATE,           G_YEAR_MONTH,   G_YEAR);

        sDateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg2->str()).begin(),
            DATE_TIME,      DATE,           G_YEAR_MONTH,   G_YEAR);

        XalanDuration * duration = fDateTime->difference(sDateTime);

        duration->setFormatType(
            (fDateTime->getFormatType() == G_YEAR_MONTH
                || sDateTime->getFormatType() == G_YEAR_MONTH
                || fDateTime->getFormatType() == G_YEAR
                || sDateTime->getFormatType() == G_YEAR)
            ? YEAR_MONTH : DAY_AND_TIME);

        duration->normalizeAttributes();

        rDuration = duration->toString();

        delete duration;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        rDuration = "";
    }

    delete fDateTime;
    delete sDateTime;

    XalanDOMString theResult(rDuration);

    return executionContext.getXObjectFactory().createString(theResult);
}

XalanDTDurationFunction::XalanDTDurationFunction()
    : XalanDateTimeFunction()
{
}

XalanDTDurationFunction::~XalanDTDurationFunction()
{
}

XObjectPtr
XalanDTDurationFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const LocatorType *         locator) const
{
    XalanDTSecondsFunction * dtSecondsFunction = new XalanDTSecondsFunction();
    
    XObjectPtr secondsObject = ((XalanDTOptionalParameterFunction *) dtSecondsFunction)->execute(
            executionContext,
            context,
            locator);

    delete dtSecondsFunction;

    return XalanDTDurationFunction::execute(
        executionContext,
        context,
        secondsObject,
        locator);
}

XObjectPtr
XalanDTDurationFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    XalanDuration * duration;

    char * rDuration;
    try
    {
        duration = new XalanDuration(0, 0, 0, 0, 0, arg->num());

        duration->setFormatType(DAY_AND_TIME);

        duration->normalizeAttributes();

        rDuration = duration->toString();

        delete duration;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        rDuration = "";
    }

    delete duration;

    XalanDOMString theResult(rDuration);

    return executionContext.getXObjectFactory().createString(theResult);
}

XalanDTFormatDateFunction::XalanDTFormatDateFunction()
    : XalanDateTimeFunction()
{
}

XalanDTFormatDateFunction::~XalanDTFormatDateFunction()
{
}

XObjectPtr
XalanDTFormatDateFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg1,
    const XObjectPtr            arg2,
    const LocatorType *         locator) const
{
    return XObjectPtr(0);
}

XalanDTHourInDayFunction::XalanDTHourInDayFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTHourInDayFunction::~XalanDTHourInDayFunction()
{
}

XObjectPtr
XalanDTHourInDayFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    double hourInDay = 0.0;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  TIME);

        hourInDay = dateTime->getHours();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        hourInDay = XalanDateTimeMath::NAN;
    }

    return executionContext.getXObjectFactory().createNumber(hourInDay);
}

XalanDTLeapYearFunction::XalanDTLeapYearFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTLeapYearFunction::~XalanDTLeapYearFunction()
{
}

XObjectPtr
XalanDTLeapYearFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    bool leapYear = false;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,      DATE,           G_YEAR_MONTH,       G_YEAR);

        leapYear = XalanDateTimeMath::isLeapYear(dateTime->getYear());

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        return executionContext.getXObjectFactory().createNumber(XalanDateTimeMath::NAN);
    }

    return executionContext.getXObjectFactory().createBoolean(leapYear);
}

XalanDTMinuteInHourFunction::XalanDTMinuteInHourFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTMinuteInHourFunction::~XalanDTMinuteInHourFunction()
{
}

XObjectPtr
XalanDTMinuteInHourFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    double minuteInHour = 0.0;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  TIME);

        minuteInHour = dateTime->getMinutes();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        minuteInHour = XalanDateTimeMath::NAN;
    }

    return executionContext.getXObjectFactory().createNumber(minuteInHour);
}

XalanDTMonthAbbreviationFunction::XalanDTMonthAbbreviationFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTMonthAbbreviationFunction::~XalanDTMonthAbbreviationFunction()
{
}

XObjectPtr
XalanDTMonthAbbreviationFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    const char * monthAbbreviation;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,      DATE,           G_YEAR_MONTH,       G_MONTH);

        monthAbbreviation = dateTime->getMonthAbbreviation();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        monthAbbreviation = "";
    }

    XalanDOMString theResult(monthAbbreviation);

    return executionContext.getXObjectFactory().createString(theResult);
}

XalanDTMonthInYearFunction::XalanDTMonthInYearFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTMonthInYearFunction::~XalanDTMonthInYearFunction()
{
}

XObjectPtr
XalanDTMonthInYearFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    double monthInYear = 0.0;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,              DATE,               G_YEAR_MONTH,
            G_MONTH,                G_MONTH_DAY);

        monthInYear = dateTime->getMonth();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        monthInYear = XalanDateTimeMath::NAN;
    }

    return executionContext.getXObjectFactory().createNumber(monthInYear);
}

XalanDTMonthNameFunction::XalanDTMonthNameFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTMonthNameFunction::~XalanDTMonthNameFunction()
{
}

XObjectPtr
XalanDTMonthNameFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    const char * monthName;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,          DATE,               G_YEAR_MONTH,
            G_MONTH);

        monthName = dateTime->getMonthName();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        monthName = "";
    }

    XalanDOMString theResult(monthName);

    return executionContext.getXObjectFactory().createString(theResult);
}

XalanDTParseDateFunction::XalanDTParseDateFunction()
    : XalanDateTimeFunction()
{
}

XalanDTParseDateFunction::~XalanDTParseDateFunction()
{
}

XObjectPtr
XalanDTParseDateFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg1,
    const XObjectPtr            arg2,
    const LocatorType *         locator) const
{
    return XObjectPtr(0);
}

XalanDTSecondInMinuteFunction::XalanDTSecondInMinuteFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTSecondInMinuteFunction::~XalanDTSecondInMinuteFunction()
{
}

XObjectPtr
XalanDTSecondInMinuteFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    double secondInMinute = 0.0;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  TIME);

        secondInMinute = dateTime->getSeconds();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        secondInMinute = XalanDateTimeMath::NAN;
    }

    return executionContext.getXObjectFactory().createNumber(secondInMinute);
}

XalanDTSecondsFunction::XalanDTSecondsFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTSecondsFunction::~XalanDTSecondsFunction()
{
}

XObjectPtr
XalanDTSecondsFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    double seconds = 0.0;

    XalanDuration * duration;

    XalanDateTime * dateTime;
    XalanDateTime * baseDateTime;

    try
    {
        dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  DATE, G_YEAR_MONTH, G_YEAR);

        baseDateTime = XalanDateTime::fromString(
            "1970-01-01T00:00:00Z", DATE_TIME);

        duration = dateTime->difference(baseDateTime);
    }
    catch (XalanInvalidDateTime & xidt)
    {
        try
        {
            duration = XalanDuration::fromString(
                TranscodeToLocalCodePage(arg->str()).begin());

            if (duration->getYear() + duration->getMonth())
            {
                throw XalanInvalidDuration();
            }
        }
        catch (XalanInvalidDuration & xid)
        {
            return executionContext.getXObjectFactory().
                createNumber(XalanDateTimeMath::NAN);
        }
    }

    seconds = duration->getTotalOfSeconds();

    delete duration;
    delete dateTime;
    delete baseDateTime;

    return executionContext.getXObjectFactory().createNumber(seconds);
}


XalanDTSumFunction::XalanDTSumFunction()
    : XalanDateTimeFunction()
{
}

XalanDTSumFunction::~XalanDTSumFunction()
{
}

XObjectPtr
XalanDTSumFunction::execute(
        XPathExecutionContext &         executionContext,
        XalanNode *                     context,
        const XObjectArgVectorType &    args,
        const LocatorType*              locator) const
{
    const XObjectArgVectorType::size_type   theArgCount = args.size();

    char * rDuration;
    if (theArgCount != 0)
    {
        XalanDuration * fDuration;
        XalanDuration * sDuration;

        try
        {
            fDuration = XalanDuration::fromString(
                TranscodeToLocalCodePage(args[0]->str()).begin());

            for (int iArg = 1; iArg < theArgCount; iArg ++)
            {
                fDuration->add(sDuration = XalanDuration::fromString(
                    TranscodeToLocalCodePage(args[iArg]->str()).begin()));

                delete sDuration;
            }

            rDuration = fDuration->toString();
        }
        catch (XalanInvalidDuration & xid)
        {
            rDuration = "";
        }

        delete fDuration;
        delete sDuration;
    }
    else
    {
        rDuration = "";
    }

    XalanDOMString theResult(rDuration);

    return executionContext.getXObjectFactory().createString(theResult);
}

XalanDTTimeFunction::XalanDTTimeFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTTimeFunction::~XalanDTTimeFunction()
{
}

XObjectPtr
XalanDTTimeFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    char * rTime;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  TIME);

        dateTime->setFormatType(TIME);

        rTime = dateTime->toString();

        delete dateTime;
    }
    catch (XalanInvalidDuration & xid)
    {
        rTime = "";
    }

    XalanDOMString theResult(rTime);

    return executionContext.getXObjectFactory().createString(theResult);
}

XalanDTWeekInMonthFunction::XalanDTWeekInMonthFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTWeekInMonthFunction::~XalanDTWeekInMonthFunction()
{
}

XObjectPtr
XalanDTWeekInMonthFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    double weekInMonth = 0.0;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  DATE);

        weekInMonth = dateTime->getWeekInMonth();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        weekInMonth = XalanDateTimeMath::NAN;
    }

    return executionContext.getXObjectFactory().createNumber(weekInMonth);
}

XalanDTWeekInYearFunction::XalanDTWeekInYearFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTWeekInYearFunction::~XalanDTWeekInYearFunction()
{
}

XObjectPtr
XalanDTWeekInYearFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    double weekInYear = 0.0;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,  DATE);

        weekInYear = dateTime->getWeekInYear();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        weekInYear = XalanDateTimeMath::NAN;
    }

    return executionContext.getXObjectFactory().createNumber(weekInYear);
}

XalanDTYearFunction::XalanDTYearFunction()
    : XalanDTOptionalParameterFunction()
{
}

XalanDTYearFunction::~XalanDTYearFunction()
{
}

XObjectPtr
XalanDTYearFunction::execute(
    XPathExecutionContext &	    executionContext,
    XalanNode *	                context,
    const XObjectPtr            arg,
    const LocatorType *         locator) const
{
    double year = 0.0;

    try
    {
        XalanDateTime * dateTime = XalanDateTime::fromString(
            TranscodeToLocalCodePage(arg->str()).begin(),
            DATE_TIME,      DATE,       G_YEAR_MONTH,    G_YEAR);

        year = dateTime->getYear();

        delete dateTime;
    }
    catch (XalanInvalidDateTime & xidt)
    {
        year = XalanDateTimeMath::NAN;
    }

    return executionContext.getXObjectFactory().createNumber(year);
}