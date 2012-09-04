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

/* The memcpy routine is defined in <string.h>
 * The time routines and structures are defined in <time.h>
 */

#include <iostream>
#include <time.h>
#include <math.h>

class XalanSystemClock
{

public:

    /* NOTES
     *  time_t may be vulnerable to wrap-around in year 2034. 
     * There is no current standard 'C' replacement for <time.h> routines.
     * Various operating systems have their own thread-safe replacements.
     * Various operating systems handle wrap-around differently.
     */

    /* COMPATIBILITY, these 'C' routines are compatible with all
     * POSIX platforms and
     * WINDOWS platforms
     * There may be some thread-unsafe notices when compiled on some systems.
     */

    /*
     * Return Value -1 = parameter error
     * Return Value 0 = success
     *
     * @param tp pointer to a type struct tm, to receive broken-down time.
     * @param tt pointer to a time_t value, to receive seconds since epoc UTC.
     */

    static int getSystemTimeUTC(struct tm * tmUTC)
    {
        if (!tmUTC)
        {
            return -1;
        }

        time_t sys_tt;
        time(& sys_tt);

        memcpy((void *) tmUTC, (void *) gmtime(& sys_tt), sizeof(struct tm));

        return 0;
    }

    static int getSystemTimeLocal(struct tm * tmLocal)
    {
        if (!tmLocal)
        {
            return -1;
        }

        time_t sys_tt;
        time(& sys_tt);

        memcpy((void *) tmLocal, (void *) localtime(& sys_tt), sizeof(struct tm));

        return 0;
    }

    static char * getLocalTimeZone(struct tm * tmLocal, struct tm * tmUTC)
    {
        int hour = 0;
        int minute = 0;

        hour += tmLocal->tm_hour - tmUTC->tm_hour;
        hour += tmLocal->tm_mday > tmUTC->tm_mday ? 24
            : tmLocal->tm_mday < tmUTC->tm_mday ? -24 : 0;

        minute = tmLocal->tm_min - tmUTC->tm_min;
        
        if (hour + minute)
        {
            if ((hour > 0 && minute < 0)
                || (hour < 0 && minute > 0))
            {
                hour += hour > 0 ? -1 : 1;
                minute = abs(minute) % 60;
            }

            char * timezone = new char[6];

            sprintf((hour > 0 && (*timezone = '+')) || (*timezone = '-') ? timezone + 1 : timezone,
                "%02d:%02d", abs(hour), minute);

            return timezone;
        }

        return "Z";
    }

};