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


#ifndef _XALANDATETIME_H
#define _XALANDATETIME_H

#include "XalanCalendar.hpp"
#include "XalanDuration.hpp"

XALAN_CPP_NAMESPACE_BEGIN

/**
 * This class defines date/time objects
 *
 * Date/time objects are specializations of Calendar objects, thus this
 * class inherits the attributes and behavior from the Calendar class.
 * Beyond the attributes inherited, date/time objects have an optional
 * timezone value.
 *
 * The timezone value is a string in format '+hh:mm', '-hh:mm' or 'Z'.
 * This class refers to the Gregorian Calendar algorithm where there is
 * no year zero.
 *
 * A complete and accurately description of date/time may be found at
 * http://www.w3.org/TR/xmlschema-2/#dateTime. Right-truncated
 * string representation are too allowed, beyond the described at this
 * link, but it is the complete representation.
 */
class XalanDateTime : public XalanCalendar
{
public:
    XalanDateTime(
        int year,
        int month,
        int day,
        int hours,
        int minutes,
        double seconds,
        char * timeZone,
        char * format)
        : XalanCalendar(
            year,
            month,
            day,
            hours,
            minutes,
            seconds),
            format(format),
            timeZone(timeZone)
    {
    }

    /**
     * This method adds to the a XalanDuration to a XalanDateTime
     *
     * Given a XalanDuration, this method adds to the XalanDateTime
     * instance's fields the corresponding values in the duration.
     * This function refers to the EXSLT DATE:ADD function described at
     * http://www.exslt.org/date/functions/add/index.html.
     * The implemented algorithm is according the one described at
     * http://www.w3.org/TR/xmlschema-2/#adding-durations-to-dateTimes.
     * There is no zero year. Thus, some adjust may be made in the
     * computation.
     *
     * @param duration A instance of XalanDuration, that has the
     *     values of date/time fields to be added to the XalanDateTime
     *     instance.
     */
    void addDuration(XalanDuration * duration);

    /**
     * Gives a string representation of the XalanDateTime instance,
     * containing its date and time values
     *
     * Based on the format of the date/time received as parameter to
     * creates a new instance from the fromString(..) method, this
     * method gives a string representation of the XalanDateTime in
     * the same format. For information about the possible formats,
     * see the information about the accepted formats in the
     * fromString(..) method.
     *
     * @return A string representing the XalanDateTime instance.
     */
    char * toString();

    /**
     * Given a string representation of date/time, this method returns
     * an instance of this class
     * 
     * Based on the date/time represented as string received as
     * argument, this method validates the string representation,
     * extract the date/time values and creates an instance of this
     * XalanDateTime class.
     * 
     * @param dateTime This argument must be a right-truncated
     *     date/time string in one of the formats defined in
     *     [XML Schema Part 2: Datatypes].
     *     The permitted formats are as follows:
     *     - xs:dateTime (CCYY-MM-DDThh:mm:ss);
     *     - xs:date (CCYY-MM-DD);
     *     - xs:gYearMonth (CCYY-MM);
     *     - xs:gYear (CCYY).
     *     These formats are accurately described at the bellow links,
     *     respectively:
     *     - http://www.w3.org/TR/xmlschema-2/#dateTime;
     *     - http://www.w3.org/TR/xmlschema-2/#date;
     *     - http://www.w3.org/TR/xmlschema-2/#gYearMonth;
     *     - http://www.w3.org/TR/xmlschema-2/#gYear.
     *
     * @return An instance of this XalanDateTime class. The values of
     *     the date/time received as argument are loaded in the
     *     instance's fields.
     *
     * @exception InvalidFormat Thrown when the date/time
     *     represented as string received as argument is not in one of
     *     the accepted formats. Also, when the timezone representation
	 *     is not valid, see isTimeZoneValid method.
     */
    static XalanDateTime * fromString(char * dateTime);

	/**
	 * Given a string representation of a timezone, this method
	 * evaluates it and determines if it is valid or not.
	 * The representation of timezone must be a duration with
	 * (integer-valued) hour and minute properties (with the hour
	 * magnitude limited to at most 14, and the minute magnitude limited
	 * to at most 59, except that if the hour magnitude is 14, the minute
	 * value must be 0); they may be both positive or both negative. The
	 * lexical representation of a timezone is a string of the form:
	 * (('+' | '-') hh ':' mm) | 'Z'
	 *
	 * @param timeZone The string representation of a timezone to be
	 *     evaluated.
	 *
	 * @return true, if the timezone is valid; false, otherwise.
	 */
	static bool isTimeZoneValid(char * timeZone);

	/* The error message when a string representation of date/time is
	   not valid. */
	static const char * INVALID_DATETIME_ERR;

	/* The error message when a string representation of timezone is
	   not valid. */
	static const char * INVALID_TIMEZONE_ERR;

private:

    /* The format of the string representation for the date/time object.
       This format is built considering the formats accepted by the
       printf function. For more information, see getFormat() method.
       It is loaded when the fromString() method is called, getting the
       format of the string received, allowing to give a representation
       in the same format. */
    char * format;

    char * timeZone;

    /**
     * Representing date/time as string, there are specifc symbols
     * for separators and default timezone.
     */
    enum DateTimeSymbols
    {
        DT_SEPR_SYM   = '-',
        TM_SEPR_SYM   = ':',
        UTC_TMZN_SYM  = 'Z'
    };

    /**
     * Representing date/time as string, there are fixed lengths for the
     * month, day, hours and minutes fields.
     */
    enum DateTimeFieldsSize
    {
		YY_FLD = 4,
        MN_FLD = 2,
        DD_FLD = 2,
        HH_FLD = 2,
        MM_FLD = 2,
        SS_FLD = 2
    };

    /**
     * Given a range value, reporting how many fields of date/time must
     * be present in the resultant format, the required format is
     * returned.
     *
     * @param range The number reporting how many fields must be present
     *     in the format.
     *
     * @return The format string, as required.
     */
    static char * getFormat(int range)
    {
        const char DTTM_FORMAT[] = 
        {
            '%', '0', '4', 'd', DT_SEPR_SYM,
            '%', '0', '2', 'd', DT_SEPR_SYM,
            '%', '0', '2', 'd', DTTM_SEPR_SYM,
            '%', '0', '2', 'd', TM_SEPR_SYM,
            '%', '0', '2', 'd', TM_SEPR_SYM,
            '%', '0', '2', 'g'
        };

        int n = 5 * range;
        char * dateFormat = new char[n];

        dateFormat[n - 1] = NULL_CHAR;
        strncpy(dateFormat, DTTM_FORMAT, n - 1);

        return dateFormat;
    }

    /**
     * Returns the floor of the quotient of a and b.
     *
     * @param a The first number.
     *
     * @param b The second number.
     *
     * @return The floor of the quotient of a and b.
     */
    static int fQuotient(double a, double b)
    {
        return floor( a / b );
    }

    /**
     * Returns the a modulo b.
     *
     * @param a The first number.
     *
     * @param b The second number.
     *
     * @return a modulo b.
     */
    static double modulo(double a, double b)
    {
        return a - fQuotient(a,b) * b;
    }
    
    /**
     * Returns the floor of the quotient of (a - low) and (high - low).
     *
     * @param a The first number.
     *
     * @param low The second number.
     *
     * @param high The third number.
     *
     * @return The floor of the quotient of (a - low) and (high - low).
     */
    static int fQuotient(double a, int low, int high)
    {
        return fQuotient(a - low, high - low);
    }

    /**
     * Returns the result of low + (a - low) modulo (high - low).
     *
     * @param a The first number.
     *
     * @param low The second number.
     *
     * @param high The third number.
     *
     * @return The result of low + (a - low) modulo (high - low)
     */
    static double modulo(double a, int low, int high)
    {
        return modulo(a - low, high - low) + low;
    }

    /**
     * Given a year and a month, this method returns the maximum day
     * of that month.
     *
     * @param yearValue The year containing the specified month.
     * @param monthValue The month in the year that the maximum day
     *     will be calculated.
     *
     * @return The maximum day of the specified month/year.
     */
    static int maximumDayInMonthFor(int yearValue, int monthValue)
    {
		if (yearValue < 0)
		{
			yearValue = abs(yearValue) - 1;
		}

		return ((monthValue < 8 && monthValue % 2 != 0)
            || (monthValue >= 8 && monthValue % 2 == 0))
            ? 31
            : (monthValue != 02 ? 30
            : (yearValue % 400 == 0 ||
              (yearValue % 100 != 0 && yearValue % 4 == 0)) ? 29
            : 28);
    }
    
    /**
     * Analyses an string received as argument and reports if it
     * determines a timezone.
     * The timezone is a string in format '+hh:mm', '-hh:mm' or 'Z'.
     * This method do not do any validation of timezones.
     *
     * @param info The string to be analyzed.
     *
     * @return true, if the string determines a timezone value;
     *     false, otherwise.
     */
    static bool isTimeZone(char * info)
    {
        return *info == UTC_TMZN_SYM
            || ((*info == '-' || *info == '+')
                && *(info + 3) == TM_SEPR_SYM);
    }
};

XALAN_CPP_NAMESPACE_END

#endif