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

#include "XalanDuration.hpp"
#include "XalanDateTimeMath.hpp"
#include "XalanDurationFormatter.hpp"
#include "XalanDateTimeExceptions.hpp"

XALAN_CPP_NAMESPACE_USE

void XalanDuration::add(const XalanDuration * other)
{
    this->year += other->year;
    this->month += other->month;
    this->day += other->day;
    this->hours += other->hours;
    this->minutes += other->minutes;
    this->seconds += other->seconds;
    
    if ((this->year + this->month + this->day
        + this->hours + this->minutes + this->seconds) !=
        (abs(this->year) + abs(this->month) + abs(this->day)
        + abs(this->hours) + abs(this->minutes) + abs(this->seconds)))
    {
        throw XalanInvalidDuration("");
    }
}

long int XalanDuration::getTotalOfSeconds()
{
    return ((day * 24 * 60) + (hours * 60) + minutes) * 60;;
}

void XalanDuration::normalizeAttributes()
{
    bool negativeDuration = 
        (year + month + day + hours + minutes + seconds) < 0;

    if (abs(seconds) >= 60)
    {
        minutes += negativeDuration
            ? - XalanDateTimeMath::fQuotient(seconds, 60)
            :   XalanDateTimeMath::fQuotient(seconds, 60);
        seconds = negativeDuration
            ? - XalanDateTimeMath::modulo(seconds, 60)
            :   XalanDateTimeMath::modulo(seconds, 60);
    }

    long int carry = 0;
    int upperBounds[] = {60, 24};
    long int * fields[] = {& minutes, & hours, & day};

    int upperBound;
    for (int iField = 0; (iField < (sizeof(fields) / sizeof(*fields)) - 1); iField++)
    {
        if (abs(*fields[iField]) >= upperBounds[iField])
        {
            *fields[iField + 1] += negativeDuration
                ? - XalanDateTimeMath::fQuotient(*fields[iField], upperBounds[iField])
                :   XalanDateTimeMath::fQuotient(*fields[iField], upperBounds[iField]);
            *fields[iField] = negativeDuration
                ? - XalanDateTimeMath::modulo(*fields[iField], upperBounds[iField])
                :   XalanDateTimeMath::modulo(*fields[iField], upperBounds[iField]);
        }
    }
}

XalanDuration * XalanDuration::fromString(const char * duration)
{
    return DurationFormatterDelegator::getInstance()->createInstance(duration);
}

XalanCalendarIFormatter<XalanDuration> * XalanDuration::getFormatter()
{
    return DurationFormatterDelegator::getInstance();
}