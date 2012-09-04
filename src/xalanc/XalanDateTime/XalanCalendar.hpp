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

#ifndef _XALAN_CALENDAR_H
#define _XALAN_CALENDAR_H

#include "XalanDateTimeDefinitons.hpp"

#include "XalanCalendarIFormatter.hpp"

XALAN_CPP_NAMESPACE_BEGIN

/**
 * This class abstract class defines Calendar objects
 *
 * Calendar are objects that have year, month, day, hours, minutes and seconds
 * attributes. The five first ones are defined as long int values, that are 32
 * bit integer limited by [-2147483648, 2147483647]; and the last one defined as
 * a double value.
 * All Calendar objects have a string representation.
 *
 * <typename T> is the specific type of the XalanCalendar.
 */
template <typename T> class XalanCalendar
{

public:

    XalanCalendar(
        long int year,
        long int month,
        long int day,
        long int hours,
        long int minutes,
        double seconds)
        : year(year),
          month(month),
          day(day),
          hours(hours),
          minutes(minutes),
          seconds(seconds)
    {
    }

    inline long int getYear() const
    {
        return year;
    }

    inline long int getMonth() const
    {
        return month;
    }

    inline long int getDay() const
    {
        return day;
    }

    inline long int getHours() const
    {
        return hours;
    }

    inline long int getMinutes() const
    {
        return minutes;
    }

    inline double getSeconds() const
    {
        return seconds;
    }

    /**
     * This function returns the string representation of the XalanCalendar
     * object. To separate the string manipulation from the model, there is
     * a formatter responsible for do that.
     *
     * @return The string representation of the XalanCalendar object.
     */
    char * toString()
    {
        return getFormatter()->createString(dynamic_cast<T *> (this));
    }

    /**
     * This function must be overwritten in the subclasses, each one
     * returning a pointer to the formatter for its values.
     *
     * @return The formatter that will manipulate the strings.
     */
    virtual XalanCalendarIFormatter<T> * getFormatter() = 0;
    
protected:

    long int year, month, day;
    
    long int hours, minutes;
    double seconds;

};

XALAN_CPP_NAMESPACE_END

#endif