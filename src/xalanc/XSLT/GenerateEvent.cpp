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
#include "GenerateEvent.hpp"



XALAN_CPP_NAMESPACE_BEGIN



GenerateEvent::GenerateEvent(EventType  eventType, MemoryManager& theManager) :
    m_eventType(eventType),
    m_characters(theManager),
    m_start(0),
    m_length(0),
    m_name(theManager),
    m_data(theManager),
    m_pAtts(0)
{
}



GenerateEvent::GenerateEvent(
            EventType               eventType,
            MemoryManager&      theManager,
            const XalanDOMChar*     name,
            AttributeListType*      atts) :
    m_eventType(eventType),
    m_characters(theManager),
    m_start(0),
    m_length(0),
    m_name(name, theManager),
    m_data(theManager),
    m_pAtts(atts)
{
}



GenerateEvent::GenerateEvent(
            EventType                   eventType,
            MemoryManager&          theManager,
            const XalanDOMString&       name,
            const AttributeListType*    atts) :
    m_eventType(eventType),
    m_characters(theManager),
    m_start(0),
    m_length(0),
    m_name(name, theManager),
    m_data(theManager),
    m_pAtts(atts)
{
}


GenerateEvent::GenerateEvent(
            EventType                   eventType,
            MemoryManager&          theManager,
            const XalanDOMChar*         ch,
            XalanDOMString::size_type   start,
            XalanDOMString::size_type   length) :
    m_eventType(eventType),
    m_characters(ch + start, theManager, length),
    m_start(start),
    m_length(length),
    m_name(theManager),
    m_data(theManager),
    m_pAtts(0)
{
}



GenerateEvent::GenerateEvent(
            EventType               eventType,
            MemoryManager&      theManager,
            const XalanDOMChar*     name,
            const XalanDOMChar*     data) :
    m_eventType(eventType),
    m_characters(theManager),
    m_start(0),
    m_length(0),
    m_name(name, theManager),
    m_data(data, theManager),
    m_pAtts(0)
{
}



GenerateEvent::GenerateEvent(
            EventType               eventType,
            MemoryManager&      theManager,
            const XalanDOMChar*     data) :
    m_eventType(eventType),
    m_characters(theManager),
    m_start(0),
    m_length(0),
    m_name(theManager),
    m_data(data, theManager),
    m_pAtts(0)
{
}



XALAN_CPP_NAMESPACE_END
