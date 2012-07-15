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

#include "XalanDatesAndTimes.hpp"
#include "XalanDateTime.hpp"
#include "XalanDuration.hpp"

XALAN_CPP_NAMESPACE_USE

char * XalanDatesAndTimes::add(char * dateTime, char * duration)
{
    XalanDateTime * dtTm = XalanDateTime::fromString(dateTime);
    XalanDuration * dur = XalanDuration::fromString(duration);

    dtTm->addDuration(dur);

    char * result = dtTm->toString();
    
    delete dtTm;
    delete dur;

    return result;
}