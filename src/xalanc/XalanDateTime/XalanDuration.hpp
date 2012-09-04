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

#ifndef _XALAN_DURATION_H
#define _XALAN_DURATION_H

#include "XalanDateTimeDefinitons.hpp"

#include "XalanCalendar.hpp"

XALAN_CPP_NAMESPACE_BEGIN

enum XalanDurationFormat
    {
        SUCCINCT,               YEAR_MONTH,             DAY_AND_TIME
    };

class XalanDuration : public XalanCalendar<XalanDuration>
{

public:

    XalanDuration(
        long int year,
        long int month,
        long int day,
        long int hours,
        long int minutes,
        double seconds)
        : XalanCalendar(
            year,
            month,
            day,
            hours,
            minutes,
            seconds),
            formatType(SUCCINCT)
    {
    }

    XalanDuration(
        long int year,
        long int month,
        long int day,
        long int hours,
        long int minutes,
        double seconds,
        XalanDurationFormat formatType)
        : XalanCalendar(
            year,
            month,
            day,
            hours,
            minutes,
            seconds),
            formatType(formatType)
    {
    }

    inline XalanDurationFormat getFormatType()
    {
        return formatType;
    }

    inline void setFormatType(XalanDurationFormat newFormatType)
    {
        this->formatType = newFormatType;
    }

    void add(const XalanDuration * duration);

    long int getTotalOfSeconds();

    void normalizeAttributes();

    XalanCalendarIFormatter<XalanDuration> * getFormatter();

    static XalanDuration * XalanDuration::fromString(const char * duration);

private:

    XalanDurationFormat formatType;

};

XALAN_CPP_NAMESPACE_END

#endif