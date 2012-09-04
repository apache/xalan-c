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

#ifndef _XALAN_DATETIME_FORMATTER_H
#define _XALAN_DATETIME_FORMATTER_H

#include "XalanDateTimeDefinitons.hpp"

#include "XalanDateTime.hpp"
#include "XalanDateTimeMath.hpp"
#include "XalanCalendarFormatter.hpp"

XALAN_CPP_NAMESPACE_BEGIN

template <typename T> class XalanDateTimeFormatter
    : public XalanCalendarFormatter<T>,
      public XalanCalendarIFormatter<XalanDateTime>
{

public:

    enum DateTimeSymbols
    {
        DTS_SYM   = '-',
        TMS_SYM   = ':',
        UTC_TMZN_SYM  = 'Z'
    };

protected:

    long int getYear(const char ** dateTime)
    {
        bool negative = (**dateTime == '-') && ++(*dateTime);
        
        int nLength;
        long double ten = 10;
        getNumberInfo(*dateTime, &nLength);
        long int year = getInteger(dateTime, 4, true, 1);
        if (nLength > 4 && (year < pow((double) 10, (double) (nLength -1))))
        {
            throw XalanInvalidDateTime("");
        }

        return negative
            ? -year
            :  year;
    }

    long int getMonth(const char ** dateTime)
    {
        return getInteger(dateTime, 2, false, 1, 12);
    }

    long int getDay(const char ** dateTime)
    {
        return getInteger(dateTime, 2, false, 1, 31);
    }

    long int getHours(const char ** dateTime)
    {
        return getInteger(dateTime, 2, false, 0, 24);
    }

    long int getMinutes(const char ** dateTime)
    {
        return getInteger(dateTime, 2, false, 0, 59);
    }

    double getSeconds(const char ** dateTime)
    {
        return getDouble(dateTime, 2, 0.0, 59.999);
    }

    void checkDay(long int year, long int month, long int day)
    {
        if (day > XalanDateTimeMath::maximumDayInMonthFor(year, month))
        {
            throw XalanInvalidDateTime("");
        }
    }

    void checkTime(long int hours, long int minutes, double seconds)
    {
        if (hours == 24 && (minutes + seconds))
        {
            throw XalanInvalidDateTime("");
        }
    }

    void checkSeparator(const char ** dateTime, const char separator)
    {
        checkChar(dateTime, separator, true);
    }

    const char * getTimeZone(const char ** dateTime)
    {
        const char * timeZone = *dateTime;

        long int hours = 0;
        long int minutes = 0;
        if (**dateTime && !checkChar(dateTime, 'Z', false))
        {
            if (!checkChar(dateTime, '+', false))
            {
                checkChar(dateTime, '-', true);
            }

            hours = getHours(dateTime);
            checkSeparator(dateTime, ':');
            minutes = getMinutes(dateTime);
        }

        if (**dateTime
            || hours > 14
            || (hours == 14 && minutes))
        {
            throw XalanInvalidDateTime("");
        }

        return timeZone;
    }

    char * getFormattedSeconds(double seconds)
    {
        char * formattedNumber = new char [12];
        sprintf(formattedNumber, "%02g", seconds);

        int nLength;
        int pointAt = getNumberInfo(formattedNumber, &nLength);

        char * format = new char[5];
        sprintf(format, "%%0%dg", pointAt ? (pointAt != 2 ? nLength : nLength + 1) : 2);

        sprintf(formattedNumber, format, seconds);

        return formattedNumber;
    }

private:

    long int getInteger(const char ** pInfo, int eLength, bool maybeLarger, int lowerBound, int upperBound = 2147483647)
    {
        long int integer = XalanCalendarFormatter::getInteger(pInfo, eLength, maybeLarger);

        if (integer < lowerBound || integer > upperBound)
        {
            throw XalanInvalidCalendar("");
        }

        return integer;
    }

    double getDouble(const char ** pInfo, int eLength, double lowerBound, double upperBound)
    {
        double dbl = XalanCalendarFormatter::getDouble(pInfo, eLength);

         if (dbl < lowerBound || dbl > upperBound)
        {
            throw XalanInvalidCalendar("");
        }

        return dbl;
    }

};

class DateTimeFormatterDelegator : public XalanDateTimeFormatter<DateTimeFormatterDelegator>
{

public:

    char * createString(XalanDateTime * dateTime);

    XalanDateTime * createInstance(const char * dateTime);

    XalanDateTime * createInstance(const char * dateTime, XalanDateTimeFormat first, ...);

private:

    XalanCalendarIFormatter<XalanDateTime> * getFormatter(XalanDateTimeFormat format);

};

class TimeFormatter : public XalanDateTimeFormatter<TimeFormatter>
{
public:
    char * createString(XalanDateTime * time);

    XalanDateTime * createInstance(const char * time);

};

class DateFormatter : public XalanDateTimeFormatter<DateFormatter>
{
public:

    char * createString(XalanDateTime * date);

    XalanDateTime * createInstance(const char * date);

};

class GDayFormatter : public XalanDateTimeFormatter<GDayFormatter>
{
public:

    char * createString(XalanDateTime * gDay);

    XalanDateTime * createInstance(const char * gDay);

};

class GYearFormatter : public XalanDateTimeFormatter<GYearFormatter>
{
public:

    char * createString(XalanDateTime * gYear);

    XalanDateTime * createInstance(const char * gYear);
    
};

class GMonthFormatter : public XalanDateTimeFormatter<GMonthFormatter>
{
public:

    char * createString(XalanDateTime * gMonth);

    XalanDateTime * createInstance(const char * gMonth);
        
};

class DateTimeFormatter : public XalanDateTimeFormatter<DateTimeFormatter>
{
public:

    char * createString(XalanDateTime * dateTime);

    XalanDateTime * createInstance(const char * dateTime);
    
};

class GMonthDayFormatter : public XalanDateTimeFormatter<GMonthDayFormatter>
{
public:

    char * createString(XalanDateTime * gMonthDay);

    XalanDateTime * createInstance(const char * gMonthDay);

};

class GYearMonthFormatter : public XalanDateTimeFormatter<GYearMonthFormatter>
{
public:

    char * createString(XalanDateTime * gYearMonth);

    XalanDateTime * createInstance(const char * gYearMonth);

};

XALAN_CPP_NAMESPACE_END

#endif _XALANDATETIMEFORMATTER_H