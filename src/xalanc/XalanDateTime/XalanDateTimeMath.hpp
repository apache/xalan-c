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

#ifndef XALAN_DATETIME_MATH_H
#define XALAN_DATETIME_MATH_H

#include "XalanDateTimeDefinitons.hpp"

#include <math.h>

XALAN_CPP_NAMESPACE_BEGIN

class XalanDateTimeMath
{

public:

    static int maximumDayInMonthFor(long int yearValue, long int monthValue)
    {
        if (yearValue < 0)
        {
            yearValue = abs(yearValue) - 1;
        }

        return ((monthValue < 8 && monthValue % 2 != 0)
            || (monthValue >= 8 && monthValue % 2 == 0))
            ? 31
            : (monthValue != 02 ? 30
            : isLeapYear(yearValue) ? 29
            : 28);
    }

    static bool isLeapYear(long int yearValue)
    {
        if (yearValue < 0)
        {
            yearValue = abs(yearValue) - 1;
        }

        return (yearValue % 400 == 0 ||
              (yearValue % 100 != 0 && yearValue % 4 == 0));
    }

    static int fQuotient(double a, double b)
    {
        return floor( a / b );
    }

    static double modulo(double a, double b)
    {
        return a - fQuotient(a,b) * b;
    }
    
    static int fQuotient(double a, int low, int high)
    {
        return fQuotient(a - low, high - low);
    }

    static double modulo(double a, int low, int high)
    {
        return modulo(a - low, high - low) + low;
    }

    static const double NAN;

private:

    static const unsigned long _NAN[2];

};

XALAN_CPP_NAMESPACE_END

#endif