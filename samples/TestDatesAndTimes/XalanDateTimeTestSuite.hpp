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

#include <stdlib.h>

#include "xalanc/XalanDatetime/XalanDateTime.hpp"
#include "xalanc/XalanDateTime/XalanDuration.hpp"
#include "xalanc/XalanDateTime/XalanDateTimeFormatter.hpp"

XALAN_CPP_NAMESPACE_BEGIN

class XalanDateTimeTestSuite {

private:

    void notifyFailure(const char * errMsg);

    void assertNumberEquals(int one, int other);

    void assertStringEquals(const char * one, const char * other);

    void assertInvalidDateTimes(
        char * invalidRepresentations[],
        const int nRepresentations,
        XalanDateTimeFormat format);

    void assertValidDateTimes(char * validRepresentations[],
        const int nRepresentations,
        XalanDateTimeFormat format);

    void assertInvalidDurations(
        char * invalidRepresentations[],
        const int nRepresentations);

    void assertValidDurations(char * validRepresentations[],
        const int nRepresentations);

public:

    XalanDateTimeTestSuite();

    void dtTimeFormatFromToStringTest();
    void dtDateFormatFromToStringTest();
    void dtGDayFormatFromToStringTest();
    void dtGYearFormatFromToStringTest();
    void dtGMonthFormatFromToStringTest();
    void dtDateTimeFormatFromToStringTest();
    void dtGMonthDayFormatFromToStringTest();
    void dtGYearMonthFormatFromToStringTest();

    void durationFromToStringTest();

    void dtDayOfWeekTest();

};

XALAN_CPP_NAMESPACE_END

