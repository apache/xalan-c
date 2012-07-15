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

#ifndef _XALANDATESANDTIMES_H
#define _XALANDATESANDTIMES_H

#include <xalanc/Include/PlatformDefinitions.hpp>

XALAN_CPP_NAMESPACE_BEGIN

class XalanDatesAndTimes
{
public:

    /**
     * This function returns the date/time resulting from adding a
     * duration to a date/time
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
     * @return The resultant date/time by adding the duration to the
     *     date/time in the same format of the date/time received as
     *     argument.
     */
    static char * add(char * dateTime, char * duration);

};

XALAN_CPP_NAMESPACE_END

#endif