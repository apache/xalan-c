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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sstream>

#ifndef _XALANCALENDAR_H
#define _XALANCALENDAR_H

#include <xalanc/Include/PlatformDefinitions.hpp>

XALAN_CPP_NAMESPACE_BEGIN

/**
 * This class defines Calendar Objects
 *
 * Calendar objects represent a moment in the time. Thus, these objects
 * have year, month and day fields regarding the date values; and hours, 
 * minutes and seconds fields regarding the time values.
 *
 * Often, moments in the time are represented as strings. Thus, they
 * may to have them values retrieved from strings and represented as
 * strings back.
 */
class XalanCalendar
{
public:
    XalanCalendar(
        int year,
        int month,
        int day,
        int hours,
        int minutes,
        double seconds)
        : year(year),
          month(month),
          day(day),
          hours(hours),
          minutes(minutes),
          seconds(seconds)
    {
    }

    /**
     * Returns the year value of the XalanCalendar object.
     */
    inline int getYear() const
    {
        return year;
    }

    /**
     * Returns the month value of the XalanCalendar object.
     */
    inline int getMonth() const
    {
        return month;
    }

    /**
     * Returns the day value of the XalanCalendar object.
     */
    inline int getDay() const
    {
        return day;
    }

    /**
     * Returns the hours value of the XalanCalendar object.
     */
    inline int getHours() const
    {
        return hours;
    }

    /**
     * Returns the minutes value of the XalanCalendar object.
     */
    inline int getMinutes() const
    {
        return minutes;
    }

    /**
     * Returns the seconds value of the XalanCalendar object.
     */
    inline double getSeconds() const
    {
        return seconds;
    }

    /**
     * Gives a string representation of the XalanCalendar instance,
     * containing its date and time values.
     *
     * @return The string representing the XalanCalendar instance.
     */
    virtual char * toString() = 0;

protected:
    int year, month, day;
    
    int hours, minutes;
    double seconds;

    /* Generally, in string representations of objects, values are
       separated by special fields. This variable represents the
       length of a separator character. */
    static const int SEPR_FLD = 1;

    /* In string representations of XalanCalendar, the date and time
       values are separated by a 'T' character, indicating the init
       of time values representation. */
    static const char DTTM_SEPR_SYM = 'T';

    /* C strings are represented as a sequence of characters ending
       with the null character. */
    static const char NULL_CHAR = '\0';
    
    /**
     * Given a string, analyses it and gives the location of the point
     * character of the number that have its string representation
     * starten at the init of the string. Numbers are any sequence of
     * digits and may include one point.
     * Futhermore, this method stores into the location pointed by
     * nLength the length of the number.
     *
     * @param info The information containing the number to have
     *     information known.
     *
     * @param nLength The pointer to the location at memory to store
     *     the length of the number.
     *
     * @return The location of the point character into the number
     *     representation.
     */
	static int getNumberInfo(char * info, int * nLength)
	{
		int len = 0;
		int pointAt = 0;
		while (isdigit(*info) 
			|| (!pointAt && (pointAt = (*info == '.') ? len + 1 : 0)))
		{
			len++;
			info++;
		}

		*nLength = *info == '.' ? 0 : len;

		return pointAt;
	}

    /**
     * Given a string and a length, extracts from the string a double
     * with the specified length. This method do not do any validation
     * of the arguments: it considers there is a double in the string
     * with the specified length.
     *
     * @param info The information to have the double extracted.
     *
     * @param length The length of the double representation into the
     *     string.
     *
     * @return The double retrieved from the string, with the
     *     specified length.
     */
    static double getDouble(char * info, int length)
    {
        char * n = new char[length + 1];
        
        n[length] = NULL_CHAR;
        strncpy(n, info, length);

        return atof(n);
    }

    /**
     * Given a string and a length, extracts from the string an integer
     * with the specified length. This method do not do any validation
     * of the arguments: it considers there is a double in the string
     * with the specified length.
     *
     * @param info The information to have the integer extracted.
     *
     * @param length The length of the integer representation into the
     *     string.
     *
     * @return The integer retrieved from the string, with the
     *     specified length.
     */
    static int getInteger(char * info, int length)
    {
        char * n = new char[length + 1];
        
        n[length] = NULL_CHAR;
        strncpy(n, info, length);

        return atoi(n);
    }
};

/**
 * Often, moments in the time are represented as strings. This class
 * represents exceptions thrown when those strings are not in valid
 * formats.
 */
class InvalidFormat : public std::invalid_argument
{
public:
	InvalidFormat(const char * message)
		: invalid_argument(message)
	{
	}
};

XALAN_CPP_NAMESPACE_END

#endif