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

#include "XalanDurationFormatter.hpp"

XALAN_CPP_NAMESPACE_USE

XalanCalendarIFormatter<XalanDuration> * DurationFormatterDelegator::getFormatter(XalanDurationFormat format)
{
    switch (format)
    {
    case SUCCINCT:
        return SuccinctDurationFormatter::getInstance();
    case YEAR_MONTH:
        return YearMonthDurationFormatter::getInstance();
    case DAY_AND_TIME:
        return DayAndTimeDurationFormatter::getInstance();
    default:
        return 0;
    }
}

char * DurationFormatterDelegator::createString(XalanDuration * duration)
{
    return getFormatter(duration->getFormatType())->createString(duration);
}

char * SuccinctDurationFormatter::createString(XalanDuration * duration)
{
    if (!(duration->getYear() + duration->getMonth() + duration->getDay()
        + duration->getHours() + duration->getMinutes() + duration->getSeconds()))
    {
        return "P0Y";
    }

    /* 604 means a possible minus signal, the maximum value of long int
       2147483647 is accepted for each field plus 2 character for P and
       T separator plus null. */
    char * rDuration = new char[604];

    long int arr[] = {0,0,0,0,0};
    char * format = getFormat(
        duration->getYear(), duration->getMonth(),
        duration->getDay(), duration->getHours(),
        duration->getMinutes(), duration->getSeconds(), arr);

    sprintf(
        (duration->getYear() + duration->getMonth() +  duration->getDay() + duration->getHours() + duration->getMinutes() + duration->getSeconds()) < 0
            && (*rDuration = '-') ? rDuration + 1 : rDuration,
        format,
        arr[0] ? abs((double) arr[0]) : abs(duration->getSeconds()),
        arr[1] ? abs((double) arr[1]) : abs(duration->getSeconds()),
        arr[2] ? abs((double) arr[2]) : abs(duration->getSeconds()),
        arr[3] ? abs((double) arr[3]) : abs(duration->getSeconds()),
        arr[4] ? abs((double) arr[4]) : abs(duration->getSeconds()),
        abs(duration->getSeconds()));

    return rDuration;
}

char * YearMonthDurationFormatter::createString(XalanDuration * duration)
{
    /* 24 means a possible minus signal, the maximum value of long int
       2147483647 is accepted for each field plus 2 character for Y and M
       separator plus null. */
    char * rDuration = new char[24];

    sprintf(
        (duration->getYear() + duration->getMonth()) < 0
            && (*rDuration = '-') ? rDuration + 1 : rDuration,
        "P%dY%dM",
        abs(duration->getYear()),
        abs(duration->getMonth()));

    return rDuration;
}

char * DayAndTimeDurationFormatter::createString(XalanDuration * duration)
{
    char * rDuration = new char[47];

    sprintf(
        (duration->getDay() + duration->getHours()
            + duration->getMinutes() + duration->getSeconds()) < 0
            && (*rDuration = '-') ? rDuration + 1 : rDuration,
        "P%dDT%dH%dM%gS",
        abs(duration->getDay()),
        abs(duration->getHours()),
        abs(duration->getMinutes()),
        abs(duration->getSeconds()));

    return rDuration;
}