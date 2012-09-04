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
#include "XalanDateTimeFormatter.hpp"

XALAN_CPP_NAMESPACE_USE

XalanCalendarIFormatter<XalanDateTime> * DateTimeFormatterDelegator::getFormatter(XalanDateTimeFormat format)
{
    switch (format)
    {
    case TIME:
        return TimeFormatter::getInstance();
    case DATE:
        return DateFormatter::getInstance();
    case G_DAY:
        return GDayFormatter::getInstance();
    case G_YEAR:
        return GYearFormatter::getInstance();
    case G_MONTH:
        return GMonthFormatter::getInstance();
    case DATE_TIME:
        return DateTimeFormatter::getInstance();
    case G_MONTH_DAY:
        return GMonthDayFormatter::getInstance();
    case G_YEAR_MONTH:
        return GYearMonthFormatter::getInstance();
    default:
        return 0;
    }
}

char * DateTimeFormatterDelegator::createString(XalanDateTime * time)
{
    return getFormatter(time->getFormatType())->createString(time);
}

XalanDateTime * DateTimeFormatterDelegator::createInstance(const char * dateTime)
{	
    return createInstance(dateTime,
        TIME,			DATE,
        G_DAY,			G_YEAR,
        G_MONTH,		DATE_TIME,
        G_MONTH_DAY,	G_YEAR_MONTH);
}

XalanDateTime * DateTimeFormatterDelegator::createInstance(const char * dateTime, XalanDateTimeFormat first, ...)
{
    va_list ap;
    va_start(ap, first);

    XalanDateTimeFormat format = first;
    do
    {
        try
        {
            return getFormatter(format)->createInstance(dateTime);
        }
        catch (XalanInvalidCalendar & invalidCalendar)
        {
            continue;
        }
    } while ((format = va_arg(ap, XalanDateTimeFormat)) < NUM_FORMATS && format >= 0);
    
    va_end(ap);

    throw XalanInvalidDateTime("");
}

char * TimeFormatter::createString(XalanDateTime * time)
{
    char * gYyMn = new char[30];

    sprintf(gYyMn, "%02ld:%02ld:%s", time->getHours(), 
        time->getMinutes(), getFormattedSeconds(time->getSeconds()));

    return strcat(gYyMn, time->getTimeZone());
}

XalanDateTime * TimeFormatter::createInstance(const char * time)
{
    long int hours = getHours(& time);      // HH
    checkSeparator(& time, TMS_SYM);        // :
    long int minutes = getMinutes(& time);  // MM
    checkSeparator(& time, TMS_SYM);        // :
    double seconds = getSeconds(& time);    // SS
    checkTime(hours, minutes, seconds);
    
    return new XalanDateTime(1, 1, 1, hours, minutes, seconds,
        getTimeZone(& time), TIME);
}

char * DateFormatter::createString(XalanDateTime * date)
{
    char * gYyMn = new char[30];

    sprintf(date->getYear() < 0 && (*gYyMn = '-') ? gYyMn + 1 : gYyMn,
        "%04ld-%02ld-%02ld", abs(date->getYear()), date->getMonth(),
        date->getDay());

    return strcat(gYyMn, date->getTimeZone());
}

XalanDateTime * DateFormatter::createInstance(const char * date)
{
    long int year = getYear(& date);    // CCYY
    checkSeparator(& date, DTS_SYM);    // -
    long int month = getMonth(& date);  // MM
    checkSeparator(& date, DTS_SYM);    // -
    long int day = getDay(& date);      // DD
    checkDay(year, month, day);
    
    return new XalanDateTime(year, month, day, 0, 0, 0.0, 
        getTimeZone(& date), DATE);
}

char * GDayFormatter::createString(XalanDateTime * gDay)
{
    char * gYyMn = new char[30];

    sprintf(gYyMn, "---%02ld", gDay->getDay());

    return strcat(gYyMn, gDay->getTimeZone());
}

XalanDateTime * GDayFormatter::createInstance(const char * gDay)
{
    checkSeparator(& gDay, DTS_SYM);    // -
    checkSeparator(& gDay, DTS_SYM);    // -
    checkSeparator(& gDay, DTS_SYM);    // -
    long int day = getDay(& gDay);      // DD
    
    return new XalanDateTime(1, 1, day, 0, 0, 0.0,
        getTimeZone(& gDay), G_DAY);
}

char * GYearFormatter::createString(XalanDateTime * gYear)
{
    char * gYyMn = new char[30];

    sprintf(gYear->getYear() < 0 && (*gYyMn = '-') ? gYyMn + 1 : gYyMn,
        "%04ld", abs(gYear->getYear()));

    return strcat(gYyMn, gYear->getTimeZone());
}

XalanDateTime * GYearFormatter::createInstance(const char * gYear)
{
    long int year = getYear(& gYear);   // CCYY

    return new XalanDateTime(year, 1, 1, 0, 0, 0.0,
        getTimeZone(& gYear), G_YEAR);
}

char * GMonthFormatter::createString(XalanDateTime * gMonth)
{
    char * gYyMn = new char[30];

    sprintf(gYyMn, "--%02ld", gMonth->getMonth());

    return strcat(gYyMn, gMonth->getTimeZone());
}

XalanDateTime * GMonthFormatter::createInstance(const char * gMonth)
{
    checkSeparator(& gMonth, DTS_SYM);      // -
    checkSeparator(& gMonth, DTS_SYM);      // -
    long int month = getMonth(& gMonth);    // MM
    
    return new XalanDateTime(1, month, 1, 0, 0, 0.0,
        getTimeZone(& gMonth), G_MONTH);
}

char * DateTimeFormatter::createString(XalanDateTime * dateTime)
{
    char * gYyMn = new char[30];

    sprintf(dateTime->getYear() < 0 && (*gYyMn = '-') ? gYyMn + 1 : gYyMn,
        "%04ld-%02ld-%02ldT%02ld:%02ld:%s", abs(dateTime->getYear()), dateTime->getMonth(),
        dateTime->getDay(), dateTime->getHours(), 
        dateTime->getMinutes(), getFormattedSeconds(dateTime->getSeconds()));

    return strcat(gYyMn, dateTime->getTimeZone());
}

XalanDateTime * DateTimeFormatter::createInstance(const char * dateTime)
{
    long int year = getYear(& dateTime);        // CCYY
    checkSeparator(& dateTime, DTS_SYM);        // -
    long int month = getMonth(& dateTime);      // MM
    checkSeparator(& dateTime, DTS_SYM);        // -
    long int day = getDay(& dateTime);          // DD
    checkDay(year, month, day);
    checkSeparator(& dateTime, DTTMS_SYM);      // T
    long int hours = getHours(& dateTime);      // HH
    checkSeparator(& dateTime, TMS_SYM);        // :
    long int minutes = getMinutes(& dateTime);  // MM
    checkSeparator(& dateTime, TMS_SYM);        // :
    double seconds = getSeconds(& dateTime);    // SS
    checkTime(hours, minutes, seconds);

    return new XalanDateTime(year, month, day, hours, minutes, seconds,
        getTimeZone(& dateTime), DATE_TIME);
}

char * GMonthDayFormatter::createString(XalanDateTime * gMonthDay)
{
    char * gYyMn = new char[30];

    sprintf(gYyMn, "--%02ld-%02ld", gMonthDay->getMonth(), gMonthDay->getDay());

    return strcat(gYyMn, gMonthDay->getTimeZone());
}

XalanDateTime * GMonthDayFormatter::createInstance(const char * gMonthDay)
{
    checkSeparator(& gMonthDay, DTS_SYM);   // -
    checkSeparator(& gMonthDay, DTS_SYM);   // -
    long int month = getMonth(& gMonthDay); // MM
    checkSeparator(& gMonthDay, DTS_SYM);   // -
    long int day = getDay(& gMonthDay);     // DD
    checkDay(-0001, month, day);

    return new XalanDateTime(1, month, day, 0, 0, 0.0,
        getTimeZone(& gMonthDay), G_MONTH_DAY);
}

char * GYearMonthFormatter::createString(XalanDateTime * gYearMonth)
{
    char * gYyMn = new char[30];

    sprintf(gYearMonth->getYear() < 0 && (*gYyMn = '-') ? gYyMn + 1 : gYyMn,
        "%04ld-%02ld", abs(gYearMonth->getYear()), gYearMonth->getMonth());

    return strcat(gYyMn, gYearMonth->getTimeZone());
}

XalanDateTime * GYearMonthFormatter::createInstance(const char * gYearMonth)
{
    long int year = getYear(& gYearMonth);      // CCYY
    checkSeparator(& gYearMonth, DTS_SYM);      // -
    long int month = getMonth(& gYearMonth);    // MM

    return new XalanDateTime(year, month, 1, 0, 0, 0.0,
        getTimeZone(& gYearMonth), G_YEAR_MONTH);
}