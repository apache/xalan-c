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
#if !defined(LOGGER_HEADER_GUARD_1357924680)
#define LOGGER_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iosfwd>
//#include <ios>
#endif



#include <xalanc/XalanDOM/XalanDOMString.hpp>



class Logger
{
public:

#if defined(XALAN_NO_STD_NAMESPACE)
    typedef ostream         StreamType;
#else
    typedef std::ostream    StreamType;
#endif

    Logger(StreamType&  stream);

    enum eLogType
    {
        eMessage = 0,
        eWarning = 1,
        eError = 2
    };

    StreamType&
    message();

    StreamType&
    warning();

    StreamType&
    error();

    StreamType&
    log(eLogType    logType);

protected:

    const char*     logText[3];

    StreamType&        m_stream;
};



#endif //LOGGER_HEADER_GUARD_1357924680

