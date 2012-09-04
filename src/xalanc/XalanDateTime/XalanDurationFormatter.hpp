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

#ifndef _XALAN_DURATION_FORMATTER_H
#define _XALAN_DURATION_FORMATTER_H

#include "XalanDateTimeDefinitons.hpp"

#include "XalanDuration.hpp"
#include "XalanCalendarFormatter.hpp"

XALAN_CPP_NAMESPACE_BEGIN

template <typename T> class XalanDurationFormatter
    : public XalanCalendarFormatter<T>,
      public XalanCalendarIFormatter<XalanDuration>
{

public:

    static const char PD_SYM = 'P';

    XalanDuration * createInstance(const char * duration)
    {
        bool negative = checkChar(& duration, '-', false);

        checkChar(& duration, 'P', true);

        if (! *duration)
        {
            throw XalanInvalidDuration("");
        }

        long int year = negative
            ? -getYear(& duration)
            :  getYear(& duration);
        long int month = negative
            ? -getMonth(& duration)
            :  getMonth(& duration);
        long int day = negative
            ? -getDay(& duration)
            :  getDay(& duration);
    
        long int hours = 0;
        long int minutes = 0;
        double seconds = 0.0;
        if (*duration)
        {
            checkChar(& duration, 'T', true);

            hours = negative
                ? -getHours(& duration)
                :  getHours(& duration);
            minutes = negative
                ? -getMinutes(& duration)
                :  getMinutes(& duration);
            seconds = negative
                ? -getSeconds(& duration)
                :  getSeconds(& duration);
        }

        if (*duration)
        {
            throw XalanInvalidDuration("");
        }
    
        return new XalanDuration(year, month, day, hours, minutes, seconds);
    }

private:

    long int getYear(const char ** duration)
    {
        return getInteger(duration, YY_SYM);
    }

    long int getMonth(const char ** duration)
    {
        return getInteger(duration, MM_SYM);
    }

    long int getDay(const char ** duration)
    {
        return getInteger(duration, DD_SYM);
    }

    long int getHours(const char ** duration)
    {
        return getInteger(duration, HH_SYM);
    }

    long int getMinutes(const char ** duration)
    {
        return getInteger(duration, MM_SYM);
    }

    double getSeconds(const char ** duration)
    {
        return getDouble(duration, SS_SYM);
    }

    long int getInteger(const char ** duration, char fieldSym)
    {
        int nLength;
        getNumberInfo(*duration, &nLength);

        long int integer = 0;
        if (*((*duration) + nLength) == fieldSym)
        {
            integer = XalanCalendarFormatter::getInteger(duration);
            (*duration)++;
        }
        
        return integer;
    }

    double getDouble(const char ** duration, char fieldSym)
    {
        int nLength;
        getNumberInfo(*duration, &nLength);

        double number = 0.0;
        if (*((*duration) + nLength) == fieldSym)
        {
            number = XalanCalendarFormatter::getDouble(duration);
            (*duration)++;
        }

        return number;
    }

};

class DurationFormatterDelegator : public XalanDurationFormatter<DurationFormatterDelegator>
{

public:

    char * createString(XalanDuration * duration);

private:

    XalanCalendarIFormatter<XalanDuration> * getFormatter(XalanDurationFormat format);

};

class SuccinctDurationFormatter : public XalanDurationFormatter<SuccinctDurationFormatter>
{

public:

    char * createString(XalanDuration * duration);

private:

    char * getFormat(
        long int year, long int month,
        long int day, long int hours,
        long int minutes, double seconds,
        long int * orderedField)
    {
        const char PD_FORMAT[] = 
        {
            '%', 'g', YY_SYM,
            '%', 'g', MN_SYM,
            '%', 'g', DD_SYM,
            '%', 'g', HH_SYM,
            '%', 'g', MM_SYM,
            '%', 'g', SS_SYM
        };
        
        char * fmt, * format;
        format = fmt = new char[sizeof(PD_FORMAT) * sizeof(*PD_FORMAT)]; 
        *fmt++ = PD_SYM;

        int iSym;
        long int fields[] = {year, month, day, hours, minutes};
        for (iSym = 0; iSym <= sizeof(fields)/sizeof(*fields); iSym++)
        {
            if ((iSym == 5) ? seconds :
                ((*orderedField = fields[iSym]) && ++orderedField))
            {
                strcpy(fmt++, &PD_FORMAT[iSym * 3]);
                strcpy(fmt++, &PD_FORMAT[(iSym * 3) + 1]);
                strcpy(fmt++, &PD_FORMAT[(iSym * 3) + 2]);
            }
            if ((iSym == 2) && (hours + minutes + seconds))
            {
                *fmt++ = DTTMS_SYM;
            }
        }

        *fmt = '\0';

        return format;
    }

};

class YearMonthDurationFormatter : public XalanDurationFormatter<YearMonthDurationFormatter>
{

    char * createString(XalanDuration * duration);

};

class DayAndTimeDurationFormatter : public XalanDurationFormatter<DayAndTimeDurationFormatter>
{

    char * createString(XalanDuration * duration);

};

XALAN_CPP_NAMESPACE_END

#endif