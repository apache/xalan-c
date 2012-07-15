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

#include "XalanDateTime.hpp"

XALAN_CPP_NAMESPACE_USE

const char * XalanDateTime::INVALID_DATETIME_ERR =
{
	"The representation must be a right-truncated date/time string in "
	"one of the formats defined in [XML Schema Part 2: Datatypes]. "
	"The permitted formats are: xs:dateTime (CCYY-MM-DDThh:mm:ss); "
	"xs:date (CCYY-MM-DD); xs:gYearMonth (CCYY-MM) and xs:gYear (CCYY). "
	"An optional minus signal is accepted at the beginning, representing "
	"a BC time. These formats are accurately described at the following "
	"links, respectively: "
    "http://www.w3.org/TR/xmlschema-2/#dateTime; "
    "http://www.w3.org/TR/xmlschema-2/#date; "
    "http://www.w3.org/TR/xmlschema-2/#gYearMonth"" and "
    "http://www.w3.org/TR/xmlschema-2/#gYear"
};

const char * XalanDateTime::INVALID_TIMEZONE_ERR =
{
	"The representation of timezone must be a duration with "
	"(integer-valued) hour and minute properties (with the hour "
	"magnitude limited to at most 14, and the minute magnitude limited "
	"to at most 59, except that if the hour magnitude is 14, the minute "
	"value must be 0); they may be both positive or both negative. The "
	"lexical representation of a timezone is a string of the form: "
	"(('+' | '-') hh ':' mm) | 'Z'"
};

void XalanDateTime::addDuration(
    XalanDuration * duration)
{
    // Temporary variables declarations
    double tDouble;
    int tInteger, tCarry;

    // Resultant variables declarations
    double rSeconds;
    int rYear, rMonth, rDay, rHours, rMinutes;

    // Calculating the month
    tInteger = this->month + duration->getMonth();
    rMonth = modulo(tInteger, 1, 13);
    tCarry = fQuotient(tInteger, 1, 13);

    // Calculating the year
    rYear = this->year + duration->getYear() + tCarry;

    // Calculating the seconds
    tDouble = this->seconds + duration->getSeconds();
    rSeconds = modulo(tDouble, 60);
    tCarry = fQuotient(tDouble, 60);

    // Calculating the minutes
    tInteger = this->minutes + duration->getMinutes() + tCarry;
    rMinutes = modulo(tInteger, 60);
    tCarry = fQuotient(tInteger, 60);

    // Calculating the hours
    tInteger = this->hours + duration->getHours() + tCarry;
    rHours = modulo(tInteger, 24);
    tCarry = fQuotient(tInteger, 24);

    // Calculating the day
    rDay = duration->getDay() + tCarry + (this->day > maximumDayInMonthFor(rYear, rMonth) ? maximumDayInMonthFor(rYear, rMonth) : this->day < 1 ? 1 : this->day);
    
    // Adjusting day, month and year
    bool negDay;
    while ((negDay = (rDay < 1)) || rDay > maximumDayInMonthFor(rYear, rMonth))
    {
		if (negDay)
        {
            tCarry = -1;
            rDay += maximumDayInMonthFor(rYear, rMonth - 1);
        }
        else
        {
            tCarry = 1;
            rDay -= maximumDayInMonthFor(rYear, rMonth);
        }

        tInteger = rMonth + tCarry;
        rMonth = modulo(tInteger, 1, 13);
        rYear += fQuotient(tInteger, 1, 13);
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

char * XalanDateTime::toString()
{
    char * dateTime = new char[30];

    sprintf(year < 0 && (*dateTime = '-') ? dateTime + 1 : dateTime,
        format, abs(year), abs(month), abs(day), abs(hours),
        abs(minutes), abs(seconds));

    return strcat(dateTime, timeZone);
}

bool XalanDateTime::isTimeZoneValid(char * timeZone)
{
	if (*timeZone == '+' || *timeZone == '-')
	{
		timeZone += SEPR_FLD;

		int hoursMinutes[2];
		int upperBounds[] = {14, 59};
		int fieldsLens[] = {HH_FLD, MM_FLD};
    
		int nLength;
		int iPointer = 0;
		do
		{
			if (getNumberInfo(timeZone, &nLength)
				|| (nLength != fieldsLens[iPointer]))
			{
				return false;
			}

			if ((hoursMinutes[iPointer] = getInteger(timeZone, fieldsLens[iPointer]))
					> upperBounds[iPointer])
			{
				return false;
			}

			timeZone += nLength;

			if (*timeZone && (*timeZone != TM_SEPR_SYM))
			{
				return false;
			}

			timeZone += !iPointer ? SEPR_FLD : 0;
		} while (*timeZone && ++iPointer);

		if (hoursMinutes[0] == 14 && hoursMinutes[1])
		{
			return false;
		}
	}
	else if (*timeZone != UTC_TMZN_SYM || *(++timeZone))
	{
		return false;
	}

	return true;
}

XalanDateTime * XalanDateTime::fromString(char * dateTime)
{
    double seconds = 0;
    int year = 1, month = 1, day = 1,
        hours = 0, minutes = 0;

    // The timezone is a string in format '+hh:mm', '-hh:mm' or 'Z'
    char * timeZone = new char[6];
    
    // Verifying if it is a negative date/time
    bool negDate;
    if (negDate = *dateTime == '-')
    {
        dateTime++;
    }

	int * fieldsPointers[] =
        {&year, &month, &day, &hours, &minutes};
    int fieldsLens[] =
        {YY_FLD, MN_FLD, DD_FLD, HH_FLD, MM_FLD};
	int upperBounds[] =
        {32767, 12, 31, 23, 59};
    
    // Getting the values for each date/time field

	
	int iPointer = 0;
	int pointAt, nLength;
    do
    {
		if (getNumberInfo(dateTime, &nLength)
			|| (iPointer ? nLength != fieldsLens[iPointer]
				: nLength < fieldsLens[iPointer]))
		{
			throw InvalidFormat(INVALID_DATETIME_ERR);
		}

		if (abs(*fieldsPointers[iPointer] = negDate && !iPointer
				? -getInteger(dateTime, fieldsLens[iPointer])
				: getInteger(dateTime, fieldsLens[iPointer]))
			> upperBounds[iPointer] || (iPointer < 3 && !*fieldsPointers[iPointer]))
		{
			throw InvalidFormat(INVALID_DATETIME_ERR);
		}

        dateTime += nLength;

        if (!*dateTime || isTimeZone(dateTime))
        {
            break;
        }

		if (*dateTime && *dateTime
			!= (iPointer == 2 ? DTTM_SEPR_SYM : iPointer < 2 ? DT_SEPR_SYM : TM_SEPR_SYM))
		{
			throw InvalidFormat(INVALID_DATETIME_ERR);
		}

        dateTime += SEPR_FLD;

        if (iPointer++ == (sizeof(fieldsLens)/sizeof(*fieldsLens)) -1)
        {
			pointAt = getNumberInfo(dateTime, &nLength);
            
			if (pointAt)
			{
				if (pointAt - 1 != SS_FLD)
				{
					throw InvalidFormat(INVALID_DATETIME_ERR);
				}
				else if (!(nLength - pointAt))
				{
					throw InvalidFormat(INVALID_DATETIME_ERR);
				}
			}
			else if (nLength != SS_FLD)
			{
				throw InvalidFormat(INVALID_DATETIME_ERR);
			}

            seconds = getDouble(dateTime, nLength);
            dateTime += nLength;

            break;
        }
    } while (*dateTime);
	
	if (iPointer == 3 || iPointer == 4
		|| day > maximumDayInMonthFor(year, month))
	{
		throw InvalidFormat(INVALID_DATETIME_ERR);
	}

	if (*dateTime && !isTimeZoneValid(dateTime))
	{
		throw InvalidFormat(INVALID_TIMEZONE_ERR);
	}

	// Getting the timezone
    strcpy(timeZone, dateTime);

    return new XalanDateTime(year, month, day, hours, minutes,
        seconds, timeZone, getFormat(iPointer + 1));
}