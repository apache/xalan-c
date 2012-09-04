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

/**
 * This header file defines the C++ classes whose objects represent exceptions
 * that are thrown when any XalanCalendar object is invalid, more specificaly a 
 * XalanDateTime or a XalanDuration one.
 * It may occur when it is not correctly represented by a string or the
 * operation that are being applied put it in an invalid state.
 *
 * In accordance with the classes XalanCalendar, XalanDateTime and XalanDuration
 * defined in the model, the classes XalanInvalidCalendar, XalanInvalidDateTime,
 * and XalanInvalidDuration were defined, respectively.
 */

#ifndef _XALAN_DATETIME_EXCEPTIONS_H
#define _XALAN_DATETIME_EXCEPTIONS_H

#include "XalanDateTimeDefinitons.hpp"

#include <stdexcept>

XALAN_CPP_NAMESPACE_BEGIN

/**
 * Objects of this class are exceptions that are thrown when any XalanCalendar
 * object is invalid.
 */
class XalanInvalidCalendar
    : public std::invalid_argument
{

public:

    XalanInvalidCalendar()
        : invalid_argument("No information about the cause of this exception.")
    {
    }

    XalanInvalidCalendar(const char * message)
        : invalid_argument(message)
    {
    }

};

/**
 * Objects of this class are exceptions that are thrown when any XalanDateTime
 * object is invalid.
 */
class XalanInvalidDateTime
    : public XalanInvalidCalendar
{

public:

    XalanInvalidDateTime()
        : XalanInvalidCalendar()
    {
    }

    XalanInvalidDateTime(const char * message)
        : XalanInvalidCalendar(message)
    {
    }

};

/**
 * Objects of this class are exceptions that are thrown when any XalanDuration
 * object is invalid.
 */
class XalanInvalidDuration
    : public XalanInvalidCalendar
{

public:

    XalanInvalidDuration()
        : XalanInvalidCalendar()
    {
    }

    XalanInvalidDuration(const char * message)
        : XalanInvalidCalendar(message)
    {
    }

};

XALAN_CPP_NAMESPACE_END

#endif