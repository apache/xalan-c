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

#ifndef _XALAN_CALENDAR_FORMATTER_H
#define _XALAN_CALENDAR_FORMATTER_H

#include "XalanDateTimeDefinitons.hpp"

#include <sstream>

#include "XalanCalendar.hpp"
#include "XalanDateTimeExceptions.hpp"

XALAN_CPP_NAMESPACE_BEGIN

template <typename T> class XalanCalendarFormatter
{

public:

    enum CalendarSymbols
    {
        YY_SYM    = 'Y',
        MN_SYM    = 'M',
        DD_SYM    = 'D',
        HH_SYM    = 'H',
        MM_SYM    = 'M',
        SS_SYM    = 'S',
        NEG_SYM   = '-',
        DTTMS_SYM = 'T'
    };

    static T * getInstance()
    {
        static T * _mInstance = new T();
        return _mInstance;
    }

    virtual long int getYear(const char ** calendar) = 0;

    virtual long int getMonth(const char ** calendar) = 0;

    virtual long int getDay(const char ** calendar) = 0;

    virtual long int getHours(const char ** calendar) = 0;

    virtual long int getMinutes(const char ** calendar) = 0;

    virtual double getSeconds(const char ** calendar) = 0;

protected:

    XalanCalendarFormatter()
    {
    }

    bool checkChar(const char ** calendar, char chr, bool mayThrowExcp)
    {
        if (**calendar != chr)
        {
            return mayThrowExcp ? throw XalanInvalidCalendar("") : false;
        }
        
        (*calendar)++;
        return true;
    }

    int getNumberInfo(const char * info, int * nLength)
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
    
    long int getInteger(const char ** pInfo, int eLength = 0, bool maybeLarger = false)
    {
        int nLength = 0;
        
        if (getNumberInfo(*pInfo, &nLength)
            || (eLength && (maybeLarger ? nLength < eLength : nLength != eLength)))
        {
            throw XalanInvalidCalendar("");
        }

        char * n = new char[nLength + 1];
        
        n[nLength] = '\0';
        strncpy(n, *pInfo, nLength);

        *pInfo += nLength;

        return atol(n);
    }

    double getDouble(const char ** pInfo, int eLength = 0)
    {
        int nLength = 0;
        int pointAt = getNumberInfo(*pInfo, &nLength);

        if (!(nLength - pointAt)
            || (eLength && ((pointAt && ((pointAt - 1) != eLength))
                || (!pointAt && (nLength != eLength)))))
        {
            throw XalanInvalidCalendar("");
        }

        char * n = new char[nLength + 1];
        
        n[nLength] = '\0';
        strncpy(n, *pInfo, nLength);

        *pInfo += nLength;

        return atof(n);
    }

private:
    
    XalanCalendarFormatter(XalanCalendarFormatter<T> const&);
    void operator=(XalanCalendarFormatter<T> const&);

};

XALAN_CPP_NAMESPACE_END

#endif