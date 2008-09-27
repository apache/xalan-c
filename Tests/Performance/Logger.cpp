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



// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include "Logger.hpp"


#include <ctime>

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#include <iomanip.h>
#else
#include <iostream>
#include <iomanip>
#endif


XALAN_USING_XALAN(XalanDOMString)




Logger::Logger(StreamType&  stream) :
    m_stream(stream)
{
    logText[eMessage] = "Message";
    logText[eWarning] = "Warning";
    logText[eError] = "Error";
}



Logger::StreamType&
Logger::message()
{
    return log(eMessage);
}



Logger::StreamType& 
Logger::warning()
{
    return log(eWarning);
}



Logger::StreamType& 
Logger::error()
{
    return log(eError);
}


Logger::StreamType& 
Logger::log(eLogType    logType)
{
#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::ctime;
    using std::time;
    using std::time_t;
#endif

    time_t theTime;
    time(&theTime);

    // Not thread safe
    char* const timeStr = ctime(&theTime);
    timeStr[24] = '\0';

    m_stream << timeStr
             << XALAN_STD_QUALIFIER setw(10)
             << logText[logType]
             << ": ";

    return m_stream;
}
