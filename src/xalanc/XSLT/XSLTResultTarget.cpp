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

// Base include file.  Must be first.
#include "XSLTResultTarget.hpp"



#include <cassert>

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <ostream>
#endif



XALAN_CPP_NAMESPACE_BEGIN



XSLTResultTarget::XSLTResultTarget(MemoryManager& theManager) :
    m_fileName(theManager),
    m_byteStream(0),
    m_encoding(theManager),
    m_characterStream(0),
    m_formatterListener(0),
    m_stream(0)
{
}



XSLTResultTarget::XSLTResultTarget(const XalanDOMString&    fileName,
                                   MemoryManager& theManager) :
    m_fileName(fileName, theManager),
    m_byteStream(0),
    m_encoding(theManager),
    m_characterStream(0),
    m_formatterListener(0),
    m_stream(0)
{
}



XSLTResultTarget::XSLTResultTarget(const XalanDOMChar*  fileName,
                                   MemoryManager& theManager) :
    m_fileName(fileName, theManager),
    m_byteStream(0),
    m_encoding(theManager),
    m_characterStream(0),
    m_formatterListener(0),
    m_stream(0)
{
}



XSLTResultTarget::XSLTResultTarget(const char*  fileName,
                                   MemoryManager& theManager) :
    m_fileName(fileName, theManager),
    m_byteStream(0),
    m_encoding(theManager),
    m_characterStream(0),
    m_formatterListener(0),
    m_stream(0)
{
}

XSLTResultTarget::XSLTResultTarget(const XSLTResultTarget& other,
                                   MemoryManager&   theManager) :
    m_fileName(other.m_fileName, theManager),
    m_byteStream(other.m_byteStream),
    m_encoding(other.m_encoding, theManager),
    m_characterStream(other.m_characterStream),
    m_formatterListener(other.m_formatterListener),
    m_stream(other.m_stream)
{
}


XSLTResultTarget::XSLTResultTarget(StreamType*  theStream,
                                   MemoryManager& theManager) :
    m_fileName(theManager),
    m_byteStream(theStream),
    m_encoding(theManager),
    m_characterStream(0),
    m_formatterListener(0),
    m_stream(0)
{
    assert(theStream != 0);
}



XSLTResultTarget::XSLTResultTarget(StreamType&  theStream,
                                   MemoryManager& theManager) :
    m_fileName(theManager),
    m_byteStream(&theStream),
    m_encoding(theManager),
    m_characterStream(0),
    m_formatterListener(0),
    m_stream(0)
{
}



XSLTResultTarget::XSLTResultTarget(Writer*  characterStream,
                                   MemoryManager& theManager) :
    m_fileName(theManager),
    m_byteStream(0),
    m_encoding(theManager),
    m_characterStream(characterStream),
    m_formatterListener(0),
    m_stream(0)
{
    assert(characterStream != 0);
}



XSLTResultTarget::XSLTResultTarget(FILE*    stream,
                                   MemoryManager& theManager) :
    m_fileName(theManager),
    m_byteStream(0),
    m_encoding(theManager),
    m_characterStream(0),
    m_formatterListener(0),
    m_stream(stream)
{
    assert(stream != 0);
}



XSLTResultTarget::XSLTResultTarget(FormatterListener&   flistener,
                                   MemoryManager& theManager) :
    m_fileName(theManager),
    m_byteStream(0),
    m_encoding(theManager),
    m_characterStream(0),
    m_formatterListener(&flistener),
    m_stream(0)
{
}



XSLTResultTarget::~XSLTResultTarget()
{
}



XALAN_CPP_NAMESPACE_END
