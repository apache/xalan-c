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
#if !defined(TIMER_HEADER_GUARD_1357924680)
#define TIMER_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <ctime>



class Timer
{
public:
    typedef XALAN_STD_QUALIFIER clock_t TimerType;

    Timer() :
        m_startTime(0),
        m_totalTime(0)
    {
    }

  void start()
    {
        m_startTime = XALAN_STD_QUALIFIER clock();

    }

    TimerType stop()
    {
        TimerType stopTime = XALAN_STD_QUALIFIER clock();

        m_totalTime = stopTime - m_startTime;

        return m_totalTime;
    }

    TimerType getElapsedTime()
    {
        return m_totalTime;
    }


private:

    TimerType m_startTime;
    TimerType m_totalTime;
    
};






#endif //TIMER_HEADER_GUARD_1357924680

