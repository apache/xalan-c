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
// Class header file
#include "FormatterStringLengthCounter.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FormatterStringLengthCounter::FormatterStringLengthCounter() :
    FormatterListener(OUTPUT_METHOD_NONE),
    m_count(0)
{
}



FormatterStringLengthCounter::~FormatterStringLengthCounter()
{
}



void
FormatterStringLengthCounter::setDocumentLocator(const Locator* const   /* locator */)
{
}



void
FormatterStringLengthCounter::startDocument()
{
}



void
FormatterStringLengthCounter::endDocument()
{
}



void
FormatterStringLengthCounter::startElement(
            const XMLCh* const  /* name */,
            AttributeListType&  /* attrs */)
{
}



void
FormatterStringLengthCounter::endElement(
            const   XMLCh* const    /* name */)
{
}



void
FormatterStringLengthCounter::characters(
            const XMLCh* const  /* chars */,
            const size_type     length)
{
    m_count += length;
}



void
FormatterStringLengthCounter::charactersRaw(
        const XMLCh* const  /* chars */,
        const size_type     /*length */)
{
}


void
FormatterStringLengthCounter::entityReference(const XMLCh* const    /* name */)
{
}



void
FormatterStringLengthCounter::ignorableWhitespace(
            const XMLCh* const  /* chars */,
            const size_type     /* length */)
{
}



void
FormatterStringLengthCounter::processingInstruction(
            const XMLCh* const  /* target */,
            const XMLCh* const  /* data */)
{
}



void
FormatterStringLengthCounter::resetDocument()
{
}



void
FormatterStringLengthCounter::comment(const XMLCh* const    /* data */)
{
}



void
FormatterStringLengthCounter::cdata(
            const XMLCh* const  /* ch */,
            const size_type     /* length */)
{
}



XALAN_CPP_NAMESPACE_END
