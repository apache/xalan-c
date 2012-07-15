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

#ifndef _XALANDURATION_H
#define _XALANDURATION_H

#include "XalanCalendar.hpp"

XALAN_CPP_NAMESPACE_BEGIN

/**
 * This class defines Duration Objects
 *
 * Duration objects are specializations of Calendar objects, thus this
 * class inherits the attributes and behavior from the Calendar class.
 */
class XalanDuration : public XalanCalendar
{
public:
    XalanDuration(
        int year,
        int month,
        int day,
        int hours,
        int minutes,
        double seconds)
        : XalanCalendar(
            year,
            month,
            day,
            hours,
            minutes,
            seconds)
    {
    }

    /**
     * Gives a string representation of the XalanCalendar instance,
     * containing its date and time values
     *
     * Based on the format accurately described at:
     *     http://www.w3.org/TR/xmlschema-2/#duration
     * and in the information also found at this link that reduced
     * precision and truncated representations of this format are
     * allowed , this method returns the most concise possible
     * representation for Duration.
     * If all fields are 0, this method returns a 'P0Y' representation.
     *
     * @return A string representing the XalanDuration instance.
     */
    char * toString();

    /**
     * Given a string representation of Duration, this method returns
     * a new instance of XalanDuration
     * 
     * Based on the Duration represented as string received as argument,
     * this method validates the string representation, extract the
     * duration values and creates an instance of this XalanDuration
     * class.
     * 
     * @param duration This argument must conforms to  the [ISO 8601]
     *     extended format PnYnMnDTnHnMnS, where nY represents the
     *     number of years, nM the number of months, nD the number of
     *     days, 'T' is the date/time separator, nH the number of
     *     hours, nM the number of minutes and nS the number of
     *     seconds. The number of seconds can include decimal digits
     *     to arbitrary precision.
     *     This format are accurately described at:
     *     http://www.w3.org/TR/xmlschema-2/#duration
     *
     * @return An instance of this XalanDuration class. The values of
     *     the duration received as argument are loaded in the
     *     instance's fields.
     *
     * @exception InvalidFormat Thrown when the date/time represented
     *     as string received as argument is not in the accepted format.
     */
    static XalanDuration * fromString(char * duration);

	/* The error message when a string representation of duration is
	   not valid. */
	static const char * INVALID_DURATION_ERR;

private:
    /**
     * Representing durations as strings, there are specifc symbols
     * for identify each field.
     */
    enum PeriodSymbols
    {
		PD_SYM = 'P',
        YY_SYM = 'Y',
        MN_SYM = 'M',
        DD_SYM = 'D',
        HH_SYM = 'H',
        MM_SYM = 'M',
        SS_SYM = 'S'
    };

};

XALAN_CPP_NAMESPACE_END

#endif