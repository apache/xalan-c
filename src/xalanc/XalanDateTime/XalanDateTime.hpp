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

#ifndef _XALAN_DATETIME_H
#define _XALAN_DATETIME_H

#include "XalanDateTimeDefinitons.hpp"

#include "XalanCalendar.hpp"
#include "XalanDuration.hpp"

#include "XalanDateTimeMath.hpp"

XALAN_CPP_NAMESPACE_BEGIN

enum XalanDateTimeFormat
    {
        TIME,               DATE,                   G_DAY,
        G_YEAR,             G_MONTH,                DATE_TIME,
        G_MONTH_DAY,        G_YEAR_MONTH,           NUM_FORMATS
    };

class XalanDateTime : public XalanCalendar<XalanDateTime>
{

public:

    XalanDateTime(
        long int year,
        long int month,
        long int day,
        long int hours,
        long int minutes,
        double seconds,
        const char * timeZone,
        XalanDateTimeFormat formatType)
        : XalanCalendar(
            year,
            month,
            day,
            hours,
            minutes,
            seconds),
            timeZone(timeZone),
            formatType(formatType)
    {
    }

    inline int getDayInWeek()
    {
        // Jan 1, 2000 was Saturday

        const int baseDay = 1;
        const int baseMonth = 1;
        const int baseYear = 2000;
        const int baseDayOfWeek = 6;

        int nDays = 0;

        bool isAfterOrIsBase = year > baseYear || (year == baseYear && month > baseMonth) ||
            (year == baseYear && month == baseMonth && day > baseDay) ||
            (year == baseYear && month == baseMonth && day == baseDay);

        int cYear = isAfterOrIsBase ? baseYear : baseYear - 1;
        while (cYear != year)
        {
            nDays += cYear ? (XalanDateTimeMath::isLeapYear(cYear) ? 366 : 365) : 0;
            cYear += isAfterOrIsBase ? 1 : -1;
        }
    
        nDays += isAfterOrIsBase ? getDayInYear()
            : (XalanDateTimeMath::isLeapYear(year) ? 366 : 365) - getDayInYear() + 1;
    
        int dayInWeek = isAfterOrIsBase ? (baseDayOfWeek + nDays - 1) % 7 : (((baseDayOfWeek - nDays) % 7) + 7) % 7;

        return dayInWeek + 1;
    }

    inline int getDayOfWeekInMonth()
    {
        return (day / 7) + ((day % 7) ? 1 : 0); 
    }

    inline int getDayInYear()
    {
        int dayInYear = 0;
    
        for (int cMonth = 1; cMonth < month; cMonth++)
        {
            dayInYear += XalanDateTimeMath::maximumDayInMonthFor(year, cMonth);
        }

        return dayInYear + day;
    }

    inline int getWeekInYear()
    {
        return ((getDayInYear() - getDayInWeek() + 5) / 7) + 1;
    }

    inline int getWeekInMonth()
    {
        int monday = day - getDayInWeek() + 2;

        return (monday / 7) + (monday > 0 && monday % 7) ? 2 : 1;
    }

    inline const char * getDayName()
    {
        const char * DAY_NAMES[7] =
        {
            "Sunday",       "Monday",       "Tuesday",
            "Wednesday",    "Thursday",     "Friday",
            "Saturday"
        };

        return DAY_NAMES[getDayInWeek() - 1];
    }

    inline const char * getDayAbbreviation()
    {
        const char * DAY_NAMES_ABBREVIATIONS[7] =
        {
            "Sun",          "Mon",          "Tue",
            "Wed",          "Thu",          "Fri",
            "Sat"
        };

        return DAY_NAMES_ABBREVIATIONS[getDayInWeek() - 1];
    }

    inline const char * getMonthName()
    {
        const char * MONTH_NAMES[12] =
        {
            "January",      "February",     "March",
            "April",        "May",          "June",
            "July",         "August",       "September",
            "October",      "November",     "December"
        };

        return MONTH_NAMES[month - 1];
    }

    inline const char * getMonthAbbreviation()
    {
        const char * MONTH_NAMES_ABBREVIATIONS[12] =
        {
            "Jan",          "Feb",          "Mar",
            "Apr",          "May",          "Jun",
            "Jul",          "Aug",          "Sep",
            "Oct",          "Nov",          "Dec"
        };

        return MONTH_NAMES_ABBREVIATIONS[month - 1];
    }

    inline const char * getTimeZone() const
    {
        return timeZone;
    }

    inline XalanDateTimeFormat getFormatType() const
    {
        return formatType;
    }

    inline void setFormatType(XalanDateTimeFormat newFormatType)
    {
        this->formatType = newFormatType;
    }

    void addDuration(XalanDuration * duration);

    XalanDuration * difference(XalanDateTime * other);

    XalanCalendarIFormatter<XalanDateTime> * getFormatter();

    static XalanDateTime * fromSystem(XalanDateTimeFormat format);

    static XalanDateTime * fromString(const char * dateTime, XalanDateTimeFormat first, ...);

private:

    const char * timeZone;

    XalanDateTimeFormat formatType;
    
};

XALAN_CPP_NAMESPACE_END

#endif