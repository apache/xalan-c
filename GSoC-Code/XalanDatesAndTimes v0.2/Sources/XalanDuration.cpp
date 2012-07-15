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

XALAN_CPP_NAMESPACE_USE

const char * XalanDuration::INVALID_DURATION_ERR =
{
	"The string representation must conforms with the format "
	"PnYnMnDTnHnMnS, where nY represents the number of "
	"years, nM the number of months, nD the number of days, "
	"'T' is the date/time separator, nH the number of "
	"hours and nM the number of minutes, where each one is represented "
	"as an unsigned integer. nS is the number of seconds, that may be "
	"represented as a floating-point number. An optional minus signal "
	"is accepted at the beginning, representing a negative duration. "
	"For detailed information, see "
	"http://www.w3.org/TR/xmlschema-2/#duration"
};

char * XalanDuration::toString()
{
    // Not implemented yet.
    return new char[1];
}

XalanDuration * XalanDuration::fromString(char * duration)
{
    double seconds = 0;
    int year = 0, month = 0, day = 0,
        hours = 0, minutes = 0;

    // Verifying if it is a negative duration
    bool negDuration;
    if (negDuration = (*duration == '-'))
	{
        duration++;
    }

	if (*duration != PD_SYM)
	{
		throw InvalidFormat(INVALID_DURATION_ERR);
	}

	duration += SEPR_FLD;

	char fieldsSym[] =
        {YY_SYM, MN_SYM, DD_SYM, HH_SYM, MM_SYM, SS_SYM};
    int * fieldsPointers[] =
        {&year, &month, &day, &hours, &minutes};

    // Getting the values for each duration field
	int pointAt = 0;
 	int numberLength = 0;
	bool gotTime = false;
	
	int iPointer = 0;
    while (*duration)
    {
		if (*duration == DTTM_SEPR_SYM)
		{
			if (gotTime || (iPointer > 3))
			{
				throw InvalidFormat(INVALID_DURATION_ERR);
			}

			iPointer = 3;
			gotTime = true;
			duration += SEPR_FLD;
 		}

		pointAt = getNumberInfo(duration, &numberLength);
		duration += numberLength;

		if ((pointAt == 1)
			|| !(numberLength - pointAt))
		{
			throw InvalidFormat(INVALID_DURATION_ERR);
		}

		while ((*duration != fieldsSym[iPointer])
			&& ((iPointer + 1) < (sizeof(fieldsSym)/sizeof(*fieldsSym))))
        {
			iPointer++;
        }

		if (*duration == SS_SYM)
        {
			if (!gotTime || *(duration + SEPR_FLD))
			{
				throw InvalidFormat(INVALID_DURATION_ERR);
			}

            seconds = negDuration
				? -getDouble(duration - numberLength, numberLength)
                : getDouble(duration - numberLength, numberLength);

            return new XalanDuration(year, month, day, hours,
                minutes, seconds);
        }
		else if ((iPointer + 1) == (sizeof(fieldsSym)/sizeof(*fieldsSym)))
		{
			throw InvalidFormat(INVALID_DURATION_ERR);
		}

		if (pointAt || (iPointer >= 3 && !gotTime))
		{
			throw InvalidFormat(INVALID_DURATION_ERR);
		}

        *fieldsPointers[iPointer] = negDuration
            ? -getInteger(duration - numberLength, numberLength)
            : getInteger(duration - numberLength, numberLength);

		iPointer++;
        duration++;
    }

	if (!iPointer)
	{
		throw InvalidFormat(INVALID_DURATION_ERR);
	}

	return new XalanDuration(year, month, day, hours,
        minutes, seconds);
}
