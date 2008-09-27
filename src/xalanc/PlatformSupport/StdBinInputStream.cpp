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
#include "StdBinInputStream.hpp"



#include <cassert>

#if !defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream>
#endif



XALAN_CPP_NAMESPACE_BEGIN



StdBinInputStream::StdBinInputStream(StreamType&    theStream) :
    ParentType(),
    m_stream(theStream)
{
}



StdBinInputStream::~StdBinInputStream()
{
}



XalanFilePos
StdBinInputStream::curPos() const
{
    return m_stream.tellg();
}



XalanSize_t
StdBinInputStream::readBytes(
            XMLByte* const      toFill,
            const XalanSize_t   maxToRead)
{
    assert(sizeof(XMLByte) == sizeof(char));

    if (!m_stream)
    {
        return 0;
    }
    else
    {
        XALAN_USING_STD(streamsize)

        assert(static_cast<XALAN_UINT64>(static_cast<streamsize>(maxToRead)) == maxToRead);

#if !defined(XALAN_CLASSIC_IOSTREAMS)
        m_stream.read(
            reinterpret_cast<char*>(toFill),
            static_cast<streamsize>(maxToRead));
#else
        m_stream.read(toFill, maxToRead);
#endif

        return m_stream.gcount();
    }
}



const XalanDOMChar*
StdBinInputStream::getContentType() const
{
    return 0;
}



XALAN_CPP_NAMESPACE_END
