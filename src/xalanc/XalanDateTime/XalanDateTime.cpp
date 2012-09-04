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

#include <stdarg.h>

#include "XalanDateTime.hpp"
#include "XalanSystemClock.hpp"
#include "XalanDateTimeFormatter.hpp"

XALAN_CPP_NAMESPACE_USE

XalanCalendarIFormatter<XalanDateTime> * XalanDateTime::getFormatter()
{
    return DateTimeFormatterDelegator::getInstance();
}

void XalanDateTime::addDuration(
    XalanDuration * duration)
{
    // Temporary variables declarations
    double tDouble;
    long int tInteger, tCarry;

    // Resultant variables declarations
    double rSeconds;
    long int rYear, rMonth, rDay, rHours, rMinutes;

    // Calculating the month
    tInteger = this->month + duration->getMonth();
    rMonth = XalanDateTimeMath::modulo(tInteger, 1, 13);
    tCarry = XalanDateTimeMath::fQuotient(tInteger, 1, 13);

    // Calculating the year
    rYear = this->year + duration->getYear() + tCarry;

    // Calculating the seconds
    tDouble = this->seconds + duration->getSeconds();
    rSeconds = XalanDateTimeMath::modulo(tDouble, 60);
    tCarry = XalanDateTimeMath::fQuotient(tDouble, 60);

    // Calculating the minutes
    tInteger = this->minutes + duration->getMinutes() + tCarry;
    rMinutes = XalanDateTimeMath::modulo(tInteger, 60);
    tCarry = XalanDateTimeMath::fQuotient(tInteger, 60);

    // Calculating the hours
    tInteger = this->hours + duration->getHours() + tCarry;
    rHours = XalanDateTimeMath::modulo(tInteger, 24);
    tCarry = XalanDateTimeMath::fQuotient(tInteger, 24);

    // Calculating the day
    rDay = duration->getDay() + tCarry + (this->day > XalanDateTimeMath::maximumDayInMonthFor(rYear, rMonth) ? XalanDateTimeMath::maximumDayInMonthFor(rYear, rMonth) : this->day < 1 ? 1 : this->day);
    
    // Adjusting day, month and year
    bool negDay;
    while ((negDay = (rDay < 1)) || rDay > XalanDateTimeMath::maximumDayInMonthFor(rYear, rMonth))
    {
        if (negDay)
        {
            tCarry = -1;
            rDay += XalanDateTimeMath::maximumDayInMonthFor(rYear, rMonth - 1);
        }
        else
        {
            tCarry = 1;
            rDay -= XalanDateTimeMath::maximumDayInMonthFor(rYear, rMonth);
        }

        tInteger = rMonth + tCarry;
        rMonth = XalanDateTimeMath::modulo(tInteger, 1, 13);
        rYear += XalanDateTimeMath::fQuotient(tInteger, 1, 13);
    }

    /* Verifying if the year value has crossed the year zero.
       If true, an adjustment is made, because there is no zero year. */
    if (this->year < 0  && rYear > this->year && rYear >= 0)
    {
        rYear++;
    }
    else if (this->year > 0  && rYear < this->year && rYear <= 0)
    {
        rYear--;
    }

    this->year = rYear;
    this->month = rMonth;
    this->day = rDay;
    this->hours = rHours;
    this->minutes = rMinutes;
    this->seconds = rSeconds;
}

XalanDuration * XalanDateTime::difference(XalanDateTime * other)
{
    return new XalanDuration(
        other->getYear() - this->getYear(),
        other->getMonth() - this->getMonth(),
        other->getDay() - this->getDay(),
        other->getHours() - this->getHours(),
        other->getMinutes() - this->getMinutes(),
        other->getSeconds() - this->getSeconds());
}


XalanDateTime * XalanDateTime::fromSystem(XalanDateTimeFormat format)
{
    tm tmUTC, tmLocal;

    XalanSystemClock::getSystemTimeUTC(& tmUTC);
    XalanSystemClock::getSystemTimeLocal(& tmLocal);

    return new XalanDateTime(
            tmUTC.tm_year + 1900,          tmUTC.tm_mon + 1,           tmUTC.tm_mday,
            tmUTC.tm_hour,          tmUTC.tm_min,           tmUTC.tm_sec,
            XalanSystemClock::getLocalTimeZone(& tmLocal, & tmUTC),
            format);
}

XalanDateTime * XalanDateTime::fromString(const char * dateTime, XalanDateTimeFormat first, ...)
{
    va_list args;
    va_start(args, first);

    XalanDateTime * result = DateTimeFormatterDelegator::getInstance()->createInstance(dateTime, first, *args);

    va_end(args);

 
    return result;
}